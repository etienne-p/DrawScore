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
    regulator.setPoint = 128;
    regulator.output = 10;
    regulator.minOutput = 0;
    regulator.maxOutput = 255;
    regulator.gain = 1;
    regulator.minGain = 1;
    regulator.gainAcceleration = .05f;
    regulator.error = 255; //high to make sure we have a proper regulation phase
    regulationActive = true;
    tolerance = 5;
    
    // setup gui
    gui = new ofxUICanvas();        //Creates a canvas at (0,0) using the default width
    toggle = gui->addToggle("REGULATION", regulationActive);
    gui->addSlider("SET_POINT", regulator.minOutput, regulator.maxOutput, regulator.setPoint);
    gui->addSlider("GAIN_ACCELERATION", 0, 0.3, regulator.gainAcceleration);
    gui->addSlider("TOLERANCE", 0, 50, tolerance);
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
        
        if (regulationActive) {
            regulator.update(avr);
            if (abs(regulator.error) < tolerance) {
                regulationActive = false;
                toggle->setValue(regulationActive);
                ofLogNotice("Regulation complete.");
            }
        }
       
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
	//vidGrabber.draw(20,20);
	videoTexture.draw(20+camWidth,20,cropRect.width,cropRect.height);
    ofDrawBitmapString("gain: " + ofToString(regulator.gain), 400, 10);
    ofDrawBitmapString("out: " + ofToString(regulator.output), 400, 30);

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
    } else if (e.getName() == "GAIN_ACCELERATION"){
        ofxUISlider *slider = e.getSlider();
        regulator.gainAcceleration = slider->getScaledValue();
    } else if (e.getName() == "TOLERANCE"){
        ofxUISlider *slider = e.getSlider();
        tolerance = slider->getScaledValue();
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
