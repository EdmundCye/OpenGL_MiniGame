#include "object.h"

// Default constructor
Object::Object(){
	this->x = 0;
	this->y = 0;
	size = 1;
	transformOnce = true;
}
Object::Object(GLfloat x, GLfloat y, GLfloat size,
		   	   GLfloat xVel, GLfloat yVel){
	this->x = x; this->y = y; //Object position
	this->xVel = xVel; this->yVel = yVel; //Object speed
	this->sizeX = size; this->sizeY = size;
	transformOnce = false;
	enlarge = false;			 	
}
// Destructor
Object::~Object(){}

void Object::drawPoint(GLfloat x, GLfloat y, GLfloat size){
	glPointSize(size); // Diameter of the dot.
	glBegin(GL_POINTS);
		glVertex2i(x,y);
	glEnd();
}

void Object::drawLine(GLfloat x1, GLfloat y1,
					  GLfloat x2, GLfloat y2,
					  GLfloat thickness){
	glPushMatrix();
	glLineWidth(thickness);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
	glPopMatrix();
}

void Object::drawTriangle(GLfloat x1, GLfloat y1,
						  GLfloat x2, GLfloat y2,
						  GLfloat x3, GLfloat y3){
	glPushMatrix();
	glBegin(GL_TRIANGLES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	glEnd();
	glPopMatrix();
}

void Object::drawQuad(GLfloat x1, GLfloat y1,
					  GLfloat x2, GLfloat y2,
					  GLfloat x3, GLfloat y3,
					  GLfloat x4, GLfloat y4){
	glPushMatrix();
	glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
		glVertex2i(x4, y4);
	glEnd();
	glPopMatrix();
}

void Object::drawRect(GLfloat x, GLfloat y, GLfloat width, GLfloat height){

	glColor3f(1.0f, 1.0f, 0.0f);
	glRectf(x,y,width,height);

}

void Object::drawRegularPolygon(GLfloat x, GLfloat y,
						GLfloat radius,
						GLint side,
						GLfloat orientation,
						GLfloat width,
						GLfloat height){
	if(side >= 3){
		// Render polygon
		GLfloat xp, yp; // Interpolation points.
		glBegin(GL_POLYGON);
			for(int i=0; i<side; i++){
				xp = x + width*radius*cos(orientation + 2*3.14159265/side*i);
				yp = y + height*radius*sin(orientation + 2*3.14159265/side*i);
				glVertex2i(xp, yp);
			}
		glEnd();
	}
	else{
		cerr<<"Could not render polygon with less than three sides."<<endl;
	}
}

void Object::translate(GLfloat tX, GLfloat tY){
	this->x = this->x + tX;
	this->y = this->y + tY;
}

void Object::scale(GLfloat sX, GLfloat sY){
	this->sizeX = this->sizeX * sX;
	this->sizeY = this->sizeY * sY;
}

void Object::rotate(GLfloat degree,
					GLfloat xAxis, GLfloat yAxis, GLfloat zAxis,
					GLfloat pivotX, GLfloat pivotY, GLfloat pivotZ){
	glTranslatef(pivotX, pivotY, pivotZ);
	glRotatef(degree, xAxis, yAxis, zAxis);
	glTranslatef(-pivotX, -pivotY, -pivotZ);
}

void Object::drawText(const char *text, GLint length, 
			  		  GLint x, GLint y, void *font){
	glMatrixMode(GL_PROJECTION); //for object that does not obey coordinate system
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);//-5 is inside the screen, and 5 is outside the screen
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
		glLoadIdentity();
		glRasterPos2i(x,y);
		for(int i=0; i<length; i++){
			glutBitmapCharacter(font, (int)text[i]);
		}
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void Object::pRotate(GLfloat centerX, GLfloat centerY,
		     GLfloat radius, GLfloat& angle, 
		     GLfloat stepSize){
	this->x = centerX + radius*cos(angle*3.14159265/180);	
	this->y = centerY + radius*sin(angle*3.14159265/180);
	if(angle < 360){
		angle += stepSize; // Rotational movements.
	}else{
		angle = 0.0;
	}	
}

void Object::drawGradient(GLfloat x, GLfloat y, 
				  	  GLfloat width, GLfloat height,
					  GLint direction, GLclampf r, GLclampf g, GLclampf b,
					  char a){
	
	for(this->x = x; this->x < x + width; this->x++){
		for(this->y = y; this->y < y + height; this->y++){
			if(a == 'r'){
				if(direction == 1){//left to right
					glColor3f((this->x -x)*r/width, g, b);
				}else if(direction == 2){//right to left
					glColor3f(1.0 - (this->x +x)*r/width, g, b);
				}else if(direction == 3){//bottom to top
					glColor3f(1.0 - (this->y -y)*r/height, g, b);
				}else if(direction == 4){//top to bottom
					glColor3f((this->y -y)*r/height, g, b);
				}else{
					cerr<<"Please enter number range from 1 to 4."<<endl;
				}
			}else if(a == 'g'){
				if(direction == 1){//left to right
					glColor3f(r, (this->x -x)*g/width, b);
				}else if(direction == 2){//right to left
					glColor3f(r, 1.0 - (this->x +x)*g/width, b);
				}else if(direction == 3){//bottom to top
					glColor3f(r, 1.0 - (this->y -y)*g/height, b);
				}else if(direction == 4){//top to bottom
					glColor3f(r,(this->y -y)*g/height, b);
				}else{
					cerr<<"Please enter number range from 1 to 4."<<endl;
				}
			}else if(a == 'b'){
				if(direction == 1){//left to right
					glColor3f(r, g, (this->x -x)*b/width);
				}else if(direction == 2){//right to left
					glColor3f(r, g, 1.0 - (this->x +x)*b/width);
				}else if(direction == 3){//bottom to top
					glColor3f(r, g, 1.0 - (this->y -y)*b/height);
				}else if(direction == 4){//top to bottom
					glColor3f(r, g,(this->y -y)*b/height);
				}else{
					cerr<<"Please enter number range from 1 to 4."<<endl;
				}
			}else{
				cerr<<"Please enter r, g, b only."<<endl;
			}
			
			this->drawPoint(this->x, this->y, 1);
		}
	}
}

void Object::drawCurveLine(GLfloat x1, GLfloat y1, 
				   GLfloat x2, GLfloat y2,
				   GLfloat x3, GLfloat y3,
				   GLfloat x4, GLfloat y4)
{
	GLfloat ctrlpoints[4][3] = { 
			{ x1, y1, 0.0 },
            { x2, y2, 0.0 },
            { x3, y3, 0.0 },
            { x4, y4, 0.0 } };
    glShadeModel(GL_FLAT);
    glLineWidth(1);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4,
            &ctrlpoints[0][0]);
 
    glEnable(GL_MAP1_VERTEX_3);
    glBegin(GL_LINE_STRIP);
 
    // Find the coordinates
    for (int i = 0; i <= 30; i++){
    	glEvalCoord1f((GLfloat)i / 30.0);
    } 
    glEnd();
    glFlush();
}

void Object::lighting(GLfloat x, GLfloat y, GLfloat brightness){

	static GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	static GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	static GLfloat ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	static GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat position[] = {x, y, brightness, 1.0f};
	short shininess = 128;
	
	glDisable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_LIGHT0);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, shininess);
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	
}



