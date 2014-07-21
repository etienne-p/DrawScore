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
    
    // setup gui
    gui = new ofxUICanvas();        //Creates a canvas at (0,0) using the default width
    gui->addToggle("REGULATION", regulationActive);
    gui->addSlider("SET_POINT", 0, 12, regulator.setPoint);
    gui->addSlider("TRIGGER_THRESHOLD", 0, 1, triggerThreshold);

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
        hSum(thresholded, hSumValues, cropRect.width, cropRect.height);
        
        // detect rectangles
        triggers.clear();
        float trigger = 0.8;
        int triggerCount = 0;
        int minTrigWidth = 2;
        int maxTrigWidth = 40;
        for(int i = 0, len = (int) cropRect.height; i < len; ++i){
            float val = (255 - hSumValues[i]) / 255; // [0, 1]
            bool hit = true;
            if (val > trigger){
                triggerCount++;
            } else {
                hit = false;
            }
            
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
	ofSetHexColor(0xffffff);
	vidGrabber.draw(20,20);
	videoTexture.draw(20+camWidth,20,cropRect.width,cropRect.height);
    ofDrawBitmapString("gain: " + ofToString(regulator.gain), 400, 10);
    ofDrawBitmapString("out: " + ofToString(regulator.output), 400, 30);
    ofDrawBitmapString("size: " + ofToString(triggers.size()), 400, 50);
    ofDrawBitmapString("output: " + ofToString(regulator.output), 400, 80);
    ofDrawBitmapString("setPoint: " + ofToString(regulator.setPoint), 400, 110);
    
    
    ofSetColor(255, 0, 0);
	ofNoFill();
	ofBeginShape();
	for(int i = 0, len = (int) cropRect.height; i < len; ++i){
		ofVertex(400 + hSumValues[i], 20 + i);
	}
	ofEndShape(false);
    
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
    
    if(e.getName() == "REGULATION") {
        ofxUIToggle *toggle = e.getToggle();
        regulationActive = toggle->getValue();
    } else if (e.getName() == "SET_POINT"){
        ofxUISlider *slider = e.getSlider();
        regulator.setPoint = slider->getScaledValue();
    } else if (e.getName() == "TRIGGER_THRESHOLD"){
        ofxUISlider *slider = e.getSlider();
        triggerThreshold = slider->getScaledValue();
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
