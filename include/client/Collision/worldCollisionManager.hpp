#pragma once

class WorldCollisionManager
{
public:
    static WorldCollisionManager& getInstance();

    void setWorldBounds(int width, int height);
    bool canOccupyPlayerFootbox(int centerX, int footY) const;

private:
    WorldCollisionManager() = default;
    WorldCollisionManager(const WorldCollisionManager&) = delete;
    WorldCollisionManager& operator=(const WorldCollisionManager&) = delete;

private:
    int m_worldWidth = 1600;
    int m_worldHeight = 1200;
};
