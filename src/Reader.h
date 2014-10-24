//
//  Reader.h
//  DrawScore
//
//  Created by etienne cella on 2014-07-24.
//
//

#ifndef DrawScore_Reader_h
#define DrawScore_Reader_h

#include "ImgProcess.h"
#include "Regulator.h"
#include "ofMain.h"

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
    void draw(ofRectangle bounds, ofTrueTypeFont * font);
    void getTriggers(vector<int> &v);

    ~Reader();
    
    int numLines;
    float triggerThreshold;
    bool regulationActive;
    float maxVariance;
    float maxAverageWeight;
    
private:
    
    vector<Trigger> triggers;
    
    string error;
    
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
    float averageWeight;

    float triggerWeightThreshold;
};

#endif
