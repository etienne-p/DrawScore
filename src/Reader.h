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

enum FrameStatus { NONE, VALID, INVALID};

class Reader {
    
public:
    
    void setup(int num);
    FrameStatus update();
    void draw();
    void setNumLines(int num);
    int getNumLines();
    void getTriggers(vector<int> &v);

    ~Reader();
    
    float triggerThreshold;
    int minTrigWidth;
    int maxTrigWidth;
    bool regulationActive;
    float maxVariance;
    
private:
    
    int numLines;
    vector<Trigger> triggers;
    
    // cam capture
    ofVideoGrabber vidGrabber;
    ofTexture videoTexture;
    ofRectangle cropRect;
    int camWidth;
    int camHeight;
    
    // image processing
    Regulator regulator;
    float * hSumValues;
    float variance;
    
    float triggerWeightThreshold;
};

#endif
