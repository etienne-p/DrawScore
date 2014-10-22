//
//  CheckBox.cpp
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#include "CheckBox.h"

void CheckBox::setActive(bool active_){
    if (active_ == true){
		ofAddListener(ofEvents().touchDown, this, &CheckBox::touchDownHandler, eventPriority);
	}else{
		ofRemoveListener(ofEvents().touchDown, this, &CheckBox::touchDownHandler, eventPriority);
	}
}

bool CheckBox::touchDownHandler(ofTouchEventArgs &touch){
    
    return false;
}

