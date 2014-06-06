#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "font.h"
#include "shared.h"
#include "texture.h"

#define ADD_FONT(name, path, size, lower, upper) \
    { {0, -1, -1}, size, (stbtt_bakedchar[upper-lower+1]){}, lower, upper},
struct font fonts[FONT_TOTAL] =
{
    #include "font.def.h"
};

#define font_char_count(f) ((f)->upper_char - (f)->lower_char + 1)

static int get_bitmap_side_size(struct font *font)
{
    float side_size;
    float chars_by_side;
    assert(font);
    /* We want to work with squared texture so we compute
     * The minimal value of a side to contains the texture*/
    side_size = sqrt(font_char_count(font) * font->height * font->height);
    chars_by_side = ceil(side_size / font->height);
    side_size = chars_by_side * font->height;
    return side_size;
}

static int load_font(struct font *font, char * filename)
{
    FILE *font_file = NULL;
    int file_len;
    char * buffer;
    unsigned char * bitmap;
    int side; /* size of one side */

    assert(font);
    assert(filename && *filename);

    font_file = fopen(filename, "rb");
    if(!font_file)
        goto fail_file;

    /* We get the size of the file */
    fseek(font_file, 0, SEEK_END);
    file_len = ftell(font_file);
    fseek(font_file, 0, SEEK_SET);

    buffer = malloc(sizeof(*buffer) * file_len);
    if(!buffer)
        goto fail_alloc_buffer;

    side = get_bitmap_side_size(font);
    bitmap = malloc(sizeof(*bitmap) * side * side);
    if(!bitmap)
        goto fail_alloc_bitmap;

    /* Generate font data from ttf file */
    fread(buffer, 1, file_len, font_file);
    stbtt_BakeFontBitmap((unsigned char*)buffer, 0, font->height, bitmap, side, side,
                         font->lower_char, font->upper_char, font->data);
    free(buffer);

    /* Load texture from bitmap buffer */
    tex_load_from_memory(&font->tex, bitmap, side, side, GL_ALPHA);
    free(bitmap);

    fclose(font_file);
    return 1;
fail_alloc_bitmap:
    free(buffer);
fail_alloc_buffer:
    fclose(font_file);
fail_file:
    return 0;
}

static void unload_font(struct font *font)
{
    assert(font);
    glDeleteTextures(1, &font->tex.id);
}

#define ADD_FONT(name, path, size, lower, upper)                        \
    if(!load_font(&fonts[i], path))                                     \
        goto fail;                                                      \
    i++;

int font_load_all(void)
{
    int i = 0;
    #include "font.def.h"
    return 1;
fail:
    while(i --> 0)
        unload_font(&fonts[i]);
    return 0;
}

/* Properly unload all font defined in
 * font.def.h
 */
void font_unload_all(void)
{
    int i;
    for(i = 0; i < FONT_TOTAL; i++)
        unload_font(&fonts[i]);
    return;
}

#define next_byte() ((*c) = (*c)+1)
#define cont (cur & 0x3F)
#define cur (**c)
/* http://en.wikipedia.org/wiki/UTF-8 */
static int get_unicode_char(char **c)
{
    int c0, c1, c2, c3;         /* Continuation bytes */
    if ((cur & 0x80) == 0)
        return cur;
    else if ((cur & 0xE0) == 0xC0)
    {
        c0 = cur;
        next_byte();
        c1 = cont;
        return ((c0 & 0x1F) << 6) | c1;
    }
    else if((cur & 0xF0) == 0xE0)
    {
        c0 = cur;
        next_byte();
        c1 = cont;
        next_byte();
        c2 = cont;
        return ((c0 & 0x0F) << 12) | (c1 << 6) | c2;
    }
    else if ((cur & 0xF8) == 0xF0)
    {
        c0 = cur;
        next_byte();
        c1 = cont;
        next_byte();
        c2 = cont;
        next_byte();
        c3 = cont;
        return (((c0 & 0x0F) << 18) | (c1 << 12) | (c2 << 6) | c3) + 65536;
    }

    return 0;
}

/* Render the text the the current OpenGL context
 * Using the font_id as font at origin point (x,y,z)
 */
void font_render(int font_id, char *txt, float x, float y, float z)
{
    struct font * font;
    assert(font_id > FONT_NONE && font_id < FONT_TOTAL);
    font = &fonts[font_id];

    y = y + font->height;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, font->tex.id);
    glColor3f(1.0, 1.0, 1.0);   /* Change font color here */

    glBegin(GL_QUADS);
    while(*txt)
    {
        int c = get_unicode_char(&txt);
        if(c >= font->lower_char && c <= font->upper_char)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(font->data, font->tex.width, font->tex.height, c-font->lower_char, &x, &y, &q, 1);
            glTexCoord2f(q.s0,q.t0); glVertex3f(q.x0,q.y0, z);
            glTexCoord2f(q.s1,q.t0); glVertex3f(q.x1,q.y0, z);
            glTexCoord2f(q.s1,q.t1); glVertex3f(q.x1,q.y1, z);
            glTexCoord2f(q.s0,q.t1); glVertex3f(q.x0,q.y1, z);
        }
        txt++;
    }
    glEnd();
}
