#include "plugin.h"

#include <QProcess>
#include <QDir>
#include <QCoreApplication>

bool Plugin::init(const ESDConfig &esdConfig) {
	deck.init(esdConfig.port, esdConfig.pluginUUID, esdConfig.registerEvent, esdConfig.info);
	if(!deck.connect())
		return false;

	connect(&deck, &QStreamDeckPlugin::keyDown, this, &Plugin::onKeyDown);
	connect(&deck, &QStreamDeckPlugin::willAppear, this, &Plugin::onWillAppear);

	return true;
}

void Plugin::launchScript(const QStreamDeckAction &a, const QString &scriptSuffix) {
	QProcess *p = new QProcess(this);

	// Find launchable
	{
		const QString base = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../cmd/" + a.action + scriptSuffix);
		QString program;

		static const QStringList suffixes{".exe", ".bat", ""};
		for(const QString &suffix: suffixes) {
			if(const QString f = base + suffix; QFile::exists(f)) {
				p->setProgram(f);
				break;
			}
		}

		if(p->program().isEmpty()) {
			qDebug() << "Could not found" << base;
			return;
		}
	}

	// Set arguments & environment
	{
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		QStringList args;

		const QJsonObject settings = a.payload["settings"].toObject();
		for(auto it = settings.begin(), end = settings.end(); it != end; it++) {
			const QString val = it.value().toString();
			args << ("--" + it.key()) << val;
			env.insert("BPS_" + it.key(), val);
		}

		p->setArguments(args);
		p->setProcessEnvironment(env);
	}

	QObject::connect(p, &QProcess::finished, p, &QProcess::deleteLater);
	QObject::connect(p, &QProcess::errorOccurred, p, &QProcess::deleteLater);
	QObject::connect(p, &QProcess::stateChanged, p, [p](QProcess::ProcessState s) {
		qDebug() << "state" << s;
	});

	QObject::connect(p, &QProcess::readyRead, p, [p, this, ctx = a.context, device = a.deviceId] {
		while(p->bytesAvailable()) {
			const QString ln = QString::fromUtf8(p->readLine()).trimmed();
			qDebug() << "read" << ln;

			const QString cmd = ln.section(' ', 0, 0);

			QString arg = ln.section(' ', 1);
			arg.replace("\\n", "\n");

			if(cmd == "setTitle")
				deck.setTitle(arg, ctx, kESDSDKTarget_HardwareAndSoftware);

			else if(cmd == "setState")
				deck.setState(arg.toInt(), ctx);

			else if(cmd == "switchProfile")
				deck.switchProfile(device, arg);

			else if(cmd == "showAlert")
				deck.showAlert(ctx);
		}
	});

	qDebug() << "run" << p->program() << p->arguments();
	p->start();
}

void Plugin::onKeyDown(const QStreamDeckAction &a) {
	launchScript(a, "");
}

void Plugin::onWillAppear(const QStreamDeckAction &a) {
	launchScript(a, "_willAppear");
}
