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

class CheckBox {
    
public:
    
    // widget
    void draw();
    void setActive(bool active_);
    
    ofVec2f position;
    float radius;
    
    bool checked;
    
    CheckBox(int id_, int eventPriority_);
    ~CheckBox();
    
private:
    
    bool touchDownHandler(ofTouchEventArgs &touch);
    
    int eventPriority;
};

#endif /* defined(__DrawScore__CheckBox__) */
