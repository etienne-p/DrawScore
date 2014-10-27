//
//  AppCore.h
//  DrawScore
//
//  Created by etienne cella on 2014-07-26.
//
//

#ifndef __DrawScore__AppCore__
#define __DrawScore__AppCore__

#include "ofMain.h"
#include "WidgetObserver.h"
#include "Widget.h"
#include "Reader.h"
#include "Slider.h"
#include "CheckBox.h"
#include "Synth.h"
#include "MusicUtils.h"

using namespace MusicUtils;

class AppCore : public WidgetObserver {
    
public:
    
    // main
    void setup(const int sampleRate);
    void update();
    void draw();
    void exit();
    void resize(float width_, float height_);
    
    // audio
    void audioRequested(float * output, int bufferSize, int nChannels);
    void audioReceived(float * output, int bufferSize, int nChannels);
    
    void setVolume(float value);
    void setNotes(int rootNote_, Mood mood_);
    
    vector<Synth*> synths;
    float volume, envelopeFactor;
    void setEnvelopeFactor(float value_);
    int rootNote;
    Mood mood;
    
    // cv
    Reader * reader;
    vector<int> triggers;
    int toggleFrame, numLines;
    void setNumLines(int arg);
    
    // ui
    void parameterChanged(int id, float value);
    void parameterChanged(int id, bool value);
    vector<Widget*> widgets;
    float width, height;
    ofTrueTypeFont font;
};

#endif
