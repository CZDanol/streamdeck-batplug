#include "button.h"

#include <QProcess>

Button::Button(const CtorData &d) : device(*d.device), action(d.action), context(d.context), payload(d.payload) {

}

void Button::onPressed() {
	QProcess *p = new QProcess(&device.plugin);
	QObject::connect(p, &QProcess::finished, p, &QProcess::deleteLater);
	QObject::connect(p, &QProcess::errorOccurred, p, &QProcess::deleteLater);

	// Set arguments
	QStringList args;
	const QJsonObject settings = payload["settings"].toObject();
	for(auto it = settings.begin(), end = settings.end(); it != end; it++)
		args << ("--" + it.key()) << it.value().toString();

	p->start("cmd/" + action, args);
}
