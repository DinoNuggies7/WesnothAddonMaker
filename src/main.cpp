
// Including imgui
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

// Including SDL2
#include <SDL2/SDL.h>

// Standard Includes
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
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
bool windowsMode = false;

char addonName[32] = "NewAddon";
char eraID[16] = "NewEraID";
char eraName[32] = "NewEra";
int factions = 1;
struct Attack {
	char name[32] = "Punch", description[128] = "Average haymaker";
	char* type = "impact";
	char* range = "melee";
	int damage = 8, number = 1;
};
struct Unit {
	char id[16] = "guy", name[32] = "Guy", description[512] = "Just some guy", advance[16] = "null";
	char* race = "human";
	char* movement_type = "armoredfoot";
	char* alignment = "neutral";
	char* usage = "scout";
	int hitpoints = 10, movement = 5, experience = 20, level = 1, cost = 12;
	int blade, impact, pierce, arcane, fire, cold;
	int attacks = 1;
	Attack attack[4];
};
struct Faction {
	char id[16] = "NewFactionID", name[32] = "NewFaction", description[512] = "Placeholder Faction", leader[256] = "guy", recruit[256] = "guy", pattern[256] = "guy";
	int units = 1;
	Unit unit[8];
};
Faction faction[8];

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

void mainWindow() {
	ImGui::Begin("Wesnoth Addon Maker", NULL, windowFlags);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(SDL_GetWindowSurface(window)->w, SDL_GetWindowSurface(window)->h));
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 10.0f;
	style.FrameBorderSize = 1.0f;
	ImGui::PushItemWidth(200);

	ImGui::Checkbox("Windows Mode", &windowsMode);
	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::InputText("Addon Name", addonName, IM_ARRAYSIZE(addonName), ImGuiInputTextFlags_CharsNoBlank);
	ImGui::NewLine();

	// Era Maker
	if (ImGui::TreeNode("Era Maker")) {
		ImGui::InputText("Era ID", eraID, IM_ARRAYSIZE(eraID), ImGuiInputTextFlags_CharsNoBlank);
		ImGui::InputText("Era Name", eraName, IM_ARRAYSIZE(eraName));
		ImGui::NewLine();

		// Faction Maker
		ImGui::Text("Faction: ");
		ImGui::SameLine();
		if (ImGui::Button("-1") and factions > 1)
			factions--;
		ImGui::SameLine();
		if (ImGui::Button("+1") and factions < 8)
			factions++;
		ImGui::PushItemWidth(200);
		for (int i = 0; i < factions; i++) {
			std::stringstream _ss;
			_ss.clear();
			_ss << "Faction " << i+1;
			std::string _factionNumber = _ss.str();
			if (ImGui::TreeNode(_factionNumber.c_str())) {
				ImGui::InputText("Faction ID", faction[i].id, IM_ARRAYSIZE(faction[i].id), ImGuiInputTextFlags_CharsNoBlank);
				ImGui::InputText("Faction Name", faction[i].name, IM_ARRAYSIZE(faction[i].name));
				ImGui::PushItemWidth(400);
				ImGui::InputText("Leaders (Use unit ID and seporate by commas)", faction[i].leader, IM_ARRAYSIZE(faction[i].leader), ImGuiInputTextFlags_CharsNoBlank);
				ImGui::InputText("Recruits (Use unit ID and seporate by commas)", faction[i].recruit, IM_ARRAYSIZE(faction[i].recruit), ImGuiInputTextFlags_CharsNoBlank);
				ImGui::InputText("Recruitment Pattern for AI (Use unit usage and seporate by commas)", faction[i].pattern, IM_ARRAYSIZE(faction[i].pattern), ImGuiInputTextFlags_CharsNoBlank);
				ImGui::PushItemWidth(200);
				ImGui::InputTextMultiline("Description", faction[i].description, IM_ARRAYSIZE(faction[i].description), ImVec2(768, 256));
				ImGui::NewLine();

				// Unit Maker
				ImGui::Text("Units: ");
				ImGui::SameLine();
				if (ImGui::Button("-1") and faction[i].units > 1)
					faction[i].units--;
				ImGui::SameLine();
				if (ImGui::Button("+1") and faction[i].units < 8)
					faction[i].units++;
				for (int j = 0; j < faction[i].units; j++) {
					std::stringstream _sss;
					_sss.clear();
					_sss << "Unit " << j+1;
					std::string _unitNumber = _sss.str();
					if (ImGui::TreeNode(_unitNumber.c_str())) {
						ImGui::InputText("Unit ID", faction[i].unit[j].id, IM_ARRAYSIZE(faction[i].unit[j].id), ImGuiInputTextFlags_CharsNoBlank);
						ImGui::InputText("Unit Name", faction[i].unit[j].name, IM_ARRAYSIZE(faction[i].unit[j].name));
						if (ImGui::Button("Unit Race")) {ImGui::OpenPopup("Unit Race");}
						ImGui::SameLine();
						if (faction[i].unit[j].race != NULL)
							ImGui::Text(faction[i].unit[j].race);
						else
							ImGui::Text("Not Selected");
						if (ImGui::BeginPopup("Unit Race")) {
							if (ImGui::Selectable("Bat")) {faction[i].unit[j].race = "bat";}
							if (ImGui::Selectable("Drake")) {faction[i].unit[j].race = "drake";}
							if (ImGui::Selectable("Elf")) {faction[i].unit[j].race = "elf";}
							if (ImGui::Selectable("Goblin")) {faction[i].unit[j].race = "goblin";}
							if (ImGui::Selectable("Gryphon")) {faction[i].unit[j].race = "gryphon";}
							if (ImGui::Selectable("Human")) {faction[i].unit[j].race = "human";}
							if (ImGui::Selectable("Lizard")) {faction[i].unit[j].race = "lizard";}
							if (ImGui::Selectable("Merman")) {faction[i].unit[j].race = "merman";}
							if (ImGui::Selectable("Monster")) {faction[i].unit[j].race = "monster";}
							if (ImGui::Selectable("Naga")) {faction[i].unit[j].race = "naga";}
							if (ImGui::Selectable("Ogre")) {faction[i].unit[j].race = "ogre";}
							if (ImGui::Selectable("Orc")) {faction[i].unit[j].race = "orc";}
							if (ImGui::Selectable("Troll")) {faction[i].unit[j].race = "troll";}
							if (ImGui::Selectable("Undead")) {faction[i].unit[j].race = "undead";}
							if (ImGui::Selectable("Wolf")) {faction[i].unit[j].race = "wolf";}
							if (ImGui::Selectable("Wose")) {faction[i].unit[j].race = "wose";}
							ImGui::EndPopup();
						}
						ImGui::PushItemWidth(76);
						ImGui::InputInt("Hitpoints", &faction[i].unit[j].hitpoints);
						if (faction[i].unit[j].hitpoints < 0) {faction[i].unit[j].hitpoints = 0;}
						if (faction[i].unit[j].hitpoints > 999) {faction[i].unit[j].hitpoints = 999;}
						ImGui::PushItemWidth(200);
						if (ImGui::Button("Movement Type")) {ImGui::OpenPopup("Movement Type");}
						ImGui::SameLine();
						if (faction[i].unit[j].movement_type != NULL)
							ImGui::Text(faction[i].unit[j].movement_type);
						else
							ImGui::Text("Not Selected");
						if (ImGui::BeginPopup("Movement Type")) {
							if (ImGui::Selectable("ArmoredFoot")) {faction[i].unit[j].movement_type = "armoredfoot";}
							if (ImGui::Selectable("ElusiveFoot")) {faction[i].unit[j].movement_type = "elusivefoot";}
							if (ImGui::Selectable("Fly")) {faction[i].unit[j].movement_type = "fly";}
							if (ImGui::Selectable("GrueFoot")) {faction[i].unit[j].movement_type = "gruefoot";}
							if (ImGui::Selectable("Mounted")) {faction[i].unit[j].movement_type = "mounted";}
							if (ImGui::Selectable("Woodland")) {faction[i].unit[j].movement_type = "woodland";}
							if (ImGui::Selectable("Small Foot")) {faction[i].unit[j].movement_type = "smallfoot";}
							if (ImGui::Selectable("Swimmer")) {faction[i].unit[j].movement_type = "swimmer";}
							if (ImGui::Selectable("Tree Folk")) {faction[i].unit[j].movement_type = "treefolk";}
							if (ImGui::Selectable("Undead Foot")) {faction[i].unit[j].movement_type = "undeadfoot";}
							ImGui::EndPopup();
						}
						ImGui::PushItemWidth(68);
						ImGui::InputInt("Movement Speed", &faction[i].unit[j].movement);
						if (faction[i].unit[j].movement < 0) {faction[i].unit[j].movement = 0;}
						if (faction[i].unit[j].movement > 99) {faction[i].unit[j].movement = 99;}
						ImGui::PushItemWidth(76);
						ImGui::InputInt("Experience to Level Up", &faction[i].unit[j].experience);
						if (faction[i].unit[j].experience < 0) {faction[i].unit[j].experience = 0;}
						if (faction[i].unit[j].experience > 999) {faction[i].unit[j].experience = 999;}
						ImGui::PushItemWidth(62);
						ImGui::InputInt("Level", &faction[i].unit[j].level);
						if (faction[i].unit[j].level < 0) {faction[i].unit[j].level = 0;}
						if (faction[i].unit[j].level > 9) {faction[i].unit[j].level = 9;}
						ImGui::PushItemWidth(200);
						if (ImGui::Button("Alignment")) {ImGui::OpenPopup("Alignment");}
						ImGui::SameLine();
						if (faction[i].unit[j].alignment != NULL)
							ImGui::Text(faction[i].unit[j].alignment);
						else
							ImGui::Text("Not Selected");
						if (ImGui::BeginPopup("Alignment")) {
							if (ImGui::Selectable("Lawful")) {faction[i].unit[j].alignment = "lawful";}
							if (ImGui::Selectable("Liminal")) {faction[i].unit[j].alignment = "liminal";}
							if (ImGui::Selectable("Neutral")) {faction[i].unit[j].alignment = "neutral";}
							if (ImGui::Selectable("Chaotic")) {faction[i].unit[j].alignment = "chaotic";}
							ImGui::EndPopup();
						}
						ImGui::InputText("Advances To (ID)", faction[i].unit[j].advance, IM_ARRAYSIZE(faction[i].unit[j].advance));
						ImGui::PushItemWidth(76);
						ImGui::InputInt("Cost", &faction[i].unit[j].cost);
						if (faction[i].unit[j].cost < 0) {faction[i].unit[j].cost = 0;}
						if (faction[i].unit[j].cost > 999) {faction[i].unit[j].cost = 999;}
						ImGui::PushItemWidth(200);
						if (ImGui::Button("Usage")) {ImGui::OpenPopup("Usage");}
						ImGui::SameLine();
						if (faction[i].unit[j].usage != NULL)
							ImGui::Text(faction[i].unit[j].usage);
						else
							ImGui::Text("Not Selected");
						if (ImGui::BeginPopup("Usage")) {
							if (ImGui::Selectable("Archer")) {faction[i].unit[j].usage = "archer";}
							if (ImGui::Selectable("Fighter")) {faction[i].unit[j].usage = "fighter";}
							if (ImGui::Selectable("Healer")) {faction[i].unit[j].usage = "healer";}
							if (ImGui::Selectable("Mixed Fighter")) {faction[i].unit[j].usage = "mixed fighter";}
							if (ImGui::Selectable("Scout")) {faction[i].unit[j].usage = "scout";}
							ImGui::EndPopup();
						}
						ImGui::PushItemWidth(68);
						ImGui::InputInt("Blade Resistance", &faction[i].unit[j].blade);
						if (faction[i].unit[j].blade < -60) {faction[i].unit[j].blade = 0;}
						if (faction[i].unit[j].blade > 60) {faction[i].unit[j].blade = 60;}
						ImGui::InputInt("Impact Resistance", &faction[i].unit[j].impact);
						if (faction[i].unit[j].impact < 0) {faction[i].unit[j].impact = 0;}
						if (faction[i].unit[j].impact > 60) {faction[i].unit[j].impact = 60;}
						ImGui::InputInt("Pierce Resistance", &faction[i].unit[j].pierce);
						if (faction[i].unit[j].pierce < 0) {faction[i].unit[j].pierce = 0;}
						if (faction[i].unit[j].pierce > 60) {faction[i].unit[j].pierce = 60;}
						ImGui::InputInt("Arcane Resistance", &faction[i].unit[j].arcane);
						if (faction[i].unit[j].arcane < 0) {faction[i].unit[j].arcane = 0;}
						if (faction[i].unit[j].arcane > 60) {faction[i].unit[j].arcane = 60;}
						ImGui::InputInt("Fire Resistance", &faction[i].unit[j].fire);
						if (faction[i].unit[j].fire < 0) {faction[i].unit[j].fire = 0;}
						if (faction[i].unit[j].fire > 60) {faction[i].unit[j].fire = 60;}
						ImGui::InputInt("Cold Resistance", &faction[i].unit[j].cold);
						if (faction[i].unit[j].cold < 0) {faction[i].unit[j].cold = 0;}
						if (faction[i].unit[j].cold > 60) {faction[i].unit[j].cold = 60;}
						ImGui::PushItemWidth(200);
						ImGui::InputTextMultiline("Description", faction[i].unit[j].description, IM_ARRAYSIZE(faction[i].unit[j].description), ImVec2(768, 256));

						// Unit Attacks
						ImGui::Text("Attacks");
						ImGui::SameLine();
						if (ImGui::Button("-1") and faction[i].unit[j].attacks > 0)
							faction[i].unit[j].attacks--;
						ImGui::SameLine();
						if (ImGui::Button("+1") and faction[i].unit[j].attacks < 8)
							faction[i].unit[j].attacks++;
						for (int t = 0; t < faction[i].unit[j].attacks; t++) {
							std::stringstream _ssss;
							_ssss.clear();
							_ssss << "Attack " << t+1;
							std::string _attackNumber = _ssss.str();
							if (ImGui::TreeNode(_attackNumber.c_str())) {
								ImGui::InputText("Attack Name", faction[i].unit[j].attack[t].name, IM_ARRAYSIZE(faction[i].unit[j].attack[t].name));
								ImGui::InputText("Attack Description", faction[i].unit[j].attack[t].description, IM_ARRAYSIZE(faction[i].unit[j].attack[t].description));
								if (ImGui::Button("Attack Type")) {ImGui::OpenPopup("Type");}
								ImGui::SameLine();
								if (faction[i].unit[j].attack[t].type != NULL)
									ImGui::Text(faction[i].unit[j].attack[t].type);
								else
									ImGui::Text("Not Selected");
								if (ImGui::BeginPopup("Type")) {
									if (ImGui::Selectable("Blade")) {faction[i].unit[j].attack[t].type = "blade";}
									if (ImGui::Selectable("Impact")) {faction[i].unit[j].attack[t].type = "impact";}
									if (ImGui::Selectable("Pierce")) {faction[i].unit[j].attack[t].type = "pierce";}
									if (ImGui::Selectable("Arcane")) {faction[i].unit[j].attack[t].type = "arcane";}
									if (ImGui::Selectable("Fire")) {faction[i].unit[j].attack[t].type = "fire";}
									if (ImGui::Selectable("Cold")) {faction[i].unit[j].attack[t].type = "cold";}
									ImGui::EndPopup();
								}
								if (ImGui::Button("Attack Range")) {ImGui::OpenPopup("Range");}
								ImGui::SameLine();
								if (faction[i].unit[j].attack[t].range != NULL)
									ImGui::Text(faction[i].unit[j].attack[t].range);
								else
									ImGui::Text("Not Selected");
								if (ImGui::BeginPopup("Range")) {
									if (ImGui::Selectable("Melee")) {faction[i].unit[j].attack[t].range = "melee";}
									if (ImGui::Selectable("Ranged")) {faction[i].unit[j].attack[t].range = "ranged";}
									ImGui::EndPopup();
								}
								ImGui::PushItemWidth(76);
								ImGui::InputInt("x", &faction[i].unit[j].attack[t].damage);
								ImGui::SameLine();
								ImGui::InputInt(" Damage", &faction[i].unit[j].attack[t].number);
								if (faction[i].unit[j].attack[t].damage < 0) {faction[i].unit[j].attack[t].damage = 0;}
								if (faction[i].unit[j].attack[t].number < 0) {faction[i].unit[j].attack[t].number = 0;}
								if (faction[i].unit[j].attack[t].damage > 999) {faction[i].unit[j].attack[t].damage = 999;}
								if (faction[i].unit[j].attack[t].number > 999) {faction[i].unit[j].attack[t].number = 999;}
								ImGui::PushItemWidth(200);
								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::NewLine();
	if (ImGui::Button("Save Addon")) {save();}

	ImGui::PopItemWidth();
	ImGui::End();
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

int WinMain() {
	main();
	return 0;
}

void save() {
	// Files and Locations
	std::stringstream _ss;
	std::string main_cfg;
	std::string era_cfg;
	std::string addonFolder;
	std::string slash;
	std::string quote;

	// Differing Windows locations from Linux
	if (windowsMode) {
		addonFolder = "\"C:\\Users\\%USERNAME%\\Documents\\My Games\\Wesnoth1.16\\data\\add-ons\\";
		slash = "\\";
		quote = "\"";
	}
	else {
		addonFolder = "~/.local/share/wesnoth/1.16/data/add-ons/";
		slash = "/";
		quote = "";
	}

	// Set main_cfg to main.cfg file location
	_ss.str("");
	_ss << addonFolder << addonName << slash << "_main.cfg";
	main_cfg = _ss.str();

	// Set era_cfg to era.cfg file location
	_ss.str("");
	_ss << addonFolder << addonName << slash << "era.cfg";
	era_cfg = _ss.str();

	// Creates addon directory
	_ss.str("");
	_ss << "mkdir " << addonFolder << addonName << quote;
	system(_ss.str().c_str());

	// Creates the directorys for the factions
	_ss.str("");
	_ss << "mkdir " << addonFolder << addonName << slash << "multiplayer" << quote;
	system(_ss.str().c_str());
	_ss.str("");
	_ss << "mkdir " << addonFolder << addonName << slash << "multiplayer" << slash << "factions" << quote;
	system(_ss.str().c_str());

	// Creates directory for the units
	_ss.str("");
	_ss << "mkdir " << addonFolder << addonName << slash << "units" << quote;
	system(_ss.str().c_str());

	// Creates main.cfg file for writing to
	_ss.str("");
	_ss << "touch " << main_cfg << quote;
	system(_ss.str().c_str());

	// Creates era.cfg file for writing to
	_ss.str("");
	_ss << "touch " << era_cfg << quote;
	system(_ss.str().c_str());

	// Creates the factions' .cfg file for writing to
	for (int i = 0; i < factions; i++) {
		_ss.str("");
		_ss << "touch " << addonFolder << addonName << slash << "multiplayer" << slash << "factions" << slash << faction[i].id << ".cfg" << quote;
		system(_ss.str().c_str());
	}

	// main.cfg contents
	// ================================ //
	_ss.str("");
	_ss << "echo '";

	_ss << "#textdomain wesnoth-" << addonName << "\n\n";
	_ss << "#ifdef MULTIPLAYER\n";
	_ss << "[binary_path]\n";
	_ss << "    path=\"data/add-ons/" << addonName << "\"\n";
	_ss << "[/binary_path]\n\n";
	_ss << "{~add-ons/" << addonName << "/era.cfg}\n\n";

	_ss << "[+units]\n";
	for (int i = 0; i < factions; i++) {
		for (int j = 0; j < faction[i].units; j++) {
			_ss << "    {~add-ons/" << addonName << "/units/" << faction[i].unit[j].id << ".cfg}\n";
		}
	}
	_ss << "[/units]\n";
	_ss << "#endif";

	_ss << "' > " << main_cfg << quote;
	system(_ss.str().c_str());
	// ================================ //

	// era.cfg contents
	// ================================ //
	_ss.str("");
	_ss << "echo '";

	_ss << "[era]\n";
	_ss << "    id=" << eraID << "\n";
	_ss << "    name= _ \"" << eraName << "\"\n";
	_ss << "    {RANDOM_SIDE}\n";

	for (int i = 0; i < factions; i++) {
		_ss << "    {~add-ons/" << addonName << "/multiplayer/factions/" << faction[i].id << ".cfg}\n";
	}

	_ss << "[/era]\n";

	_ss << "' > " << era_cfg << quote;
	system(_ss.str().c_str());
	// ================================ //

	// Factions' .cfg file contents
	// ================================ //
	for (int i = 0; i < factions; i++) {
		_ss.str("");
		_ss << "echo '";

		_ss << "#textdomain wesnoth-" << faction[i].id << "\n";
		_ss << "[multiplayer_side]\n";
		_ss << "    id=" << faction[i].id << "\n";
		_ss << "    name= _\"" << faction[i].name << "\"\n";
		_ss << "    type=random\n";
		_ss << "    leader=" << faction[i].leader << "\n";
		_ss << "    random_leader=" << faction[i].leader << "\n";
		_ss << "    recruit=" << faction[i].recruit << "\n";
		_ss << "    description=\"" << faction[i].description << "\"\n";
		_ss << "    [ai]\n";
		_ss << "        recruitment_pattern=" << faction[i].pattern << "\n";
		_ss << "    [/ai]\n";
		_ss << "[/multiplayer_side]\n";

		_ss << "' > " << addonFolder << addonName << slash << "multiplayer" << slash << "factions" << slash << faction[i].id << ".cfg" << quote;
		system(_ss.str().c_str());
	}
	// ================================ //

	// Units' .cfg file contents
	// ================================ //
	for (int i = 0; i < factions; i++) {
		for (int j = 0; j < faction[i].units; j++) {
			_ss.str("");
			_ss << "echo '";

			_ss << "#textdomain wesnoth-units\n";
			_ss << "[unit_type]\n";
			_ss << "    id=" << faction[i].unit[j].id << "\n";
			_ss << "    name=" << faction[i].unit[j].name << "\n";
			_ss << "    race=" << faction[i].unit[j].race << "\n";
			_ss << "    hitpoints=" << faction[i].unit[j].hitpoints << "\n";
			_ss << "    movement_type=" << faction[i].unit[j].movement_type << "\n";
			_ss << "    movement=" << faction[i].unit[j].movement << "\n";
			_ss << "    experience=" << faction[i].unit[j].experience << "\n";
			_ss << "    level=" << faction[i].unit[j].level << "\n";
			_ss << "    alignment=" << faction[i].unit[j].alignment << "\n";
			_ss << "    advaces_to=" << faction[i].unit[j].advance << "\n";
			_ss << "    {AMLA_DEFAULT}\n";
			_ss << "    cost=" << faction[i].unit[j].cost << "\n";
			_ss << "    usage=" << faction[i].unit[j].usage << "\n";
			_ss << "    description=\"" << faction[i].unit[j].description << "\"\n";
			_ss << "    diesound=groan.wav\n";
			_ss << "    [resistances]\n";
			_ss << "        Blade=" << faction[i].unit[j].blade << "\n";
			_ss << "        Impact=" << faction[i].unit[j].impact << "\n";
			_ss << "        Pierce=" << faction[i].unit[j].pierce << "\n";
			_ss << "        Arcane=" << faction[i].unit[j].arcane << "\n";
			_ss << "        Fire=" << faction[i].unit[j].fire << "\n";
			_ss << "        Cold=" << faction[i].unit[j].cold << "\n";
			_ss << "    [/resistances]\n";

			for (int t = 0; t < faction[i].unit[j].attacks; t++) {
				_ss << "    [attack]\n";
				_ss << "        name=" << faction[i].unit[j].attack[t].name << "\n";
				_ss << "        description=\"" << faction[i].unit[j].attack[t].description << "\"\n";
				_ss << "        type=" << faction[i].unit[j].attack[t].type << "\n";
				_ss << "        range=" << faction[i].unit[j].attack[t].range << "\n";
				_ss << "        damage=" << faction[i].unit[j].attack[t].damage << "\n";
				_ss << "        number=" << faction[i].unit[j].attack[t].number << "\n";
				_ss << "    [/attack]\n";
			}
			_ss << "[/unit_type]\n";

			_ss << "' > " << addonFolder << addonName << slash << "units" << slash << faction[i].unit[j].id << ".cfg" << quote;
			system(_ss.str().c_str());
		}
	}
	// ================================ //
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