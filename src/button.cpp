#include "button.h"

#include <QProcess>
#include <QCoreApplication>
#include <QDir>

Button::Button(const CtorData &d) : device(*d.device), action(d.action), context(d.context), payload(d.payload) {

}

void Button::onPressed() {
	QProcess *p = new QProcess(&device.plugin);
	QObject::connect(p, &QProcess::finished, p, &QProcess::deleteLater);
	QObject::connect(p, &QProcess::errorOccurred, p, &QProcess::deleteLater);

	QObject::connect(p, &QProcess::readyRead, p, [p, plugin = &device.plugin, ctx = context, device = device.deviceID] {
		const QString ln = QString::fromUtf8(p->readLine()).trimmed();
		const QString cmd = ln.section(' ', 0, 0);
		const QString arg = ln.section(' ', 1);

		if(cmd == "setTitle")
			plugin->deck.setTitle(arg, ctx, kESDSDKTarget_HardwareAndSoftware);

		else if(cmd == "setState")
			plugin->deck.setState(arg.toInt(), ctx);

		else if(cmd == "switchProfile")
			plugin->deck.switchProfile(device, arg);

		else if(cmd == "showAlert")
			plugin->deck.showAlert(ctx);
	});

	// Set arguments
	{
		QStringList args;
		const QJsonObject settings = payload["settings"].toObject();
		for(auto it = settings.begin(), end = settings.end(); it != end; it++)
			args << ("--" + it.key()) << it.value().toString();

		p->setArguments(args);
	}

	p->setProgram(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("../cmd/" + action));

	p->start();
}
