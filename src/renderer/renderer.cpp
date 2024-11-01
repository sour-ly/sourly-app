#include "renderer.hpp"
//define for all renderer class methods

Renderer::Renderer() {
	//default constructor
}

Renderer::~Renderer() {
	//default destructor
}

bool Renderer::start() {
	
	if (this->running) {
		Log::log("Renderer::start", "Renderer is already running\n");
		return false;
	}
	//start the renderer
	WindowContext window = {};
	WindowSettings settings = {"Sourly", this->width, this->height, {255, 255, 255, 255}};
	init_window(&settings, &window);
	this->ctx = &window;
	this->running = true;
	this->tick();
	return true;
}

bool Renderer::stop() {
	//stop the renderer
	this->running = false;
	free_window(this->ctx->window);
	this->ctx = NULL;
	return true;
}



void Renderer::tick() {
	PollEvent<PollEventType> e = {};
	//leave variable to calculate framerate
	float frameTime = 0;
	while(this->running) {
		//lets calculate the framerate
		float oldTime = frameTime;
		frameTime = SDL_GetTicks();
		this->ctx->currentFramerate = 1000 / (frameTime - oldTime);
		if(!clear_window(this->ctx)) {
			Log::log("Renderer::tick", "Failed to clear window\n");
		};
		while (poll_events(this->ctx, &e)) {
			if (e.event->type == SDL_QUIT) {
				this->emit("onQuit", this->ctx);
				this->running = false;
			}
			free(e.event);
		}
		this->emit("onFrame", this->ctx);
		
		draw_content(this->ctx);
	}
}

