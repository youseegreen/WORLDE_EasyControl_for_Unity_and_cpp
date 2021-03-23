#include "MidiControllerAPI.h"
#ifndef UNITY_MODE

// option
#include "OpenCV.h"
#include "UDP.h"

#include <iostream>
#include <windows.h>


int main() {

	// Open the Midi Controller
	OpenMidiController();

	// Prevent the chattering
	SetButtonOnInterval(100);

	// variables
	float slider[9] = { 0 };
	bool button[4] = { 0 };
	bool play = false;

	while (!play) {
		
		// get the controllers states
		GetSliderValues(slider);
		GetButtonStates(button, 0, 4);
		play = Get1SystemButtonState("PLAY");


		// show the states

		// slider
		for (int i = 0; i < 9; i++) {
			if (slider[i] == -1)printf("undefined ");
			else printf("%.2f ", slider[i]);
		}
		printf("\n");

		// button
		for (int i = 0; i < 4; i++) {
			printf("%s ", button[i] ? "on" : "off");
		}
		printf("\n");

		// play button
		if(play)
			printf("play button is pushed!\n");


		// if OpenCV can be used...
		// the following code vizualize the slider values
		using namespace cv;
		int width = 100;
		int height = 500;
		Mat img = Mat(Size(width * 9, height), CV_8UC3, Scalar(255, 255, 255));
		for (int i = 0; i < 9; i++) {
			rectangle(img, Rect(i * width + 2, (int)(height - slider[i] * (height - 1) - 1), width - 2, slider[i] * (height - 1)), Scalar(0, 0, 255), -1);
		}
		imshow("midi slider", img);
		waitKey(1);


		// if you want to use UDP trans.
		// please use the following code
		static UDP udp("192.168.xx.y", 50000);
		std::string msg = "mc,9";
		for (int i = 0; i < 9; i++) msg += "," + std::to_string(slider[i]);
		msg += "end";
		udp.send(msg);


		Sleep(5);
	}

	CloseMidiController();

	return 1;
}

#endif 
