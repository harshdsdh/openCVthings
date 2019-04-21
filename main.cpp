#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core/types_c.h>
#include "opencv2/opencv.hpp"
#include <iostream>

#include "VideoFaceDetector.h"
using namespace std;
using namespace cv;
const cv::String    WINDOW_NAME("Camera video");
const cv::String    CASCADE_FILE("C:/Users/harsh/source/repos/opencv_template/haarcascade_frontalface_default.xml");

int main(int argc, char** argv)
{
	// Try opening camera
	//cv::VideoCapture camera(0);
	cv::VideoCapture camera("C:/Users/harsh/Desktop/v45_112_Life_Of_Pi_Lying_Actress_tilts_her_head_talking.mp4");
	int h = camera.get(cv::CAP_PROP_FRAME_HEIGHT);
	int w = camera.get(cv::CAP_PROP_FRAME_WIDTH);
	int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');
	VideoWriter video("outcpp1.avi", codec, 10, Size(w, h));
	if (!camera.isOpened()) {
		fprintf(stderr, "Error getting camera...\n");
		exit(1);
	}

	cv::namedWindow(WINDOW_NAME, cv::WINDOW_KEEPRATIO | cv::WINDOW_AUTOSIZE);

	VideoFaceDetector detector(CASCADE_FILE, camera);
	cv::Mat frame;
	double fps = 0, time_per_frame;
	while (true)
	{
		auto start = cv::getCPUTickCount();
		detector >> frame;
		auto end = cv::getCPUTickCount();

		time_per_frame = (end - start) / cv::getTickFrequency();
		fps = (15 * fps + (1 / time_per_frame)) / 16;

		printf("Time per frame: %3.3f\tFPS: %3.3f\n", time_per_frame, fps);

		if (detector.isFaceFound())
		{
			cv::rectangle(frame, detector.face(), cv::Scalar(255, 0, 0));
			//cv::circle(frame, detector.facePosition(), 30, cv::Scalar(0, 255, 0));
		}
		video.write(frame);
		cv::imshow(WINDOW_NAME, frame);
		if (cv::waitKey(25) == 27) break;
	}

	return 0;
}