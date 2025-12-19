#include "Polygon.h"

#define PEQUENO	0.1
#define MEDIO	0.25
#define GRANDE	0.5

class Asteroid : public Polygon
{
private:
	int asteroidType;
	GLfloat asteroidSize;
public:
	int getType();
	GLfloat getSize();
	
	// Construtor padrão.
	Asteroid();
	Asteroid(int tipo, GLfloat tamanho);
	void draw();
};

Asteroid::Asteroid()
{
	asteroidType = (rand() % 4 + 1);
	asteroidSize = GRANDE;
	R = 0.4*asteroidSize;
}

int Asteroid::getType()
{
	return asteroidType;
}

GLfloat Asteroid::getSize()
{
	return asteroidSize;
}

Asteroid::Asteroid(int tipo, GLfloat tamanho)
{
	asteroidType = tipo;
	asteroidSize = tamanho;
	R = 0.4*asteroidSize;

}

void Asteroid::draw()
{
	GLfloat tam = asteroidSize;

	switch(asteroidType) {
	case 1: // Asteróide tipo 1.
		tam *= 3.6;
		glBegin(GL_LINE_LOOP);
			glVertex2f(Xpos+tam*0.03f, Ypos+tam*0.05f);
			glVertex2f(Xpos+tam*0.10f, Ypos+tam*0.03f);
			glVertex2f(Xpos+tam*0.08f, Ypos-tam*0.08f);
			glVertex2f(Xpos+tam*0.02f, Ypos-tam*0.10f);
			glVertex2f(Xpos-tam*0.04f, Ypos-tam*0.10f);
			glVertex2f(Xpos-tam*0.10f, Ypos+tam*0.00f);
			glVertex2f(Xpos-tam*0.10f, Ypos+tam*0.05f);
			glVertex2f(Xpos-tam*0.04f, Ypos+tam*0.05f);
			glVertex2f(Xpos-tam*0.03f, Ypos+tam*0.11f);
			glVertex2f(Xpos+tam*0.02f, Ypos+tam*0.11f);
		glEnd();
		break;
	case 2: // Asteróide tipo 2.
		tam *= 1.05;
		glBegin(GL_LINE_LOOP);
			glVertex2f(Xpos+tam*0.08f, Ypos-tam*0.25f);
			glVertex2f(Xpos+tam*0.35f, Ypos-tam*0.20f);
			glVertex2f(Xpos+tam*0.30f, Ypos+tam*0.15f);
			glVertex2f(Xpos+tam*0.05f, Ypos+tam*0.15f);
			glVertex2f(Xpos+tam*0.15f, Ypos+tam*0.35f);
			glVertex2f(Xpos-tam*0.15f, Ypos+tam*0.35f);
			glVertex2f(Xpos-tam*0.20f, Ypos+tam*0.15f);
			glVertex2f(Xpos-tam*0.30f, Ypos+tam*0.15f);
			glVertex2f(Xpos-tam*0.35f, Ypos-tam*0.10f);
			glVertex2f(Xpos-tam*0.22f, Ypos-tam*0.15f);
			glVertex2f(Xpos-tam*0.22f, Ypos-tam*0.30f);
			glVertex2f(Xpos+tam*0.08f, Ypos-tam*0.35f);
		glEnd();
		break;
	case 3:
		glBegin(GL_LINE_LOOP);
			glVertex2f(Xpos+tam*0.00f, Ypos-tam*0.25f);
			glVertex2f(Xpos+tam*0.15f, Ypos-tam*0.35f);
			glVertex2f(Xpos+tam*0.35f, Ypos-tam*0.15f);
			glVertex2f(Xpos+tam*0.10f, Ypos+tam*0.00f);
			glVertex2f(Xpos+tam*0.30f, Ypos+tam*0.13f);
			glVertex2f(Xpos+tam*0.30f, Ypos+tam*0.20f);
			glVertex2f(Xpos+tam*0.00f, Ypos+tam*0.35f);
			glVertex2f(Xpos-tam*0.20f, Ypos+tam*0.35f);
			glVertex2f(Xpos-tam*0.10f, Ypos+tam*0.15f);
			glVertex2f(Xpos-tam*0.35f, Ypos+tam*0.15f);
			glVertex2f(Xpos-tam*0.35f, Ypos-tam*0.10f);
			glVertex2f(Xpos-tam*0.20f, Ypos-tam*0.37f);
		glEnd();
		break;
	case 4:
		tam *= 1.15;
		glBegin(GL_LINE_LOOP);
			glVertex2f(Xpos+tam*0.00f, Ypos+tam*0.00f);
			glVertex2f(Xpos-tam*0.05f, Ypos-tam*0.30f);
			glVertex2f(Xpos+tam*0.10f, Ypos-tam*0.30f);
			glVertex2f(Xpos+tam*0.35f, Ypos-tam*0.05f);
			glVertex2f(Xpos+tam*0.35f, Ypos+tam*0.05f);
			glVertex2f(Xpos+tam*0.10f, Ypos+tam*0.30f);
			glVertex2f(Xpos-tam*0.10f, Ypos+tam*0.30f);
			glVertex2f(Xpos-tam*0.35f, Ypos+tam*0.05f);
			glVertex2f(Xpos-tam*0.20f, Ypos+tam*0.00f);
			glVertex2f(Xpos-tam*0.35f, Ypos-tam*0.10f);
			glVertex2f(Xpos-tam*0.15f, Ypos-tam*0.30f);
		glEnd();
		break;
	}
}

