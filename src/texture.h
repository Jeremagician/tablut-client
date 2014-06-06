#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/gl.h>

#define ADD_TEXTURE(name, path) TEX_##name,
enum
{
    TEX_NONE = -1,
#include "texture.def.h"
    TEX_TOTAL
};

struct texture
{
    GLuint id;
    int width;
    int height;
};

struct texture_info
{
    struct texture tex;
    const char *file;
    int is_loaded;
};

extern struct texture_info textures[TEX_TOTAL];

/* Useful constant to set default value */
extern const struct texture TEXTURE_NONE;

/* Load a texture from a file an return
 * 1 if texture successfully loaded
 * 0 otherwise
 *
 * id cannot be NULL
 *
 * Width and height will be set to the right
 * value if not NULL
 *
 * This function provide a manual load of texture
 * But the texture will have to be bound manually
 * and free to using glDeleteTextures(1, &tex_id)
 *
 * You should use this function, only if you don't
 * know which texture you load until the runtime
 */
int tex_load_from_file(struct texture *tex, char * filename);
int tex_load_from_memory(struct texture *tex, unsigned char * buffer, int width, int height, int format);

/* Properly bind a texture */
inline void tex_bind(int id);

/* Load the provided texture id, return
 * 1 if texture successfully loaded
 * 0 otherwise
 */
int tex_load(int id);

inline void tex_unload(int id);

#endif /* TEXTURE_H_ */
