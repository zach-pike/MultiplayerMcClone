#include "MyApp.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "Utility/GL/ShaderProgram/ShaderProgram.hpp"
#include "Utility/GL/VertexArray/VertexArray.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"

#include "CameraControllers/FreecamController/FreecamController.hpp"
#include "MyUtility/ShaderLoaders/VertexFragment/VertexFragment.hpp"

#include <iostream>
#include <exception>
#include <functional>

static glm::vec3 cubeVertexData[] = {
    glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3( 1.0f, 1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3( 1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3( 1.0f,-1.0f,-1.0f),
	glm::vec3( 1.0f, 1.0f,-1.0f),
	glm::vec3( 1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3( 1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f,-1.0f, 1.0f),
	glm::vec3( 1.0f,-1.0f, 1.0f),
	glm::vec3( 1.0f, 1.0f, 1.0f),
	glm::vec3( 1.0f,-1.0f,-1.0f),
	glm::vec3( 1.0f, 1.0f,-1.0f),
	glm::vec3( 1.0f,-1.0f,-1.0f),
	glm::vec3( 1.0f, 1.0f, 1.0f),
	glm::vec3( 1.0f,-1.0f, 1.0f),
	glm::vec3( 1.0f, 1.0f, 1.0f),
	glm::vec3( 1.0f, 1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3( 1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f,-1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3( 1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3( 1.0f,-1.0f, 1.0f)
};

static glm::vec3 cubeColorData[] = {
    glm::vec3(0.583f,  0.771f,  0.014f),
	glm::vec3(0.609f,  0.115f,  0.436f),
	glm::vec3(0.327f,  0.483f,  0.844f),
	glm::vec3(0.822f,  0.569f,  0.201f),
	glm::vec3(0.435f,  0.602f,  0.223f),
	glm::vec3(0.310f,  0.747f,  0.185f),
	glm::vec3(0.597f,  0.770f,  0.761f),
	glm::vec3(0.559f,  0.436f,  0.730f),
	glm::vec3(0.359f,  0.583f,  0.152f),
	glm::vec3(0.483f,  0.596f,  0.789f),
	glm::vec3(0.559f,  0.861f,  0.639f),
	glm::vec3(0.195f,  0.548f,  0.859f),
	glm::vec3(0.014f,  0.184f,  0.576f),
	glm::vec3(0.771f,  0.328f,  0.970f),
	glm::vec3(0.406f,  0.615f,  0.116f),
	glm::vec3(0.676f,  0.977f,  0.133f),
	glm::vec3(0.971f,  0.572f,  0.833f),
	glm::vec3(0.140f,  0.616f,  0.489f),
	glm::vec3(0.997f,  0.513f,  0.064f),
	glm::vec3(0.945f,  0.719f,  0.592f),
	glm::vec3(0.543f,  0.021f,  0.978f),
	glm::vec3(0.279f,  0.317f,  0.505f),
	glm::vec3(0.167f,  0.620f,  0.077f),
	glm::vec3(0.347f,  0.857f,  0.137f),
	glm::vec3(0.055f,  0.953f,  0.042f),
	glm::vec3(0.714f,  0.505f,  0.345f),
	glm::vec3(0.783f,  0.290f,  0.734f),
	glm::vec3(0.722f,  0.645f,  0.174f),
	glm::vec3(0.302f,  0.455f,  0.848f),
	glm::vec3(0.225f,  0.587f,  0.040f),
	glm::vec3(0.517f,  0.713f,  0.338f),
	glm::vec3(0.053f,  0.959f,  0.120f),
	glm::vec3(0.393f,  0.621f,  0.362f),
	glm::vec3(0.673f,  0.211f,  0.457f),
	glm::vec3(0.820f,  0.883f,  0.371f),
	glm::vec3(0.982f,  0.099f,  0.879f)
};

void MyApp::init() {
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


    const char* vendor = (char*)glGetString(GL_VENDOR); // Returns the vendor
    const char* renderer = (char*)glGetString(GL_RENDERER); // Returns a hint to the model

    printf("Vendor: %s\nRenderer: %s\n", vendor, renderer);
}

void MyApp::loop() {
    // VAO
    VertexArray vao;
    vao.bindVertexArray();

    // Simple buffer
    Buffer<glm::vec3> vertexBuffer;
    Buffer<glm::vec3> colorBuffer;

    vertexBuffer.bufferData(cubeVertexData);
    colorBuffer.bufferData(cubeColorData);

    // Camera stuff
    FreecamController cameraController(glm::vec3(4, 5, 0));

    // Object shader
    auto objectShader = loadVertexFragmentShader("./shader/object/");
    GLuint objectViewProjectionUniform = objectShader->getUniformLocation("viewProjection");

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

        ImGui::ShowDemoWindow();

        objectShader->use();
        glUniformMatrix4fv(objectViewProjectionUniform, 1, GL_FALSE, &viewProjection[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getBufferId());
        glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer.getBufferId());
        glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        // Render objects
        glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

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

void MyApp::cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void MyApp::runGame() {
    init();
    loop();
    cleanup();
}