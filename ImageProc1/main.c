#pragma

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define EPSILON 1

typedef struct BGR
{
	uchar blue;
	uchar green;
	uchar red;
} BGR;

typedef struct DATA
{
	BGR data[1080][1920];
	int width;
	int height;
} DATA;

typedef struct SEGMENT
{
	BGR pixel[2073600];
	unsigned int size;
	unsigned int i[2073600];
	unsigned int j[2073600];
} SEGMENT;

typedef struct GROUP
{
	BGR mean;
	SEGMENT segment;
} GROUP;

DATA g_data;
GROUP g_group[32];
CvMat* g_pImg = NULL;
int g_K = 8;

void setSegment();
bool loadImage(const char* fileName);
void saveImage(const char* fileName);
void printBGR(BGR* data);
void copyData(BGR* destionation ,const BGR* source);
void setMeans(unsigned char k);
void setGroup();
double getDistance(BGR* color1, BGR* color2);
int findSmallestElement(double* distances, int size);
void findAvarage(BGR* data);
double getEpsilon(BGR* avarage);
void setSegmentData();

int main()
{
	int i = 0;
	//g_group = (GROUP*)malloc(K8 * sizeof(GROUP));

	const char* loadFileName = "C:/Users/EKREMBÜLBÜL/Desktop/1.jpg";
	const char* saveFileName[3] = { "C:/Users/EKREMBÜLBÜL/Desktop/out1.8.jpg", "C:/Users/EKREMBÜLBÜL/Desktop/out1.16.jpg", "C:/Users/EKREMBÜLBÜL/Desktop/out1.32.jpg" };

	for (i = 0; i < 3; i++)
	{
		auto result = loadImage(loadFileName);
		if (!result) return 1;
		if (i == 0) g_K = 8;
		else if (i == 1) g_K = 16;
		else if (i == 2) g_K = 32;
		setSegment();
		setSegmentData();
		saveImage(saveFileName[i]);
	}

	return 0;
}

bool loadImage(const char* fileName)
{
	int i = 0, j = 0;

	printf("Reading file...\n");

	g_pImg = cvLoadImageM(fileName, CV_LOAD_IMAGE_UNCHANGED);
	//IplImage* img = cvLoadImage(fileName, CV_LOAD_IMAGE_UNCHANGED);
	if (!g_pImg)
	{
		printf("Image file could not be read!\n");
		return false;
	}

	g_data.width = g_pImg->cols;
	g_data.height = g_pImg->rows;

	for (i = 0; i < g_data.height; i++)
		for (j = 0; j < g_data.width; j++)
		{
			CvScalar cs;
			cs = cvGet2D(g_pImg, i, j);
			g_data.data[i][j].blue = (uchar)cs.val[0];
			g_data.data[i][j].green = (uchar)cs.val[1];
			g_data.data[i][j].red = (uchar)cs.val[2];
		}

	printf("File read.\n");

	return true;
}

void saveImage(const char* fileName)
{
	int i = 0, j = 0;

	printf("Saving file...\n");

	for (i = 0; i < g_data.height; i++)
		for (j = 0; j < g_data.width; j++)
		{
			CvScalar cs;
			cs.val[0] = g_data.data[i][j].blue;
			cs.val[1] = g_data.data[i][j].green;
			cs.val[2] = g_data.data[i][j].red;
			cvSet2D(g_pImg, i, j, cs);
		}

	cvSaveImage(fileName, g_pImg, 0);

	printf("File saved.\n");
}

void setSegment()
{
	int i = 0;
	setMeans(g_K);
	double epsilon = (double)LLONG_MAX;

	printf("Grouping pixels...\n");

	while (epsilon > EPSILON)
	{
		for (i = 0; i < g_K; i++) g_group[i].segment.size = 0;
		setGroup();
		BGR avarage[32];
		findAvarage(avarage);
		epsilon = getEpsilon(avarage);
		for (i = 0; i < g_K; i++) copyData(&g_group[i].mean, &avarage[i]);
		printf("%lf\n", epsilon);
	}

	printf("Pixels grouped.\n");

	for (i = 0; i < g_K; i++) printBGR(&g_group[i].mean);
}

void setMeans(unsigned char k)
{
	unsigned char i = 0;
	srand((unsigned int)time(NULL));

	for (i = 0; i < k; i++)
	{
		int iRand = rand() % g_data.height;
		int jRand = rand() % g_data.width;
		copyData(&(g_group + i)->mean, &g_data.data[iRand][jRand]);
	}
}

// base
void setGroup()
{
	unsigned int i = 0, j = 0, k = 0;

	for (i = 0; i < g_data.height; i++)
		for (j = 0; j < g_data.width; j++)
		{
			double distances[32];
			for (k = 0; k < g_K; k++) distances[k] = getDistance(&g_data.data[i][j], &g_group[k].mean);
			int index = findSmallestElement(distances, g_K);
			g_group[index].segment.i[g_group[index].segment.size] = i;
			g_group[index].segment.j[g_group[index].segment.size] = j;
			copyData(&g_group[index].segment.pixel[g_group[index].segment.size], &g_data.data[i][j]);
			g_group[index].segment.size++;
		}
}

void printBGR(BGR* data)
{
	printf("%d %d %d\n", data->blue, data->green, data->red);
}

void copyData(BGR* destionation, const BGR* source)
{
	(*destionation).blue = (*source).blue;
	(*destionation).green = (*source).green;
	(*destionation).red = (*source).red;
}

double getDistance(BGR* color1, BGR* color2)
{
	int blue = abs(color1->blue - color2->blue);
	int green = abs(color1->green - color2->green);
	int red = abs(color1->red - color2->red);
	return sqrt(blue * blue + green * green + red * red);
}

int findSmallestElement(double* distances, int size)
{
	int i = 0;
	double smallest = *distances;
	int index = 0;
	for (i = 1; i < size; i++)
		if (*(distances + i) < smallest)
		{
			smallest = *(distances + i);
			index = i;
		}
	return index;
}

void findAvarage(BGR* data)
{
	unsigned int i = 0, j = 0;

	for (i = 0; i < g_K; i++)
	{
		int blueSum = 0;
		int greenSum = 0;
		int redSum = 0;
		for (j = 0; j < g_group[i].segment.size; j++)
		{
			blueSum += g_group[i].segment.pixel[j].blue;
			greenSum += g_group[i].segment.pixel[j].green;
			redSum += g_group[i].segment.pixel[j].red;
		}
		if (blueSum != 0) (data + i)->blue = blueSum / g_group[i].segment.size;
		if (greenSum != 0) (data + i)->green = greenSum / g_group[i].segment.size;
		if (redSum != 0) (data + i)->red = redSum / g_group[i].segment.size;
	}
}

double getEpsilon(BGR* avarage)
{
	int i = 0;
	double epsilon = 0;
	for (i = 0; i < g_K; i++) epsilon += getDistance(&g_group[i].mean, avarage + i);
	return epsilon;
}

void setSegmentData()
{
	int i = 0, j = 0;
	for (i = 0; i < g_K; i++)
		for (j = 0; j < g_group[i].segment.size; j++)
			copyData(&g_data.data[g_group[i].segment.i[j]][g_group[i].segment.j[j]], &g_group[i].mean);
}
