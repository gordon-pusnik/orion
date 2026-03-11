#!/bin/bash
../../android-ndk-r9d/ndk-build clean
../../android-ndk-r9d/ndk-build
mkdir lib
cp -r libs/armeabi lib
zip -r armeabi.jar lib
mv armeabi.jar libs
rm -rf lib