//
//  AppCore.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-26.
//
//

#include "AppCore.h"

// Parameters ID
#define P_REGULATION 0
#define P_TRIGGER_THRESHOLD 1
#define P_MAX_VARIANCE 2
#define P_MAX_AVERAGE_WEIGHT 3
#define P_NUM_LINES 4
#define P_ROOT_NOTE 5
#define P_MAJOR 6
#define P_VOLUME 7

//--------------------------------------------------------------
void AppCore::setup(const int numOutChannels, const int numInChannels,
                    const int sampleRate, const int ticksPerBuffer) {
    
	numLines = 7;
    toggleFrame = 1;
    invalidFramesCount = 0;
    
    // we store the 2 latest frames, reusing the same vectors
    while (triggers.size() < 2) triggers.push_back(vector<int>());
    
	reader = new Reader();
    reader->setup(numLines);
    
    volume = 0.5f;
    
    rootNote = 48; // C3
    mood = MAJOR;
    
    // setup gui
    ofTrueTypeFont::setGlobalDpi(72);
    //font.loadFont("fonts/verdana.ttf", 12);
    int eventPriority = 0;
    
    CheckBox * wRegulation = new CheckBox(P_REGULATION, this, "Regulation", &font, eventPriority);
    wRegulation->checked = reader->regulationActive;
    widgets.push_back(wRegulation);
   
    CheckBox * wMood = new CheckBox(P_MAJOR, this, "Major Scale", &font, eventPriority);
    wMood->checked = mood == MAJOR;
    widgets.push_back(wMood);
    
    Slider * wTriggerThreshold = new Slider(P_TRIGGER_THRESHOLD, this, "Trigger Threshold", &font, 0.f, 1.f, eventPriority);
    wTriggerThreshold->setValue(reader->triggerThreshold);
    widgets.push_back(wTriggerThreshold);
    
    Slider * wMaxVariance = new Slider(P_MAX_VARIANCE, this, "Max Variance", &font, 0.f, .4f, eventPriority);
    wMaxVariance->setValue(reader->maxVariance);
    widgets.push_back(wMaxVariance);
    
    Slider * wMaxAverageWeight = new Slider(P_MAX_AVERAGE_WEIGHT, this, "Max Average Weight", &font, 0.f, .2f, eventPriority);
    wMaxAverageWeight->setValue(reader->maxAverageWeight);
    widgets.push_back(wMaxAverageWeight);
    
    Slider * wNumLines = new Slider(P_NUM_LINES, this, "Num Lines", &font, 3, 12, eventPriority);
    wNumLines->setStep(1.f);
    wNumLines->setValue(numLines);
    widgets.push_back(wNumLines);
    
    Slider * wRootNote = new Slider(P_ROOT_NOTE, this, "Root Note", &font, 10, 80, eventPriority);
    wRootNote->setStep(1.f);
    wRootNote->setValue(rootNote);
    widgets.push_back(wRootNote);
    
    Slider * wVolume = new Slider(P_VOLUME, this, "Volume", &font, 0.f, 1.f, eventPriority);
    wVolume->setValue(volume);
    widgets.push_back(wVolume);
    
    for (int i = widgets.size() - 1; i > -1; i--) widgets[i]->setActive(true);
    
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
    
    
    bool frameValid = (reader->update() == VALID);
    
    if (!frameValid && ++invalidFramesCount < 4) return;
    
    // detect changes
    toggleFrame *= -1;
    int prev = toggleFrame > 0 ? 0 : 1;
    int cur = toggleFrame > 0 ? 1 : 0;
    
    if (frameValid){
        invalidFramesCount = 0;
        reader->getTriggers(triggers[cur]);
    } else {
        for (int i = 0; i < numLines; ++i) triggers[cur][i] = -1;
    }
    
    // update patches if the frame is valid, or if we are having a serie of invalid frames
    // so we avoid audio glitches in case of isolated invalid frames
    for (int i = 0; i < numLines; ++i){
        if (triggers[cur][i] != triggers[prev][i]){
            pd.sendSymbol(instances[i].dollarZeroStr()+"-instance", triggers[cur][i] > 0 ? "on" : "off");
        }
    }
}

//--------------------------------------------------------------
void AppCore::resize(float width_, float height_) {
    
    float radius = MIN((height_ / widgets.size()) * .2f, 20.f);
    float space = radius * .5f;
    float slWidth = width_ * .8f;
    float uiHeight = 0.f;
    
    font.loadFont("fonts/verdana.ttf", (int) floorf(MAX(10.f, radius)));
    //font.setLineHeight(radius);
    
    // set checkboxes radius
    CheckBox * cb = NULL;
    cb = (CheckBox *) widgets[0];
    cb->radius = radius;
    cb = (CheckBox *) widgets[1];
    cb->radius = radius;
    uiHeight += cb->getHeight();
    
    // set sliders radius
    for (int i = 2; i < widgets.size(); i++){        
        Slider * sl = (Slider*) widgets[i];
        sl->setCursorRadius(radius);
        sl->setWidth(slWidth);
        uiHeight += sl->getHeight();
    }
    
    uiHeight += (widgets.size() - 2) * space;
    
    ofVec2f wPos((width_ - slWidth) * .5f, (height_ - uiHeight) * .5f);
    
    // place check boxes
    cb = (CheckBox *) widgets[0];
    cb->position.set(wPos);
    cb = (CheckBox *) widgets[1];
    cb->position.set(wPos + ofVec2f(slWidth * .5f, 0));
    wPos.y += cb->getHeight() + space;
    
    // place sliders
    for (int i = 2; i < widgets.size(); i++){
        Slider * sl = (Slider*) widgets[i];
        sl->position.set(wPos);
        wPos.y += sl->getHeight() + space;
    }}

//--------------------------------------------------------------
void AppCore::draw() {
    //reader->draw();
    for (int i = widgets.size() - 1; i > -1; i--) widgets[i]->draw();
}

//--------------------------------------------------------------
void AppCore::exit() {
    
    // close PD patches
	for(int i = 0; i < instances.size(); ++i) pd.closePatch(instances[i]);
	instances.clear();
}

//--------------------------------------------------------------
void AppCore::setNumLines(int arg){
    
    numLines = arg;
    reader->numLines = numLines;
    for (int i = 0; i < 2; ++i) triggers[i].resize(numLines, 0);
    
    // create / delete voices as needed
    if (numLines < instances.size()){
        while (instances.size()  > numLines){
            pd.closePatch(instances.back());
            instances.pop_back();
        }
    } else {
        // open one patch per line / voice
        for(int i = instances.size(); i < numLines; ++i) {
            Patch p = pd.openPatch("pd/toggle_tone.pd");
            instances.push_back(p);
        }
    }
    setNotes(rootNote, mood);
    setVolume(volume);
}

void AppCore::setNotes(int rootNote_, Mood mood_){
    
    // set midi note for each voice
    int * notes = getMidiNotes(rootNote_, mood_, instances.size()); // 48 = C3
    
    for(int i = 0, len = instances.size(); i < len; ++i) {
        pd.startMessage();
        pd.addSymbol("note");
        pd.addFloat((float) notes[i]);
        pd.finishList(instances[i].dollarZeroStr()+"-instance");
	}
    
    delete[] notes; // necessary?
    
}

void AppCore::setVolume(float value){
    for(int i = 0, len = instances.size(); i < len; ++i) {
        pd.startMessage();
        pd.addSymbol("volume");
        pd.addFloat(volume);
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

//--------------------------------------------------------------
void AppCore::parameterChanged(int id, float value) {
    cout << "slider id: " << ofToString(id) << " value: " << ofToString(value) << endl;
    switch (id) {
        case P_TRIGGER_THRESHOLD:
            reader->triggerThreshold = value;
            break;
        case P_MAX_VARIANCE:
            reader->maxVariance = value;
            break;
        case P_MAX_AVERAGE_WEIGHT:
            reader->maxAverageWeight = value;
            break;
        case P_NUM_LINES:
            numLines = (int) value;
            setNumLines(numLines);
            break;
        case P_ROOT_NOTE:
            rootNote = (int) value;
            setNotes(rootNote, mood);
            break;
        case P_VOLUME:
            volume = value;
            setVolume(volume);
            break;
    }
}

//--------------------------------------------------------------
void AppCore::parameterChanged(int id, bool value) {
    cout << "checkbox id: " << ofToString(id) << " value: " << ofToString(value) << endl;
    switch (id) {
        case P_REGULATION:
            reader->regulationActive = value;
            break;
        case P_MAJOR:
            mood = value ? MAJOR : MINOR;
            setNotes(rootNote, mood);
            break;
    }
}



