//
//  AssetManager.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-17.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__AssetManager__
#define __Orion__AssetManager__

#include <cstdio>

#ifdef __ANDROID__
#include <android/asset_manager.h>
#endif

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();
    
    FILE* filePtrFromAsset(const char *filename, long *length);
    void fillBufferWithAsset(const char *filename, char **buffer);
    void closeAsset();

#ifdef __ANDROID__
    void setAAssetManager(AAssetManager *mgr);
    AAssetManager* getAAssetManager();
    void setFilesDir(const char* filesDir);
#endif
    
private:
#ifdef __ANDROID__
    AAssetManager *m_pAAssetManager;
    AAsset *m_pAsset;
    char m_filesDir[255];
#endif
};

#endif /* defined(__Orion__AssetManager__) */
