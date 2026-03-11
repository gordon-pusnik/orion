#ifndef __PNGLOADER__
#define __PNGLOADER__

#include <cstdio>

/* Mipmapping parameters */
#define PNG_NOMIPMAPS      0 /* No mipmapping                        */
#define PNG_BUILDMIPMAPS  -1 /* Calls a clone of gluBuild2DMipmaps() */
#define PNG_SIMPLEMIPMAPS -2 /* Generates mipmaps without filtering  */
#define PNG_NOMIPMAP     PNG_NOMIPMAPS
#define PNG_BUILDMIPMAP  PNG_BUILDMIPMAPS
#define PNG_SIMPLEMIPMAP PNG_SIMPLEMIPMAPS

/* Transparency parameters */
#define PNG_CALLBACK  -3 /* Call the callback function to generate alpha   */
#define PNG_ALPHA     -2 /* Use alpha channel in PNG file, if there is one */
#define PNG_SOLID     -1 /* No transparency                                */
#define PNG_STENCIL    0 /* Sets alpha to 0 for r=g=b=0, 1 otherwise       */
#define PNG_BLEND1     1 /* a = r+g+b                                      */
#define PNG_BLEND2     2 /* a = (r+g+b)/2                                  */
#define PNG_BLEND3     3 /* a = (r+g+b)/3                                  */
#define PNG_BLEND4     4 /* a = r*r+g*g+b*b                                */
#define PNG_BLEND5     5 /* a = (r*r+g*g+b*b)/2                            */
#define PNG_BLEND6     6 /* a = (r*r+g*g+b*b)/3                            */
#define PNG_BLEND7     7 /* a = (r*r+g*g+b*b)/4                            */
#define PNG_BLEND8     8 /* a = sqrt(r*r+g*g+b*b)                          */

/* Errors */
#define NO_PNG_ERROR	0
#define FILE_NOT_FOUND	1
#define PINFO_NULL		2
#define SIG_ERROR		3
#define SETJMP_ERROR	4

typedef struct PngInfo
{
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int alpha;
} PngInfo;

typedef struct PngRawInfo
{
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int alpha;

	unsigned int components;
	unsigned char *data;
	unsigned char *palette;
} PngRawInfo;

class PngLoader
{
public:
	static int pngLoadRaw(const char* filename, PngRawInfo* rawinfo, int orientation);
	static int pngLoadRawF(FILE* file, PngRawInfo* rawinfo, int orientation);
};

#endif