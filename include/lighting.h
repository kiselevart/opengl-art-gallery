#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>
#include "shader.h"      

struct Light {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

void setLightProperties(Shader &shader, const Light &light, const glm::vec3 &viewPos);

#endif
