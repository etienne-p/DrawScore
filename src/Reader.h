//
//  Reader.h
//  DrawScore
//
//  Created by etienne cella on 2014-07-24.
//
//

#ifndef DrawScore_Reader_h
#define DrawScore_Reader_h

#include "ofxUI.h"
#include "ImgProcess.h"
#include "Regulator.h"

using namespace ImgProcess;

typedef struct Trigger {
	float position;
	float weight;
} Trigger;

class Reader {
    
public:
    void setup(int num);
    void update();
    void exit();
    void draw();
    void setNumLines(int num);
    ~Reader();
    
private:
    
    int numLines;
    
    // UI related
    void guiEvent(ofxUIEventArgs &e);
    ofxUICanvas *gui;
    
    // cam capture
    ofVideoGrabber vidGrabber;
    ofTexture videoTexture;
    int camWidth;
    int camHeight;
    ofRectangle cropRect;
    
    // image processing
    Regulator regulator;
    bool regulationActive;
    float * hSumValues;
    
    float triggerThreshold;
    int minTrigWidth;
    int maxTrigWidth;
    
    float variance;
    float maxVariance;
    vector<Trigger> triggers;
    vector<float> memory;
    
};

#endif
