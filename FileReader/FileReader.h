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
// 文件: /FileReader/FileReader.h
//       这个文件声明了读取装配文件的接口。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef FILEREADER_FILEREADER_H_
#define FILEREADER_FILEREADER_H_

#include "../VAARDataModel/DataModel.h"

namespace vaar_file {

// 定义了装配文件的读取接口
class FileReader {
public:
	// 传入文件路径，返回DataModel对象
	virtual __declspec(dllexport) void Read(const char* file_path, vaar_data::DataModel& data_model) = 0;
};

} // namespace vaar_file

#endif