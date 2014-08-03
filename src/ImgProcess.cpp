//
//  ScoreReader.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-12.
//
//

#include "ImgProcess.h"

namespace ImgProcess {
    
    // 3 to 1 channels
    unsigned char * grayScale(unsigned char * input, int width, int height){
        int totalPixels = width * height;
        unsigned char * output = new unsigned char[totalPixels];
        for (int i = 0; i < totalPixels; i++){
            // The formula for luminosity is 0.21 R + 0.72 G + 0.07 B.
            unsigned char value = 0.21 * input[i * 3] + 0.72 * input[i * 3 + 1] + 0.07 * input[i * 3 + 2];
            output[i] = value;
        }
        return output;
    }
    
    unsigned char * extractChannel(unsigned char * input, int width, int height, int channel){
        int totalPixels = width * height;
        unsigned char * output = new unsigned char[totalPixels];
        for (int i = 0; i < totalPixels; i++){
            output[i] = input[i * 3 + channel];
        }
        return output;
    }

    
    // 1 to 1 channel
    unsigned char * threshold(unsigned char * input, int width, int height, unsigned char threshold){
        int totalPixels = width * height;
        unsigned char * output = new unsigned char[totalPixels];
        for (int i = 0; i < totalPixels; i++){
            output[i] = input[i] > threshold ? 255 : 0;
        }
        return output;
    }
    
    // 1 to 1 channel
    unsigned char * offset(unsigned char * input, int width, int height, unsigned char offset){
        int totalPixels = width * height;
        unsigned char * output = new unsigned char[totalPixels];
        for (int i = 0; i < totalPixels; i++){
            output[i] = max(min(input[i] + offset, 255), 0);
        }
        return output;
    }

    // in 1 channel
    float averagePixelValue(unsigned char * input, int width, int height){
        int totalPixels = width * height;
        int sum = 0;
        for (int i = 0; i < totalPixels; i++){
            sum += input[i];
        }
        return sum / totalPixels;
    }
    
    // 3 to 3 channels
    unsigned char * crop(unsigned char * input, int sourceWidth, int sourceHeight, int x, int y, int width, int height){
        int totalPixels = width * height;
        unsigned char * output = new unsigned char[totalPixels * 3];
        for (int i = 0; i < totalPixels; i++){
            int index = 3 * ((x + i % width) + (y + i / width) * sourceWidth);
            output[i * 3] = input[index];
            output[i * 3 + 1] = input[index + 1];
            output[i * 3 + 2] = input[index + 2];
        }
        return output;
    }
    
    // 1 to 3 channels
    unsigned char * oneToThreeChannels(unsigned char * input, int width, int height){
        int totalPixels = width * height;
        unsigned char * output = new unsigned char[totalPixels * 3];
        for (int i = 0; i < totalPixels; i++){
            output[i * 3] = output[i * 3 + 1] = output[i * 3 + 2] = input[i];
        }
        return  output;
    }
        
    unsigned char * lowpass(unsigned char * input, unsigned char * previous, int width, int height, float alpha){
        int totalPixels = width * height;
        unsigned char * output = new unsigned char[totalPixels];
        for (int i = 0; i < totalPixels; i++){
            output[i] = previous[i] + alpha * (input[i] - previous[i]);            
        }
        return output;
    }
    
    void yAxisHistogram(unsigned char * input, float * output, int width, int height){
        float sum;
        for (int i = 0; i < height; ++i){
            sum = 0;
            for (int j = 0; j < width; ++j) sum += input[i * width + j];
            output[i] = sum / width;
        }
    }

}



