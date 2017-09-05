#pragma once

#include <array>
#include <SDL2/SDL_ttf.h>
#include <polar/asset/base.h>
#include <polar/support/font/glyphentry.h>
#include <polar/util/sdl.h>

namespace polar { namespace asset {
	struct font : base {
		using glyphentry = support::font::glyphentry;

		TTF_Font *ttf;
		int maxWidth = 0;
		int maxHeight = 0;
		int lineSkip = 0;
		std::array<glyphentry, 128> glyphs;
	};

	template<> inline std::string name<font>() { return "font"; }

	inline deserializer & operator>>(deserializer &s, font &asset) {
		std::string data;
		s >> data;
		char *buffer = static_cast<char *>(malloc(data.size()));
		if(buffer == NULL) {
			debugmanager()->fatal("failed to allocate memory for font data");
		} else {
			memcpy(buffer, data.data(), data.size());
		}

		SDL_RWops *rwopts;
		SDL(rwopts = SDL_RWFromConstMem(buffer, data.size()));
		SDL(asset.ttf = TTF_OpenFontRW(rwopts, false, 144));

		SDL(asset.lineSkip = TTF_FontLineSkip(asset.ttf));

		for(size_t i = 0; i < asset.glyphs.size(); ++i) {
			if(TTF_GlyphIsProvided(asset.ttf, i)) {
				auto &glyph = asset.glyphs[i];
				glyph.active = true;
				glyph.origin = asset.maxWidth;
				SDL(TTF_GlyphMetrics(asset.ttf, i, &glyph.min.x, &glyph.max.x, &glyph.min.y, &glyph.max.y, &glyph.advance));

				asset.maxWidth += glyph.max.x - glyph.min.x;
				int h = glyph.max.y - glyph.min.y;
				if(h > asset.maxHeight) { asset.maxHeight = h; }

				// render outline first for drop-shadow effect
				SDL(TTF_SetFontOutline(asset.ttf, 2));
				SDL(glyph.surface = TTF_RenderGlyph_Blended(asset.ttf, i, { 0, 0, 0, 255 }));

				// render foreground and blit on top of outline
				SDL(TTF_SetFontOutline(asset.ttf, 0));
				SDL_Surface *fg;
				SDL(fg = TTF_RenderGlyph_Blended(asset.ttf, i, { 255, 255, 255, 255 }));
				SDL(SDL_BlitSurface(fg, NULL, glyph.surface, NULL));
				SDL(SDL_FreeSurface(fg));
			}
		}

		return s;
	}
} }
