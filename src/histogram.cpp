#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <math.h>


int main(int argc, char const *argv[])
{
    std::string line;
    std::ifstream myfile2 ("../img/example.pgm");
    if (myfile2.is_open())
    {
        std::string fileFormat;
        int width, height;

        getline (myfile2,line);
        fileFormat = line;

        getline (myfile2,line);

        std::istringstream iss(line);

        std::string s; iss >> s;
        width = std::stoi(s);
        // std::cout << s << '\n';

        iss >> s;
        height = std::stoi(s);
        // std::cout << s << '\n';
        int totalNumber = height * width;

        float histogram[256] = {0};
        std::vector<int> imgData;
        float imgDataEqua[totalNumber];
        getline (myfile2,line);

        // for(auto& s: result)
        //     std::cout << s << '\n';
        while ( getline (myfile2,line))
        {
            std::istringstream iss(line);
            // std::cout << line << std::endl;
            for(std::string s; iss >> s; ) {
                histogram[std::stoi(s)]++ ;
                imgData.push_back(std::stoi(s));
            }
        }
        myfile2.close();

        float total = histogram[0] /= totalNumber;
        for (int i = 1; i <= 255; i++) {
            total = histogram[i] = total + histogram[i] / totalNumber;
            // std::cout << i << ": " << histogram[i] << std::endl;
        }
        std::cout << "total: " << total << std::endl;

        for (int i = 0; i < totalNumber; i++) {
        //    imgData[i] = histogram[imgData[i]] * 255;
              imgDataEqua[i] = histogram[imgData[i]] * 255;
        }

        std::ofstream myfile;
        myfile.open ("../img/exampleEqualize.pgm");
        myfile << "P2\n";
        myfile << width << " " << height << "\n";
        myfile << 255 << "\n";

        int t = 0;
        for (int i = 0; i < height; i++) {
            for (int y = 0; y < width; y++) {
                myfile << round(imgDataEqua[t])<< " ";
                if (i + y > 0 && t % 32 == 0 )
                {
                    myfile << "\n";
                }
                t++;
            }
        }

        myfile.close();
    }

    else std::cout << "Unable to open file";
    std::cout << "finished" << std::endl;
    return 0;
}
