#include "KeyboardEventRouter.h"

bool KeyboardEventRouter::AddHandler(
	int key, KeyStatusType key_press_status, IEventHandler *handler
) {
	KeyHandlerMap *key_handler_map;
	if (key_press_status == KeyStatusType::KEY_DOWN) {
		key_handler_map = &_key_down_handler_map;
	} else if (key_press_status == KeyStatusType::KEY_UP) {
		key_handler_map = &_key_up_handler_map;
	} else {
		return false;
	}

	KeyHandlerMap::iterator it = key_handler_map->find(key);
	if (it == key_handler_map->end()) {
		(*key_handler_map)[key] = handler;
	} else {
		return false;
	}
	return true;
} // AddFunction


bool KeyboardEventRouter::handle(
	const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa
) {
	KeyHandlerMap::iterator it;
	int key = ea.getKey();
	switch(ea.getEventType()) {
		case osgGA::GUIEventAdapter::KEYDOWN: {
			it = _key_down_handler_map.find(ea.getKey());
			if (it != _key_down_handler_map.end()) {
				_key_down_handler_map[key]->Handle();
				return true;
			}
			break;
		}
		case osgGA::GUIEventAdapter::KEYUP: {
			it = _key_up_handler_map.find(ea.getKey());
			if (it != _key_up_handler_map.end()) {
				_key_up_handler_map[key]->Handle();
				return true;
			}
			break;
		}
		default:
			return false;
	}
	return false;
} // handle