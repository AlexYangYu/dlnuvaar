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
// 文件: /FileReader/DataModelInputDecorater/InputDecorater.h
//       这个文件定义了解析XML数据并填充到数据类型的接口。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef FILEREADER_DATAMODELINPUTDECORATOR_INPUTDECORATOR_H_
#define FILEREADER_DATAMODELINPUTDECORATOR_INPUTDECORATOR_H_

#include <xercesc/dom/DOMElement.hpp>

namespace vaar_file {

// 定义操作接口
class InputDecorator {
public:
	virtual void Parse(const char* id, xercesc::DOMElement* element) = 0;
}; // class InputDecorator
}; // namespace vaar_file
#endif