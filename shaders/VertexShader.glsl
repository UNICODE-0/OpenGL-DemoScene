#version 460
#extension GL_ARB_shading_language_include : require
#include "/TransformationMatrices.glsl"

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoordinate;
layout (location = 2) in vec3 vertexNormal;

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

out vec3 imvNormal;
out vec3 lightDir;
out vec3 viewVertPos;
out vec3 lightDirHalfVec;
out vec2 texCoord;

void main(void)
{
    vec4 color;
    
    viewVertPos = (mvMatrix * vec4(vertexPosition, 1.0)).xyz;
    imvNormal = (inverseMV * vec4(vertexNormal, 1.0)).xyz;
    lightDir = light.position - viewVertPos;
    lightDirHalfVec = (lightDir + (-viewVertPos)).xyz;
    
    texCoord = vec2(textureCoordinate.x, 1 -textureCoordinate.y);
    gl_Position = projMatrix * mvMatrix * vec4(vertexPosition, 1.0);
}