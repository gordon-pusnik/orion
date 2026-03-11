//
//  Renderer.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-31.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Renderer__
#define __Orion__Renderer__

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

enum BlendMode {
	BLEND_SOLID,
	BLEND_ADDITIVE,
	BLEND_ALPHA,
	BLEND_ALPHA_ONE,
	BLEND_ALPHA_PREMULT
};

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void init(bool hirez);
    void setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void clearScreen();
    void setViewport(int x, int y, int w, int h);
    bool setBlendMode(BlendMode m);
    void enableDepthTest(bool b);
    void enableCullFace(bool b);
    int getDepthBufferDepth() { return m_iGlDepth; };
    bool isHighRez() { return m_bHiRez; };
    int getViewPortWidth() { return m_iViewPortWidth; };
    int getViewPortHeight() { return m_iViewPortHeight; };
    
private:
    int m_iGlDepth;
    bool m_bHiRez;
    int m_iViewPortWidth;
    int m_iViewPortHeight;
};

#endif /* defined(__Orion__Renderer__) */
