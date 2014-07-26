//
//  AppCore.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-26.
//
//

#include "AppCore.h"

//--------------------------------------------------------------
void AppCore::setup(const int numOutChannels, const int numInChannels,
                    const int sampleRate, const int ticksPerBuffer) {
    
	
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
    ofAddListener(gui->newGUIEvent, this, &AppCore::guiEvent);
    
	ofSetVerticalSync(true);
    
	if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) {
		OF_EXIT_APP(1);
	}
    
	// add message receiver, disables polling (see processEvents)
	pd.addReceiver(*this);   // automatically receives from all subscribed sources
	// add the data/pd folder to the search path
	pd.addToSearchPath("pd");
	// audio processing on
	pd.start();
    // open patch
	Patch patch = pd.openPatch("pd/test.pd");
	
}

//--------------------------------------------------------------
void AppCore::update() {
	reader->update();
}

//--------------------------------------------------------------
void AppCore::draw() {
    reader->draw();

}

//--------------------------------------------------------------
void AppCore::exit() {
    gui->saveSettings("settings.xml");
    delete gui;
}

//--------------------------------------------------------------
void AppCore::guiEvent(ofxUIEventArgs &e) {
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
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void AppCore::receiveBang(const std::string& dest) {
	cout << "OF: bang " << dest << endl;
}

void AppCore::receiveFloat(const std::string& dest, float value) {
	cout << "OF: float " << dest << ": " << value << endl;
}

void AppCore::receiveSymbol(const std::string& dest, const std::string& symbol) {
	cout << "OF: symbol " << dest << ": " << symbol << endl;
}

void AppCore::receiveList(const std::string& dest, const List& list) {
	cout << "OF: list " << dest << ": ";
    
	// step through the list
	for(int i = 0; i < list.len(); ++i) {
		if(list.isFloat(i))
			cout << list.getFloat(i) << " ";
		else if(list.isSymbol(i))
			cout << list.getSymbol(i) << " ";
	}
    
	// you can also use the built in toString function or simply stream it out
	// cout << list.toString();
	// cout << list;
    
	// print an OSC-style type string
	cout << list.types() << endl;
}

void AppCore::receiveMessage(const std::string& dest, const std::string& msg, const List& list) {
	cout << "OF: message " << dest << ": " << msg << " " << list.toString() << list.types() << endl;
}

//--------------------------------------------------------------
void AppCore::print(const std::string& message) {
	cout << message << endl;
}
