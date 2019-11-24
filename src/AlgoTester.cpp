//Records algorithtm's times and produces other metrics
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "Counters.h"
#include "ImageEqualizer.h"
#include "ImageManager.h"

double clearcache [30000000];
ImageManager imgMngr;
std::string outfile;

void clearCache () {
    for (unsigned i = 0; i < 30000000; ++i)
        clearcache[i] = i;
}

/**
 * @brief Executes the algorithm with the number of iterations and threads specified
 *
 * @param imgData // data from imported image
 * @param intensity // color range of image (always 255)
 * @param dimension // area of image width * height
 * @param iterations //number of times the algorithm will be run
 * @param threads //number of threads to use on the algorithm
 * @param store //if user wants to store the equalized image (default off)
 */
void test(
    std::vector<int> &imgData,
    int intensity,
    int width,
    int height,
    int iterations,
    std::vector<int> threads,
    bool store = false
) {
    std::vector<int> imgEqualized;
    int dimension = width * height;
    for (auto const& thread : threads) {
        for (int i = 0; i < iterations; i++) {
            ImageEqualizer hist(imgData, intensity, dimension, thread);
            clearCache();
            start();
            hist.equalizeImage();
            stop();
            std::cout  << thread <<  "," << dimension << "," << getMeasurement(i) << std::endl;
            imgEqualized = hist.imgEqualized;
        }
        clearMeasurements();
    }
    //if wants to store image (disabled by default because of slow write)
    if (store) {
        imgMngr.writeImage(outfile, "P2", width, height, intensity, imgEqualized);
    }
}

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        std::cout  << "USAGE:\n equalizer \e[1mimagepath\e[0m [thread ...] [-print] [-gen] [-it=<iterations>]" << std::endl;
        exit(1);
    }
    std:: string infile = argv[1];
    std::vector<int> threads{2, 4, 6, 8, 10, 12, 16, 20, 24, 28, 32, 38, 40};
    bool print = false;
    int iterations = 5;

    bool foundThread = true;
    bool readImage = true;

    if (argc > 2) {
        for (int i = 2; i < argc; ++i) {
            if ((std::string) argv[i] == "-print") {
                print = true;
                continue;
            }
            std::string it = argv[i];
            if (it.find("-it") != std::string::npos) {
                std::size_t pos = it.find("-it");
                iterations = std::stoi(it.substr(pos+4));
                continue;
            }
            if ((std::string) argv[i] == "-gen") {
                readImage = false;
                continue;
            }
            if (foundThread) {
                threads.clear();
                foundThread = false;
            }
            threads.push_back(std::atoi(argv[i]));
        }
    }

    outfile = infile + "E";
    std::string format;
    int width, height, intensity;
    std::vector<int> imgData;

    if (readImage) {
        imgMngr.readImage(infile, imgData, format, width, height, intensity);
    } else {
        int dimension = std::atoi(argv[1]);
        imgData = std::vector<int>(dimension);
        #pragma omp parallel for
        for (auto i = 0; i < dimension; i++){
            imgData[i] = rand() % 256;
        }
        width = sqrt(dimension);
        height = width;
        intensity = 255;
        outfile = "gen.pgm";
    }

    int dimension = height * width;
    test(imgData, intensity, width, height, iterations, threads, print);

    return 0;
}
