#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>
#include<omp.h>
#include "counters.h"

//TODO: Turn this into an object
int width, height, intesity;

void readFile(
    std::string fileName,
    float histogram[],
    std::vector<int> &imgData,
    /** FIXME: to remove later **/
    std::string &format,
    int &width,
    int &height,
    int &intesity)
{
    std::ifstream file(fileName);
    std::string line;

    //TODO: substitute for try/catch later
    if (file.is_open()) {
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
        iss >> intesity;

        //read img data FIXME: possibly move to a function?
        while (getline(file, line)) {
            std::istringstream iss(line);
            for(std::string s; iss >> s;) {
                histogram[std::stoi(s)]++ ;
                imgData.push_back(std::stoi(s));
            }
        }
    }
    file.close();
}

void equalizeHistogram(float histogram[], int intesity, int dimension)
{
    float total = 0;
    int intensityPlus = intesity + 1;

    #pragma omp parallel num_threads(1)
    #pragma omp for
    for (int i = 0; i <= intesity; i++) {
            total = (total + histogram[i] / dimension);
            histogram[i] = round(total * 255);
    }
}

void writeFile(
    std::string fileName,
    float histogram[],
    std::vector<int> &imgData,
    /** FIXME: to remove later **/
    std::string format,
    int width,
    int height,
    int dimension,
    int intesity)
{
    std::ofstream file;
    file.open(fileName);
    if (file.is_open()) {
        file << format << "\n";
        file << width << " " << height << "\n";
        file << intesity << "\n";

        for (int i = 0; i < dimension; i++) {
            file << histogram[imgData[i]] << " ";
            //TODO: improve line break (LOW PRIORITY)
            if (i > 0 && i % 32 == 0 ) {
                file << "\n";
            }
        }
    }
    file.close();
}

void equalizeImage(std::string inFileName) {
    std::string outFileName = inFileName + "E";
    std::string format;

    float histogram[256] = {0};
    std::vector<int> imgData;

    readFile(inFileName, histogram, imgData, format, width, height, intesity);

    int totalNumber = height * width;
    float imgDataEqua[totalNumber];

    equalizeHistogram(histogram, intesity, totalNumber);
    writeFile(outFileName, histogram, imgData, format, width, height, totalNumber, intesity);
}

int main(int argc, char const *argv[])
{
    start();

    equalizeImage(argv[1]);

    stop(-1);

    std::cout << "Dimensions w: " << width << " h:" << height << std::endl;
    printResults();
    return 0;
}
