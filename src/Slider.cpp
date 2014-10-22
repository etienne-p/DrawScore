//
//  UISlider.cpp
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#include "Slider.h"

Slider::Slider(int id_, float min_, float max_, int eventPriority_, WidgetObserver * observer_){
    id = id_;
    min = min_;
    max = max_;
    eventPriority = eventPriority_;
    observer = observer_;
    position.set(.0f, .0f);
    cursorPosition.set(.0f, .0f);
    width = .0f;
    cursorRadius = .0f;
    currentTouchId = -1;
    setValue(0);
}

Slider::~Slider(){
    setActive(false);
    observer = NULL;
}

void Slider::draw(){
    ofSetColor(0, 0, 255);    //set te color to blue
    ofPushMatrix();
    ofTranslate(position);
    ofRect(cursorRadius, cursorRadius - 2.f, width - 2.f * cursorRadius, 4.f);
    ofCircle(cursorPosition, cursorRadius);
    ofPopMatrix();
    bool touched = currentTouchId == -1; // highlight on touch
}

void Slider::setActive(bool active_){
    currentTouchId = -1;
    if (active_ == true){
		ofAddListener(ofEvents().touchDown, this, &Slider::touchDownHandler, eventPriority);
		ofAddListener(ofEvents().touchMoved, this, &Slider::touchMoveHandler, eventPriority);
		ofAddListener(ofEvents().touchUp, this, &Slider::touchUpHandler, eventPriority);
	}else{
		ofRemoveListener(ofEvents().touchDown, this, &Slider::touchDownHandler, eventPriority);
		ofRemoveListener(ofEvents().touchMoved, this, &Slider::touchMoveHandler, eventPriority);
		ofRemoveListener(ofEvents().touchUp, this, &Slider::touchUpHandler, eventPriority);
	}
}

bool Slider::touchDownHandler(ofTouchEventArgs &touch){
    if (cursorPosition.distance(globalToLocal(touch.x, touch.y)) < cursorRadius){
        currentTouchId = touch.id;
        return true;
    }
    return false;
}

bool Slider::touchMoveHandler(ofTouchEventArgs &touch){
    if (touch.id == currentTouchId){
        setCursorPosition(globalToLocal(touch.x, touch.y));
    }
    return false;
}

bool Slider::touchUpHandler(ofTouchEventArgs &touch){
    currentTouchId = -1;
    return false;
}

void Slider::setCursorPosition(ofVec2f position_){
    cursorPosition.x = MAX(cursorRadius, MIN(position_.x, width - cursorRadius));
    syncValueOnCursor();
}

void Slider::syncValueOnCursor(){
    float ratio = (cursorPosition.x - cursorRadius) / (width - 2 * cursorRadius);
    value = min + (max - min) * ratio;
    observer->parameterChanged(id, value);
}

void Slider::setValue(float value_){
    value = MIN(max, MAX(min, value_));
    float ratio = (value - min) / (max - min);
    cursorPosition.x = cursorRadius + (width - 2 * cursorRadius) * ratio;
}

float Slider::getValue(){
    return value;
}

void Slider::setWidth(float width_){
    width = width_;
    setValue(value);
}

float Slider::getWidth(){
    return width;
}

void Slider::setCursorRadius(float radius_){
    cursorRadius = radius_;
    cursorPosition.y = cursorRadius;
    setValue(value);
}

float Slider::getCursorRadius(){
    return cursorRadius;
}

// Helpers

ofVec2f Slider::globalToLocal(ofVec2f position_){
    return position_ - position;
}

ofVec2f Slider::globalToLocal(float x_, float y_){
    return globalToLocal(ofVec2f(x_, y_));
}
