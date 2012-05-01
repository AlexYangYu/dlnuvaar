#include <osg/Matrix>
#include <osg/Vec3>
#include <cstdio>

void OutputMatrix(const osg::Matrix *mat) {
	double *ptr = (double*)(mat->ptr());
	for (int i = 0; i < 4; i += 1) {
		for (int j = 0; j < 4; j += 1)
			printf("%.4lf ", ptr[j * 4 + i]);
		printf("\n");
	}
}

int main(int argc, char **argv) {
	osg::Matrix mat;
	mat.makeRotate(osg::Quat(osg::DegreesToRadians(90.0), 1.0, 0.0, 0.0));
	OutputMatrix(&mat);
	return 0;
}