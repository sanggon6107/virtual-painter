#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;

vector<vector<int>> my_colors;

// hmin 17, hmax 24, smin 114, smax 184, vmin 178, vmax 255
void FindColor(Mat img)
{
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	
	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);
	inRange(imgHSV, lower, upper, mask);
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