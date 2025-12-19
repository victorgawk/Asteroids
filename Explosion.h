class Point : public Shape
{
private:
	GLfloat X, Y;
public:
	Point();
	void draw();
	void atualizaDistancia();
	bool horaDeApagar();
};

Point::Point()
{
	X = 0.0f;
	Y = 0.0f;
}

void Point::draw()
{
	glBegin(GL_POINTS);
		glVertex2f(Xpos, Ypos);
	glEnd();
}

void Point::atualizaDistancia()
{
	X += Xdir;
	Y += Ydir;
}

bool Point::horaDeApagar()
{
	return(fabs(X) > 0.2f || fabs(Y) > 0.2f);
}

class Line : public Shape
{
private:
	GLfloat angle;
public:
	void setAngle(GLfloat aa);
	GLfloat getAngle();
	void draw();
};

void Line::setAngle(GLfloat aa)
{
	angle = aa;
}

GLfloat Line::getAngle()
{
	return angle;
}

void Line::draw()
{
	glBegin(GL_LINE_STRIP);
		glVertex2f(Xpos+cos(angle)*0.04f, Ypos+sin(angle)*0.04f);
		glVertex2f(Xpos+cos(angle+PI)*0.04f, Ypos+sin(angle+PI)*0.04f);
	glEnd();
}

