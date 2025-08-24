#pragma once 

#include "publicStruct.hpp"

class LoadXml
{
public:
    static ImageSet parseImageXml(const std::string& path);
    static void printImageXml(const ImageSet& imageSet);

    static SpriteData parseSpriteXML(const std::string& path);
    static void printSpriteData(const SpriteData& spriteData);
};