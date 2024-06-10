// Compilation command:
// $ mkdir bin/
// $ cd bin/
// $ cmake ..
// $ cmake --build .

#include <stdbool.h>

#include "bin/_deps/sdl2-src/include/SDL.h"
#include "bin/_deps/sdl2-src/include/SDL_events.h"
#include "bin/_deps/sdl2-src/include/SDL_pixels.h"
#include "bin/_deps/sdl2-src/include/SDL_render.h"
#include "bin/_deps/sdl2-src/include/SDL_video.h"

#define SILK_IMPLEMENTATION
#include "../../silk.h"

int main(int argc, const string argv[]) {
    pixel_buffer buffer = silkCreatePixelBuffer(
        800, 
        600
    );

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        silkLogErr("SDL: Couldn't initialize SDL.");
        silkPixelBufferFree(&buffer);
        return SILK_FAILURE;
    }

    silkLogInfo("SDL: Platform initialized successfully.");

    SDL_Window* sdl_window = SDL_CreateWindow(
        "Silk 1.0 - SDL Example", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        800, 
        600, 
        0
    );

    if(!sdl_window) {
        silkLogErr("SDL: Couldn't create an SDL Window.");
        silkPixelBufferFree(&buffer);
        SDL_Quit();
        return SILK_FAILURE;
    }

    silkLogInfo("SDL: Window created successfully.");

    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(
        sdl_window, 
        0, 
        0
    );

    if(!sdl_renderer) {
        silkLogErr("SDL: Couldn't create an SDL Renderer.");
        silkPixelBufferFree(&buffer);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return SILK_FAILURE;
    }

    silkLogInfo("SDL: Renderer created successfully.");

    SDL_Texture* sdl_texture = SDL_CreateTexture(
        sdl_renderer, 
        SDL_PIXELFORMAT_ABGR8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        800, 
        600
    );

    if(!sdl_texture) {
        silkLogErr("SDL: Couldn't create an SDL Texture.");
        silkPixelBufferFree(&buffer);
        SDL_DestroyRenderer(sdl_renderer);
        SDL_DestroyWindow(sdl_window);
        SDL_Quit();
        return SILK_FAILURE;
    }

    silkLogInfo("SDL: Texture created successfully.");

    bool exit = false;
    while(!exit) {
        SDL_Event event = { 0 };
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    exit = true;
                } break;
            }
        }

        silkClearPixelBufferColor(&buffer, 0xffffffff);
        
        silkDrawTriangleEquilateral(
            &buffer, 
            (vec2i) {
                buffer.size.x / 2.0f,
                buffer.size.y / 2.0
            },
            128,
            0xff0000ff
        );
        
        SDL_Rect source_rect = {
            0,
            0,
            buffer.size.x,
            buffer.size.y
        };

        SDL_Rect destination_rect = {
            0,
            0,
            buffer.size.x,
            buffer.size.y
        };

        SDL_UpdateTexture(
            sdl_texture,
            &source_rect,
            buffer.buffer, 
            buffer.size.x * sizeof(pixel)
        );

        SDL_RenderCopyEx(
            sdl_renderer, 
            sdl_texture, 
            &source_rect, 
            &destination_rect,
            0.0,
            NULL,
            SDL_FLIP_NONE
        );

        SDL_RenderPresent(sdl_renderer);
    }

    SDL_DestroyTexture(sdl_texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    silkPixelBufferFree(&buffer);

    return 0;
}