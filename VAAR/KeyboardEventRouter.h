#ifndef KEYBOARD_EVENT_ROUTER_H_
#define KEYBOARD_EVENT_ROUTER_H_

#include <map>
#include <osgGA/GUIEventHandler>

#include "IEventHandler.h"

class KeyboardEventRouter : public osgGA::GUIEventHandler {
public:
	KeyboardEventRouter() {}
	enum KeyStatusType {
		KEY_UP, 
		KEY_DOWN 
	};

	// Storage for list of registered key, function to execute and 
	// current state of key.
	typedef std::map<int, IEventHandler*> KeyHandlerMap;

	// Overloaded version allows users to specify if the function should 
	// be associated with KEY_UP or KEY_DOWN event.
	bool AddHandler(int key, KeyStatusType key_press_status, 
		IEventHandler *handler);

	// The handle method checks the current key down event against 
	// list of registered key/key status entries. If a match is found 
	// and it's a new event (key was not already down) corresponding 
	// function is invoked.
	virtual bool handle(const osgGA::GUIEventAdapter& ea,
		osgGA::GUIActionAdapter&);

	// Overloaded accept method for dealing with event handler visitors
	virtual void accept(osgGA::GUIEventHandlerVisitor& v) {
		v.visit(*this); 
	};

protected:

	// Storage for registered 'key down' methods and key status
	KeyHandlerMap _key_down_handler_map;

	// Storage for registered 'key up' methods and key status
	KeyHandlerMap _key_up_handler_map;
}; // KeyboardEventHandler

#endif