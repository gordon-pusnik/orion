//
//  ShaderManager.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-06.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "ShaderManager.h"
#include "Utils.h"

ShaderManager::ShaderManager()
{
    
}

ShaderManager::~ShaderManager()
{
    for (ShaderListItor itor = m_shaders.begin(); itor != m_shaders.end(); ++itor) {
        Shader* s = (*itor);
        delete s;
    }
}

Shader* ShaderManager::getShaderForEffectType(EffectType e)
{
    for (ShaderListItor itor = m_shaders.begin(); itor != m_shaders.end(); ++itor) {
        Shader* s = (*itor);
        if (s->m_type == e) {
            s->m_iRefCount++;
            return s;
        }
    }
    return 0;
}

void ShaderManager::setShaderForEffectType(EffectType et, GLBaseEffect *e)
{
    Shader *s = new Shader(et, e);
    m_shaders.push_back(s);
    LOGI("ShaderManager - Added new Shader for type: %d, total: %d\n", et, (int) m_shaders.size());
}

void ShaderManager::releaseShaderForEffectType(EffectType e)
{
    for (ShaderListItor itor = m_shaders.begin(); itor != m_shaders.end(); ++itor) {
        Shader* s = (*itor);
        if (s->m_type == e) {
            s->m_iRefCount--;
            if (s->m_iRefCount == 0) {
                m_shaders.erase(itor);
                delete s;
                return;
            }
        }
    }
}