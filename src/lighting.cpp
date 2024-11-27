#include "lighting.h"
#include "shader.h"

void setLightProperties(Shader &shader, const Light &light, const glm::vec3 &viewPos) {
    shader.setVec3("light.position", light.position);
    shader.setVec3("light.ambient", light.ambient);
    shader.setVec3("light.diffuse", light.diffuse);
    shader.setVec3("light.specular", light.specular);
    shader.setVec3("viewPos", viewPos);
}
