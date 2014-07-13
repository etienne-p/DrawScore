#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
	camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;
    
    cropRect.x = (camWidth - 20) / 2;
    cropRect.y = 0;
    cropRect.width = 20;
    cropRect.height = camHeight;
    
    targetAvr = 5;
    threshold = 120;
    
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
        unsigned char * pixels = vidGrabber.getPixels();
        unsigned char * cropped = ScoreReader::crop(pixels, camWidth ,camHeight, cropRect.x, cropRect.y, cropRect.width, cropRect.height);
        unsigned char * gray = ScoreReader::grayScale(cropped, cropRect.width, cropRect.height);
        unsigned char * thresholded = ScoreReader::threshold(gray, cropRect.width, cropRect.height, threshold);
		avr = ScoreReader::averagePixelValue(thresholded, cropRect.width, cropRect.height);
        unsigned char * expanded = ScoreReader::oneToThreeChannels(thresholded, cropRect.width, cropRect.height);
        videoTexture.loadData(expanded, cropRect.width, cropRect.height, GL_RGB);
       
        // TODO: remove delete and observe memory leak.
        delete[] cropped;
        delete[] gray;
        delete[] thresholded;
        delete[] expanded;
        
        // very naive threshold auto
        threshold += avr > targetAvr ? 1 : -1;
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetHexColor(0xffffff);
	//vidGrabber.draw(20,20);
	videoTexture.draw(20+camWidth,20,cropRect.width,cropRect.height);
    ofDrawBitmapString(ofToString(avr), 10, 10);
    ofDrawBitmapString(ofToString(threshold), 10, 30);

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
