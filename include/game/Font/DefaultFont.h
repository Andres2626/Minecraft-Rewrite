#pragma once

#include <Graphics/Font/IFont.h>
#include <Graphics/Texture.h>
#include <Math/Math.h>

#include <array>

using namespace MC;
using namespace Graphics;
using namespace Math;

class DefaultFont : public IFont
{
protected:
	Texture m_Ftex;
	std::array<Glyph, 256> m_Glyphs;
public:
	DefaultFont();
	~DefaultFont() override;
public:
	const Glyph &GetGlyph(u8t c) const override;
	Texture &GetTexture() override;
};