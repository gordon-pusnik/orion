//
//  TextureManager.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-06.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "TextureManager.h"
#include "Utils.h"

TextureManager::TextureManager()
{
    
}

TextureManager::~TextureManager()
{
    for (TextureListItor itor = m_textures.begin(); itor != m_textures.end(); ++itor) {
        Tex* t = (*itor);
        delete t;
    }
}

Tex* TextureManager::getTextureForFileName(std::string str)
{
    for (TextureListItor itor = m_textures.begin(); itor != m_textures.end(); ++itor) {
        Tex* t = (*itor);
        if (t->m_type == str) {
            t->m_iRefCount++;
            return t;
        }
    }
    return 0;
}

void TextureManager::setTextureForFileName(std::string str, GLuint i)
{
    Tex *t = new Tex(str, i);
    m_textures.push_back(t);
    LOGI("TextureManager - Added new Texture for type: %s, total: %d\n", str.c_str(), (int) m_textures.size());
}

void TextureManager::releaseTextureForFileName(std::string str)
{
    for (TextureListItor itor = m_textures.begin(); itor != m_textures.end(); ++itor) {
        Tex* t = (*itor);
        if (t->m_type == str) {
            t->m_iRefCount--;
            if (t->m_iRefCount == 0) {
                m_textures.erase(itor);
                delete t;
                return;
            }
        }
    }
}