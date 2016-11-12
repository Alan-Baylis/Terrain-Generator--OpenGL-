#include <stdio.h>
#include <stdlib.h>

// import mesh structures - Height (y) , Faces(v1,v2,v3,v4)
#include "mesh.h"

//import math 
#include <math.h>

// to fix random function for selecting random points using ctime
#include <ctime>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

	

//Globals
int Length, Width; // length of grid (x axis) that is changed by user input  // width of grid (z axis) that is changed by user input
Height** grid;  // declaring grid as an array of pointers // height is y axis
float camPos[] = {Length*2, fmax(Length,Width)*1.5, Width*2};	//where the camera is /change for 9 inputs 
int Iterations;
const int OFF = 0;
const int ON = 1;
int Wireframe = ON;
int QUADS = OFF; 
float MinHeight;
float MaxHeight;
// wireframe colour 

//OpenGL functions

/* Grid Size Input by User 
 ask for input by user for length (Length = user input)
 ask for input by user for width (Width = user input)
 catches for values less than 50 and greater than 300
 */
void GridSize()
{
	 printf("Please input Length of Grid Size as an integer between 50 and 300: ");
	 scanf("%d", &Length);
	 printf("Please input Width of Grid Size as an integer between 50 and 300: ");
	 scanf("%d", &Width);
}

void NewGrid() 
{
	grid = new Height*[Length];
	for (int i = 0; i < Length; i++)
	{
		grid[i] = new Height[Width];
	}

	for (int i = 0; i < Length; i++)
	{
		for (int j = 0; j < Width; j++)
		{
			grid[i][j].y = 0; 
		} 
	}
}

void gridHeight(int iterations)
{
	for (int i = 0; i < iterations; i++)
	{
		int Rand_Point_x = rand()%Length;
		int Rand_Point_z = rand()%Width;
		float disp = 10.0;
		int Radius = fmin(Length,Width)/2;

		for(int i = 0; i < Length; i++)
		{
			for (int j = 0; j < Width; j++)
			{
				// algorithm here
				int x = i - Rand_Point_x;
				int z = j - Rand_Point_z;
				float distance = sqrtf((x*x) + (z*z));
				float pd = (distance*2)/Radius;

				if (fabs(pd) <= 1)
				{
					grid[i][j].y += ((rand()%1)*2)-1 * ( (disp/2) + (cos(pd*3.14159))*(disp/2) ); 
				}
				
				if (grid[i][j].y < MinHeight)
				{
					MinHeight = grid[i][j].y;
				}
				if (grid[i][j].y > MaxHeight)
				{
					MaxHeight = grid[i][j].y;
				}

			}
		}
		
	}	

	// max min height grayscale stuff here

}

void DrawMap()
{
	for (int i = 0; i < Length-1; i++)
	{
		for (int j = 0; j < Width-1; j++)
		{
			if (QUADS == ON)
			{
				glBegin(GL_QUADS);
					glColor3f( ((grid[i][j].y-MinHeight)/(MaxHeight-MinHeight)) , ((grid[i][j].y-MinHeight)/(MaxHeight-MinHeight)), ((grid[i][j].y-MinHeight)/(MaxHeight-MinHeight)) );
					glVertex3f(i, grid[i][j].y, j);
					glVertex3f(i, grid[i][j+1].y, j+1);
					glVertex3f(i+1, grid[i+1][j+1].y, j+1);
					glVertex3f(i+1, grid[i+1][j].y, j);
				glEnd();
			}
			if (QUADS == OFF)
			{
				glBegin(GL_TRIANGLES);
					glColor3f( ((grid[i][j].y-MinHeight)/(MaxHeight-MinHeight)) , ((grid[i][j].y-MinHeight)/(MaxHeight-MinHeight)), ((grid[i][j].y-MinHeight)/(MaxHeight-MinHeight)) );
					glVertex3f(i, grid[i][j].y, j);
					glVertex3f(i, grid[i][j+1].y, j+1);
					glVertex3f(i+1, grid[i+1][j+1].y, j+1);
				glEnd();

				glBegin(GL_TRIANGLES);
					glVertex3f(i, grid[i][j].y, j);
					glVertex3f(i+1, grid[i+1][j+1].y, j+1);
					glVertex3f(i+1, grid[i+1][j].y, j);
				glEnd();
			}
		}
	}
}






void keyboard(unsigned char key, int xIn, int yIn)
{
	switch (key)
	{
		case 'q':
		case 27:	//27 is the esc key
			exit(0);
			break;
		
		case 'W':
		case 'w':
			if (Wireframe == ON)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				Wireframe = OFF;
				break;
			}
			if (Wireframe == OFF)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				Wireframe = ON;
				break;
			}
		case 't':
		case 'T':
			if (QUADS == OFF)
			{
				QUADS = ON;
				break;
			}
		case 'y':
		case 'Y':
			if (QUADS == ON)
			{
				QUADS = OFF;
				break;
			}	
/*
		case GLUT_KEY_UP:
			camPos[0] --;
			camPos[1] ++;
			break;
		case GLUT_KEY_DOWN:
			camPos[0] ++;
			camPos[1] --;
			break;
		case GLUT_KEY_LEFT:
			camPos[0] --;
			camPos[2] ++;
			break;
		case GLUT_KEY_RIGHT:
			camPos[0] ++;
			camPos[2] --;
			break;
*/
	}

	glutPostRedisplay(); 

}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, fmax(Length,Width)*10);
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(Length*2, fmax(Length,Width)*1.5, Width*2, 0, -fmax(Length,Width)/2,0, 0,1,0);
	glColor3f(1,1,1);

	DrawMap();

	//swap buffers double? 
	glutSwapBuffers();
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	srand(time(NULL));

	// ask user for input for grid size 
	GridSize();
	NewGrid();
	gridHeight(fmax(Length,Width)/2);

	glutInit		(&argc, argv);		//starts up GLUT
	glutInitDisplayMode(GLUT_RGBA);

	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("3GC3 Boilerplate");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	// special func stuff like mousefunc

	// Menu
	init();

	// do stuff


	glutMainLoop();				//starts the event glutMainLoop
	return(0);					//return may not be necessary on all compilers
}
