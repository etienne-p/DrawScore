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
    
        // convert color to grayscale using luminosity method
        static unsigned char * grayScale(unsigned char * input, int width, int height);
    
        // convert to binary
        static unsigned char * threshold(unsigned char * input, int width, int height, unsigned char threshold);
    
        // meant to be used on a thresholded image
        static float averagePixelValue(unsigned char * input, int width, int height);
    
        // crop an image
        static unsigned char * crop(unsigned char * input, int sourceWidth, int sourceHeight, int x, int y, int width, int height);
    
        //
        static unsigned char * oneToThreeChannels(unsigned char * input, int width, int height);


    
    
    
};

#endif
