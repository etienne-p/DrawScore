//
//  Widget.cpp
//  DrawScore
//
//  Created by Etienne on 2014-10-23.
//
//

#include "Widget.h"


Widget::Widget(int id_, WidgetObserver * observer_){
    id = id_;
    observer = observer_;
}

Widget::~Widget(){
    observer = NULL;
}

ofVec2f Widget::globalToLocal(ofVec2f position_){
    return position_ - position;
}

ofVec2f Widget::globalToLocal(float x_, float y_){
    return globalToLocal(ofVec2f(x_, y_));
}