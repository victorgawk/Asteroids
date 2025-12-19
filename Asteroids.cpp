/*	JOGO ASTEROIDS
 *
 *	### FUNCIONAMENTO DAS TECLAS :
 *
 *	UP								:	aciona o jato propulsor.
 *	LEFT							:	gira a espaçonave para a esquerda.
 *	RIGHT							:	gira a espaçonave para a direita.
 *	'A', 'a' ou BARRA DE ESPAÇO 	:	dispara projéteis phaser.
 *	'H' ou 'h'						:	salta para o hiper-espaço.
 *	'P' ou 'p'						: 	pausa o jogo.
 *	'Q', 'q' ou ESC					: 	finaliza o jogo.
 *	'R' ou 'r'						: 	reseta o jogo.
 *
 *  ### COMO COMPILAR :
 *
 *  make Asteroids
 *
 */

#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "Star.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "DiscoVoador.h"

// Ponteiro para a nave.
Ship* espaconave;

// Matriz que representa o máximo de asteróides possíveis exibidos na tela.
// O valor 12 representa a quantidade máxima de asteróides grandes que podem ser exibidos simultaneamente.
// O valor 6 representa a quantidade máxima que um asteróide grande pode se dividir.
Asteroid* asteroides[12][6];

// Vetor que representa o máximo de disparos phasers exibidos simultaneamente na tela.
Bullet* phasers[20];

// Vetores para os efeitos de explosão dos asteróides.
Point* explosao[40];
Line* explosaoDaNave[3];

// Ponteiros para os discos voadores (grande e pequeno).
DiscoVoador* discoG;
DiscoVoador* discoP;

bool rotateLeft, rotateRight, jatoPropulsor, dispararPhaser, gameOver, pause, final;
int qtdGrandes, maxAsteroides, pontuacao, life, reviveDelay, discoGdelay, discoPdelay;


/***************************** INÍCIO DE DESENHA_2D *************************/
/*
	Trecho do código Moonlander, para desenhar uma string numa projeção 2D.
	Autor: 			Selan R. dos Santos
	Retirado de:	Jogo Moonlander
*/

#ifndef __FREEGLUT_EXT_H__
// Função para desenhar um texto na tela com fonte bitmap.
void glutBitmapString(void *fonte,char *texto)
{
	// Percorre todos os caracteres
	while (*texto)
    	glutBitmapCharacter(fonte, *texto++);
}
#endif

// Escreve uma string na tela, usando uma projeção ortográfica.
// normalizada (0..1, 0..1)
void Escreve2D(float x, float y, char *str)
{
	glMatrixMode(GL_PROJECTION);
	// Salva projeção perspectiva corrente
	glPushMatrix();
	glLoadIdentity();
	// E cria uma projeção ortográfica normalizada (0..1,0..1)
	gluOrtho2D(0,1,0,1);
	
	glMatrixMode(GL_MODELVIEW);
	// Salva modelview corrente
	glPushMatrix();
	glLoadIdentity();

	// Posiciona no canto inferior esquerdo da janela
	glRasterPos2f(x,y);
	glColor3f(1,1,1);
	// "Escreve" a mensagem
	glutBitmapString(GLUT_BITMAP_9_BY_15,str);
	
	glMatrixMode(GL_PROJECTION);
	// Restaura a matriz de projeção anterior
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	// ... e a modelview anterior
	glPopMatrix();
}

/***************************** FIM DE DESENHA_2D *************************/


// Função responsável por inicializar os asteróides.
void InicializaAsteroides()
{
	int i;

	// Variável que armazena a qtd. de asteróides que foram destruídos.
	maxAsteroides = 0;

	// Reseta o tempo de espera para o surgimento dos discos voadores.	
	discoGdelay = 0;
	discoPdelay = 0;

	// Iniciar o random de forma a gerar pontos diferentes cada vez.
	srand((int) time(NULL));

	// Inicializando os asteroides.
	for(i = 0; i < qtdGrandes; i++) {
		int rx = (rand() % 10 + 3);
		int ry = (rand() % 10 + 3);
		int rxx = pow(-1, (rand() % 2 + 1));
		int ryy = pow(-1, (rand() % 2 + 1));
		asteroides[i][0] = new Asteroid();
		if(espaconave != NULL)
			asteroides[i][0]->setPos(espaconave->getXpos()+0.1f*rx*rxx, espaconave->getYpos()+0.1f*ry*ryy);
		else
			asteroides[i][0]->setPos(0.1f*rx*rxx, 0.1f*ry*ryy);
		asteroides[i][0]->setDir(0.0005f*rx*rxx, 0.0005f*ry*ryy);
	}
}

// Função responsável por apagar e desreferenciar os asteróides.
void FinalizaAsteroides(void)
{
	int i, j;
	for(i=0;i<qtdGrandes; i++) {
		for(j=0;j<6;j++) {
			if(asteroides[i][j] != NULL) {
				delete asteroides[i][j];
				asteroides[i][j] = NULL;
			}
		}
	}
}

// Função responsável por inicializar parâmetros e variáveis.
void Inicializa(void) {	
	rotateLeft = false;
	rotateRight = false;
	jatoPropulsor = false;
	pause = false;
	dispararPhaser = false;
	gameOver = false;
	final = false;

	qtdGrandes = 4;
	pontuacao = 0;
	life = 4;
	reviveDelay = 0;

	// Inicializando a nave.
	espaconave = new Ship();
	espaconave->setPos(0,0);
	espaconave->setDir(0,0);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	InicializaAsteroides();
}

// Função responsável por apagar e desreferenciar todos os objetos, exceto os asteróides.
void Finaliza(void)
{
	int i;

	if(espaconave != NULL) {
		delete espaconave;
		espaconave = NULL;
	}

	for(i=0; i<20; i++) {
		if(phasers[i] != NULL) {
			delete phasers[i];
			phasers[i] = NULL;
		}
	}

	for(i=0; i<40; i++) {
		if(explosao[i] != NULL) {
			delete explosao[i];
			explosao[i] = NULL;
		}
	}

	for(i=0; i<3; i++) {
		if(explosaoDaNave[i] != NULL) {
			delete explosaoDaNave[i];
			explosaoDaNave[i] = NULL;
		}
	}
	
	if(discoG != NULL) {
		delete discoG;
		discoG = NULL;
		discoGdelay = 0;
	}
	
	if(discoP != NULL) {
		delete discoP;
		discoP = NULL;
		discoPdelay = 0;
	}
}

// Função callback de redesenho da janela de visualização.
void Desenha(void)
{
	// Auxiliares de iteração.
	int i, j;

	// Limpa a janela de visualização com a cor de fundo predefinida.
	glClear(GL_COLOR_BUFFER_BIT);

	// Definindo cor: BRANCO.
	glColor3f(1, 1, 1);

	// Espaçonave.
	if(espaconave != NULL) {
		espaconave->draw(); // Desenhando a espaçonave.
		if(jatoPropulsor && pause == false)
				espaconave->desenhaTurbina(); // Desenhando a turbina.
	}

	// Asteróides.
	for(i=0; i<qtdGrandes; i++) {
		for(j=0; j<6; j++) {
			if(asteroides[i][j] != NULL)
				asteroides[i][j]->draw();
		}
	}

	// Disparos phasers.
	for(i=0; i<20; i++) {
		if(phasers[i] != NULL) {
			phasers[i]->draw();
		}
	}

	// Explosão dos asteróides.
	for(i=0; i<40; i++) {
		if(explosao[i] != NULL)
			explosao[i]->draw();
	}
	
	// Explosão da espaçonave.
	for(i=0; i<3; i++) {
		if(explosaoDaNave[i] != NULL)
			explosaoDaNave[i]->draw();
	}
	
	// Disco voador grande.
	if(discoG != NULL)
		discoG->draw();

	// Disco voador pequeno.
	if(discoP != NULL)
		discoP->draw();

	char strMsg[20];
	sprintf_s(strMsg, "FASE  : %d", qtdGrandes-3);
	Escreve2D(0.03, 0.95, strMsg);
	// Desenhar pontuação no canto superior esquerdo da tela.
	sprintf_s(strMsg, "SCORE : %d", pontuacao);
	Escreve2D(0.03, 0.92, strMsg);
	// Desenhar número de vidas abaixo da pontuação.
	sprintf_s(strMsg, "VIDAS : %d", life);
	Escreve2D(0.03, 0.89, strMsg);

	glColor3f(1.0f, 1.0f, 0.0f);
	if(pause == true)
		Escreve2D(0.46, 0.5, (char*) "PAUSE");

	glColor3f(1.0f, 1.0f, 0.0f);
	if(gameOver == true)
		Escreve2D(0.42, 0.47, (char*) "GAME OVER");

	glColor3f(1.0f, 1.0f, 0.0f);
	if(final == true)
		Escreve2D(0.02, 0.02, (char*) "PARABENS VOCE ZEROU!!! =D");

	// Executa os comandos OpenGL.
	glutSwapBuffers();
}

// Função de callback responsável pela animação.
void Anima(int value)
{
	int i, j, k;

	if(pause == false) {
		// Testar colisões entre a espaçonave e os asteroides.
		for(i=0; i<qtdGrandes; i++) {
			for(j=0; j<6; j++) {
				if(asteroides[i][j] && espaconave && detectaColisao(asteroides[i][j], espaconave)) {
					// Alocando a explosão da Nave.
					for(k=0; k<3; k++) {
						int rx = rand() % 14 + -7;
						int ry = rand() % 14 + -7;
						if(rx == 0) rx = 1;
						if(ry == 0) ry = 1;
						explosaoDaNave[k] = new Line();
						explosaoDaNave[k]->setPos(espaconave->getXpos(), espaconave->getYpos());
						explosaoDaNave[k]->setDir(0.001f*rx, 0.001f*ry);
					}
					explosaoDaNave[0]->setAngle(0);
					explosaoDaNave[1]->setAngle(PI/3);
					explosaoDaNave[2]->setAngle(2*PI/3);
					
					life--;
					maxAsteroides++;
					
					// Apagando e desreferenciando a espaçonave.
					delete espaconave;
					espaconave = NULL;

					// Alocando novos asteróides, se necessário.
					Asteroid* tmp = asteroides[i][j];
					asteroides[i][j] = NULL;
					if(tmp->getSize() == GRANDE) {
						asteroides[i][0] = new Asteroid(tmp->getType(), MEDIO);
						asteroides[i][0]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][0]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());
					
						asteroides[i][3] = new Asteroid(tmp->getType(), MEDIO);
						asteroides[i][3]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][3]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());
					}
					else if(tmp->getSize() == MEDIO) {
						asteroides[i][j] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());
					
						asteroides[i][j+1] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j+1]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j+1]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());
					
						asteroides[i][j+2] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j+2]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j+2]->setDir(1.2*tmp->getXdir(), 1.2*tmp->getYdir());
					}
					delete tmp;
					tmp = NULL;
				}
				if(espaconave == NULL)
					break;
			}
			if(espaconave == NULL)
				break;
		}

		// Detecção de colisão entre asteróides e phasers.
		for(i=0; i<qtdGrandes; i++) {
			for(j=0; j<6; j++) {
				for(k=0; k<20; k++) {
					if(asteroides[i][j] && phasers[k] && detectaColisao(asteroides[i][j], phasers[k])) {
						maxAsteroides++;

						// Alocando novos asteróides, se necessário.
						Asteroid* tmp = asteroides[i][j];
						asteroides[i][j] = NULL;
						if(tmp->getSize() == GRANDE) {
							asteroides[i][0] = new Asteroid(tmp->getType(), MEDIO);
							asteroides[i][0]->setPos(tmp->getXpos(), tmp->getYpos());
							asteroides[i][0]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());

							asteroides[i][3] = new Asteroid(tmp->getType(), MEDIO);
							asteroides[i][3]->setPos(tmp->getXpos(), tmp->getYpos());
							asteroides[i][3]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());
							
							if(phasers[k]->getDono() == ALIADO)
								pontuacao += 2;
						}
						else if(tmp->getSize() == MEDIO) {
							asteroides[i][j] = new Asteroid(tmp->getType(), PEQUENO);
							asteroides[i][j]->setPos(tmp->getXpos(), tmp->getYpos());
							asteroides[i][j]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());

							asteroides[i][j+1] = new Asteroid(tmp->getType(), PEQUENO);
							asteroides[i][j+1]->setPos(tmp->getXpos(), tmp->getYpos());
							asteroides[i][j+1]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());

							asteroides[i][j+2] = new Asteroid(tmp->getType(), PEQUENO);
							asteroides[i][j+2]->setPos(tmp->getXpos(), tmp->getYpos());
							asteroides[i][j+2]->setDir(1.2*tmp->getXdir(), 1.2*tmp->getYdir());

							if(phasers[k]->getDono() == ALIADO)
								pontuacao += 5;
						}
						else {
							if(phasers[k]->getDono() == ALIADO)
								pontuacao += 10;
						}
						
						// Apagando e desreferenciando o phaser.
						delete phasers[k];
						phasers[k] = NULL;

						// Alocando as variáveis para explosão do asteróide.
						int c = 0;
						for(k=0; k<40 && c<4; k++) {
							if(explosao[k] == NULL) {
								int rx = (rand() % 7 + 4);
								int ry = (rand() % 7 + 4);
								int rxx = pow(-1, (rand() % 2 + 1));
								int ryy = pow(-1, (rand() % 2 + 1));
								explosao[k] = new Point();
								explosao[k]->setPos(tmp->getXpos(), tmp->getYpos());
								explosao[k]->setDir(0.001f*rx*rxx, 0.001f*ry*ryy);
								++c;
							}
						}

						delete tmp;
					}
				}
			}
		}
		
		// Detecção de colisão entre asteróides e discoG.
		for(i=0; i<qtdGrandes && discoG; i++) {
			for(j=0; j<6 && discoG; j++) {
				if(asteroides[i][j] && detectaColisao(asteroides[i][j], discoG)) {
					delete discoG;
					discoG = NULL;
					discoGdelay = 0;
					
					maxAsteroides++;

					// Alocando novos asteróides, se necessário.
					Asteroid* tmp = asteroides[i][j];
					asteroides[i][j] = NULL;
					if(tmp->getSize() == GRANDE) {
						asteroides[i][0] = new Asteroid(tmp->getType(), MEDIO);
						asteroides[i][0]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][0]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());

						asteroides[i][3] = new Asteroid(tmp->getType(), MEDIO);
						asteroides[i][3]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][3]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());
					}
					else if(tmp->getSize() == MEDIO) {
						asteroides[i][j] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());

						asteroides[i][j+1] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j+1]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j+1]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());

						asteroides[i][j+2] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j+2]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j+2]->setDir(1.2*tmp->getXdir(), 1.2*tmp->getYdir());
					}

					// Alocando as variáveis para explosão do asteróide.
					int c = 0;
					for(k=0; k<40 && c<4; k++) {
						if(explosao[k] == NULL) {
							int rx = (rand() % 7 + 4);
							int ry = (rand() % 7 + 4);
							int rxx = pow(-1, (rand() % 2 + 1));
							int ryy = pow(-1, (rand() % 2 + 1));
							explosao[k] = new Point();
							explosao[k]->setPos(tmp->getXpos(), tmp->getYpos());
							explosao[k]->setDir(0.001f*rx*rxx, 0.001f*ry*ryy);
							++c;
						}
					}

					delete tmp;
				}
			}
		}
		
		// Detecção de colisão entre asteróides e discoP.
		for(i=0; i<qtdGrandes && discoP; i++) {
			for(j=0; j<6 && discoP; j++) {
				if(asteroides[i][j] && detectaColisao(asteroides[i][j], discoP)) {
					delete discoP;
					discoP = NULL;
					discoPdelay = 0;
					
					maxAsteroides++;

					// Alocando novos asteróides, se necessário.
					Asteroid* tmp = asteroides[i][j];
					asteroides[i][j] = NULL;
					if(tmp->getSize() == GRANDE) {
						asteroides[i][0] = new Asteroid(tmp->getType(), MEDIO);
						asteroides[i][0]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][0]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());

						asteroides[i][3] = new Asteroid(tmp->getType(), MEDIO);
						asteroides[i][3]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][3]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());
					}
					else if(tmp->getSize() == MEDIO) {
						asteroides[i][j] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j]->setDir(-1.2*tmp->getXdir(), 1.2*tmp->getYdir());

						asteroides[i][j+1] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j+1]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j+1]->setDir(1.2*tmp->getXdir(), -1.2*tmp->getYdir());

						asteroides[i][j+2] = new Asteroid(tmp->getType(), PEQUENO);
						asteroides[i][j+2]->setPos(tmp->getXpos(), tmp->getYpos());
						asteroides[i][j+2]->setDir(1.2*tmp->getXdir(), 1.2*tmp->getYdir());
					}

					// Alocando as variáveis para explosão do asteróide.
					int c = 0;
					for(k=0; k<40 && c<4; k++) {
						if(explosao[k] == NULL) {
							int rx = (rand() % 7 + 4);
							int ry = (rand() % 7 + 4);
							int rxx = pow(-1, (rand() % 2 + 1));
							int ryy = pow(-1, (rand() % 2 + 1));
							explosao[k] = new Point();
							explosao[k]->setPos(tmp->getXpos(), tmp->getYpos());
							explosao[k]->setDir(0.001f*rx*rxx, 0.001f*ry*ryy);
							++c;
						}
					}

					delete tmp;
				}
			}
		}

		// Detecta colisão entre balas e nave.
		for(i=0; i<20 && espaconave != NULL; i++) {
			if(phasers[i] && detectaColisao(phasers[i], espaconave)) {
				delete phasers[i];
				phasers[i] = NULL;

				// Alocando a explosão da Nave.
				for(k=0; k<3; k++) {
					int rx = rand() % 14 + -7;
					int ry = rand() % 14 + -7;
					if(rx == 0) rx = 1;
					if(ry == 0) ry = 1;
					explosaoDaNave[k] = new Line();
					explosaoDaNave[k]->setPos(espaconave->getXpos(), espaconave->getYpos());
					explosaoDaNave[k]->setDir(0.001f*rx, 0.001f*ry);
				}
				explosaoDaNave[0]->setAngle(0);
				explosaoDaNave[1]->setAngle(PI/3);
				explosaoDaNave[2]->setAngle(2*PI/3);

				life--;

				delete espaconave;
				espaconave = NULL;
			}
		}
		
		// DiscoG X espaçonave.
		if(discoG && espaconave && detectaColisao(discoG, espaconave)) {
			delete discoG;
			discoG = NULL;
			discoGdelay = 0;

			// Alocando a explosão da Nave.
			for(k=0; k<3; k++) {
				int rx = rand() % 14 + -7;
				int ry = rand() % 14 + -7;
				if(rx == 0) rx = 1;
				if(ry == 0) ry = 1;
				explosaoDaNave[k] = new Line();
				explosaoDaNave[k]->setPos(espaconave->getXpos(), espaconave->getYpos());
				explosaoDaNave[k]->setDir(0.001f*rx, 0.001f*ry);
			}
			explosaoDaNave[0]->setAngle(0);
			explosaoDaNave[1]->setAngle(PI/3);
			explosaoDaNave[2]->setAngle(2*PI/3);

			life--;

			delete espaconave;
			espaconave = NULL;
		}
		
		// DiscoP X espaçonave.
		if(discoP && espaconave && detectaColisao(discoP, espaconave)) {
			delete discoP;
			discoP = NULL;
			discoPdelay = 0;

			// Alocando a explosão da Nave.
			for(k=0; k<3; k++) {
				int rx = rand() % 14 + -7;
				int ry = rand() % 14 + -7;
				if(rx == 0) rx = 1;
				if(ry == 0) ry = 1;
				explosaoDaNave[k] = new Line();
				explosaoDaNave[k]->setPos(espaconave->getXpos(), espaconave->getYpos());
				explosaoDaNave[k]->setDir(0.001f*rx, 0.001f*ry);
			}
			explosaoDaNave[0]->setAngle(0);
			explosaoDaNave[1]->setAngle(PI/3);
			explosaoDaNave[2]->setAngle(2*PI/3);

			life--;

			delete espaconave;
			espaconave = NULL;
		}
		
		// Detecta colisão entre balas e disco voador grande.
		for(i=0; i<20 && discoG != NULL; i++) {
			if(phasers[i] && detectaColisao(phasers[i], discoG)) {
				if(phasers[i]->getDono() == ALIADO)
					pontuacao += 15;
				
				// Alocando as variáveis para o efeito de explosão do disco voador.
				int c = 0;
				for(k=0; k<40 && c<3; k++) {
					if(explosao[k] == NULL) {
						int rx = (rand() % 7 + 4);
						int ry = (rand() % 7 + 4);
						int rxx = pow(-1, (rand() % 2 + 1));
						int ryy = pow(-1, (rand() % 2 + 1));
						explosao[k] = new Point();
						explosao[k]->setPos(discoG->getXpos(), discoG->getYpos());
						explosao[k]->setDir(0.001f*rx*rxx, 0.001f*ry*ryy);
						++c;
					}
				}
				
				delete phasers[i];
				phasers[i] = NULL;				
				discoGdelay = 0;

				delete discoG;
				discoG = NULL;
			}
		}
		
		// Detecta colisão entre balas e disco voador pequeno.
		for(i=0; i<20 && discoP != NULL; i++) {
			if(phasers[i] && detectaColisao(phasers[i], discoP)) {
				if(phasers[i]->getDono() == ALIADO)
					pontuacao += 30;
				
				// Alocando as variáveis para o efeito de explosão do disco voador.
				int c = 0;
				for(k=0; k<40 && c<2; k++) {
					if(explosao[k] == NULL) {
						int rx = (rand() % 7 + 4);
						int ry = (rand() % 7 + 4);
						int rxx = pow(-1, (rand() % 2 + 1));
						int ryy = pow(-1, (rand() % 2 + 1));
						explosao[k] = new Point();
						explosao[k]->setPos(discoP->getXpos(), discoP->getYpos());
						explosao[k]->setDir(0.001f*rx*rxx, 0.001f*ry*ryy);
						++c;
					}
				}

				delete phasers[i];
				phasers[i] = NULL;
				discoPdelay = 0;

				delete discoP;
				discoP = NULL;
			}
		}

		// Movimentando a espaçonave.
		if(espaconave != NULL) {
			if(dispararPhaser == true) {
				for(i=0; i<20 && dispararPhaser; i++) {
					if(phasers[i] == NULL) {
						espaconave->disparoPhaser(phasers, i);
						dispararPhaser = false;
					}
				}
			}
			espaconave->move();
			if(rotateLeft) espaconave->rotate(PI/20);
			if(rotateRight) espaconave->rotate(-PI/20);
			if(jatoPropulsor) espaconave->run();
		}
		else {
			if(reviveDelay < 50)
				++reviveDelay;
			else {
				for(i=0; i<3; i++) {
					if(explosaoDaNave[i] != NULL) {
						delete explosaoDaNave[i];
						explosaoDaNave[i] = NULL;
					}
				}

				if(life > 0) {
					// Revivendo a nave.
					espaconave = new Ship();
					espaconave->setPos(0,0);
					espaconave->setDir(0,0);

					// Resetando o delay.
					reviveDelay = 0;
				}
				else {
					gameOver = true;
				}
			}
		}

		// Movimentando os disparos phasers.
		for(i=0; i<20; i++) {
			if(phasers[i] != NULL) {
				phasers[i]->move();
				phasers[i]->atualizaDistancia();

				if(phasers[i]->horaDeApagar()) {
					delete phasers[i];
					phasers[i] = NULL;
				}
			}
		}

		// Movimenta os asteróides.
		for(i=0; i<qtdGrandes; i++) {
			for(j=0; j<6; j++) {
				if(asteroides[i][j] != NULL)
					asteroides[i][j]->move();
			}
		}

		for(i=0; i<40; i++) {
			if(explosao[i] != NULL) {
				explosao[i]->move();
				explosao[i]->atualizaDistancia();
			
				if(explosao[i]->horaDeApagar()) {
					delete explosao[i];
					explosao[i] = NULL;
				}
			}
		}
		
		for(i=0; i<3; i++) {
			if(explosaoDaNave[i] != NULL) {
				explosaoDaNave[i]->move();
			}
		}
		
		// Animando o disco voador grande.
		if(discoG != NULL) {
			discoG->move();
			if(discoG->horaDeAtirar()) {
				for(i=0; i<20 && discoG->horaDeAtirar(); i++) {
					if(phasers[i] == NULL)
						discoG->disparoRandom(phasers, i);
				}
			}
			if(discoG->horaDeApagar()) {
				delete discoG;
				discoG = NULL;
				discoGdelay = 0;
			}
		}
		else {
			if(discoGdelay < 300)
				discoGdelay++;
			else {
				int ry = (rand() % 9 + 1);
				int rxx = pow(-1, (rand() % 2 + 1));
				int ryy = pow(-1, (rand() % 2 + 1));
				discoG = new DiscoVoador(DISCO_G);
				discoG->setPos(rxx, 0.1f*ry*ryy);
				discoG->setDir(-rxx*0.01, 0);
			}
		}
		
		// Animando o disco voador pequeno.
		if(discoP != NULL) {
			discoP->move();
			if(discoP->horaDeMudarDir()) {
				discoP->mudaDir();
			}
			if(discoP->horaDeAtirar()) {
				for(i=0; i<20 && discoP->horaDeAtirar(); i++) {
					if(phasers[i] == NULL) {
						if(espaconave != NULL)
							discoP->disparoEmAlvo(phasers, i, espaconave->getXpos(), espaconave->getYpos());
						else
							discoP->disparoRandom(phasers, i);
					}
				}
			}
			if(discoP->horaDeApagar()) {
				delete discoP;
				discoP = NULL;
				discoPdelay = 0;
			}
		}
		else {
			if(discoPdelay < 500)
				discoPdelay++;
			else {
				int ry = (rand() % 9 + 1);
				int rxx = pow(-1, (rand() % 2 + 1));
				int ryy = pow(-1, (rand() % 2 + 1));
				discoP = new DiscoVoador(DISCO_P);
				discoP->setPos(rxx, 0.1f*ry*ryy);
				discoP->setDir(-rxx*0.005, -ryy*0.007);
			}
		}
	}

	if(maxAsteroides == 9*qtdGrandes) {
		if(qtdGrandes < 12) {
			qtdGrandes++;
			FinalizaAsteroides();
			if(discoG != NULL) {
				delete discoG;
				discoG = NULL;
				discoGdelay = 0;
			}
			if(discoP != NULL) {
				delete discoP;
				discoP = NULL;
				discoPdelay = 0;
			}
			InicializaAsteroides();
		}
		else {
			final = true;
		}
	}

	glutPostRedisplay();

	glutTimerFunc(33, Anima, 1);
}


// Função callback chamada para gerenciar eventos de teclas especiais(F1,PgDn,...).
void TeclasEspeciais(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT: // Rotaciona a espaçonave em 12 graus p/ esquerda.
			rotateLeft = true;
			break;
		case GLUT_KEY_RIGHT: // Rotaciona a espaçonave em 12 graus p/ direita.
			rotateRight = true;
			break;
		case GLUT_KEY_UP: // Aciona os jatos propulsores.
			jatoPropulsor = true;
			break;
	}
}

void TeclasEspeciaisUp(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
			rotateLeft = false;
			break;
		case GLUT_KEY_RIGHT:
			rotateRight = false;
			break;
		case GLUT_KEY_UP:
			jatoPropulsor = false;
			break;
	}
}

// Função callback chamada para gerenciar eventos de teclas.
void Teclado(unsigned char key, int x, int y)
{
	switch(key) {
		case 27: case 'q': case 'Q':
			Finaliza();
			FinalizaAsteroides();
			exit(0);
			break;
		case 32: case 'a': case 'A':
			if(espaconave != NULL)
				dispararPhaser = true;
			break;
		case 'p': case 'P':
			if(pause == false)
				pause = true;
			else
				pause = false;
			break;
		case 'r': case 'R':
			Finaliza();
			FinalizaAsteroides();
			Inicializa();
			break;
		case 'h': case 'H':
			if(espaconave != NULL) {
				GLint rx = rand() % 10;
				GLint ry = rand() % 10;
				GLint rxx = pow(-1, (rand() % 2 + 1));
				GLint ryy = pow(-1, (rand() % 2 + 1));
				espaconave->setPos(rx*rxx*0.1f, ry*ryy*0.1f);
				espaconave->setDir(0, 0);
			}
			break;
	}
}

int main(int argc, char* argv[])
{
	// Inicia o GLUT.
	glutInit(&argc, argv);
	// Define do modo de operação da GLUT.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// Especifica a posição inicial da janela GLUT.
	glutInitWindowPosition(5,5);
	// Especifica o tamanho inicial em pixels da janela GLUT.
	glutInitWindowSize(500,500);
	// Cria a janela passando como argumento o título da mesma.
	glutCreateWindow("Asteroids");

	// Registra a função callback para redesenho da janela de visualização.
	glutDisplayFunc(Desenha);
	// Registra a função callback para tratamento das teclas especiais.
	glutSpecialFunc(TeclasEspeciais);
	glutSpecialUpFunc(TeclasEspeciaisUp);
	glutIgnoreKeyRepeat(true);
	// Registra a função callback para tratamento das teclas ASCII.
	glutKeyboardFunc(Teclado);
	// Registra a função callback que será chamada a cada intervalo de tempo.
	glutTimerFunc(33, Anima, 1);

	// Chama a função responsável por fazer as inicializações.
	Inicializa();
	// Inicia o processamento e aguarda interações do usuário.
	glutMainLoop();
	// Apaga toda a memória que tenha sido alocada dinamicamente.
	Finaliza();
	FinalizaAsteroides();

	return 0;
}

