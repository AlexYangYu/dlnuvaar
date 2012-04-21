using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SWAssemblyDocExtractionApplication.SWDataModel {
    /// <summary>
    /// 面的几何信息
    /// </summary>
    public class SWSurface {
        #region 成员变量

        /// <summary>
        /// 类型
        /// </summary>
        private SWSurfaceType _surfaceType;

        /// <summary>
        /// 参数
        /// 目前只支持前5种类型
        /// </summary>
        private double[] _params = new double[8];

        #endregion

        #region 属性

        /// <summary>
        /// 类型
        /// </summary>
        public SWSurfaceType SurfaceType {
            get {
                return _surfaceType;
            }
            set {
                _surfaceType = value;
            }
        }

        /// <summary>
        /// 参数
        /// 目前只支持PLANE, CYLINDER, CONE, SPHERE, TORUS,类型
        /// </summary>
        public double[] Params {
            get {
                return _params;
            }
            set {
                _params = value;
            }
        }

        #endregion
    }

    /// <summary>
    /// 几何面的类型
    /// </summary>
    public enum SWSurfaceType {
        PLANE,
        CYLINDER,
        CONE,
        SPHERE,
        TORUS,
        BSURF,
        BLEND,
        OFFSET,
        EXTRU,
        SREV
    }


}
