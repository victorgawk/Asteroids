#include "Circle.h"

class Star : public Circle
{
public:
	// Desenha a estrela no espaço.
	void draw();
};

// Desenha uma estrela branca no espaço.
void Star::draw()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS);
		glVertex2f(Xpos, Ypos);
	glEnd();
}

