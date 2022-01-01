#include "device.h"

#include "plugin.h"

#include "button.h"

Button *createButton(Device *dev, const Button::CtorData &d) {
	Button *btn = new Button(d);
	dev->buttons[d.context] = btn;

	return btn;
}

Device::Device(Plugin &plugin, const QString &deviceID) : plugin(plugin), deviceID(deviceID) {

}

Device::~Device() {
	qDeleteAll(buttons);
}

void Device::onAppear(const QStreamDeckAction &action) {
	createButton(this, Button::CtorData{this, action.action, action.context, action.payload});
}

void Device::onDisappear(const QStreamDeckAction &action) {
	delete buttons.take(action.context);
}

void Device::onSendToPlugin(const QStreamDeckAction &action) {
	// Basically recreate the button
	Button *btn = buttons.take(action.context);
	if(!btn)
		return;

	Button::CtorData d{this, action.action, action.context, btn->payload};
	QJsonObject s = d.payload["settings"].toObject();

	for(auto it = action.payload.begin(), end = action.payload.end(); it != end; it++)
		s[it.key()] = it.value();

	d.payload["settings"] = s;
	createButton(this, d);

	delete btn;
}