#include "Polygon.h"
#include "Bullet.h"

class Ship : public Polygon
{
public:
	// Auxiliar para a alternância de cor no desenho da turbina.
	bool mudaCorTurbina;
	// Gravidade.
	GLfloat gravity;

	// Construtor para a Nave.
	Ship();
	// Desenha uma nave no espaço.
	void draw();
	// Determina o movimento da nave, visto que a mesma é afetada pela gravidade.
	void move();
	// Liga o jato propulsor, adicionando velocidade à nave.
	void run();
	// Realiza um disparo phaser na direção da nave.
	void disparoPhaser(Bullet** phaser, int idx);
	// Desenha a turbina da nave.
	void desenhaTurbina();
};

Ship::Ship()
{
	R = 0.035f;
	angle = PI/2;
	gravity = 0.0003f;
}

void Ship::move()
{
	GLfloat tmp;

	setPos(Xpos + Xdir, Ypos + Ydir);
	
	tmp = Xdir;
	if(Xdir < 0)
		Xdir += gravity;
	else if(Xdir > 0)
		Xdir -= gravity;
	if(tmp*Xdir < 0)
		Xdir = 0;
		
	tmp = Ydir;
	if(Ydir < 0)
		Ydir += gravity;
	else if(Ydir > 0)
		Ydir -= gravity;
	if(tmp*Ydir < 0)
		Ydir = 0;
}

void Ship::draw()
{
	glBegin(GL_LINE_LOOP);
		glVertex2f(Xpos+cos(angle-2*PI/3)*0.05f, Ypos+sin(angle-2*PI/3)*0.05f);
		glVertex2f(Xpos+cos(angle)*0.08f, Ypos+sin(angle)*0.08f);
		glVertex2f(Xpos+cos(angle+2*PI/3)*0.05f, Ypos+sin(angle+2*PI/3)*0.05f);
	glEnd();
}

void Ship::run()
{
	if(fabs(Xdir) < 0.05f)
		Xdir += 0.0015f*cos(angle);
	if(fabs(Ydir) < 0.05f)
		Ydir += 0.0015f*sin(angle);
}

void Ship::desenhaTurbina()
{
	if(mudaCorTurbina) {
		glBegin(GL_LINE_STRIP);
			glVertex2f(Xpos+cos(angle-2*PI/3)*0.05f, Ypos+sin(angle-2*PI/3)*0.05f);
			glVertex2f(Xpos+cos(angle+PI)*0.08f, Ypos+sin(angle+PI)*0.08f);
			glVertex2f(Xpos+cos(angle+2*PI/3)*0.05f, Ypos+sin(angle+2*PI/3)*0.05f);
		glEnd();
		mudaCorTurbina = false;
	}
	else
		mudaCorTurbina = true;
}

void Ship::disparoPhaser(Bullet** phaser, int idx)
{
	// Alocando nova bala a ser disparada.
	phaser[idx] = new Bullet(ALIADO);
	// A bala recebe as coordenadas da frente da nave.
	phaser[idx]->setPos(Xpos+cos(angle)*0.08f, Ypos+sin(angle)*0.08f);
	// Definindo o vetor direção da bala em função do ângulo atual da nave.
	phaser[idx]->setDir(Xdir+0.04f*cos(angle), Ydir+0.04f*sin(angle));
}

