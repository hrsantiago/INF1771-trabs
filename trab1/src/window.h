#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <string>

class Window
{
public:
    void init();
    void terminate();
    void poll();

    SDL_Window *getSDLWindow() { return m_window; }

public:
    void move(int x, int y);
    void resize(int width, int height);
    void show();
    void hide();
    void maximize();
    void minimize();

    void setTitle(const std::string& title);
    void setMinimumSize(int width, int height);

    void getSize(int& width, int& height);

    bool hasFinished() { return m_finished; }

private:

    SDL_Window *m_window = nullptr;
    SDL_Cursor *m_defaultCursor;
    bool m_finished = false;
};

extern Window g_window;

#endif
