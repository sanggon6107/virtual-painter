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




class VirtualPainter
{
private :
	VirtualPainter(vector<vector<int>> my_colors, vector<Scalar> pen_colors) : my_colors_(my_colors), pen_colors_(pen_colors) {}
	VirtualPainter(const VirtualPainter& painter) = delete;

public:
	void GetContours(Mat mask, vector<Point>& detected_points)
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

				detected_points.emplace_back(bound_rect[point].x + (bound_rect[point].width / 2),
					bound_rect[point].y + (bound_rect[point].height / 2));

				drawContours(img_, con_poly, point, Scalar(255, 0, 255), 2);
				rectangle(img_, bound_rect[point].tl(), bound_rect[point].br(), Scalar(0, 255, 0), 5);
			}
		}
	}

	void FindColor(Mat img)
	{
		Mat imgHSV;
		Mat mask;
		cvtColor(img, imgHSV, COLOR_BGR2HSV);


		for (int color = 0; color < my_colors_.size(); color++)
		{
			Scalar lower(my_colors_[color][static_cast<int>(HSV::HMIN)],
				my_colors_[color][static_cast<int>(HSV::SMIN)],
				my_colors_[color][static_cast<int>(HSV::VMIN)]);

			Scalar upper(my_colors_[color][static_cast<int>(HSV::HMAX)],
				my_colors_[color][static_cast<int>(HSV::SMAX)],
				my_colors_[color][static_cast<int>(HSV::VMAX)]);


			inRange(imgHSV, lower, upper, mask);
			imshow(to_string(color), mask);

			vector<Point> detected_points;

			GetContours(mask, detected_points);

			if (detected_points.size() != 0)
			{
				for (auto point : detected_points)
				{
					drawing_points_.push_back({ move(point.x), move(point.y), color });
				}
			}
		}
	}

	void DrawOnCanvas(vector<vector<int>>& drawing_points, vector<Scalar>& pen_colors_)
	{
		for (int point = 0; point < drawing_points.size(); point++)
		{
			circle(img_, Point(drawing_points[point][static_cast<int>(POINT_ELEMENTS::X)],
				drawing_points[point][static_cast<int>(POINT_ELEMENTS::Y)]),
				10,
				pen_colors_[drawing_points[point][static_cast<int>(POINT_ELEMENTS::COLOR)]],
				FILLED);
		}
	}

	void Start()
	{
		VideoCapture cap(0);

		while (true)
		{
			cap.read(img_);
			FindColor(img_);
			DrawOnCanvas(drawing_points_, pen_colors_);
			imshow("image", img_);
			if (waitKey(30) >= 0) break;
		}
		destroyAllWindows();
	}

	static VirtualPainter& GetInstance() {
		static VirtualPainter painter({
			{ 17, 24, 114, 184, 178, 255 },   // yellow
			{ 117, 135, 66, 124, 125, 240 }   // purple
			},
			{ {0, 255, 255},	// yellow
			{255, 0, 255}		// purple
			});

		return painter;
	}

private:
	Mat img_;
	vector<vector<int>> drawing_points_;
	vector<vector<int>> my_colors_;
	vector<Scalar> pen_colors_;
};







int main()
{
	VirtualPainter& instance = VirtualPainter::GetInstance();

	instance.Start();
}