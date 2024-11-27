#version 330 core

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;      // Interpolated fragment position from the vertex shader
in vec3 Normal;       // Interpolated normal vector
in vec2 TexCoords;    // Interpolated texture coordinates

out vec4 FragColor;   // Final color output

uniform Light light;        // Light properties
uniform vec3 viewPos;       // Camera position
uniform sampler2D texture1; // Texture

void main() {
    // Ambient lighting
    vec3 ambient = light.ambient * texture(texture1, TexCoords).rgb;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture1, TexCoords).rgb;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Shininess = 32
    vec3 specular = light.specular * spec;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4
