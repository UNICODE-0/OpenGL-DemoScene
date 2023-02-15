#include "3DModelUtilities.hpp"

WavefrontModel::WavefrontModel(const char *filePath)
{
    WavefrontParser wavefrontParser = WavefrontParser();
    wavefrontParser.parse(filePath);
    numVertices = wavefrontParser.getNumVertices();
    rawVertices = wavefrontParser.getVertices();
    rawTexCoords = wavefrontParser.getTextureCoordinates();
    rawNormals = wavefrontParser.getNormals();

    for (int i = 0; i < numVertices; i++)
    {
        vertices.push_back(glm::vec3(rawVertices[i*3], rawVertices[i*3+1], rawVertices[i*3+2]));
        texCoords.push_back(glm::vec2(rawTexCoords[i*2], rawTexCoords[i*2+1]));
        normals.push_back(glm::vec3(rawNormals[i*3], rawNormals[i*3+1], rawNormals[i*3+2]));
    }
}

int WavefrontModel:: getNumVertices() { return numVertices; }
std::vector<float> WavefrontModel::getRawVertices() { return rawVertices; } 
std::vector<float> WavefrontModel::getRawTextureCoords() { return rawTexCoords;} 
std::vector<float> WavefrontModel::getRawNormals() {return rawNormals; }
std::vector<glm::vec3> WavefrontModel::getVertices() { return vertices; } 
std::vector<glm::vec2> WavefrontModel::getTextureCoords() { return texCoords;} 
std::vector<glm::vec3> WavefrontModel::getNormals() {return normals; }

void WavefrontParser::parse(const char *filePath)
{
    float x, y, z;
    std::string content;
    std::ifstream fileStream(filePath, std::ifstream::in);
    std::string line = "";
    while (!fileStream.eof()) 
    {
        getline(fileStream, line); 
        if (line.compare(0, 2, "v ") == 0)
        {
            std::stringstream stringStream(line.erase(0, 1));
            stringStream >> x;
            stringStream >> y;
            stringStream >> z;
            vertexValues.push_back(x);
            vertexValues.push_back(y);
            vertexValues.push_back(z);
        } else if (line.compare(0, 2, "vt") == 0)
        {
            std::stringstream stringStream(line.erase(0, 2));
            stringStream >> x;
            stringStream >> y;
            textureCoordValues.push_back(x);
            textureCoordValues.push_back(y);
        } else if (line.compare(0, 2, "vn") == 0)
        {
            std::stringstream stringStream(line.erase(0, 2)); 
            stringStream >> x;
            stringStream >> y;
            stringStream >> z; 
            normalValues.push_back(x); 
            normalValues.push_back(y);
            normalValues.push_back(z);
        } else if (line.compare(0, 2, "f ") == 0)
        {
            std::string triangleVertex, vertex, texCoord, normal;
            std::stringstream stringStream(line.erase(0, 2));

            for (int i = 0; i < 3; i++) 
            {
                getline(stringStream, triangleVertex,' '); 
                std::stringstream triangleStringStream(triangleVertex); 
                getline(triangleStringStream, vertex, '/'); 
                getline(triangleStringStream, texCoord, '/'); 
                getline(triangleStringStream, normal, '/');

                int intVertex = (stoi(vertex) - 1) * 3; 
                int intTexCoord = (stoi(texCoord) - 1) * 2;
                int intNormal = (stoi(normal) - 1) * 3;

                triangleVertices.push_back(vertexValues[intVertex]);
                triangleVertices.push_back(vertexValues[intVertex + 1]);
                triangleVertices.push_back(vertexValues[intVertex + 2]);

                textureCoords.push_back(textureCoordValues[intTexCoord]);
                textureCoords.push_back(textureCoordValues[intTexCoord + 1]);

                normals.push_back(normalValues[intNormal]);
                normals.push_back(normalValues[intNormal+ 1]);
                normals.push_back(normalValues[intNormal + 2]);
            }
        }
        
    }
}

WavefrontParser:: WavefrontParser(){}
int WavefrontParser::getNumVertices() { return (triangleVertices.size() / 3); } 
std::vector<float> WavefrontParser::getVertices() { return triangleVertices; } 
std::vector<float> WavefrontParser::getTextureCoordinates() { return textureCoords; } 
std::vector<float> WavefrontParser::getNormals() { return normals; }