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
// 文件: /FileReader/DataModelInputDecorater/ComponentInputDecorater.cpp
//       这个文件实现了一个负责解析XML文件并填充到Component数据类型的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#include <cstdlib>

#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

#include "ComponentInputDecorator.h"
#include "FaceInputDecorator.h"
#include "EdgeInputDecorator.h"
#include "VertexInputDecorator.h"
#include "../../VAARDataModel/Component.h"

namespace vaar_file {

// 传入的DOM模型为
// <SWComponent> 
// 	<Name>Tutor2-1</Name> 
// 	<SubComponents/> 
// 	<IsRoot>false</IsRoot> 
// 	<BoundingBox>
// 	<Bodies>
// 		<SWBody> 
// 			<Name>切除-拉伸1</Name>
// 			<Faces>
// 			<Edges>
// 			<Vertices>
// 		</SWBody> 
// 	</Bodies> 
//  <Mates />
// </SWComponent>
void ComponentInputDecorator::Parse(const char* id, xercesc::DOMElement* element) {
	//if (NULL == element || !vertices_.valid() || !normals_.valid() || NULL == component_parent_map_.get())
	//	return;

	if (NULL == id || NULL == element || NULL == component_parent_map_)
		return;

	char index[255];
	xercesc::DOMElement* current_element =NULL;
	xercesc::DOMNodeList* list = NULL;
	component_ = std::tr1::shared_ptr<vaar_data::Component>(new vaar_data::Component);

	// 解析Name
	std::string component_name;
	element = element->getFirstElementChild();

	if (NULL != element &&
		0 == strcmp("Name", xercesc::XMLString::transcode(element->getTagName()))) {
		component_name = xercesc::XMLString::transcode(element->getTextContent());

		component_->SetName(component_name.c_str());
	}

	// 生成ID
	std::string component_id(id);
	component_id += component_name;
	component_->SetID(component_id.c_str());

	// 解析SubComponents
	std::vector<vaar_data::Component*>* sub_components = new std::vector<vaar_data::Component*>();
	element = element->getNextElementSibling();
	if (NULL != element &&
		0 == strcmp("SubComponents", xercesc::XMLString::transcode(element->getTagName()))) {

		ComponentInputDecorator* component_input_decorator = NULL;
		int sub_component_index = 0;
		vaar_data::Component* sub_component = NULL;

		current_element = element->getFirstElementChild();
		do {
			if (NULL == current_element)
				break;

			// 分配SubComponent的ID
			std::string sub_component_id = component_id;
			
			_itoa_s(sub_component_index, index, 10);
			sub_component_id += "_sub";
			sub_component_id += index;
			++sub_component_index;

			//component_input_decorator = new ComponentInputDecorator(vertices_, normals_, component_parent_map_);
			component_input_decorator = new ComponentInputDecorator(component_parent_map_);
			component_input_decorator->Parse(sub_component_id.c_str(), current_element);
			sub_component = component_input_decorator->GetComponent();

			if (NULL != sub_component) {
				sub_components->push_back(sub_component);
				// 建立SubComponentID到Component的映射关系
				component_parent_map_->insert(
					std::pair<std::string, vaar_data::Component*>(sub_component_id, component_.get())
				);
			}
		} while (
			current_element != element->getLastElementChild(),
			current_element = current_element->getNextElementSibling()
		);
	}
	component_->SetSubComponents(sub_components);

	// 解析Faces
	std::vector<vaar_data::Face*>* faces = new std::vector<vaar_data::Face*>();
	element = element->getNextElementSibling();
	element = element->getNextElementSibling();
	element = element->getNextElementSibling();

	if (NULL != element &&
		0 == strcmp("Bodies", xercesc::XMLString::transcode(element->getTagName()))) {
		
		FaceInputDecorator* face_input_decorator = NULL;
		vaar_data::Face* face =NULL;

		list = element->getElementsByTagName(xercesc::XMLString::transcode("SWFace"));
		if (NULL != list) {
			for (XMLSize_t i = 0; i < list->getLength(); ++i) {
				current_element = dynamic_cast<xercesc::DOMElement*>(list->item(i));

				// 分配Face的ID
				std::string face_id = component_id;

				_itoa_s(i, index, 10);
				face_id += "_face";
				face_id += index;

				//face_input_decorator = new FaceInputDecorator();
				face_input_decorator = new FaceInputDecorator(
					component_->GetRefPtrTriangles(), 
					component_->GetRefPtrNormals()
				);
				face_input_decorator->Parse(face_id.c_str(), current_element);
				face = face_input_decorator->GetFace();

				if (NULL != face) {
					faces->push_back(face);
					// 建立SubFaceID到Component的映射关系
					component_parent_map_->insert(
						std::pair<std::string, vaar_data::Component*>(face_id, component_.get())
					);
				}
			}
		}
	}
	component_->SetFaces(faces);

	// 解析Edges
	std::vector<vaar_data::Edge*>* edges = new std::vector<vaar_data::Edge*>();
	EdgeInputDecorator* edge_input_decorator = NULL;
	vaar_data::Edge* edge = NULL;

	list = element->getElementsByTagName(xercesc::XMLString::transcode("SWEdge"));
	if (NULL != list) {
		for (XMLSize_t i = 0; i < list->getLength(); ++i) {
			current_element = dynamic_cast<xercesc::DOMElement*>(list->item(i));

			// 分配Edge的ID
			std::string edge_id = component_id;
			_itoa_s(i, index, 10);
			edge_id += "_edge";
			edge_id += index;

			edge_input_decorator = new EdgeInputDecorator();
			edge_input_decorator->Parse(edge_id.c_str(), current_element);
			edge = edge_input_decorator->GetEdge();

			if (NULL != edge) {
				edges->push_back(edge);
				// 建立SubEdgeID到Component的映射关系
				component_parent_map_->insert(
					std::pair<std::string, vaar_data::Component*>(edge_id, component_.get())
				);
			}
		}
	}
	component_->SetEdges(edges);

	// 解析Vertices
	std::vector<vaar_data::Vertex*>* vertices = new std::vector<vaar_data::Vertex*>();
	VertexInputDecorator* vertex_input_decorator = NULL;
	vaar_data::Vertex* vertex = NULL;

	list = element->getElementsByTagName(xercesc::XMLString::transcode("SWVertex"));
	if (NULL != list) {
		for (XMLSize_t i = 0; i < list->getLength(); ++i) {
			current_element = dynamic_cast<xercesc::DOMElement*>(list->item(i));

			// 分配Vertex的ID
			std::string vertex_id = component_id;
			_itoa_s(i, index, 10);
			vertex_id += "_vertex";
			vertex_id += index;

			vertex_input_decorator = new VertexInputDecorator();
			vertex_input_decorator->Parse(vertex_id.c_str(), current_element);
			vertex = vertex_input_decorator->GetVertex();

			if (NULL != vertex) {
				vertices->push_back(vertex);
				// 建立SubVertexID到Component的映射关系
				component_parent_map_->insert(
					std::pair<std::string, vaar_data::Component*>(vertex_id, component_.get())
				);
			}
		}
	}
	component_->SetVertices(vertices);

}

// 返回
vaar_data::Component* ComponentInputDecorator::GetComponent() {
	return component_.get();
}

} // namespace vaar_file