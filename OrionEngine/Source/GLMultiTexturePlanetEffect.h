//
//  GLMultiTexturePlanetEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-14.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GLMultiTexturePlanetEffect__
#define __Orion__GLMultiTexturePlanetEffect__

#include "GLBaseEffect.h"
#include "Vector3.h"
#include "Vector4.h"

class GLMultiTexturePlanetEffect : public GLBaseEffect
{
public:
    GLMultiTexturePlanetEffect();
    ~GLMultiTexturePlanetEffect();
    
    bool init();
    void setLightPos(Vector3f lightPos);
    void setLightColor(Vector4f lightColor);
    void prepareToRender();
    
private:
    Vector3f m_vLightPos;
    Vector4f m_vColor;
    
    GLint m_iLocLight;
    GLint m_iLocTextureUnit1;
    GLint m_iLocTextureUnit2;
    GLint m_iLocTextureUnit3;
};

#endif /* defined(__Orion__GLMultiTexturePlanetEffect__) */
