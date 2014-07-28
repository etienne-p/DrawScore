#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // the number of libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)
    #ifdef TARGET_LINUX_ARM
    // longer latency for Raspberry PI
    int ticksPerBuffer = 32; // 32 * 64 = buffer len of 2048
    int numInputs = 0; // no built in mic
    #else
    int ticksPerBuffer = 8; // 8 * 64 = buffer len of 512
    int numInputs = 1;
    #endif
    
	// setup OF sound stream
	ofSoundStreamSetup(2, numInputs, this, 44100, ofxPd::blockSize()*ticksPerBuffer, 3);
    
	// setup the app core
	core.setup(2, numInputs, 44100, ticksPerBuffer);
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
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
