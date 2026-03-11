//
//  AssetManager.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-17.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "AssetManager.h"
#include "Utils.h"
#include <cstring>

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#endif

AssetManager::AssetManager()
{
    
}

AssetManager::~AssetManager()
{
    
}

FILE* AssetManager::filePtrFromAsset(const char *filename, long *length)
{
    FILE *fp = 0;
#ifdef __APPLE__
    NSString *resource;
    NSString *type;
    NSString *directory;
    
    // Format will be directory/file
    int i = 0;
    char str[100];
    strcpy (str, filename);
    char *pch = strtok(str, "/.");
    while (pch != NULL) {
//        LOGI("%s\n",pch);
        switch(i) {
            case 0:
                directory = [NSString stringWithUTF8String:pch];
                break;
            case 1:
                resource = [NSString stringWithUTF8String:pch];
                break;
            case 2:
                type = [NSString stringWithUTF8String:pch];
                break;
        };
        i++;
        pch = strtok (NULL, "/.");
    };
    
    char *file = (char*) [[[NSBundle mainBundle] pathForResource:resource ofType:type inDirectory:directory] UTF8String];
	fp = fopen(file, "rb");
    
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp);
    rewind(fp);
#elif WIN32
	fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);
    *length = ftell(fp);
    rewind(fp);
#else
//	LOGI("file: %s\n", filename);
	off_t outStart, outLength;
	m_pAsset = AAssetManager_open(m_pAAssetManager, filename, AASSET_MODE_UNKNOWN);
    if (m_pAsset == NULL) {
    	LOGI("Asset null: %s\n", filename);
    	return NULL;
    }
    
    int fd = AAsset_openFileDescriptor(m_pAsset, &outStart, &outLength);
	if (fd >= 0) {
		fp = fdopen(fd, "rb");
		fseek(fp, outStart, SEEK_SET);
        *length = outLength;
   		LOGI("Successfully opened file %ld, %ld\n", outStart, outLength);
	}
	else {
		LOGI("Open via AAsset_openFileDescriptor failed for %s, dumping to temp.\n", filename);

	    char resource[255];
	    char type[255];
	    char directory[255];

	    // Format will be directory/file
	    int i = 0;
	    char str[255];
	    strncpy (str, filename, 254);
        str[254] = '\0';
	    char *pch = strtok(str, "/.");
	    while (pch != NULL) {
	//        LOGI("%s\n",pch);
	        switch(i) {
	            case 0:
	                strcpy(directory, pch);
	                break;
	            case 1:
	                strcpy(resource, pch);
	                break;
	            case 2:
	                strcpy(type, pch);
	                break;
	        };
	        i++;
	        pch = strtok (NULL, "/.");
	    };

	    char file[255];
	    strcpy(file, m_filesDir);
	    strcat(file, "/");
        if (i > 2) {
            strcat(file, directory);
            strcat(file, "_");
        }
	    strcat(file, resource);
	    strcat(file, ".");
	    strcat(file, type);

	    LOGI("Writing temp file: %s\n", file);

		char *buffer = NULL;
        off_t bufferSize = 0;
		m_pAsset = AAssetManager_open(m_pAAssetManager, filename, AASSET_MODE_BUFFER);
        if (m_pAsset) {
            bufferSize = AAsset_getLength(m_pAsset);
            buffer = new char[bufferSize];
            AAsset_read(m_pAsset, buffer, bufferSize);
            
            FILE *outfile = fopen(file, "wb");
            if (outfile) {
                fwrite(buffer, 1, bufferSize, outfile);
                fclose(outfile);
                
                fp = fopen(file, "rb");
                if (fp) {
                    fseek(fp, 0, SEEK_END);
                    *length = ftell(fp);
                    rewind(fp);
                } else {
                    LOGI("Could not open temp file for reading - %s\n", file);
                }
            } else {
                LOGI("Could not write temp file - %s\n", file);
            }
            delete[] buffer;
        } else {
            LOGI("Could not open asset for buffering - %s\n", filename);
        }

		return fp;
	}
#endif
    return fp;
}

void AssetManager::fillBufferWithAsset(const char *filename, char **buffer)
{
#ifdef __ANDROID__
    m_pAsset = AAssetManager_open(m_pAAssetManager, filename, AASSET_MODE_UNKNOWN);
    if (m_pAsset == NULL) {
    	LOGI("Asset null\n");
    	return;
    }
    
    off_t bufferSize = AAsset_getLength(m_pAsset);
    *buffer = new char[bufferSize + 1];
    int numBytesRead = AAsset_read(m_pAsset, *buffer, bufferSize);
    (*buffer)[bufferSize] = 0;
//    LOGI("Successfully read %d bytes\n", numBytesRead);
#elif WIN32
	FILE *fp = fopen(filename, "rb");
    
    fseek(fp, 0, SEEK_END);
    int bufferSize = (int)ftell(fp);
    rewind(fp);
    
    *buffer = new char[bufferSize + 1];
    fread(*buffer, 1, bufferSize, fp);
    (*buffer)[bufferSize] = 0;    
#else
    FILE *fp = 0;
    NSString *resource;
    NSString *type;
    NSString *directory;
    
    // Format will be directory/file
    int i = 0;
    char str[100];
    strcpy (str, filename);
    char *pch = strtok(str, "/.");
    while (pch != NULL) {
        //        LOGI("%s\n",pch);
        switch(i) {
            case 0:
                directory = [NSString stringWithUTF8String:pch];
                break;
            case 1:
                resource = [NSString stringWithUTF8String:pch];
                break;
            case 2:
                type = [NSString stringWithUTF8String:pch];
                break;
        };
        i++;
        pch = strtok (NULL, "/.");
    };
    
    char *file = (char*) [[[NSBundle mainBundle] pathForResource:resource ofType:type inDirectory:directory] UTF8String];
	fp = fopen(file, "rb");
    
    fseek(fp, 0, SEEK_END);
    int bufferSize = (int)ftell(fp);
    rewind(fp);
    
    *buffer = new char[bufferSize + 1];
    fread(*buffer, 1, bufferSize, fp);
    (*buffer)[bufferSize] = 0;    
#endif
}

void AssetManager::closeAsset()
{
#ifdef __ANDROID__
	AAsset_close(m_pAsset);
//	LOGI("Asset closed\n");
#endif
}

#ifdef __ANDROID__
void AssetManager::setAAssetManager(AAssetManager *mgr)
{
    m_pAAssetManager = mgr;
}

AAssetManager* AssetManager::getAAssetManager()
{
    return m_pAAssetManager;
}

void AssetManager::setFilesDir(const char* filesDir)
{
	strcpy(m_filesDir, filesDir);
}
#endif
