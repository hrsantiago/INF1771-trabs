#ifndef PAINTEL_H
#define PAINTER_H

struct SDL_Renderer;
struct SDL_Rect;

class Painter
{
public:
    bool create();
    void destroy();

    void swapBuffers();
    void setColor(int r, int g, int b, int a);

    void drawFilledRect(int x, int y, int width, int height);

    void setRenderer(SDL_Renderer *renderer) { m_sdlRenderer = renderer; }
    SDL_Renderer *getRenderer() { return m_sdlRenderer; }

private:
    SDL_Renderer *m_sdlRenderer;

};

extern Painter g_painter;

#endif
