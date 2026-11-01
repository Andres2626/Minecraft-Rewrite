#pragma once
#include "common.h"

#include "../Texture.h"

#include "Math/Math.h"

namespace MC 
{
	namespace Graphics 
	{
		
        struct Glyph
        {
            Math::vec2 uvBegin;
            Math::vec2 uvEnd;
            int advance;
        };

        class IFont
        {
        public:
            virtual ~IFont() = default;
        public:
            virtual Texture &GetTexture() = 0;
            virtual const Glyph &GetGlyph(u8t c) const = 0;
        };
        
	}
}