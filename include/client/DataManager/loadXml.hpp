#pragma once 

#include "characterStruct.hpp"
#include "persona.hpp"
#include "partBase.hpp"

// LoadXml 负责解析 TMWA 资源 XML 并构建客户端角色模板数据
class LoadXml
{
public:
    // 解析图片集 XML 文件。
    static ImageSet parseImageXml(const std::string& path);
    // 打印图片集解析结果用于调试。
    static void printImageXml(const ImageSet& imageSet);

    // 解析精灵动作 XML 文件。
    static SpriteData parseSpriteXML(const std::string& path);
    // 打印精灵动作数据用于调试。
    static void printSpriteData(const SpriteData& spriteData);
    
    // 解析一个完整人物模板的 XML。
    static void parsePersonaXml(const std::string& path, std::shared_ptr<Persona> persona);

    // 解析一个部件 XML 并填充到 PartBase。
    static void parsePartBaseXml(const std::string& path, PartBase& part);
    // 解析带额外扩展路径格式的部件 XML。
    static void parsePartBaseXmlEx(std::string& path, PartBase& part);

    // 扫描并解析所有可用人物模板。
    static std::vector<std::shared_ptr<Persona>> parseAllPersonaXml();

public:
    // 资源 XML 根目录
    static std::string m_xmlPath;
    // 图形资源根目录
    static std::string m_graphicsPath;
    // NPC 模板 XML 目录
    static std::string m_npcXmlPath;
    // 中性动作模板 XML 目录
    static std::string m_neutralXmlPath;
};
