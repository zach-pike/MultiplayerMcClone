#pragma once

#include <memory>
#include <string>

#include "Utility/GL/ShaderProgram/ShaderProgram.hpp"

std::unique_ptr<ShaderProgram> loadVertexFragmentShader(std::string basePath);