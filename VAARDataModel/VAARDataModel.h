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
// 作者: Yang Yu, alex890714@gmail.com
// 文件: /VAARDataModel/VAARDataModel.h
//       这个文件声明了数据包的入口。
// 修改历史：
///////////////////////////////////////////////////////////////////////////
#ifndef VAARDATAMODEL_VAARDATEMODEL_H_
#define VAARDATAMODEL_VAARDATEMODEL_H_

#include "DataModel.h"
#include "Component.h"
#include "Curve.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "Surface.h"
#include "Mate.h"
#include "MateEntity.h"

namespace vaar_data {

// 此类作为入口函数的类，不起任何作用
class VAARDataModel {
public:
	VAARDataModel(){}
	~VAARDataModel(){}
	__declspec(dllexport) void Main();
}; // class VAARDataModel

}; // namespace vaar_data.

#endif