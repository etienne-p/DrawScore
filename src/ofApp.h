#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "ImgProcess.h"
#include "Regulator.h"

using namespace ImgProcess;

typedef struct Trigger {
	float position;
	float weight;
} Trigger;

class ofApp : public ofBaseApp{

	public:
    
        void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        // UI related
        void exit();
        void guiEvent(ofxUIEventArgs &e);
        ofxUICanvas *gui;
    
        // cam capture
        ofVideoGrabber vidGrabber;
        ofTexture videoTexture;
        int camWidth;
        int camHeight;
        ofRectangle cropRect;
        Regulator regulator;
        bool regulationActive;
        float * hSumValues;
    
        float triggerThreshold;
        int minTrigWidth;
        int maxTrigWidth;

    
        vector<Trigger> triggers;
};
