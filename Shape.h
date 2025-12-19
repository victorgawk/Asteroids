#include <math.h>
#include <GL/glut.h>

#ifndef SHAPE_H
#define SHAPE_H

#define PI 3.1415926

// Entidade que representa todo objeto que se move.
class Shape
{
public:
	// Representa o raio do circulo.
	GLfloat R;
	// Posição atual.
	GLfloat Xpos, Ypos;
	// Direção atual.
	GLfloat Xdir, Ydir;
	
	GLfloat getXpos();
	GLfloat getYpos();
	GLfloat getXdir();
	GLfloat getYdir();
	
	// Modifica o raio atual.
	void setR(GLfloat raio);

	// Altera a posição.
	void setPos(GLfloat x, GLfloat y);
	// Muda os valores da direção.
	void setDir(GLfloat x, GLfloat y);
	// Modifica a posição atual do objeto em função de sua direção.
	void move();
};

GLfloat Shape::getXpos()
{
	return Xpos;
}

GLfloat Shape::getYpos()
{
	return Ypos;
}

GLfloat Shape::getXdir()
{
	return Xdir;
}

GLfloat Shape::getYdir()
{
	return Ydir;
}

void Shape::setR(GLfloat raio)
{
	R = raio;
}

void Shape::setPos(GLfloat x, GLfloat y)
{
	GLfloat t = 1;

	if(fabs(x) > t) {
		if(x < 0)
			Xpos = t;
		else
			Xpos = -t;
	}
	else
		Xpos = x;

	if(fabs(y) > t) {
		if(y < 0)
			Ypos = t;
		else
			Ypos = -t;
	}
	else
		Ypos = y;
}

void Shape::setDir(GLfloat x, GLfloat y)
{
	Xdir = x;
	Ydir = y;
}

void Shape::move()
{
	setPos(Xpos + Xdir, Ypos + Ydir);
}

// Determina se dois círculos interceptam entre si.
bool detectaColisao(Shape* a, Shape* b)
{
	// Calculando a distancia entre a e b.
	GLfloat distAB = sqrt(pow(a->Xpos - b->Xpos, 2) + pow(a->Ypos - b->Ypos, 2));	
	// Se a soma dos raios é maior que a distancia entre seus centros, os circulos se colidem.
	return(distAB < (a->R + b->R));
}

#endif

