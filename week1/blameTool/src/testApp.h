#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
        // will probably be removed in favor of camWidth/camHeight
        // since I am no longer captureing anything.
        int captureWidth;
        int captureHeight;

        // Variables for webCam
        ofVideoGrabber 		vidGrabber;
        unsigned char * 	videoInverted;
        ofTexture			videoTexture;
        int 				camWidth;
        int 				camHeight;

		ofImage		image;
        ofImage     blameText;
		ofxCvHaarFinder finder;	 // object used to detect haar features
};
