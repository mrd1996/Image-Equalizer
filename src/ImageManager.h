#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <vector>
#include <stdio.h>
#include <iostream>

/**
 * @brief Responsible for reading and writing files
 *
 */
class ImageManager
{
public:
    void readImage(std::string fileName, std::vector<int> &imgData, std::string &format, int &width, int &height, int &intensity);
    void writeImage(std::string fileName, std::string format, int width, int height, int intensity, std::vector<int> newimg);
};

#endif