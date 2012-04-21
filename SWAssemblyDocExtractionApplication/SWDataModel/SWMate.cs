using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SWAssemblyDocExtractionApplication.SWDataModel {
    /// <summary>
    /// 配合
    /// </summary>
    public class SWMate {
        #region 成员变量

        /// <summary>
        /// 配合名称
        /// </summary>
        //private string _name = string.Empty;
        
        /// <summary>
        /// 配合类型
        /// </summary>
        private SWMateType _mateType;

        /// <summary>
        /// 最大差值
        /// </summary>
        private double _maxVariation;

        /// <summary>
        /// 最小差值
        /// </summary>
        private double _minVariation;

        /// <summary>
        /// 距离和角度是否翻转
        /// </summary>
        private bool _isFlipped = false;

        /// <summary>
        /// 配合的相关实体
        /// </summary>
        private List<SWMateEntity> _mateEntities = new List<SWMateEntity>();

        #endregion

        #region 属性

        /// <summary>
        /// 配合类型
        /// </summary>
        public SWMateType MateType {
            get {
                return _mateType;
            }
            set {
                _mateType = value;
            }
        }

        /// <summary>
        /// 最大差值
        /// </summary>
        public double MaxVariation {
            get {
                return _maxVariation;
            }
            set {
                _maxVariation = value;
            }
        }

        /// <summary>
        /// 最小差值
        /// </summary>
        public double MinVariation {
            get {
                return _minVariation;
            }
            set {
                _minVariation = value;
            }
        }

        /// <summary>
        /// 距离和角度是否翻转
        /// </summary>
        public bool Flipped {
            get {
                return _isFlipped;
            }
            set {
                _isFlipped = value;
            }
        }

        /// <summary>
        /// 配合名称
        /// </summary>
        /*
        public string Name {
            get {
                return _name;
            }
            set {
                _name = value;
            }
        }
        */

        /// <summary>
        /// 配合的相关实体
        /// </summary>
        public List<SWMateEntity> MateEntities {
            get {
                return _mateEntities;
            }
            set {
                _mateEntities = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// 配和类型
    /// </summary>
    public enum SWMateType {
        /// <summary>
        /// 重合
        /// </summary>
        COINCIDENT,
        /// <summary>
        /// 同心
        /// </summary>
        CONCENTRIC,
        /// <summary>
        /// 垂直
        /// </summary>
        PERPENDICULAR,   
        /// <summary>
        /// 平行
        /// </summary>
        PARALLEL,
        /// <summary>
        /// 相切
        /// </summary>
        TANGENT,
        /// <summary>
        /// 距离
        /// </summary>
        DISTANCE,
        /// <summary>
        /// 角度
        /// </summary>
        ANGLE,
        /// <summary>
        /// 未知
        /// </summary>
        UNKNOWN,
        /// <summary>
        /// 对称
        /// </summary>
        SYMMETRIC,
        /// <summary>
        /// 凸轮顶杆
        /// </summary>
        CAMFOLLOWER,
        /// <summary>
        /// 齿轮
        /// </summary>
        GEAR,
        /// <summary>
        /// 宽度
        /// </summary>
        WIDTH,
        /// <summary>
        /// Lock to sketch
        /// </summary>
        LOCKTOSKETCH,
        /// <summary>
        /// 齿条和齿轮
        /// </summary>
        RACKPINION,
        /// <summary>
        /// Max Mates
        /// </summary>
        MAXMATES,
        /// <summary>
        /// 路径配合
        /// </summary>
        PATH,
        /// <summary>
        /// 限制
        /// </summary>
        LOCK,
        /// <summary>
        /// 螺旋配合
        /// </summary>
        SCREW,
        /// <summary>
        /// 线性耦合器
        /// </summary>
        LINEARCOUPLER,
        /// <summary>
        /// 万向节
        /// </summary>
        UNIVERSALJOINT,
        /// <summary>
        /// 坐标系
        /// </summary>
        COORDINATE,
        /// <summary>
        /// 槽
        /// </summary>
        SLOT,
        /// <summary>
        /// 铰链
        /// </summary>
        HINGE,
        /// <summary>
        /// 滑块
        /// </summary>
        SLIDER
    }
}
