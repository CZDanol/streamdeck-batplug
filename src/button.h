#pragma once

#include <QString>
#include <QJsonObject>

#include "device.h"
#include "plugin.h"

class Button {

public:
	struct CtorData {
		Device *device;
		QString action;
		QString context;
		QJsonObject payload;
	};

public:
	Button(const CtorData &d);

public:
	void onPressed();

public:
	Device &device;
	const QString action;
	const QString context;
	const QJsonObject payload;

};

