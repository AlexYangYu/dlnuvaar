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
// 文件: /FileReader/DataModelInputDecorater/DataModelInputDecorater.cpp
//       这个文件实现了一个负责解析XML文件并填充到DataModel数据类型的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMNodeList.hpp>

#include "DataModelInputDecorator.h"
#include "ComponentInputDecorator.h"
#include "MateInputDecorator.h"

namespace vaar_file {

// 传入的DOM模型为
// <SWComponent xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"> 
// 	<Name>tutor</Name>
// 	<SubComponents />
//	<BoundingBox />
//  <IsRoot>true</IsRoot> 
// 	<Bodies/>
// 	<Mates />
// </SWComponent>
void DataModelInputDecorator::Parse(const char* id, xercesc::DOMElement* element) {
	if (NULL == element)
		return;

	char index[255];
	xercesc::DOMElement* current_element = NULL;
	data_model_ = std::shared_ptr<vaar_data::DataModel>(new vaar_data::DataModel());
	
	std::hash_map<std::string, vaar_data::Component*>* component_parent_map = 
		new std::hash_map<std::string, vaar_data::Component*>();
	data_model_->SetRelation(component_parent_map);

	// 解析Component树
	ComponentInputDecorator* com_input = new ComponentInputDecorator(data_model_->GetRefRelation());
	com_input->Parse("", element);
	data_model_->SetRoot(com_input->GetComponent());

	// 解析Mates
	std::vector<vaar_data::Mate*>* mates = new std::vector<vaar_data::Mate*>();
	xercesc::DOMNodeList* list = element->getElementsByTagName(xercesc::XMLString::transcode("SWMate"));
	if (NULL != list) {

		MateInputDecorator* mate_input = NULL;
		vaar_data::Mate* mate = NULL;
		
		for (XMLSize_t i = 0; i < list->getLength(); ++i) {
			current_element = dynamic_cast<xercesc::DOMElement*>(list->item(i));

			// 分配MateID
			std::string mate_id("mate");
			_itoa_s(i, index, 10);
			mate_id += index;

			// 解析SWMate
			mate_input = new MateInputDecorator();
			mate_input->Parse(mate_id.c_str(), current_element);
			mate = mate_input->GetMate();

			if (NULL != mate)
				mates->push_back(mate);
		}
	}
	data_model_->SetMates(mates);
}

// 获取DataModel对象指针
vaar_data::DataModel* DataModelInputDecorator::GetDataModel() {
	return data_model_.get();
}

} // namespacae vaar_file