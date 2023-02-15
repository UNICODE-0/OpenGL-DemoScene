#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

class WavefrontModel
{
    private:
        int numVertices;
        std::vector<float> rawVertices;
        std::vector<float> rawTexCoords;
        std::vector<float> rawNormals;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;
    public:
        WavefrontModel(const char *filePath); int getNumVertices();
        std::vector<float> getRawVertices();
        std::vector<float> getRawTextureCoords();
        std::vector<float> getRawNormals();
        std::vector<glm::vec3> getVertices();
        std::vector<glm::vec2> getTextureCoords();
        std::vector<glm::vec3> getNormals();
};
    
class WavefrontParser
{
    private:
        std::vector<float> vertexValues;
        std::vector<float> textureCoordValues;
        std::vector<float> normalValues;
        std::vector<float> triangleVertices;
        std::vector<float> textureCoords;
        std::vector<float> normals;
    public:
        WavefrontParser();
        void parse(const char *filePath);
        int getNumVertices();
        std::vector<float> getVertices();
        std::vector<float> getTextureCoordinates(); 
        std::vector<float> getNormals();
};