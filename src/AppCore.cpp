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
void AppCore::setup(const int sampleRate) {
    
	numLines = 7;
    toggleFrame = 1;
    
    reader = new Reader();
    reader->setup(numLines);
    
    volume = 0.f; // preserve your ears ;)
    
    rootNote = 48; // C3
    mood = MAJOR;
    
    // setup gui
    ofTrueTypeFont::setGlobalDpi(72);
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
    
    ofSetVerticalSync(true);
    setNumLines(numLines);
}

//--------------------------------------------------------------
void AppCore::update() {
    
    if (reader->update() == VALID){
        reader->getTriggers(triggers);
    } else {
        for (int i = 0; i < numLines; ++i) triggers[i] = -1;
    }
    
    for (int i = 0; i < numLines; ++i){
        float mul = ((triggers[i] > 0 ? 1.f : -1.f) * .05f + 1.f) * synths[i]->mul;
        synths[i]->mul = MIN(1.f, mul);
    }
}

//--------------------------------------------------------------
void AppCore::resize(float width_, float height_) {
    
    float radius = MIN((height_ / widgets.size()) * .2f, 20.f);
    float space = radius * .5f;
    float slWidth = width_ * .8f;
    float uiHeight = 0.f;
    
    font.loadFont("fonts/verdana.ttf", (int) floorf(MAX(10.f, radius)));
    
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
    
    // TODO: destroy synths, ui
}

//--------------------------------------------------------------
void AppCore::setNumLines(int arg){
    
    numLines = arg;
    reader->numLines = numLines;
    triggers.resize(numLines, 0);
    
    // create / delete voices as needed
    if (numLines < synths.size()){
        while (synths.size()  > numLines){
            Synth * st = synths.back();
            synths.pop_back();
            delete st;
        }
    } else {
        // open one patch per line / voice
        for(int i = synths.size(); i < numLines; ++i) {
            synths.push_back(new Synth());
        }
    }
    setNotes(rootNote, mood);
    setVolume(volume);
}

void AppCore::setNotes(int rootNote_, Mood mood_){
    
    int * notes = getMidiNotes(rootNote_, mood_, synths.size());
    
    for(int i = 0, len = synths.size(); i < len; ++i) {
        synths[i]->frequency = MusicUtils::midiToFrequency(notes[i]);
	}
    
    delete[] notes;
}

void AppCore::setVolume(float value){
    for(int i = 0, len = synths.size(); i < len; ++i) {
        synths[i]->volume = value; // TODO: scale depending on synth count & freqs?
	}
}

//--------------------------------------------------------------
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	
}

//--------------------------------------------------------------
void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	
    vector<float> buf(bufferSize, 0.0);
    
    for(int i = synths.size() - 1; i > -1; i--) {
        synths[i]->processAudio(&buf[0], bufferSize);
	}
    
    for (int i = 0; i < bufferSize; ++i){
		output[i * nChannels] = output[i * nChannels + 1] = buf[i]; // assuming 2 channels
	}
}

//--------------------------------------------------------------
void AppCore::parameterChanged(int id, float value) {
    // cout << "slider id: " << ofToString(id) << " value: " << ofToString(value) << endl;
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
    // cout << "checkbox id: " << ofToString(id) << " value: " << ofToString(value) << endl;
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



