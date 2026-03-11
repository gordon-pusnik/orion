//
//  TextureManager.h
//  Orion
//
//  Class which manages redundant textures.
//
//  Created by Gordon Pusnik on 2013-03-1.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__TextureManager__
#define __Orion__TextureManager__

#include "GLUtils.h"
#include <string>

class Tex
{
public:
    Tex(std::string str, GLuint t) {
        m_iRefCount = 1;
        m_type = str;
        m_uiTex = t;
    };
    
    ~Tex() {
        if (m_uiTex)
            glDeleteTextures(1, &m_uiTex);
    };
    
    int m_iRefCount;
    std::string m_type;
    
    GLuint m_uiTex;
};

#include <list>
typedef std::list<Tex*> TextureList;
typedef TextureList::iterator TextureListItor;

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();
    
    Tex* getTextureForFileName(std::string str);
    void setTextureForFileName(std::string str, GLuint t);
    void releaseTextureForFileName(std::string str);
    
    TextureList m_textures;
};

#endif /* defined(__Orion__TextureManager__) */
