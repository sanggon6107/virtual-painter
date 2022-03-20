#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <vector>


using namespace std;
using namespace cv;

enum class HSV {HMIN = 0, HMAX = 1, SMIN = 2, SMAX = 3, VMIN = 4, VMAX = 5};
enum class POINT_ELEMENTS {X = 0, Y = 1, COLOR = 2};

Mat img;
vector<vector<int>> drawing_points;


// hmin 17, hmax 24, smin 114, smax 184, vmin 178, vmax 255
vector<vector<int>> my_colors{ {17, 24, 114, 184, 178, 255}, // yellow
								{117, 135, 66, 124, 125, 240} }; // purple
vector<Scalar> pen_colors{ {0, 255, 255},   // yellow
								{255, 0, 255} }; // purple



void GetContours(Mat mask, Point& detected_point)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	
	findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> con_poly(contours.size());
	vector<Rect> bound_rect(contours.size());

	for (int point = 0; point < contours.size(); point++)
	{
		if (int area = contourArea(contours[point]);
						               area > 1000)
		{
			double peri = arcLength(contours[point], true);
			approxPolyDP(contours[point], con_poly[point], 0.02 * peri, true); 
			bound_rect[point] = boundingRect(con_poly[point]);

			detected_point.x = bound_rect[point].x + (bound_rect[point].width / 2);
			detected_point.y = bound_rect[point].y + (bound_rect[point].height / 2);


			drawContours(img, con_poly, point, Scalar(255, 0, 255), 2);
			rectangle(img, bound_rect[point].tl(), bound_rect[point].br(), Scalar(0, 255, 0), 5);
		}
	}

	

}


void FindColor(Mat img)
{
	Mat imgHSV;
	Mat mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	
	
	for (int color = 0; color < my_colors.size(); color++)
	{
		Scalar lower(my_colors[color][static_cast<int>(HSV::HMIN)],
					my_colors[color][static_cast<int>(HSV::SMIN)],
					my_colors[color][static_cast<int>(HSV::VMIN)]);

		Scalar upper(my_colors[color][static_cast<int>(HSV::HMAX)],
					my_colors[color][static_cast<int>(HSV::SMAX)],
					my_colors[color][static_cast<int>(HSV::VMAX)]);


		inRange(imgHSV, lower, upper, mask);
		imshow(to_string(color), mask);

		// structured binding
		Point detected_point(0, 0);
		GetContours(mask, detected_point);

		if (detected_point.x != 0 && detected_point.y != 0)
		{
			drawing_points.push_back({ move(detected_point.x), move(detected_point.y), color } );
		}
	}
}

void DrawOnCanvas(vector<vector<int>>& drawing_points, vector<Scalar>& pen_colors)
{
	for (int point = 0; point < drawing_points.size(); point++)
	{
		circle(img, Point(drawing_points[point][static_cast<int>(POINT_ELEMENTS::X)],
						  drawing_points[point][static_cast<int>(POINT_ELEMENTS::Y)]),
					10,
					pen_colors[drawing_points[point][static_cast<int>(POINT_ELEMENTS::COLOR)]], FILLED);
	}
}



int main()
{
	VideoCapture cap(0);
	

	while (true)
	{
		cap.read(img);
		FindColor(img);
		DrawOnCanvas(drawing_points, pen_colors);
		imshow("image", img);
		if (waitKey(30) >= 0) break;
	}
	destroyAllWindows();
}