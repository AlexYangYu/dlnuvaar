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
// 文件: /VAARDataModel/Vertex.h
//       这个文件声明顶点数据类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef VAARDATAMODEL_VERTEX_H_
#define VAARDATAMODEL_VERTEX_H_

#include <memory>
#include <string>

namespace vaar_data {

class Vertex {
public:
	// 构造函数、析构函数
	Vertex(){}
	Vertex(const char* id, const double x, const double y, const double z) {
		SetID(id);
		SetX(x);
		SetY(y);
		SetZ(z);
	}
	~Vertex(){}

	// getter and setter
	void SetX(const double x) {
		x_ = x;
	}
	double GetX() {
		return x_;
	}

	void SetY(const double y) {
		y_ = y;
	}
	double GetY() {
		return y_;
	}

	void SetZ(const double z) {
		z_ = z;
	}
	double GetZ() {
		return z_;
	}

	void SetID(const char* id) {
		id_ = std::tr1::shared_ptr<std::string>(new std::string(id));
	}
	char* GetID() {
		return (char*)id_.get()->c_str();
	}
private:
	// ID
	std::tr1::shared_ptr<std::string> id_;
	// 坐标
	double x_;
	double y_;
	double z_;
}; // class Vertex

} // namespace vaar_data
#endif