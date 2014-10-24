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
    ofFill();
    ofSetColor(ofColor::gray);
    ofPushMatrix();
    ofTranslate(position);
    float height = getHeight();
    font->drawString(label, radius * 2.f + font->getStringBoundingBox(label, 0, 0).height * .5f, (height - font->getLineHeight()) * 0.5f + font->getSize());
    if (!checked) ofSetColor(ofColor::lightGray);
    ofCircle(radius, height * 0.5f, radius);
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
    ofRectangle bnd = font->getStringBoundingBox(label, 0, 0);
    return radius * 2.f + bnd.width + bnd.height * .5f;
}

float CheckBox::getHeight(){
     return MAX(font->getLineHeight(), radius * 2.f);
}


