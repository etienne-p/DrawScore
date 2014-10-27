//
//  UISlider.cpp
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#include "Slider.h"

Slider::Slider(int id_, WidgetObserver * observer_, string label_, ofTrueTypeFont * font_, float min_, float max_, int eventPriority_) : Widget(id_, observer_){
    label = label_;
    font = font_;
    min = min_;
    max = max_;
    eventPriority = eventPriority_;
    position.set(.0f, .0f);
    cursorPosition.set(.0f, .0f);
    width = .0f;
    cursorRadius = .0f;
    currentTouchId = -1;
    step = -1.f;
    radiusTouchFactor = 1.8f;
    setValue(0);
}

void Slider::draw(){
    ofFill();
    ofSetColor(ofColor::gray);
    ofPushMatrix();
    ofTranslate(position);
    
    ofSetColor(ofColor::lightGray);
    
    ofRect(cursorRadius, cursorPosition.y - cursorRadius, width - 2.f * cursorRadius, cursorRadius * 2.f);
    
    ofCircle(cursorRadius, cursorPosition.y, cursorRadius);
    
    ofCircle(width - cursorRadius, cursorPosition.y, cursorRadius);
    
    ofSetColor(ofColor::gray);
    font->drawString(label, 0, font->getLineHeight());
    string vStr = ofToString(value);
    font->drawString(vStr, width - font->getStringBoundingBox(vStr, 0, 0).width, font->getLineHeight());
    ofCircle(cursorPosition, cursorRadius);
    ofPopMatrix();
    
    bool touched = currentTouchId == -1; // highlight on touch
}

Slider::~Slider(){
    font = NULL;
    setActive(false);
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
    if (cursorPosition.distance(globalToLocal(touch.x, touch.y)) < cursorRadius * radiusTouchFactor){
        currentTouchId = touch.id;
        return true;
    }
    return false;
}

bool Slider::touchMoveHandler(ofTouchEventArgs &touch){
    if (touch.id == currentTouchId){
        setCursorPosition(globalToLocal(touch.x, touch.y));
        return true;
    }
    return false;
}

bool Slider::touchUpHandler(ofTouchEventArgs &touch){
    currentTouchId = -1;
    setValue(value);
    return false;
}

void Slider::setCursorPosition(ofVec2f position_){
    cursorPosition.x = MAX(cursorRadius, MIN(position_.x, width - cursorRadius));
    syncValueOnCursor();
}

void Slider::syncValueOnCursor(){
    float ratio = (cursorPosition.x - cursorRadius) / (width - 2 * cursorRadius);
    setValue(min + (max - min) * ratio);
    observer->parameterChanged(id, value);
}

void Slider::setValue(float value_){
    float steppedValue = value_;
    if (step > 0.f) steppedValue = floorf(value_ / step) * step;
    value = MIN(max, MAX(min, steppedValue));
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

float Slider::getHeight(){
    return cursorRadius * 2.f + font->getLineHeight() * 1.4f;
}

void Slider::setCursorRadius(float radius_){
    cursorRadius = radius_;
    cursorPosition.y = cursorRadius + font->getLineHeight() * 1.4f;
    setValue(value);
}

float Slider::getCursorRadius(){
    return cursorRadius;
}

void Slider::setStep(float step_){
    step = step_;
    setValue(value);
}
