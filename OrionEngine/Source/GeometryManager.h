//
//  GeometryManager.h
//  Orion
//
//  Class which manages redundant geometry.
//
//  Created by Gordon Pusnik on 2012-12-27.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GeometryManager__
#define __Orion__GeometryManager__

#include "Entity.h"
#include "GLBatch.h"

class Geometry
{
public:
    Geometry(EntityType et, GLBatch *b1, GLBatch *b2, GLBatch *b3) {
        m_iRefCount = 1;
        m_type = et;
        m_pBatch1 = b1;
        m_pBatch2 = b2;
        m_pBatch3 = b3;
    };
    
    ~Geometry() {
        if (m_pBatch1)
            delete m_pBatch1;
        if (m_pBatch2)
            delete m_pBatch2;
        if (m_pBatch3)
            delete m_pBatch3;
    };

    int m_iRefCount;
    EntityType m_type;
    
    // Only 3 per object for now.
    GLBatch *m_pBatch1;
    GLBatch *m_pBatch2;
    GLBatch *m_pBatch3;
};

#include <list>
typedef std::list<Geometry*> GeometryList;
typedef GeometryList::iterator GeometryListItor;

class GeometryManager
{
public:
    GeometryManager();
    ~GeometryManager();
    
    Geometry* getGeometryForEntityType(EntityType e);
    void setGeometryForEntityType(EntityType e, GLBatch *b1, GLBatch *b2, GLBatch *b3);
    void releaseGeometryForEntityType(EntityType e);
    
    GeometryList m_geometries;
};

#endif /* defined(__Orion__GeometryManager__) */
