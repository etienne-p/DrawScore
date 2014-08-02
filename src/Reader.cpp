//
//  Reader.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-24.
//
//

#include "Reader.h"

void Reader::setup(int num){
    
    numLines = num;
    
    error = "default";
    
    camWidth = 320;	// try to grab at this size.
	camHeight = 240;
    
    // init zone of interest
    cropRect.x = (camWidth - 12) / 2;
    cropRect.y = 0;
    cropRect.width = 12;
    cropRect.height = camHeight;
    hSumValues = new float[(int) cropRect.height];
    
    // init regulator
    regulator.output = 128;
    regulator.minOutput = 0;
    regulator.maxOutput = 255;
    regulator.gain = 1;
    regulator.setPoint = 1;
    regulator.error = 0;
    regulationActive = true;
    
    triggerThreshold = 0.5;
    
    maxVariance = 0.05;
    
    maxAverageWeight = 0.05;
    
    triggerWeightThreshold = 0;
    
    // select device
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ) cout << endl;
        else cout << " - unavailable " << endl;
	}
    
    // TODO: select back camera
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight);
    videoTexture.allocate(cropRect.width,cropRect.height, GL_RGB);
}

FrameStatus Reader::update(){
    
    vidGrabber.update();
    
	if (!vidGrabber.isFrameNew()) return NONE;
    
    // we assume the frame is valid
    // we will perform a serie of test on it,
    // it will be interrupted in case of failure
    FrameStatus returnValue = VALID;
    error = "no error";
    
    float regulationValue = 1;
    
    // image processing
    unsigned char * pixels = vidGrabber.getPixels();
    unsigned char * cropped = crop(pixels, camWidth ,camHeight, cropRect.x, cropRect.y, cropRect.width, cropRect.height);
    unsigned char * gray = grayScale(cropped, cropRect.width, cropRect.height); // we switch to a 1 channel image
    unsigned char * thresholded = threshold(gray, cropRect.width, cropRect.height, regulator.output);
    unsigned char * expanded = oneToThreeChannels(thresholded, cropRect.width, cropRect.height); // and back to a 3 channels image
    videoTexture.loadData(expanded, cropRect.width, cropRect.height, GL_RGB);
    yAxisHistogram(thresholded, hSumValues, cropRect.width, cropRect.height); // sum values horizontaly
    
    vector<float> thresholdCrossing; // detect threshold crossings
    float prevVal = 0; // so we store a crossing in case of black border
    
    for(int i = 0, len = (int) cropRect.height; i < len; ++i){
        
        float val = (255 - hSumValues[i]) / 255; // [0, 1]
        
        if ((prevVal <= triggerThreshold && val > triggerThreshold) ||
            (prevVal >= triggerThreshold && val < triggerThreshold)) {
            thresholdCrossing.push_back((float) i / (len - 1));
        }
        prevVal = val;
    }
    
    if (prevVal > triggerThreshold) thresholdCrossing.push_back(1); // last crossing
    
    // detect extremely rare cases (val == threshold)
    if (thresholdCrossing.size() % 2 != 0) {
        error = "[INVALID] odd number of crossings: " + ofToString(thresholdCrossing.size()) ;
        returnValue = INVALID;
    }
    
    // extract triggers
    if (returnValue == VALID){
        
        triggers.clear();
        averageWeight = 0;
        
        for(int i = 0, len = thresholdCrossing.size(); i < len; i = i + 2){
            
            float d = thresholdCrossing[i + 1] - thresholdCrossing[i];
            
            Trigger trigger;
            trigger.position = thresholdCrossing[i] + d * 0.5;
            trigger.weight = d;
            averageWeight += d;
            triggers.push_back(trigger);
        }
        
        averageWeight /= triggers.size();
        
        if (averageWeight > maxAverageWeight) { // check average weight
            error = "[INVALID] above max average weight";
            regulationValue += 0.1;
            returnValue = INVALID;
        } else if (triggers.size() != numLines) { // verify the number of lines
            error = "[INVALID] wrong number of lines";
            regulationValue += triggers.size() > numLines ? 0.1 : -0.1;
            returnValue = INVALID;
        }
    }
    
    // check line space
    if (returnValue == VALID){
        
        // control line spacing with variance
        float averageSpaceBetweenTriggers = 0;
        float deltas[triggers.size() - 1];
        for(int i = 0, len = triggers.size() - 1; i < len; ++i){
            deltas[i] = triggers[i + 1].position - triggers[i].position;
            averageSpaceBetweenTriggers += deltas[i] / (len - 2);
        }
        
        variance = 0;
        for(int i = 0, len = triggers.size() - 1; i < len; ++i){
            float d = deltas[i] - averageSpaceBetweenTriggers;
            variance += d * d / (len * averageSpaceBetweenTriggers);
        }
        
        if (variance > maxVariance) {
            error = "[INVALID] high variance";
            returnValue = INVALID;
        }
        
    }

    if (regulationActive && regulationValue != 1) regulator.update(regulationValue);
    
    delete[] cropped;
    delete[] gray;
    delete[] thresholded;
    delete[] expanded;
    
    return returnValue;
}

void Reader::getTriggers(vector<int> &v){
    //hack, let's consider that top and bottom line are calibration lines
    triggerWeightThreshold = 1.5 * (0.5 * (triggers.front().weight + triggers.back().weight));
    for(int i = 0, len = triggers.size() - 1; i < len; ++i){
        v[i] = triggers[i].weight > triggerWeightThreshold ? 1 : 0;
    }
}

// used for debugging, Reader won't have a UI in production
void Reader::draw(){
    
    // draw source & processed capture
	ofSetColor(255, 255, 255);
	int cx = 250;
	videoTexture.draw(cx,0,cropRect.width,cropRect.height);
        // draw histogram
    ofNoFill();
	ofBeginShape();
    for(int i = 0, len = (int) cropRect.height; i < len; ++i){
		ofVertex(cx + hSumValues[i], i);
	}
	ofEndShape(false);
    
    // draw triggers
    ofSetColor(0, 255, 0);
    ofFill();
    for(int i = 0, len = triggers.size(); i < len; ++i){
        ofCircle(cx + 255, cropRect.height * triggers[i].position, triggers[i].weight * cropRect.height);
    }
    
    // print data
    ofSetColor(255, 0, 0);
    cx += cropRect.width;
    ofDrawBitmapString("size: " + ofToString(triggers.size()), cx, 10);
    ofDrawBitmapString("output: " + ofToString(regulator.output), cx, 30);
    ofDrawBitmapString("setPoint: " + ofToString(regulator.setPoint), cx, 50);
    ofDrawBitmapString("variance: " + ofToString(variance), cx, 70);
    ofDrawBitmapString("average weight: " + ofToString(averageWeight), cx, 90);
    ofDrawBitmapString("error: " + error, cx, 110);
}

Reader::~Reader(){
    delete[] hSumValues;
}
