
// Including imgui
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

// Including SDL2
#include <SDL2/SDL.h>

// Standard Includes
#include <stdio.h>
#include <vector>
#include <fstream>

// Making sure we are using the right SDL version
#if !SDL_VERSION_ATLEAST(2,0,17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

// SDL Window and Renderer
SDL_Window* window;
SDL_Renderer* renderer;

// Imgui Window Stuff
int windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;

// Addon Stuff
char addonName[32];
char eraName[32];
std::vector<char> factionName[32];
std::vector<char> unitName[32];

std::fstream main_cfg;
std::fstream era_cfg;
std::vector<std::fstream> units;
std::vector<std::fstream> factions;

// Main Functions
void init();
void update();
void render();
void cleanup();

// My Functions
void save();

// Imgui Windows
bool done = false;
bool demoWindowOpen = false;

bool mainWindowOpen = true;
bool eraWindowOpen = false;
bool factionWindowOpen = false;
bool unitWindowOpen = false;

void mainWindow() {
	ImGui::Begin("Wesnoth Addon Maker", NULL, windowFlags);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h));
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 10.0f;
	style.FrameBorderSize = 1.0f;

	ImGui::Checkbox("Demo Window", &demoWindowOpen);
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::InputText("Addon Name", addonName, IM_ARRAYSIZE(addonName), ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_CharsHexadecimal);
	ImGui::NewLine();
	ImGui::Checkbox("Era Maker", &eraWindowOpen);
	ImGui::Checkbox("faction Maker", &factionWindowOpen);
	ImGui::Checkbox("Unit Maker", &unitWindowOpen);
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::Button("Save Addon");

	ImGui::End();
}

void eraWindow() {

}

void factionWindow() {

}

void unitWindow() {

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

void save() {

}

void init() {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
		printf("Error: %s\n", SDL_GetError());

	// Setup window
	SDL_WindowFlags windowFlagsSDL = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	window = SDL_CreateWindow("Wesnoth Addon Maker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, windowFlagsSDL);

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
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE)
				done = true;
		}
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