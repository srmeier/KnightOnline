/*
*/

#include "GLItemViewer.h"

//-----------------------------------------------------------------------------
GLItemViewer::GLItemViewer(int x, int y, int w, int h, const char* l):
Fl_Gl_Window(x, y, w, h, l) {
	bShadersBuilt = false;
	Fl::add_timeout(1.0/24.0, RenderCallBack, (void*)this);
	end();
}

//-----------------------------------------------------------------------------
bool GLItemViewer::BuildShaders(void) {

	GLenum glError = glewInit();
	if(glError != GLEW_OK) {
		fprintf(stderr, "ERROR(glewInit): %s\n", glewGetErrorString(glError));
		return false;
	}

	glEnable(GL_DEPTH_TEST);



	return true;
}
