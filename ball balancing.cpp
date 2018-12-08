#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
//#include "SerialPort.h"

#include <string>
#include <sstream>

using namespace cv;
using namespace std;



//char port[] = "\\\\.\\COM3";
//char output[MAX_DATA_LENGTH];
//char incoming[MAX_DATA_LENGTH];


int main(int argc, char** argv) {

	//SerialPort arduino(port);
	//if (arduino.isConnected()) {
	//	cout << "port connection achieved" << endl;
	//}

	VideoCapture cap(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}
	
	namedWindow("Control", 1); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)  
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	//while (arduino.isConnected()) {
		while (true)
		{
			Mat imgOriginal;

			bool bSuccess = cap.read(imgOriginal); // read a new frame from video

			if (!bSuccess) //if not success, break loop
			{
				cout << "Cannot read a frame from video stream" << endl;
				break;
			}

			Mat imgHSV;

			cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //cvtColour(input_matrix_vecotr, output_matrix_vecotr, transition_type);

			Mat imgThresholded;

			inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image with the HSV vals

			//alternatively, define colour matrix for inRange()

		   //morphological opening (remove small objects from the foreground)
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));    //noise cleanup

			//morphological closing (fill small holes in the foreground)
			dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			namedWindow("Thresholded Image", 1);
			namedWindow("Original", 1);


			imshow("Original", imgOriginal); //show the original image

			///////////////////Object tracking//////////////////


			Moments omoment = moments(imgThresholded);                         //compute moments

			Point p(omoment.m10 / omoment.m00, omoment.m01 / omoment.m00);    //m10 =  dx axis, m01 = dy axis, m00 = dArea

			int xcoord = (omoment.m10 / omoment.m00);
			cout << xcoord << endl; //print out coordiantes
			circle(imgThresholded, p, 5, Scalar(128, 0, 0), -1);

			imshow("Thresholded Image", imgThresholded); //show the thresholded image
			//string XCOORD;
			//cin >> XCOORD;

			//stringstream ss;                                         //num2str bascially
			//ss << xcoord << endl;									//converts coordintaes to a string value
			//string XCOORD = ss.str();								//define XCOORD to be a string of the coordinates	
			

			//char *Xcoord = new char[XCOORD.size() + 1];		//dynamic memory alocation to store the string
			//copy(XCOORD.begin(), XCOORD.end(), Xcoord);   //copies XCOORD and puts into Xcoord
			//Xcoord[XCOORD.size()] = '\n';						//add delimiter at the end of array

			//arduino.writeSerialPort(Xcoord, MAX_DATA_LENGTH);	// pass the string to arduino.
			//arduino.readSerialPort(output, MAX_DATA_LENGTH);    //read the string from arduino.
			

			//cout << output;
			//delete[] Xcoord;	//delete memory


			if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			{
				cout << "esc key is pressed by user" << endl;
				break;
			}



		//}
	}
	
}