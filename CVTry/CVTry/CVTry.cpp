#include "pch.h"
#include "opencv2/highgui.hpp"
#include "opencv2/aruco.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;
using namespace aruco;

/*
void detectMarkers(InputArray image, const Ptr<Dictionary> &dictionary, OutputArrayOfArrays corners, OutputArray ids);

void detectMarkers(InputArray image, const Ptr<Dictionary> &dictionary, OutputArrayOfArrays corners, OutputArray ids) {

	Mat gray;
	
	
}*/

int main(int argc, char *argv[]) {
	Ptr<Dictionary> dictionary = getPredefinedDictionary(DICT_ARUCO_ORIGINAL);
	Ptr<DetectorParameters> detectorParams = DetectorParameters::create();
	Mat camMatrix, distCoeffs;

	camMatrix = (Mat_<double>(3, 3) << 576.88606, 0, 326.23928, 0, 578.34444, 247.88512, 0, 0, 1);
	distCoeffs = (Mat_<double>(1, 4) << 0.06108, -0.11259, -0.00508, 0.00425);

	float markerLength = 200;

	VideoCapture inputVideo;
	inputVideo.open(0);
	// Accessing WebCam over http stream
	//inputVideo.open("http://10.68.74.72:8080/videofeed");

	while (inputVideo.grab()) {
		Mat image, imageCopy;
		inputVideo.retrieve(image);

		vector<int> ids;
		vector<vector<Point2f>> corners, rejected;
		vector<Vec3d> rvecs, tvecs;

		detectMarkers(image, dictionary, corners, ids, detectorParams, rejected);

		image.copyTo(imageCopy);

		if (ids.size() > 0) {
			drawDetectedMarkers(imageCopy, corners, ids);
			estimatePoseSingleMarkers(corners, markerLength, camMatrix, distCoeffs, rvecs, tvecs);

			putText(imageCopy, "MARKER DETECTED! Custom value is: " + to_string(ids.at(0)), Point(imageCopy.cols / 4, imageCopy.rows - 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 0));

			for (int i = 0; i < ids.size(); i++)
			{
				drawAxis(imageCopy, camMatrix, distCoeffs, rvecs[i], tvecs[i], markerLength * 0.5f);
			}

			/*
			for (vector<Point2f> p : corners) {
				line(imageCopy, p.at(0), p.at(1), Scalar(255, 255, 0), 2);
				line(imageCopy, p.at(0), p.at(3), Scalar(255, 255, 0), 2);

				//line(imageCopy, p.at(0), p.at(2), Scalar(255, 255, 0), 2);
				//line(imageCopy, p.at(1), p.at(3), Scalar(255, 255, 0), 2);

				line(imageCopy, p.at(2), p.at(1), Scalar(255, 255, 0), 2);
				line(imageCopy, p.at(2), p.at(3), Scalar(255, 255, 0), 2);

				putText(imageCopy, to_string(ids.at(0)), Point(p.at(0).x + ((p.at(1).x - p.at(0).x) / 2), (p.at(3).y)), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);

				//rectangle(imageCopy, p.at(0), p.at(1), Scalar(255, 255, 0), 2);

				float w = (p.at(1).x - p.at(0).x) / 7;
				float h = (p.at(3).y - p.at(0).y) / 7;

				cout << w << " " << h << endl;

				//float matr[5][5];
				
				// vertical line
				line(imageCopy, Point(p.at(0).x + (1 * w), p.at(0).y), Point(p.at(3).x + (1 * w), p.at(3).y), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x + (2 * w), p.at(0).y), Point(p.at(3).x + (2 * w), p.at(3).y), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x + (3 * w), p.at(0).y), Point(p.at(3).x + (3 * w), p.at(3).y), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x + (4 * w), p.at(0).y), Point(p.at(3).x + (4 * w), p.at(3).y), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x + (5 * w), p.at(0).y), Point(p.at(3).x + (5 * w), p.at(3).y), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x + (6 * w), p.at(0).y), Point(p.at(3).x + (6 * w), p.at(3).y), Scalar(255, 255, 0));
				
				// horizontal line
				line(imageCopy, Point(p.at(0).x, p.at(0).y + (1 * h)), Point(p.at(1).x, p.at(1).y + (1 * h)), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x, p.at(0).y + (2 * h)), Point(p.at(1).x, p.at(1).y + (2 * h)), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x, p.at(0).y + (3 * h)), Point(p.at(1).x, p.at(1).y + (3 * h)), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x, p.at(0).y + (4 * h)), Point(p.at(1).x, p.at(1).y + (4 * h)), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x, p.at(0).y + (5 * h)), Point(p.at(1).x, p.at(1).y + (5 * h)), Scalar(255, 255, 0));
				line(imageCopy, Point(p.at(0).x, p.at(0).y + (6 * h)), Point(p.at(1).x, p.at(1).y + (6 * h)), Scalar(255, 255, 0));

			}
			*/
		}

		if (rejected.size() > 0) {
			drawDetectedMarkers(imageCopy, rejected, noArray(), Scalar(100, 0, 255));
		}

		//cvtColor(image, image, COLOR_BGR2GRAY);
		//Mat dst;
		//threshold(image, dst, 125, 255, THRESH_BINARY | THRESH_OTSU);

		//Mat c;
		//findContours(imageCopy, c, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		putText(imageCopy, "Aruco Marker | OpenCV | Rych Emrycho", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);
		//imshow("IMAGE Aruco Marker Detector OpenCV", dst);
		imshow("Aruco Marker Detector OpenCV", imageCopy);
		char key = (char)waitKey(30);
		if (key == 27) {
			break;
		}
	}

	return 0;
}