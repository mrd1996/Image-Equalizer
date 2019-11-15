#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

int width, height, intesity;

void readFileHist(
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

        // parallelRead(file, histogram, imgData);

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

int main(int argc, char const *argv[])
{
    std::string infile = argv[1];
    std::string outFileName = infile + "D";
    std::string format;

    float histogram[256] = {0}; //FIXME:
    std::vector<int> imgData;

    readFileHist(argv[1], histogram, imgData, format, width, height, intesity);

    int dimension = height * width;

    std::ofstream file;
    file.open(outFileName);
    if (file.is_open()) {
        file << format << "\n";
        file << width * 2 << " " << height * 2 << "\n";
        file << intesity << "\n";

        for (int y = 0; y < 2; y++) {
            for (int k = 0; k < height; k++) {
                for (int z= 0 ;z < 2; z++) {
                    for (int i = 0; i < width; i++) {
                        int row = k* width;
                        file << imgData[row + i] << " ";
                        //TODO: improve line break (LOW PRIORITY)
                        if (i > 0 && i % 32 == 0 ) {
                            file << "\n";
                        }
                    }
                }
            }
        }
    }
    file.close();

    return 0;
}
