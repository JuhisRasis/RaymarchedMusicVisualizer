#pragma once
#include <stdio.h>
#include <GLEW/GL/glew.h>

void CheckOpenGLErrors()
{
	GLuint err = glGetError();
	if (err != GL_NO_ERROR)
	{
		switch (err)
		{
		case GL_INVALID_ENUM:
			fprintf(stderr, "Open GL error invalid enum\n");
			break;
		case GL_INVALID_VALUE:
			fprintf(stderr, "Open GL error invalid value\n");
			break;
		case GL_INVALID_OPERATION:
			fprintf(stderr, "Open GL error invalid operation\n");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			fprintf(stderr, "Open GL error invalid frame buffer operation\n");
			break;
		case GL_OUT_OF_MEMORY:
			fprintf(stderr, "Open GL error out of memory\n");
			break;
		default:
			fprintf(stderr, "Open GL error code: %d\n", (int)err);
			break;
		}
	}
}