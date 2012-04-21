using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SWAssemblyDocExtractionApplication.SWDataModel {
    /// <summary>
    /// 边
    /// </summary>
    public class SWEdge {
        
        #region 成员变量

        /// <summary>
        /// ID
        /// </summary>
        private int _id;

        /// <summary>
        /// 几何信息对象
        /// </summary>
        private SWCurve _curve = null;

        /// <summary>
        /// 参数
        /// 说明：
        ///     http://help.solidworks.com/2010/English/api/sldworksapi/solidworks.interop.sldworks~solidworks.interop.sldworks.iedge~getcurveparams2.html
        /// </summary>
        private double[] _params = new double[11];

        #endregion

        #region 属性

        /// <summary>
        /// ID
        /// </summary>
        public int ID {
            get {
                return _id;
            }
            set {
                _id = value;
            }
        }

        /// <summary>
        /// 几何信息
        /// </summary>
        public SWCurve Curve {
            get {
                return _curve;
            }
            set {
                _curve = value;
            }
        }

        /// <summary>
        /// 参数
        /// 说明：
        ///     http://help.solidworks.com/2010/English/api/sldworksapi/solidworks.interop.sldworks~solidworks.interop.sldworks.iedge~getcurveparams2.html
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
}
