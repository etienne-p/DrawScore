//
//  Slider.h
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#ifndef __DrawScore__Slider__
#define __DrawScore__Slider__

#include "ofMain.h"
#include "Widget.h"
#include "WidgetObserver.h"

class Slider : public Widget{
    
public:
    
    // widget
    void setActive(bool arg);
    void draw();
    
    // display object
    void setWidth(float width_);
    float getWidth();
    void setCursorRadius(float radius_);
    float getCursorRadius();
    ofVec2f position;
        
    void setValue(float value_);
    float getValue();
    
    Slider(int id_, float min_, float max, int eventPriority_, WidgetObserver * observer_);
    ~Slider();
 
private:
    
    int id;
    
    WidgetObserver * observer;
    float value, min, max;
    
    bool touchDownHandler(ofTouchEventArgs &touch);
    bool touchMoveHandler(ofTouchEventArgs &touch);
    bool touchUpHandler(ofTouchEventArgs &touch);
    
    int currentTouchId;
    int eventPriority;

    void setCursorPosition(ofVec2f position_);
    void syncValueOnCursor();
    
    float width;    
    ofVec2f cursorPosition;
    float cursorRadius;
    
    // helper
    ofVec2f globalToLocal(ofVec2f position_);
    ofVec2f globalToLocal(float x_, float y_);
};


#endif /* defined(__DrawScore__Slider__) */
