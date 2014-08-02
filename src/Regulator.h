//
//  Regulator.h
//  DrawScore
//
//  Created by etienne cella on 2014-07-13.
//
//

#ifndef DrawScore_Regulator_h
#define DrawScore_Regulator_h

class Regulator {
    
public:
    
    float setPoint;
    float output;
    float minOutput;
    float maxOutput;
    float gain;
    float error;
    
    Regulator(){
        gain = 1;
        output = 0;
        error = 1;
        minOutput = 0;
        maxOutput = 1;
        setPoint = .5f;
    }
    
    float update(float feedback){
        error = setPoint - feedback;
        output += feedback > setPoint ? -gain : gain;
        output = min(maxOutput, max(minOutput, output));
        return output;
    }
};

#endif