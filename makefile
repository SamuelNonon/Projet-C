all:
	gcc `sdl2-config --cflags --libs`  -o main.exe main.c scr/tourelle.c scr/graphics.c scr/etudiant.c scr/jeu.c scr/read.c scr/projectiles.c -Iheaders -W -I/usr/local/include/SDL2 -lSDL2_image -lSDL2_ttf -fsanitize=address && ./main.exe

test:

	gcc `sdl2-config --cflags --libs`  -o main.exe main.c scr/tourelle.c scr/graphics.c scr/etudiant.c scr/jeu.c scr/read.c scr/projectiles.c -Iheaders -W -lSDL2_image -lSDL2_ttf && leaks -atExit --list -- ./main.exe
img : 
	gcc `sdl2-config --cflags --libs` img.c -o img.exe -I/usr/local/include/SDL2 -lSDL2_ttf && ./img.exe

txt:
	gcc `sdl2-config --cflags --libs` test.c -o txt.exe -I/usr/local/include/SDL2 && ./txt.exe