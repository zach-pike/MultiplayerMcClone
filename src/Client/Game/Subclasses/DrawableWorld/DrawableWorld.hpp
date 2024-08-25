#pragma once

#include "Common/World/World.hpp"

#include <memory>
#include "Game/WorldRenderInfo/WorldRenderInfo.hpp"

#include <glm/glm.hpp>

class DrawableWorld : public World {
protected:
    void convertChunksToDrawableChunks();
public:
    DrawableWorld();
    virtual ~DrawableWorld();   

    void drawWorld();
    void renderWorld(std::unique_ptr<WorldRenderInfo>& renderInfo, glm::mat4 viewProjection);

    void generateWorld();
    void deserialize(const std::vector<std::uint8_t>& d);
};