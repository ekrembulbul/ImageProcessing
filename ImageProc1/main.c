#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

//1836x3264

int loadImage(const char* fileName);

int main()
{
	const char* fileName = "C:/Users/EKREMBÜLBÜL/Desktop/2.jpg";
	CvMat* img = cvLoadImageM(fileName, CV_LOAD_IMAGE_UNCHANGED);
	//IplImage* img = cvLoadImage(fileName, CV_LOAD_IMAGE_UNCHANGED);
	CvScalar s;
	s = cvGet2D(img, 1835, 0);
	printf("%lf\n", s.val[0]);
	printf("%lf\n", s.val[1]);
	printf("%lf\n", s.val[2]);

	if (-1) printf("merhaba");

	return 0;
}

int loadImage(const char* fileName)
{
	return 0;
}
