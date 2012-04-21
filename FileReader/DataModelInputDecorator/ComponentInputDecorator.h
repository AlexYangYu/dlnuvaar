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
// 作者: 杨雨, alex890714@gmail.com
// 文件: /FileReader/DataModelInputDecorater/ComponentInputDecorater.h
//       这个文件定义了一个负责解析XML文件并填充到Component数据类型的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef FILEREADER_DATAMODELINPUTDECORATOR_COMPONENTINPUTDECORATOR_H_
#define FILEREADER_DATAMODELINPUTDECORATOR_COMPONENTINPUTDECORATOR_H_

#include <memory>
#include <hash_map>

#include <osg/ref_ptr>
#include <osg/Array>

#include "InputDecorator.h"
#include "../../VAARDataModel/Component.h"

namespace vaar_file {

// 负责解析SWComponent节点的XML数据，并填充到Face对象
class ComponentInputDecorator : public InputDecorator{

public:
	// 构造函数和析构函数
	ComponentInputDecorator(
		//osg::ref_ptr<osg::Vec3Array>& vertices, 
		//osg::ref_ptr<osg::Vec3Array>& normals,
		//std::hash_map<std::string, vaar_data::Component*>* component_parent_map
		std::tr1::shared_ptr<std::hash_map<std::string, vaar_data::Component*>>& component_parent_map) {
		//vertices_ = vertices;
		//normals_ = normals;
		component_parent_map_ = component_parent_map;
	}
	ComponentInputDecorator() {}
	~ComponentInputDecorator() {}

	// 解析SWComponent节点的XML数据，并填充到Component对象。
	virtual void Parse(const char* id, xercesc::DOMElement* element);
	// 获取Face对象指针
	vaar_data::Component* GetComponent();
private:
	// 操作句柄
	std::tr1::shared_ptr<vaar_data::Component> component_;
	
	// 三角顶点列表句柄
	//osg::ref_ptr<osg::Vec3Array> vertices_;
	// 三角顶点法向量列表句柄
	//osg::ref_ptr<osg::Vec3Array> normals_;
	
	// ComponentID, EdgeID, VertexID, FaceID到父节点的映射关系
	std::tr1::shared_ptr<std::hash_map<std::string, vaar_data::Component*>> component_parent_map_;
	//std::hash_map<std::string, vaar_data::Component*>* component_parent_map_;

}; // class ComponentInputDecorator

} // namespace vaar_file

#endif