#pragma once

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Camera.h"
#include "StateBase.h"
#include "StatesStack.h"
#include "Globals.h"
#include "Vertex.h"
#include "Utils.h"
#include "Defines.h"
#include <string>

using namespace std;

#ifdef WIN32
#else
int Init();
void Update(float deltaTime = 0 );
void Draw();
void ProcessMouseFunc ( unsigned int mouseEvent, int x, int y, int id = 0);
void Key (unsigned char key, bool bIsPressed);
#endif
void Pause();
void Resume();
void SetWidthHeight(int w, int h);
void DrawQuad(GLuint textID, GLuint vertexBuffer, int x, int y, int width, int height);
extern GLuint LoadAndBindTexture(const char* textureDir, int* iWidth, int* iHeight, int* bpp);
extern void DrawCoordinateSystem();
extern void DeleteBuffer(GLsizei n, GLuint* buffer);