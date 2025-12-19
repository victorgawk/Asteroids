#include "Shape.h"
#include "Circle.h"

#ifndef POLYGON_H
#define POLYGON_H

class Polygon : public Shape
{
public:
	// O ângulo (em radianos) que o polígono faz com o eixo X.
	GLfloat angle;

	// Rotaciona o polígono em função do ângulo.
	void rotate(GLfloat a);
};

void Polygon::rotate(GLfloat a)
{
	angle += a;
}

#endif

