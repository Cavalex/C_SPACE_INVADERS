#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h> 
#include <stdbool.h> // Para criar funções booleanas
#include <conio.h>

#define NUMBER_OF_ENEMIES 24
#define ARRAY_COMPRIMENTO 3
#define ALTURA 21
#define COMPRIMENTO 50
#define Y_PLAYER (ALTURA - 2)
bool GAME_OVER = false;
int map[COMPRIMENTO][ALTURA];
int velocity = -1; // A velocidade dos inimigos
int SCORE = 0;
int VIDAS = 3;
int MAX_SHOTS = 5;
int shots = 0;
int enemies[NUMBER_OF_ENEMIES][ARRAY_COMPRIMENTO];

// FunÃ§Ã£o que encontrei para mudar o tamanho da consola.
// Para mudar o tamanho mexam nas variÃ¡veis em cima, podem ignorar o cÃ³digo dentro desta funÃ§Ã£o.
void setConsoleSize(int C, int A){
	HANDLE wHnd;    // Handle to write to the console.
	
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    // Change the window title:
    SetConsoleTitle("Space Invaders!");
    // Set up the required window size:
    SMALL_RECT windowSize = {0, 0, C, A};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);
}

void game(){
	void fillEnemies(){
		int l = 1;
		int j = 0, h = 0;
		for (j = 0; j < NUMBER_OF_ENEMIES; j++){
			l += 5;
			for (h = 0; h < ARRAY_COMPRIMENTO; h++){
				
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
	
	void fillMap(int x2, int y2){
		int x = 0;
		int y = 0;
		int e = 0;
		for (y = 0; y < ALTURA; y++){
			for (x = 0; x < COMPRIMENTO; x++){
				
				if(y == 0 || y == ALTURA - 1){
					map[x][y] = 0;
					continue;
				}
				
				// Desenhar a parte de baixo
				if(x == 0 || x == COMPRIMENTO - 1){
					map[x][y] = 0;
					continue;
				}
				
				// Preencher inimigos
				int a = 0;
				for (e = 0; e <= NUMBER_OF_ENEMIES; e++){
					if (enemies[e][2] == 1){
						if (enemies[e][1] == y){
							if (enemies[e][0] == x){
								map[x][y] = 10;
								a = 1;
							}
						}
					}
				}
				if (a == 1){
					a = 0;
					continue;
				}
				
				if (x == x2 && y == y2){
					map[x][y] = 2;
					continue;
				}
				
				// Fazer barreiras
				if ((x == 6 || x == 7 || x == 16 || x == 17 || x == 26 || x == 27 || x == 36 || x == 37 ) && (y == Y_PLAYER - 2 || y == Y_PLAYER - 3)){
					map[x][y] = 9;
				}
				
				else{
					map[x][y] = 1;
				}
			}
			printf("");
		}
	}
	
	void timer(int n) {
	    // Tempo inicial
	    clock_t start_time = clock(); 
	  
	    // Uma espécie de sleep, a função timer só acaba quando o while terminar,
	    //logo só termina quando se passar o tempo que colocamos no parâmetro.
	    while (clock() < start_time + n) 
			;
	}
	
	void showMap(int new_xP, int old_xP){
		bool changeY = false;
		// Visto que não podemos somente declarar a variável sozinha (dá erro ao atualizar se a variável não tem valor)
		// vamos colocar coordenadas que são impossíveis de obter, negativas.
		int skipShotX = -100;
		int skipShotY = -100;
		int y = 0;
		int x = 0;
		int tick = 0;
		int shotss = 0;
		for (y = 0; y < ALTURA; y++){
			for (x = 0; x < COMPRIMENTO; x++){
				
				if (y >= ALTURA - 1 && (map[x][y] == 10 || map[x][y] == 11 || map[x][y] == 12 || map[x][y] == 13 || map[x][y] == 14 )){
					GAME_OVER = true;
				}
				
				// Movimento lateral
				if (map[x][y] == 14){
					if (velocity < 0){
						map[x-1][y] = 10;
						map[x][y] = 1;
					}
					else {
						map[x+1][y] = 10;
						map[x][y] = 1;
					}
				}
				
				if ((map[x][y] == 13 && x <= 2) || (map[x][y] == 13 && x >= COMPRIMENTO - 3)){
					if (map[x-1][y] == 9){
						map[x-1][y] = 1;
					}
					if (map[x+1][y] == 9){
						map[x+1][y] = 1;
					}
					// MUDAR DE LINHA
					if (velocity < 0 && x <= 2){
						changeY = true;
					}
					if (velocity > 0 && x >= COMPRIMENTO - 3){
						changeY = true;
					}
					// Movimento vertical
					if (changeY){
						int y2 = 0;
						int x2 = 0;
						for(y2 = 0; y2 < ALTURA; y2++){
							for(x2 = 0; x2 < COMPRIMENTO; x2++){
								if (map[x2][y2] >= 13){
									map[x2][y2+1] = 10;
									map[x2][y2] = 1;
								}
							}
						}
						changeY = false;
						velocity = -velocity;
					}
				}
				
				// Atualização do jogador
				if(x == new_xP && new_xP != old_xP && y == Y_PLAYER){
					map[x][y] = 2;
				}
				if(x == old_xP && new_xP != old_xP && y == Y_PLAYER){
					map[x][y] = 1;
				}
				
				void printMap(){
					switch(map[x][y]){
						// Casos normais que duram 1 tick
						case 0: printf("#"); break; // BARREIRA
						case 1: printf(" "); break; // ESPAÇO
						case 2: printf("A"); break; // JOGADOR
						case 4: printf("^"); break; // MISSIL JOGADOR
						case 5: 
							if(map[x][y-1] == 7){
								printf(" ");
							}
							else{
								printf("|");
							}
							break; // MISSIL INIMIGO
						case 6: printf("X"); break; // EXPLOSION
						case 7: printf(" "); break; // CASO ESPECIAL DO MISSIL INIMIGO
						case 8: printf("%c", 209); break; // CASO ESPECIAL MISSIL-INIMIGO EM CIMA DE INIMIGO
						case 9: printf("%c", 233); break; // BARREIRA
						
						// INIMIGOS
						case 10: printf("%c", 209); map[x][y] += 1; break;
						case 11: printf("%c", 209); map[x][y] += 1; break;
						case 12: printf("%c", 209); map[x][y] += 1; break;
						case 13: printf("%c", 209); map[x][y] += 1; break;
						
						// RESERVADO
						default: printf("%d", map[x][y]); break;
					}
				}
				printMap();
				
				// Passar de X para Espaço Vazio
				if(map[x][y] == 6){
					map[x][y] = 1;
				}
				
				// Spawn Missil Inimigo
				if (map[x][y] == 10 || map[x][y] == 11 || map[x][y] == 12 || map[x][y] == 13 || map[x][y] == 14 ){
					if (!(map[x][y+2] == 10 || map[x][y+2] == 11 || map[x][y+2] == 12 || map[x][y+2] == 13 || map[x][y+2] == 14)){
						if (!(map[x][y+4] == 10 || map[x][y+4] == 11 || map[x][y+4] == 12 || map[x][y+4] == 13 || map[x][y+4] == 14)){
							//int r = getRandomNumber(1, 30);
							int r = (rand() % (30 + 1 - 1)) + 1;
							if (r == 1){
								map[x][y+1] = 5; 
							}
						}
					}
				}
				
				// Atualização do missil inimigo
				// Já que esta função atualiza de baixo para cima e o tiro tbm vai de baixo para cima, 
				// temos que nos certificar que ele não escreveu já um "|", logo criamos um caso especial,
				// o 7, que dá "track" ao movimento do tiro.
				if(map[x][y] == 5){
					// 1 Se acerta no jogador
					if (map[x][y+1] == 2){
						printMap();
						if(VIDAS == 0){
							GAME_OVER = true;
						}
						else{
							VIDAS -= 1;
						}
						map[x][y-1] = 1;
					}
					// 2 Se acerta no jogador
					else if (map[x][y] == 2){
						printMap();
						GAME_OVER = true;
						map[x][y-1] = 1;
					}
					// Se bate na barreira
					else if (y >= ALTURA - 2){
						map[x][y] = 1;
						map[x][y-1] = 1;
					}
					else if (map[x][y+1] == 9){
						map[x][y] = 1;
						map[x][y+1] = 1;
					}
					// Se tem um espaço vazio à frente
					else{
						if (!(map[x][y-1] == 7)){
							map[x][y] = 7;
							map[x][y+1] = 5;
						}
						else{
							map[x][y-1] = 1;
						}
					}
				}
				
				// Atualização do missil do jogador
				if(map[x][y] == 4){
					// Colisão
					if (map[x][y-1] == 10 || map[x][y-1] == 11 || map[x][y-1] == 12 || map[x][y-1] == 13 || map[x][y-1] == 14){
						map[x][y-1] = 6;
						map[x][y] = 1;
						// RESERVADO PARA DIFICULDADE
						SCORE += 100;
					}
					// Limite
					else if (y <= 1){
						map[x][y] = 1;
						shotss -= 1;
					}
					// Movimento
					else{
						shotss += 1;
						map[x][y-1] = 4;
						map[x][y] = 1;
					}
				}
			}
			printf("\n");
		}
		shots = shotss;
	}
	
	// SÃ³ para ser mais rÃ¡pido arranjar um nÃºmero aleatÃ³rio.
	int getRandomNumber(int min, int max){
		// um nÃºmero aleatÃ³rio entre 0 e a
		int r = (rand() % (max + 1 - min)) + min;
	
		return r;
	}
	
	void startGame(){
		
		int tick = 0;
		int i = 0;
		int highestX = 0; // O que está mais à esquerda ou mais à direita
		
		char move;
		
		int loop_enemy = 0;
		
		int y = Y_PLAYER;
		int x = COMPRIMENTO / 2;
		int old_x = x;
		
		fillEnemies();
		fillMap(x, y);
		
		setConsoleSize(COMPRIMENTO, ALTURA);
		
		srand(time(NULL));
		
		// Game loop
		while (true){
			system("cls"); // Para limpar o showMap anterior
			//showMap(x, y); // O showMap com as coordenadas do jogador.
			showMap(x, old_x);
			
			// GAME OVER
			if (GAME_OVER){
				char nome[100];
				system("cls");
				printf("\n        GAME OVER!!!"); // MESNAGEM DE GAME OVER
				printf("         PONTUACAO: %d", SCORE);
				printf("\n\n Qual e o seu nome? --> ");
				gets(nome);
				printf(" ");
				puts(nome);
				// RESERVADO Espaço para a leaderboard
				
				Sleep(2000);
				break;
			}
			
			// Movimento do jogador
	    	if (kbhit()){
	    		move = getch();
			}
	    	if (move == 75 && x > 1){
	    		old_x = x;
				x -= 1;
			}
			if (move == 77 && x < COMPRIMENTO - 2){
				old_x = x;
				x += 1;
			}
			
			// Quando carregamos no "UP", é lançado um míssil
			if (move == 72){
				if (!(shots >= MAX_SHOTS)){
					map[x][Y_PLAYER-1] = 4;
				}
			}
			
			move = 0;
			timer(10); // Para o loop;
			//Sleep(10);
			tick += 1;
			if(VIDAS == 1){     
				printf("SCORE: %d     \t\t\t VIDAS:        <3", SCORE);
			}
			else if (VIDAS == 2){
				printf("SCORE: %d     \t\t\t VIDAS:     <3 <3", SCORE);
			}
			else if (VIDAS == 3){
				printf("SCORE: %d     \t\t\t VIDAS:  <3 <3 <3", SCORE);
			}
			//Beep(4000, 300); // Para fazer sons!
		}
	}
	startGame();
}

void easterEggs()
{
	system("cls");
	char * nada = "a";
    char * string_1 = "Neve no cimo dos montes";
    char * string_2 = "Parece sol ca de baixo.";
    char * string_3 = "Tenho um sonho de quem quero";
    char * string_4 = "Mas se a procuro nao acho.";
    printf("                                  /   \\ \n");
    printf(" _                        )      ((   ))     (\n");
    printf("(@)                      /|\\      ))_((     /|\\ \n");
    printf("|-|                     / | \\    (/\\|/\\)   / | \\                      (@)\n");
    printf("| | -------------------/--|-voV---\\`|'/--Vov-|--\\---------------------|-|\n");
    printf("|-|                         '^`   (o o)  '^`                          | |\n");
    printf("| |                               `\\Y/'                               |-|\n");
    printf("|-|                      %s                      | |\n", string_1);
    printf("| |                      %s                      |-|\n", string_2);
    printf("|-|                      %s                 | |\n", string_3);
    printf("| |                      %s                   |-|\n", string_4);
    printf("|_|___________________________________________________________________| |\n");
    printf("(@)              l   /\\ /         ( (       \\ /\\   l                `\\|-|\n");
    printf("                 l /   V           \\ \\       V   \\ l                  (@)\n");
    printf("                 l/                _) )_          \\I\n");
    printf("                                   `\\ /'\n");
    printf("                                            \n");
    printf("");
    scanf("%s", &nada);
    scanf("%s", &nada);
    printf("What are you doing here? Go whine about the meaning of life with that poem...");
    scanf("%s", &nada);
    printf("Boy oh boy, you don't want to see my special pla- *ahem* dragon food... do you?");
    scanf("%s", &nada);
    printf("Are you still here? Really? Are you that bored?");
    scanf("%s", &nada);
    printf("Do you want me to show you the dragon food for you to stop bothering me?");
    scanf("%s", &nada);
    printf("Ok then I guess I'll show you my plans... but promise to not tell anyone you hearing me?");
    scanf("%s", &nada);
    printf("...WIP...");
    printf("Press anything to go to the main menu: ");
    scanf("%s", &nada);
    main();
}

bool missed = false;
void menu(){
	system("cls");
	
	if (missed){
		printf("\n You missed the available numbers, try again.");
		missed = false;
	}
	int choice;
	setConsoleSize(30, 50);
	printf("\n Initializing");
	Sleep(450);
	printf(".");
	Sleep(350);
	printf(".");
	Sleep(350);
	printf(".\n\n\n");
	Sleep(350);
	printf(" 1 --> SPACE INVADERS");
	printf("\n 2 --> SETTINGS");
	printf("\n 3 --> EXIT\n");
	puts(""
	"                       *     .--.\n"
	"                            / /  `\n"
	"           +               | |\n"
	"                  '         \ \__,\n"
	"              *          +   '--'  *\n"
	"                  +   /\\\n"
	"     +              .'  '.   *\n"
	"            *      /======\\      +\n"
	"                  ;:.  _   ;\n"
	"                  |:. (_)  |\n"
	"                  |:.  _   |\n"
	"        +         |:. (_)  |          *\n"
	"                  ;:.      ;\n"
	"                .' \:.    / `.\n"
	"               / .-'':._.'`-. \\\n"
	"               |/    /||\\    \\|\n"
	"             _..--\"\"\"````\"\"\"--..\n"
	"       _.-'``                    ``'-._\n"
	"     -'                                '-\n"
	""
	);

	scanf("%d", &choice);
	switch (choice){
		case 1: game(); system("cls"); break;
		//case 2: settings(); system("cls"); break;
		//case 3: credits(); break;
		case 4: easterEggs(); system("cls"); break;
		default: missed = true; main(); break;
	}
}

void main(){
	menu();
}
