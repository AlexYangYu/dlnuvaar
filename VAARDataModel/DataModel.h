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
// 文件: /VAARDataModel/Component.h
//       这个文件声明了数据模型类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef VAARDATAMODEL_DATAMODEL_H_
#define VAARDATAMODEL_DATAMODEL_H_

#include <memory>
#include <vector>
#include <hash_map>

#include <osg\Array>
#include <osg\ref_ptr>

namespace vaar_data {

//声明并引入
class Component;
class Mate;

// 数据模型类
// 封装了装配数据模型，显示数据模型，碰撞检测数据模型
// 以及模型之间的关系
class DataModel {

public:
	// 构造函数与虚构函数
	DataModel(){}
	DataModel(
		      Component* root,
			  std::vector<Mate*>* mates,
			  //osg::Vec3Array* triangles,
			  //osg::Vec3Array* normals,
			  std::hash_map<std::string, Component*>* relation_map) {
		SetRoot(root);
		SetMates(mates);
		//SetTriangles(triangles);
		//SetNormals(normals);
		SetRelation(relation_map);
	}
	~DataModel(){
		/*
		if (NULL != component_parent_map_)
			delete component_parent_map_;
		*/
	}

	// getter and setter
	// 设置装配模型树
	void SetRoot(Component* root) {
		root_ = std::tr1::shared_ptr<Component>(root);
	}
	// 获取装配模型树根节点
	Component* GetRoot() {
		return root_.get();
	}

	void SetMates(std::vector<Mate*>* mates) {
		mates_ = std::tr1::shared_ptr<std::vector<Mate*>>(mates);
	}
	std::vector<Mate*>* GetMates() {
		mates_.get();
	}
	/*
	void SetTriangles(osg::Vec3Array* triangles) {
		triangle_vertices_ = osg::ref_ptr<osg::Vec3Array>(triangles);
	}
	osg::Vec3Array* GetTriangles() {
		return triangle_vertices_.get();
	}

	void SetNormals(osg::Vec3Array* normals) {
		triangle_normals_ = osg::ref_ptr<osg::Vec3Array>(normals);
	}
	osg::Vec3Array* GetNormals() {
		return triangle_normals_.get();
	}
	*/

	void SetRelation(std::hash_map<std::string, Component*>* relation_map) {
		//component_parent_map_ = new std::hash_map<std::string, Component*>(*relation_map);
		component_parent_map_ = std::tr1::shared_ptr<std::hash_map<std::string, Component*>>(relation_map);
	}
	std::hash_map<std::string, Component*>* GetRelation() {
		return component_parent_map_.get();
		//return component_parent_map_;
	}
	
	std::tr1::shared_ptr<std::hash_map<std::string, Component*>> GetRefRelation() {
		return component_parent_map_;
	}
	
private:
	// 装配数据模型的根节点（树形拓扑结构）
	std::tr1::shared_ptr<Component> root_;
	// 配合列表
	std::tr1::shared_ptr<std::vector<Mate*>> mates_;
	
	// 三角面片顶点列表
	//osg::ref_ptr<osg::Vec3Array> triangle_vertices_;
	// 三角面片顶点法向量列表
	//osg::ref_ptr<osg::Vec3Array> triangle_normals_;
	
	// ComponentID, EdgeID, VertexID, FaceID到父节点的映射关系
	std::tr1::shared_ptr<std::hash_map<std::string, Component*>> component_parent_map_;
	//std::hash_map<std::string, Component*>* component_parent_map_;
	
	// 其它映射关系

}; // class DataModel

} // namespace vaar_data
#endif