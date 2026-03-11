#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "VBitMap.h"

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <string>

class Texture
{
public:
	Texture(const char* file);
	~Texture();

	VBitMap*	mVBitMap;		// The raw image pixels, height and width.
//	bool		mAlpha;			// Has alpha.

	static VBitMap* flipVBitMap(VBitMap *srcBitMapPtr);
	static void flipBitMap(unsigned char* srcPixels, unsigned char* dstPixels, int width, int height);
};

#endif // __TEXTURE_H__