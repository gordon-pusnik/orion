#include "GLFont.h"

#define NUM_FONTS 256

HDC GLFont::m_WGLContext = NULL;

GLFont::GLFont()
{
	m_FontList = 0;
	m_WindowsFont = NULL;
}

GLFont::~GLFont()
{
	if (m_FontList)
		glDeleteLists(m_FontList, NUM_FONTS);
	if (m_WindowsFont)
		::DeleteObject(m_WindowsFont);
}

void GLFont::buildFontGL(char* fontname, short face, GLint size)
{
	int weight;
	int bold		= (face & 0x01);
	int italic		= (face & 0x02);
	int underline	= (face & 0x04);

	if (bold) {
		weight = FW_BOLD;
	}
	else {
		weight = FW_NORMAL;
	} 	

	m_FontList = glGenLists(NUM_FONTS);

	size = -size; // Size has to be negative (Ask Billy why!)

	m_WindowsFont = ::CreateFont ( 
						size,							// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						weight,							// Font Weight
						italic,							// Italic
						underline,						// Underline
						false,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						fontname);						// Font Name

	::SelectObject(m_WGLContext, m_WindowsFont);		// Selects The Font We Want

	::GetTextMetrics(m_WGLContext, &m_TextMetric);

	::wglUseFontBitmapsW(m_WGLContext, 0, NUM_FONTS, m_FontList);
}
