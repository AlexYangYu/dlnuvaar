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
// 文件: /FileReader/DataModelInputDecorater/FaceInputDecorater.cpp
//       这个文件实现了一个负责解析XML文件并填充到Face数据类型的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "../StrX.h"

#include <xercesc/util/XMLString.hpp>

#include "FaceInputDecorator.h"
#include "SurfaceInputDecorator.h"

namespace vaar_file {

// 传入的DOM模型为
// <SWFace> 
// 	   <ID>0</ID>
// 	   <BoundingBox/>
// 	   <TessTriangles/>
// 	   <TessNormals/>
// 	   <Surface/>
// </SWFace>
void FaceInputDecorator::Parse(const char* id, xercesc::DOMElement* element) {
	if (NULL == element || !vertices_.valid() || !normals_.valid())
		return;

	int vertices_length = vertices_->size();
	int normals_length = normals_->size();
	
	xercesc::DOMElement* current_element = NULL;
	SurfaceInputDecorator* input = new SurfaceInputDecorator();
	vaar_data::Surface* surface = NULL;

	std::vector<int>* triangles_index = new std::vector<int>();
	std::vector<int>* normals_index = new std::vector<int>();

	float current_value[3];
	int current_value_index = 0;

	// 解析TessTriangles
	element = element->getFirstElementChild();
	element = element->getNextElementSibling();
	element = element->getNextElementSibling();
	
	if (NULL != element &&
		0 == strcmp("TessTriangles", xercesc::XMLString::transcode(element->getTagName()))) {
		
		current_element = element->getFirstElementChild();
		do {
			sscanf_s(
				xercesc::XMLString::transcode(current_element->getTextContent()),
				"%f",
				&current_value[current_value_index++]
			);

			if (3 == current_value_index) {
				// 添加到三角面片顶点列表 
				vertices_->push_back(osg::Vec3(current_value[0], current_value[1], current_value[2]));
				// 添加三角面片的索引到三角面片索引列表
				triangles_index->push_back(vertices_length++);
				
				current_value_index %= 3;
			}
		} while (
			current_element != element->getLastElementChild(),
			current_element = current_element->getNextElementSibling()
		);
	}

	// 解析TessNormals
	element = element->getNextElementSibling();
	current_value_index = 0;

	if (NULL != element &&
		0 == strcmp("TessNormals", xercesc::XMLString::transcode(element->getTagName()))) {

			current_element = element->getFirstElementChild();
			do {
				sscanf_s(
					xercesc::XMLString::transcode(current_element->getTextContent()),
					"%f",
					&current_value[current_value_index++]
				);

				if (3 == current_value_index) {
					// 添加到三角面片顶点列表 
					normals_->push_back(osg::Vec3(current_value[0], current_value[1], current_value[2]));
					// 添加三角面片的索引到三角面片索引列表
					normals_index->push_back(normals_length++);

					current_value_index %= 3;
				}
			} while (
				current_element != element->getLastElementChild(),
				current_element = current_element->getNextElementSibling()
			);
	}

	// 解析Surface
	element = element->getNextElementSibling();
	if (NULL != element &&
		0 == strcmp("Surface", xercesc::XMLString::transcode(element->getTagName()))) {
		
		// 分配ID
		std::string surface_id(id);
		surface_id += "_curve";

		input->Parse(surface_id.c_str(), element);
		surface = input->GetSurface();
	}

	// 创建并填充
	face_ = std::tr1::shared_ptr<vaar_data::Face>(new vaar_data::Face(id, surface, triangles_index, normals_index));
}

// 获取Face对象指针
vaar_data::Face* FaceInputDecorator::GetFace() {
	return face_.get();
}

} // namespace vaar_file
