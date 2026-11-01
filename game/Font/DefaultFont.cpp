#include "Font/DefaultFont.h"

#include <Graphics/GL/GL.h>
#include <Log/Log.h>

#define CHARS_PER_ROW 16
#define CHARS_PER_COLUMN 16
#define GLYPH_SIZE 0.0625f /* 1/16  */

DefaultFont::DefaultFont()
{
	if (!m_Ftex.LoadFromFile("/default.gif", GL_NEAREST)) {
		mc_fatal("Error loading Default font texture");
		return;
	}

	if (m_Ftex.x != 128 || m_Ftex.y != 128) {
		mc_error("Invalid texture size for default font");
		return;
	}

	for (int i = 0; i < 256; i++) {
		int xt = i & (CHARS_PER_ROW - 1);
		int yt = i >> 4;
		int x = 0;

		for (bool empty = false; x < 8 && !empty; x++) {
			int xindex = xt * 8 + x;
			empty = true;

			for (int y = 0; y < 8 && empty; ++y) {
				int yindex = (yt * 8 + y) * m_Ftex.x;
				int pindex = (yindex + xindex) * m_Ftex.nr_channels;
				u8t pixel = m_Ftex.pixels[pindex];
				if (pixel > 128)
					empty = false;
			}
		}

		if (i == 32)
			x = 4;


		m_Glyphs[i].uvBegin = GLYPH_SIZE * vec2(xt, yt);
		m_Glyphs[i].uvEnd = m_Glyphs[i].uvBegin + GLYPH_SIZE;
		m_Glyphs[i].advance = x;
	}

	m_Ftex.Free();
}

DefaultFont::~DefaultFont()
{

}

Texture &DefaultFont::GetTexture()
{
	return m_Ftex;
}

const Glyph &DefaultFont::GetGlyph(u8t c) const
{
	return m_Glyphs[c];
}

