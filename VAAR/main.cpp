#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include <osg/PrimitiveSet>
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

class KeyboardEventHandler : public osgGA::GUIEventHandler {
public:
	// Constructor
	KeyboardEventHandler(osg::MatrixTransform *matrix) : osgGA::GUIEventHandler(){
		_scale_matrix = matrix;
		_scale = 1.0;
	} // constructor

	virtual bool handle(
		const osgGA::GUIEventAdapter &event_adapter, 
		osgGA::GUIActionAdapter &acation_adapter
	) {
		//osg::notify(osg::NOTICE) << "Handling" << std::endl;
		switch(event_adapter.getEventType()) {
			case osgGA::GUIEventAdapter::KEYDOWN: {
				switch(event_adapter.getKey()) {
					case osgGA::GUIEventAdapter::KEY_J:
						//osg::notify(osg::NOTICE) << "J pressed" << std::endl;
						_scale -= 0.1;
						_scale_matrix->setMatrix(osg::Matrix::scale(_scale, _scale, _scale));
						osg::notify(osg::NOTICE) << _scale_matrix->getMatrix().getScale() << std::endl;
						return true;
					case osgGA::GUIEventAdapter::KEY_K:
						//osg::notify(osg::NOTICE) << "K pressed" << std::endl;
						_scale += 0.1;
						_scale_matrix->setMatrix(osg::Matrix::scale(_scale, _scale, _scale));
						osg::notify(osg::NOTICE) << _scale_matrix->getMatrix().getScale() << std::endl;
						return true;
				}
			}
			default:
				return false;
		}
	} // handle

private:
	osg::ref_ptr<osg::MatrixTransform> _scale_matrix;
	float _scale;
}; // KeyboardEventHandler

void Run(vaar_data::DataModel& data_model) {
	// create a root node
	osg::ref_ptr<osg::Group> root = new osg::Group;
	osg::ref_ptr<osg::MatrixTransform> scale_matrix = new osg::MatrixTransform();
	osgViewer::Viewer viewer;

	// attach root node to the viewer
	viewer.setSceneData(root.get());

	// add relevant handlers to the viewer
	viewer.addEventHandler(new KeyboardEventHandler(scale_matrix.get()));
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgViewer::WindowSizeHandler);
	viewer.addEventHandler(new osgViewer::ThreadingHandler);
	viewer.addEventHandler(new osgViewer::HelpHandler);


	// preload the video and tracker
	int _video_id = osgART::PluginManager::instance()->load("osgart_video_artoolkit2");
	//int _tracker_id = osgART::PluginManager::instance()->load("osgart_tracker_sstt");
	int _tracker_id = osgART::PluginManager::instance()->load("osgart_tracker_artoolkit2");

	// Load a video plugin.
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
	osg::ref_ptr<osg::MatrixTransform> marker_trans_2 = new osg::MatrixTransform();
	osgART::attachDefaultEventCallbacks(marker_trans_2.get(), marker_2.get());

	osg::ref_ptr<osg::Geode> marker_geode_1 = new osg::Geode;
	osg::ref_ptr<osg::Geode> marker_geode_2 = new osg::Geode;
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

	osg::ref_ptr<osg::Group> videoBackground = CreateImageBackground(video.get());
	videoBackground->getOrCreateStateSet()->setRenderBinDetails(0, "RenderBin");

	osg::ref_ptr<osg::Camera> cam = calibration->createCamera();
	scale_matrix->addChild(marker_trans_1.get());
	scale_matrix->addChild(marker_trans_2.get());
	cam->addChild(scale_matrix.get());
	cam->addChild(videoBackground.get());

	root->addChild(cam.get());
	video->start();
	viewer.run();
	video->close();
}

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
}