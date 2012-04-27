#ifndef KEYBOARD_EVENT_HANDLER_H_
#define KEYBOARD_EVENT_HANDLER_H_

#include <osg/ref_ptr>
#include <osg/Switch>

#include "IEventHandler.h"

class KeyBDownHandler : public IEventHandler {
public:
	KeyBDownHandler(osg::Switch *switcher) {
		_switch = switcher;
	}

	void Handle() {
		bool val = _switch->getValue(0);
		_switch->setValue(0, !val);
	}
private:
	osg::ref_ptr<osg::Switch> _switch;	
};

#endif