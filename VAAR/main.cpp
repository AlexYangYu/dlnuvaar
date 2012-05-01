#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include <osg/PrimitiveSet>
#include <osg/ComputeBoundsVisitor>
#include <osg/BoundingBox>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/NodeCallback>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgDB/FileUtils>

#include <osgART/Foundation>
#include <osgART/VideoLayer>
#include <osgART/PluginManager>
#include <osgART/VideoGeode>
#include <osgART/Utils>
#include <osgART/GeometryUtils>
#include <osgART/MarkerCallback>
#include <osgART/TransformFilterCallback>

#include "XMLReader.h"
#include "FileReader.h"
#include "../VAARDataModel/DataModel.h"
#include "../VAARDataModel/Component.h"
#include "KeyboardEventRouter.h"
#include "KeyboardEventHandler.h"


osg::Geometry* CreatGeometry(
	vaar_data::Component* component,
	const osg::Vec4f color
) {
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	
	geometry->setVertexArray(component->GetRefPtrTriangles().get());
	geometry->setNormalArray(component->GetRefPtrNormals().get());
	geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

	osg::ref_ptr<osg::Vec4Array> color_array = new osg::Vec4Array;
	color_array->push_back(color);
	geometry->setColorArray(color_array.get());
	geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	geometry->addPrimitiveSet(
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, component->GetRefPtrTriangles()->size())
	);

	return geometry.release();
} // CreatGeometry


osg::Group* CreateImageBackground(osg::Image* video) {
	osgART::VideoLayer* _layer = new osgART::VideoLayer();
	_layer->setSize(*video);
	osgART::VideoGeode* _geode = new osgART::VideoGeode(osgART::VideoGeode::USE_TEXTURE_RECTANGLE, video);
	addTexturedQuad(*_geode,video->s(),video->t());
	_layer->addChild(_geode);
	return _layer;
} // CreateImageBackground


osg::Geode* CreateBoundingBox(
	const osg::BoundingBox *bounding_box,
	const float line_width,
	const osg::Vec4 color
) {
	osg::ref_ptr<osg::Geode> box = new osg::Geode();
	
	osg::ref_ptr<osg::ShapeDrawable> box_shape = new osg::ShapeDrawable(
		new osg::Box(
			osg::Vec3(bounding_box->center()), 
			bounding_box->xMax() - bounding_box->xMin(), 
			bounding_box->yMax() - bounding_box->yMin(),
			bounding_box->zMax() - bounding_box->zMin()
		)
	);
	box_shape->setColor(color);
	osg::ref_ptr<osg::StateSet> box_state = box_shape->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> box_mode = new osg::PolygonMode(
		osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE
	);
	box_state->setAttributeAndModes(box_mode.get());
	box_state->setAttribute(new osg::LineWidth(line_width));
	box->addDrawable(box_shape.get());

	return box.release();
} // CreateBoundingBox


class WorldCoordNodeVistor : public osg::NodeVisitor {
public:
	WorldCoordNodeVistor():
	    osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_PARENTS),
		done(false) {
		_wc_matrix = NULL;
	} // WorldCoordNodeVistor

	~WorldCoordNodeVistor() {
		delete _wc_matrix;
	}

	virtual void apply(osg::Node &node) {
		if (!done) {
			if (0 == node.getNumParents()) {
				_wc_matrix = new osg::Matrix();
				_wc_matrix->set(osg::computeLocalToWorld(this->getNodePath()));
				done = true;
			}
			traverse(node);
		}
	} // apply

	osg::Matrix* GetMatrix() {
		return _wc_matrix;
	}
private:
	bool done;
	osg::Matrix *_wc_matrix;
}; // WorldCoordNodeVistor


class CollisionDetectionCallback : public osg::NodeCallback {
public:
	CollisionDetectionCallback(
		osg::BoundingBox *box_1, 
		osg::BoundingBox *box_2,
		osg::Node *node_1,
		osg::Node *node_2
	) {
		_box_1 = box_1;
		_box_2 = box_2;
		_node_1 = node_1;
		_node_2 = node_2;
	} // CollisionDetectionCallback

	~CollisionDetectionCallback() {
	}

	virtual void operator()(osg::Node *node, osg::NodeVisitor *node_visitor) {
		// Get the local to world matrix.
		osg::Matrix *wc_mat_1, *wc_mat_2;
		WorldCoordNodeVistor wc_visitor_1, wc_visitor_2;

		_node_1->accept(wc_visitor_1);
		wc_mat_1 = wc_visitor_1.GetMatrix();
		_node_2->accept(wc_visitor_2);
		wc_mat_2 = wc_visitor_2.GetMatrix();

		if (wc_mat_1 == NULL || wc_mat_2 == NULL) {
			osg::notify(osg::NOTICE) << "Not Collision Detected!" << std::endl;
			return;
		}
		
		// Update bounding box.
		osg::BoundingBox temp_box_1, temp_box_2;
		for (unsigned int i = 0; i < 8; ++i) {
			temp_box_1.expandBy(_box_1->corner(i) * (*wc_mat_1));
			temp_box_2.expandBy(_box_2->corner(i) * (*wc_mat_2));
		}

		// BoundingBox-BoudingBox collision detection.
		if (temp_box_1.intersects(temp_box_2)) {
			osg::notify(osg::NOTICE) << "Collision Detected - Bounding Box!" << std::endl;
			// BoundgingBox-Triangles collision detection.
			// Triangles-Triangles collision detection.
		} else {
			osg::notify(osg::NOTICE) << "Not Collision Detected!" << std::endl;
			return;
		}
	} // operator£¨)
private:
	osg::BoundingBox *_box_1, *_box_2;
	osg::Node *_node_1, *_node_2;
}; // CollisionDetectionCallback

void Run(vaar_data::DataModel& data_model) {
	// create a root node
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->setName("Root");

	osgViewer::Viewer viewer;
	osg::ref_ptr<osg::Switch> switcher_b = new osg::Switch();
	switcher_b->setName("Switcher B");
	osg::ref_ptr<osg::Switch> switcher_n = new osg::Switch();
	switcher_n->setName("Switcher N");
	
	// initialize keyboard handler
	KeyboardEventRouter *key_event_router = new KeyboardEventRouter();
	key_event_router->AddHandler(
		osgGA::GUIEventAdapter::KEY_B, 
		KeyboardEventRouter::KEY_DOWN,
		new KeyBDownHandler(switcher_b.get())
	);
	key_event_router->AddHandler(
		osgGA::GUIEventAdapter::KEY_N, 
		KeyboardEventRouter::KEY_DOWN,
		new KeyBDownHandler(switcher_n.get())
	);

	// attach root node to the viewer
	viewer.setSceneData(root.get());

	// add relevant handlers to the viewer
	viewer.addEventHandler(key_event_router);
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.addEventHandler(new osgViewer::ThreadingHandler);
	viewer.addEventHandler(new osgViewer::HelpHandler);


	// preload the video and tracker
	int _video_id = osgART::PluginManager::instance()->load("osgart_video_artoolkit2");
	//int _tracker_id = osgART::PluginManager::instance()->load("osgart_tracker_sstt");
	int _tracker_id = osgART::PluginManager::instance()->load("osgart_tracker_artoolkit2");

	// load a video plugin.
	osg::ref_ptr<osgART::Video> video = 
		dynamic_cast<osgART::Video*>(osgART::PluginManager::instance()->get(_video_id));

	// check if an instance of the video stream could be started
	if (!video.valid()) {   
		// Without video an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not initialize video plugin!" << std::endl;
		exit(-1);
	}

	// Open the video. This will not yet start the video stream but will
	// get information about the format of the video which is essential
	// for the connected tracker
	video->open();

	osg::ref_ptr<osgART::Tracker> tracker = 
		dynamic_cast<osgART::Tracker*>(osgART::PluginManager::instance()->get(_tracker_id));

	if (!tracker.valid()) {
		// Without tracker an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not initialize tracker plugin!" << std::endl;
		exit(-1);
	}

	// get the tracker calibration object
	osg::ref_ptr<osgART::Calibration> calibration = tracker->getOrCreateCalibration();

	// load a calibration file
	if (!calibration->load(std::string("data/camera_para.dat"))) {
		// the calibration file was non-existing or couldnt be loaded
		osg::notify(osg::FATAL) << "Non existing or incompatible calibration file" << std::endl;
		exit(-1);
	}

	// set the image source for the tracker
	tracker->setImage(video.get());

	osgART::TrackerCallback::addOrSet(root.get(), tracker.get());

	osg::ref_ptr<osgART::Marker> marker_1 = tracker->addMarker("single;data/patt.hiro;80;0;0");
	osg::ref_ptr<osgART::Marker> marker_2 = tracker->addMarker("single;data/patt.kanji;80;0;0");
	if (!marker_1.valid()) {
		// Without marker an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not add marker_1!" << std::endl;
		exit(-1);
	}
	if (!marker_2.valid()) {
		// Without marker an AR application can not work. Quit if none found.
		osg::notify(osg::FATAL) << "Could not add marker_2!" << std::endl;
		exit(-1);
	}

	marker_1->setActive(true);
	marker_2->setActive(true);

	osg::ref_ptr<osg::MatrixTransform> marker_trans_1 = new osg::MatrixTransform();
	osgART::attachDefaultEventCallbacks(marker_trans_1.get(), marker_1.get());
	marker_trans_1->setName("MarkerTrans 1");
	osg::ref_ptr<osg::MatrixTransform> marker_trans_2 = new osg::MatrixTransform();
	osgART::attachDefaultEventCallbacks(marker_trans_2.get(), marker_2.get());
	marker_trans_2->setName("MarkerTrans 2");

	osg::ref_ptr<osg::Geode> marker_geode_1 = new osg::Geode;
	marker_geode_1->setName("MarkerGeode 1");
	osg::ref_ptr<osg::Geode> marker_geode_2 = new osg::Geode;
	marker_geode_2->setName("MarkerGeode 2");
	vaar_data::Component* component = data_model.GetRoot();
	marker_geode_1->addDrawable(
		CreatGeometry(component->GetSubComponents()->at(0), osg::Vec4f(1.0f, 0.0f, 0.0f, 1.0f))
	);
	marker_geode_2->addDrawable(
		CreatGeometry(component->GetSubComponents()->at(1), osg::Vec4f(0.0f, 0.0f, 0.8f, 1.0f))
	);
	marker_trans_1->addChild(marker_geode_1.get());
	marker_trans_1->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");
	marker_trans_2->addChild(marker_geode_2.get());
	marker_trans_2->getOrCreateStateSet()->setRenderBinDetails(100, "RenderBin");

	// Bounding Box
	osg::ComputeBoundsVisitor bound_visitor;
	marker_geode_1->accept(bound_visitor);
	osg::BoundingBox bounding_box_1 = bound_visitor.getBoundingBox();
	switcher_b->addChild(
		CreateBoundingBox(
			&bounding_box_1, 5.0, osg::Vec4(0.0, 1.0, 0.0, 1.0)
		)
	);
	marker_trans_1->addChild(switcher_b.get());
	bound_visitor.reset();
	marker_geode_2->accept(bound_visitor);
	osg::BoundingBox bounding_box_2 = bound_visitor.getBoundingBox();
	switcher_n->addChild(
		CreateBoundingBox(
			&bounding_box_2, 5.0, osg::Vec4(0.0, 1.0, 0.0, 1.0)
		)
	);
	marker_trans_2->addChild(switcher_n.get());

	osg::ref_ptr<osg::Group> videoBackground = CreateImageBackground(video.get());
	videoBackground->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");

	osg::ref_ptr<osg::Camera> cam = calibration->createCamera();
	cam->addChild(marker_trans_1.get());
	cam->addChild(marker_trans_2.get());
	cam->addChild(videoBackground.get());

	root->addChild(cam.get());
	root->setUpdateCallback(
		new CollisionDetectionCallback(
			&bounding_box_1, &bounding_box_2, marker_geode_1.get(), marker_geode_2.get()
		)
	);
	video->start();
	viewer.run();
	video->close();
} // Run

int main() {
	vaar_data::DataModel* data_model = new vaar_data::DataModel();
	vaar_file::FileReader* file_reader = new vaar_file::XMLReader;
	
	file_reader->Read("tutor.xml", *data_model);
	if (NULL != file_reader)
		delete file_reader;

	Run(*data_model);

	if (NULL != data_model)
		delete data_model;
	
	return 0;
} // main