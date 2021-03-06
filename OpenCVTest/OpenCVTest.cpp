#include "stdafx.h"

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;

void detectAndDraw(Mat& img, CascadeClassifier& faceCascade,CascadeClassifier& eyeCascade, double scale);

string cascadeName, nestedCascadeName;

int main()
{
	VideoCapture capture; 
	Mat frame, image;
 
	CascadeClassifier cascade, nestedCascade;
	const double scale=1;
 
	nestedCascade.load("haarcascade_eye.xml") ;
 
	cascade.load("haarcascade_frontalface_default.xml") ; 
 
	capture.open(0); 
	if( capture.isOpened() )
	{
		cout << "Face Detection Started...." << endl;
		while(true)
		{
			capture >> frame;
			if( frame.empty() )
				break;
			auto frame1 = frame.clone();
			detectAndDraw( frame1, cascade, nestedCascade, scale );
			const auto c = static_cast<char>(waitKey(10));
		 
			if( c == 27 || c == 'q' || c == 'Q' ) 
				break;
		}
	}
	else
		cout<<"Could not Open Camera";
	return 0;
}

void detectAndDraw(Mat& img, CascadeClassifier& faceCascade,CascadeClassifier& eyeCascade, double scale)
{
	vector<Rect> faces, faces2;
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	const auto fx = 1 / scale;

	resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
	equalizeHist( smallImg, smallImg );

	faceCascade.detectMultiScale(smallImg, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

	for ( size_t i = 0; i < faces.size(); i++ )
	{
		const auto r = faces[i];
		vector<Rect> eyeObjects;
		Point center;
		const auto color = Scalar(255, 0, 0);

		rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
					cvPoint(cvRound((r.x + r.width-1)*scale), 
					cvRound((r.y + r.height-1)*scale)), color, 3, 8, 0);

		if(eyeCascade.empty())
			continue;

		eyeCascade.detectMultiScale(smallImg(r), eyeObjects, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) ); 
		 
		for ( size_t j = 0; j < eyeObjects.size(); j++ ) 
		{
			const auto nr = eyeObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
			center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
			const auto radius = cvRound((nr.width + nr.height)*0.25*scale);

			circle(img, center, radius, color, 3, 8, 0);
			cout << "Face Detected \n";
		}
	}
 
	imshow("Face Detection", img);
}

