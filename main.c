#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h> // Para criar funções booleanas
#include <conio.h>
#include <locale.h>

#define NUMBER_OF_ENEMIES 24
#define ARRAY_COMPRIMENTO 3
#define ALTURA 21
#define COMPRIMENTO 50
#define Y_PLAYER (ALTURA - 2)
bool GAME_OVER = false;
bool WON_GAME = false;
int dif = 1;
int map[COMPRIMENTO][ALTURA];
int velocity = -1; // A velocidade dos inimigos
int SCORE = 0;
int VIDAS;
int MAX_SHOTS = 5;
int MAX_ROUNDS = 3;
int BOSS_HP = 10;
int shots = 0;
int enemies[NUMBER_OF_ENEMIES][ARRAY_COMPRIMENTO];
int ScoreLine;
//bool invulnerability = true;

// PARA TESTAR
void showM(int m[COMPRIMENTO][ALTURA]){
	int x = 0, y = 0;
	for (y = 0; y < ALTURA; y++){
		for (x = 0; x < COMPRIMENTO; x++){
			printf("%d", m[x][y]);
		}
		printf("\n");
	}
}

void printHighScore(void){
	char * novalinha;
	char personHighScore[150];
	void getNome(){
		int n = 0;
		FILE * file = fopen("nomes.txt", "r");
		while(!feof(file)){
			n++;
			fgets(personHighScore, 150, file);
			novalinha = strtok(personHighScore, "\n");
			if (n == ScoreLine) break;
		}
		fclose(file);
		//printf("%s", nome2);
	}

	FILE * file;
	file = fopen("scores.txt", "r");
	int c;
	int n = 0;
	int b = -1;
	while(!feof(file)){
		fscanf(file, "%d", &c);
		if (fgetc(file) == '\n') n++;
		if (c > b){
			b = c;
			ScoreLine = n;
		}
	}
	getNome();
	//printf("\n HighScore - %d linha - %d", b, ScoreLine);
	printf(" HighScore:\n--> %s %d ", novalinha, b);
	fclose(file);
}

// Função que encontrei para mudar o tamanho da consola.
// Para mudar o tamanho mexam nas variáveis acima, podem ignorar o código dentro desta função.
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

                // Desenhar a parte de cima e de baixo
				if(y == 0 || y == ALTURA - 1){
					map[x][y] = 0;
					continue;
				}

				// Desenhar a parte lateral
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

                // O jogador
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

    void fillMapBoss(int x2, int y2){
		int x = 0;
		int y = 0;
		int e = 0;
		for (y = 0; y < ALTURA; y++){
			for (x = 0; x < COMPRIMENTO; x++){

                // Desenhar a parte de cima e de baixo
				if(y == 0 || y == ALTURA - 1){
					map[x][y] = 0;
					continue;
				}

				// Desenhar a parte lateral
				if(x == 0 || x == COMPRIMENTO - 1){
					map[x][y] = 0;
					continue;
				}

                // O boss
                if(x == COMPRIMENTO / 2 && y == 3){
                    map[x][y] = 20;
                    continue;
                }
                if(map[x - 1][y] == 20){
                    map[x][y] = 21;
                    continue;
                }
                if(map[x][y - 1] == 20){
                    map[x][y] = 22;
                    continue;
                }
                if(map[x - 1][y] == 22){
                    map[x][y] = 23;
                    continue;
                }

                // O jogador
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
		bool noEnemies = true;
		// Visto que não podemos somente declarar a variável sozinha (da erro ao atualizar se a variável não tem valor)
		// vamos colocar coordenadas que são impossíveis de obter, negativas.
		int skipShotX = -100;
		int skipShotY = -100;
		int y = 0;
		int x = 0;
		int shotss = 0;
		for (y = 0; y < ALTURA; y++){
			for (x = 0; x < COMPRIMENTO; x++){

				// SE ESTÁ MORTO, ESTÁ MORTO
				if (VIDAS == 0){
					GAME_OVER = true;
				}

                // Perde-se o jogo quando os inimigos chegam ao chão
				if (y >= ALTURA - 1 && (map[x][y] == 10 || map[x][y] == 11 || map[x][y] == 12 || map[x][y] == 13 || map[x][y] == 14 )){
					GAME_OVER = true;
				}

                // Contagem de inimigos para saber se o jogo acabou ou não
				// Se não há inimigos, noEnemies fica true e o jogo acaba, senão noEnemies fica false e continua o jogo
				if ((map[x][y] == 10 || map[x][y] == 11 || map[x][y] == 12 || map[x][y] == 13 || map[x][y] == 14 || map[x][y] == 20 || map[x][y] == 24) && noEnemies == true)
                    noEnemies = false;

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

				// MUDAR DE LINHA - BOSS
				if((map[x][y] == 24 && x <= 2 && velocity < 0) || (map[x][y] == 25 && x >= COMPRIMENTO - 3 && velocity > 0)){
                    if (map[x][y] == 24 && velocity < 0){ // Temos de apagar o de cima e mov»e-lo para baixo
                        map[x][y] = 1;
                        map[x+1][y] = 1;
                        map[x][y+1] = 20;
                        map[x+1][y+1] = 21;
                        map[x][y+2] = 22;
                        map[x+1][y+2] = 23;
                    }
                    if (map[x][y] == 25 && velocity > 0){
                        map[x][y] = 1;
                        map[x-1][y] = 1;
                        map[x-1][y+1] = 20;
                        map[x][y+1] = 21;
                        map[x-1][y+2] = 22;
                        map[x][y+2] = 23;
                    }
                    velocity = -velocity;
				}

                // CASO ESPECIAL BOSS
                if(map[x][y] == 29) map[x][y] = 1;
                if(map[x][y] == 28) map[x][y] = 29;

				// Movimento lateral Boss
                if (map[x][y] == 24 && velocity < 0){
						map[x-1][y] = 20;
						map[x][y] = 21;
						map[x][y+1] = 23;
						map[x-1][y+1] = 22;
						map[x+1][y] = 1;
						map[x+1][y+1] = 1;
				}else if (map[x][y] == 25 && velocity > 0 && map[x-2][y] != 29 && map[x-2][y] != 28){
                        map[x][y] = 20;
						map[x+1][y] = 21;
						map[x][y+1] = 22;
						map[x+1][y+1] = 23;
						map[x-1][y] = 28;
						map[x-1][y+1] = 1;
				}

				// MUDAR DE LINHA - INIMIGO
				if ((map[x][y] == 13 && x <= 2) || (map[x][y] == 13 && x >= COMPRIMENTO - 3)){
					if (map[x-1][y] == 9){
						map[x-1][y] = 1;
					}
					if (map[x+1][y] == 9){
						map[x+1][y] = 1;
					}
					// MUDAR DE LINHA
					if (velocity < 0 && x <= 1){
						changeY = true;
					}
					if (velocity > 0 && x >= COMPRIMENTO - 2){
						changeY = true;
					}
					// Movimento vertical
					if (changeY){
						int y2 = 0;
						int x2 = 0;
						for(y2 = 0; y2 < ALTURA; y2++){
							for(x2 = 0; x2 < COMPRIMENTO; x2++){
								if (map[x2][y2] >= 13){
                                    if (y == Y_PLAYER) GAME_OVER = true;
                                    else {
                                        map[x2][y2+1] = 10;
                                        map[x2][y2] = 1;
                                    }
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
						case 6: printf("X"); break; // EXPLOSIONNNNN
						case 7: printf(" "); break; // CASO ESPECIAL DO MISSIL INIMIGO
						case 8: printf("%c", 203); break; // CASO ESPECIAL MISSIL-INIMIGO EM CIMA DE INIMIGO
						case 9: printf("%c", 219); break; // BARREIRA

						// INIMIGOS
						case 10: printf("%c", 203); map[x][y] += 1; break;
						case 11: printf("%c", 203); map[x][y] += 1; break;
						case 12: printf("%c", 203); map[x][y] += 1; break;
						case 13: printf("%c", 203); map[x][y] += 1; break;

						// BOSS --> SÃO 4 PARTES, MAS MOVEM-SE MAIS RAPIDAMENTE QUE OS MONSTROS, O DOBRO DA VELOCIDADE, LOGO PRECISAMOS DE 8 CASOS
						case 20: printf("/"); map[x][y] += 4; break;
						case 21: printf("\\"); map[x][y] += 4; break;
						case 22: printf("\\"); map[x][y] += 4; break;
						case 23: printf("/"); map[x][y] += 4; break;
						case 24: printf("/"); break;
						case 25: printf("\\"); break;
						case 26: printf("\\"); break;
						case 27: printf("/"); break;

						case 28: printf(" "); break; // CASO1 ESPECIAL DO BOSS
						case 29: printf(" "); break; // CASO2 ESPECIAL DO BOSS

						case 34: printf("X"); break; // EXPLOSÃO DO BOSS

						case 40: // BONUS 1 --> DÁ 150 PONTOS, COMUM
						    if(map[x][y-1] == 42){
								printf(" ");
							}
							else{
								printf("o");
							}
						    break;
						case 41: // BONUS 2 --> DÁ UMA VIDA, MUITO RARO
						    if(map[x][y-1] == 42){
								printf(" ");
							}
							else{
								printf("O");
							}
						    break;
						case 42: printf(" "); break; // CASO ESPECIAL BONUS(ES)(lol)

						case 43: printf("o");
						case 44: printf("O");

						default: printf("%d", map[x][y]); break;
					}
				}

				printMap();

				// Passar de X para Espaço Vazio
				if(map[x][y] == 6 || map[x][y] == 34){
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
							// Se o missil faz spawn diretamente em cima do jogador.
							if(map[x][y+2] == 2)
                                VIDAS -= 1;
						}
					}
				}

                // UM TIRO DO BOSS
                /*
				if (map[x][y] == 22 || map[x][y] == 23){
                    int r = (rand() % (10 + 1 - 1)) + 1;
                    if (r == 1){
                        map[x][y+1] = 5;
                    }
				}
				*/

				// DOIS TIROS DO BOSS
                if (map[x][y] == 22 || map[x][y] == 26){
                    int r = (rand() % (10 + 1 - 1)) + 1;
                    if (r == 1){
                        map[x][y+1] = 5;
                        map[x+1][y+1] = 5;
                    }
				}

				// Atualização do missil inimigo
				// Já que esta função atualiza de baixo para cima e o tiro tbm vai de baixo para cima,
				// temos que nos certificar que ele não escreveu já um "|", logo criamos um caso especial,
				// o 7, que dá "track" ao movimento do tiro.
				if(map[x][y] == 5){
                    // Se tem um inimigo atrás e a barreira à frente, ou seja,
                    // se há 1 espaço de diferença entre o inimigo e a barreira.
                    if(map[x][y+1] == 0 && (map[x][y-1] == 10 || map[x][y-1] == 11 || map[x][y-1] == 12 || map[x][y-1] == 13 || map[x][y-1] == 14)){ // Se o que está à frente é barreira e o que está atrás é inimigo:
						map[x][y-1] = 1;
                    }
					// 1 Se acerta no jogador
					else if (map[x][y+1] == 2 && map[x][y-1] == 7){
						if(VIDAS == 0){
							GAME_OVER = true;
						}
						else{
							if(VIDAS == 3){
								VIDAS = 2;
							}
							else if(VIDAS == 2){
								VIDAS = 1;
							}
							else if(VIDAS == 1){
								VIDAS = 0;
								GAME_OVER = true;
							}
							//VIDAS -= 1;
						}
						map[x][y-1] = 1;
						printMap();
					}
					/*
					// 2 Se acerta no jogador
					else if (map[x][y] == 2 && map[x][y-1] == 7){
						VIDAS -= 1;
						if (VIDAS == 0){
                            GAME_OVER = true;
						}
						map[x][y-1] = 1;
						printMap();
					}
					*/
					// Se bate na barreira
					else if (y >= ALTURA - 2){
						map[x][y] = 1;
						// Este if está aqui para no caso do inimigo ter acabado de dar spawn ao tiro, este não o eliminar.
						if (!(map[x][y-1] == 10 || map[x][y-1] == 11 || map[x][y-1] == 12 || map[x][y-1] == 13 || map[x][y-1] == 14))
                            map[x][y-1] = 1;
					}
					// Se bate na parede
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
						SCORE += 100;

						// SPAWN DO BÓNUS
						int r1 = (rand() % (30 + 1 - 1)) + 1; // 3.3% de spawn
						int r2 = (rand() % (100 + 1 - 1)) + 1; // 1% de spawn
                        if (r1 == 1 && map[x][y+1] == 1 && map[x][y+3] == 1) map[x][y+1] = 40;
                        else if (r2 == 1 && map[x][y+1] == 1 && map[x][y+3] == 1 && VIDAS <= 2) map[x][y+1] = 41;
					}
					// Colisão com o boss
                    else if (map[x][y-1] == 22 || map[x][y-1] == 23 || map[x][y-1] == 26 || map[x][y-1] == 27){
						map[x][y] = 34;
                        switch(dif){
                            case 1: SCORE += 200; break;
                            case 2: SCORE += 250; break;
                            case 3: SCORE += 300; break;
                            default: SCORE += 200; break;
                        }
						BOSS_HP -= 1;
					}
					// Se bate na parede
					else if (map[x][y-1] == 9){
						map[x][y] = 1;
						map[x][y-1] = 1;
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

				// Movimento do(s) bónus
				if(map[x][y] == 40 || map[x][y] == 41){
					// Se acerta no jogador
					if (map[x][y+1] == 2 && map[x][y-1] == 42){
                        if (map[x][y] == 40) SCORE += 150;
                        else if (map[x][y] == 41 && VIDAS <= 3) VIDAS += 1;
						map[x][y-1] = 1;
					}
					// Se bate na barreira
					// // Vamos fazer com que ele fique parado no chão
					else if (y >= ALTURA - 2){
                        map[x][y-1] = 1;
                        map[x][y] = 1;
                        /*
						if (map[x][y] == 40) map[x][y] = 43;
						if (map[x][y] == 41) map[x][y] = 44;
                        */
					}
					// Vamos fazer que se o bónus bate na barreira ele desaparece, não temos como fazê-lo passar pela parede.
					// Se bate na parede
					else if (map[x][y+1] == 9){
						map[x][y] = 1;
						map[x][y-1] = 1;
					}
					// Se tem um espaaço vazio à frente
					else{
						if (!(map[x][y-1] == 42)){
							if (map[x][y] == 40) map[x][y+1] = 40;
							if (map[x][y] == 41) map[x][y+1] = 41;
                            map[x][y] = 42;
						}
						else{
							map[x][y-1] = 1;
						}
					}
				}
			}
			printf("\n");
		}
		shots = shotss;

        // Para acabar o jogo:
        if (noEnemies){ // GANHASTE!
            GAME_OVER = true;
            WON_GAME = true;
        }
        else noEnemies = true;

        if (BOSS_HP <= 0){
            GAME_OVER = true;
            WON_GAME = true;
        }
	}

	// Só para ser mais rápido arranjar um número aleatório
	int getRandomNumber(int min, int max){
		// um número aleatório entre 0 e a
		int r = (rand() % (max + 1 - min)) + min;

		return r;
	}

	void startGame(){

		switch(dif){
			case 1: VIDAS = 3; break;
			case 2: VIDAS = 2; break;
			case 3: VIDAS = 1; break;
			default: VIDAS = 3; break;
		}

		int i = 0;
		int highestX = 0; // O que está mais à esquerda ou mais à direita

		char move;

		int loop_enemy = 0;

		int y = Y_PLAYER;
		int x = COMPRIMENTO / 2;
		int old_x = x;
		int rounds = 1; // A primeira tbm conta

		fillEnemies();
		fillMap(x, y);
		// Esta linha é para testes.
		//fillMapBoss(x, y);

		setConsoleSize(COMPRIMENTO, ALTURA);

		srand(time(NULL));

		// Game loop
		while (true){
			system("cls"); // Para limpar o showMap anterior
			//showMap(x, y); // O showMap com as coordenadas do jogador.
			showMap(x, old_x);

			// GAME OVER
			if (GAME_OVER){
                rounds += 1;
                // Se o jogo já acabou:
                if(rounds > MAX_ROUNDS){
                    // Isto aqui é só para fazer testes
                    //showM(map);
                    //printf(noEnemies ? "true" : "false");
                    //Sleep(200000);
                    int ScoreLine;
                    char nome[100];
                    system("cls");
                    // Depois é para eliminar isto!
                    if (WON_GAME == true)
                        printf("\n       GANHASTE!!! :D");
                        if (VIDAS == 3) SCORE += 300;
                        else if (VIDAS == 2) SCORE += 200;
                        else if (VIDAS == 1) SCORE += 100;
                    else
                        printf("\n       PERDESTE!!! D:"); // MESNAGEM DE GAME OVER
                    Sleep(1000);
                    printf("        PONTUACAO: %d", SCORE);
                    printf("\n\n Qual e o seu nome? --> ");
                    //gets(nome);
                    // Queremos ter o scanf porque se houver algum tipo de erro ao acabar o jogo,
                    // como por exemplo spammar as teclas, não queremos que o nome do jogador fique "1".
                    //printf(" "); // ???
                    scanf("%s", &nome);
					//scanf("%d", &score);
					//printf("%s %d", nome, SCORE);
					FILE * file = fopen("scores.txt", "a");
					fprintf(file, "%d\n", SCORE);
					fclose(file);
					FILE * file2 = fopen("nomes.txt", "a");
					fprintf(file2, "%s\n", nome);
					fclose(file2);
					printHighScore();
					printf("\n Press anyhting to close the game: ");
					char a;
					scanf("%c", a);
					rounds = 1;
                	y = Y_PLAYER;
                    x = COMPRIMENTO / 2;
                    fillEnemies();
                    fillMap(x, y);
                    switch(dif){
                    	case 1: VIDAS = 3; break;
                    	case 2: VIDAS = 2; break;
                    	case 3: VIDAS = 1; break;
                    	default: VIDAS = 3; break;
					}
                    SCORE = 0;
                    GAME_OVER = false;
                    WON_GAME = false;
                    main();
                }
                // Se o jogo ainda não acabou
                else {
                    // Se for a última ronda, então luta-se contra o boss, senão é uma ronda normal
                    if(rounds == MAX_ROUNDS){
                        system("cls");
                        printf("                  ROUND %d", rounds);
                        // Para recomeçar o jogo
                        y = Y_PLAYER;
                        x = COMPRIMENTO / 2;
                        fillMapBoss(x, y);
                        switch(dif){
                        	case 1: VIDAS = 3; break;
                        	case 2: VIDAS = 2; break;
                        	case 3: VIDAS = 1; break;
                        	default: VIDAS = 3; break;
						}
                        GAME_OVER = false;
                        WON_GAME = false;
                        Sleep(4000);
                    }
                    else{
                        system("cls");
                        printf("                  ROUND %d", rounds);
                        // Para recomeçar o jogo
                        y = Y_PLAYER;
                        x = COMPRIMENTO / 2;
                        fillMap(x, y);
                        switch(dif){
                        	case 1: VIDAS = 3; break;
                        	case 2: VIDAS = 2; break;
                        	case 3: VIDAS = 1; break;
                        	default: VIDAS = 3; break;
						}
                        GAME_OVER = false;
                        WON_GAME = false;
                        Sleep(4000);
                    }
                }
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
			if (move == 27){
                int i = 1;
                while(true){
                    printf("\n\n\n                THE GAME IS PAUSED\n");
                    printf("             PRESS ESC AGAIN TO RESUME");
                    system("cls");
                    char pause_move;
                    if (kbhit()){
                        pause_move = getch();
                    }
                    if (pause_move == 27){
                        pause_move = 1;
                        break;
                    }
                }
			}

			// Quando carregamos no "UP", é lançado um missil
			if (move == 72){
				if (!(shots >= MAX_SHOTS)){
					map[x][Y_PLAYER-1] = 4;
				}
			}

			move = 0;
			//timer(10); // Para o loop;
			//Sleep(10);
			if(VIDAS == 1){
				printf("SCORE: %d   \t  RONDA: %d \t VIDAS:  <3", SCORE, rounds);
			}
			else if (VIDAS == 2){
				printf("SCORE: %d   \t  RONDA: %d \t VIDAS:  <3 <3", SCORE, rounds);
			}
			else if (VIDAS == 3){
				printf("SCORE: %d   \t  RONDA: %d \t VIDAS:  <3 <3 <3", SCORE, rounds);
			}
			//Beep(4000, 300); // Para fazer sons!
		}
	}
	startGame();
}

void difficulty(){
	system("cls");
	printf(" Escolha a dificuldade:\n");
	printf(" 1 --> Normal\n");
	printf(" 2 --> Difícil\n");
	printf(" 3 --> Impossível\n");
	int a;
	scanf("%d", &a);
	switch(a){
		case 1: dif = 1; break;
		case 2: dif = 2; break;
		case 3: dif = 3; break;
		default: dif = 1; break;
	}
	menu();
}

void credits(){
	system("cls");
	printf("Criadores do jogo:\n-->João Manuel Alves Pereira\n-->Mateus Medeiros Pereira\n-->Matheus Carlos Santos Costa");
	Sleep(10000);
	menu();
}

void easterEggs(){
    // Um poema aleatório
    // Tivemos demasiado tempo para fazer o jogo xD
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
    printf("...Well, guess this group deserves a 20...\n");
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
	setConsoleSize(30, 70);
	printf("\n Initializing");
	Sleep(450);
	printf(".");
	Sleep(350);
	printf(".");
	Sleep(350);
	printf(".\n\n\n");
	Sleep(350);
	printf(" 1 --> SPACE INVADERS");
	printf("\n 2 --> DIFFICULTY");
	printf("\n 3 --> CREDITS");
	printf("\n 4 --> EXIT\n");
	puts(""
	"	                       *     .--.\n"
	"	                            / /  `\n"
	"	           +               | |\n"
	"	                  '         \ \__,\n"
	"	              *          +   '--'  *\n"
	"	                  +   /\\\n"
	"	     +              .'  '.   *\n"
	"	            *      /======\\      +\n"
	"	                  ;:.  _   ;\n"
	"	                  |:. (_)  |\n"
	"	                  |:.  _   |\n"
	"	        +         |:. (_)  |          *\n"
	"	                  ;:.      ;\n"
	"	                .' \:.    / `.\n"
	"	               / .-'':._.'`-. \\\n"
	"	               |/    /||\\    \\|\n"
	"	             _..--\"\"\"````\"\"\"--..\n"
	"	       _.-'``                    ``'-._\n"
	"	     -'                                '-\n"
	""
	);

	scanf("%d", &choice);
	switch (choice){
		case 1: game(); system("cls"); break;
		case 2: difficulty(); system("cls"); break;
		case 3: credits(); system("cls"); break;
		case 4: system("cls"); exit(1); break;
		case 5: easterEggs(); system("cls"); break;
		default: missed = true; main(); break;
	}
}

int main(){
	//setlocale(LC_ALL, "Portuguese");
	menu();
	return 0;
}
