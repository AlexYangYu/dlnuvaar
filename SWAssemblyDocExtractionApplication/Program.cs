using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using SolidWorks.Interop.sldworks;
using SolidWorks.Interop.swconst;

using SWAssemblyDocExtractionApplication.SWDataModel;
using System.Xml.Serialization;
using System.IO;
using System.Diagnostics;

namespace SWAssemblyDocExtractionApplication {
    class Program {
        public static SldWorks swApp;

        static void Main(string[] args) {
            swApp = new SldWorks();
            ExtractTree();

            System.Console.WriteLine("Extraction finished!");
            System.Console.ReadKey();
        }

        /// <summary>
        /// 提取装备文件拓扑结构
        /// </summary>
        public static void ExtractTree() {
            ModelDoc2 swModel = default(ModelDoc2);
            Component2 rootComponent = default(Component2);
            SWComponent swComponent = default(SWComponent);

            swModel = swApp.ActiveDoc;
            rootComponent = swModel.ConfigurationManager.ActiveConfiguration.GetRootComponent3(true);
            swComponent = new SWComponent();

            CreateTree(rootComponent, ref swComponent);

            //序列化
            XmlSerializer xmlSerializer = new XmlSerializer(typeof(SWComponent));
            StreamWriter file = new StreamWriter(@"" + swComponent.Name + ".xml");
            xmlSerializer.Serialize(file, swComponent);
            file.Close();
        }

        /// <summary>
        /// 递归创建装配树
        /// </summary>
        /// <param name="component"></param>
        /// <param name="swComponet"></param>
        private static void CreateTree(Component2 component, ref SWComponent swComponent) {
            if (component == null) {
                return;
            }

            /////////复制组件相关信息///////////

            //名称
            swComponent.Name = component.Name2;

            //是否根组件
            swComponent.IsRoot = component.IsRoot();

            //提取配合,存放在RootComponent节点
            //if (swComponent.IsRoot) {
            swComponent.Mates = GetSWMatesOfRootComponent(component);
            //}

            //包围盒
            double[] box = null;
            try {
                box = component.GetBox(false, false);
            } catch (Exception e) {
                //发生异常，说明该组件（根组件）没有包围盒
                box = null;
            }
            if (box == null) {
                swComponent.BoundingBox = CreateSWBoundingBox(box);
            } else {
                swComponent.BoundingBox = new SWBoundingBox();
            }

            //提取Body
            object bodyInfo;
            object[] bodies = component.GetBodies3((int)swBodyType_e.swSolidBody, out bodyInfo);
            if (bodies != null) {
                foreach (object objBody in bodies) {
                    swComponent.Bodies.Add(CreateSWBody((Body2)objBody));
                }
            }

            //继续遍历
            object[] subComponents = component.GetChildren();
            if (subComponents != null) { 
                foreach (object obj in subComponents) {
                    SWComponent newComponent = new SWComponent();
                    CreateTree((Component2)obj, ref newComponent);
                    swComponent.SubComponents.Add(newComponent);
                }
            }
        }

        /// <summary>
        /// 获得根组件的配合
        /// </summary>
        /// <param name="rootComponent"></param>
        /// <returns></returns>
        private static List<SWMate> GetSWMatesOfRootComponent(Component2 rootComponent) {
            if (rootComponent == null)
                return null;

            List<Mate2> mates = GetMatesOfRootComponent(rootComponent);
            List<SWMate> swMates = new List<SWMate>();

            if (mates != null) {
                foreach (Mate2 mate in mates) {
                    swMates.Add(CreateSWMate(mate));
                }
            }

            return swMates;
        }

        /// <summary>
        /// 创建SWMate对象
        /// </summary>
        /// <param name="mate"></param>
        /// <returns></returns>
        private static SWMate CreateSWMate(Mate2 mate) {
            if (mate == null)
                return null;

            int entityCount = 0;
            SWMate swMate = new SWMate();

            //角度和距离是否反向
            swMate.Flipped = mate.Flipped;
            //最大差值
            swMate.MaxVariation = mate.MaximumVariation;
            //最小差值
            swMate.MinVariation = mate.MinimumVariation;
            //配合相关实体
            entityCount = mate.GetMateEntityCount();
            for (int i = 0; i < entityCount; ++i) {
                swMate.MateEntities.Add(CreateSWMateEntity((MateEntity2)mate.MateEntity(i)));
            }

            #region switch mate type

            switch (mate.Type) {
                case (int)swMateType_e.swMateANGLE:
                    swMate.MateType = SWMateType.ANGLE;
                    break;
                case (int)swMateType_e.swMateCAMFOLLOWER:
                    swMate.MateType = SWMateType.CAMFOLLOWER;
                    break;
                case (int)swMateType_e.swMateCOINCIDENT:
                    swMate.MateType = SWMateType.COINCIDENT;
                    break;
                case (int)swMateType_e.swMateCONCENTRIC:
                    swMate.MateType = SWMateType.CONCENTRIC;
                    break;
                case (int)swMateType_e.swMateCOORDINATE:
                    swMate.MateType = SWMateType.COORDINATE;
                    break;
                case (int)swMateType_e.swMateDISTANCE:
                    swMate.MateType = SWMateType.DISTANCE;
                    break;
                case (int)swMateType_e.swMateGEAR:
                    swMate.MateType = SWMateType.GEAR;
                    break;
                case (int)swMateType_e.swMateHINGE:
                    swMate.MateType = SWMateType.HINGE;
                    break;
                case (int)swMateType_e.swMateLINEARCOUPLER:
                    swMate.MateType = SWMateType.LINEARCOUPLER;
                    break;
                case (int)swMateType_e.swMateLOCK:
                    swMate.MateType = SWMateType.LOCK;
                    break;
                case (int)swMateType_e.swMateLOCKTOSKETCH:
                    swMate.MateType = SWMateType.LOCKTOSKETCH;
                    break;
                case (int)swMateType_e.swMateMAXMATES:
                    swMate.MateType = SWMateType.MAXMATES;
                    break;
                case (int)swMateType_e.swMatePARALLEL:
                    swMate.MateType = SWMateType.PARALLEL;
                    break;
                case (int)swMateType_e.swMatePATH:
                    swMate.MateType = SWMateType.PATH;
                    break;
                case (int)swMateType_e.swMatePERPENDICULAR:
                    swMate.MateType = SWMateType.PERPENDICULAR;
                    break;
                case (int)swMateType_e.swMateRACKPINION:
                    swMate.MateType = SWMateType.RACKPINION;
                    break;
                case (int)swMateType_e.swMateSCREW:
                    swMate.MateType = SWMateType.SCREW;
                    break;
                case (int)swMateType_e.swMateSLIDER:
                    swMate.MateType = SWMateType.SLIDER;
                    break;
                case (int)swMateType_e.swMateSLOT:
                    swMate.MateType = SWMateType.SLOT;
                    break;
                case (int)swMateType_e.swMateSYMMETRIC:
                    swMate.MateType = SWMateType.SYMMETRIC;
                    break;
                case (int)swMateType_e.swMateTANGENT:
                    swMate.MateType = SWMateType.TANGENT;
                    break;
                case (int)swMateType_e.swMateUNIVERSALJOINT:
                    swMate.MateType = SWMateType.UNIVERSALJOINT;
                    break;
                case (int)swMateType_e.swMateUNKNOWN:
                    swMate.MateType = SWMateType.UNKNOWN;
                    break;
                case (int)swMateType_e.swMateWIDTH:
                    swMate.MateType = SWMateType.WIDTH;
                    break;
            }

            #endregion

            return swMate;
        }

        /// <summary>
        /// 创建SWMateEntity对象
        /// </summary>
        /// <param name="entity"></param>
        /// <returns></returns>
        private static SWMateEntity CreateSWMateEntity(MateEntity2 entity) {
            if (entity == null)
                return null;

            SWMateEntity swMateEntity = new SWMateEntity();
            
            swMateEntity.ComponentName = entity.ReferenceComponent.Name2;

            #region switch mate entity type

            switch (entity.ReferenceType) {
                case (int)swSelectType_e.swSelEDGES:
                    swMateEntity.MateEntityType = SWMateEntityType.LINE;
                    break;
                case (int)swSelectType_e.swSelFACES:
                    swMateEntity.MateEntityType = SWMateEntityType.PLANE;
                    break;
                case (int)swSelectType_e.swSelVERTICES:
                    swMateEntity.MateEntityType = SWMateEntityType.POINT;
                    break;
                default:
                    throw new Exception("未知的配合实体类型");
            }

            #endregion

            for (int i = 0; i < 8; ++i) {
                swMateEntity.Params[i] = entity.EntityParams[i];
            }

            return swMateEntity;
        }
        /// <summary>
        /// 获取根组件的配合(MATE)
        /// </summary>
        /// <param name="rootComponent"></param>
        /// <returns></returns>
        private static List<Mate2> GetMatesOfRootComponent(Component2 rootComponent) {
            if (rootComponent == null)
                return null;

            Feature feature = null;
            ModelDoc2 doc = rootComponent.GetModelDoc2();
            List<Mate2> mates = new List<Mate2>();
            Mate2 pMate = null;

            //获得配合组（MateGroup）
            feature = doc.FirstFeature();
            while (feature != null) {
                if (feature.GetTypeName2() == "MateGroup")
                    break;
                feature = feature.GetNextFeature();
            }

            // 从配合组的子特征中提取配合
            if (feature != null) {
                feature = feature.GetFirstSubFeature();
                while (feature != null) {
                    pMate = feature.GetSpecificFeature2();
                    mates.Add(pMate);
                    feature = feature.GetNextSubFeature();
                }
            }

            return mates;
        }

        /// <summary>
        /// 创建SWBody对象
        /// </summary>
        /// <param name="body"></param>
        /// <returns></returns>
        private static SWBody CreateSWBody(Body2 body) {
            if (body == null)
                return null;

            SWBody swBody = new SWBody();

            swBody.Name = body.Name;

            //提取Face
            Face2 face = body.GetFirstFace();
            while (face != null) {
                swBody.Faces.Add(CreateSWFace(face));
                face = face.GetNextFace();
            }

            //提取Edge
            object[] edges = body.GetEdges();
            if (edges != null) {
                foreach (object objEdge in edges) {
                    if (objEdge != null) {
                        swBody.Edges.Add(CreateSWEdge((Edge)objEdge));
                    }
                }
            }

            //提取Vertex
            object[] vertices = body.GetVertices();
            if (vertices != null) {
                foreach (object objVertex in vertices) {
                    if (objVertex != null) {
                        swBody.Vertices.Add(CreateSWVertex((Vertex)objVertex));
                    }
                }
            }

            return swBody;
        }

        /// <summary>
        /// 创建顶点对象
        /// </summary>
        /// <param name="vertex"></param>
        /// <returns></returns>
        private static SWVertex CreateSWVertex(Vertex vertex) {
            if (vertex == null)
                return null;

            SWVertex swVertex = new SWVertex();
            double[] point = vertex.GetPoint();

            if (point != null) {
                /*
                swVertex.Point = new SWPoint() {
                    X = point[0],
                    Y = point[1],
                    Z = point[2]
                };
                 * */
                swVertex.X = point[0];
                swVertex.Y = point[1];
                swVertex.Z = point[2];
            }

            return swVertex;
        }

        /// <summary>
        /// 创建边对象
        /// </summary>
        /// <param name="edge"></param>
        /// <returns></returns>
        private static SWEdge CreateSWEdge(Edge edge) {
            if (edge == null)
                return null;

            SWEdge swEdge = new SWEdge();

            //ID
            swEdge.ID = edge.GetID();

            //参数
            double[] par = edge.GetCurveParams();
            if (par != null) {
                for (int i = 0; i < par.Length; ++i) {
                    swEdge.Params[i] = par[i];
                }
            }

            //几何信息
            swEdge.Curve = CreateSWCurve(edge.GetCurve());

            return swEdge;
        }

        /// <summary>
        /// 创建边的几何信息对象
        /// </summary>
        /// <param name="curve"></param>
        /// <returns></returns>
        private static SWCurve CreateSWCurve(Curve curve) {
            if (curve == null)
                return null;

            SWCurve swCurve = new SWCurve();
            int i = 0;
            double[] pars = null;

            switch (curve.Identity()) {
                case (int)swCurveTypes_e.LINE_TYPE:
                    swCurve.CurveType = SWCurveType.LINE;
                    pars = curve.LineParams;
                    for (i = 0; i < 6; ++i) {
                        swCurve.Params[i] = pars[i];
                    }
                    break;
                case (int)swCurveTypes_e.CIRCLE_TYPE:
                    swCurve.CurveType = SWCurveType.CIRCLE;
                    pars = curve.CircleParams;
                    for (i = 0; i < 7; ++i) {
                        swCurve.Params[i] = pars[i];
                    }
                    break;
                case (int)swCurveTypes_e.ELLIPSE_TYPE:
                    swCurve.CurveType = SWCurveType.ELLIPSE;
                    pars = curve.GetEllipseParams();
                    for (i = 0; i < 11; ++i) {
                        swCurve.Params[i] = pars[i];
                    }
                    break;
                case (int)swCurveTypes_e.BCURVE_TYPE:
                    swCurve.CurveType = SWCurveType.BCURVE;
                    break;
                case (int)swCurveTypes_e.INTERSECTION_TYPE:
                    swCurve.CurveType = SWCurveType.INTERSECTION;
                    break;
                case (int)swCurveTypes_e.SPCURVE_TYPE:
                    swCurve.CurveType = SWCurveType.SPCURVE;
                    break;
                case (int)swCurveTypes_e.TRIMMED_TYPE:
                    swCurve.CurveType = SWCurveType.TRIMMED;
                    break;
                case (int)swCurveTypes_e.CONSTPARAM_TYPE:
                    swCurve.CurveType = SWCurveType.CONST_PARAM;
                    break;
            }

            return swCurve;
        }
        /// <summary>
        /// 创建面对象
        /// </summary>
        /// <param name="face"></param>
        /// <returns></returns>
        private static SWFace CreateSWFace(Face2 face) {
            if (face == null)
                return null;

            //复制Face信息
            SWFace swFace = new SWFace();

            //ID
            swFace.ID = face.GetFaceId();
            //包围盒
            swFace.BoundingBox = CreateSWBoundingBox(face.GetBox());

            //三角面片
            //int tessTriCount = face.GetTessTriangleCount();
            swFace.TessTriangles = face.GetTessTriangles(false);
            swFace.TessNormals = face.GetTessNorms();

            //OutPutTessTriangles(tessTriCount, arrTriangles);
            /* 
            if (arrTriangles != null) {
                for (int i = 0; i < tessTriCount; i += 9) {
                    swFace.TessTriangles.Add(CreateSWTriangle(arrTriangles, i));
                }
            }
            */
           
            //几何信息
            swFace.Surface = CreateSWSurface(face.GetSurface());

            return swFace;
        }

        /// <summary>
        /// 创建面的几何信息对象
        /// </summary>
        /// <param name="surface"></param>
        /// <returns></returns>
        private static SWSurface CreateSWSurface(Surface surface) {
            if (surface == null)
                return null;

            int i = 0;
            SWSurface swSurface = new SWSurface();

            switch (surface.Identity()) {
                case (int)swSurfaceTypes_e.PLANE_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.PLANE;
                    for (i = 0; i < 6; ++i) {
                        swSurface.Params[i] = surface.PlaneParams[i];
                    }
                    break;
                case (int)swSurfaceTypes_e.SPHERE_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.SPHERE;
                    for (i = 0; i < 4; ++i) {
                        swSurface.Params[i] = surface.SphereParams[i];
                    }
                    break;
                case (int)swSurfaceTypes_e.CYLINDER_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.CYLINDER;
                    for (i = 0; i < 7; ++i) {
                        swSurface.Params[i] = surface.CylinderParams[i];
                    }
                    break;
                case (int)swSurfaceTypes_e.CONE_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.CONE;
                    for (i = 0; i < 8; ++i) {
                        swSurface.Params[i] = surface.ConeParams[i];
                    }
                    break;
                case (int)swSurfaceTypes_e.TORUS_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.TORUS;
                    for (i = 0; i < 8; ++i) {
                        swSurface.Params[i] = surface.TorusParams[i];
                    }
                    break;
                case (int)swSurfaceTypes_e.SREV_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.SREV;
                    break;
                case (int)swSurfaceTypes_e.BLEND_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.BLEND;
                    break;
                case (int)swSurfaceTypes_e.BSURF_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.BSURF;
                    break;
                case (int)swSurfaceTypes_e.EXTRU_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.EXTRU;
                    break;
                case (int)swSurfaceTypes_e.OFFSET_TYPE:
                    swSurface.SurfaceType = SWSurfaceType.OFFSET;
                    break;
            }

            return swSurface;
        }

        /// <summary>
        /// 创建三角面片对象
        /// </summary>
        /// <param name="arrPoints"></param>
        /// <param name="startIndex"></param>
        /// <returns></returns>
        private static SWTriangle CreateSWTriangle(float[] arrPoints, int startIndex) {
            SWTriangle swTriangle = new SWTriangle() {
                A = new SWPoint() {
                    X = arrPoints[startIndex + 0],
                    Y = arrPoints[startIndex + 1],
                    Z = arrPoints[startIndex + 2]
                },
                B = new SWPoint() {
                    X = arrPoints[startIndex + 3],
                    Y = arrPoints[startIndex + 4],
                    Z = arrPoints[startIndex + 5]
                },
                C = new SWPoint() {
                    X = arrPoints[startIndex + 6],
                    Y = arrPoints[startIndex + 7],
                    Z = arrPoints[startIndex + 8]
                }
            };
            return swTriangle;
        }

        /// <summary>
        /// 包围盒
        /// </summary>
        /// <param name="box"></param>
        /// <returns></returns>
        private static SWBoundingBox CreateSWBoundingBox(double[] box) {
            if (box == null)
                return null;

            //乘以1000转换到mm进制
            SWBoundingBox swBoundingBox = new SWBoundingBox() {
                LowerCorner = new SWVertex() {
                    X = box[0],
                    Y = box[1],
                    Z = box[2]
                },
                UpperCorner = new SWVertex() {
                    X = box[3],
                    Y = box[4],
                    Z = box[5]
                }
            };

            return swBoundingBox;
        }

        /// <summary>
        /// 米转换到毫米
        /// </summary>
        /// <param name="x"></param>
        /// <returns></returns>
        private static double CreateM2MM(double x) {
            return x * 1000;
        }

        /// <summary>
        ///  输出配合类型
        /// </summary>
        /// <param name="typeOfMate"></param>
        private static void OutPutMateType(int typeOfMate) {
            switch (typeOfMate) {
                case 0:
                    Debug.Print(" Mate type: Coincident");
                    break;
                case 1:
                    Debug.Print(" Mate type: Concentric");
                    break;
                case 2:
                    Debug.Print(" Mate type: Perpendicular");
                    break;
                case 3:
                    Debug.Print(" Mate type: Parallel");
                    break;
                case 4:
                    Debug.Print(" Mate type: Tangent");
                    break;
                case 5:
                    Debug.Print(" Mate type: Distance");
                    break;
                case 6:
                    Debug.Print(" Mate type: Angle");
                    break;
                case 7:
                    Debug.Print(" Mate type: Unknown");
                    break;
                case 8:
                    Debug.Print(" Mate type: Symmetric");
                    break;
                case 9:
                    Debug.Print(" Mate type: CAM follower");
                    break;
                case 10:
                    Debug.Print(" Mate type: Gear");
                    break;
                case 11:
                    Debug.Print(" Mate type: Width");
                    break;
                case 12:
                    Debug.Print(" Mate type: Lock to sketch");
                    break;
                case 13:
                    Debug.Print(" Mate type: Rack pinion");
                    break;
                case 14:
                    Debug.Print(" Mate type: Max mates");
                    break;
                case 15:
                    Debug.Print(" Mate type: Path");
                    break;
                case 16:
                    Debug.Print(" Mate type: Lock");
                    break;
                case 17:
                    Debug.Print(" Mate type: Screw");
                    break;
                case 18:
                    Debug.Print(" Mate type: Linear coupler");
                    break;
                case 19:
                    Debug.Print(" Mate type: Universal joint");
                    break;
                case 20:
                    Debug.Print(" Mate type: Coordinate");
                    break;
                case 21:
                    Debug.Print(" Mate type: Slot");
                    break;
                case 22:
                    Debug.Print(" Mate type: Hinge");
                    break;
                // Add new mate types introduced after SolidWorks 2010 FCS here
            }
        }

        /// <summary>
        /// 输出基础面类型
        /// </summary>
        /// <param name="typeOfSurface"></param>
        private static void OutPutSurfaceType(Surface surface) {
            if (surface == null) {
                return;
            }

            switch (surface.Identity()) {
                case (int)swSurfaceTypes_e.PLANE_TYPE:
                    Debug.Print("平面:");
                    Debug.Print("\t中心点：[" + (double)surface.PlaneParams[0] + "," + (double)surface.PlaneParams[1] + "," +
                        (double)surface.PlaneParams[2] + "]");
                    Debug.Print("\t法向量:[" + (double)surface.PlaneParams[3] + "," + (double)surface.PlaneParams[4] + "," +
                        (double)surface.PlaneParams[5] + "]");
                    break;
                case (int)swSurfaceTypes_e.SPHERE_TYPE:
                    Debug.Print("球面:");
                    Debug.Print("\t圆心:[" + (double)surface.SphereParams[0] + "," + (double)surface.SphereParams[1] + "," +
                        (double)surface.SphereParams[2] + "]");
                    Debug.Print("\t半径:[" + (double)surface.SphereParams[3] + "]");
                    break;
                case (int)swSurfaceTypes_e.CYLINDER_TYPE:
                    Debug.Print("圆柱面:");
                    Debug.Print("\t中心点:[" + (double)surface.CylinderParams[0] + "," + (double)surface.CylinderParams[1] + "," +
                        (double)surface.CylinderParams[2] + "]");
                    Debug.Print("\t旋转:[" + (double)surface.CylinderParams[3] + "," + (double)surface.CylinderParams[4] + "," +
                        (double)surface.CylinderParams[5] + "]");
                    Debug.Print("\t半径:[" + (double)surface.CylinderParams[6] + "]");
                    break;
                case (int)swSurfaceTypes_e.CONE_TYPE:
                    Debug.Print("圆锥面:");
                    Debug.Print("\t中心点:[" + (double)surface.ConeParams[0] + "," + (double)surface.ConeParams[1] + "," +
                        (double)surface.ConeParams[2] + "]");
                    Debug.Print("\t旋转:[" + (double)surface.ConeParams[3] + "," + (double)surface.ConeParams[4] + "," +
                        (double)surface.ConeParams[5] + "]");
                    Debug.Print("\t半径:[" + (double)surface.ConeParams[6] + "]");
                    Debug.Print("\t半角弧度:[" + (double)surface.ConeParams[7] + "]");
                    break;
                case (int)swSurfaceTypes_e.TORUS_TYPE:
                    Debug.Print("圆环面:");
                    Debug.Print("\t中心点:[" + (double)surface.TorusParams[0] + "," + (double)surface.TorusParams[1] + "," +
                        (double)surface.TorusParams[2] + "]");
                    Debug.Print("\t旋转:[" + (double)surface.TorusParams[3] + "," + (double)surface.TorusParams[4] + "," +
                        (double)surface.TorusParams[5] + "]");
                    Debug.Print("\t大圆半径:[" + (double)surface.TorusParams[6] + "]");
                    Debug.Print("\t小圆半径:[" + (double)surface.TorusParams[7] + "]");
                    break;
                case (int)swSurfaceTypes_e.SREV_TYPE:
                    Debug.Print("旋转曲面:");
                    break;
                case (int)swSurfaceTypes_e.BLEND_TYPE:
                    Debug.Print("混合曲面:");
                    break;
                case (int)swSurfaceTypes_e.BSURF_TYPE:
                    Debug.Print("平滑曲面:");
                    break;
                case (int)swSurfaceTypes_e.EXTRU_TYPE:
                    Debug.Print("挤压曲面:");
                    break;
                case (int)swSurfaceTypes_e.OFFSET_TYPE:
                    Debug.Print("偏移曲面:");
                    break;
            }
        }

        /// <summary>
        /// 输出面的三角面片数组
        /// </summary>
        /// <param name="tessTrianglesCount"></param>
        /// <param name="arrTriangles"></param>
        private static void OutPutTessTriangles(int tessTriCount, float[] arrTriangles) {
            Debug.Print("" + tessTriCount);
            if (arrTriangles != null) {
                for (int i = 0; i < tessTriCount; ++i) {
                    Debug.Print("" + arrTriangles[i * 9 + 0] + " " +
                        arrTriangles[i * 9 + 1] + " " + arrTriangles[i * 9 + 2]);
                    Debug.Print("" + arrTriangles[i * 9 + 3] + " " +
                        arrTriangles[i * 9 + 4] + " " + arrTriangles[i * 9 + 5]);
                    Debug.Print("" + arrTriangles[i * 9 + 6] + " " +
                        arrTriangles[i * 9 + 7] + " " + arrTriangles[i * 9 + 8]);
                }
            }
        }
    }
}
