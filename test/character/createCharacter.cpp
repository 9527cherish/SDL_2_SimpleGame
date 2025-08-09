#include "createCharacter.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include "spdlog/sinks/rotating_file_sink.h"
#include <SDL_stdinc.h>


// 将字符串转换为方向枚举
Direction stringToDirection(const std::string& dirStr) {
    if (dirStr == "down") return Direction::DOWN;
    if (dirStr == "left") return Direction::LEFT;
    if (dirStr == "up") return Direction::UP;
    if (dirStr == "right") return Direction::RIGHT;
    if (dirStr == "default") return Direction::DEFAULT;
    return Direction::UNKNOWN;
}


CreateCharacter::CreateCharacter()
{
    logName = "../../logs/test/logfile";
    logPath = "../../logs/test";

    xmlPath = "../../images/tmwa/";
    graphicsPath = "../../images/tmwa/graphics/sprites/";
    npcXmlPath = "npcs/_include.xml";
    neutralXmlPath = "graphics/sprites/model/neutral.xml";
    hairstyleXmlPath = "graphics/sprites/hairstyles/hairstyle01.xml";
    init_logger(logName);

    iFrame = 0;
}

CreateCharacter::~CreateCharacter()
{
}

void CreateCharacter::init_logger(const std::string &log_name)
{
    // 自动创建日志目录
    std::filesystem::create_directories(logPath);

    // 控制台彩色输出
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    // 文件输出（支持每天一个文件）
    auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        log_name, 1024 * 1024 * 5, 3);
    file_sink->set_level(spdlog::level::debug);;

    // 日志器合并
    std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("main_logger", sinks.begin(), sinks.end());

    // 设置为默认 logger
    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::info); // 自动 flush

    // 日志等级与格式
    spdlog::set_level(spdlog::level::debug); // 显示全部
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
}

void CreateCharacter::parseAllNpcXml()
{
    pugi::xml_document doc;
    std::string npcpath = xmlPath + npcXmlPath;
    if (!doc.load_file(npcpath.c_str())) {
        return;
    }

    pugi::xml_node npcs = doc.child("npcs");

    for (pugi::xml_node include : npcs.children("include")) 
    {
        const char* name = include.attribute("name").as_string();
        if (name && name[0] != '\0') 
        {
            vecNpcXmlpath.push_back(name);
        }
    }

}

NPC CreateCharacter::parseSingleNpcXml()
{
    std::string path = xmlPath + vecNpcXmlpath[0];
    return std::move(parseNpcXml(path));
}

std::vector<ColorGroup> CreateCharacter::parseColorGroups(const std::string &colorStr)
{
    std::vector<ColorGroup> groups;
    std::istringstream iss(colorStr);
    std::string groupStr;
    
    // 按分号 ';' 分割颜色组
    while (std::getline(iss, groupStr, ';')) {
        ColorGroup group;
        std::istringstream groupIss(groupStr);
        std::string color;
        
        // 按逗号 ',' 分割颜色
        while (std::getline(groupIss, color, ',')) {
            // 添加 # 前缀（如果不存在）
            if (!color.empty() && color[0] != '#') {
                color = "#" + color;
            }
            group.colors.push_back(color);
        }
        
        if (!group.colors.empty()) {
            groups.push_back(group);
        }
    }
    
    return groups;
}

NPC CreateCharacter::parseNpcXml(const std::string &path)
{
    pugi::xml_document doc;
    if (!doc.load_file(path.c_str())) {
        return NPC();
    }

    // 获取根节点
    pugi::xml_node npcsNode  = doc.child("npcs");
    if (!npcsNode ) {
        return NPC();
    }

     // 遍历所有 <npc> 节点
    for (pugi::xml_node npcNode : npcsNode.children("npc")) {
        NPC npc;
        
        // 获取 NPC ID
        pugi::xml_attribute idAttr = npcNode.attribute("id");
        if (!idAttr) {
            spdlog::error("警告: NPC 缺少 id 属性，跳过");
            continue;
        }
        npc.id = idAttr.as_int();
        
        // 遍历所有 <sprite> 子节点
        for (pugi::xml_node spriteNode : npcNode.children("sprite")) {
            std::string spriteData = spriteNode.text().get();
            
            // 分割路径和颜色数据
            size_t pos = spriteData.find('|');
            if (pos == std::string::npos) {
                spdlog::error("警告: 无效的 sprite 数据格式，跳过: " + spriteData);
                continue;
            }
            
            SpritePart part;
            part.path = spriteData.substr(0, pos);
            std::string colorData = spriteData.substr(pos + 1);
            part.groups = parseColorGroups(colorData);
            
            npc.parts.push_back(part);
        }

        // 只有一个，直接return
        return npc;
    }
    return NPC();
}

void CreateCharacter::parseNpcXml()
{
    std::string path = xmlPath + vecNpcXmlpath[0];
    NPC npc = parseNpcXml(path);
    spdlog::info("parse NpcXml"); 
    printNPCData(npc);

    for (size_t i = 0; i < npc.parts.size(); ++i) 
    {
        const SpritePart& part = npc.parts[i];
        spdlog::info("  部件 #" + std::to_string(i + 1) +  ": "  +  part.path);
    }
}

void CreateCharacter::printNPCData(const NPC& npc)
{
    spdlog::info("---------NPC ID: " + std::to_string(npc.id) + "---------");
    spdlog::info("精灵部件数量: " + std::to_string(npc.parts.size()));
    for (size_t i = 0; i < npc.parts.size(); ++i) 
    {
        const SpritePart& part = npc.parts[i];
        spdlog::info("  部件 #" + std::to_string(i + 1) +  ": "  +  part.path);
        
        for (size_t j = 0; j < part.groups.size(); ++j) {
            const ColorGroup& group = part.groups[j];
            spdlog::info("  颜色组 #" + std::to_string(j + 1) +  ": ");
            
            for (size_t k = 0; k < group.colors.size(); ++k) {
                spdlog::info("      " + group.colors[k]);
            }
            spdlog::info("");
        }
    }
}

void CreateCharacter::printNPCData()
{
    std::string path = xmlPath + vecNpcXmlpath[0];
    NPC npc = parseNpcXml(path);
    spdlog::info("parse NpcXml"); 
    printNPCData(npc);
}

void CreateCharacter::parseNeutralXml()
{
    std::string path = xmlPath + neutralXmlPath;
    SpriteData spriteData = parseSpriteXML(path);
    spdlog::info("parse NeutralXml");
    printSpriteData(spriteData);
}

void CreateCharacter::parseHairstyleXml()
{
    std::string path = xmlPath + hairstyleXmlPath;
    SpriteData spriteData = parseSpriteXML(path);
    spdlog::info("parse HairstyleXml"); 
    printSpriteData(spriteData);
}

ImageSet CreateCharacter::parseImageXml(const std::string &path)
{
    pugi::xml_document doc;
    if (!doc.load_file(path.c_str())) {
        return ImageSet();
    }

        // 获取根节点
    pugi::xml_node spriteNode = doc.child("sprite");
    if (!spriteNode) {
        return ImageSet();
    }

    ImageSet imageSet;
    // 解析 sprite 属性
    imageSet.variants = spriteNode.attribute("variants").as_int(-1);
    imageSet.variantOffset = spriteNode.attribute("variant_offset").as_int(-1);

    for (pugi::xml_node child : spriteNode.children()) 
    {
        // 检查节点类型（元素节点）
        if (child.type() == pugi::node_element) {
            std::string nodeName = child.name();
            
            // 处理 <imageset> 元素
            if (nodeName == "imageset") {
                imageSet.imageSetName = child.attribute("name").as_string();
                std::string src = child.attribute("src").as_string();
                imageSet.frameWidth = child.attribute("width").as_int();
                imageSet.frameHeight = child.attribute("height").as_int();

                std::istringstream srcStream(src);
                
                // 用 | 分割路径和参数
                if (std::getline(srcStream, imageSet.imageSetPath, '|')) {
                    // 提取参数部分
                    std::string param;
                    while (std::getline(srcStream, param, ';')) {
                        if (!param.empty()) {
                            imageSet.imageSetParam.push_back(param);
                        }
                    }
                } else {
                    // 没有 | 分隔符时，整个字符串作为路径
                    imageSet.imageSetPath = src;
                }
            }
            else if (nodeName == "include") {
                imageSet.file = child.attribute("file").as_string();
            }
        }
    }

    return imageSet;
}

void CreateCharacter::printImageXml(const ImageSet &imageSet)
{
    spdlog::info("----------ImageSet  ----------");
    spdlog::info("Variants: " + std::to_string(imageSet.variants));
    spdlog::info("Variant Offset: " + std::to_string(imageSet.variantOffset));
    spdlog::info("ImageSetName: " + imageSet.imageSetName);
    spdlog::info("imageSetPath: " + imageSet.imageSetPath);
    std::string param = "";
    for (size_t i = 0; i < imageSet.imageSetParam.size(); ++i) 
    {
        param = param + imageSet.imageSetParam[i] + " ";
    }
    spdlog::info("imageSetParam: " + param);
    spdlog::info("file: " +  imageSet.file);
    spdlog::info("Frame Size: " + std::to_string(imageSet.frameWidth) 
        + "*"  + std::to_string(imageSet.frameHeight));
}

SpriteData CreateCharacter::parseSpriteXML(const std::string& path)
{
    pugi::xml_document doc;
    if (!doc.load_file(path.c_str())) {
        return SpriteData();
    }

        // 获取根节点
    pugi::xml_node spriteNode = doc.child("sprite");
    if (!spriteNode) {
        return SpriteData();
    }
    
    SpriteData spriteData;
    
    // 解析 sprite 属性
    spriteData.variants = spriteNode.attribute("variants").as_int(-1);
    spriteData.variantOffset = spriteNode.attribute("variant_offset").as_int(-1);
    
    // 解析 imageset
    pugi::xml_node imagesetNode = spriteNode.child("imageset");
    if (imagesetNode) {
        spriteData.imageSetName = imagesetNode.attribute("name").as_string();
        spriteData.imageSetSrc = imagesetNode.attribute("src").as_string();
        spriteData.frameWidth = imagesetNode.attribute("width").as_int();
        spriteData.frameHeight = imagesetNode.attribute("height").as_int();
    }
    
    // 解析所有 action
    for (pugi::xml_node actionNode : spriteNode.children("action")) {
        Action action;
        action.name = actionNode.attribute("name").as_string();
        action.imageset = actionNode.attribute("imageset").as_string();
        
        // 解析每个方向的动画
        for (pugi::xml_node animNode : actionNode.children("animation")) {
            std::string dirStr = animNode.attribute("direction").as_string();
            Direction dir = stringToDirection(dirStr);
            
            AnimationSequence sequence;
            
            // 解析帧或序列
            for (pugi::xml_node frameNode : animNode.children()) {
                std::string nodeName = frameNode.name();
                
                if (nodeName == "frame") {
                    Frame frame;
                    frame.index = frameNode.attribute("index").as_int();
                    frame.delay = 0; // 单帧无延迟

                    frame.offsetX = frameNode.attribute("offsetX").as_int(0);    
                    frame.offsetY = frameNode.attribute("offsetY").as_int(0);
                    frame.delay = frameNode.attribute("delay").as_int(0);

                    sequence.frames.push_back(frame);
                }
                else if (nodeName == "sequence") {
                    int start = frameNode.attribute("start").as_int();
                    int end = frameNode.attribute("end").as_int();
                    int delay = frameNode.attribute("delay").as_int();
                    
                    for (int i = start; i <= end; i++) {
                        Frame frame;
                        frame.index = i;
                        frame.delay = delay;
                        sequence.frames.push_back(frame);
                    }
                }
                else if (nodeName == "end") {
                    sequence.loop = false;
                }
            }
            
            action.directionAnim.animations[dir] = sequence;
        }
        
        spriteData.actions.push_back(action);
    }
    
    return spriteData;
}

void CreateCharacter::printSpriteData(const SpriteData &spriteData)
{
    spdlog::info("----------Sprite Data ----------");
    spdlog::info("Variants: " + std::to_string(spriteData.variants));
    spdlog::info("Variant Offset: " + std::to_string(spriteData.variantOffset));
    spdlog::info("ImageSet: " + spriteData.imageSetName);
    spdlog::info("Source: " +  spriteData.imageSetSrc);
    spdlog::info("Frame Size: " + std::to_string(spriteData.frameWidth) 
        + "*"  + std::to_string(spriteData.frameHeight));

    spdlog::info("Actions:");
    for (const Action& action : spriteData.actions) {
        spdlog::info("  Action Name: "  + action.name  + " (ImageSet: " + action.imageset + ")" );

        for (const auto& [dir, sequence] : action.directionAnim.animations) {
            std::string dirStr;
            switch (dir) {
                case Direction::DOWN: dirStr = "DOWN"; break;
                case Direction::LEFT: dirStr = "LEFT"; break;
                case Direction::UP: dirStr = "UP"; break;
                case Direction::RIGHT: dirStr = "RIGHT"; break;
                case Direction::DEFAULT: dirStr = "DEFAULT"; break;
                default: dirStr = "UNKNOWN"; break;
            }
            if(sequence.frames.size() < 1)
            {
                continue;
            }
            spdlog::info("    Direction: " + dirStr + " - Frames: " + std::to_string(sequence.frames.size()));
            if(sequence.loop)
                spdlog::info("      Loop: Yes" );
            else
                spdlog::info("      Loop: No" );

            for (size_t i = 0; i < sequence.frames.size(); i++) {
                const Frame& frame = sequence.frames[i];
                spdlog::info("      index=" + std::to_string(frame.index)
                         + ", offsetX=" + std::to_string(frame.offsetX)
                         + ", offsetY=" + std::to_string(frame.offsetY)
                         + ", delay=" +  std::to_string(frame.delay) +  "ms" );
            }
             spdlog::info("");
        }
    }
}

std::string CreateCharacter::getXmlPath()
{
    return xmlPath;
}

std::string CreateCharacter::getGraphicsPath()
{
    return graphicsPath;
}

// Color CreateCharacter::parseHexColor(const std::string &hex)
// {
//     if (hex.empty() || hex[0] != '#') 
//         return Color();
    
//     std::string hexDigits = hex.substr(1);
//     if (hexDigits.length() != 6 && hexDigits.length() != 8) 
//         return Color();
    
//     try {
//         Uint8 r = static_cast<Uint8>(std::stoi(hexDigits.substr(0, 2), nullptr, 16));
//         Uint8 g = static_cast<Uint8>(std::stoi(hexDigits.substr(2, 2), nullptr, 16));
//         Uint8 b = static_cast<Uint8>(std::stoi(hexDigits.substr(4, 2), nullptr, 16));
//         Uint8 a = 255;
        
//         if (hexDigits.length() == 8) {
//             a = static_cast<Uint8>(std::stoi(hexDigits.substr(6, 2), nullptr, 16));
//         }
        
//         return Color(r, g, b, a);
//     } catch (...) {
//         return Color();
//     }
// }
