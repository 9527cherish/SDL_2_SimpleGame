#include "createCharacter.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Neutral.hpp"


int main(int argc, const char** argv) {

    CreateCharacter createCharacter;
    createCharacter.parseAllNpcXml();
    // createCharacter.parseNeutralXml();
    // createCharacter.parseHairstyleXml();
    // createCharacter.printNPCData();

    NPC npc = createCharacter.parseSingleNpcXml();
    
    std::vector<SpritePart> vecSpritePart = npc.parts;

    // 解析 basexml
    std::string basePath = createCharacter.getGraphicsPath() +  vecSpritePart[0].path;
    ImageSet baseImage = createCharacter.parseImageXml(basePath);
    spdlog::info("");
    spdlog::info("==========baseImage==========");
    createCharacter.printImageXml(baseImage);
    
    std::string baseFile = createCharacter.getGraphicsPath() + baseImage.file;
    SpriteData baseSprite = createCharacter.parseSpriteXML(baseFile);
    spdlog::info("");
    spdlog::info("==========baseSprite==========");
    createCharacter.printSpriteData(baseSprite);



    // 解析 hairstyleXml
    std::string hairstylePath = createCharacter.getGraphicsPath() +  vecSpritePart[1].path;
    ImageSet hairstyleImage = createCharacter.parseImageXml(hairstylePath);
    spdlog::info("");
    spdlog::info("==========hairstyleImage==========");
    createCharacter.printImageXml(hairstyleImage);

    std::string hairstyleFile = createCharacter.getGraphicsPath() + hairstyleImage.file;
    SpriteData hairstyleSprite = createCharacter.parseSpriteXML(hairstyleFile);
    spdlog::info("");
    spdlog::info("==========hairstyleSprite==========");
    createCharacter.printSpriteData(hairstyleSprite);

    // 解析feet
    std::string feetPath = createCharacter.getGraphicsPath() +  vecSpritePart[2].path;
    ImageSet feetImage = createCharacter.parseImageXml(feetPath);
    spdlog::info("");
    spdlog::info("==========feetImage==========");
    createCharacter.printImageXml(feetImage);

    // 解析legs
    std::string legsPath = createCharacter.getGraphicsPath() +  vecSpritePart[3].path;
    ImageSet legsImage = createCharacter.parseImageXml(legsPath);
    spdlog::info("");
    spdlog::info("==========legsImage==========");
    createCharacter.printImageXml(legsImage);

    // 解析chest
    std::string chestPath = createCharacter.getGraphicsPath() +  vecSpritePart[4].path;
    ImageSet chestImage = createCharacter.parseImageXml(chestPath);
    spdlog::info("");
    spdlog::info("==========chestImage==========");
    createCharacter.printImageXml(chestImage);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        spdlog::error("SDL初始化失败: " + std::string(SDL_GetError()));
        return 1;
    }
    
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        spdlog::error("SDL_image初始化失败: " + std::string(IMG_GetError()));
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL2 Player character",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        spdlog::error( "无法创建窗口: " + std::string(SDL_GetError()) );
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        spdlog::error( "无法创建渲染器: " + std::string(SDL_GetError()) );
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Neutral* neutral = new Neutral(baseImage, baseSprite);
    neutral->initTexture(renderer);

    Neutral* hairstyle = new Neutral(hairstyleImage, hairstyleSprite);
    hairstyle->initTexture(renderer);

    Neutral* feet= new Neutral(feetImage, baseSprite);
    feet->initTexture(renderer);

    Neutral* legs = new Neutral(legsImage, baseSprite);
    legs->initTexture(renderer);

    Neutral* chest = new Neutral(chestImage, baseSprite);
    chest->initTexture(renderer);
    
    // 初始状态
    CharacterAction current_action = CharacterAction::STAND;
    Direction current_dir = Direction::DOWN;
    std::string currentAction = "stand";
    int x = 400 - 32; // 居中显示
    int y = 300 - 32;
    int current_variant = 0;
    
    // 主循环控制
    bool quit = false;
    SDL_Event e;
    Uint32 last_frame_time = SDL_GetTicks();
    
    while (!quit) {
        // 计算delta时间
        Uint32 current_time = SDL_GetTicks();
        int delta_time = current_time - last_frame_time;
        last_frame_time = current_time;
        
        // 处理事件
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_w:
                        current_dir = Direction::UP;
                        current_action = CharacterAction::WALK;
                        currentAction = "walk";
                        y -= 2;
                        break;
                    case SDLK_s:
                        current_dir = Direction::DOWN;
                        current_action = CharacterAction::WALK;
                        currentAction = "walk";
                        y += 2;
                        break;
                    case SDLK_a:
                        current_dir = Direction::LEFT;
                        current_action = CharacterAction::WALK;
                        currentAction = "walk";
                        x -= 2;
                        break;
                    case SDLK_d:
                        current_dir = Direction::RIGHT;
                        current_action = CharacterAction::WALK;
                        currentAction = "walk";
                        x += 2;
                        break;
                    case SDLK_SPACE:
                        current_action = CharacterAction::ATTACK;
                        currentAction = "attack";
                        // sprite.resetAnimation(current_action, current_dir);
                        break;
                    case SDLK_1:
                        current_action = CharacterAction::STAND;
                        currentAction = "stand";
                        break;
                    case SDLK_2:
                        current_action = CharacterAction::SIT;
                        currentAction = "sit";
                        break;
                    case SDLK_3:
                        current_action = CharacterAction::DEAD;
                        currentAction = "dead";
                        break;
                    case SDLK_4:
                        current_action = CharacterAction::ATTACK_SWORD_STAB;
                        // sprite.resetAnimation(current_action, current_dir);
                        currentAction = "attack_sword_stab";
                        break;
                    case SDLK_5:
                        current_action = CharacterAction::ATTACK_BOW;
                        // sprite.resetAnimation(current_action, current_dir);
                        currentAction = "attack_sword_stab";
                        break;
                    case SDLK_6:
                        current_action = CharacterAction::CAST;
                        // sprite.resetAnimation(current_action, current_dir);
                        
                        currentAction = "attack_bow";
                        break;
                    case SDLK_v:
                        current_variant = (current_variant + 1) % 135;
                        break;
                }
            } else if (e.type == SDL_KEYUP) {
                // 当移动键释放时，回到站立状态
                if ((e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_s || 
                     e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_d) && 
                    currentAction == "walk") {
                    currentAction = "stand";
                    
                }
            }
        }
        
        // 限制位置在窗口内
        if (x < 0) x = 0;
        if (x > 800 - 64) x = 800 - 64;
        if (y < 0) y = 0;
        if (y > 600 - 64) y = 600 - 64;
        
        neutral->update(currentAction, current_dir, delta_time);
        hairstyle->update(currentAction, current_dir, delta_time);
        feet->update(currentAction, current_dir, delta_time);
        legs->update(currentAction, current_dir, delta_time);
        chest->update(currentAction, current_dir, delta_time);
        // 清空屏幕
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        neutral->render(renderer, currentAction, current_dir, x, y);
        hairstyle->render(renderer, currentAction, current_dir, x, y);
        feet->render(renderer, currentAction, current_dir, x, y);
        legs->render(renderer, currentAction, current_dir, x, y);
        chest->render(renderer, currentAction, current_dir, x, y);

        SDL_RenderPresent(renderer);
        // 控制帧率
        SDL_Delay(16); // 约60FPS
    }
    
    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    
    return 0;
 
}