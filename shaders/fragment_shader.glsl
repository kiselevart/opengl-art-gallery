#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Light light;
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform sampler2D texture1;

// Function to calculate directional light contribution
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    // Combine results
    vec3 ambient = light.ambient * texture(texture1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture1, TexCoords).rgb;
    vec3 specular = light.specular * spec;
    
    return (ambient + diffuse + specular);
}

// Function to calculate point light contribution
vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
    // Combine results
    vec3 ambient = light.ambient * texture(texture1, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(texture1, TexCoords).rgb;
    vec3 specular = light.specular * spec;
    
    return (ambient + diffuse + specular);
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Calculate both lighting contributions
    vec3 dirResult = CalcDirLight(dirLight, norm, viewDir);
    vec3 pointResult = CalcPointLight(light, norm, FragPos, viewDir);
    
    // Combine both lighting results
    vec3 result = dirResult + pointResult;
    
    FragColor = vec4(result, 1.0);
}