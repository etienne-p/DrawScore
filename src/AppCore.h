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
#include "ofxPd.h"
#include "WidgetObserver.h"
#include "Widget.h"
#include "Reader.h"
#include "Slider.h"
#include "CheckBox.h"
#include "MusicUtils.h"

using namespace MusicUtils;
using namespace pd;

class AppCore : public PdReceiver, public WidgetObserver {
    
public:
    
    // main
    void setup(const int numOutChannels, const int numInChannels,
               const int sampleRate, const int ticksPerBuffer);
    void update();
    void draw();
    void exit();
    void resize(float width_, float height_);
    
    // audio callbacks
    void audioReceived(float * input, int bufferSize, int nChannels);
    void audioRequested(float * output, int bufferSize, int nChannels);
    
    // pd message receiver callbacks
    void print(const std::string& message);
    
    void receiveBang(const std::string& dest);
    void receiveFloat(const std::string& dest, float value);
    void receiveSymbol(const std::string& dest, const std::string& symbol);
    void receiveList(const std::string& dest, const List& list);
    void receiveMessage(const std::string& dest, const std::string& msg, const List& list);
    
    ofxPd pd;
    vector<Patch> instances;
    
    void setVolume(float value);
    void setNotes(int rootNote_, Mood mood_);
    
    int rootNote;
    Mood mood;
    
    // CV
    Reader * reader;
    
    vector<vector<int> > triggers;
    
    int invalidFramesCount;
    int toggleFrame;
    
    int numLines;
    void setNumLines(int arg);
    
    float volume;
    
    void parameterChanged(int id, float value);
    void parameterChanged(int id, bool value);
    vector<Widget*> widgets;
    
    ofTrueTypeFont font;
};

#endif
