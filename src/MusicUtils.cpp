//
//  MusicUtils.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-29.
//
//

#include "MusicUtils.h"

namespace MusicUtils {
    
    int * getMidiNotes(int rootNote, Mood mood, int count){
    
        // http://www.ijpam.eu/contents/2011-67-2/5/5.pdf
        // http://www.phys.unsw.edu.au/jw/notes.html
        int major[7] = { 2, 2, 1, 2, 2, 2, 1 };
        int minor[7] = { 2, 1, 2, 2, 1, 2, 2 };
        int * scale = mood == MAJOR ? major : minor;
        
        int midiNote = rootNote;
        int * rv = new int[count + 1];
        rv[0] = midiNote;
        
        for (int i = 0; i < count; ++i){
            midiNote += scale[i % 7];
            rv[1 + i] = midiNote;
        }
        return rv;
    }

    
}