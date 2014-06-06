/*
 * main.c
 * This file contains all game essential initialisations
 * And the main game loop
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>
#include <GL/gl.h>

#include "shared.h"
#include "font.h"


#define WINDOW_TITLE "Tafl"
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

static int initialize(void);
static void terminate(void);

static SDL_Window * window;
static SDL_GLContext context;

int main(void)
{
    char text[1 << 10] = {0};
    if(!initialize())
        return EXIT_FAILURE;

    bool running = true;

    SDL_StartTextInput();

    glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);

    while(running)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            switch(e.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYUP:
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    int start_index = strlen(text)-1;
                    int cont = 1;

                    while(start_index != -1 && cont)
                    {
                        cont = ((text[start_index] & 0xC0) == 0x80);
                        text[start_index] = 0;
                        start_index--;
                    }
                }
                break;

            case SDL_TEXTINPUT:
                /* Add new text onto the end of our text */
                strcat(text, e.text.text);
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        font_render(FONT_DUM1_32, text, 0, 0, 0);
        SDL_GL_SwapWindow(window);
    }

    terminate();
    return EXIT_SUCCESS;
}

static void init_gl(void)
{
    GLfloat ambient[] = {1.f, 1.f, 1.f, 1.0f};

    glEnable (GL_DEPTH_TEST);

    /* Transparency */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* Multisampling */
	glEnable(GL_MULTISAMPLE);

    /* Antialiasing */
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

    /* Lights */
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL); /* shortcut to enable glColor on materials */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glClearColor(0,0.4,0.6, 1.0);
}

static int initialize(void)
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        goto fail_sdl;
    }

    window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
        );

    if(!window)
    {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        goto fail_window;
    }

    context = SDL_GL_CreateContext(window);

    if(!context)
    {
        fprintf(stderr, "Could not create graphic context: %s\n", SDL_GetError());
        goto fail_context;
    }

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    init_gl();

    if(!font_load_all())
    {
        fprintf(stderr,"Unable to load fonts\n");
        goto fail_font;
    }

    return 1;
fail_font:
    SDL_GL_DeleteContext(context);
fail_context:
    SDL_DestroyWindow(window);
fail_window:
    SDL_Quit();
fail_sdl:
    return 0;
}

static void terminate(void)
{
    font_unload_all();
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
