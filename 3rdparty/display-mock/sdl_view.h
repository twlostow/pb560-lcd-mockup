#ifndef __SDL_VIEW_H
#define __SDL_VIEW_H

#include <cstdio>
#include <cstdlib>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include <cstdint>

struct Point {
    double x, y;

    Point() : x(0),y(0) {};
    Point(double x_, double y_): x(x_), y(y_) {};

    double Cross( const Point& b) const
    {
         return  x * b.y - y * b.x;

    }

	double Dot( const Point& b) const
    {
         return  x * b.x + y * b.y;

    }

	const Point operator-(const Point&b ) const
	{
		return Point(x-b.x, y-b.y) ;
	}

	const double Length() const {
		return sqrt(x*x+y*y);
	}

	bool operator==(const Point&b ) const
	{
		return x == b.x && y == b.y;
	}

	bool operator!=(const Point&b ) const
	{
		return x != b.x || y != b.y;
	}
};


struct Color {
    Color(): r(0.0), g(0.0), b(0.0), a(255.0) {}

    Color (int _r, int _g, int _b, int _a=255)
{
    r = _r; g = _g; b = _b; a= _a;
}

    Color(int n)
    {
	static struct { int r,g,b; } tab [] = {
	{128, 0, 0},
	{0, 128, 0},
	{0, 128, 128},
	{128, 128, 0},
	{0, 128, 128},
	{255,255,255},
	{128, 128, 128}};
	a=128.0;
	if(n<7)
	{
    	    r=tab [n].r;
    	    g=tab [n].g;
    	    b=tab [n].b;
	}
    }
    
    Color Lighter(double factor)
    {
	Color t;
	t.r = r * (1.0-factor) + 255.0 * factor;
	t.g = g * (1.0-factor) + 255.0 * factor;
	t.b = b * (1.0-factor) + 255.0 * factor;
	t.a = a;
	return t;
    }

    double r, g, b, a;
};


class SDLView {
    public:	

	SDLView ( int w, int h );

	void Clear();
	bool Update();
	SDL_Surface *Screen() { return m_screen; }
	virtual void UserUpdate() = 0;
	virtual void OnKeyEvent(int key, bool up) = 0;

protected:

	SDL_Surface *m_screen;
	int m_width, m_height;
};

#endif
