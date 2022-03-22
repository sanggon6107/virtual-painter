#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>

using namespace std;
using namespace cv;

class VirtualPainter
{
private:
	VirtualPainter(vector<vector<int>> my_colors, vector<Scalar> pen_colors);
	VirtualPainter(const VirtualPainter&) = delete;
	void operator=(const VirtualPainter&) = delete;

public:
	void GetContours(Mat mask, vector<Point>& detected_points);

	void FindColor();

	void DrawOnCanvas();

	void Start();

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