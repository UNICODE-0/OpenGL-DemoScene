#include "FileSystemUtilities.hpp"

std::string readFromFile(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ifstream::in);
	if(!fileStream)
	{
		std::cout << getFormattedString("Can't read file, path does not exist: ", StringColor::Red, StringType::Bold) << filePath << std::endl;
		std::exit(1);
	} 
	
	std::string line = "";
	while(!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + '\n');
	}

	fileStream.close();
	return content;
}

GLuint loadTexture(const char *texImagePath)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(texImagePath, &width, &height, &nrChannels, 0);
	if(data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    	glGenerateMipmap(GL_TEXTURE_2D);

		if(glewIsSupported("GL_EXT_texture_filter_anisotropic"))
		{
			GLfloat anisoSetting = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting);
		}
	}
	else
	{
		std::cout << getFormattedString("Can't load texture, path does not exist: ", StringColor::Red, StringType::Bold) << texImagePath << std::endl;
	}

	stbi_image_free(data);
	
	return textureID;
}