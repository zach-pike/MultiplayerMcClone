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

        // Drawing code
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Multiplayer")) {
            ImGui::InputText("Host IP", hostIPAddress, sizeof(hostIPAddress));
            ImGui::InputInt("Host port", &hostPort);


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

        if (client.isConnected()) {
            ImGui::Begin("Server menu");

            if (ImGui::Button("Request vertex data!")) {
                client.sendMessage(Message(1337));
            }

            ImGui::End();
        }

        // Check client for messages
        while(client.isMessageAvailable()) {
            Message m = client.getMessage();

            switch(m.id) {
                case 1337: {
                    world->deserialize(m.data);
                    world->drawWorld();
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