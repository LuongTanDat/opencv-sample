#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	cv::VideoCapture cap(0);

	while (1)
	{
		cv::Mat frame;
		cap >> frame;
		if (frame.empty())
			break;
		
		cv::flip(frame, frame, 1);
		cv::imshow("Frame", frame);

		char c = (char)cv::waitKey(25);
		if (c == 27) break;
	}
	cap.release();
	cv::destroyAllWindows();
	return 0;
}

// g++ main.cpp -o app -std=c++11 `pkg-config --cflags --libs opencv`