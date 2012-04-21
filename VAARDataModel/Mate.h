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
// 文件: /VAARDataModel/Mate.h
//       这个文件声明了配合数据类。
// 修改历史：
///////////////////////////////////////////////////////////////////////////

#ifndef VAARDATAMODEL_MATE_H_
#define VAARDATAMODEL_MATE_H_

#include <memory>
#include <vector>
#include <string>

namespace vaar_data {

// 声明并引入MateEntity类
class MateEntity;

// 配合类型
enum MateType {
	// 重合
	COINCIDENT_MATE,
	// 同心
	CONCENTRIC_MATE,
	// 垂直
	PERPENDICULAR_MATE,   
	// 平行
	PARALLEL_MATE,
	// 相切
	TANGENT_MATE,
	// 距离
	DISTANCE_MATE,
	// 角度
	ANGLE_MATE,
	// 未知
	UNKNOWN_MATE,
	// 对称
	SYMMETRIC_MATE,
	// 凸轮顶杆
	CAMFOLLOWER_MATE,
	// 齿轮
	GEAR_MATE,
	// 宽度
	WIDTH_MATE,
	// Lock to sketch
	LOCKTOSKETCH_MATE,
	// 齿条和齿轮
	RACKPINION_MATE,
	// Max Mates
	MAXMATES_MATE,
	// 路径配合
	PATH_MATE,
	// 限制
	LOCK_MATE,
	// 螺旋配合
	SCREW_MATE,
	// 线性耦合器
	LINEARCOUPLER_MATE,
	// 万向节
	UNIVERSALJOINT_MATE,
	// 坐标系
	COORDINATE_MATE,
	// 槽
	SLOT_MATE,
	// 铰链
	HINGE_MATE,
	// 滑块
	SLIDER_MATE
}; // enum MateType

// 配合数据类
class Mate {
public:
	// 构造函数和析构函数
	Mate(){}
	Mate(const char* id,
		 const MateType type,
		 const double min_variation,
		 const double max_variation,
		 std::vector<MateEntity*>* entities,
		 const bool flipped) {
		SetID(id);
		SetMateType(type);
		SetMaxVariation(max_variation);
		SetMinVariation(min_variation);
		SetMateEntities(entities);
		SetFlipped(flipped);
	}
	~Mate(){}

	// getter and setter
	void SetID(const char* id) {
		id_ = std::tr1::shared_ptr<std::string>(new std::string(id));
	}
	char* GetID() {
		return (char*)id_.get()->c_str();
	}

	void SetMateType(const MateType type) {
		type_ = type;
	}
	MateType GetMateType() {
		return type_;
	}

	void SetMinVariation(const double par_value) {
		min_variation_ = par_value;
	} 
	double GetMinVariation() {
		return min_variation_;
	}

	void SetMaxVariation(const double par_value) {
		max_variation_ = par_value;
	} 
	double GetMaxVariation() {
		return max_variation_;
	}

	void SetFlipped(const bool filpped) {
		is_flipped_ = filpped;
	}
	bool GetFlipped() {
		return is_flipped_;
	}

	void SetMateEntities(std::vector<MateEntity*>* entities) {
		mate_entities_ = std::tr1::shared_ptr<std::vector<MateEntity*>>(entities);
	}
	std::vector<MateEntity*>* GetMateEntities() {
		return mate_entities_.get();
	}
private:
	// ID
	std::tr1::shared_ptr<std::string> id_;
	// 配合类型
	MateType type_;
	// 最大差值
	double max_variation_;
	// 最小差值
	double min_variation_;
	// 距离和角度是否翻转
	bool is_flipped_;

	// 配合的相关实体
	std::tr1::shared_ptr<std::vector<MateEntity*>> mate_entities_;
}; // class Mate

} // namespace vaar_data
#endif