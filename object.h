#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <iostream>
#include "math.h"
#include <gl/glut.h>

using namespace std;

class Object{
public:
	Object();
	Object(GLfloat x, GLfloat y, GLfloat size,
		   GLfloat xVel, GLfloat yVel);
	~Object();
	
	GLfloat x, y;
	GLfloat size, sizeX, sizeY;
	GLfloat xVel, yVel;
	GLboolean transformOnce, enlarge;
	
	void drawPoint(GLfloat x, GLfloat y, GLfloat size);
	void drawLine(GLfloat x1, GLfloat y1,
				  GLfloat x2, GLfloat y2,
				  GLfloat thickness);
	void drawTriangle(GLfloat x1, GLfloat y1,
					  GLfloat x2, GLfloat y2,
					  GLfloat x3, GLfloat y3);
	void drawQuad(GLfloat x1, GLfloat y1,
				  GLfloat x2, GLfloat y2,
				  GLfloat x3, GLfloat y3,
				  GLfloat x4, GLfloat y4);
	void drawRect(GLfloat x, GLfloat y, GLfloat width, GLfloat height);
	void drawRegularPolygon(GLfloat x, GLfloat y,
							GLfloat radius,
							GLint side,
							GLfloat orientation,
							GLfloat width,
							GLfloat height);
	
	// Transformation functions
	void translate(GLfloat tX, GLfloat tY);
	void scale(GLfloat sX, GLfloat sY);
	void rotate(GLfloat degree,
				GLfloat xAxis, GLfloat yAxis, GLfloat zAxis,
				GLfloat pivotX, GLfloat pivotY, GLfloat pivotZ);
	void drawText(const char *text, GLint length, 
				  GLint x, GLint y, void *font);
	void pRotate(GLfloat centerX, GLfloat centerY,
				 GLfloat radius, GLfloat& angle, 
				 GLfloat stepSize);
				 
	void drawGradient(GLfloat x, GLfloat y, 
				  	  GLfloat width, GLfloat height,
				      GLint direction, GLclampf r, GLclampf g, GLclampf b,
				      char a);
	void drawCurveLine(GLfloat x1, GLfloat y1, 
				   		GLfloat x2, GLfloat y2,
				   		GLfloat x3, GLfloat y3,
				   		GLfloat x4, GLfloat y4);
	void lighting(GLfloat x, GLfloat y, GLfloat brightness);
	
};

#endif
