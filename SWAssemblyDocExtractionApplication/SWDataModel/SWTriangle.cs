using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SWAssemblyDocExtractionApplication.SWDataModel {
    /// <summary>
    /// 三维空间三角形
    /// </summary>
    public class SWTriangle {
        #region 成员变量
        
        private SWPoint _a;
        private SWPoint _b;
        private SWPoint _c;

        #endregion 

        #region 属性

        public SWPoint A {
            get {
                return _a;
            }
            set {
                _a = value;
            }
        }

        public SWPoint B {
            get {
                return _b;
            }
            set {
                _b = value;
            }
        }

        public SWPoint C {
            get {
                return _c;
            }
            set {
                _c = value;
            }
        }

        #endregion
    }
}
