#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

#include <stdio.h>

using namespace std;
using namespace cv;

const string WindowName = "Face Detection example";

class CascadeDetectorAdapter : public DetectionBasedTracker::IDetector
{
public:
	CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector) :
		IDetector(),
		Detector(detector)
	{
		CV_Assert(detector);
	}

	void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) CV_OVERRIDE
	{
		Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
	}

	virtual ~CascadeDetectorAdapter() CV_OVERRIDE
	{}

private:
	CascadeDetectorAdapter();
	cv::Ptr<cv::CascadeClassifier> Detector;
};

int main(int, char**)
{
	namedWindow(WindowName);
	VideoCapture VideoStream("C:/Users/harsh/Desktop/v45_112_Life_Of_Pi_Lying_Actress_tilts_her_head_talking.mp4");
	//VideoCapture VideoStream(0);
	int h = VideoStream.get(cv::CAP_PROP_FRAME_HEIGHT);
	int w = VideoStream.get(cv::CAP_PROP_FRAME_WIDTH);
	int codec = VideoWriter::fourcc('M', 'J', 'P', 'G');
	VideoWriter video("outcpp_inital_faciallandmarks.avi", codec, 10, Size(w, h));
	if (!VideoStream.isOpened())
	{
		printf("Error: Cannot open video stream from camera\n");
		return 1;
	}

	std::string cascadeFrontalfilename = samples::findFile("data/lbpcascades/lbpcascade_frontalface.xml");
	cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
	cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);
	if (cascade->empty())
	{
		printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
		return 2;
	}

	cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
	cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);
	if (cascade->empty())
	{
		printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
		return 2;
	}

	DetectionBasedTracker::Parameters params;
	DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

	if (!Detector.run())
	{
		printf("Error: Detector initialization failed\n");
		return 2;
	}

	Mat ReferenceFrame;
	Mat GrayFrame;
	vector<Rect> Faces;

	do
	{
		VideoStream >> ReferenceFrame;
		cvtColor(ReferenceFrame, GrayFrame, COLOR_BGR2GRAY);
		Detector.process(GrayFrame);
		Detector.getObjects(Faces);

		for (size_t i = 0; i < Faces.size(); i++)
		{
			rectangle(ReferenceFrame, Faces[i], Scalar(0, 255, 0));
		}
		video.write(ReferenceFrame);
		imshow(WindowName, ReferenceFrame);
	} while (waitKey(30) < 0);

	Detector.stop();

	return 0;
}
