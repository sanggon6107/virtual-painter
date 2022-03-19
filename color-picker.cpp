#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main()
{

	VideoCapture cap(0);
	Mat img;
	Mat imgHSV;
	Mat mask;
	
	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;



	namedWindow("Trackbars", (440, 400));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);

	while (true)
	{
		cap.read(img);

		Scalar lower(hmin, smin, vmin); // hue(색상) minimum, saturation(채도) minimum, value(명도) minimum.
		Scalar upper(hmax, smax, vmax);

		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, lower, upper, mask);

		imshow("img", img);
		imshow("Image HSV", imgHSV);
		imshow("image mask", mask);

		if (waitKey(1) >= 0) break;
	}
	destroyAllWindows();
}