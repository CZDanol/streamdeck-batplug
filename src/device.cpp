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