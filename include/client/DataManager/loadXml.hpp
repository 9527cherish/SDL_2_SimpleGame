#pragma once 

#include "characterStruct.hpp"
#include "persona.hpp"
#include "partBase.hpp"

class LoadXml
{
public:
    static ImageSet parseImageXml(const std::string& path);
    static void printImageXml(const ImageSet& imageSet);

    static SpriteData parseSpriteXML(const std::string& path);
    static void printSpriteData(const SpriteData& spriteData);
    
    static void parsePersonaXml(const std::string& path, Persona& persona);

    static void parsePartBaseXml(const std::string& path, PartBase& part);

    static std::vector<Persona> parseAllPersonaXml();

public:
    static std::string m_xmlPath;
    static std::string m_graphicsPath;
    static std::string m_npcXmlPath;
    static std::string m_neutralXmlPath;
};