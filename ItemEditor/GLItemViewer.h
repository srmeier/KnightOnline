#ifndef __GLITEMVIEWER_H_
#define __GLITEMVIEWER_H_

#include "defines.h"

//-----------------------------------------------------------------------------
class GLItemViewer: public Fl_Gl_Window {
private:
	bool bShadersBuilt;

	GLint glUniModel;
	GLint glUniView;
	GLint glUniProj;

	GLuint glVertArray;
	GLuint glVertBuffer;
	GLuint glEleBuffer;
	GLuint glTeXBuffer;
	GLuint glProgram;

	float min_y;
	float max_x;
	float max_y;
	float max_z;

	float pDist;
	float pDistP;

private:
	void draw(void);
	bool BuildShaders(void);

	static void RenderCallBack(void* data) {
		GLItemViewer* item_viewer = (GLItemViewer*)data;
		item_viewer->redraw();
		Fl::repeat_timeout(1.0/24.0, RenderCallBack, data);
	}

public:
	void PushDataToGPU(void);

public:
	GLItemViewer(int x, int y, int w, int h, const char* l = NULL);
};

#endif
