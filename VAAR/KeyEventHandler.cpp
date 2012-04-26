#include "KeyboardEventHandler.h"

bool KeyboardEventHandler::AddFunction(
	int key, KeyStatusType key_press_status, FunctionType func
) {
	KeyFuncMap *key_func_map;
	if (key_press_status == KeyStatusType::KEY_DOWN) {
		key_func_map = &_key_down_func_map;
	} else if (key_press_status == KeyStatusType::KEY_UP) {
		key_func_map = &_key_up_func_map;
	} else {
		return false;
	}

	KeyFuncMap::iterator it = key_func_map->find(key);
	if (it == key_func_map->end()) {
		(*key_func_map)[key] = FunctionStatusType(key_press_status, func);
		return true;
	} else {
		return false;
	}
	return true;
} // AddFunction


bool KeyboardEventHandler::handle(
	const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa
) {
	KeyFuncMap::iterator it;
	int key = ea.getKey();
	switch(ea.getEventType()) {
		case osgGA::GUIEventAdapter::KEYDOWN: {
			it = _key_down_func_map.find(ea.getKey());
			if (it != _key_down_func_map.end()) {
				_key_down_func_map[key]._key_func();
				return true;
			}
			break;
		}
		case osgGA::GUIEventAdapter::KEYUP: {
			it = _key_up_func_map.find(ea.getKey());
			if (it != _key_up_func_map.end()) {
				_key_up_func_map[key]._key_func();
				return true;
			}
			break;
		}
		default:
			return false;
	}
	return false;
} // handle