//
//  ScoreReader.h
//  DrawScore
//
//  Created by etienne cella on 2014-07-06.
//
//

#ifndef DrawScore_ScoreReader_h
#define DrawScore_ScoreReader_h

class ScoreReader {
    
    public:
    
        // TODO: optimmize: keep one channel only once we're in grayscale, expand to 3 channel
        // in the end, for rendering
    
        // convert color to grayscale using luminosity method, keeps 3 channels
        static unsigned char * grayScale(unsigned char * input, int width, int height);
    
        // convert to binary, keeps 3 channels
        static unsigned char * threshold(unsigned char * input, int width, int height, unsigned char threshold);
    
        // meant to be used on a thresholded image
        static float averagePixelValue(unsigned char * input, int width, int height);
    
        // crop an image
        static unsigned char * crop(unsigned char * input, int sourceWidth, int sourceHeight, int x, int y, int width, int height);
    
        //
        static unsigned char * oneToThreeChannels(unsigned char * input, int width, int height);


    
    
    
};

#endif
