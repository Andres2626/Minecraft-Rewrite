#pragma once

/* Shaderflags.h -- definitions for shader */

#define BIT_L(x) 1 << x

// vertex layout
#define SHADER_VERTEX_BIT 0 // reserved

// color layout loc
#define SHADER_COLOR_BIT 1

// texture layout loc
#define SHADER_TEX_BIT 2

// brightness layout loc
#define SHADER_BRIG_BIT 3

// opaque layout loc
#define SHADER_OPAQ_BIT 4

// use color in shader (layout 1)
#define SHADER_USE_COLOR BIT_R(SHADER_COLOR_BIT)

// use texture in shader (layout 2)
#define SHADER_USE_TEX BIT_R(SHADER_TEX_BIT)

// use brightness in shader (layout 3)
#define SHADER_USE_BRIG BIT_R(SHADER_BRIG_BIT)

// use opacity in shader (layout 4)
#define SHADER_USE_SCALAR BIT_R(SHADER_OPAQ_BIT)
