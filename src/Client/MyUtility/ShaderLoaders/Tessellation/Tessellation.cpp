#include "Tessellation.hpp"

#include <iostream>

std::unique_ptr<ShaderProgram> loadTessellationShader(std::string basePath) {
    Shader vertexShader(GL_VERTEX_SHADER);
    vertexShader.addSourceCodeFromPath(basePath + "vertex.glsl");
    vertexShader.compile();

    if (!vertexShader.isShaderCompiled()) {
        std::string vertexShaderLog = vertexShader.getInfoLog();
        std::cout << vertexShaderLog << '\n';
        std::exit(1);
    }

    Shader tcsShader(GL_TESS_CONTROL_SHADER);
    tcsShader.addSourceCodeFromPath(basePath + "tcs.glsl");
    tcsShader.compile();

    if (!tcsShader.isShaderCompiled()) {
        std::string tcsShaderLog = tcsShader.getInfoLog();
        std::cout << tcsShaderLog << '\n';
        std::exit(1);
    }

    Shader tesShader(GL_TESS_EVALUATION_SHADER);
    tesShader.addSourceCodeFromPath(basePath + "tes.glsl");
    tesShader.compile();

    if (!tesShader.isShaderCompiled()) {
        std::string tesShaderLog = tesShader.getInfoLog();
        std::cout << tesShaderLog << '\n';
        std::exit(1);
    }

    Shader fragmentShader(GL_FRAGMENT_SHADER);
    fragmentShader.addSourceCodeFromPath(basePath + "fragment.glsl");
    fragmentShader.compile();

    if (!fragmentShader.isShaderCompiled()) {
        std::string fragmentShaderLog = fragmentShader.getInfoLog();
        std::cout << fragmentShaderLog << '\n';
        std::exit(1);
    }

    // Assemble shaders together
    auto program = std::make_unique<ShaderProgram>();

    program->addShader(std::move(vertexShader));
    program->addShader(std::move(tcsShader));
    program->addShader(std::move(tesShader));
    program->addShader(std::move(fragmentShader));

    program->linkProgram();

    if (!program->isProgramLinked()) {
        std::string programLog = program->getInfoLog();
        std::cout << programLog << '\n';
        std::exit(1);
    }

    program->getShaderList().clear();

    return program;
}