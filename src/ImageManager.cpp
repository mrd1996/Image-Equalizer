#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include "ImageManager.h"

std::string imgName;

/**
 * @brief Reads .pgm image and extracts its contents
 *
 * @param fileName // name of file to be read
 * @param imgData  // will have the data of the image
 * @param format   // will have the format of image eg: P2
 * @param width    // will have the width
 * @param height   // will have the height
 * @param intensity // will have color range eg: 255
 */
void ImageManager::readImage(
    std::string fileName,
    std::vector<int> &imgData,
    std::string &format,
    int &width,
    int &height,
    int &intensity
) {
    std::ifstream file(fileName);
    std::string line;

    if (file.is_open()) {
        imgName = fileName;
        //first line has the format eg: P2
        getline (file,line);
        format = line;
        //get next line and check if its a comment
        getline(file,line);
        if (line.at(0) == '#') {
            getline(file,line);
        }
        //get dimensions eg: 1920 1080
        std::istringstream iss(line);
        iss >> width;
        iss >> height;
        //get intesinty eg: 255
        getline(file,line);
        iss.clear();
        iss.str(line);
        iss >> intensity;
        while (getline(file, line)) {
            float section[256];
            std::istringstream iss(line);
            for(std::string s; iss >> s;) {
                imgData.push_back(std::stoi(s));
            }
        }
    }
    file.close();
}

/**
 * @brief Writes image to designated output
 *
 * @param fileName
 * @param format
 * @param width
 * @param height
 * @param intensity
 * @param newimg
 */
void ImageManager::writeImage(std::string fileName, std::string format, int width, int height, int intensity, std::vector<int> newimg) {
    std::ofstream file;
    file.open(fileName);
    if (file.is_open()) {
        file << format << "\n";
        file << width << " " << height << "\n";
        file << intensity << "\n";
        for (const auto& data : newimg) {
            file << data << "\n";
        }
    }
    file.close();
}