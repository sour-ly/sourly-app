#include "../log/log.h"
#include <string>
#include "../events/events.h"
//@TITLEDOC Renderer class
//This header file contains the declaration of the Renderer class and all renderering related macros and functions. The goal of this will to be write a simple renderer that can call more complex renderers for different platforms. Such as: Windows, SDL, and whatever Mac uses
#ifndef RENDERER_H
#define RENDERER_H

typedef struct WindowContext WindowContext;
typedef struct WindowHandle WindowHandle;
typedef class Renderer Renderer;

enum RendererType {
	SDL,
	GDI,
};

struct RGBA {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	RGBA(int _r, int _g, int _b, int _a) : r(_r), g(_g), b(_b), a(_a) {r = _r, g = _g, b = _b, a = _a;}
};

struct Vector2 {
	double x;
	double y;

	Vector2(double _x, double _y) : x(_x), y(_y) { x = _x, y = _y; }
	Vector2(int _x, int _y) : x((double) _x), y((double) _y) { x = (double) _x, y = (double) _y; }
};

struct WindowContext {
	int width;
	int height;
	const char* title;
	WindowHandle* window;
	//leave a variable for the current framerate
	float currentFramerate = 0.0f;

};

struct WindowSettings {
	int width = 800;
	int height = 600;
	const char* title = "Untitled";
	RGBA background = {0, 0, 0, 255};
	WindowSettings() {}
	WindowSettings(const char* _title, int _width, int _height) : title(_title), width(_width), height(_height) { _title = title, _width = width, _height = height; }
	WindowSettings(const char* _title, int _width, int _height, RGBA _background) : title(_title), width(_width), height(_height), background(_background) { _title = title, _width = width, _height = height, _background = background; }
};

struct WindowHandle {
	RendererType type;	
	void* handle;
};


struct TextSettings {
	const char* text;
	float size;
	RGBA color;
	Vector2 dimensions = {100, 25};
	TextSettings(const char* _text, float _size, RGBA _color, Vector2 _D) : text(_text), size(_size), color(_color), dimensions(_D) { _text = text, _size = size, _color = color, _D = dimensions; }
	int flags = 0;
};

enum TextFlags {
	BOLD = 1,
	ITALIC = 2,
	UNDERLINE = 4,
	STRIKETHROUGH = 8,
	ANTI_ALIAS = 16,
};

template <typename T>
struct PollEvent {
	T* event = NULL;
	PollEvent() {}
	PollEvent(T* _event) : event(_event) { event = _event; }
};

//windows specific logic for rendering
#ifdef _WIN32
#include <windows.h>

using PollEventType = MSG;



#endif
//linux specific logic for rendering
#if defined(__linux) || defined(__APPLE__)

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>

using PollEventType = SDL_Event;

struct SDLContext {
	TTF_Font* font;
	RGBA color = {0, 0, 0, 255};
	SDL_Renderer* renderer;
};

static SDLContext sdlContext = {};

inline void free_window_sdl(WindowHandle* window) {
	if (window == NULL) {
		Log::log("free_window_sdl", "Window handle is null\n");
		return;
	}
	SDL_DestroyWindow((SDL_Window*)window->handle);
	SDL_Quit();
}


inline bool create_renderer_sdl(WindowContext* ctx) {
	if (ctx->window == NULL) {
		Log::log("create_renderer_sdl", "Window handle is null\n");
		return false;
	}
	if (sdlContext.renderer != NULL) {
		Log::log("create_renderer_sdl", "Renderer already exists\n");
		return false;
	}
	SDL_Window* window = (SDL_Window*)ctx->window->handle;
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL) {
		Log::log("create_renderer_sdl", "Failed to create renderer (%s)\n", SDL_GetError());
		return false;
	}
	sdlContext.renderer = renderer;
	return true;
}

inline SDL_Renderer* get_renderer_sdl(WindowContext* ctx) {
	if (sdlContext.renderer == NULL) {
		Log::fxAssert(create_renderer_sdl(ctx), "get_renderer_sdl", "Failed to create renderer (%s)\n", SDL_GetError());
	}
	return sdlContext.renderer;
}

inline bool clear_sdl(WindowContext* ctx) {
	SDL_Window* window = (SDL_Window*)ctx->window->handle;
	SDL_Renderer* renderer = get_renderer_sdl(ctx);
	SDL_SetRenderDrawColor(renderer, sdlContext.color.r, sdlContext.color.g, sdlContext.color.b, sdlContext.color.a);
	int a = SDL_RenderClear(renderer);
	return a == 0;
}

/* DRAW FUNCTIONS */

inline void draw_rect_sdl(WindowContext* ctx, Vector2 pos, Vector2 size, RGBA color) {
	SDL_Window* window = (SDL_Window*)ctx->window->handle;
	SDL_Renderer* renderer = get_renderer_sdl(ctx);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect;
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = size.x;
	rect.h = size.y;
	SDL_RenderFillRect(renderer, &rect);
}

inline void draw_text_sdl(WindowContext* ctx, Vector2 pos, TextSettings* settings) {
	SDL_Window* window = (SDL_Window*)ctx->window->handle;
	SDL_Renderer* renderer = get_renderer_sdl(ctx);
	SDL_Color color = {settings->color.r, settings->color.g, settings->color.b, settings->color.a};	
	if (sdlContext.font == NULL) {
		sdlContext.font = TTF_OpenFont("./assets/neue.otf", settings->size);
		if (sdlContext.font == NULL) {
			Log::log("draw_text_sdl", "Failed to load font (%s)\n", TTF_GetError());
			return;
		}
		Log::log("draw_text_sdl", "Loaded font %s\n", "./assets/neue.otf");
	}
	SDL_Surface* surface = TTF_RenderText_Solid(sdlContext.font, settings->text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect textRect;
	textRect.x = pos.x;
	textRect.y = pos.y;
	textRect.w = settings->dimensions.x; 
	textRect.h = settings->dimensions.y;

	//draw
	SDL_RenderCopy(renderer, texture, NULL, &textRect);
	//free
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

inline Vector2 getMouse_sdl(WindowContext* out) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	Vector2 pos = Vector2(x, y);

	return pos;
}

inline void draw_window_sdl(WindowSettings* settings, WindowContext* out) {
	SDL_Window* window = NULL;
	SDL_GLContext context = NULL;
	TTF_Init();

	if (SDL_Init(SDL_INIT_VIDEO ) < 0) {
		Log::log("draw_window_sdl", "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	window = SDL_CreateWindow(settings->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, settings->width, settings->height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == NULL) {
		Log::log("draw_window_sdl", "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		Log::log("draw_window_sdl", "OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return;
	}

	sdlContext.color = settings->background;

	//flags 

	/* AI Generated Code 
	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	*/
	Log::log("draw_window_sdl", "Handing off window context to caller\n");
	//check if the window handle is null
	if (out->window == NULL) {
		out->window = (WindowHandle*)malloc(sizeof(WindowHandle));
		out->window->type = SDL;
		out->window->handle = window;
	} else {
		Log::fxAssert(false, "draw_window_sdl", "Window handle is not null\n");
	}
	
}

inline void draw_content_sdl(WindowContext* ctx) {
	SDL_Window* window = (SDL_Window*)(ctx->window->handle);
	SDL_RenderPresent(get_renderer_sdl(ctx));
	SDL_GL_SwapWindow(window);
}

#endif

//DO NOT FREE WINDOWSETTINGS
inline bool init_window(WindowSettings* settings, WindowContext* out) {
#ifdef _WIN32
	//windows specific logic for rendering
	Log::log("draw_window", "Device: Windows (GDI)\n");
#elif defined(__linux__) || defined(__APPLE__)
	//linux specific logic for rendering
	Log::log("draw_window", "Device: Linux (SDL2)\n");
	Log::log("draw_window", "Device: Apple (SDL3)\n");
	draw_window_sdl(settings, out);
	Log::fxAssert(out != NULL, "draw_window", "Failed to draw window\n");
	return true;
#endif
	return false;
}

inline void draw_content(WindowContext* ctx) {
#ifdef _WIN32
	//windows specific logic for rendering
#elif defined(__linux__) || defined(__APPLE__)
	//linux specific logic for rendering
	draw_content_sdl(ctx);
#endif

}


inline int poll_events(WindowContext* ctx, PollEvent<PollEventType>* out) {
#ifdef _WIN32
	//windows specific logic for rendering
#elif defined(__linux__) || defined(__APPLE__)
	//linux specific logic for rendering
	SDL_Event* e = (SDL_Event*)malloc(sizeof(SDL_Event));
	int r = SDL_PollEvent(e);	
	if (r == 0) {
		free(e);
		return 0;
	}
	out->event = e;
	return r;

#endif
	return 0;
}

inline bool clear_window(WindowContext* ctx) {
#ifdef _WIN32
	//windows specific logic for rendering
#elif __linux__ || __APPLE__
	//linux specific logic for rendering
	return clear_sdl(ctx);
#endif
	return false;
}

inline void free_window(WindowHandle* window) {
#ifdef _WIN32
	//windows specific logic for rendering
	Log::log("free_window", "Device: Windows (GDI)\n");
#elif defined(__linux__) || defined(__APPLE__)
	//linux specific logic for rendering
	Log::log("free_window", "Device: Linux (SDL2)\n");
	free_window_sdl(window);
#endif
}

inline bool init_renderer(WindowContext* ctx) {
#ifdef _WIN32
	//windows specific logic for rendering
	Log::log("init_renderer", "Device: Windows (GDI)\n");
#elif defined(__linux__) || defined(__APPLE__)
	//linux specific logic for rendering
	Log::log("init_renderer", "Device: Linux (SDL2)\n");
	Log::log("init_renderer", "Device: Apple (SDL2)\n");
	/*
	bool quit = false;
	SDL_Event e;
	SDL_Window* window = (SDL_Window*)(ctx->window->handle);

	TextSettings settings = TextSettings("I know where you live", 512.0f, {0,0,0, 255}, Vector2(512, 64));
	Vector2 pos = {0, 0};
	while (!quit) {
		//clear the screen
		if(!clear_sdl(ctx)) {
			Log::log("init_renderer", "Failed to clear screen (%s)\n", SDL_GetError());
		}	

		//handle events
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}

		//nonsense code
		pos = getMouse_sdl(ctx);
		draw_text_sdl(ctx, pos, &settings);

		//clear the screen
		SDL_RenderPresent(sdlContext.renderer);
		SDL_GL_SwapWindow(window);
		*/
#endif

	return true;
}

/* DRAW FUNCTIONS ABSTRACTED */

inline void draw_text(WindowContext* ctx, Vector2 pos, TextSettings* settings) {
#ifdef _WIN32
	//windows specific logic for rendering
	Log::fxAssert(false, "draw_text", "Not implemented\n");
#elif defined(__linux__) || defined(__APPLE__)
	//linux specific logic for rendering
	draw_text_sdl(ctx, pos, settings);
#endif
}

inline void draw_rect(WindowContext* ctx, Vector2 pos, Vector2 size, RGBA color) {
#ifdef _WIN32
	//windows specific logic for rendering
	Log::fxAssert(false, "draw_rect", "Not implemented\n");
#elif defined(__linux__) || defined(__APPLE__)
	//linux specific logic for rendering
	draw_rect_sdl(ctx, pos, size, color);
#endif
}


class Renderer : public Eventful<WindowContext> {
public:
	Renderer(); 
	Renderer(int _width, int _height) : width(_width), height(_height) { width = _width, height = _height; }
	~Renderer();
	bool start(); 
	bool stop(); 
	int getWidth() { return this->width; }
	int getHeight() { return this->height; }
private:
	RendererType type;
	WindowContext* ctx;
	bool running = false;
	inline void tick();
	int width = 800;
	int height = 600;
};

#endif

