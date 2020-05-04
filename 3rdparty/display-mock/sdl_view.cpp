
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <vector>

#include <unistd.h>
#include <omp.h>
#include "sdl_view.h"

#include <SDL/SDL_video.h>

using namespace std;


static void quit_tutorial(int code)
{
    /*
     * Quit SDL so we can release the fullscreen
     * mode and restore the previous video settings,
     * etc.
     */
    SDL_Quit();

    /* Exit program. */
    exit(code);
}

static void handle_key_down(SDL_keysym *keysym)
{

    /* 
     * We're only interested if 'Esc' has
     * been presssed.
     *
     * EXERCISE: 
     * Handle the arrow keys and have that change the
     * viewing position/angle.
     */
    switch (keysym->sym)
    {
    case SDLK_ESCAPE:
        quit_tutorial(0);
        break;
    case SDLK_SPACE:
        //        should_rotate = !should_rotate;
        break;
    default:
        break;
    }
}

static void process_events(void)
{
    /* Our SDL event placeholder. */
}

SDLView::SDLView(int w, int h)
{
    /* Color depth in bits of our window. */
    int bpp = 0;
    /* Flags we will pass into SDL_SetVideoMode. */
    int flags = 0;
    bpp = 32;
    m_width = w;
    m_height = h;
    m_screen = SDL_SetVideoMode(m_width, m_height, bpp, flags);
}

bool SDLView::Update()
{
    SDL_Event event;

    //SDL_FillRect(m_screen, NULL, 0); //screen_surface, NULL, color);

    UserUpdate();

    /* Grab all the events off the queue. */
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            return false;


        else if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return false;

            this->OnKeyEvent( event.key.keysym.sym, true );
        } else if (event.type == SDL_KEYUP)
        {
        OnKeyEvent( event.key.keysym.sym, false );
        }
    }

    SDL_Flip(m_screen);
    return true;
}

