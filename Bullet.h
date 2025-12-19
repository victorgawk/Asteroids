#include <stdio.h>
#include "Circle.h"

#ifndef ALIADO
#define ALIADO 1
#endif

#ifndef INIMIGO
#define INIMIGO 0
#endif

class Bullet : public Circle
{
private:
	// Contador para o tempo de vida da bala.
	int lifeTime;
	// Variável booleana que indica a quem pertence o disparo (aliado/inimigo).
	bool dono;
public:
	Bullet(bool b);
	// Desenha uma bala no espaço.
	void draw();
	// Atualiza a distância que a bala já percorreu.
	void atualizaDistancia();
	// Determina se a bala deve ser apagada.
	bool horaDeApagar();
	bool getDono();
};

Bullet::Bullet(bool b)
{
	dono = b;
	R = 0.004f;
	lifeTime = 0;
}

bool Bullet::getDono()
{
	return dono;
}

void Bullet::draw()
{
	glPushMatrix();
	glTranslatef(Xpos, Ypos, 0);
	glutSolidSphere(R, 15, 15);
	glPopMatrix();
}

void Bullet::atualizaDistancia()
{
	lifeTime++;
}

bool Bullet::horaDeApagar()
{
	return(lifeTime > 30);
}

