#include "utils.cpp"

static void clear_screen(int colour) {
	if (colour < 0 || colour > 0xffffff) {
		colour = 0x60495a;
	}
	u32* pixel = (u32*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = colour;
		}
	}
}

static void drawRectBySizePixels(int leftX, int botY, int width, int height, u32 colour) {
	int botLeft = render_state.width *botY+leftX;
	// can do without curPxI
	int curPxI = 0;
	u32* pixel = (u32*)render_state.memory;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			curPxI = botLeft + i + j*render_state.width;
			if (curPxI < render_state.width * render_state.height) {
				pixel[curPxI] = colour;
			}
		}
	}
}



static void draw_rect_pixels(int x0, int y0, int x1, int y1, u32 colour) {

	x0 = clamp(0, x0, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	x1 = clamp(0, x1, render_state.width);
	y1 = clamp(0, y1, render_state.height);
	for (int y = y0; y < y1; y++) {
		u32* pixel = (u32*)render_state.memory+x0+y*render_state.width;
		for (int x = x0; x < x1; x++) {
			*pixel++ = colour;
		}
	}
}

static void draw_rect(float x, float y, float halfSizeX, float halfSizeY, u32 colour) {
	// change to pixels
	x *= render_state.height * render_state.render_scale;
	y *= render_state.height * render_state.render_scale;
	halfSizeX *= render_state.height * render_state.render_scale;
	halfSizeY *= render_state.height * render_state.render_scale;

	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	int x0 = x - halfSizeX;
	int x1 = x + halfSizeX;
	int y0 = y - halfSizeY;
	int y1 = y + halfSizeY;
	draw_rect_pixels(x0,y0,x1,y1,colour);
}