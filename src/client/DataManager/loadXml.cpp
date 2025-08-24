#include "loadXml.hpp"
#include <pugixml.hpp>

ImageSet LoadXml::parseImageXml(const std::string &path)
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

void LoadXml::printImageXml(const ImageSet &imageSet)
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

SpriteData LoadXml::parseSpriteXML(const std::string &path)
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
        action.action =  ActionMapper::from_string(actionNode.attribute("name").as_string());
        action.imageset = actionNode.attribute("imageset").as_string();
        
        // 解析每个方向的动画
        for (pugi::xml_node animNode : actionNode.children("animation")) {
            action.direction = DirectionMapper::from_string(animNode.attribute("direction").as_string());
            
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
            spriteData.animations[action] = sequence;
        }           
    }
    
    return spriteData;
}

void LoadXml::printSpriteData(const SpriteData &spriteData)
{
    spdlog::info("----------Sprite Data ----------");
    spdlog::info("Variants: " + std::to_string(spriteData.variants));
    spdlog::info("Variant Offset: " + std::to_string(spriteData.variantOffset));
    spdlog::info("ImageSet: " + spriteData.imageSetName);
    spdlog::info("Source: " +  spriteData.imageSetSrc);
    spdlog::info("Frame Size: " + std::to_string(spriteData.frameWidth) 
        + "*"  + std::to_string(spriteData.frameHeight));

    spdlog::info("Actions:");


    for (const auto& [action, sequence] : spriteData.animations) {
        spdlog::info("  Action Name: "  + ActionMapper::to_string(action.action)  + " (ImageSet: " + action.imageset + ")"
                        + "  Action Direction: "  + DirectionMapper::to_string(action.direction) );

            if(sequence.frames.size() < 1)
            {
                continue;
            }
            spdlog::info("    Direction: " + DirectionMapper::to_string(action.direction) + " - Frames: " + std::to_string(sequence.frames.size()));
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
