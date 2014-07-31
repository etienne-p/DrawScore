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
    
	numLines = 4;
    toggleFrame = 1;
    
    // we store the 2 latest frames, reusing the same vectors
    while (triggers.size() < 2) triggers.push_back(vector<int>());
    
	reader = new Reader();
    reader->setup(numLines);
    
    // setup gui
    gui = new ofxUICanvas();
    gui->addToggle("REGULATION", reader->regulationActive);
    gui->addSlider("SET_POINT", 0, 12, numLines);
    gui->addSlider("TRIGGER_THRESHOLD", 0, 1, reader->triggerThreshold);
    gui->addSlider("MIN_TRIG_WIDTH", 0, 10, reader->minTrigWidth);
    gui->addSlider("MAX_TRIG_WIDTH", 0, 60, reader->maxTrigWidth);
    gui->addSlider("MAX_VARIANCE", 0, 0.5, reader->maxVariance);
    gui->autoSizeToFitWidgets();
    gui->loadSettings("settings.xml");
    ofAddListener(gui->newGUIEvent, this, &AppCore::guiEvent);
    
    // TODO: nicer exit
	if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) OF_EXIT_APP(1);
    
	pd.addReceiver(*this);   // automatically receives from all subscribed sources
	pd.addToSearchPath("pd");
	pd.start();
    
    
    
    ofSetVerticalSync(true);
    setNumLines(numLines);
}

//--------------------------------------------------------------
void AppCore::update() {
    
    // detect changes
    toggleFrame *= -1;
    int prev = toggleFrame > 0 ? 0 : 1;
    int cur = toggleFrame > 0 ? 1 : 0;
    
    if (reader->update() == VALID) reader->getTriggers(triggers[cur]);
    else for (int i = 0; i < numLines; ++i) triggers[cur][i] = -1;
    
    // update patches on change
    for (int i = 0; i < numLines; ++i){
        if (triggers[cur][i] != triggers[prev][i]){
            pd.sendSymbol(instances[i].dollarZeroStr()+"-instance", triggers[cur][i] > 0 ? "on" : "off");
        }
    }
}

//--------------------------------------------------------------
void AppCore::draw() {
    reader->draw();
}

//--------------------------------------------------------------
void AppCore::exit() {
    
    // close PD patches
	for(int i = 0; i < instances.size(); ++i) pd.closePatch(instances[i]);
	instances.clear();
    
    // save & destroy GUI
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
        setNumLines(slider->getScaledValue());
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
void AppCore::setNumLines(int arg){
    numLines = arg;
    reader->setNumLines(numLines);
    for (int i = 0; i < 2; ++i) triggers[i].resize(numLines, 0);
    
    if (numLines < instances.size()){
        while (instances.size()  > numLines){
            pd.closePatch(instances.back());
            instances.pop_back();
        }
    } else {
        // open one patch per line / voice
        for(int i = instances.size(); i < numLines; ++i) {
            Patch p = pd.openPatch("pd/toggle_tones.pd");
            instances.push_back(p);
        }
    }
    
    // set midi note for each voice
    vector<int> notes;
    getMidiNotes(notes, MAJOR, instances.size());
    
    for(int i = 0, len = instances.size(); i < len; ++i) {
        pd.startMessage();
        pd.addSymbol("note");
        pd.addFloat(notes[i]);
        pd.finishList(instances[i].dollarZeroStr()+"-instance");
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
