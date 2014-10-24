#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //#ifdef TARGET_LINUX_ARM
   
    //#else
    
    //#endif
    
    int samplerate = 44100;
    
	// setup OF sound stream
	ofSoundStreamSetup(2, 0, this, samplerate, 1024, 3);
    
	// setup the app core
	core.setup(samplerate);
    core.resize(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
    core.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    core.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {
    core.exit();
}

//--------------------------------------------------------------
void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {
	core.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels) {
	core.audioRequested(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    touchArg.x = x;
    touchArg.y = y;
    ofNotifyEvent( ofEvents().touchMoved, touchArg );
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    touchArg.x = x;
    touchArg.y = y;
    ofNotifyEvent( ofEvents().touchDown, touchArg );
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    touchArg.x = x;
    touchArg.y = y;
    ofNotifyEvent( ofEvents().touchUp, touchArg );
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    core.resize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
