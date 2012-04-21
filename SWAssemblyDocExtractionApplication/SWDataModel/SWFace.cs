using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SWAssemblyDocExtractionApplication.SWDataModel {
    /// <summary>
    /// 三维面
    /// </summary>
    public class SWFace {
        #region 成员变量

        /// <summary>
        /// ID
        /// </summary>
        private int _id;

        /// <summary>
        /// 包围盒
        /// </summary>
        private SWBoundingBox _boundingBox = null;

        /// <summary>
        /// 三角面片
        /// </summary>
        //private List<SWTriangle> _tessTriangles = new List<SWTriangle>();
        private float[] _tessTriangles = null;

        /// <summary>
        /// 三角面片法向量
        /// </summary>
        private float[] _tessNormals = null;

        /// <summary>
        /// 几何信息
        /// </summary>
        private SWSurface _surface = null;

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
        /// 包围盒
        /// </summary>
        public SWBoundingBox BoundingBox {
            get {
                return _boundingBox;
            }
            set {
                _boundingBox = value;
            }
        }

        /// <summary>
        /// 三角面片
        /// </summary>
        public float[] TessTriangles {
            get {
                return _tessTriangles;
            }
            set {
                _tessTriangles = value;
            }
        }
        /*
        public List<SWTriangle> TessTriangles {
            get {
                return _tessTriangles;
            }
            set {
                _tessTriangles = value;
            }
        }
         * */

        /// <summary>
        /// 三角面片法向量
        /// </summary>
        public float[] TessNormals {
            get {
                return _tessNormals;
            }
            set {
                _tessNormals = value;
            }
        }

        /// <summary>
        /// 几何信息
        /// </summary>
        public SWSurface Surface {
            get {
                return _surface;
            }
            set {
                _surface = value;
            }
        }

        #endregion
    }
}
