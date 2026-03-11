#ifndef __VBITMAP_H__
#define __VBITMAP_H__

#include <cstdlib>

class VBitMap
{
public:
    VBitMap() {
        pixels = 0;
    };
    ~VBitMap() {
        if (pixels) {
            free(pixels);
        };
    }
	unsigned char*	pixels;
	int				width;
	int				height;
};

#endif // __VBITMAP_H__
