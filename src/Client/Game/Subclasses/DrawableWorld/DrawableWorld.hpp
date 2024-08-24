#pragma once

#include "Common/World/World.hpp"

#include <memory>
#include "Game/WorldRenderInfo/WorldRenderInfo.hpp"

#include <glm/glm.hpp>

class DrawableWorld : public World {
public:
    DrawableWorld();
    virtual ~DrawableWorld();   

    void drawWorld();

    void renderWorld(std::unique_ptr<WorldRenderInfo>& renderInfo, glm::mat4 viewProjection);

    void generateWorld();
};