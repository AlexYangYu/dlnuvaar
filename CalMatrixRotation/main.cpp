#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/Vec3>
#include <cstdio>

//#define MARKER_LEN 40.0
//#define MARKER_DIS 100.0
#define MARKER_LEN 20.0
#define MARKER_DIS 50.0

void OutputMatrix(const osg::Matrix *mat, char ch, float len) {
	printf("data/multi/patt_%c\n", ch);
	printf("%.1f\n", len);
	printf("0.0 0.0\n");
	double *ptr = (double*)(mat->ptr());
	for (int i = 0; i < 3; i += 1) {
		for (int j = 0; j < 4; j += 1)
			printf("%.4lf ", ptr[j * 4 + i]);
		printf("\n");
	}
}

int main(int argc, char **argv) {
	freopen("../VAAR/data/multi/marker.dat", "w", stdout);
	
	printf("5\n\n");
	
	osg::Matrix mat_a;
	OutputMatrix(&mat_a, 'a', MARKER_LEN);
	printf("\n");

	osg::ref_ptr<osg::MatrixTransform> mat_b = new osg::MatrixTransform();
	mat_b->setMatrix(
		osg::Matrix::rotate(osg::Quat(osg::DegreesToRadians(90.0), osg::Vec3(1, 0, 0))) * 
		osg::Matrix::translate(0.0, -20.0, -20.0)
	);
	OutputMatrix(&(mat_b->getMatrix()), 'b', MARKER_LEN);
	printf("\n");

	osg::ref_ptr<osg::MatrixTransform> mat_c = new osg::MatrixTransform();
	mat_c->setMatrix(
		osg::Matrix::rotate(osg::Quat(osg::DegreesToRadians(180.0), osg::Vec3(0, 0, 1))) *
		osg::Matrix::rotate(osg::Quat(osg::DegreesToRadians(-90.0), osg::Vec3(1, 0, 0))) *
		osg::Matrix::translate(0.0, 20.0, -20.0)
	);
	OutputMatrix(&(mat_c->getMatrix()), 'c', MARKER_LEN);
	printf("\n");

	osg::ref_ptr<osg::MatrixTransform> mat_d = new osg::MatrixTransform();
	mat_d->setMatrix(
		osg::Matrix::rotate(osg::Quat(osg::DegreesToRadians(-90.0), osg::Vec3(0, 0, 1))) *
		osg::Matrix::rotate(osg::Quat(osg::DegreesToRadians(-90.0), osg::Vec3(0, 1, 0))) *
		osg::Matrix::translate(-20.0, 0.0, -20.0)
		);
	OutputMatrix(&(mat_d->getMatrix()), 'd', MARKER_LEN);
	printf("\n");

	osg::ref_ptr<osg::MatrixTransform> mat_f = new osg::MatrixTransform();
	mat_f->setMatrix(
		osg::Matrix::rotate(osg::Quat(osg::DegreesToRadians(90.0), osg::Vec3(0, 0, 1))) *
		osg::Matrix::rotate(osg::Quat(osg::DegreesToRadians(90.0), osg::Vec3(0, 1, 0))) *
		osg::Matrix::translate(20.0, 0.0, -20.0)
	);
	OutputMatrix(&(mat_f->getMatrix()), 'f', MARKER_LEN);

	return 0;
}