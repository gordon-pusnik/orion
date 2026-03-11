//
//  GeometryManager.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-27.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GeometryManager.h"
#include "Utils.h"

GeometryManager::GeometryManager()
{
    
}

GeometryManager::~GeometryManager()
{
    for (GeometryListItor itor = m_geometries.begin(); itor != m_geometries.end(); ++itor) {
        Geometry* g = (*itor);
        delete g;
    }    
}

Geometry* GeometryManager::getGeometryForEntityType(EntityType e)
{
    for (GeometryListItor itor = m_geometries.begin(); itor != m_geometries.end(); ++itor) {
        Geometry* g = (*itor);
        if (g->m_type == e) {
            g->m_iRefCount++;
            return g;
        }
    }
    return 0;
}

void GeometryManager::setGeometryForEntityType(EntityType e, GLBatch *b1, GLBatch *b2, GLBatch *b3)
{
    Geometry *g = new Geometry(e, b1, b2, b3);
    m_geometries.push_back(g);
    LOGI("GeometryManager - Added new geometry for type: %d, total: %d\n", e, (int) m_geometries.size());
}

void GeometryManager::releaseGeometryForEntityType(EntityType e)
{
    for (GeometryListItor itor = m_geometries.begin(); itor != m_geometries.end(); ++itor) {
        Geometry* g = (*itor);
        if (g->m_type == e) {
            g->m_iRefCount--;
            if (g->m_iRefCount == 0) {
                m_geometries.erase(itor);
                delete g;
                return;
            }
        }
    }    
}