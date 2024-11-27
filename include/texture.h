#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <stb_image.h>
#include <string>

GLuint loadTexture(const std::string &path);

#endif
