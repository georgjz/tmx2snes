#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define cimg_use_png
#include <CImg.h>

#include "InputParser.hpp"

int main(int argc, char** argv)
{
    // parse arguments
    std::vector <std::string> tokens;
    for(auto i = 1; i < argc; ++i)
    {
        tokens.push_back(std::string(argv[i]));
    }

    // get input file name
    std::string inputFileName  { getCmdOption(tokens, "-f") };
    std::string outputFileName { getCmdOption(tokens, "-o") };
    std::string color { getCmdOption(tokens, "-c") };
    std::string wantedTag { "<data encoding=\"csv\">" };

    // check tokens
    if( inputFileName.empty() | outputFileName.empty() | color.empty() )
    {
        std::cout << "Flag(s) missing! Please use all three flags: " << std::endl
                  << "tmx2snes -f inputfile.tmx -c palettenumber -o output.vra" << std::endl;

        return -1;
    }

    // helper strings
    std::string line, data;
    // open input file
    std::ifstream in(inputFileName);

    bool tagFound = false;
    while(std::getline(in, line))
    {
        std::string tmp;
        for(auto i = 0; i < line.length(); ++i)
        {
            if(line[i] == ' ' && tmp.size() == 0) {}

            else
            {
                tmp += line[i];
            }
        }

        if(tmp == wantedTag)
        {
            tagFound = true;
            continue;
        }
        else if(tmp == "</data>")
        {
            tagFound = false;
        }

        if(tagFound)
        {
            data += tmp;
        }
    } // while

    // remove whitespace from data
    data.erase(std::remove_if(
        begin(data), end(data),
        [l = std::locale{}](auto ch) { return std::isspace(ch, l); }
    ), end(data));

    std::string delimiter = ",";
    std::vector<uint32_t> tileMapData;

    size_t pos = 0;
    std::string token;
    while ((pos = data.find(delimiter)) != std::string::npos)
    {
        token = data.substr(0, pos);
        uint32_t num = std::stoul(token);
        tileMapData.push_back(num);
        data.erase(0, pos + delimiter.length());
    }
    // add last element
    tileMapData.push_back(std::stoul(data));

    // create output file
    std::fstream outFile;
    outFile.open(outputFileName, std::ios::binary | std::ios::out);

    // calculate color mask
    uint16_t colorMask = std::stoi(color);
    colorMask = colorMask << 10;

    // convert to uint16_t
    std::vector<uint16_t> snesTileMap;
    for(auto i = 0; i < tileMapData.size(); ++i)
    {
        // substract 1 from data
        uint32_t num = tileMapData[i];
        uint16_t snesNum {0};
        --num;
        // get tile/chr number
        snesNum |= num;
        // snesNum *= 2;
        // set to correct tile number
        snesNum = 2 * (snesNum % 8) + 32 * (snesNum / 8);
        // check H-flip
        if(num & 0x80000000)
            snesNum |= 0x4000;
        // check V-flip
        if(num & 0x40000000)
            snesNum |= 0x8000;
        // set color number
        snesNum |= colorMask;

        uint8_t lower = snesNum & 0x00ff;
        uint8_t higher = snesNum >> 8;
        outFile << lower;
        outFile << higher;
    }

    return 0;
}
