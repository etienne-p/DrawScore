//
//  Reader.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-24.
//
//

#include "Reader.h"

void Reader::setup(int num){
    
    setNumLines(num);
    
    camWidth = 320;	// try to grab at this size.
	camHeight = 240;
    
    // init zone of interest
    cropRect.x = (camWidth - 20) / 2;
    cropRect.y = 0;
    cropRect.width = 20;
    cropRect.height = camHeight;
    hSumValues = new float[(int) cropRect.height];
    
    // init regulator
    regulator.output = 128;
    regulator.minOutput = 0;
    regulator.maxOutput = 255;
    regulator.gain = 1;
    regulationActive = true;
    
    triggerThreshold = 0.5;
    minTrigWidth = 2;
    maxTrigWidth = 40;
    
    maxVariance = 0.15;
    
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

void Reader::setNumLines(int num){
    numLines = num;
    regulator.setPoint = numLines - 0.1;
    regulator.error = numLines; //high to make sure we have a proper regulation phase
}

int Reader::getNumLines(){ return numLines; }

int Reader::update(){
    
    vidGrabber.update();
    
	if (!vidGrabber.isFrameNew()) return FRAME_NONE;
    
    // process image
    unsigned char * pixels = vidGrabber.getPixels();
    
    unsigned char * cropped = crop(pixels, camWidth ,camHeight, cropRect.x, cropRect.y, cropRect.width, cropRect.height);
    
    // we switch to a 1 channel image
    unsigned char * gray = grayScale(cropped, cropRect.width, cropRect.height);
    
    unsigned char * thresholded = threshold(gray, cropRect.width, cropRect.height, regulator.output);
    
    // and back to a 3 channels image
    unsigned char * expanded = oneToThreeChannels(thresholded, cropRect.width, cropRect.height);
    
    videoTexture.loadData(expanded, cropRect.width, cropRect.height, GL_RGB);
    
    // sum values horizontaly
    yAxisHistogram(thresholded, hSumValues, cropRect.width, cropRect.height);
    
    // detect lines
    triggers.clear();
    int triggerCount = 0;
    
    for(int i = 0, len = (int) cropRect.height; i < len; ++i){
        
        float val = (255 - hSumValues[i]) / 255; // [0, 1]
        bool hit = true;
        if (val > triggerThreshold) triggerCount++;
        else hit = false;
        
        // for regulation, large triggers must be splitted in several triggers
        // so we know that we are detcting TOO MUCH triggers, and should increase
        // the threshold
        if (!hit || triggerCount > maxTrigWidth){
            if (triggerCount > minTrigWidth){
                Trigger trigger;
                trigger.position = (i - triggerCount * 0.5) / len;
                trigger.weight = triggerCount / len;
                triggers.push_back(trigger);
            }
            triggerCount = 0;
        }
    }
    
    int returnCode = FRAME_INVALID;
    
    if (triggers.size() == numLines){
        
        // and lines are regularly spaced
        float averageSpaceBetweenTriggers = 0;
        float deltas[triggers.size() - 1];
        for(int i = 1, len = triggers.size(); i < len; ++i){
            deltas[i] = triggers[i].position - triggers[i - 1].position;
            averageSpaceBetweenTriggers += deltas[i] / (len - 2);
        }
        
        variance = 0;
        for(int i = 0, len = triggers.size() - 1; i < len; ++i){
            float d = deltas[i] - averageSpaceBetweenTriggers;
            variance += d * d / (len * averageSpaceBetweenTriggers);
        }
        
        if (variance < maxVariance) returnCode = FRAME_VALID;
    }
    
    if (regulationActive) regulator.update(triggers.size());
    
    delete[] cropped;
    delete[] gray;
    delete[] thresholded;
    delete[] expanded;
    
    return returnCode;
}

void Reader::getTriggers(vector<int> &v){
    //hack, let's consider that top and bottom line are calibration lines
    //triggerWeightThreshold = 1.5 * (0.5 * (triggers[0].weight + triggers[triggers.size() - 1].weight));
    for(int i = 0, len = triggers.size() - 1; i < len; ++i){
        //v[i] = triggers[i].weight > triggerWeightThreshold ? 1 : 0;
        v[i] = ofRandom(0, 1) > .5 ? 1 : 0;

    }
}

// used for debugging, Reader won't have a UI in production
void Reader::draw(){
    
    // draw source & processed capture
	ofSetColor(255, 255, 255);
	int cx = 250;
	videoTexture.draw(cx,0,cropRect.width,cropRect.height);
    
    // print data
    ofSetColor(255, 0, 0);
    cx += cropRect.width;
    ofDrawBitmapString("size: " + ofToString(triggers.size()), cx, 10);
    ofDrawBitmapString("output: " + ofToString(regulator.output), cx, 30);
    ofDrawBitmapString("setPoint: " + ofToString(regulator.setPoint), cx, 50);
    ofDrawBitmapString("variance: " + ofToString(variance), cx, 70);
    
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
}

Reader::~Reader(){
    delete[] hSumValues;
}
