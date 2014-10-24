//
//  Reader.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-24.
//
//

#include "Reader.h"

void Reader::setup(int num){
    
    regulator.output = 128;
    regulator.minOutput = 0;
    regulator.maxOutput = 255;
    regulator.gain = 1;
    regulator.setPoint = 1;
    regulator.error = 0;
    regulationActive = true;
    
    numLines = num;
    error = "";
    triggerThreshold = 0.5;
    maxVariance = 0.05;
    maxAverageWeight = 0.05;
    triggerWeightThreshold = 0;
    
    // select device
    /*
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ) cout << endl;
        else cout << " - unavailable " << endl;
	}
     */
    
    // TODO: select back camera
    vidGrabber.setVerbose(true);
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(640,480);
    
    camWidth = vidGrabber.width;	// try to grab at this size.
	camHeight = vidGrabber.height;
    
    cropRect.x = (camWidth - 12) / 2;
    cropRect.y = 0;
    cropRect.width = 12;
    cropRect.height = camHeight;
    hSumValues = new float[(int) cropRect.height];
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
    
    // clean
    delete[] cropped;
    delete[] gray;
    delete[] thresholded;
    delete[] expanded;
    
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
    
    //
    if (thresholdCrossing.size() % 2 != 0) {
        error = "[INVALID] odd number of crossings: " + ofToString(thresholdCrossing.size()) ;
        regulationValue += thresholdCrossing.size() > numLines * 2 ? 0.1 : -0.1;
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
    
    return returnValue;
}

void Reader::getTriggers(vector<int> &v){
    // let's consider that top and bottom line are calibration lines
    triggerWeightThreshold = 1.5f * (.5f * (triggers.front().weight + triggers.back().weight));
    for(int i = 0, len = triggers.size(); i < len; ++i){
        v[i] = triggers[i].weight > triggerWeightThreshold ? 1 : 0;
    }
}

void Reader::draw(ofRectangle bounds, ofTrueTypeFont * font){
    
	// draw image & graph
    ofSetColor(ofColor::white);
    ofPushMatrix();
    ofTranslate(bounds.position);
    ofScale(bounds.width / (cropRect.width + 255), bounds.height * .5f / cropRect.height);
    
	videoTexture.draw(0, 0, cropRect.width,cropRect.height);
    
    ofNoFill();
    ofSetColor(ofColor::gray);
    ofBeginShape();
    for(int i = 0, len = (int) cropRect.height; i < len; ++i){
		ofVertex(cropRect.width + hSumValues[i], i);
	}
	ofEndShape(false);
    
    ofSetColor(ofColor::white);
    float rectSide = 8.f;
    for(int i = 0, len = triggers.size(); i < len; ++i){
        float y = cropRect.height * triggers[i].position;
        ofNoFill();
        ofLine(cropRect.width, y, 255, y);
        ofFill();
        ofRect(cropRect.width + 255 - rectSide, y - rectSide * .5f, rectSide, rectSide);
    }
    
    ofPopMatrix();
    
    // text info
    ofSetColor(ofColor::gray);
    ofPushMatrix();
    ofTranslate(bounds.x, bounds.y + .5f * bounds.height);
    
    string info[12] = {
        "size:", ofToString(triggers.size()),
        "output:", ofToString(regulator.output),
        "setPoint:", ofToString(regulator.setPoint),
        "variance:", ofToString(variance),
        "average weight:", ofToString(averageWeight),
        error, ""
    };
    
    ofNoFill();
    for (int i = 0; i < 6; i++){
        float y = (1 + 2 * i) * font->getLineHeight();
        font->drawString(info[2 * i], 0, y);
        font->drawString(info[2 * i + 1], bounds.width - font->getStringBoundingBox(info[2 * i + 1], 0.f, 0.f).width, y);
        y += font->getLineHeight() * .4f;
        ofLine(0, y, bounds.width, y);
    }
    
    ofPopMatrix();
}

Reader::~Reader(){
    delete[] hSumValues;
}
