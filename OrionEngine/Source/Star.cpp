//
//  Star.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-25.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Star.h"
#include "GLUtils.h"
#include "Utils.h"
#include "Constants.h"
#include "Defines.h"
#include "Manager.h"

Star::Star() : Entity()
{
    m_fStep = 0;
    m_dPrevDistance = 0;
}

Star::~Star()
{
    Manager::geometryManager()->releaseGeometryForEntityType(m_type);
//    glDeleteTextures(1, &m_uiTextureID);
}

void Star::init(EntityType e, Vector3f position, float radius, const char *texture, Vector4f color, float rotInc)
{
    // Load texture.
/*
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    if (GLUtils::loadPNGTexture(texture, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("Texture loaded\n");
    }
*/    
    m_fBoundingRadius = radius;
    
    m_vPosition[0] = position[0];
    m_vPosition[1] = position[1];
    m_vPosition[2] = position[2];
    
    m_fRotation = 0.0f;
    m_fRotInc = rotInc;
    
    m_fColor[0] = color[0];
    m_fColor[1] = color[1];
    m_fColor[2] = color[2];
    m_fColor[3] = color[3];
    
    m_type = e;
    
    // Check if this shader has already been created, otherwise create it.
    ShaderManager *shaderManager = Manager::shaderManager();
    
    Shader *shader1 = shaderManager->getShaderForEffectType(BASE_EFFECT);
    if (shader1) {
        m_pEffect = shader1->m_pEffect;
    }
    else {
        m_pEffect = new GLBaseEffect;
        m_pEffect->init();
        shaderManager->setShaderForEffectType(BASE_EFFECT, m_pEffect);
    }
    
    Shader *shader2 = shaderManager->getShaderForEffectType(CORONA_EFFECT);
    if (shader2) {
        m_pCoronaEffect = (GLCoronaEffect*)shader2->m_pEffect;
    }
    else {
        m_pCoronaEffect = new GLCoronaEffect;
        m_pCoronaEffect->init();
        shaderManager->setShaderForEffectType(CORONA_EFFECT, m_pCoronaEffect);
    }    
    
    // Check if this same geometry has already been created, otherwise create it.

    GeometryManager *geoManager = Manager::geometryManager();
    Geometry* geometry = geoManager->getGeometryForEntityType(m_type);
    if (geometry) {
        m_pTriangleBatch = geometry->m_pBatch1;
        m_pCoronaBatch = geometry->m_pBatch2;
        m_pPointBatch = geometry->m_pBatch3;
        return;
    }
    else {
        m_pTriangleBatch = new GLBatch;
        m_pCoronaBatch = new GLBatch;
        m_pPointBatch = new GLBatch;
        geoManager->setGeometryForEntityType(m_type,
                                          m_pTriangleBatch,
                                          m_pCoronaBatch,
                                          m_pPointBatch);
    }
    
    int numVertices;
#if INTERLEAVE_DATA
    m_iNumIndices = GLUtils::genSphere(128, radius, &m_pVertices, &m_pIndices, &numVertices);
#else
    m_iNumIndices = GLUtils::genSphere(128, radius, &m_pVertices, &m_pNormals, &m_pTexCoords, &m_pIndices, &numVertices);
#endif

#if USE_VBOS
#if INTERLEAVE_DATA
    m_pTriangleBatch->beginInterleaved(GL_TRIANGLES, numVertices, 1);
    m_pTriangleBatch->copyIndices(m_pIndices, m_iNumIndices);
    m_pTriangleBatch->copyInterleavedVertexData(m_pVertices);
    m_pTriangleBatch->endInterleaved();
    
    delete[] m_pVertices;
    delete[] m_pIndices;
#else
    m_pTriangleBatch->begin(GL_TRIANGLES, numVertices, 1);
    m_pTriangleBatch->copyIndices(m_pIndices, m_iNumIndices);
    m_pTriangleBatch->copyVertexData3f(m_pVertices);
    m_pTriangleBatch->copyNormalData3f(m_pNormals);
    m_pTriangleBatch->copyTexCoordData2f(m_pTexCoords, 0);
    m_pTriangleBatch->end();
    
    delete[] m_pVertices;
    delete[] m_pNormals;
    delete[] m_pTexCoords;
    delete[] m_pIndices;
#endif
#endif

    // Create the corona

    Vector4f colorTo = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    float rad = m_fBoundingRadius * 15.0f;
    // Update bounding radius to include corona so that frustum culling works correctly.
    m_fBoundingRadius = rad;
    
    int iv = 0;
    m_pCoronaVerts = NULL;
    m_pCoronaVerts = (GLfloat*) realloc(m_pCoronaVerts, 3 * sizeof(GLfloat));
    m_pCoronaVerts[iv++] = 0.0f;
    m_pCoronaVerts[iv++] = 0.0f;
    m_pCoronaVerts[iv++] = 0.0f;
    
    m_iNumCoronaVerts = 1;

    int ic = 0;
    m_pCoronaColors = NULL;
    m_pCoronaColors = (GLfloat*) realloc(m_pCoronaColors, 4 * sizeof(GLfloat));
    m_pCoronaColors[ic++] = color[0];
    m_pCoronaColors[ic++] = color[1];
    m_pCoronaColors[ic++] = color[2];
    m_pCoronaColors[ic++] = 0.95f;
    
    float step = k2Pi / 20.0;
    
	for (float ang = k2Pi; ang > 0; ang -= step) {
        m_pCoronaVerts = (GLfloat*) realloc(m_pCoronaVerts, (iv / 3 + 1) * (sizeof(GLfloat) * 3));
        
        m_pCoronaVerts[iv++] = rad * sin(ang);
        m_pCoronaVerts[iv++] = rad * cos(ang);
        m_pCoronaVerts[iv++] = 0.0f;
        
        m_iNumCoronaVerts++;
        
        m_pCoronaColors = (GLfloat*) realloc(m_pCoronaColors, (ic / 4 + 1) * (sizeof(GLfloat) * 4));
        m_pCoronaColors[ic++] = colorTo[0];
        m_pCoronaColors[ic++] = colorTo[1];
        m_pCoronaColors[ic++] = colorTo[2];
        m_pCoronaColors[ic++] = colorTo[3];
	}

    m_pCoronaVerts = (GLfloat*) realloc(m_pCoronaVerts, (iv / 3 + 1) * (sizeof(GLfloat) * 3));
    m_pCoronaVerts[iv++] = rad * sin(0.0);
    m_pCoronaVerts[iv++] = rad * cos(0.0);
    m_pCoronaVerts[iv++] = 0.0f;
    
    m_iNumCoronaVerts++;

    m_pCoronaColors = (GLfloat*) realloc(m_pCoronaColors, (ic / 4 + 1) * (sizeof(GLfloat) * 4));
    m_pCoronaColors[ic++] = colorTo[0];
    m_pCoronaColors[ic++] = colorTo[1];
    m_pCoronaColors[ic++] = colorTo[2];
    m_pCoronaColors[ic++] = colorTo[3];

    m_iNumCoronaIndices = m_iNumCoronaVerts * 3 - 6;
    m_pCoronaIndices = new GLushort[m_iNumCoronaIndices];
    
    int k = 0; //aids in specifying indices.
    for (int i = 0; i < m_iNumCoronaIndices; i += 3) {
        m_pCoronaIndices[i + 0] = 0;//(GLushort) (k + 0);
        m_pCoronaIndices[i + 1] = (GLushort) (k + 1);
        m_pCoronaIndices[i + 2] = (GLushort) (k + 2);
        k++;
    }

    // Create interleaved array
    GLfloat *vertexes = new GLfloat[7 * m_iNumCoronaVerts];
    
    int i = 0;
    int j = 0;
    k = 0;
    
    for (int vert = 0; vert < m_iNumCoronaVerts; vert++) {
        vertexes[i++] = m_pCoronaVerts[j++];
        vertexes[i++] = m_pCoronaVerts[j++];
        vertexes[i++] = m_pCoronaVerts[j++];
        vertexes[i++] = m_pCoronaColors[k++];
        vertexes[i++] = m_pCoronaColors[k++];
        vertexes[i++] = m_pCoronaColors[k++];
        vertexes[i++] = m_pCoronaColors[k++];
    }

#if USE_VBOS
#if INTERLEAVE_DATA
    m_pCoronaBatch->beginInterleaved(GL_TRIANGLES, m_iNumCoronaVerts, 0);
    m_pCoronaBatch->copyIndices(m_pCoronaIndices, m_iNumCoronaIndices);
    m_pCoronaBatch->copyInterleavedVertexColorData(vertexes);
    m_pCoronaBatch->endInterleavedColor();
    
    m_pPointBatch->begin(GL_POINTS, 1, 0);
//    m_pPointBatch->copyIndices(0, 1);
    Vector3f vert = { 0.0, 0.0, 0.0 };
    m_pPointBatch->copyVertexData3f(vert);
    m_pPointBatch->end();
    
    free(m_pCoronaVerts);
    free(m_pCoronaColors);
    delete[] m_pCoronaIndices;
    delete[] vertexes;
#else
    m_pCoronaBatch->begin(GL_TRIANGLES, m_iNumCoronaVerts, 0);
    m_pCoronaBatch->copyIndices(m_pCoronaIndices, m_iNumCoronaIndices);
    m_pCoronaBatch->copyVertexData3f(m_pCoronaVerts);
    m_pCoronaBatch->copyColorData4f(m_pCoronaColors);
    m_pCoronaBatch->end();

    m_pPointBatch->begin(GL_POINTS, 1, 0);
    m_pPointBatch->copyIndices(m_pCoronaIndices, 1);
    m_pPointBatch->copyVertexData3f(m_pCoronaVerts);
    m_pPointBatch->copyColorData4f(m_pCoronaColors);
    m_pPointBatch->end();
    
    free(m_pCoronaVerts);
    free(m_pCoronaColors);
    delete[] m_pCoronaIndices;
    delete[] vertexes;
#endif
#endif    
}

void Star::update(float dt)
{    
    Camera *camera = Manager::camera();

    m_pointModelViewMatrix = camera->viewMatrix();

    // Log scale to make planet look correct at distance.
    double distance = camera->distance(m_vPosition);
//    LOGI("distance: %f\n", distance);

    Vector3d campos;
    camera->getPosition(campos);
    
    double newDist = Utils::getScaled(kDistMaxVisibleStar);
    if (distance > newDist) {
        double scale = newDist / distance;
        m_vRelPosition[0] = (m_vPosition[0] + campos[0]) * scale;
        m_vRelPosition[1] = (m_vPosition[1] + campos[1]) * scale;
        m_vRelPosition[2] = (m_vPosition[2] + campos[2]) * scale;
        m_bRenderFar = true;
    }
    else {
        m_vRelPosition[0] = m_vPosition[0] + campos[0];
        m_vRelPosition[1] = m_vPosition[1] + campos[1];
        m_vRelPosition[2] = m_vPosition[2] + campos[2];
        m_bRenderFar = false;

        double distance = camera->distance(m_vPosition);
        double screenrad = 500.0 * m_fBoundingRadius / distance;
        
        if (screenrad > 0.5) {
            m_bRenderBody = true;
        }
        else {
            m_bRenderBody = false;
        }
    }
 
    m_pointModelViewMatrix.translate(m_vRelPosition);
    
    m_modelViewMatrixBody = camera->viewMatrix();
    m_modelViewMatrixBody.translate(m_vRelPosition);
    
    if (m_bRenderFar) {
        return; // Don't need to update the rest
    }

    m_fRotation += m_fRotInc * dt;
    if (m_fRotation >= k2Pi) {
        m_fRotation = 0.0f;
    }

    m_modelViewMatrixBody.rotate(m_fRotation, 0.0f, 1.0f, 0.0f);

    m_modelViewMatrixCorona = camera->viewMatrix();
    m_modelViewMatrixCorona.translate(m_vRelPosition);

    // Face corona to the the camera.
    Vector3f viewCoords;
    camera->getViewCoords(viewCoords);
    
    Vector3f zaxis;
    zaxis[0] = viewCoords[0];
    zaxis[1] = viewCoords[1];
    zaxis[2] = viewCoords[2];
    Vector3::normalize(zaxis);
    
    Vector3f xaxis;
    Vector3f u;
    u[0] = 0.0f;
    u[1] = 1.0f;
    u[2] = 0.0f;
    Vector3::crossProduct(xaxis, u, zaxis);
    Vector3::normalize(xaxis);
    
    Vector3f yaxis;
    Vector3::crossProduct(yaxis, zaxis, xaxis);
    
    Matrix4 rot = Matrix4::makeRotationMatrix(xaxis, yaxis, zaxis);
    rot = rot.inverseOf();
    
    m_modelViewMatrixCorona.multiply(rot);
    
    m_fStep += 0.001 * dt;
}

void Star::render()
{
    Camera *camera = Manager::camera();
    m_pEffect->setColor(m_fColor);
    m_pEffect->setMatrices(camera->projectionMatrixPerspective(), m_modelViewMatrixBody);
    
    if (m_bRenderFar) {
        Manager::renderer()->enableDepthTest(false);
        m_pEffect->prepareToRender();
        m_pPointBatch->render();
        Manager::renderer()->enableDepthTest(true);
    }
    else {
        if (m_bRenderBody) {
            // Draw star body
        /*
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
        */
            
            m_pEffect->prepareToRender();
            
#if USE_VBOS
#if INTERLEAVE_DATA
            m_pTriangleBatch->renderInterleaved();
#else
            m_pTriangleBatch->render();
#endif
#else
            glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_pVertices);
            glEnableVertexAttribArray(VertexAttribVertex);
            
            glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_pNormals);
            glEnableVertexAttribArray(VertexAttribNormal);
            
            glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), m_pTexCoords);
            glEnableVertexAttribArray(VertexAttribTexCoord0);
            
            glDrawElements(GL_TRIANGLES, m_iNumIndices, GL_UNSIGNED_SHORT, m_pIndices);
            
            glDisableVertexAttribArray(VertexAttribVertex);
            glDisableVertexAttribArray(VertexAttribNormal);
            glDisableVertexAttribArray(VertexAttribTexCoord0);
#endif
        }
            
        // Draw star corona
        Manager::renderer()->enableDepthTest(false);
        Manager::renderer()->setBlendMode(BLEND_ADDITIVE);
        
        m_pCoronaEffect->setTime(m_fStep);
        m_pCoronaEffect->setMatrices(camera->projectionMatrixPerspective(), m_modelViewMatrixCorona);
        
        m_pCoronaEffect->prepareToRender();
        
#if USE_VBOS
#if INTERLEAVE_DATA
        m_pCoronaBatch->renderInterleavedColor();
#else
        m_pCoronaBatch->render();
#endif
#else    
        glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_pCoronaVerts);
        glEnableVertexAttribArray(VertexAttribVertex);
        
        glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), m_pCoronaColors);
        glEnableVertexAttribArray(VertexAttribColor);
        
        glDrawElements(GL_TRIANGLES, m_iNumCoronaIndices, GL_UNSIGNED_SHORT, m_pCoronaIndices);
        
        glDisableVertexAttribArray(VertexAttribVertex);
        glDisableVertexAttribArray(VertexAttribColor);
#endif
        
        Manager::renderer()->setBlendMode(BLEND_ALPHA);
        Manager::renderer()->enableDepthTest(true);
    }
}
