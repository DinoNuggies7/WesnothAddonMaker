
// Including imgui
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

// Including SDL2
#include <SDL2/SDL.h>

// Standard Includes
#include <fstream>
#include <stdio.h>

// Making sure we are using the right SDL version
#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

// SDL Window and Renderer
SDL_Window* window;
SDL_Renderer* renderer;

// Main Functions
void init();
void update();
void render();
void cleanup();

// Imgui Windows
bool done = false;
bool mainWindowOpen = true;
bool eraWindowOpen = false;

bool demoWindowOpen = false;

void mainWindow() {
	ImGui::Begin("Wesnoth Addon Maker");

	ImGui::Checkbox("Demo Window", &demoWindowOpen);
	ImGui::NewLine();

	ImGui::Checkbox("Era Maker", &eraWindowOpen);

	ImGui::End();
}

void eraWindow() {

}

void unitMaker() {

}

// Main code
int main() {
	// Initialize SDL and Imgui
	init();

	// Main loop
	while (!done) {
		// Poll Events and update Imgui Frame
		update();

		// Imgui Windows //
		//===============//
		if (mainWindowOpen)
			mainWindow();

		if (eraWindowOpen)
			eraWindow();

		if (demoWindowOpen)
			ImGui::ShowDemoWindow();
		//===============//

		// Render the Imgui Windows to SDL
		render();
	}

	// Delete SDL Window and Renderer, and Shut Down Imgui
	cleanup();
	return 0;
}

void init() {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
		printf("Error: %s\n", SDL_GetError());

	// Setup window
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("Wesnoth Addon Maker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);

	// Setup SDL_Renderer instance
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		SDL_Log("Error creating SDL_Renderer!");

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);
}

void update() {
	// Poll Window Events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			done = true;
		if (event.type == SDL_WINDOWEVENT and event.window.event == SDL_WINDOWEVENT_CLOSE and event.window.windowID == SDL_GetWindowID(window))
			done = true;
	}

	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void render() {
	ImGui::Render();
	SDL_SetRenderDrawColor(renderer, Uint8(0), Uint8(0), Uint8(0), Uint8(0));
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
}

void cleanup() {
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}