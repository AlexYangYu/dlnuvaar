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
// 文件: /FileReader/StrX.h
//       这个文件定义了一个可以将XMLCh(UTF16)类型的数据转换为本地数据的类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef FILEREADER_STRX_H
#define FILEREADER_STRX_H

#include <xercesc/util/XMLString.hpp>

namespace vaar_file {

// 这是一个简单的类，让我们能轻松的将XMLCh（UTF16）类型的数据在本地输出。
class StrX {
public :
	// 构造函数。
	// 输入XMLCh字符串，用xercesc类库中提供的方法进行转换，
	// 并保存在fLocalForm成员变量中。
	StrX(const XMLCh* const toTranscode)
	{
		fLocalForm = xercesc::XMLString::transcode(toTranscode);
	}

	//析构函数
	~StrX()
	{
		xercesc::XMLString::release(&fLocalForm);
	}

	// 获取转换后的本地char数组。
	const char* LocalForm() const
	{
		return fLocalForm;
	}

private :
	// 存储转换后的本地字符串。
	char*   fLocalForm;
};

// 操作符重载
inline XERCES_STD_QUALIFIER ostream& operator<<(XERCES_STD_QUALIFIER ostream& target, const StrX& toDump) {
	target << toDump.LocalForm();
	return target;
}

} // namespace vaar_file

#endif