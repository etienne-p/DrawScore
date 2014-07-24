#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	reader = new Reader();
    reader->setup(4);
    
	ofSetVerticalSync(true);
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
    reader->update();
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
