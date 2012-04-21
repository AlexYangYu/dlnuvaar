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
// 文件: /VAARDataModel/Curve.h
//       这个文件声明了曲线数据类，Curve是Edge的几何信息。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef VAARDATAMODEL_CURVE_H_
#define VAARDATAMODEL_CURVE_H_

#define CURVE_PARAMS_LENGTH 11

#include <memory>
#include <string>

namespace vaar_data {

// 曲线类型
enum CurveType {
	LINE_CURVE,
	CIRCLE_CURVE,
	ELLIPSE_CURVE,
	INTERSECTION_CURVE,
	BCURVE_CURVE,
	SPCURVE_CURVE,
	CONST_PARAM_CURVE,
	TRIMMED_CURVE
}; // enum CurveType

class Curve  {
public:
	// 构造函数和析构函数
	Curve(){}
	Curve(const char* id, const CurveType type, const double params[CURVE_PARAMS_LENGTH]) {
		SetID(id);
		SetCurveType(type);
		SetParams(params);
	}
	~Curve(){}

	// getter and setter
	// 设置params属性
	void SetParams(const double params[CURVE_PARAMS_LENGTH]) {
		for (int i = 0; i < CURVE_PARAMS_LENGTH; ++i)
			params_[i] = params[i];
	}
	// 返回一个长度为11的params数组，size存长度。
	double* GetParams(int &size) {
		size = CURVE_PARAMS_LENGTH;
		return params_;
	}

	void SetCurveType(const CurveType type) {
		type_ = type;
	}
	CurveType GetCurveType() {
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
	CurveType type_;
	//参数
	double params_[CURVE_PARAMS_LENGTH];

}; // class Curve


}; // namespace vaar_data
#endif