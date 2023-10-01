#include <windows.h> // Load W32 window, console window.
#include <gl/glut.h> // OpenGL toolkit
#include "object.h"
#define SPACEBAR 32 // Allow the usage of Spacebar key

using namespace std; // There is no need to specific the std prefix for I/O stream. e.g. std::cin, std::cout.

GLfloat rotationAngle = 360.0;
GLfloat angle2 = 0.0;
GLfloat angle3 = 120.0;
GLfloat charge = 0.0;
GLfloat housePower = 0.0;
GLfloat vel = 1.0;
GLfloat stop = 0.0;
GLfloat windLength = 0.0;
GLfloat windPattern = 1.0;
GLfloat mouseX = 0.0;
GLfloat mouseY = 0.0;

bool sun = true;
bool power = false;
bool moveLeft = false;
bool moveRight = false;
bool moving = false;
bool getPower = false;
bool putTable = false;
bool dropFloor = true;
bool getPower_table = false;
bool jump = false;
bool switchScene = true;
bool light = false;

string text;
string scene = "menu";
string eyeFacing = "right";

Object objOutdoor;
Object objSun = Object(-75, 540, 1.0, 0.1, 0.1);
Object objMoon = Object(-75, 540, 1.0, 0.1, 0.1);
Object objTurbineBlade = Object(400, 300, 1.0, 0.1, 0.1);
Object objStickman = Object(400, 560, 1.0, 0.1, 0.1);
Object objEye = Object(405, 560, 1.0, 0.1, 0.1);
Object objBatteryMenu = Object(450, 150, 1.0, 0.1, 0.1);
Object objBattery = Object(0, 150, 1.0, 0.1, 0.1);
Object objBattery2 = Object(0, 150, 1.0, 0.1, 0.1);
Object objIndoor = Object(50, 150, 1.0, 0.1, 0.1);
Object objEnergyBar = Object(30, 200, 1.0, 0.1, 0.1);
Object txtObj = Object();
Object objLighting = Object();
Object title = Object(); 
Object objLight = Object();

void init(){
	switchScene = true;
	scene = "menu";
	objStickman.x = 400;
	objStickman.y = 560;
	objEye.x = 405;
	objEye.y = 560;
}

void mouseMove(int x, int y){
    mouseX = x;
    mouseY = y;
}

void mouse(GLint button, GLint state, GLint x, GLint y){
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){
				if (mouseX >= 300 && mouseX <= 450 && mouseY >= 250 && mouseY <= 310){
					scene = "outdoor";
					objStickman.x = 50;
					objStickman.y = 150;
					objEye.x = 55;
					objEye.y = 155;
					switchScene = true;
				}
				if (mouseX >= 300 && mouseX <= 450 && mouseY >= 315  && mouseY <= 365){
									
					int windowID = glutCreateWindow("Wind Turbine Generator");
					glutDestroyWindow(windowID);
					exit(0);
				}
			}
		break;
	}
	glutPostRedisplay();
}

void keyboardControl1(unsigned char key, GLint x, GLint y){
	switch(key){
		case 'w': case 'W': // jump
			moving = true;
			jump = true;	
			break; 
		case 'a': case 'A': // right to left
			eyeFacing = "left";
			if(objStickman.x > 10 & scene == "menu"){
				objStickman.x -= 7;
				objEye.x -= 7; 
				moving = true;
				if(!dropFloor){
					objBatteryMenu.x -= 7; 
				}
			}else if(objStickman.x > 10 & scene == "outdoor"){
				objStickman.x -= 7;
				objEye.x -= 7; 
				moving = true;
			}else if(scene == "indoor"){
				objStickman.x -= 7; 
				objEye.x -= 7; 
				moving = true; 
				if(objStickman.x < -10){
					scene = "outdoor";
					objStickman.x = 300;
					objStickman.y = 150;
					objEye.x = 305;
					objEye.y = 155;
					switchScene = true;
				}
			}	
			break; 
		case 'd': case 'D': // left to right
			eyeFacing = "right";
			if(objStickman.x < 790 & scene == "menu"){
				objStickman.x += 7;
				objEye.x += 7; 
				moving = true;
				if(!dropFloor){
					objBatteryMenu.x += 7; 
				}
			}else if(objStickman.x < 790 & scene == "outdoor"){
				objStickman.x += 7; 
				objEye.x += 7; 
				moving = true; 
			}else if(scene == "indoor"){
				objStickman.x += 7; 
				objEye.x += 7; 
				moving = true; 
				if(objStickman.x > 810){
					scene = "outdoor";
					objStickman.x = 300;
					objStickman.y = 150;
					objEye.x = 305;
					objEye.y = 155;
					switchScene = true;
				}
			}
			break;

		case SPACEBAR: 
			if(!dropFloor & scene == "menu"){
				dropFloor = true;
			}
			else if(objStickman.x >= objBatteryMenu.x-10 & objStickman.x <= objBatteryMenu.x+10 & dropFloor & scene == "menu"){
				dropFloor = false;
			}
			else if(objStickman.x >= 605 & objStickman.x <= 675 & power & scene == "outdoor"){
				getPower = true;
				power = false;
				charge = 0;
			}
			else if(objStickman.x >= 250 & objStickman.x <= 350 & scene == "outdoor"){
				scene = "indoor";
				objStickman.x = 50;
				objStickman.y = 200;
				objEye.x = 55;
				objEye.y = 205;
				switchScene = true;
				
			}
			else if(objStickman.x >= 700 & objStickman.x <= 800 & getPower == false & light == true & housePower >= 120){
				getPower = true;
				housePower = 0;	
				light = false;
			}
			else if(objStickman.x >= 700 & objStickman.x <= 800 & getPower == true){
				getPower = false;
				light = true;
			} 
			else if(objStickman.x >= 350 & objStickman.x <= 560 & putTable == true & getPower == false){
				getPower = true;
				putTable = false;
			}
			else if(objStickman.x >= 350 & objStickman.x <= 560 & putTable == false & getPower == true){
				getPower = false;
				putTable = true;
			} 
			break;	
		
		// Case 27 = esc key	
		case 27: 
			init();
			break; // Esc key to reset to the default perspective view
	}
	glutPostRedisplay();
}
void sceneMenu(){
	// Title box
	glColor3f(0.0f, 0.0f, 0.0f);
	title.drawQuad(300, 420, 300, 450, 450, 450, 450, 420);
	
	// Stickman head and body
	glColor3f(0.0f, 0.0f, 0.0f); 
	objStickman.drawRegularPolygon(objStickman.x, objStickman.y, 15, 36, 10, 1, 1);
	objStickman.drawLine(objStickman.x, objStickman.y, objStickman.x, objStickman.y-75, 10);
	
	// Stickman eye
	if(eyeFacing == "right"){
		glColor3f(1.0f, 1.0f, 1.0f);
		objEye.drawRegularPolygon(objEye.x, objEye.y, 5, 36, 10, 0.5, 1);
	}else if(eyeFacing == "left"){
		glColor3f(1.0f, 1.0f, 1.0f);
		objEye.drawRegularPolygon(objEye.x-10, objEye.y, 5, 36, 10, 0.5, 1);
	}
	// Stickman legs movement
	glColor3f(0.0f, 0.0f, 0.0f); 
	if(moving){
		if(angle2 >= 30){
			vel = -0.5;
		}else if(angle2 <= -30){
			vel = 0.5;
		}
		angle2 = angle2 + vel;	
		glPushMatrix();
			glTranslatef(125,400,0);
			glRotatef(angle2,0.0,0.0,1.0);
			glTranslatef(-125,-400,0);
		glPopMatrix();
		glPushMatrix();	
			objStickman.rotate(angle2, 0, 0, 1, objStickman.x, objStickman.y-75, 0);
			objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
		glPopMatrix();	
		glPushMatrix();
			glTranslatef(125,400,0);
			glRotatef(-angle2,0.0,0.0,1.0);
			glTranslatef(-125,-400,0);
		glPopMatrix();
		glPushMatrix();	
			objStickman.rotate(-angle2, 0, 0, 1, objStickman.x, objStickman.y-75, 0);
			objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
		glPopMatrix();
		stop += 1;
		
		if(jump & stop <= 25){
			objStickman.y += 1; 
			objEye.y += 1;
		}else if(jump & stop > 25 & objStickman.y > 560){
			objStickman.y -= 1; 
			objEye.y -= 1;
		}
		if(stop>= 50){
			moving = false;
			stop = 0;
			jump = false;
		}
	}else{
		objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
	}
	
	if(!dropFloor){// Battery on stickman	
		glColor3f(0.0f, 1.0f, 0.5f);
		objBattery2.drawLine(objStickman.x, objStickman.y-62, objStickman.x, objStickman.y-25, 20);
		objBattery2.drawLine(objStickman.x, objStickman.y-60, objStickman.x, objStickman.y-20, 12);
		glColor3f(1.0f,1.0f,0.0f);
		objBattery2.drawLine(objStickman.x-8, objStickman.y-45, objStickman.x+5, objStickman.y-25, 5);
		objBattery2.drawLine(objStickman.x-8, objStickman.y-45, objStickman.x+8, objStickman.y-45, 5);
		objBattery2.drawLine(objStickman.x+5, objStickman.y-45, objStickman.x-8, objStickman.y-60, 5);
	}else if(dropFloor){// Battery on floor	
		glColor3f(0.0f, 1.0f, 0.5f);
		objBatteryMenu.drawLine(objBatteryMenu.x, 460, objBatteryMenu.x, 497, 20);
		objBatteryMenu.drawLine(objBatteryMenu.x, 462, objBatteryMenu.x, 502, 12);
		glColor3f(1.0f,1.0f,0.0f);
		objBatteryMenu.drawLine(objBatteryMenu.x-8, 477, objBatteryMenu.x+5, 492, 5);
		objBatteryMenu.drawLine(objBatteryMenu.x-8, 477, objBatteryMenu.x+5, 477, 5);
		objBatteryMenu.drawLine(objBatteryMenu.x+5, 477, objBatteryMenu.x-8, 462, 5);
	}
	//Title texts
	glColor3f(0.882f, 0.172f, 0.172f); 
	text = "2.5D";
	title.drawText(text.data(), text.size(), 355, 425, GLUT_BITMAP_TIMES_ROMAN_24);
	
	glColor3f(0.007f, 0.243f, 0.541f);
	text = "Energy Generator Simulation";
	title.drawText(text.data(), text.size(), 225, 375, GLUT_BITMAP_TIMES_ROMAN_24);

	// Button Start
	glColor3f(0.627f, 0.768f, 1.0f);
	title.drawQuad(300, 350, 300, 300, 450, 300, 450, 350);
		
	glColor3f(1.0f, 1.0f, 1.0f); 
	text = "Start";
	title.drawText(text.data(), text.size(), 345, 315, GLUT_BITMAP_TIMES_ROMAN_24);
		
	// Button Quit
	glColor3f(0.627f, 0.768f, 1.0f);
	title.drawQuad(300, 285, 300, 235, 450, 235, 450, 285);
		
	glColor3f(1.0f, 1.0f, 1.0f); 
	text = "Quit";
	title.drawText(text.data(), text.size(), 350, 255, GLUT_BITMAP_TIMES_ROMAN_24);
		
	// Instruction
	glColor3f(0.0f, 0.0f, 0.0f); 
	text = "W = jump";
	title.drawText(text.data(), text.size(), 520, 330, GLUT_BITMAP_TIMES_ROMAN_24);
	text = "A = Left";
	title.drawText(text.data(), text.size(), 520, 300, GLUT_BITMAP_TIMES_ROMAN_24);
	text = "D = Right";
	title.drawText(text.data(), text.size(), 520, 270, GLUT_BITMAP_TIMES_ROMAN_24);
	text = "SPACE = Action";
	title.drawText(text.data(), text.size(), 520, 240, GLUT_BITMAP_TIMES_ROMAN_24);

	// Developer Info
	glColor3f(0.0f, 0.0f, 0.0f); 
	text = "Developed by: ";
	title.drawText(text.data(), text.size(), 100, 200, GLUT_BITMAP_TIMES_ROMAN_24);
	text = "1) Edmund ";
	title.drawText(text.data(), text.size(), 120, 160, GLUT_BITMAP_TIMES_ROMAN_24);
	text = "2) WYT ";
	title.drawText(text.data(), text.size(), 120, 120, GLUT_BITMAP_TIMES_ROMAN_24);
	text = "3) KJH";
	title.drawText(text.data(), text.size(), 120, 80, GLUT_BITMAP_TIMES_ROMAN_24);
	text = "4) Jeffery ";
	title.drawText(text.data(), text.size(), 120, 40, GLUT_BITMAP_TIMES_ROMAN_24);

}		

void sceneOutdoor(){
	// Sun and moon
	if(sun){
		glColor3f(0.529f, 0.807f, 0.921f); 
		glRectf(0,50,800,600);
		glColor3f(1.0f, 1.0f, 0.0f);
		objSun.translate(objSun.xVel, 0.0);
		objSun.drawRegularPolygon(objSun.x, objSun.y, 50, 36, 10, 1, 1);
		//Morning lighting
		if(objSun.x <= 50){
			objLighting.lighting(objSun.x, objSun.y, 100);
		}else if(objSun.x <= 80){
			objLighting.lighting(objSun.x, objSun.y, 150);
		}else if(objSun.x <= 875){
			objLighting.lighting(objSun.x, objSun.y, 250);
		}else if(objSun.x >= 875){
			objSun.x = -75;
			sun = false;
		}
	}else if(!sun){
		
		if(light){
			//Door lighting
			objLighting.lighting(250, 135, 50);
		}else{
			//Night lighting
			objLighting.lighting(1000, 1000, 100);
		}	
		glColor3f(0.047f, 0.078f, 0.270f); //12, 20, 69
		glRectf(0,50,800,600);
		glColor3f(1.0f, 1.0f, 1.0f);
		objMoon.translate(objMoon.xVel, 0.0);
		objMoon.drawRegularPolygon(objMoon.x, objMoon.y, 50, 36, 10, 1, 1);
			
		if(objMoon.x >= 875){
			objMoon.x = -75;
			sun = true;
		}
	}		
	//Roof of house
	glColor3f(0.8f, 0.4f, 0.0f); // Set dark brown brush.
	objOutdoor.drawTriangle(20, 300, 210, 450, 420, 300);
	//Body of house
	glColor3f(1.0f, 0.698f, 0.4f); // Set light brown brush
	objOutdoor.drawQuad(20, 50, 420, 50, 420, 300, 20, 300);
	//Door of house
	glColor3f(0.8f, 0.4f, 0.0f); // Set cyan brush
	objOutdoor.drawQuad(250, 50, 350, 50, 350, 220, 250, 220);
	//Door knob of house
	glColor3f(0.5f, 0.35f, 0.05f); // Set brown brush
	objOutdoor.drawRegularPolygon(270, 120, 10, 48, 0, 0.5, 0.5);
	
	// Body of wind turbine
	glColor3f(1.0f, 0.7f, 0.0f); // Set gold brush.
	objOutdoor.drawQuad(605, 50, 675, 50, 655, 300, 625, 300);		
	// Base of wind turbine
	glColor3f(0.5f, 0.0f, 0.0f); // Set dark red brush.
	objOutdoor.drawLine(590, 50, 690, 50, 25);	
	// Head of wind turbine
	glColor3f(1.0f, 0.3f, 0.0f); // Set blue brush.
	objOutdoor.drawQuad(620, 334, 660, 334, 660, 265, 620, 265);
	objOutdoor.drawQuad(605, 320, 674, 320, 674, 280, 605, 280);
		
	objOutdoor.drawRegularPolygon(620, 320, 15, 36, 10, 1, 1);
	objOutdoor.drawRegularPolygon(660, 320, 15, 36, 10, 1, 1);
	objOutdoor.drawRegularPolygon(620, 280, 15, 36, 10, 1, 1);
	objOutdoor.drawRegularPolygon(660, 280, 15, 36, 10, 1, 1);
		
	// Ground 
	if(sun){
		glColor3f(0.0f, 0.603f, 0.090f); //0,154,23
		glRectf(0,0,800,50);
	}else if(!sun){
		glColor3f(0.0f, 0.303f, 0.090f); //0,154,23
		//glColor3f(0.0f, 0.0f, 0.0f); //12, 20, 69
		glRectf(0,0,800,50);
	}
	
	// Blade of wind turbine
	glColor3f(0.9f, 0.9f, 0.9f); // Set white brush.
	angle3 = angle3+1;
	glPushMatrix();
		glTranslatef(125,400,0);
		glRotatef(angle3,0.0,0.0,1.0);
		glTranslatef(-125,-400,0);
	glPopMatrix();
	glPushMatrix();	
		objTurbineBlade.rotate(angle3+1, 0, 0, 1, 640, 300, 0);
		objTurbineBlade.drawTriangle(640, 300, 640, 450, 660, 375);
		objTurbineBlade.rotate(120, 0, 0, 1, 640, 300, 0);
		objTurbineBlade.drawTriangle(640, 300, 640, 450, 660, 375);
		objTurbineBlade.rotate(120, 0, 0, 1, 640, 300, 0);
		objTurbineBlade.drawTriangle(640, 300, 640, 450, 660, 375);		
	glPopMatrix();
		
	// Nose cone of wind turbine
	glColor3f(1.0f, 0.7f, 0.0f); 
	objOutdoor.drawRegularPolygon(640, 300, 15, 36, 10, 1, 1);

	// Battery bar for wind turbine
	glColor3f(1.0f, 1.0f, 1.0f); 
	objOutdoor.drawLine(640, 70, 640, 120, 10);
	if(charge < 50){
		charge = charge + 0.1;
	}else if(charge >= 50){
		power = true;
	}
	glColor3f(0.0f, 1.0f, 0.0f); 
	objOutdoor.drawLine(640, 70, 640, 70+charge, 10);
		
	// Stickman head and body
	glColor3f(0.0f, 0.0f, 0.0f); 
	objStickman.drawRegularPolygon(objStickman.x, objStickman.y, 15, 36, 10, 1, 1);
	objStickman.drawLine(objStickman.x, objStickman.y, objStickman.x, objStickman.y-75, 10);
	// Stickman legs movement
	if(moving){
		if(angle2 >= 30){
			vel = -0.5;
		}else if(angle2 <= -30){
			vel = 0.5;
				
		}
		angle2 = angle2 + vel;
			
		glPushMatrix();
		glTranslatef(125,400,0);
		glRotatef(angle2,0.0,0.0,1.0);
		glTranslatef(-125,-400,0);
		glPopMatrix();
		glPushMatrix();	
		objStickman.rotate(angle2, 0, 0, 1, objStickman.x, objStickman.y-75, 0);
		objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
		glPopMatrix();
			
		glPushMatrix();
		glTranslatef(125,400,0);
		glRotatef(-angle2,0.0,0.0,1.0);
		glTranslatef(-125,-400,0);
		glPopMatrix();
		glPushMatrix();	
		objStickman.rotate(-angle2, 0, 0, 1, objStickman.x, objStickman.y-75, 0);
		objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
		glPopMatrix();
		stop += 1;
		
		if(jump & stop <= 25){
			objStickman.y += 1; 
			objEye.y += 1;
		}else if(jump & stop > 25 & objStickman.y > 150){
			objStickman.y -= 1; 
			objEye.y -= 1;
		}
		if(stop>= 50){
			moving = false;
			stop = 0;
			jump = false;
		}
	}else{
		objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
	}
	
	// Wind particle
	glColor3f(0.0f, 0.0f, 0.0f); // Set black color.
	if(windLength <= 60 & windPattern == 1){
		objOutdoor.drawCurveLine(50+windLength, 280, 100+windLength, 260, 120+windLength*1.5, 260, 250+windLength*1.5, 290);
	}else if(windLength <= 60 & windPattern == 2){
		objOutdoor.drawCurveLine(350+windLength, 330, 410+windLength, 350, 430+windLength*1.5, 350, 550+windLength*1.5, 320);
	}else if(windLength <= 60 & windPattern == 3){
		objOutdoor.drawCurveLine(150+windLength, 400, 200+windLength, 380, 220+windLength*1.5, 370, 350+windLength*1.5, 420);
	}else if(windLength <= 60 & windPattern == 4){
		objOutdoor.drawCurveLine(450+windLength, 450, 510+windLength, 480, 530+windLength*1.5, 470, 650+windLength*1.5, 440);
	}else if(windLength > 90){
		windLength = 0;
		windPattern += 1;
		if(windPattern == 5){
			windPattern = 1;
		}
	}
	windLength +=0.3;
	
	// Stickman eye
	if(eyeFacing == "right"){
		glColor3f(1.0f, 1.0f, 1.0f);
		objEye.drawRegularPolygon(objEye.x, objEye.y, 5, 36, 10, 0.5, 1);
	}else if(eyeFacing == "left"){
		glColor3f(1.0f, 1.0f, 1.0f);
		objEye.drawRegularPolygon(objEye.x-10, objEye.y, 5, 36, 10, 0.5, 1);
	}
			
	// Floating text
	if(objStickman.x >= 605 & objStickman.x <= 675 & power){
		text = "(Press 'space' to collect energy)";
		// Shadow casting for text.
		glColor3f(0.0f, 0.0f, 0.345f); 
		txtObj.drawText(text.data(), text.size(), 501, 205, GLUT_BITMAP_8_BY_13);
		// Actual text.
		glColor3f(0.0f, 0.0f, 0.545f); 
		txtObj.drawText(text.data(), text.size(), 500, 205, GLUT_BITMAP_8_BY_13);
	}else if(objStickman.x >= 250 & objStickman.x <= 350){
		text = "(Press 'space' to enter house)";
		// Shadow casting for text.
		glColor3f(0.0f, 0.0f, 0.345f);
		txtObj.drawText(text.data(), text.size(), 181, 225, GLUT_BITMAP_8_BY_13);
		// Actual text.
		glColor3f(0.0f, 0.0f, 0.545f);
		txtObj.drawText(text.data(), text.size(), 180, 225, GLUT_BITMAP_8_BY_13);
	}
	
	//Battery on head of stickman
	if(getPower){
		glColor3f(0.0f, 1.0f, 0.5f);
		objBattery2.drawLine(objStickman.x, objStickman.y+13, objStickman.x, objStickman.y+50, 20);
		objBattery2.drawLine(objStickman.x, objStickman.y+15, objStickman.x, objStickman.y+55, 12);
		glColor3f(1.0f,1.0f,0.0f);
		objBattery2.drawLine(objStickman.x-9, objStickman.y+25, objStickman.x+5, objStickman.y+45, 5);
		objBattery2.drawLine(objStickman.x-9, objStickman.y+25, objStickman.x+8, objStickman.y+30, 5);
		objBattery2.drawLine(objStickman.x+5, objStickman.y+30, objStickman.x-8, objStickman.y+15, 5);
	}
}

void sceneIndoor(){
	glColor3f(1.0f, 0.988f, 0.859f); // Light yellow wall
	objIndoor.drawQuad(0, 150, 800, 150, 800, 600, 0, 600);
	
	glColor3f(0.0f, 0.0f, 0.0f); // Black color wall line
	objIndoor.drawLine(0, 150, 800, 150, 3);
	
	glColor3f(0.8f, 0.8f, 0.8f); // Grey for ground
	objIndoor.drawQuad(0, 0, 800, 0, 800, 150, 0, 150);
	
	glColor3f(0.5f, 0.35f, 0.05f); // Set brown for window
	objIndoor.drawQuad(100, 240, 300, 240, 300, 460, 100, 460);
	
	glColor3f(1.0f, 1.0f, 1.0f); // Set white for window
	objIndoor.drawQuad(110, 250, 195, 250, 195, 450, 110, 450);
	objIndoor.drawQuad(205, 250, 290, 250, 290, 450, 205, 450);
	
	glColor3f(0.658824, 0.658824, 0.658824); // Set grey for bulb holder
	objIndoor.drawQuad(370, 590, 640, 590, 640, 600, 370, 600);
	objIndoor.drawLine(380, 590, 380, 570, 3);
	objIndoor.drawLine(380, 570, 390, 570, 3);
	objIndoor.drawLine(630, 590, 630, 570, 3);
	objIndoor.drawLine(630, 570, 620, 570, 3);
	
	glColor3f(0.95f, 0.95f, 0.95f); // Set white for bulb
	objIndoor.drawQuad(390, 560, 620, 560, 620, 580, 390, 580);

	glColor3f(0.0f, 0.0f, 0.0f); // Set black for bulb
	objIndoor.drawRegularPolygon(390, 570, 10, 48, 0, 0.4, 1.0);
	objIndoor.drawRegularPolygon(620, 570, 10, 48, 0, 0.4, 1.0);
		
	
	glColor3f(0.5f, 0.35f, 0.05f); // Set brown for table
	objIndoor.drawQuad(400, 250, 407, 250, 407, 145, 400, 145);	
	objIndoor.drawQuad(550, 250, 557, 250, 557, 145, 550, 145);
	objIndoor.drawQuad(350, 220, 357, 220, 357, 125, 350, 125);
	objIndoor.drawQuad(500, 220, 507, 220, 507, 125, 500, 125);
	objIndoor.drawQuad(400, 250, 557, 250, 507, 220, 350, 220);
	objIndoor.drawQuad(400, 250, 557, 250, 507, 220, 350, 220);
	
	glColor3f(0.8f, 0.8f, 0.8f); // Set grey for battery generator 
	objIndoor.drawQuad(700, 200, 800, 200, 800, 350, 700, 350);	
	
	// Stickman legs movement
	glColor3f(0.0f, 0.0f, 0.0f); 
	if(moving){
		if(angle2 >= 30){
			vel = -0.5;
		}else if(angle2 <= -30){
			vel = 0.5;
		}
		angle2 = angle2 + vel;
		
		glPushMatrix();
			glTranslatef(125,400,0);
			glRotatef(angle2,0.0,0.0,1.0);
			glTranslatef(-125,-400,0);
		glPopMatrix();
		glPushMatrix();	
			objStickman.rotate(angle2, 0, 0, 1, objStickman.x, objStickman.y-75, 0);
			objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(125,400,0);
			glRotatef(-angle2,0.0,0.0,1.0);
			glTranslatef(-125,-400,0);
		glPopMatrix();
		glPushMatrix();	
			objStickman.rotate(-angle2, 0, 0, 1, objStickman.x, objStickman.y-75, 0);
			objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
		glPopMatrix();
		stop += 1;
		
		if(jump & stop <= 25){
			objStickman.y += 1; 
			objEye.y += 1;
		}else if(jump & stop > 25 & objStickman.y > 200){
			objStickman.y -= 1; 
			objEye.y -= 1;
		}
		if(stop>= 50){
			moving = false;
			stop = 0;
			jump = false;
		}
	}else{
		objStickman.drawLine(objStickman.x, objStickman.y-75, objStickman.x, objStickman.y-100, 10);
	}
	// Generator energy power
	if(housePower <= 0){
		housePower = 0;
		//light = false;
	}else{
		housePower = 150;
		light = true;
	}
	// Generator energy power bar
	glColor3f(0.0f, 1.0f, 0.0f); 
	objIndoor.drawLine(705, 200, 705, 200+housePower, 50);
	objIndoor.drawLine(715, 200, 715, 200+housePower, 50);
	objIndoor.drawLine(725, 200, 725, 200+housePower, 50);
	objIndoor.drawLine(735, 200, 735, 200+housePower, 50);
	objIndoor.drawLine(745, 200, 745, 200+housePower, 50);
	objIndoor.drawLine(755, 200, 755, 200+housePower, 50);
	objIndoor.drawLine(765, 200, 765, 200+housePower, 50);
	objIndoor.drawLine(775, 200, 775, 200+housePower, 50);
	objIndoor.drawLine(785, 200, 785, 200+housePower, 50);
	objIndoor.drawLine(795, 200, 795, 200+housePower, 50);
	objIndoor.drawLine(805, 200, 805, 200+housePower, 50);
	objIndoor.drawLine(815, 200, 815, 200+housePower, 50);
	objIndoor.drawLine(825, 200, 825, 200+housePower, 50);
	objIndoor.drawLine(835, 200, 835, 200+housePower, 50);

	// Lighting of bulb
	if(light){
		objLighting.lighting(505, 500, 500);
		housePower = 150;
		glColor3f(0.9647f, 0.745f, 0.0f);
		objLight.drawQuad(392, 560, 618, 560, 618, 580, 392, 580);
		glColor3f(1.0f, 0.994f, 0.5f);
		objLight.drawQuad(300, 430, 710, 430, 618, 560, 392, 560);
	}else{
		objLighting.lighting(1000, 1000, 100);	
	}
	// Battery on table	
	if(putTable){
		glColor3f(0.0f, 1.0f, 0.5f);
		objBattery.drawLine(450, 235, 450, 272, 20);
		objBattery.drawLine(450, 237, 450, 277, 12);
		glColor3f(1.0f,1.0f,0.0f);
		objBattery.drawLine(442, 252, 455, 267, 5);
		objBattery.drawLine(442, 252, 458, 252, 5);
		objBattery.drawLine(455, 252, 442, 237, 5);
	}
	
	glColor3f(1.0f, 1.0f, 0.0f); // Set yellow for battery generator symbol
	objIndoor.drawLine(780, 340, 720, 270, 7);
	objIndoor.drawLine(720, 270, 770, 280, 7);
	objIndoor.drawLine(770, 280, 720, 210, 7);
	
	// Moving battery on stickman head
	if(getPower){
		// Battery body
		glColor3f(0.0f, 1.0f, 0.5f);
		objBattery2.drawLine(objStickman.x, objStickman.y+13, objStickman.x, objStickman.y+50, 20);
		objBattery2.drawLine(objStickman.x, objStickman.y+15, objStickman.x, objStickman.y+55, 12);
		// Battery logo
		glColor3f(1.0f,1.0f,0.0f);
		objBattery2.drawLine(objStickman.x-8, objStickman.y+30, objStickman.x+5, objStickman.y+45, 5);
		objBattery2.drawLine(objStickman.x-8, objStickman.y+30, objStickman.x+8, objStickman.y+30, 5);
		objBattery2.drawLine(objStickman.x+5, objStickman.y+30, objStickman.x-8, objStickman.y+15, 5);
	}
	
	// Floating text when near table and generator
	if(objStickman.x >= 350 & objStickman.x <= 570 & putTable == false & getPower == true){
		text = "(Press 'space' to place energy on table)";
		// Shadow casting for text.
		glColor3f(0.0f, 0.0f, 0.345f);
		txtObj.drawText(text.data(), text.size(), 320.5, 287, GLUT_BITMAP_8_BY_13);
		// Actual text.
		glColor3f(0.0f, 0.0f, 0.545f);
		txtObj.drawText(text.data(), text.size(), 320, 287, GLUT_BITMAP_8_BY_13);
	} 
	else if(objStickman.x >= 350 & objStickman.x <= 570 & putTable == true & getPower == false){
		text = "(Press 'space' to collect energy from table)";
		// Shadow casting for text.
		glColor3f(0.0f, 0.0f, 0.345f);
		txtObj.drawText(text.data(), text.size(), 320.5, 287, GLUT_BITMAP_8_BY_13);
		// Actual text.
		glColor3f(0.0f, 0.0f, 0.545f);
		txtObj.drawText(text.data(), text.size(), 320, 287, GLUT_BITMAP_8_BY_13);
	} 
	else if (objStickman.x >= 700 & objStickman.x <= 800 & getPower == true & housePower == 0){
		text = "(Press 'space' to insert energy)";
		// Shadow casting for text.
		glColor3f(0.0f, 0.0f, 0.345f);
		txtObj.drawText(text.data(), text.size(), 545.5, 370, GLUT_BITMAP_8_BY_13);
		// Actual text.
		glColor3f(0.0f, 0.0f, 0.545f); 
		txtObj.drawText(text.data(), text.size(), 545, 370, GLUT_BITMAP_8_BY_13);
	}
	else if (objStickman.x >= 700 & objStickman.x <= 800 & getPower == false & housePower >= 150){
		text = "(Press 'space' to collect back energy)";
		// Shadow casting for text
		glColor3f(0.0f, 0.0f, 0.345f);
		txtObj.drawText(text.data(), text.size(), 490.5, 370, GLUT_BITMAP_8_BY_13);
		// Actual text.
		glColor3f(0.0f, 0.0f, 0.545f); 
		txtObj.drawText(text.data(), text.size(), 490, 370, GLUT_BITMAP_8_BY_13);
	}	
	
	// Stickman head and body
	glColor3f(0.0f, 0.0f, 0.0f); 
	objStickman.drawRegularPolygon(objStickman.x, objStickman.y, 15, 36, 10, 1, 1);
	objStickman.drawLine(objStickman.x, objStickman.y, objStickman.x, objStickman.y-75, 10);
	
	// Stickman eye
	if(eyeFacing == "right"){
		glColor3f(1.0f, 1.0f, 1.0f);
		objEye.drawRegularPolygon(objEye.x, objEye.y, 5, 36, 10, 0.5, 1);
	}else if(eyeFacing == "left"){
		glColor3f(1.0f, 1.0f, 1.0f);
		objEye.drawRegularPolygon(objEye.x-10, objEye.y, 5, 36, 10, 0.5, 1);
	}
}		   
void render(){ // Render function definition
	glClearColor(0.972f, 0.929f, 0.921f, 0.0f); // RGBA
	// Canvas settings
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 600); // Set canvas resolution to 800 by 600 pixels.
	glClear(GL_COLOR_BUFFER_BIT); // Load frame buffer.
	
	if(switchScene){	
		glFlush();
		glutSwapBuffers();
		glutPostRedisplay();
		switchScene = false;
	}
	
	if(scene == "menu"){
		sceneMenu();			
	}else if(scene == "outdoor"){
		sceneOutdoor();
	}else if(scene == "indoor"){
		sceneIndoor();
	}
	
	glutSwapBuffers(); // Swap foreground and background frames.
	glutPostRedisplay();// Update the canvas.
	glFlush(); // Clear all GL executions.
	glFinish(); // Block render function until all GL executions are completed.
}

int main(){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Double frame buffer with RGB colors.
	glutInitWindowSize(800, 600); // OpenGL window size.
	glutInitWindowPosition(50, 50); // Window position on the screen.
	glutCreateWindow("2D motion & animation!"); // OpenGL window title.
	glutDisplayFunc(render); // Load render function.
	glutKeyboardFunc(keyboardControl1);
	glutPassiveMotionFunc(mouseMove);
	glutMouseFunc(mouse);
	glEnable(GL_BLEND); //Enable blending.
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	glutMainLoop(); // Loop frame forever.
	
	system("PAUSE"); // Pause the frame.
	return 0; // Close program.
}
