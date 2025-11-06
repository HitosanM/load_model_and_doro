#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <stb_image.h>
#include <ctime>
#include <iostream>

// callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float camYaw = 0.0f;
float camPitch = 45.0f;

unsigned int cubeVAO = 0, cubeVBO = 0;
void renderCube()
{
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // positions          // normals           // texcoords
            // Back face
            -0.5f, -0.5f, -0.5f, 0.0f,0.0f, -1.0f, 0.0f,0.0f,
             0.5f, 0.5f, -0.5f, 0.0f,0.0f, -1.0f, 1.0f,1.0f,
             0.5f, -0.5f, -0.5f, 0.0f,0.0f, -1.0f, 1.0f,0.0f,
             0.5f, 0.5f, -0.5f, 0.0f,0.0f, -1.0f, 1.0f,1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f,0.0f, -1.0f, 0.0f,0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f,0.0f, -1.0f, 0.0f,1.0f,
            // Front face
            -0.5f, -0.5f, 0.5f, 0.0f,0.0f, 1.0f, 0.0f,0.0f,
             0.5f, -0.5f, 0.5f, 0.0f,0.0f, 1.0f, 1.0f,0.0f,
             0.5f, 0.5f, 0.5f, 0.0f,0.0f, 1.0f, 1.0f,1.0f,
             0.5f, 0.5f, 0.5f, 0.0f,0.0f, 1.0f, 1.0f,1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f,0.0f, 1.0f, 0.0f,1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f,0.0f, 1.0f, 0.0f,0.0f,
            // Left face
            -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f, 1.0f,0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f,0.0f,0.0f, 1.0f,1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,0.0f,0.0f, 0.0f,1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,0.0f,0.0f, 0.0f,1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f,0.0f,0.0f, 0.0f,0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f,0.0f,0.0f, 1.0f,0.0f,
            // Right face
             0.5f, 0.5f, 0.5f, 1.0f,0.0f,0.0f, 1.0f,0.0f,
             0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f,1.0f,
             0.5f, 0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f,1.0f,
             0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f,1.0f,
             0.5f, 0.5f, 0.5f, 1.0f,0.0f,0.0f, 1.0f,0.0f,
             0.5f, -0.5f, 0.5f, 1.0f,0.0f,0.0f, 0.0f,0.0f,
             // Bottom face
             -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,0.0f, 0.0f,1.0f,
              0.5f, -0.5f, -0.5f, 0.0f, -1.0f,0.0f, 1.0f,1.0f,
              0.5f, -0.5f, 0.5f, 0.0f, -1.0f,0.0f, 1.0f,0.0f,
              0.5f, -0.5f, 0.5f, 0.0f, -1.0f,0.0f, 1.0f,0.0f,
             -0.5f, -0.5f, 0.5f, 0.0f, -1.0f,0.0f, 0.0f,0.0f,
             -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,0.0f, 0.0f,1.0f,
             // Top face
             -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,0.0f, 0.0f,1.0f,
              0.5f, 0.5f, 0.5f, 0.0f, 1.0f,0.0f, 1.0f,0.0f,
              0.5f, 0.5f, -0.5f, 0.0f, 1.0f,0.0f, 1.0f,1.0f,
              0.5f, 0.5f, 0.5f, 0.0f, 1.0f,0.0f, 1.0f,0.0f,
             -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,0.0f, 0.0f,1.0f,
             -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,0.0f, 0.0f,0.0f
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// Floor texture
unsigned int floorTexture = 0;
// Wall texture
unsigned int wallTexture = 0;
// Roof texture
unsigned int roofTexture = 0;

const float PLAYER_RADIUS = 1.0f; // Player collision radius

const float MAP_HALF = 245.0f;
const float WALL_LENGTH = 490.0f;
const float WALL_HEIGHT = 20.0f;

// Boat-like player animation state
float g_playerYaw = 0.0f;      // smoothed yaw (radians)
float g_prevPlayerYaw = 0.0f;  // previous frame yaw (radians)

// Boat movement state
float g_boatSpeed = 0.0f;        // current speed (units/sec)
float g_boatHeading = 0.0f;      // heading (radians), 0 => +Z

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL UV Layout", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // load shaders
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load model
    Model ourModel(FileSystem::getPath("resources/objects/3d_daily_ghibli/Untitled.obj"));
    Model doroModel(FileSystem::getPath("resources/objects/3d_doro/Untitled.obj"));
    // load UV Layout texture (for debugging/matching)
    unsigned int uvTexture;
    glGenTextures(1, &uvTexture);
    glBindTexture(GL_TEXTURE_2D, uvTexture);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // important for matching Blender UVs
    unsigned char* data = stbi_load("resources/objects/3d_daily_ghibli/uv.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load UV Layout texture\n";
    }
    stbi_image_free(data);

    // Load floor 
    glGenTextures(1, &floorTexture);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    int flwidth, flheight, flChannels;
    unsigned char* fldata = stbi_load(FileSystem::getPath("resources/textures/skybox/bottom.jpg").c_str(), &flwidth, &flheight, &flChannels, 0);
    if (fldata)
    {
        GLenum format = flChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, flwidth, flheight, 0, format, GL_UNSIGNED_BYTE, fldata);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load floor texture!" << std::endl;
    }
    stbi_image_free(fldata);

    // Load wall texture
    glGenTextures(1, &wallTexture);
    glBindTexture(GL_TEXTURE_2D, wallTexture);
    int wallWidth, wallHeight, wallChannels;
    unsigned char* wallData = stbi_load(FileSystem::getPath("resources/textures/skybox/front.jpg").c_str(), &wallWidth, &wallHeight, &wallChannels, 0);
    if (wallData)
    {
        GLenum wallFormat = wallChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, wallFormat, wallWidth, wallHeight, 0, wallFormat, GL_UNSIGNED_BYTE, wallData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load wall texture!" << std::endl;
    }
    stbi_image_free(wallData);

    // Load roof texture
    glGenTextures(1, &roofTexture);
    glBindTexture(GL_TEXTURE_2D,roofTexture);
    int rfWidth, rfHeight, rfChannels;
    unsigned char* rfData = stbi_load(FileSystem::getPath("resources/textures/skybox/top.jpg").c_str(), &rfWidth, &rfHeight, &rfChannels, 0);
    if (rfData)
    {
        GLenum rfFormat = rfChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, rfFormat, rfWidth, rfHeight, 0, rfFormat, GL_UNSIGNED_BYTE, rfData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load wall texture!" << std::endl;
    }
    stbi_image_free(rfData);

    // Model positions
    glm::vec3 playerPos(0.0f, 0.5f, 0.0f);
    glm::vec3 playerDir(0.0f, 0.0f, 1.0f); // initial facing direction

    // Doro (companion) as an obstacle
    glm::vec3 doroPos = playerPos - glm::vec3(0.0f, 0.0f, 10.0f); // initial world position behind player
    const float DORO_RADIUS = 4.0f; // collision radius for Doro

    // Boat control parameters
    const float MAX_SPEED = 30.0f;          // units/sec forward
    const float MAX_REVERSE = 8.0f;         // reverse speed
    const float ACCEL = 25.0f;              // acceleration units/sec^2
    const float TURN_SPEED_DEG = 60.0f;     // degrees per second at full forward speed
    const float DRAG = 8.0f;                // natural slowdown

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // --- Boat-style movement ---
        // Inputs
        float throttle = 0.0f; // +1 forward, -1 backward
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) throttle += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) throttle -= 1.0f;
        float steer = 0.0f; // -1 left, +1 right
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) steer -= 1.0f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) steer += 1.0f;

        // Update speed with simple acceleration and drag
        if (throttle != 0.0f)
        {
            // stronger acceleration when changing direction
            g_boatSpeed += throttle * ACCEL * deltaTime;
        }
        else
        {
            // apply drag to naturally slow down when not throttling
            if (g_boatSpeed > 0.0f)
            {
                g_boatSpeed -= DRAG * deltaTime;
                if (g_boatSpeed < 0.0f) g_boatSpeed = 0.0f;
            }
            else if (g_boatSpeed < 0.0f)
            {
                g_boatSpeed += DRAG * deltaTime;
                if (g_boatSpeed > 0.0f) g_boatSpeed = 0.0f;
            }
        }

        // Clamp speeds
        if (g_boatSpeed > MAX_SPEED) g_boatSpeed = MAX_SPEED;
        if (g_boatSpeed < -MAX_REVERSE) g_boatSpeed = -MAX_REVERSE;

        // Turning: turn rate scales with forward speed magnitude (so you can pivot more when moving)
        float speedFactor = glm::clamp(glm::abs(g_boatSpeed) / MAX_SPEED, 0.0f, 1.0f);
        // allow a small minimum turn when reversing to help control
        if (glm::abs(g_boatSpeed) < 0.1f && throttle == 0.0f)
            speedFactor = 0.25f; // allow slow turning even when almost stopped

        float turnSpeedRad = glm::radians(TURN_SPEED_DEG) * (0.3f + 0.7f * speedFactor); // base + scaled
        // steering sign: A = left should decrease heading (rotate counter-clockwise looking from top)
        g_boatHeading += -steer * turnSpeedRad * deltaTime; // negative because our forward uses sin(x) for X

        // Move boat along heading
        glm::vec3 forwardDir = glm::normalize(glm::vec3(sin(g_boatHeading), 0.0f, cos(g_boatHeading)));
        playerPos += forwardDir * g_boatSpeed * deltaTime;

        // Wall hit detection: prevent moving through walls
        playerPos.x = glm::clamp(playerPos.x, -MAP_HALF + PLAYER_RADIUS, MAP_HALF - PLAYER_RADIUS);
        playerPos.z = glm::clamp(playerPos.z, -MAP_HALF + PLAYER_RADIUS, MAP_HALF - PLAYER_RADIUS);

        // Doro collision (treat Doro like a wall/obstacle)
        {
            glm::vec3 diff = playerPos - doroPos;
            float dist = glm::length(diff);
            float minDist = PLAYER_RADIUS + DORO_RADIUS;
            if (dist < minDist)
            {
                // avoid division by zero
                glm::vec3 n;
                if (dist > 0.0001f)
                    n = diff / dist;
                else
                    n = -forwardDir; // fallback direction
                // push player outside the Doro collision radius
                playerPos = doroPos + n * minDist;
                // stop the boat so it doesn't push through
                g_boatSpeed = 0.0f;
            }
        }

        // update playerDir for any other logic
        playerDir = forwardDir;
        // --- end boat movement ---

        // Camera position and front
        float camHeight = 30.0f;
        float camRadius = 15.0f;
        float yawRad = glm::radians(camYaw);
        float pitchRad = glm::radians(camPitch);
        glm::vec3 camOffset(
            camRadius * cos(pitchRad) * cos(yawRad),
            camRadius * sin(pitchRad),
            camRadius * cos(pitchRad) * sin(yawRad)
        );
        camera.Position = playerPos + camOffset;
        camera.Front = glm::normalize(playerPos - camera.Position);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            1.0f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // bind UV layout texture
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, uvTexture);
        //ourShader.setInt("texture_diffuse1", 0);

        // Draw ground
        ourShader.setInt("texture_diffuse1", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glm::mat4 groundModel = glm::scale(glm::mat4(1.0f), glm::vec3(WALL_LENGTH, 1.0f, WALL_LENGTH));
        groundModel = glm::translate(groundModel, glm::vec3(0.0f, -0.5f, 0.0f));
        ourShader.setMat4("model", groundModel);
        renderCube();

        for (int i = 0; i < 4; ++i) {
            glm::mat4 wallModel = glm::mat4(1.0f);
            if (i < 2) {
                float zPos = (i == 0) ? -MAP_HALF : MAP_HALF;
                wallModel = glm::translate(wallModel, glm::vec3(0.0f, WALL_HEIGHT, zPos));
         
                wallModel = glm::scale(wallModel, glm::vec3(WALL_LENGTH, WALL_HEIGHT * 2, 1.0f));
            }
            else {
                float xPos = (i == 2) ? -MAP_HALF : MAP_HALF;
                wallModel = glm::translate(wallModel, glm::vec3(xPos, WALL_HEIGHT, 0.0f));
                wallModel = glm::rotate(wallModel, glm::radians(90.0f), glm::vec3(0, 1, 0));
                wallModel = glm::scale(wallModel, glm::vec3(WALL_LENGTH, WALL_HEIGHT * 2, 1.0f));
            }
            ourShader.setMat4("model", wallModel);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wallTexture);
            ourShader.setInt("texture_diffuse1", 0);
            renderCube();
        }

        {
            glm::mat4 roofModel = glm::mat4(1.0f);
            // place roof so its bottom sits on top of walls (walls top = 2 * WALL_HEIGHT)
            float roofThickness = 1.0f; // vertical thickness in world units
            float roofCenterY = 2.0f * WALL_HEIGHT + roofThickness * 0.5f;
            roofModel = glm::translate(roofModel, glm::vec3(0.0f, roofCenterY, 0.0f));
            // roof spans the whole map in X/Z, thin in Y
            roofModel = glm::scale(roofModel, glm::vec3(WALL_LENGTH, roofThickness, WALL_LENGTH));
            ourShader.setMat4("model", roofModel);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, roofTexture);
            ourShader.setInt("texture_diffuse1", 0);
            renderCube();
        }

        // draw model
        //glBindTexture(GL_TEXTURE_2D, uvTexture);
        // compute desired yaw based on boat heading
        float desiredYaw = g_boatHeading;

        // smooth yaw interpolation (radians)
        float prevYawBefore = g_prevPlayerYaw;
        float yawLerpSpeed = 6.0f; // larger = snappier
        float t = glm::clamp(yawLerpSpeed * deltaTime, 0.0f, 1.0f);
        // normalize angle difference to avoid long wrap-around lerps
        float diff = desiredYaw - g_prevPlayerYaw;
        while (diff > glm::pi<float>()) diff -= glm::two_pi<float>();
        while (diff < -glm::pi<float>()) diff += glm::two_pi<float>();
        g_playerYaw = g_prevPlayerYaw + diff * t;

        // compute yaw rate for banking
        float yawRate = 0.0f;
        if (deltaTime > 0.0f)
            yawRate = (g_playerYaw - prevYawBefore) / deltaTime; // radians per second
        // banking angle (roll) proportional to yawRate
        float maxBankDeg = 12.0f; // degrees
        float bankFactor = 2.5f; // adjust responsiveness
        float bankAngleDeg = glm::clamp(-glm::degrees(yawRate) * bankFactor, -maxBankDeg, maxBankDeg);
        float bankAngle = glm::radians(bankAngleDeg);

        // pitch (tilt forward/back) based on forward speed
        float forwardSpeed = glm::clamp(g_boatSpeed / MAX_SPEED, -1.0f, 1.0f);
        float maxPitchDeg = 6.0f;
        float pitchAngle = glm::radians(maxPitchDeg * glm::abs(forwardSpeed));

        // bobbing (vertical) to simulate buoyancy
        float bobSpeed = 2.2f;
        float bobAmplitude = 0.05f; // meters
        float moveInfluence = (glm::abs(g_boatSpeed) > 0.01f) ? 1.0f : 0.25f; // stronger when moving
        float bob = bobAmplitude * moveInfluence * sin(bobSpeed * currentFrame);

        // build player model with boat-like animation
        glm::mat4 playerModel = glm::mat4(1.0f);
        playerModel = glm::translate(playerModel, playerPos + glm::vec3(0.0f, bob, 0.0f));
        // apply yaw (Y), then bank (Z), then pitch (X) to produce local tilt
        playerModel = glm::rotate(playerModel, g_playerYaw, glm::vec3(0.0f, 1.0f, 0.0f));
        playerModel = glm::rotate(playerModel, bankAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        playerModel = glm::rotate(playerModel, pitchAngle, glm::vec3(1.0f, 0.0f, 0.0f));
        playerModel = glm::scale(playerModel, glm::vec3(0.05f));
        ourShader.setMat4("model", playerModel);
        ourModel.Draw(ourShader);

        // draw companion Doro behind the player
        float doroDistance = .0f; // distance behind player
        glm::mat4 doroModelMat = glm::mat4(1.0f);
        // position Doro at its world position (obstacle)
        doroModelMat = glm::translate(doroModelMat, glm::vec3(doroPos.x, 0.0f, doroPos.z));
        // scale to match scene (adjust as needed)
        doroModelMat = glm::scale(doroModelMat, glm::vec3(1.0f));
         // Make sure no previously bound textures (from floor/wall/boat) leak into Doro drawing.
         // Clear a reasonable number of texture units (most GPUs support at least 16)
         for (int tu = 0; tu < 16; ++tu) {
             glActiveTexture(GL_TEXTURE0 + tu);
             glBindTexture(GL_TEXTURE_2D, 0);
         }
         ourShader.setMat4("model", doroModelMat);
         doroModel.Draw(ourShader);
        

        // store yaw for next frame
        g_prevPlayerYaw = g_playerYaw;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// input callbacks
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;
    static bool firstMouse = true;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Invert up/down
    lastX = xpos;
    lastY = ypos;
    float sensitivity = 0.1f;
    camYaw += xoffset * sensitivity;
    camPitch -= yoffset * sensitivity; // Inverted
    camPitch = glm::clamp(camPitch, 0.0f, 89.0f); //0=top-down,89=side
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
