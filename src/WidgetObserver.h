//
//  WidgetObserver.h
//  DrawScore
//
//  Created by Etienne on 2014-10-22.
//
//

#ifndef DrawScore_WidgetObserver_h
#define DrawScore_WidgetObserver_h


class WidgetObserver {

public:
    
    virtual void parameterChanged(int id, float value) = 0; // slider
    virtual void parameterChanged(int id, bool value) = 0; // checkbox
};

#endif
