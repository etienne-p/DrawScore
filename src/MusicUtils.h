//
//  MusicUtils.h
//  DrawScore
//
//  Created by etienne cella on 2014-07-29.
//
//

#ifndef __DrawScore__MusicUtils__
#define __DrawScore__MusicUtils__

#include <vector>
#include <math.h> 

using namespace std;

namespace MusicUtils {
    
    enum Mood { MINOR, MAJOR };
    
    int * getMidiNotes(int rootNote, Mood mood, int count);
    
    float midiToFrequency(int midiNote);

};

#endif /* defined(__DrawScore__MusicUtils__) */
