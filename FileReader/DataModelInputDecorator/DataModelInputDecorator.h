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
// 文件: /FileReader/DataModelInputDecorater/DataModelInputDecorater.h
//       这个文件定义了一个负责解析XML文件并填充到DataModel数据类型的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef FILEREADER_DATAMODELINPUTDECORATOR_DATAMODELINPUTDECORATOR_H_
#define FILEREADER_DATAMODELINPUTDECORATOR_DATAMODELINPUTDECORATOR_H_

#include <memory>

#include "InputDecorator.h"
#include "../../VAARDataModel/DataModel.h"

namespace vaar_file {

// 负责解析SWComponent Root节点的XML数据，并填充到DataModel对象
class DataModelInputDecorator : public InputDecorator {
public:
	// 构造函数和析构函数
	DataModelInputDecorator() {}
	~DataModelInputDecorator() {}

	// 解析SWComponent Root节点的XML数据，并填充到DataModel对象。
	virtual void Parse(const char* id, xercesc::DOMElement* element);
	// 获取Face对象指针
	vaar_data::DataModel* GetDataModel();
private:
	// 操作句柄
	std::tr1::shared_ptr<vaar_data::DataModel> data_model_;
}; // class DataModelInputDeocrator

} // namespcae vaar_file
#endif