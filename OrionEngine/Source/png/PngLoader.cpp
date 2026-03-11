#include "PngLoader.h"
#include "png.h"
#include "../Manager.h"
#include "../Utils.h"
#include <cstring>
#include <cstdlib>

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

int PngLoader::pngLoadRaw(const char* filename, PngRawInfo* pinfo, int orientation) 
{
    long lSize;
	FILE *fp = Manager::assetManager()->filePtrFromAsset(filename, &lSize);
	if (fp == NULL) 
		return FILE_NOT_FOUND;

	int result = pngLoadRawF(fp, pinfo, orientation);

	if (fclose(fp) != 0) {
		if (result == NO_PNG_ERROR) {
			free(pinfo->data);
			free(pinfo->palette);
		}
		return NO_PNG_ERROR;
	}

	Manager::assetManager()->closeAsset();

	return result;
}

int PngLoader::pngLoadRawF(FILE* fp, PngRawInfo* pinfo, int orientation)
{
	unsigned char 	header[8];
	png_structp 	png;
	png_infop   	info;
	png_infop   	endinfo;
	png_bytep   	data;
	png_bytep  		*row_p;
	//double		fileGamma;
	png_uint_32 	width, height;
	int 			depth, color;
	png_uint_32 	i;

	if (pinfo == NULL) 
		return PINFO_NULL;

	fread(header, 1, 8, fp);
	if (! png_check_sig(header, 8))
		return SIG_ERROR;

	png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info = png_create_info_struct(png);
	endinfo = png_create_info_struct(png);

	if (setjmp(png->jmpbuf)) {
		png_destroy_read_struct(&png, &info, &endinfo);
		return SETJMP_ERROR;
	}

	png_init_io(png, fp);
	png_set_sig_bytes(png, 8);
	png_read_info(png, info);
	png_get_IHDR(png, info, &width, &height, &depth, &color, NULL, NULL, NULL);

	pinfo->width  = (unsigned int) width;
	pinfo->height = (unsigned int) height;
	pinfo->depth  = depth;

	png_read_update_info(png, info);
	data = (png_bytep) malloc(png_get_rowbytes(png, info) * height);	
	row_p = (png_bytep*) malloc ( sizeof(png_bytep) * height);

	for (i = 0; i < height; i++) {
		if (orientation)
			row_p[height - 1 - i] = &data[png_get_rowbytes(png, info) * i];
		else
			row_p[i] = &data[png_get_rowbytes ( png, info ) * i];
	}

	png_read_image(png, row_p);

	free (row_p);

	if (color == PNG_COLOR_TYPE_PALETTE) {
		int cols;
		png_get_PLTE(png, info, (png_colorp *) &pinfo->palette, &cols);
	}
	else {
		pinfo->palette = NULL;
	}

	if (color & PNG_COLOR_MASK_ALPHA) {
		if (color & PNG_COLOR_MASK_PALETTE || color == PNG_COLOR_TYPE_GRAY_ALPHA)
			pinfo->components = 2;
		else
			pinfo->components = 4;
		pinfo->alpha = 8;
	}
	else  {
		if  (color & PNG_COLOR_MASK_PALETTE || color == PNG_COLOR_TYPE_GRAY)
			pinfo->components = 1;
		else
			pinfo->components = 3;
		pinfo->alpha = 0;
	}

	pinfo->data = data;

	png_read_end(png, endinfo);
	png_destroy_read_struct(&png, &info, &endinfo);

	return NO_PNG_ERROR;
}
