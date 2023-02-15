#version 460

in vec3 imvNormal;
in vec3 lightDir;
in vec3 viewVertPos;
in vec3 lightDirHalfVec;
in vec2 texCoord;

struct PointLight
{
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform PointLight light;
uniform Material material;
uniform vec4 globalAmbient;
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
uniform mat4 inverseMV;
uniform float time;

layout (binding = 0) uniform sampler2D samp;

out vec4 fragColor;

void main(void)
{
    vec3 nLightDir = normalize(lightDir);
    vec3 nImvNormal = normalize(imvNormal);
    vec3 nLightDirHalfVec = normalize(lightDirHalfVec);

    float lightAndNormalAngle = dot(nLightDir,nImvNormal);
    float halfLightAndNormalAngle = dot(nLightDirHalfVec,nImvNormal);

    vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(lightAndNormalAngle, 0.0);
    vec3 specular = material.specular.xyz * light.specular.xyz * pow(max(halfLightAndNormalAngle, 0.0), material.shininess * 3.0);
    
    fragColor = vec4(texture(samp, texCoord).xyz * (ambient + diffuse + specular).xyz, 1.0);
}