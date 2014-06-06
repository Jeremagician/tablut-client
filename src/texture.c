#include <stb_image.h>
#include <assert.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "texture.h"
#include "shared.h"

const struct texture TEXTURE_NOT_LOADED = { 0, -1, -1};

#define ADD_TEXTURE(name, path) { {0, -1, -1}, path, 0},
struct texture_info textures[TEX_TOTAL] = {
    #include "texture.def.h"
};

int tex_load_from_memory(struct texture *tex, unsigned char * buffer, int width, int height, int format)
{
    assert(tex);
    assert(buffer);
    tex->width = width;
    tex->height = height;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, format, tex->width, tex->height, format, GL_UNSIGNED_BYTE, buffer);
    return 1;
}

int tex_load_from_file(struct texture *tex, char * filename)
{
    unsigned char *data;
    int width, height;
    assert(filename && *filename);
    assert(tex);

    data = stbi_load(filename, &width, &height, NULL, 4);
    if(data == NULL)
        goto fail;

    if(!tex_load_from_memory(tex, data, width, height, GL_RGBA))
        goto fail_load;

    stbi_image_free(data);
    return 1;
fail_load:
    stbi_image_free(data);
fail:
    return 0;
}

int tex_load(int id)
{
    if(id > TEX_NONE && id < TEX_TOTAL)
    {
        int valid = tex_load_from_file(
            &textures[id].tex,
            (char*)textures[id].file
            );

        if(valid)
        {
            textures[id].is_loaded = 1;
            return 1;
        }
    }
    return 0;
}

inline void tex_bind(int id)
{
    if(id > TEX_NONE && id < TEX_TOTAL && textures[id].is_loaded)
        glBindTexture(GL_TEXTURE_2D, textures[id].tex.id);
}

inline void tex_unload(int id)
{
    if(id > TEX_NONE && id < TEX_TOTAL && textures[id].is_loaded)
    {
        glDeleteTextures(1, &(textures[id].tex.id));
        textures[id].is_loaded = 0;
    }
}
