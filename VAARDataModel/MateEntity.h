//////////////////////////////////////////////////////////////////////////
// 版权声明 (C) 2011  大连民族学院
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// 作者: Yang Yu, alex890714@gmail.com
// 文件: /VAARDataModel/MateEntity.h
//       这个文件声明了配合实体数据类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef VAARDATAMEDEL_MATEENTITY_H_
#define VAARDATAMEDEL_MATEENTITY_H_

#define MATEENTITY_PARAMS_LENGTH 8

#include <memory>
#include <string>

namespace vaar_data {

// 声明并引入Component类
class Component;

// 配合实体的类型
enum MateEntityType {
	POINT_MATEENTITY,
	LINE_MATEENTITY,
	PLANE_MATEENTITY,
	CYLINDER_MATEENTITY,
	CONE_MATEENTITY
}; // enum MateEntityType

// 配合实体
class MateEntity {

public:
	// 构造函数和析构函数
	MateEntity(){}
	MateEntity(
		const char* id,
		const char* component_name, 
		const double params[MATEENTITY_PARAMS_LENGTH] ,
		const MateEntityType type) {

		SetComponentName(component_name);
		SetParams(params);
		SetMateEntityType(type);
	}
	~MateEntity(){}

	// getter and setter
	void SetID(const char* id) {
		id_ = std::tr1::shared_ptr<std::string>(new std::string(id));
	}
	char* GetID() {
		return (char*)id_.get()->c_str();
	}

	void SetComponentName(const char* name) {
		component_name_ = std::tr1::shared_ptr<std::string>(new std::string(name));
	}
	char* GetComponentName() {
		return (char*)component_name_.get()->c_str();
	}

	// 设置参数
	void SetParams(const double params[MATEENTITY_PARAMS_LENGTH]) {
		for (int i = 0; i < MATEENTITY_PARAMS_LENGTH; ++i)
			params_[i] = params[i];
	}
	// 返回一个长度为8的参数数组，size存长度。
	double* GetParams(int &size) {
		size = MATEENTITY_PARAMS_LENGTH;
		return params_;
	}

	void SetMateEntityType(const MateEntityType type) {
		type_ = type;
	}
	MateEntityType GetMateEntityType() {
		return type_;
	}

	void SetComponent(Component* component) {
		component_  = std::tr1::shared_ptr<Component>(component);
	}
	Component* GetComponent() {
		return component_.get();
	}

private:
	// ID
	std::tr1::shared_ptr<std::string> id_;
	// 实体所在组件的名称
	std::tr1::shared_ptr<std::string> component_name_;
	// 组件句柄
	std::tr1::shared_ptr<Component> component_;
	// 参数
	double params_[MATEENTITY_PARAMS_LENGTH];
	// 类型
	MateEntityType type_;
}; // class MateEntitu

} // namespace vaar_data
#endif