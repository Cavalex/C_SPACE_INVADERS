#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> 
#include <stdbool.h> // Para criar funções booleanas
#include <conio.h>

#define NUMBER_OF_ENEMIES 24
#define ARRAY_LENGTH 3
#define WIDTH 20
#define LENGTH 50

int missile[] = {0, 0, 0};
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
			if (h == 0 && j >= 8 && j < 16){
				enemies[j][h] = l; // l- 2
			}
			if (h == 0 && j >= 16 && j < 24){
				enemies[j][h] = l;
			}
			
			// Para definir o Y;
			if (h == 1 && j < 8){
				enemies[j][h] = 3;
			}
			if (h == 1 && j >= 8 && j < 16){
				enemies[j][h] = 5;
			}
			if (h == 1 && j >= 16 && j < 24){
				enemies[j][h] = 7;
			}
			
			// Preencher o estado de cada monstro/nave
			if (h == 2)
				enemies[j][h] = 1;
		}
		if (j == 7 || j == 15 || j == 23){
			l = 1;
		}
	}
}

void timer(int n) 
{
    // Tempo inicial
    clock_t start_time = clock(); 
  
    // Uma espécie de sleep, a função timer só acaba quando o while terminar,
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
				printf("A");
				continue;
			}
			
			if (missile[2] == 1){
				if (j == missile[0] && i == missile[1]){
					printf("^");
				}
				else{
					printf(" ");
				}
				
				if (i == missile[1] && i <= 1){
					missile[2] = 0;
				}
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
	
	int tick = 0;
	int i = 0;
	int highestX = 0;
	
	int velocity = -1;
	
	char move;
	
	int loop_enemy = 0;
	
	int y = WIDTH - 3;
	int x = LENGTH / 2;
	
	// Coordenadas do missil
	int mY;
	int mX;
	
	fillEnemies();
	
	setConsoleSize();
	
	// Game loop
	while (true){
		system("cls"); // Para limpar o showMap anterior
		showMap(x, y); // O showMap com as coordenadas do jogador.
		
		// Movimento do jogador
    	if (kbhit()){
    		move = getch();
		}
    	if (move == 75 && x != 1){
			x -= 1;
		}
		if (move == 77 && x != LENGTH - 1){
			x += 1;
		}
		
		// Quando carregamos no "UP", o "missile" fica true e é atualizado no showMap
		if (move == 72 && missile[2] != 1){
			missile[2] = 1;
			missile[1] = y - 1;
			missile[0] = x;
		}
		
		// Movimento dos Monstros
		if (tick % 5 == 0){
			bool changeY = false; // Para saber se muda o Y
			if (enemies[highestX][0] <= 1 && velocity < 0){
				changeY = true;
				velocity = -velocity;
				highestX = 0;
			}
			if (enemies[highestX][0] >= LENGTH - 2 && velocity > 0){
				changeY = true;
				velocity = -velocity;
			}
			// Loop dos inimigos.
			for (i = 0; i < NUMBER_OF_ENEMIES; i++){
				if (velocity < 0 ){
					if (enemies[i][0] < highestX)
						highestX = i;
				}
				if (velocity > 0 ){
					if (enemies[i][0] > highestX)
						highestX = i;
				}
				if (changeY){
					enemies[i][1] += 1;
				}
				enemies[i][0] += velocity;
			}
			changeY = 0;
		}
		
		if(missile[2] == 1){
			missile[1] -= 1;
		}
		
		move = 0;
		timer(10); // Para o loop;
		tick += 1;
		//Beep(4000, 300); // Para fazer sons!
	}
}

