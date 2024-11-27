#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <string>
#include "stb_image.h"

GLuint loadTexture(const std::string &path);

#endif
