#!/bin/bash
# ==========================================
#
# Concrete assets to unitypackage
#
# (C) 2018 Yamaha Corporation
#        Confidential
#
# ==========================================

SCRIPT_PATH=$(cd "$(dirname "$0")" ; pwd -P)
PRJECT_PATH=$SCRIPT_PATH/../
SRC=Assets/SoundXR
DST=$SCRIPT_PATH/soundxr_sdk.unitypackage

#
/Applications/Unity/Hub/Editor/2018.2.18f1/Unity.app/Contents/MacOS/Unity \
-quit -batchmode -nographics \
-logfile ConcreteToPackage.log \
-projectPath $PRJECT_PATH \
-exportPackage $SRC $DST

cd $PRJECT_PATH
rm -rf Library
rm -rf ProjectSettings
rm -rf Packages

# EOF