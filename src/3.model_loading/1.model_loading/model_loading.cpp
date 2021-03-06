#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader_m.h>

#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 800;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 25.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// movement
float pace = 0.0003f;
int movement = 0;

// Position of light and planet
glm::vec3 lightPos(7.0f, -1.0f, 1.0f);

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // flip loaded texture's on the y-axis before loading model.
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader program
    Shader lightingShader("C:/Users/Thodoris/Documents/ME/5.2.light_casters.vs.txt", "C:/Users/Thodoris/Documents/ME/5.2.light_casters.fs.txt");
    Shader lightCubeShader("C:/Users/Thodoris/Documents/ME/5.2.light_cube.vs.txt", "C:/Users/Thodoris/Documents/ME/5.2.light_cube.fs.txt");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    // positions all containers

    glm::vec3 cubePositions[] = {
        glm::vec3(2.0f,  1.0f,   2.5f),
        glm::vec3(4.05f,  3.02f,  2.05f),
        glm::vec3(2.15f,  1.0f,  -3.0f),
        glm::vec3(-1.0f, -4.1f,   1.02f),
        glm::vec3(-3.1f,  1.00f,   -2.0f),
        glm::vec3(0.0f,  -1.26f,  5.0f),
    };

    // cube's VAO
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light's VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // load textures
    unsigned int diffuseMap = loadTexture("C:/Users/Thodoris/Documents/ME/container2.png");
    unsigned int specularMap = loadTexture("C:/Users/Thodoris/Documents/ME/container2_specular.png");

    // shader configuration
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    // build and compile model shaders
    Shader ourShader("C:/Users/Thodoris/Documents/ME/1.model_loading.vs.txt", "C:/Users/Thodoris/Documents/ME/1.model_loading.fs.txt");

    // load models
    Model ourModel("C:/Users/Thodoris/Documents/ME/planet/planet.obj");

    float myspeed = 0.0;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 change = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f);

        // draw light
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", change);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, myspeed, glm::vec3(1.0f, 5.0f, 0.0f));
        model = glm::translate(model, lightPos);
        glm::mat4 round(model);
        model = glm::scale(model, glm::vec3(0.5f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        
        // draw planet
        ourShader.use();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", change);

        model = glm::rotate(model, myspeed/2, glm::vec3(1.0f, 5.0f, 0.0f));
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);
        
        // cubes
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", cameraPos);

        // light properties
        lightingShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        lightingShader.setVec3("light.diffuse", 7.0f, 7.0f, 7.0f);
        lightingShader.setVec3("light.specular", 3.0f, 3.0f, 3.0f);
        lightingShader.setFloat("light.constant", 1.0f);
        lightingShader.setFloat("light.linear", 0.09f);
        lightingShader.setFloat("light.quadratic", 0.032f);

        // material properties
        lightingShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", change);
        lightingShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // render cubes
        glBindVertexArray(cubeVAO);

        glm::mat4 models[6];

        if (movement == 0) {

            myspeed += pace;
            for (unsigned int i = 0; i < 6; i++) {

                model = glm::mat4(1.0f);
                // using the planet's position to rotate around it
                model = model * round;
                model = glm::rotate(model, float(myspeed*1.5), glm::vec3(1.0, 1.0, 1.0));
                model = glm::translate(model, cubePositions[i]);

                // every cube has to rotate differently
                float random = 10.0f * (i + 1);
                if (i % 2 == 0)
                    model = glm::rotate(model, myspeed, glm::vec3(random, random, random));
                else
                    model = glm::rotate(model, myspeed, glm::vec3(-random, -random, -random));

                // make the cubes a bit smaller
                model = glm::scale(model, glm::vec3(0.8f)); 
                lightingShader.setMat4("model", model);

                models[i] = model;

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        else {
            // if movement is paused
            for (unsigned int i = 0; i < 6; i++)
            {
                lightingShader.setMat4("model", models[i]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate all resources
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // terminate
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
    // end execution
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // pause or resume
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        movement = 1;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        movement = 0;

    // make it faster or slower
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        pace += 0.0000001f;
        if (pace >= 0.01f)
            pace = 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        pace -= 0.0000001f;
        if (pace <= 0.00005f)
            pace = 0.00005f;
    }

    // move camera up down left and right
    float cameraSpeed = 3.5 * deltaTime;
    float zoom = cameraSpeed * 2.0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraRight)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraRight)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    // zoom in
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += zoom * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= zoom * cameraFront;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// utility function for loading a 2D texture from file
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
