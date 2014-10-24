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
#ifndef PI
#define PI 3.1415927f
#endif
#ifndef TWOPI
#define TWOPI 6.2831853f
#endif

class Synth {
    
public:
    
    float volume, mul, frequency;
    
    void processAudio(float * output, int bufferSize);
    
    Synth();
    
private:
    
    float phase, TWOPI_BY_SAMPLERATE;
};


#endif /* defined(__DrawScore__Synth__) */
