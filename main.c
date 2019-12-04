#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> 
#include <stdbool.h> // Para criar funções booleanas
#include <conio.h>

#define NUMBER_OF_ENEMIES 25
#define ARRAY_LENGTH 3
#define WIDTH 30
#define LENGTH 50

int enemies[NUMBER_OF_ENEMIES][ARRAY_LENGTH];

void fillEnemies(){
	int l = 1;
	int j = 0, h = 0;
	for (j = 0; j < NUMBER_OF_ENEMIES; j++){
		l += 5;
		for (h = 0; h < ARRAY_LENGTH; h++){
			// Para definir o X
		
			if (h == 0 && j < 8){
				enemies[j][h] = l;
			}
			if (h == 0 && j >= 8 && j <= 17){
				enemies[j][h] = (l - 2);
			}
			if (h == 0 && j > 16 && j <= 25){
				enemies[j][h] = l;
			}
			
			// Para definir o Y;
			if (h == 1 && j < 8){
				enemies[j][h] = 3;
			}
			if (h == 1 && j >= 8 && j <= 17){
				enemies[j][h] = 5;
			}
			if (h == 1 && j > 16 && j <= 25){
				enemies[j][h] = 7;
			}
			
			
			
			if (h == 2)
				enemies[j][h] = 1;
		}
		if (j == 8 || j == 16 || j == 25){
			l = 1;
		}
	}
}

void delay(int n) 
{
    // Tempo inicial
    clock_t start_time = clock(); 
  
    // Uma espécie de sleep, a função delay só acaba quando o while terminar,
    //logo só termina quando se passar o tempo que colocamos no parâmetro.
    while (clock() < start_time + n) 
        ; 
}

// FunÃ§Ã£o que encontrei para mudar o tamanho da consola.
// Para mudar o tamanho mexam nas variÃ¡veis em cima, podem ignorar o cÃ³digo dentro desta funÃ§Ã£o.
void setConsoleSize(){
	HANDLE wHnd;    // Handle to write to the console.
	
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    // Change the window title:
    SetConsoleTitle("Space Invaders!");
    // Set up the required window size:
    SMALL_RECT windowSize = {0, 0, LENGTH, WIDTH};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);
}

// Esta funÃ§Ã£o vai ser a mais importante para o jogo.
void showMap(int x, int y){
	int i, j, c;
	// Desenhar o mapa
	for(i = 0; i < WIDTH; i++){
		c = 0;
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
			int e = 0;
			int a = 0;
			for (e = 0; e <= NUMBER_OF_ENEMIES; e++){
				if (enemies[e][1] == i){
					if (enemies[e][0] == j){
						printf("T");
						a = 1;
					}
				}
			}
			if (a == 1) {
				a = 0;
				continue;
			}

			if (j == x && i == y){
					printf("O");
			}
			
			
			
			else{
				printf(" ");
			}
		}
		printf("\n");
	}
}

// SÃ³ para ser mais rÃ¡pido arranjar um nÃºmero aleatÃ³rio.
int getRandomNumber(int min, int max){
	
	// um nÃºmero aleatÃ³rio entre 0 e a
	int r = (rand() % (max + 1 - min)) + min;

	return r;
}

void main() {
	
	int y = WIDTH - 3;
	int x = LENGTH / 2;
	
	fillEnemies();
	
	setConsoleSize();
	
	while (true){
		delay(300); // Para os sons.
		system("cls"); // Para limpar o showMap anterior
		showMap(x, y); // O showMap com as coordenadas do jogador.
		Beep(4000, 300); // Para fazer sons!
	}
}
