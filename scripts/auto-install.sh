#!/bin/bash
# auto-install.sh -- install all MC submodules
# This script only works for unix users
cd .. && git submodule init
cd .. && git submodule update