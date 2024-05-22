#include "shader.hpp"
#include "stb_image.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <glad/glad.h>
#include <glm/ext/quaternion_common.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
glm::quat rot_quat(float theta, glm::vec3 axis);

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720 

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "glfw_window", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // record opengl screen
  std::string cmd_str = "ffmpeg -r 60 -f rawvideo -pix_fmt rgba -s " + std::to_string(WIN_WIDTH) + "x" + std::to_string(WIN_HEIGHT) + " -i - " 
                        "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";
  const char* cmd = cmd_str.c_str();

  FILE* ffmpeg = popen(cmd, "w");

  int* buf_ffmpeg = new int[WIN_WIDTH*WIN_HEIGHT];


  float rect_old[] = {
  // positions           texture coords
  //|-----------------| |---------|
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // top right
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // top left
  };

  float rect[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
  };
  int ind[] = {
    0, 1, 2, 
    3, 0, 2,
  };

  Shader shaderProgram("shaders/shader.vert", "shaders/shader.frag");

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
  glEnableVertexAttribArray(0); 
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // load textures
  unsigned int texture;
  glGenTextures(1, &texture);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set texture wrapping/filtering options for currently bound texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  stbi_set_flip_vertically_on_load(true);
  // get textures
  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  shaderProgram.use();
  shaderProgram.setInt("aTexture", 0);

  // math
  glm::quat rot = rot_quat(10.0f, glm::vec3(0.2f, 0.0f, 1.0f));
  glm::quat target = rot_quat(90.0f, glm::vec3(1.0f, 1.0f, 1.0f));
  glm::mat4 model = glm::mat4_cast(rot);
  float time = glfwGetTime();
  float time_end = 5.0f;

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIN_WIDTH) / static_cast<float>(WIN_HEIGHT), 0.1f, 100.0f);

  shaderProgram.setMat4("model", glm::value_ptr(model));
  shaderProgram.setMat4("view", glm::value_ptr(view));
  shaderProgram.setMat4("projection", glm::value_ptr(projection));
  glEnable(GL_DEPTH_TEST);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.1f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram.use();
    if (time <= time_end) {
      float percent = time / time_end;
      glm::quat rot_quat = glm::slerp(rot, target, percent);
      model = glm::mat4_cast(rot_quat);
      shaderProgram.setMat4("model", glm::value_ptr(model));
      time = glfwGetTime();
    }

    //glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glReadPixels(0, 0, WIN_WIDTH, WIN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, buf_ffmpeg);
    fwrite(buf_ffmpeg, sizeof(int)*WIN_WIDTH*WIN_HEIGHT, 1, ffmpeg);
    glfwPollEvents();
  }
  glfwTerminate();
  pclose(ffmpeg);
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

glm::quat rot_quat(float theta, glm::vec3 axis) {
  glm::vec3 norm = glm::normalize(axis);
  float cos_val = cos(glm::radians(theta / 2));
  float sin_val = sin(glm::radians(theta / 2));
  return glm::quat(cos_val, sin_val * norm.x, sin_val * norm.y, sin_val * norm.z);
}
