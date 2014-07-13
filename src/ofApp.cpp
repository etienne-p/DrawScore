#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;
    
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
    
	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(100,100, GL_RGB);
	ofSetVerticalSync(true);
}


//--------------------------------------------------------------
void ofApp::update(){
    
	ofBackground(100,100,100);
    
	vidGrabber.update();
    
	if (vidGrabber.isFrameNew()){
        unsigned char * pixels = vidGrabber.getPixels();
        //unsigned char * gray = ScoreReader::grayScale(pixels, camWidth ,camHeight);
        //unsigned char * thresholded = ScoreReader::threshold(gray, camWidth ,camHeight, 120);
		//avr = ScoreReader::averagePixelValue(gray, camWidth ,camHeight);
        unsigned char * cropped = ScoreReader::crop(pixels, camWidth ,camHeight, camWidth - 100, camHeight - 100, 100, 100);
        videoTexture.loadData(cropped, 100, 100, GL_RGB);
        //delete[] gray;
        //delete[] thresholded;
        delete[] cropped;


	}
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xffffff);
	vidGrabber.draw(20,20);
	videoTexture.draw(20+camWidth,20,100,100);
    ofDrawBitmapString(ofToString(avr), 10, 10);
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
