#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	reader = new Reader();
    reader->setup(4);
    
    // setup gui
    gui = new ofxUICanvas();
    gui->addToggle("REGULATION", reader->regulationActive);
    gui->addSlider("SET_POINT", 0, 12, reader->getNumLines());
    gui->addSlider("TRIGGER_THRESHOLD", 0, 1, reader->triggerThreshold);
    gui->addSlider("MIN_TRIG_WIDTH", 0, 10, reader->minTrigWidth);
    gui->addSlider("MAX_TRIG_WIDTH", 0, 60, reader->maxTrigWidth);
    gui->addSlider("MAX_VARIANCE", 0, 0.5, reader->maxVariance);
    gui->autoSizeToFitWidgets();
    gui->loadSettings("settings.xml");
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    
	ofSetVerticalSync(true);
}

void ofApp::guiEvent(ofxUIEventArgs &e) {
    if(e.getName() == "REGULATION") {
        ofxUIToggle *toggle = e.getToggle();
        reader->regulationActive = toggle->getValue();
    } else if (e.getName() == "SET_POINT"){
        ofxUISlider *slider = e.getSlider();
        reader->setNumLines(slider->getScaledValue());
    } else if (e.getName() == "TRIGGER_THRESHOLD"){
        ofxUISlider *slider = e.getSlider();
        reader->triggerThreshold = slider->getScaledValue();
    } else if (e.getName() == "MIN_TRIG_WIDTH"){
        ofxUISlider *slider = e.getSlider();
        reader->minTrigWidth = slider->getScaledValue();
    } else if (e.getName() == "MAX_TRIG_WIDTH"){
        ofxUISlider *slider = e.getSlider();
        reader->maxTrigWidth = slider->getScaledValue();
    } else if (e.getName() == "MAX_VARIANCE"){
        ofxUISlider *slider = e.getSlider();
        reader->maxVariance = slider->getScaledValue();
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    reader->update();
	
}

//--------------------------------------------------------------
void ofApp::draw(){
    reader->draw();
    
}

void ofApp::exit() {
    gui->saveSettings("settings.xml");
    delete gui;
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
