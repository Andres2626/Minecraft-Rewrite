#!/bin/bash
# auto-build.sh -- build MC project
# This script only works for Unix users. Please note that when running, you must 
# have run auto-install.sh before installing dependencies via Git modules.
cmake -S ../ -B ../build -DMC_AUTODEP=OFF
cd build/ && make