#pragma once

#include <stb/stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <iostream>

#include "StringUtilities.hpp"

std::string readFromFile(const char* filePath);

GLuint loadTexture(const char *texImagePath);