//
//  Synth.cpp
//  DrawScore
//
//  Created by Etienne on 2014-10-24.
//
//

#include "Synth.h"

Synth::Synth(){
    volume = 0.f;
    mul = 0.f;
    curMul = 0.f;
    mulInterpolation = 0.f;
    phase = 0.f;
    frequency = 0.f;
    TWOPI_BY_SAMPLERATE = TWOPI / 44100.f;
}

void Synth::processAudio(float * output, int bufferSize){
    for (int i = 0; i < bufferSize; i++){
        phase += frequency * TWOPI_BY_SAMPLERATE;
		if (phase >= TWOPI) phase -= TWOPI;
        curMul += mulInterpolation * (mul - curMul);
		output[i] += volume * curMul * sin(phase);
	}
}

void Synth::noteOn(float value_){
    mul = value_;
}