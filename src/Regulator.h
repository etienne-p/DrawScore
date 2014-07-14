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
    
    float minGain;
    float setPoint;
    float output;
    float minOutput;
    float maxOutput;
    float gainAcceleration;
    float gain;
    float error;
    
    Regulator(){
        minGain = 1;
        gainAcceleration = .2f;
        gain = 1;
        output = 0;
        error = 1;
        minOutput = 0;
        maxOutput = 1;
        setPoint = .5f;
    }
    
    float update(float feedback){
        float currentError = setPoint - feedback;
        gain *= 1 + (currentError > error ? gainAcceleration : -gainAcceleration);
        gain = max(gain, minGain);
        error = currentError;
        output += feedback > setPoint ? gain : -gain;
        output = min(maxOutput, max(minOutput, output));
        return output;
    }
    
    
};

#endif