//============================================================================
// Name        : nostalgiaPic.cpp
// Author      : ade
// Version     :
// Copyright   : ade
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define		ALPHA_WEIGHT		0.6

int gSave = 0;

void resizePic(Mat &src, int width, int height)
{
	Mat dst;
	float scale1, scale2, scale;
	int new_width, new_height;
	scale1 = (float)width / src.cols;
	scale2 = (float)height / src.rows;

	scale = scale1 < scale2 ? scale1:scale2;
	if (scale1 < scale2)
	{
		new_width = width;
		new_height = src.rows * scale1;
	}
	else
	{
		new_width = src.cols * scale2;
		new_height = height;
	}
	// resize
	resize(src, dst, Size(0,0), scale, scale);

	src = dst;
}

void mixPic(Mat src1, Mat src2, Mat &dst)
{
	Rect roi;
	if (src1.cols == src2.cols)
	{
		roi=Rect(0, (src1.rows-src2.rows)/2, src2.cols, src2.rows);
	}
	else
	{
		roi=Rect((src1.cols-src2.cols)/2, 0, src2.cols, src2.rows);
	}
	// add weight
	addWeighted(src1(roi), ALPHA_WEIGHT, src2, 1-ALPHA_WEIGHT, 0, dst);
}

void onMouse(int event, int x, int y, int, void*)
{
	switch(event)
	{
	case CV_EVENT_LBUTTONUP:				// left click
		break;
	case CV_EVENT_LBUTTONDBLCLK:			// left double click
		// save picture
		gSave = 1;							// start tracking
		break;
	}
}

int main(int argc, char* argv[]) {

	VideoCapture cap;
	Mat frame, oldPic, dst;
	int width, height;

	if (argc != 2)
	{
		cout << "Usage: nostalgiaPic  org_picture." << endl;
		return 1;
	}
	cap.open(0);
	if (!cap.isOpened())
	{
		cout << "Open camera failed!" << endl;
		return 2;
	}

	namedWindow("camera", 0);
	setMouseCallback("camera", onMouse, 0);

	// get orignal picture
	oldPic = imread(argv[1]);
	// get camera width and height
	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	// resize the old pic to fix the camera
	resizePic(oldPic, width, height);

	while (1)
	{
		cap >> frame;

		mixPic(frame, oldPic, dst);
		imshow("camera", dst);

		if (gSave == 1)
		{
			imwrite("test.jpg", frame);
			gSave = 0;
		}
		waitKey(20);
	}

	return 0;
}
