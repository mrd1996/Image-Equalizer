#ifndef IMAGEEQUALIZER_H
#define IMAGEEQUALIZER_H

#include <vector>

/**
 * @brief Class that hold the image equalizer algorithm
 *
 */
class ImageEqualizer
{
public:
    std::vector<int> imgData; // data from imported image
    int intesity; // color range of image mostly 255
    int dimension; // area of image width * height
    std::vector<int> imgEqualized; //number of threads to use on the algorithm
    float histogram[256] = {0}; // histogram for the colors
    int threads; //number of threads to use on the algorithm

    ImageEqualizer(std::vector<int> imgData, int intensity, int dimension, int threads);
    void equalizeImage();
    void equalizeHistogram();
    void buildImage();
    void buildHistogram();
};

#endif