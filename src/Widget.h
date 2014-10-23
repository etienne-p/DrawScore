//
//  Widget.h
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#ifndef DrawScore_Widget_h
#define DrawScore_Widget_h

#include "ofMain.h"
#include "WidgetObserver.h"

class Widget {
    
public:
    
    virtual void draw() = 0;
    virtual void setActive(bool active) = 0;
    
    virtual float getWidth() = 0;
    virtual float getHeight() = 0;
    
    ofVec2f position;
    
    ofVec2f globalToLocal(ofVec2f position_);
    ofVec2f globalToLocal(float x_, float y_);
    
    Widget(int id_, WidgetObserver * observer_);
    ~Widget();
    
protected:
    
    int id;
    WidgetObserver * observer;
};

#endif
