//
//  ScoreReader.h
//  DrawScore
//
//  Created by etienne cella on 2014-07-06.
//
//

#ifndef DrawScore_ImgProcess_h
#define DrawScore_ImgProcess_h

#include <algorithm>  

using namespace std;

namespace ImgProcess {

    // convert color to grayscale using luminosity method
    unsigned char * extractChannel(unsigned char * input, int width, int height, int channel);
    
    // convert color to grayscale using luminosity method
    unsigned char * grayScale(unsigned char * input, int width, int height);
    
    // convert to binary
    unsigned char * threshold(unsigned char * input, int width, int height, unsigned char threshold);
    
    unsigned char * offset(unsigned char * input, int width, int height, unsigned char offset);
    
    // meant to be used on a thresholded image
    float averagePixelValue(unsigned char * input, int width, int height);
    
    // crop an image
    unsigned char * crop(unsigned char * input, int sourceWidth, int sourceHeight, int x, int y, int width, int height);
    
    // convert from 1 to 3 channels image (ex: grayscale -> RGB)
    unsigned char * oneToThreeChannels(unsigned char * input, int width, int height);
    
    void yAxisHistogram(unsigned char * input, float * output, int width, int height);
};

#endif
