#pragma once

#include "StringUtilities.hpp"
#include <GL/glew.h>
#include <iostream>

void printShaderLog(GLuint shader);

void printProgramLog(int prog);

bool checkOpenGLError();