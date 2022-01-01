#pragma once

#include <QString>
#include <QJsonArray>

#include <qtstreamdeck/qstreamdeckaction.h>

class Plugin;
class Button;

class Device {

public:
	Device(Plugin &plugin, const QString &deviceID);
	~Device();

public:
	void onAppear(const QStreamDeckAction &action);
	void onDisappear(const QStreamDeckAction &action);
	void onSendToPlugin(const QStreamDeckAction &action);

public:
	Plugin &plugin;
	const QString deviceID;

public:
	QHash<QString, Button*> buttons;

};

