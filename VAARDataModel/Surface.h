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
// 文件: /VAARDataModel/Surface.h
//       这个文件声明了表面数据类，Surface是Face的几何信息。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef VAARDATAMODEL_SURFACE_H_
#define VAARDATAMODEL_SURFACE_H_

#define SURFACE_PARAMS_LENGTH 8

#include <memory>
#include <string>

namespace vaar_data {

// Surface类型的枚举。
enum SurfaceType {
	PLANE_SURFACE,
	CYLINDER_SURFACE,
	CONE_SURFACE,
	SPHERE_SURFACE,
	TORUS_SURFACE,
	BSURF_SURFACE,
	BLEND_SURFACE,
	OFFSET_SURFACE,
	EXTRU_SURFACE,
	SREV_SURFACE
};  // enum SurfaceType

// Surface类是Face类的几何信息，其对象作为Face的成员变量。
class Surface {
public:
	// 构造函数和析构函数
	Surface(){}
	Surface(const char* id, const SurfaceType type, const double params[SURFACE_PARAMS_LENGTH]) {
		SetID(id);
		SetSurfaceType(type);
		SetParams(params);
	}
	~Surface(){}

	// getter and setter
	// 设置params属性
	void SetParams(const double params[SURFACE_PARAMS_LENGTH] ) {
		for (int i = 0; i < SURFACE_PARAMS_LENGTH; ++i)
			params_[i] = params[i];
	}
	// 返回一个长度为8的params数组，size存长度。
	double* GetParams(int &size) {
		size = SURFACE_PARAMS_LENGTH;
		return params_;
	}

	void SetSurfaceType(const SurfaceType type) {
		type_ = type;
	}
	SurfaceType GetSurfaceType() {
		return type_;
	}

	void SetID(const char* id) {
		id_ = std::tr1::shared_ptr<std::string>(new std::string(id));
	}
	char* GetID() {
		return (char*)id_.get()->c_str();
	}
private:
	//ID
	std::tr1::shared_ptr<std::string> id_;
	//类型
	SurfaceType type_;
	//参数
	double params_[SURFACE_PARAMS_LENGTH];

}; // class Surface.


} // namespace vaar_data

#endif 