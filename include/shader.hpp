#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>

class Shader {
public:
  // program ID
  unsigned int ID;

  // constructor
  Shader(const char *vertexPath, const char *fragmentPath);

  // use function
  void use();

  // utility/uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif
