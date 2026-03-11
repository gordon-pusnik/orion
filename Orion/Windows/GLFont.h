#ifndef __GLFONT_H__
#define __GLFONT_H__

#include "stdafx.h"
#include <GL/glew.h>

class GLFont
{
public:
	GLFont();
	~GLFont();
	void buildFontGL(char* fontname, short face, GLint size);
	static void setWGLContext(HDC hdc) {m_WGLContext = hdc;}
	static HDC getWGLContext(void) {return m_WGLContext;}
	TEXTMETRIC getTextMetric(void) {return m_TextMetric;}
    HFONT getWindowsFont(void) {return m_WindowsFont;}
	GLuint getFontList() {return m_FontList;}

private:
	static HDC m_WGLContext;
	TEXTMETRIC m_TextMetric;
    HFONT m_WindowsFont;
	GLuint m_FontList;
};

#define ALIGN_LEFT 		0
#define ALIGN_CENTER 	1
#define ALIGN_RIGHT		2

#define	NORMAL			0
#define	BOLD			0x01
#define	ITALIC			0x02

#endif