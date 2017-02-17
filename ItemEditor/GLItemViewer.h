#ifndef __GLITEMVIEWER_H_
#define __GLITEMVIEWER_H_

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "fl/gl.h"
#include "fl/fl_gl_window.h"

//-----------------------------------------------------------------------------
class GLItemViewer: public Fl_Gl_Window {
private:
	bool bShadersBuilt;

private:
	bool BuildShaders(void);
	static void RenderCallBack(void* data) {
		GLItemViewer* item_viewer = (GLItemViewer*)data;
		item_viewer->redraw();
		Fl::repeat_timeout(1.0/24.0, RenderCallBack, data);
	}

public:
	GLItemViewer(int x, int y, int w, int h, const char* l = NULL);
};

#endif
