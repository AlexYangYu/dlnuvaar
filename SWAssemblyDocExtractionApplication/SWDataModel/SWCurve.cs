using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SWAssemblyDocExtractionApplication.SWDataModel {
    /// <summary>
    /// 边的几何信息
    /// </summary>
    public class SWCurve {
        #region 成员变量

        /// <summary>
        /// 边的几何类别
        /// </summary>
        private SWCurveType _curveType;

        /// <summary>
        /// 边的几何参数
        /// </summary>
        private double[] _params = new double[11];
        
        #endregion

        #region 属性

        /// <summary>
        /// 边的几何类别
        /// </summary>
        public SWCurveType CurveType {
            get {
                return _curveType;
            }
            set {
                _curveType = value;
            }
        }

        /// <summary>
        /// 边的几何参数
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
    /// 边的几何类别
    /// </summary>
    public enum SWCurveType {
        LINE,
        CIRCLE,
        ELLIPSE,
        INTERSECTION,
        BCURVE,
        SPCURVE,
        CONST_PARAM,
        TRIMMED
    }
}
