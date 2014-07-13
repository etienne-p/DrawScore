//
//  ScoreReader.cpp
//  DrawScore
//
//  Created by etienne cella on 2014-07-12.
//
//

#include "ScoreReader.h"

unsigned char * ScoreReader::grayScale(unsigned char * input, int width, int height){
    int totalPixels = width * height;
    unsigned char * output = new unsigned char[totalPixels * 3];
    for (int i = 0; i < totalPixels; i++){
        // The formula for luminosity is 0.21 R + 0.72 G + 0.07 B.
        unsigned char value = 0.21 * input[i * 3] + 0.72 * input[i * 3 + 1] + 0.07 * input[i * 3 + 2];
        output[i * 3] = output[i * 3 + 1] = output[i * 3 + 2] = value;
    }
    return output;
}

unsigned char * ScoreReader::threshold(unsigned char * input, int width, int height, unsigned char threshold){
    int totalPixels = width * height;
    unsigned char * output = new unsigned char[totalPixels * 3];
    for (int i = 0; i < totalPixels; i++){
        // The formula for luminosity is 0.21 R + 0.72 G + 0.07 B.
        unsigned char value = input[i * 3] > threshold ? 255 : 0;
        output[i * 3] = output[i * 3 + 1] = output[i * 3 + 2] = value;
    }
    return output;
}

float ScoreReader::averagePixelValue(unsigned char * input, int width, int height){
    int totalPixels = width * height;
    int sum = 0;
    for (int i = 0; i < totalPixels; i++){
        sum += input[i];
    }
    return sum / totalPixels;
}

unsigned char * ScoreReader::crop(unsigned char * input, int sourceWidth, int sourceHeight, int x, int y, int width, int height){
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



