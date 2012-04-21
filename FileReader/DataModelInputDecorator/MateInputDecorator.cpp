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
// 文件: /FileReader/DataModelInputDecorater/MateEntityInputDecorater.cpp
//       这个文件实现了一个负责解析XML文件并填充到MateEntity数据类型的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include "MateInputDecorator.h"
#include "MateEntityInputDecorator.h"

namespace vaar_file {

// 传入的DOM模型为
// <SWMate>
// 	<MateType>COINCIDENT</MateType> 
// 	<MaxVariation>0</MaxVariation> 
// 	<MinVariation>0</MinVariation> 
// 	<Flipped>false</Flipped>
// 	<MateEntities/>
// </SWMate>
void MateInputDecorator::Parse(const char* id, xercesc::DOMElement* element) {
	if (NULL == id || NULL == element)
		return;

	char index[255];
	mate_ = std::tr1::shared_ptr<vaar_data::Mate>(new vaar_data::Mate());

	// ID
	mate_->SetID(id);

	// 解析MateType
	element = element->getFirstElementChild();
	if (NULL != element && 
		0 == strcmp("MateType", xercesc::XMLString::transcode(element->getTagName()))) {
		mate_->SetMateType(GetMateType(xercesc::XMLString::transcode(element->getTextContent())));
	}

	// 解析MaxVariation
	double max_variation = 0.0;
	element = element->getNextElementSibling();

	if (NULL != element && 
		0 == strcmp("MaxVariation", xercesc::XMLString::transcode(element->getTagName()))) {
	
		sscanf_s(
			xercesc::XMLString::transcode(element->getTextContent()),
			"%lf",
			&max_variation
		);
		mate_->SetMaxVariation(max_variation);
	}


	// 解析MinVariation
	double min_variation = 0.0;
	element = element->getNextElementSibling();

	if (NULL != element && 
		0 == strcmp("MinVariation", xercesc::XMLString::transcode(element->getTagName()))) {

		sscanf_s(
			xercesc::XMLString::transcode(element->getTextContent()),
			"%lf",
			&min_variation
			);
		mate_->SetMinVariation(min_variation);
	}

	// 解析Flipped
	bool flipped = false;
	element = element->getNextElementSibling();

	if (NULL != element && 
		0 == strcmp("Flipped", xercesc::XMLString::transcode(element->getTagName()))) {

		if (0 == strcmp("false", xercesc::XMLString::transcode(element->getTextContent())))
			flipped = false;
		else
			flipped = true;
	}
	mate_->SetFlipped(flipped);

	// 解析MateEntities
	std::vector<vaar_data::MateEntity*>* mate_entities = new std::vector<vaar_data::MateEntity*>();
	element = element->getNextElementSibling();

	if (NULL != element && 
		0 == strcmp("MateEntities", xercesc::XMLString::transcode(element->getTagName()))) {

		vaar_data::MateEntity* mate_entity = NULL;
		MateEntityInputDecorator* mate_entity_input_decorator = NULL;
		xercesc::DOMElement* current_element = NULL;
		xercesc::DOMNodeList* list = 
			element->getElementsByTagName(xercesc::XMLString::transcode("SWMateEntity"));

		if (NULL != list) {
			for (XMLSize_t i = 0; i < list->getLength(); ++i) {
				current_element = dynamic_cast<xercesc::DOMElement*>(list->item(i));

				// 分配SubMateEntity的ID
				std::string sub_mate_entity_id(id);
				_itoa_s(i, index, 10);
				sub_mate_entity_id += "_entity";
				sub_mate_entity_id += index;

				mate_entity_input_decorator = new MateEntityInputDecorator();
				mate_entity_input_decorator->Parse(sub_mate_entity_id.c_str(), current_element);
				mate_entity = mate_entity_input_decorator->GetMateEntity();

				if (NULL != mate_entity)
					mate_entities->push_back(mate_entity);
			}
		}
	}
	mate_->SetMateEntities(mate_entities);
}

// 获取Mate对象指针
vaar_data::Mate* MateInputDecorator::GetMate() {
	return mate_.get();
}

// 根据MateType的字符串值获取枚举对象
vaar_data::MateType MateInputDecorator::GetMateType(const char* name) {
	if (0 == strcmp(name, "ANGLE"))
		return vaar_data::MateType::ANGLE_MATE;
	if (0 == strcmp(name, "CAMFOLLOWER"))
		return vaar_data::MateType::CAMFOLLOWER_MATE;
	if (0 == strcmp(name, "COINCIDENT"))
		return vaar_data::MateType::COINCIDENT_MATE;
	if (0 == strcmp(name, "CONCENTRIC"))
		return vaar_data::MateType::CONCENTRIC_MATE;
	if (0 == strcmp(name, "COORDINATE"))
		return vaar_data::MateType::COORDINATE_MATE;
	if (0 == strcmp(name, "DISTANCE"))
		return vaar_data::MateType::DISTANCE_MATE;
	if (0 == strcmp(name, "GEAR"))
		return vaar_data::MateType::GEAR_MATE;
	if (0 == strcmp(name, "HINGE"))
		return vaar_data::MateType::HINGE_MATE;
	if (0 == strcmp(name, "LINEARCOUPLER"))
		return vaar_data::MateType::LINEARCOUPLER_MATE;
	if (0 == strcmp(name, "LOCK"))
		return vaar_data::MateType::LOCK_MATE;
	if (0 == strcmp(name, "LOCKTOSKETCH"))
		return vaar_data::MateType::LOCKTOSKETCH_MATE;
	if (0 == strcmp(name, "MAXMATES"))
		return vaar_data::MateType::MAXMATES_MATE;
	if (0 == strcmp(name, "PARALLE"))
		return vaar_data::MateType::PARALLEL_MATE;
	if (0 == strcmp(name, "PATH"))
		return vaar_data::MateType::PATH_MATE;
	if (0 == strcmp(name, "PERPENDICULAR"))
		return vaar_data::MateType::PERPENDICULAR_MATE;
	if (0 == strcmp(name, "SCREW"))
		return vaar_data::MateType::SCREW_MATE;
	if (0 == strcmp(name, "SLIDER"))
		return vaar_data::MateType::SLIDER_MATE;
	if (0 == strcmp(name, "SLOT"))
		return vaar_data::MateType::SLOT_MATE;
	if (0 == strcmp(name, "SYMMETRIC"))
		return vaar_data::MateType::SYMMETRIC_MATE;
	if (0 == strcmp(name, "TANGENT"))
		return vaar_data::MateType::TANGENT_MATE;
	if (0 == strcmp(name, "UNIVERSALJOINT"))
		return vaar_data::MateType::UNIVERSALJOINT_MATE;
	if (0 == strcmp(name, "UNKNOWN"))
		return vaar_data::MateType::UNKNOWN_MATE;
	if (0 == strcmp(name, "WIDTH"))
		return vaar_data::MateType::WIDTH_MATE;

	return vaar_data::MateType::UNKNOWN_MATE;
}

}; // namespcae vaar_file