#include "window.h"
#include "painter.h"

Window g_window;

void Window::init()
{
    SDL_Init(SDL_INIT_VIDEO);

    int flags = SDL_WINDOW_RESIZABLE;

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    m_window = SDL_CreateWindow("ait1",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                640, 480,
                                flags);

    if(!m_window)
        exit(1);

    g_painter.create();
}

void Window::terminate()
{
    g_painter.destroy();
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    SDL_Quit();
}

void Window::poll()
{
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
        case SDL_KEYDOWN:
            //g_keyboard.processKeyDown(m_keyMap[e.key.keysym.sym]);
            break;
        case SDL_KEYUP:
            //g_keyboard.processKeyUp(m_keyMap[e.key.keysym.sym]);
            break;
        case SDL_QUIT:
            m_finished = true;
            break;
        }
    }
}

void Window::move(int x, int y)
{
    if(x < 0)
        x = SDL_WINDOWPOS_CENTERED;
    if(y < 0)
        y = SDL_WINDOWPOS_CENTERED;

    SDL_SetWindowPosition(m_window, x, y);
}

void Window::resize(int width, int height)
{
    if(m_window)
        SDL_SetWindowSize(m_window, width, height);
}

void Window::show()
{
    SDL_ShowWindow(m_window);
}

void Window::hide()
{
    SDL_HideWindow(m_window);
}

void Window::maximize()
{
    SDL_MaximizeWindow(m_window);
}

void Window::minimize()
{
    SDL_MinimizeWindow(m_window);
}

void Window::setTitle(const std::string& title)
{
    SDL_SetWindowTitle(m_window, title.c_str());
}

void Window::setMinimumSize(int width, int height)
{
    SDL_SetWindowMinimumSize(m_window, width, height);
}
