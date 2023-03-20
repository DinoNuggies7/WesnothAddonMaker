all:
	clear
	mkdir -p build/
	ccache g++ -c src/main.cpp -o build/main.o
	ccache g++ -c src/imgui.cpp -o build/imgui.o
	ccache g++ -c src/imgui_demo.cpp -o build/imgui_demo.o
	ccache g++ -c src/imgui_draw.cpp -o build/imgui_draw.o
	ccache g++ -c src/imgui_tables.cpp -o build/imgui_tables.o
	ccache g++ -c src/imgui_widgets.cpp -o build/imgui_widgets.o
	ccache g++ -c src/imgui_impl_sdl.cpp -o build/imgui_impl_sdl.o
	ccache g++ -c src/imgui_impl_sdlrenderer.cpp -o build/imgui_impl_sdlrenderer.o
	ccache g++ build/*.o -lSDL2

windows:
	clear
	mkdir -p build/
	ccache x86_64-w64-mingw32-g++ -c src/main.cpp -o build/main.o
	ccache x86_64-w64-mingw32-g++ -c src/imgui.cpp -o build/imgui.o
	ccache x86_64-w64-mingw32-g++ -c src/imgui_demo.cpp -o build/imgui_demo.o
	ccache x86_64-w64-mingw32-g++ -c src/imgui_draw.cpp -o build/imgui_draw.o
	ccache x86_64-w64-mingw32-g++ -c src/imgui_tables.cpp -o build/imgui_tables.o
	ccache x86_64-w64-mingw32-g++ -c src/imgui_widgets.cpp -o build/imgui_widgets.o
	ccache x86_64-w64-mingw32-g++ -c src/imgui_impl_sdl.cpp -o build/imgui_impl_sdl.o
	ccache x86_64-w64-mingw32-g++ -c src/imgui_impl_sdlrenderer.cpp -o build/imgui_impl_sdlrenderer.o
	ccache x86_64-w64-mingw32-g++ -o WesnothAddonMaker.exe build/*.o -lSDL2 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic