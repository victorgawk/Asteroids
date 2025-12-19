#include "Shape.h"

#ifndef DISCOVOADOR_H
#define DISCOVOADOR_H

#define DISCO_G 2
#define DISCO_P 1

class DiscoVoador : public Shape
{
private:
	// Variável que determina qual o tipo do disco voador (grande ou pequeno).
	GLint tamanho;
	// Tempo de espera para mudar a direção.
	GLint direcaoDelay;
	// Tempo de espera para disparar.
	GLint disparoDelay;

public:
	DiscoVoador(GLint tam);
	void setPos(GLfloat x, GLfloat y);
	void draw();
	bool horaDeApagar();
	void move();
	bool horaDeAtirar();
	void disparoRandom(Bullet** phaser, int idx);
	void disparoEmAlvo(Bullet** phaser, int idx, GLfloat x, GLfloat y);
	bool horaDeMudarDir();
	void mudaDir();
};

bool DiscoVoador::horaDeMudarDir()
{
	
	return(direcaoDelay > 10*(rand() % 100 + 10));
}

void DiscoVoador::mudaDir()
{
	// Simplesmente negar o Y.
	setDir(Xdir, -Ydir);
	// Resetar o delay de mudar direção.
	direcaoDelay = 0;
}

DiscoVoador::DiscoVoador(GLint tam)
{
	setPos(0.0f, 0.0f);
	setDir(0.0f, 0.0f);
	R = 0.025f*tam;
	tamanho = tam;
	direcaoDelay = 0;
	disparoDelay = 0;
}

void DiscoVoador::setPos(GLfloat x, GLfloat y)
{
		Xpos = x;
		Ypos = y;
}

void DiscoVoador::move()
{
	setPos(Xpos + Xdir, Ypos + Ydir);
	disparoDelay++;
	direcaoDelay++;
}

void DiscoVoador::draw()
{
	glBegin(GL_LINE_STRIP);
		glVertex2f(Xpos+0.03f*tamanho, Ypos);
		glVertex2f(Xpos+0.02f*tamanho, Ypos+0.025f*tamanho);
		glVertex2f(Xpos-0.02f*tamanho, Ypos+0.025f*tamanho);
		glVertex2f(Xpos-0.03f*tamanho, Ypos);
		glVertex2f(Xpos+0.03f*tamanho, Ypos);
		glVertex2f(Xpos+0.02f*tamanho, Ypos-0.025f*tamanho);
		glVertex2f(Xpos-0.02f*tamanho, Ypos-0.025f*tamanho);
		glVertex2f(Xpos-0.03f*tamanho, Ypos);
	glEnd();
}

bool DiscoVoador::horaDeApagar()
{
	return(fabs(Xpos) > 1.0f);
}

bool DiscoVoador::horaDeAtirar()
{
	return(disparoDelay > 70);
}

void DiscoVoador::disparoRandom(Bullet** phaser, int idx)
{
	GLfloat dist = R+0.03f;
	// Gerando valor random.
	int r = 1 + 2*(rand() % 4);
	// Alocando nova bala a ser disparada.
	phaser[idx] = new Bullet(INIMIGO);
	// A bala recebe coordenadas random.
	phaser[idx]->setPos(Xpos+dist*cos(r*PI/4), Ypos+dist*sin(r*PI/4));
	// Definindo o vetor direção da bala.
	phaser[idx]->setDir(0.04f*cos(r*PI/4), 0.04f*sin(r*PI/4));
	// Resetando o delay para atirar.
	disparoDelay = 0;
}

void DiscoVoador::disparoEmAlvo(Bullet** phaser, int idx, GLfloat x, GLfloat y)
{
	GLfloat dist = R+0.03f;
	// Calculando vetor entre o disco voador e o alvo.
	GLfloat vx = x-Xpos;
	GLfloat vy = y-Ypos;
	// Calculando o módulo desse vetor.
	GLfloat hip = sqrt(pow(vx,2) + pow(vy,2));

	// Alocando nova bala a ser disparada.
	phaser[idx] = new Bullet(INIMIGO);
	// A bala recebe coordenadas iniciais.
	phaser[idx]->setPos(Xpos+dist*vx/hip, Ypos+dist*vy/hip);
	// Definindo o vetor direção da bala.
	phaser[idx]->setDir(0.04f*vx/hip, 0.04f*vy/hip);
	// Resetando o delay para atirar.
	disparoDelay = 0;
}

#endif

