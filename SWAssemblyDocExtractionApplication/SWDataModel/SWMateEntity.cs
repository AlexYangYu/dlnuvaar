using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SWAssemblyDocExtractionApplication.SWDataModel {
    /// <summary>
    /// 配合实体
    /// </summary>
    public class SWMateEntity {
        #region 成员变量

        /// <summary>
        /// 配合实体类型
        /// </summary>
        private SWMateEntityType _mateEntityType;

        /// <summary>
        /// 实体参数
        /// </summary>
        private double[] _params = new double[8];

        /// <summary>
        /// 组件名称
        /// </summary>
        private string _componentName = string.Empty;

        #endregion

        #region 属性

        /// <summary>
        /// 配合实体类型
        /// </summary>
        public SWMateEntityType MateEntityType {
            get {
                return _mateEntityType;
            }
            set {
                _mateEntityType = value;
            }
        }

        /// <summary>
        /// 实体参数
        /// </summary>
        public double[] Params {
            get {
                return _params;
            }
            set {
                _params = value;
            }
        }

        /// <summary>
        /// 组件名称
        /// </summary>
        public string ComponentName {
            get {
                return _componentName;
            }
            set {
                _componentName = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// 配合实体类型
    /// </summary>
    public enum SWMateEntityType {
        POINT,
        LINE,
        PLANE,
        CYLINDER,
        CONE
    }
}
