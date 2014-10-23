//
//  CheckBox.h
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#ifndef __DrawScore__CheckBox__
#define __DrawScore__CheckBox__

#include "ofMain.h"
#include "Widget.h"

class CheckBox : public Widget {
    
public:
    
    // widget
    void draw();
    void setActive(bool active_);
    
    float getWidth();
    float getHeight();
    
    float radius;
    
    bool checked;
    
    CheckBox(int id_, WidgetObserver * observer_, string label, ofTrueTypeFont * font_, int eventPriority_);
    ~CheckBox();
    
private:
    
    string label;
    ofTrueTypeFont * font;
    
    bool touchDownHandler(ofTouchEventArgs &touch);
    
    int eventPriority;
};

#endif /* defined(__DrawScore__CheckBox__) */
