#ifndef __GLITEMVIEWER_H_
#define __GLITEMVIEWER_H_

#include "defines.h"

#include "GL/glew.h"

#include "fl/fl_window.h"
#include "fl/gl.h"
#include "fl/fl_gl_window.h"
#include "fl/math.h"

//-----------------------------------------------------------------------------
class GLItemViewer: public Fl_Gl_Window {
private:
	bool bShadersBuilt;

	GLuint glVertArray;
	GLuint glVertBuffer;
	GLuint glEleBuffer;
	GLuint glTeXBuffer;
	GLuint glProgram;

	GLint glUniModel;
	GLint glUniView;
	GLint glUniProj;

private:
	void draw(void);
	bool BuildShaders(void);
	static void RenderCallBack(void* data) {
		GLItemViewer* item_viewer = (GLItemViewer*)data;
		item_viewer->redraw();
		Fl::repeat_timeout(1.0/24.0, RenderCallBack, data);
	}

public:
	GLuint getVertArray(void) {return glVertArray;}
	GLuint getVertBuffer(void) {return glVertBuffer;}
	GLuint getEleBuffer(void) {return glEleBuffer;}
	GLuint getTexBuffer(void) {return glTeXBuffer;}
	GLuint getProgram(void) {return glProgram;}

public:
	GLItemViewer(int x, int y, int w, int h, const char* l = NULL);
};

#endif
