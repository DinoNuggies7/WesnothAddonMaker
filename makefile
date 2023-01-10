all:
	clear
	ccache g++ -c src/main.cpp -o build/main.o
	ccache g++ -c src/imgui.cpp -o build/imgui.o
	ccache g++ -c src/imgui_demo.cpp -o build/imgui_demo.o
	ccache g++ -c src/imgui_draw.cpp -o build/imgui_draw.o
	ccache g++ -c src/imgui_tables.cpp -o build/imgui_tables.o
	ccache g++ -c src/imgui_widgets.cpp -o build/imgui_widgets.o
	ccache g++ -c src/imgui_impl_sdl.cpp -o build/imgui_impl_sdl.o
	ccache g++ -c src/imgui_impl_sdlrenderer.cpp -o build/imgui_impl_sdlrenderer.o
	ccache g++ build/main.o build/imgui.o build/imgui_demo.o build/imgui_draw.o build/imgui_tables.o build/imgui_widgets.o build/imgui_impl_sdl.o build/imgui_impl_sdlrenderer.o -lSDL2
	./a.out