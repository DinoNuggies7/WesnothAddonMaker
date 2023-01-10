all:
	clear
	ccache g++ -c src/main.cpp -o build/main.o
	ccache g++ build/main.o
	./a.out