#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <stdio.h>
#include <iostream>
#include<vector>

using namespace std;
using namespace cv;

void extract_frames(const string &videoFilePath, vector<Mat>& frames) {

	try {
		//open the video file
		VideoCapture cap(videoFilePath); // open the video file
		if (!cap.isOpened())  // check if we succeeded
			cout<<"Can not open Video file";

		//cap.get(CV_CAP_PROP_FRAME_COUNT) contains the number of frames in the video;
		for (int frameNum = 0; frameNum < cap.get(cv::CAP_PROP_FRAME_COUNT);frameNum++)
		{
			Mat frame;
			cap >> frame; // get the next frame from video
			frames.push_back(frame);
		}
	}
	catch (cv::Exception& e) {
		cerr << e.msg << endl;
		exit(1);
	}

}

/*
It saves a vector of frames into jpg images into the outputDir as 1.jpg,2.jpg etc where 1,2 etc represents the frame number
*/
void save_frames(vector<Mat>& frames, const string& outputDir) {
	vector<int> compression_params;
	vector<int> f;
	compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	ostringstream str1;
	int l = frames.size();
	string g = to_string(l);
	int x = g.length();
	int frameNumber = 1;
	for (std::vector<Mat>::iterator frame = frames.begin();frame != frames.end(); ++frame) {
		string filePath = outputDir + format("Frame_%0*d",x, frameNumber) +".jpg";
		imwrite(filePath, *frame, compression_params);
		frameNumber += 1;
	}
}

int main(){
	vector<Mat> frames;
	//path to extract frames from
	extract_frames("C:/Users/harsh/Downloads/acne.mp4", frames);
	//path to save frames to
	save_frames(frames, "C:/Users/harsh/Desktop/CFG/");
	getchar();
	return 0;
}