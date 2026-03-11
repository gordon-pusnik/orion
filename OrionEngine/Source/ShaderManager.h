//
//  ShaderManager.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-06.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__ShaderManager__
#define __Orion__ShaderManager__

#include "GLBaseEffect.h"

class Shader
{
public:
    Shader(EffectType et, GLBaseEffect *e) {
        m_type = et;
        m_iRefCount = 1;
        m_pEffect = e;
    };
    
    ~Shader() {
        if (m_pEffect)
            delete m_pEffect;
    };
    
    EffectType m_type;
    int m_iRefCount;

    GLBaseEffect *m_pEffect;
};

#include <list>
typedef std::list<Shader*> ShaderList;
typedef ShaderList::iterator ShaderListItor;

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();
    
    Shader* getShaderForEffectType(EffectType e);
    void setShaderForEffectType(EffectType et, GLBaseEffect *e);
    void releaseShaderForEffectType(EffectType e);
    
    ShaderList m_shaders;
};

#endif /* defined(__Orion__ShaderManager__) */
