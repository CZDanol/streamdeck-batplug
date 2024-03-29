#pragma once

#include <QString>
#include <QScopedPointer>
#include <QSharedPointer>

#include <qtstreamdeck/qstreamdeckplugin.h>

struct ESDConfig {
	int port;
	QString pluginUUID;
	QString registerEvent;
	QString info;
};

class Plugin : public QObject {
	Q_OBJECT

public:
	bool init(const ESDConfig &esdConfig);
	void launchScript(const QStreamDeckAction &a, const QString &scriptSuffix);

public slots:
	void onKeyDown(const QStreamDeckAction &a);
	void onWillAppear(const QStreamDeckAction &a);

public:
	QStreamDeckPlugin deck;

};