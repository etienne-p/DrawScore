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

using namespace std;

namespace MusicUtils {
    
    enum Mood { MINOR, MAJOR };
    
    void getMidiNotes(vector<int> &v, Mood mood, int count);

};

#endif /* defined(__DrawScore__MusicUtils__) */
