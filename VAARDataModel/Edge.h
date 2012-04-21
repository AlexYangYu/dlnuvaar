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
// 文件: /VAARDataModel/Edge.h
//       这个文件声明了线段数据类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef VAARDATAMODEL_EDGE_H_
#define VAARDATAMODEL_EDGE_H_

#define EDGE_PARAMS_LENGTH 11

#include <memory>
#include <string>

namespace vaar_data {

//声明并引入Curve类
class Curve;
	
//线段类
class Edge {
public:
	// 构造函数和析构函数
	Edge(){}
	Edge(const char* id, Curve* curve, const double params[EDGE_PARAMS_LENGTH]) {
		SetID(id);
		SetCurve(curve);
		SetParams(params);
	}
	~Edge(){}

	// getter and setter
	// 设置params属性
	void SetParams(const double params[EDGE_PARAMS_LENGTH]) {
		for (int i = 0; i < EDGE_PARAMS_LENGTH; ++i)
			params_[i] = params[i];
	}
	// 返回一个长度为11的params数组，size存长度。
	double* GetParams(int &size) {
		size = EDGE_PARAMS_LENGTH;
		return params_;
	}

	void SetCurve(Curve* curve) {
		curve_ = std::tr1::shared_ptr<Curve>(curve);
	}
	Curve* GetCurve() {
		return curve_.get();
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
	//几何信息
	std::tr1::shared_ptr<Curve> curve_;
	//参数
	double params_[EDGE_PARAMS_LENGTH];
}; // class Edge

} // namespace vaar_data
#endif