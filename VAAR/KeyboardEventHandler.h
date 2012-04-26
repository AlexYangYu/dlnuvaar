#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <map>
#include <osgGA/GUIEventHandler>


class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:
	KeyboardEventHandler() {}
	typedef void (*FunctionType) ();
	enum KeyStatusType {
		KEY_UP, 
		KEY_DOWN 
	};

	// A struct for storing current status of each key and 
	// function to execute. Keep track of key's state to avoid
	// redundant calls. (If the key is already down, don't call the
	// key down method again.)
	struct FunctionStatusType {
		FunctionStatusType() {}
		FunctionStatusType(KeyStatusType key_status_type, FunctionType func) {
			_key_state = key_status_type; 
			_key_func = func;
		}
		FunctionType _key_func;
		KeyStatusType _key_state;
	};

	// Storage for list of registered key, function to execute and 
	// current state of key.
	typedef std::map<int, FunctionStatusType> KeyFuncMap;

	// Overloaded version allows users to specify if the function should 
	// be associated with KEY_UP or KEY_DOWN event.
	bool AddFunction(int key, KeyStatusType key_press_status, 
		FunctionType func);

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
	KeyFuncMap _key_down_func_map;

	// Storage for registered 'key up' methods and key status
	KeyFuncMap _key_up_func_map;
}; // KeyboardEventHandler

#endif