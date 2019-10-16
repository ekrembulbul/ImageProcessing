#pragma

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

//1836x3264

typedef struct BGR
{
	uchar blue;
	uchar green;
	uchar red;
} BGR;

typedef struct DATA
{
	BGR data[5000][5000];
	int width;
	int height;
} DATA;

DATA g_data;
CvMat* g_img = NULL;

bool loadImage(const char* fileName);
bool saveImage(const char* fileName);
void printMat();

int main()
{
	int i = 0, j = 0;
	const char* loadFileName = "C:/Users/EKREMBÜLBÜL/Desktop/1.jpg";
	const char* saveFileName = "C:/Users/EKREMBÜLBÜL/Desktop/out1.jpg";

	bool result = loadImage(loadFileName);
	if (!result) return 1;

	printMat();

	for (i = 0; i < 20; i++)
		for (j = 0; j < g_data.width; j++)
		{
			g_data.data[i][j].blue = 0;
			g_data.data[i][j].green = 0;
			g_data.data[i][j].red = 255;
		}

	saveImage(saveFileName);

	return 0;
}

bool loadImage(const char* fileName)
{
	int i = 0, j = 0;

	printf("Reading file...\n");

	g_img = cvLoadImageM(fileName, CV_LOAD_IMAGE_UNCHANGED);
	//IplImage* img = cvLoadImage(fileName, CV_LOAD_IMAGE_UNCHANGED);
	if (!g_img)
	{
		printf("Image file could not be read!\n");
		return false;
	}

	g_data.width = g_img->cols;
	g_data.height = g_img->rows;

	for (i = 0; i < g_data.height; i++)
		for (j = 0; j < g_data.width; j++)
		{
			CvScalar cs;
			cs = cvGet2D(g_img, i, j);
			g_data.data[i][j].blue = (uchar)cs.val[0];
			g_data.data[i][j].green = (uchar)cs.val[1];
			g_data.data[i][j].red = (uchar)cs.val[2];
		}

	printf("File read.\n");

	return true;
}

bool saveImage(const char* fileName)
{
	int i = 0, j = 0;

	for (i = 0; i < g_data.height; i++)
		for (j = 0; j < g_data.width; j++)
		{
			CvScalar cs;
			cs.val[0] = g_data.data[i][j].blue;
			cs.val[1] = g_data.data[i][j].green;
			cs.val[2] = g_data.data[i][j].red;
			cvSet2D(g_img, i, j, cs);
		}

	cvSaveImage(fileName, g_img, 0);
}

void printMat()
{
	int i = 0, j = 0;

	printf("%d x %d\n", g_data.width, g_data.height);

	for (i = 0; i < g_data.width; i++)
	{
		printf("( %d %d %d )\n", g_data.data[0][i].blue, g_data.data[0][i].green, g_data.data[0][i].red);

		/*
		for (j = 0; j < width; j++)
			printf("( %d %d %d ) ", data[i][j].blue, data[i][j].green, data[i][j].red);
		printf("\n\n");
		*/
	}
}
