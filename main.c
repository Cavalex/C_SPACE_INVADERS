#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> 
#include <stdbool.h>
//#include <iostream>
#include <conio.h>

// Esta era para o caso de termos gráficos no jogo.
//#include <graphics.h>

int WIDTH = 50;
int LENGTH = 100;

// Função que encontrei para mudar o tamanho da consola.
// Para mudar o tamanho mexam nas variáveis em cima, podem ignorar o código dentro desta função.
void setConsoleSize(){
	HANDLE wHnd;    // Handle to write to the console.
	HANDLE rHnd;    // Handle to read from the console.
	
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    // Change the window title:
    SetConsoleTitle("Space Invaders!");
    // Set up the required window size:
    SMALL_RECT windowSize = {0, 0, LENGTH, WIDTH};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);
    // Change the console window size:
    // Create a COORD to hold the buffer size:
    COORD bufferSize = {10, 10};
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

// Esta função vai ser a mais importante para o jogo.
void showMap(){
	int i, j;
	// Desenhar o mapa
	for(i = 0; i < WIDTH; i++){
		for(j = 0; j < LENGTH; j++){
			
			// Desenhar a parte de cima
			if(i == 0 || i == WIDTH - 1){
				printf("#");
				continue;
			}
			
			// Desenhar a parte de baixo
			if(j == 0 || j == LENGTH - 1){
				printf("#");
				continue;
			}
			
			//############ RESERVADO PARA AS NAVES/BOSS ############
			
			
			else{
				printf(" ");
			}
		}
		printf("\n");
	}
}

// Só para ser mais rápido arranjar um número aleatório.
int getRandomNumber(int min, int max){
	
	// um número aleatório entre 0 e a
	int r = (rand() % (max + 1 - min)) + min;

	return r;
}

int main() {
	
	char ch;

	int pause;
	
	int x;
	int y;
	bool food_bool = false;
	
	setConsoleSize();
	
	while (true){
		showMap();
		sleep(10000); // Para para o programa
	}
	return 0;
}

