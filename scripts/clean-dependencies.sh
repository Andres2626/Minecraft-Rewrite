#!/bin/bash
# unistall-deps.sh -- delete all submodules source files
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
rm -rf "$DIR/../dependencies/glfw"
rm -rf "$DIR/../dependencies/gleq"
rm -rf "$DIR/../dependencies/glm"
rm -rf "$DIR/../dependencies/stb"
rm -rf "$DIR/../dependencies/zlib"