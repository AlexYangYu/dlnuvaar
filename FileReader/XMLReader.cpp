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
// 文件: /FileReader/XMLReader.cpp
//       这个文件实现了XMLReader类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/sax/HandlerBase.hpp>

#include "XMLReader.h"
#include "StrX.h"
#include "DataModelInputDecorator/DataModelInputDecorator.h"

namespace vaar_file {

// 传入文件路径，返回DataModel对象
__declspec(dllexport) void XMLReader::Read(const char* file_path, vaar_data::DataModel& data_model) {
	try {
		// 初始化xerces
		xercesc::XMLPlatformUtils::Initialize();
   
		xercesc::XercesDOMParser *parser = new xercesc::XercesDOMParser();
		if (NULL == parser)
			return;

		// 配置
		parser->setValidationScheme(xercesc::XercesDOMParser::Val_Auto);
		parser->setDoNamespaces(false);
		parser->setDoSchema(false);

		// 配置错误处理
		xercesc::ErrorHandler* errHandler = (xercesc::ErrorHandler*) new xercesc::HandlerBase();
		parser->setErrorHandler(errHandler);

		// 解析文件
		parser->parse(file_path);
		
		// 获取DOM文档模型
		xercesc::DOMDocument* document = parser->getDocument();
		if (NULL == document) {
			parser->adoptDocument();
			delete errHandler;
			delete parser;
			return;
		}

		// 获取Root element.
		xercesc::DOMElement* element = document->getDocumentElement();
		if (NULL == document) {
			parser->adoptDocument();
			delete errHandler;
			delete parser;
			return;
		}

		input_ = new DataModelInputDecorator();
		input_->Parse("", element);
		data_model = *input_->GetDataModel();

		parser->adoptDocument();
		delete document;
		delete errHandler;
		delete parser;
	} catch (const xercesc::XMLException& toCatch) {	
		std::cerr << "Exception message is: \n"
			<< StrX(toCatch.getMessage()) << "\n";
		return;
	} catch (const xercesc::DOMException& toCatch) {
		std::cerr << "Exception message is: \n"
			<< StrX(toCatch.getMessage()) << "\n";
		return;
	} // try
	
	xercesc::XMLPlatformUtils::Terminate();
}; //Read

} // namespace vaar_file