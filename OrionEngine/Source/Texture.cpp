#include "Texture.h"
#include "png/PngLoader.h"

Texture::Texture(const char* file)
{
//	mAlpha = false;
	PngRawInfo pngRawInfo;
		
	if (PngLoader::pngLoadRaw(file, &pngRawInfo, 0)) {
		mVBitMap = NULL;
		return;
	}
					
	mVBitMap = new VBitMap;
	mVBitMap->width = pngRawInfo.width;
	mVBitMap->height = pngRawInfo.height;
	
	if (pngRawInfo.alpha) { // Have 8 bit alpha channel.
		if (pngRawInfo.components == 4 && pngRawInfo.depth == 8) { // 32 bit RGBA.
			mVBitMap->pixels = pngRawInfo.data;
		}
		else if (pngRawInfo.components == 2 && pngRawInfo.depth == 8) { // Grayscale or palette with alpha.
			if (pngRawInfo.palette == NULL) { // Grayscale.
				mVBitMap->pixels = (unsigned char*) malloc(mVBitMap->width * mVBitMap->height * 4);
				
				int i = 0;
				int j = 0;
				for (int y = 0; y < mVBitMap->height; y++) {
					for (int x = 0; x < mVBitMap->width; x++) {
						unsigned char gray = pngRawInfo.data[j++];
						mVBitMap->pixels[i++] = gray;
						mVBitMap->pixels[i++] = gray;
						mVBitMap->pixels[i++] = gray;
						mVBitMap->pixels[i++] = pngRawInfo.data[j++];				
					}
				}
				
				free(pngRawInfo.data);		
			}
			else { // Palette.
				mVBitMap->pixels = (unsigned char*) malloc(mVBitMap->width * mVBitMap->height * 4);
				
				int i = 0;
				int j = 0;
				for (int y = 0; y < mVBitMap->height; y++) {
					for (int x = 0; x < mVBitMap->width; x++) {
						int index = pngRawInfo.data[j++] * 3;
						mVBitMap->pixels[i++] = pngRawInfo.palette[index + 0];
						mVBitMap->pixels[i++] = pngRawInfo.palette[index + 1];
						mVBitMap->pixels[i++] = pngRawInfo.palette[index + 2];
						mVBitMap->pixels[i++] = pngRawInfo.data[j++];
					}
				}
				
				free(pngRawInfo.data);
			}
		}
/*		
		// Checking the bitmap to see if it actually is transparent.
		int i = 0;
		for (int y = 0; y < mVBitMap->height; y++) {
			for (int x = 0; x < mVBitMap->width; x++) {
				i += 3;
				if (mVBitMap->pixels[i] < 255) {
					mAlpha = true;
					return;
				}
			}
		}		
 */
	}
	else { // No alpha channel.
		if (pngRawInfo.components == 3 && pngRawInfo.depth == 8) { // 24 bit RGB.
			mVBitMap->pixels = (unsigned char*) malloc(mVBitMap->width * mVBitMap->height * 4);
			
			int i = 0;
			int j = 0;
			for (int y = 0; y < mVBitMap->height; y++) {
				for (int x = 0; x < mVBitMap->width; x++) {
					mVBitMap->pixels[i++] = pngRawInfo.data[j++];
					mVBitMap->pixels[i++] = pngRawInfo.data[j++];
					mVBitMap->pixels[i++] = pngRawInfo.data[j++];
					mVBitMap->pixels[i++] = 255;				
				}
			}
			
			free(pngRawInfo.data);
		}
		else if (pngRawInfo.components == 1 && pngRawInfo.depth == 8) { // Grayscale or palette no alpha.
			if (pngRawInfo.palette == NULL) { // Grayscale.
				mVBitMap->pixels = (unsigned char*) malloc (mVBitMap->width * mVBitMap->height);
				
				int i = 0;
				int j = 0;
				for (int y = 0; y < mVBitMap->height; y++) {
					for (int x = 0; x < mVBitMap->width; x++) {
						unsigned char gray = pngRawInfo.data[j++];
						mVBitMap->pixels[i++] = gray;
//						mVBitMap->pixels[i++] = gray;
//						mVBitMap->pixels[i++] = gray;
//						mVBitMap->pixels[i++] = 255;				
					}
				}
				
				free(pngRawInfo.data);
			}
			else { // Palette.
				mVBitMap->pixels = (unsigned char*) malloc(mVBitMap->width * mVBitMap->height * 4);
				
				int i = 0;
				int j = 0;
				for (int y = 0; y < mVBitMap->height; y++) {
					for (int x = 0; x < mVBitMap->width; x++) {
						int index = pngRawInfo.data[j++] * 3;
						mVBitMap->pixels[i++] = pngRawInfo.palette[index + 0];
						mVBitMap->pixels[i++] = pngRawInfo.palette[index + 1];
						mVBitMap->pixels[i++] = pngRawInfo.palette[index + 2];
						mVBitMap->pixels[i++] = 255;
					}
				}
				
				free(pngRawInfo.data);		
			}
		}
	}
}

Texture::~Texture()
{
	delete mVBitMap;
}

VBitMap* Texture::flipVBitMap(VBitMap* srcBitMapPtr)
{
	int width = srcBitMapPtr->width;
	int height = srcBitMapPtr->height;

	VBitMap* dstBitMapPtr = new VBitMap;

	dstBitMapPtr->pixels = (unsigned char*) malloc(width * height * 4);

	dstBitMapPtr->width = width;
	dstBitMapPtr->height = height;

	unsigned char* srcPixels = srcBitMapPtr->pixels;
	unsigned char* dstPixels = dstBitMapPtr->pixels;
	
	int row = height - 1;
	int srcIndex = 0;
	int dstIndex = 0;
	int width_x4 = width * 4;

	for (int y = 0; y < height; y++) {
		dstIndex = width_x4 * row;
		row--;
		
		memcpy(&dstPixels[srcIndex], &srcPixels[dstIndex], width_x4);

		srcIndex += width_x4;
	}
	
	free(srcBitMapPtr->pixels);
	delete srcBitMapPtr;
	
	return dstBitMapPtr;
}

void Texture::flipBitMap(unsigned char* srcPixels, unsigned char* dstPixels, int width, int height)
{
	int row = height - 1;
	int srcIndex = 0;
	int dstIndex = 0;
	int width_x4 = width * 4;

	for (int y = 0; y < height; y++) {
		dstIndex = width_x4 * row;
		row--;
		
		memcpy(&dstPixels[srcIndex], &srcPixels[dstIndex], width_x4);

		srcIndex += width_x4;
	}
}
