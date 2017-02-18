/*
*/

#include "GLItemViewer.h"

//-----------------------------------------------------------------------------
// TEMP
extern e_ItemType eType;
extern int m_iMaxNumIndices0;
extern int m_nFC;

//-----------------------------------------------------------------------------
GLItemViewer::GLItemViewer(int x, int y, int w, int h, const char* l):
Fl_Gl_Window(x, y, w, h, l) {
	bShadersBuilt = false;
	Fl::add_timeout(1.0/24.0, RenderCallBack, (void*)this);
	end();
}

//-----------------------------------------------------------------------------
bool GLItemViewer::BuildShaders(void) {
	if(bShadersBuilt) return true;

	GLenum glError = glewInit();
	if(glError != GLEW_OK) {
		fprintf(stderr, "ERROR(glewInit): %s\n", glewGetErrorString(glError));
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	const GLchar* vertSource = {
		"#version 150 core\n"\
		"\n"\
		"in vec3 pos;\n"\
		"in vec2 texcoord;\n"\
		"\n"\
		"out vec2 fragTexcoord;\n"\
		"\n"\
		"uniform mat4 model;\n"\
		"uniform mat4 view;\n"\
		"uniform mat4 proj;\n"\
		"\n"\
		"void main() {\n"\
			"fragTexcoord = texcoord;\n"\
			"gl_Position = proj*view*model*vec4(pos, 1.0);\n"\
		"}\n"
	};

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);

	GLint status;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		char buffer[512];
		glGetShaderInfoLog(vertShader, 512, NULL, buffer);
		fprintf(stderr, "ERROR(glCompileShader):\n%s\n", buffer);
		return false;
	}

	const GLchar* fragSource = {
		"#version 150 core\n"\
		"\n"\
		"in vec2 fragTexcoord;\n"\
		"uniform sampler2D tex;\n"\
		"\n"\
		"void main() {\n"\
			"gl_FragColor = texture2D(tex, fragTexcoord)*vec4(1.0, 1.0, 1.0, 1.0);\n"\
		"}\n"
	};

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) {
		char buffer[512];
		glGetShaderInfoLog(fragShader, 512, NULL, buffer);
		fprintf(stderr, "ERROR(glCompileShader):\n%s\n", buffer);
		return false;
	}

	glProgram = glCreateProgram();
	glAttachShader(glProgram, vertShader);
	glAttachShader(glProgram, fragShader);
	glLinkProgram(glProgram);

	glGetProgramiv(glProgram, GL_LINK_STATUS, &status);
	if(status == GL_FALSE) {
		char buffer[512];
		glGetProgramInfoLog(glProgram, 512, NULL, buffer);
		fprintf(stderr, "ERROR(glLinkProgram):\n%s\n", buffer);
		return false;
	}

	glUseProgram(glProgram);

	glUniModel = glGetUniformLocation(glProgram, "model");
	glUniView  = glGetUniformLocation(glProgram, "view");
	glUniProj  = glGetUniformLocation(glProgram, "proj");

	glGenVertexArrays(1, &glVertArray);
	glGenBuffers(1, &glVertBuffer);
	glGenBuffers(1, &glEleBuffer);
	glGenTextures(1, &glTeXBuffer);

	bShadersBuilt = true;

	return true;
}

//-----------------------------------------------------------------------------
void GLItemViewer::draw(void) {
	if(!bShadersBuilt) BuildShaders();

	if(!valid()) {
		valid(1);
		glViewport(0, 0, pixel_w(), pixel_h());
	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	static glm::mat4 model = {};
	model = glm::rotate(model, (float)(M_PI/100.0), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 proj;
	proj = glm::perspective(45.0f, (float)pixel_w()/(float)pixel_h(), 1.0f, 120.0f);

	float pDist  = 2.0f;
	float pDistP = 0.2f;
	glm::mat4 view = glm::lookAt(
		glm::vec3(pDist,  pDist, pDist),
		glm::vec3( 0.0f, pDistP,  0.0f),
		glm::vec3( 0.0f,   1.0f,  0.0f)
	);

	glUniformMatrix4fv(glUniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv( glUniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv( glUniProj, 1, GL_FALSE, glm::value_ptr(proj));

	if(eType == ITEM_TYPE_PLUG) {
		glDrawElements(GL_TRIANGLES, m_iMaxNumIndices0, GL_UNSIGNED_INT, 0);
	} else if(eType == ITEM_TYPE_PART) {
		glDrawElements(GL_TRIANGLES, 3*m_nFC, GL_UNSIGNED_INT, 0);
	} else if(eType==ITEM_TYPE_ICONONLY || eType==ITEM_TYPE_GOLD || eType==ITEM_TYPE_SONGPYUN) {
		glm::mat4 iden = {};
		glUniformMatrix4fv(glUniModel, 1, GL_FALSE, glm::value_ptr(iden));
		glUniformMatrix4fv( glUniView, 1, GL_FALSE, glm::value_ptr(iden));
		glUniformMatrix4fv( glUniProj, 1, GL_FALSE, glm::value_ptr(iden));
		glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, 0);
	}
}
