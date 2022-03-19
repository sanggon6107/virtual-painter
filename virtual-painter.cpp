#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

enum HSV {HMIN = 0, HMAX = 1, SMIN = 2, SMAX = 3, VMIN = 4, VMAX = 5};

// hmin 17, hmax 24, smin 114, smax 184, vmin 178, vmax 255
vector<vector<int>> my_colors{ {17, 24, 114, 184, 178, 255} };
vector<vector<int>> pen_colors{ {255, 255, 0} };


void FindColor(Mat img)
{
	Mat imgHSV;
	Mat mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	
	
	for (int i = 0; i < my_colors.size(); i++)
	{
		Scalar lower(my_colors[i][HSV::HMIN], my_colors[i][HSV::SMIN], my_colors[i][HSV::VMIN]);
		Scalar upper(my_colors[i][HSV::HMAX], my_colors[i][HSV::SMAX], my_colors[i][HSV::VMAX]);

		inRange(imgHSV, lower, upper, mask);
		imshow(to_string(i), mask);
	}
	
}


int main()
{
	VideoCapture cap(0);
	Mat img;

	while (true)
	{
		cap.read(img);
		FindColor(img);

		imshow("image", img);
		if (waitKey(30) >= 0) break;
	}
	destroyAllWindows();
}