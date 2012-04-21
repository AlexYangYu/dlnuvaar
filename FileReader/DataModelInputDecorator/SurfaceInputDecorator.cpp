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
// 文件: /FileReader/DataModelInputDecorater/SurfaceInputDecorater.cpp
//       这个文件实现了一个负责解析XML文件并填充到Surface数据类型的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#include <cstring>

#include <xercesc/util/XMLString.hpp>

#include "SurfaceInputDecorator.h"

namespace vaar_file {

// 传入的DOM模型为
// <Surface> 
// 	<SurfaceType>PLANE</SurfaceType> 
// 	<Params> 
// 	<double>0</double> 
// 	<double>0</double> 
// 	<double>-1</double> 
// 	<double>-4.163336342344337E-17</double> 
// 	<double>0.12</double> 
// 	<double>0</double> 
// 	<double>0</double> 
// 	<double>0</double> 
// 	</Params> 
// </Surface>
void SurfaceInputDecorator::Parse(const char *id, xercesc::DOMElement* element) {
	if (NULL == element)
		return;

	vaar_data::SurfaceType type = vaar_data::SurfaceType::PLANE_SURFACE;
	double params[SURFACE_PARAMS_LENGTH];
	xercesc::DOMElement* current_element = NULL;
	int i = 0;

	// 解析SurfaceType
	element = element->getFirstElementChild();
	if (NULL != element && 
		0 == strcmp("SurfaceType", xercesc::XMLString::transcode(element->getTagName()))) {
		type = GetSurfaceType(xercesc::XMLString::transcode(element->getTextContent()));
	}

	// 解析Params
	element = element->getNextElementSibling();
	
	if (NULL != element && 
		0 == strcmp("Params", xercesc::XMLString::transcode(element->getTagName()))) {
		
		current_element = element->getFirstElementChild();
		
		do {
			if (current_element == NULL)
				break;

			sscanf_s(
				xercesc::XMLString::transcode(current_element->getTextContent()),
				"%lf",
				&params[i++]
			);
		} while (
			current_element != element->getLastElementChild(),
			current_element = current_element->getNextElementSibling()
		);
	}

	// 创建并填充对象
	surface_ = std::tr1::shared_ptr<vaar_data::Surface>(new vaar_data::Surface(id, type, params));
}

// 获取Surface对象指针
vaar_data::Surface* SurfaceInputDecorator::GetSurface() {
	return surface_.get();
}

// 获取SurfaceType
vaar_data::SurfaceType SurfaceInputDecorator::GetSurfaceType(const char* name) {
	if (0 == strcmp(name, "BLEND"))
		return vaar_data::SurfaceType::BLEND_SURFACE;
	if (0 == strcmp(name, "BSURF"))
		return vaar_data::SurfaceType::BSURF_SURFACE;
	if (0 == strcmp(name, "CONE"))
		return vaar_data::SurfaceType::CONE_SURFACE;
	if (0 == strcmp(name, "CYLINDER"))
		return vaar_data::SurfaceType::CYLINDER_SURFACE;
	if (0 == strcmp(name, "EXTRU"))
		return vaar_data::SurfaceType::EXTRU_SURFACE;
	if (0 == strcmp(name, "OFFSET"))
		return vaar_data::SurfaceType::OFFSET_SURFACE;
	if (0 == strcmp(name, "PLANE"))
		return vaar_data::SurfaceType::PLANE_SURFACE;
	if (0 == strcmp(name, "SPHERE"))
		return vaar_data::SurfaceType::SPHERE_SURFACE;
	if (0 == strcmp(name, "SREV"))
		return vaar_data::SurfaceType::SREV_SURFACE;
	if (0 == strcmp(name, "TORUS"))
		return vaar_data::SurfaceType::TORUS_SURFACE;

	return vaar_data::SurfaceType::PLANE_SURFACE;
}

} // namaspace vaar_file