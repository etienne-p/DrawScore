//
//  CheckBox.cpp
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#include "CheckBox.h"


CheckBox::CheckBox(int id_, WidgetObserver * observer_, string label_, ofTrueTypeFont * font_, int eventPriority_) : Widget(id_, observer_){
    label = label_;
    font = font_;
    eventPriority = eventPriority_;
    radius = 0.f;
}

CheckBox::~CheckBox(){
    font = NULL;
    setActive(false);
}

void CheckBox::draw(){
    ofSetColor(ofColor::gray);
    ofPushMatrix();
    ofTranslate(position);
    float height = getHeight();
    ofCircle(radius, height * 0.5f, radius);
    font->drawString(label, radius * 2.f, (height - font->getLineHeight()) * 0.5f + font->getSize());
    if (!checked){
        ofSetColor(ofColor::white);
        ofCircle(radius, height * 0.5f, radius * 0.8);
    }
    ofPopMatrix();
}

void CheckBox::setActive(bool active_){
    if (active_ == true){
		ofAddListener(ofEvents().touchDown, this, &CheckBox::touchDownHandler, eventPriority);
	}else{
		ofRemoveListener(ofEvents().touchDown, this, &CheckBox::touchDownHandler, eventPriority);
	}
}

bool CheckBox::touchDownHandler(ofTouchEventArgs &touch){
    if (ofVec2f(radius, radius).distance(globalToLocal(touch.x, touch.y)) < radius){
        checked = !checked;
        observer->parameterChanged(id, checked);
        return true;
    }
    return false;
}

float CheckBox::getWidth(){
    return radius * 2.f + font->getStringBoundingBox(label, 0, 0).width;
}

float CheckBox::getHeight(){
     return MAX(font->getLineHeight(), radius * 2.f);
}


