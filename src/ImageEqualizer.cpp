#include <vector>
#include <math.h>
#include <omp.h>
#include "ImageEqualizer.h"

/**
 * @brief Construct a new Image Equalizer:: Image Equalizer object
 *
 * @param imgData // data from imported image
 * @param intensity // color range of image (always 255)
 * @param dimension // area of image width * height
 * @param threads //number of threads to use on the algorithm
 */
ImageEqualizer::ImageEqualizer(std::vector<int> imgData, int intensity, long int dimension, int threads):
    imgData(imgData),
    intesity(intensity),
    dimension(dimension),
    threads(threads) {
    imgEqualized.resize(dimension);
}

/**
 * @brief Creates equalized histogram
 *
 * NOTE: This part of the code was not parallelized
 * because its parallelization is not trivial
 * and also because its not an hotstop since
 * it only loops 256 times.
 *
 * Example:
 *
 *  Histogram                Equalized Histogram
 *  0   1    2   ...   255   0   1   2  ...   255
 *  +---+----+----+-----+    +---+---+---+-----+
 *  | 1 | 54 | 23 | 103 | -> | 1 | 3 | 3 | 255 |
 *  +---+----+----+-----+    +---+---+---+-----+
 *
 * Equalized histogram now holds the new value of
 * of its intensity eg: 0 will be 1 in equalized image.
 */
void ImageEqualizer::equalizeHistogram() {
    float total = 0;

    for (int i = 0; i <= intesity; i++) {
            total = (total + histogram[i] / dimension);
            histogram[i] = round(total * 255);
    }
}

/**
 * @brief Builds image based on equalized histogram
 *
 * NOTE: One major hotspot since it looped
 * trough the whole area of the image. We tried various
 * approaches eg: schedule (static|dynamic|guided, 16)
 * where we thought each thread would get a
 * whole line of cache (reducing false sharing)
 * but in practice it didnt improve runtimes.
 */
void ImageEqualizer::buildImage() {
    #pragma omp parallel for num_threads(threads)
    for (long int i = 0; i < dimension; i++) {
        imgEqualized[i] = histogram[imgData[i]];
    }
}

/**
 * @brief Builds histogram from image provided
 *
 * NOTE: One major hotspot since it looped
 * trough the whole area of the image.
 * We initially had an array that contained
 * all array results and at the end of the
 * parallel region we had the single that
 * aggretated all but this solution is more
 * cleaner and had the same performance.
 */
void ImageEqualizer::buildHistogram() {
    #pragma omp parallel for num_threads(threads) reduction(+:histogram[:256])
    for (long int i = 0; i < dimension; i++) {
        histogram[imgData[i]]++;
    }
}

/**
 * @brief Builds and equalizes histogram and image
 *
 */
void ImageEqualizer::equalizeImage() {
    buildHistogram();
    equalizeHistogram();
    buildImage();
}
