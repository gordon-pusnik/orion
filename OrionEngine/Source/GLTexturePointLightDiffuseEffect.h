//
//  GLTexturePointLightDiffuseEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-14.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GLTexturePointLightDiffuseEffect__
#define __Orion__GLTexturePointLightDiffuseEffect__

#include "GLBaseEffect.h"
#include "Vector3.h"
#include "Vector4.h"

class GLTexturePointLightDiffuseEffect : public GLBaseEffect
{
public:
    GLTexturePointLightDiffuseEffect();
    ~GLTexturePointLightDiffuseEffect();
    
    bool init();
    void setLightPos(Vector3f lightPos);
    void setLightColor(Vector4f lightColor);
    void prepareToRender();
    void setMin(float min) { m_fMin = min; };
    
private:
    Vector3f m_vLightPos;
    Vector4f m_vColor;
    GLfloat m_fMin;
    
    GLint m_iLocLight;
    GLint m_iLocTextureUnit;
    GLint m_iLocMin;
};

#endif /* defined(__Orion__GLTexturePointLightDiffuseEffect__) */
