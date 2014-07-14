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
    float gainAcceleration;
    float gain;
    
    Regulator(){
        minGain = 1;
        gainAcceleration = .2f;
        gain = 1;
        output = 0;
        error = 0;
    }
    
    float update(float feedback){
        float currentError = setPoint - feedback;
        gain *= 1 + (currentError > error ? gainAcceleration : -gainAcceleration);
        gain = max(gain, minGain);
        error = currentError;
        output += feedback > setPoint ? gain : -gain;
        return output;
    }
    
private:
    float error;
};

#endif