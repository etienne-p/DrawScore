#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;
    
    // init zone of interest
    cropRect.x = (camWidth - 20) / 2;
    cropRect.y = 0;
    cropRect.width = 20;
    cropRect.height = camHeight;
   
    // init regulator
    regulator.setPoint = 3.9;
    regulator.output = 128;
    regulator.minOutput = 0;
    regulator.maxOutput = 255;
    regulator.gain = 1;
    regulator.error = 4; //high to make sure we have a proper regulation phase
    regulationActive = true;
    
    triggerThreshold = 0.5;
    minTrigWidth = 2;
    maxTrigWidth = 40;
    
    // setup gui
    gui = new ofxUICanvas();        //Creates a canvas at (0,0) using the default width
    gui->addToggle("REGULATION", regulationActive);
    gui->addSlider("SET_POINT", 0, 12, regulator.setPoint);
    gui->addSlider("TRIGGER_THRESHOLD", 0, 1, triggerThreshold);
    gui->addSlider("MIN_TRIG_WIDTH", 0, 10, minTrigWidth);
    gui->addSlider("MAX_TRIG_WIDTH", 0, 60, maxTrigWidth);


    gui->autoSizeToFitWidgets();
    gui->loadSettings("settings.xml");
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    
    //we can now get back a list of devices.
	vector<ofVideoDevice> devices = vidGrabber.listDevices();
    
    for(int i = 0; i < devices.size(); i++){
		cout << devices[i].id << ": " << devices[i].deviceName;
        if( devices[i].bAvailable ){
            cout << endl;
        }else{
            cout << " - unavailable " << endl;
        }
	}
    
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight);
    
    hSumValues = new float[(int) cropRect.height];
    
	videoTexture.allocate(cropRect.width,cropRect.height, GL_RGB);
	ofSetVerticalSync(true);
}


//--------------------------------------------------------------
void ofApp::update(){
    
	ofBackground(100,100,100);
    
	vidGrabber.update();
    
	if (vidGrabber.isFrameNew()){
        
        // process image
        unsigned char * pixels = vidGrabber.getPixels();
        
        unsigned char * cropped = crop(pixels, camWidth ,camHeight, cropRect.x, cropRect.y, cropRect.width, cropRect.height);
        // we switch to a 1 channel image
        unsigned char * gray = grayScale(cropped, cropRect.width, cropRect.height);

        unsigned char * thresholded = threshold(gray, cropRect.width, cropRect.height, regulator.output);
        
        // and back to a 3 channels image
        unsigned char * expanded = oneToThreeChannels(thresholded, cropRect.width, cropRect.height);
        
        videoTexture.loadData(expanded, cropRect.width, cropRect.height, GL_RGB);
        
        // average pixel color is used as a feedback value to regulate threshold
        float avr = averagePixelValue(thresholded, cropRect.width, cropRect.height);
        
        // sum values horizontaly
        yAxisHistogram(thresholded, hSumValues, cropRect.width, cropRect.height);
        
        // detect rectangles
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
                    trigger.weight = triggerCount;
                    triggers.push_back(trigger);
                }
                triggerCount = 0;
            }
        }
        
        //update memory if frame matches expected trigger count
        if (triggers.size() > 3){
        
            // compute average space between triggers
            float averageSpaceBetweenTriggers = 0;
            float deltas[triggers.size() - 1];
            for(int i = 1, len = triggers.size(); i < len; ++i){
                deltas[i] = triggers[i].position - triggers[i - 1].position;
                averageSpaceBetweenTriggers += deltas[i] / (len - 2);
            }
            
            // are triggers approximately equidistant (variance)?
            variance = 0;
            for(int i = 0, len = triggers.size() - 1; i < len; ++i){
                float d = deltas[i] - averageSpaceBetweenTriggers;
                variance += d * d / (len * averageSpaceBetweenTriggers);
            }
        }
         
        //
        if (regulationActive) regulator.update(triggers.size());
        
        // TODO: remove delete and observe memory leak.
        delete[] cropped;
        delete[] gray;
        delete[] thresholded;
        delete[] expanded;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw source & processed capture
	ofSetHexColor(0xffffff);
	vidGrabber.draw(20,20);
	videoTexture.draw(20+camWidth,20,cropRect.width,cropRect.height);
    
    // print data
    ofDrawBitmapString("gain: " + ofToString(regulator.gain), 400, 10);
    ofDrawBitmapString("out: " + ofToString(regulator.output), 400, 30);
    ofDrawBitmapString("size: " + ofToString(triggers.size()), 400, 50);
    ofDrawBitmapString("output: " + ofToString(regulator.output), 400, 80);
    ofDrawBitmapString("setPoint: " + ofToString(regulator.setPoint), 400, 110);
    ofDrawBitmapString("variance: " + ofToString(variance), 400, 140);

    
    // draw histogram
    ofSetColor(255, 0, 0);
	ofNoFill();
	ofBeginShape();
	for(int i = 0, len = (int) cropRect.height; i < len; ++i){
		ofVertex(400 + hSumValues[i], 20 + i);
	}
	ofEndShape(false);
    
    // draw triggers
    ofSetColor(0, 255, 0);
    ofFill();
    for(int i = 0, len = triggers.size(); i < len; ++i){
        ofCircle(600, 20 + cropRect.height * triggers[i].position, triggers[i].weight);
    }
}

void ofApp::exit() {
    gui->saveSettings("settings.xml");
    delete gui;
}

void ofApp::guiEvent(ofxUIEventArgs &e) {
    // TODO: dry below code...
    if(e.getName() == "REGULATION") {
        ofxUIToggle *toggle = e.getToggle();
        regulationActive = toggle->getValue();
    } else if (e.getName() == "SET_POINT"){
        ofxUISlider *slider = e.getSlider();
        regulator.setPoint = slider->getScaledValue();
    } else if (e.getName() == "TRIGGER_THRESHOLD"){
        ofxUISlider *slider = e.getSlider();
        triggerThreshold = slider->getScaledValue();
    } else if (e.getName() == "MIN_TRIG_WIDTH"){
        ofxUISlider *slider = e.getSlider();
        minTrigWidth = slider->getScaledValue();
    } else if (e.getName() == "MAX_TRIG_WIDTH"){
        ofxUISlider *slider = e.getSlider();
        maxTrigWidth = slider->getScaledValue();
    }
   
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
