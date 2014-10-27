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
    float getWidth();
    float getHeight();
    void setWidth(float width_);
    
    void setCursorRadius(float radius_);
    float getCursorRadius();
    float radiusTouchFactor;
        
    void setValue(float value_);
    float getValue();
    
    void setStep(float step_);
    
    Slider(int id_, WidgetObserver * observer_, string label_, ofTrueTypeFont * font_, float min_, float max, int eventPriority_);
    ~Slider();
 
private:
    
    ofTrueTypeFont * font;
    string label;
    float step;
    
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
};


#endif /* defined(__DrawScore__Slider__) */
