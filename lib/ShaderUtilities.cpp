#include "ShaderUtilities.hpp"

void setIncludeFromFile(const char *includeName, const char* filename)
{
    char tmpstr[200];
    sprintf(tmpstr, "%s/%s", filename, includeName);
    const char *includeString = readFromFile(tmpstr).c_str();
    sprintf(tmpstr, "/%s", includeName);
	
    glNamedStringARB(GL_SHADER_INCLUDE_ARB, strlen(tmpstr), tmpstr, strlen(includeString), includeString);
}