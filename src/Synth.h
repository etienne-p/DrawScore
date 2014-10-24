//
//  Synth.h
//  DrawScore
//
//  Created by Etienne on 2014-10-24.
//
//

#ifndef __DrawScore__Synth__
#define __DrawScore__Synth__

#include <math.h> 

#define PI 3.1415927f
#ifndef TWOPI
#define TWOPI 6.2831853f
#endif

class Synth {
    
public:
    
    float volume, mul, phase, frequency, TWOPI_BY_SAMPLERATE;
    
    void processAudio(float * output, int bufferSize);
    
    Synth();
    
};


#endif /* defined(__DrawScore__Synth__) */
