#include <SDL.h>
#include "painter.h"
#include "window.h"

Painter g_painter;

bool Painter::create()
{
    uint32_t flags = SDL_RENDERER_ACCELERATED;


    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    m_sdlRenderer = SDL_CreateRenderer(g_window.getSDLWindow(), -1, flags);
    if(!m_sdlRenderer)
        return false;

    return true;
}

void Painter::destroy()
{
    SDL_DestroyRenderer(m_sdlRenderer);
    m_sdlRenderer = nullptr;
}

void Painter::swapBuffers()
{
    SDL_RenderPresent(m_sdlRenderer);
}
