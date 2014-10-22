//
//  Widget.h
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#ifndef DrawScore_Widget_h
#define DrawScore_Widget_h


class Widget {
    
public:
    
    virtual void draw() = 0;
    virtual void setActive(bool active) = 0;
};

#endif
