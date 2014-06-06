#ifndef FONT_H_
#define FONT_H_

#include <GL/gl.h>
#include <stb_truetype.h>

#include "texture.h"

struct font
{
    struct texture tex;
    const float height;
    stbtt_bakedchar *data;
    const int lower_char;
    const int upper_char;
};

#define ADD_FONT(name, path, size, lower, upper) FONT_##name,
enum
{
    FONT_NONE = -1,
    #include "font.def.h"
    FONT_TOTAL
};

extern struct font fonts[FONT_TOTAL];

/* Load all font defined in font.def.h
 * Return 1 if success 0 on error
 */
int font_load_all(void);

/* Properly unload all font defined in
 * font.def.h
 */
void font_unload_all(void);

/* Render the text the the current OpenGL context
 * Using the font_id as font at origin point (x,y,z)
 */
void font_render(int font_id, char *txt, float x, float y, float z);

#endif /* FONT_H_ */
