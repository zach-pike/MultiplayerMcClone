#include "Game.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Utility/GL/VertexArray/VertexArray.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"

#include "CameraControllers/FreecamController/FreecamController.hpp"

#include "WorldRenderInfo/WorldRenderInfo.hpp"

#include <iostream>
#include <exception>
#include <functional>

#include "Subclasses/DrawableWorld/DrawableWorld.hpp"

#include "Common/GameMessages/GameMessageTypes.hpp"
#include "Common/GameMessages/Connection/Connect/SCConnectInfo/SCConnectInfo.hpp"
#include "Common/GameMessages/Connection/Connect/CSConnectInfo/CSConnectInfo.hpp"
#include "Common/GameMessages/Player/CSUpdatePlayerInfo/CSUpdatePlayerInfo.hpp"
#include "Common/GameMessages/Player/SCPlayerData/SCPlayerData.hpp"

#include "Common/MessagePreprocessor/MessagePreprocessor.hpp"

#include "Utility/ShaderLoaders/VertexFragment/VertexFragment.hpp"

static const GLfloat playerVertexData[] = { 
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f
};

static const GLfloat playerUVData[] = { 
	0.000059f, 1.0f-0.000004f, 
	0.000103f, 1.0f-0.336048f, 
	0.335973f, 1.0f-0.335903f, 
	1.000023f, 1.0f-0.000013f, 
	0.667979f, 1.0f-0.335851f, 
	0.999958f, 1.0f-0.336064f, 
	0.667979f, 1.0f-0.335851f, 
	0.336024f, 1.0f-0.671877f, 
	0.667969f, 1.0f-0.671889f, 
	1.000023f, 1.0f-0.000013f, 
	0.668104f, 1.0f-0.000013f, 
	0.667979f, 1.0f-0.335851f, 
	0.000059f, 1.0f-0.000004f, 
	0.335973f, 1.0f-0.335903f, 
	0.336098f, 1.0f-0.000071f, 
	0.667979f, 1.0f-0.335851f, 
	0.335973f, 1.0f-0.335903f, 
	0.336024f, 1.0f-0.671877f, 
	1.000004f, 1.0f-0.671847f, 
	0.999958f, 1.0f-0.336064f, 
	0.667979f, 1.0f-0.335851f, 
	0.668104f, 1.0f-0.000013f, 
	0.335973f, 1.0f-0.335903f, 
	0.667979f, 1.0f-0.335851f, 
	0.335973f, 1.0f-0.335903f, 
	0.668104f, 1.0f-0.000013f, 
	0.336098f, 1.0f-0.000071f, 
	0.000103f, 1.0f-0.336048f, 
	0.000004f, 1.0f-0.671870f, 
	0.336024f, 1.0f-0.671877f, 
	0.000103f, 1.0f-0.336048f, 
	0.336024f, 1.0f-0.671877f, 
	0.335973f, 1.0f-0.335903f, 
	0.667969f, 1.0f-0.671889f, 
	1.000004f, 1.0f-0.671847f, 
	0.667979f, 1.0f-0.335851f
};

void Game::init() {
    if (!glfwInit()) {
        std::cout << "Failed to init GLFW!\n";
        std::exit(1);
    }

    window = glfwCreateWindow(1024, 768, "Window", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load OpenGL/GLAD!\n";
        std::exit(1);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    glClearColor(.25f, .5f, .75f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    const char* vendor = (char*)glGetString(GL_VENDOR); // Returns the vendor
    const char* renderer = (char*)glGetString(GL_RENDERER); // Returns a hint to the model

    printf("Vendor: %s\nRenderer: %s\n", vendor, renderer);
}

void Game::loop() {
    // VAO
    VertexArray vao;
    vao.bindVertexArray();

    // Camera stuff
    FreecamController cameraController(glm::vec3(0, 0, -5));

    auto worldRenderInfo = std::make_unique<WorldRenderInfo>();
    auto world           = std::make_unique<DrawableWorld>();

    char hostIPAddress[20] = "127.0.0.1";
    int  hostPort = 1500;

    char username[20] = "Zachary";

    int i = 0;

    std::vector<std::string> playerNames;
    std::vector<Vec3fPosition> playerPositions;

    // Player renderer
    auto playerShader = loadVertexFragmentShader("./shader/player/");
    playerShader->use();
    GLuint playerMVPUniform = playerShader->getUniformLocation("MVP");

    bool networkReady = false;


    GLuint pVertexBuffer, pUVBuffer, pPositionBuffer;
    glGenBuffers(1, &pVertexBuffer);
    glGenBuffers(1, &pUVBuffer);
    glGenBuffers(1, &pPositionBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertexData), playerVertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, pUVBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerUVData), playerUVData, GL_STATIC_DRAW);

    double lastFrameStartTime = glfwGetTime();
    float aspectRatio;
    while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        double frameStartTime = glfwGetTime();

        // DeltaT calc
        double deltaTime = frameStartTime - lastFrameStartTime;
    
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        if (windowWidth != 0 && windowHeight != 0) {
            aspectRatio = (float)windowWidth / (float)windowHeight;
        }
        
        // 3D Math stuff...
        glm::mat4 viewMatrix = cameraController.getCamera().getViewMatrix();
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        glm::mat4 viewProjection = projectionMatrix * viewMatrix;
        
        // Camera controller logic
        cameraController.step(window, deltaTime);

        if (client.isConnected() && networkReady) {
            // Send player info
            auto info = std::make_shared<MessageTypes::CSUpdatePlayerInfo>();
            auto pos = cameraController.getCamera().pos;
            info->setPosition(Vec3fPosition{ pos.x, pos.y, pos.z });

            client.sendMessage(processMessage(info));
        }

        // Drawing code
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Multiplayer")) {
            ImGui::InputText("Host IP", hostIPAddress, sizeof(hostIPAddress));
            ImGui::InputInt("Host port", &hostPort);
            ImGui::InputText("Username", username, sizeof(username));


            if (!client.isConnected()) {
                if (ImGui::Button("Connect!")) {
                    asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(hostIPAddress), hostPort);
                    client.connect(endpoint);
                }
            } else {
                if (ImGui::Button("Disconnect!")) {
                    client.disconnect();
                }
            }

            if (ImGui::Button("Reload shaders")) {
                worldRenderInfo->reloadShaders();
            }

            ImGui::End();
        }

        // render world
        world->renderWorld(worldRenderInfo, viewProjection);

        // Setup for player render
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, pVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, pUVBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, pPositionBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3fPosition)*playerPositions.size(), playerPositions.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        playerShader->use();
        glUniformMatrix4fv(playerMVPUniform, 1, GL_FALSE, &viewProjection[0][0]);

        glVertexAttribDivisor(2, 1);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, playerPositions.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        // Check client for messages
        while(client.isMessageAvailable()) {
            Message m = client.getMessage();

            auto t = (MessageTypes::GameMessageType)m.id;

            switch(t) {
                case MessageTypes::GameMessageType::SC_Connect_Info: {
                    // Send Username
                    auto info = std::make_shared<MessageTypes::CSConnectInfo>();
                    info->setUsername(username);

                    client.sendMessage(processMessage(info));

                    networkReady = true;
                } break;

                case MessageTypes::GameMessageType::SC_Player_Data: {
                    auto info = std::make_shared<MessageTypes::SCPlayerData>();
                    info->deserialize(m.data);

                    playerNames = info->getPlayerUsernames();
                    playerPositions = info->getPlayerPositions();
                } break;
            }
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Frame limiter
        const double frameEndTime = glfwGetTime();
        const double frameTimeMS = (frameEndTime - frameStartTime) * 1e6;
        const double requiredFrameTimeMS = 8333.33;
        const double sleepTime = requiredFrameTimeMS - frameTimeMS;

        if (sleepTime > 0) std::this_thread::sleep_for(std::chrono::microseconds((std::int64_t)sleepTime));

        // Update dt calculation
        lastFrameStartTime = frameStartTime;

        glfwSwapBuffers(window);
        
        // Make viewport always the same size as window
        glViewport(0, 0, windowWidth, windowHeight);
    } 
}

void Game::cleanup() {
    if (client.isConnected()) {
        client.disconnect();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Game::runGame() {
    init();
    loop();
    cleanup();
}