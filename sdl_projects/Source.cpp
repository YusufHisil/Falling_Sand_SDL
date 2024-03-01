#include <SDL.h>
#include <iostream>

#define WIN_H 600
#define WIN_W 1200
#define SAND 5
bool running = true;
SDL_Event e;
bool cast = false;

int main(int argc, char* argv[]) {

    bool map[WIN_H / SAND+1][WIN_W / SAND+1];
    for (int i = 0; i < WIN_H / SAND; i++)
        for (int j = 0; j < WIN_W / SAND; j++) map[i][j] = false;

    int mouseX, mouseY;
    SDL_Rect particle{0,0,SAND,SAND};


    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Falling Sand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer for the window
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) running = false;

            if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
            {
                cast = !cast;
                std::cout << cast << std::endl;
            }
        }

        if (cast) {
            SDL_GetMouseState(&mouseX, &mouseY);
            map[mouseY / SAND][mouseX / SAND] = 
                map[mouseY / SAND+1][mouseX / SAND+1] =
                map[mouseY / SAND+1][mouseX / SAND-1] = true;
            std::cout << mouseX / SAND << " " << mouseY / SAND << std::endl;
        }

        // Set the drawing color (black) and clear the window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // RGBA
        SDL_RenderClear(renderer);

        // Present the renderer's content to the window
        for (int i = WIN_H / SAND; i >= 0; i--)
            for (int j = WIN_W / SAND; j >= 0; j--)
            {
                //check for sand thats supposed to fall
                if (map[i][j] == true)
                {
                    //check if directly below is occupied
                    if (!map[i+1][j])
                    {
                        
                        map[i+1][j] = true;
                        map[i][j] = false;
                    }//check if it's free to fall to either left or right and fall randomly if so
                    else if (!map[i + 1][j + 1] && !map[i + 1][j - 1])
                    {
                        //make sand fall randomly based on time elapsed
                        if (SDL_GetTicks() % 2)
                        {
                            map[i + 1][j + 1] = true;
                            map[i][j] = false;
                        }
                        else
                        {
                            map[i + 1][j -1] = true;
                            map[i][j] = false;
                        
                        }
                    }//check if only right is free
                    else if (!map[i + 1][j + 1])
                    {
                        map[i + 1][j + 1] = true;
                        map[i][j] = false;
                    }//check if only left is free
                    else if (!map[i + 1][j - 1])
                    {
                        map[i + 1][j - 1] = true;
                        map[i][j] = false;
                    }

                    particle.x = j * SAND;
                    particle.y = i * SAND;



                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &particle);
                }
            }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderPresent(renderer);

        // Wait for a few seconds before closing the window
        SDL_Delay(7);
    }

    // Clean up resources before exiting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
