#pragma once
#include "../../utilities/csgo.hpp"

enum font_flags {
	fontflag_none,
	fontflag_italic = 0x001,
	fontflag_underline = 0x002,
	fontflag_strikeout = 0x004,
	fontflag_symbol = 0x008,
	fontflag_antialias = 0x010,
	fontflag_gaussianblur = 0x020,
	fontflag_rotary = 0x040,
	fontflag_dropshadow = 0x080,
	fontflag_additive = 0x100,
	fontflag_outline = 0x200,
	fontflag_custom = 0x400,
	fontflag_bitmap = 0x800,
};

namespace render {
	void initialize();

	void draw_line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, color colour);
	void draw_text_wchar(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t* string, color colour);
	void draw_text_wchar(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t* string, bool text_centered, color colour);
	void draw_text_string(std::int32_t x, std::int32_t y, unsigned long font, std::string string, bool text_centered, color colour);
	void draw_rect(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color color);
	void draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color colour);
	void draw_outline(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, color colour);
	void draw_textured_polygon(std::int32_t n, vertex_t* vertice, color col);
	void draw_circle(std::int32_t x, std::int32_t y, std::int32_t r, std::int32_t s, color col);
	vec2_t get_text_size(unsigned long font, std::string text);
	void draw_3d_cube(float scalar, vec3_t angles, vec3_t middle_origin, std::int32_t red, std::int32_t blue, std::int32_t green, std::int32_t alpha);


	namespace fonts {
		extern unsigned long watermark_font;
		extern unsigned long player_font;
		extern unsigned long name_font_small;
		extern unsigned long name_font;
		extern unsigned long name_font_big;
		extern unsigned long name_font_bigger;
		extern unsigned long icon_font;
	}

}