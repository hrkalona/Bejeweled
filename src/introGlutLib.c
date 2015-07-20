/******************************************************************************
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_

					Introductory Graphics Library using GLUT/OpenGL 
					Northwestern University CS110 'Intro to Programming'

11/10/2003: Created --Andrew William Proksel
11/17/2003: Formatted, put fcn bodies in .c files, fixed order dependence, 
			changed floats to doubles, fixed float calls to int GL fcns,
			commented function internals, added more drawing and animation 
			prototypes. --Jack Tumblin
12/05/2003: Created function bodies for the prototypes, tested --A.W. Proksel
12/12/2003: Cleanup comments, revise disclaimer, added internal comments to
			all new functions measured angles from X axis, fixed sin/cos swap, 
			cleanup for consistent formatting, changed all floats to doubles 
			(again!), made #define for all angular steps in the .h file,
			removed redundant variables & openGL calls, added double-buffering
			(#define FINAL_GRADE  A) --J. Tumblin
4/12/2004:  Small clean-up by Prasun Choudhury. To be used in CS110 projects for
			Spring 2004.
8/20/2005:	Corrected and added comments.-- V. Doufexi

 ===============================================================================
  DISCLAIMER:
 introGlutLib.c and  introGlutLib.h originated as a student project written by
 Andrew William Proksel at Northwestern University, Evanston IL, as part of his
 classwork in the "Introduction to Computer Programming" course for non-majors 
 (CS110) taught by Jack Tumblin in Fall 2003.  It applies the wonderfully
  platform-independent 'GLUT' library written Mark Kilgard, ported to Win32 
 (Windows 95,98,Me,NT,2000,XP) by Nate Robins, and we thank them both for
 freely providing their software to everyone.  To obtain it, please visit:
	http://www.xmission.com/~nate/glut.html

  You are welcome to use and modify this code in any way you wish, but:
	1) Please include this disclaimer in any code that includes our work, and 
	please mention us in the credits or bibliography of published work
		(we're academics--publicity is the best reward).
	2) You must hold us harmless for any use you make of this code. Said in  
	plain English, this means you cannot sue us or Northwestern University for 
	any mistakes we made in this software, no matter how you decide to use it 
	or change it, and no matter how badly you were hurt because of it.
	3) This code is provided 'as-is'. While we will gladly answer reasonable
	questions about it, you cannot expect us to maintain the code for you,
	nor can we act as a customer-service desk that helps you debug your code.
*******************************************************************************/

#include "introGlutLib.h"				//holds all function prototypes.

/*=============================================================================
Function Bodies for startup/shutdown
==============================================================================*/

//------------------------------------------------------------------------------
// InitGraphics()
//
//  Do all initialization and setup needed to use this 'introductory graphics'
//  library. 

void InitGraphics()
{    
  char* args[] = {"foobar", 0};
  int one=1;
  // glutInit wants the main's args, but we don't need them for our programs
  // so we'll just pass it some junk.
  	glutInit(&one,args);
	srand( (int) time(NULL) );
	glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGBA);
									// single buffering, use full 32-bit color
	glutInitWindowPosition(NU_SCREEN_XPOS, NU_SCREEN_YPOS);
	glutInitWindowSize(NU_SCREENWIDTH,NU_SCREENHEIGHT);
	glutCreateWindow("Bejeweled!");
									// Open a window (Microsoft Windows)
	glClearColor(1.0,1.0,1.0,0.0);	// Set the background color
	glColor3d(0.0,0.0,0.0);			// Set the default 'drawing pen' color
	glPointSize(3.0);				// Set the line width.

	glMatrixMode(GL_PROJECTION);	// Select world-to-camera transform:
	glLoadIdentity();				// initialize it, then set
	gluOrtho2D(0.0, (GLdouble)NU_SCREENWIDTH, 0.0, (GLdouble)NU_SCREENHEIGHT);
	// left, right,bottom top;
									// an orthographic camera matrix; treats
									// glVertex() values as pixel measurements.
	glutDisplayFunc(myDisplay);		// Register the fcn. GLUT calls for drawing
	glutKeyboardFunc(myKeyboard);	// Register the fcn. GLUT calls for keyboard input
	glutMouseFunc(myMouse);		// Register the fcn. GLUT calls for mouse input
	glutIdleFunc(myIdle);			// Register the fcn. GLUT calls when idle.

}

//------------------------------------------------------------------------------
// CloseGraphics()
//
//  Start the GLUT system, which manages the on-screen windows and makes calls 
// to our 'myDisplay()' function when that window needs to be redrawn.

void CloseGraphics()
{
	glutMainLoop();	
}

//------------------------------------------------------------------------------
// SetWindow(left, right, bottom, top)
//
// Called when users drag the corners of the display window to change its size.
// Changes the OpenGL world-to-camera coordinate transformation matrix.

void setWindow(double left, double right, double bottom, double top)
{
	glMatrixMode(GL_PROJECTION);			// Select the matrix to change,
	glLoadIdentity();						// clear it,
	gluOrtho2D(left, right, bottom, top);	// multiply by new coordinates.
}

//------------------------------------------------------------------------------
// setViewport(left, right, bottom, top)
//
// Change the OpenGL camera-to-screen coordinate system.

void setViewport(int left, int right, int bottom, int top)
{
	glViewport(left, bottom, right - left, top - bottom);
}

/*==============================================================================
		Function Bodies for on-screen drawing and animation
==============================================================================*/

//------------------------------------------------------------------------------
// SetPenColor(red, green, blue)
//
// Set the drawing color. red, green, blue range from 0.0 to 1.0.
// 
//   Commonly used colors:
//		(0.0,0.0,0.0) = black		(1.0,1.0,1.0) = white
//		(1.0,0.0,0.0) = red			(0.0,1.0,1.0) = cyan
//		(0.0,1.0,0.0) = green		(1.0,0.0,1.0) = magenta
//		(0.0,0.0,1.0) = blue		(1.0,1.0,0.0) = yellow
//		(0.5,0.5,0.5) = grey		(1.0,0.5,0.0) = orange
//		(0.5,0.0,0.5) = purple		(0.5,0.25,0.0) = brown
//		(0.0,0.25,0.0) = forest green
//		(0.0,0.0,0.25) = midnight blue

 
void SetPenColor(double red, double green, double blue)
{
	glColor3d(red,green,blue);
}
				
//------------------------------------------------------------------------------
// DrawLine(x0, y0, x1, y1)
// 
// Draw a line from (x0,y0) and ending at (x1,y1). 
// (doesn't use relative addressing).
// x,y are measured in pixels.

void DrawLine(double x0, double y0, double x1, double y1)
{
	glBegin(GL_LINES);				// Draw lines between pairs of points.
	glVertex2d(x0,y0);				// first line: begins at this point,
	glVertex2d(x1,y1);				// ends at this point.
	glEnd();						// No more pairs of points to draw.
	glFlush();						// Finish any pending drawing commands.
}

//-----------------------------------------------------------------------------
// DrawBox (x0, y0, x1, y1)
// 
// Draw a box using the current pen color with lower left corner at (x0,y0)
// and upper right corner at x1, y1.
// x,y are measured in pixels.

void DrawBox(double x0, double y0, double x1, double y1)
{
	glBegin(GL_LINE_STRIP);			// Draw a connected line from
	glVertex2d(x0,y0);				// corner to
	glVertex2d(x1,y0);				// corner to
	glVertex2d(x1,y1);				// corner to
	glVertex2d(x0,y1);				// corner to
	glVertex2d(x0,y0);				// corner, 
	glEnd();						// then stop--we're finished.
	glFlush();						// Finish any pending drawing commands.
}

//------------------------------------------------------------------------------
// DrawFillBox (x0, y0, x1, y1)
// 
// Draw a box and fill the region inside it using the current pen color. 
// You can specify the pen color with the SetPenColor() function.
// The default color is black.
// x,y are measured in pixels.

void DrawFillBox(double x0, double y0, double x1, double y1)

{
	glBegin(GL_POLYGON);			// Draw a connected line from
	glVertex2d(x0,y0);				// corner to
	glVertex2d(x1,y0);				// lcorner to
	glVertex2d(x1,y1);				// corner to
	glVertex2d(x0,y1);				// corner to
	glVertex2d(x0,y0);				// corner, 
	glEnd();						// then stop--we're finished.
	glFlush();						// Finish any pending drawing commands.
}

//------------------------------------------------------------------------------
// DrawFillTriangle(x0, y0, x1, y1, x2, y2)
//
// Draw a triangle and fill the region inside it using the current pen color
// Default fill color is black, but you can change it with the SetPenColor() function.
// x,y are measured in pixels.

void DrawFillTriangle(double x0, double y0, double x1, double y1, double x2, double y2)

{
	glBegin(GL_POLYGON);			// Draw a connected line from
	glVertex2d(x0,y0);				// Vertex 0
	glVertex2d(x1,y1);				// Vertex 1
	glVertex2d(x2,y2);				// Vertex 2
	glVertex2d(x0,y0);				// corner, 
	glEnd();						// then stop--we're finished.
	glFlush();						// Finish any pending drawing commands.
}


//-----------------------------------------------------------------------------
// DrawEllipse(xctr, yctr, radius_x, radius_y)
//
// Draw the outline of an ellipse centered at (xctr,yctr), 
// with width  'radius_x'  and height 'radius_y' 
// using the current pen color and line width. Default pen color is black, but 
// the 'SetPenColor()' function can change it.
// Draws the circle as a sequence of very short,straight lines. 
// x,y, radius are measured in pixels.

void DrawEllipse(double xctr, double yctr, double radiusX, double radiusY) 
{
	double vectorX,vectorY;			// vector to a point on circle from its center
	double angle;					// Angle in radians from circle start point.

	glBegin(GL_LINE_STRIP);		// Tell OpenGL to draw a series of lines:
	for(angle=0; angle < 2.0*M_PI + NU_ANGLESTEP; angle+= NU_ANGLESTEP)			
	{								// (>2PI so that circle is always closed)
		vectorX= xctr + radiusX * cos(angle);	// set line endpoint
		vectorY= yctr + radiusY * sin(angle);		
		glVertex2d(vectorX,vectorY);	// plot the line endpoint.
	}
	glEnd();						// finished drawing line segments.
	glFlush();						// Finish any pending drawing commands
}

void DrawFillEllipse(double xctr, double yctr, double radiusX, double radiusY) 
{
	double vectorX0,vectorY0, vectorX1, vectorY1;			// vector to a point on circle from its center
	double angle;					// Angle in radians from circle start point.

	glBegin(GL_TRIANGLES);		// Tell OpenGL to draw a series of lines
	vectorX1 = xctr + radiusX;
	vectorY1 = yctr;
	for(angle=0; angle < 2.0*M_PI + NU_ANGLESTEP; angle+= NU_ANGLESTEP)			
	{								// (>2PI so that circle is always closed)
		vectorX0 = vectorX1;
		vectorY0 = vectorY1;
		vectorX1= xctr + radiusX * cos(angle);	// set line endpoint
		vectorY1= yctr + radiusY * sin(angle);		
		glVertex2d(xctr,yctr);		// plot the points of a triangle (CCW order)
		glVertex2d(vectorX0,vectorY0);	// center-->old pt-->new pt.
		glVertex2d(vectorX1,vectorY1);
	}
	glEnd();						// finished drawing line segments.
	glFlush();						// Finish any pending drawing commands
}

//-----------------------------------------------------------------------------
// DrawCircle(xctr, yctr, radius)
//
// Draw the outline of a circle centered at (xctr,yctr) and width of 'radius'. 
// using the current pen color and line width. Default pen color is black, but 
// the 'SetPenColor()' function can change it.
// Draws the circle as a sequence of very short,straight lines. 
// x,y, radius are measured in pixels.

void DrawCircle(double xctr, double yctr, double radius)
{
	double vectorX,vectorY;			// vector to a point on circle from its center
	double angle;					// Angle in radians from circle start point.

	glBegin(GL_LINE_STRIP);		// Tell OpenGL to draw a series of lines:
	for(angle=0; angle < 2.0*M_PI + NU_ANGLESTEP; angle+= NU_ANGLESTEP)			
	{								// (>2PI so that circle is always closed)
		vectorX= xctr + radius * cos(angle);	// set line endpoint
		vectorY= yctr + radius * sin(angle);		
		glVertex2d(vectorX,vectorY);	// plot the line endpoint.
	}
	glEnd();						// finished drawing line segments.
	glFlush();						// Finish any pending drawing commands

}

//----------------------------------------------------------------------------
// DrawFillCircle(xctr, yctr, radius)
//
// Draw a circle and fill the region inside it using the the current pen color.  
// Default pen color is black, but the SetFillColor() function will change it.
// Draws the shape using filled triangles.
// x,y, radius are measured in pixels

void DrawFillCircle(double xctr, double yctr, double radius)
{
	double vectorX1,vectorY1;		// vector to a point on circle from its center
	double vectorX0,vectorY0;		// previous version of vectorX1,Y1;
	double angle;					// Angle in radians from circle start point.

	glBegin(GL_TRIANGLES);		// Tell OpenGL to draw a series of triangles
	vectorX1 = xctr + radius;	// Start at the circle's rightmost point.
	vectorY1 = yctr;		
	for(angle=NU_ANGLESTEP;		// step through all other points on circle;
		angle < 2.0*M_PI + NU_ANGLESTEP; angle+= NU_ANGLESTEP)			
	{								// (>2PI so that circle is always closed)
		vectorX0 = vectorX1;		// save previous point's position,
		vectorY0 = vectorY1;
		vectorX1= xctr + radius*cos(angle);	// find a new point on the circle,
		vectorY1= yctr + radius*sin(angle);		
		glVertex2d(xctr,yctr);		// plot the points of a triangle (CCW order)
		glVertex2d(vectorX0,vectorY0);	// center-->old pt-->new pt.
		glVertex2d(vectorX1,vectorY1);
	}
	glEnd();						// finished drawing triangles.
	glFlush();						// Finish any pending drawing commands
}

//-----------------------------------------------------------------------------
// DrawArc (xctr, yctr, radius, startAngle, endAngle)
//
// Draw a circular arc using the current pen color. Plots a portion of the
// circle whose center is (xctr,yctr) and width is given by 'radius', but only
// draws the portion of the circle between 'startAngle' and 'endAngle'.  Both
// the start and end angles are measured from the right-most point on the circle
// (e.g. the x-axis intercept for a circle centered at the origin) in the 
// counter-clockwise direction, in degrees.  Default pen color is black, but 
// the 'SetPenColor()' function can change it for you.
// Draws the arc as a sequence of short, straight-line segments.
// x,y, radius are measured in pixels
// Angles are measured in degrees

void DrawArc (double xctr, double yctr, double radius, double startAngle, double endAngle)
{
	double vectorX,vectorY;			// vector to a point on circle from its center
	double angle, ang0,ang1;

	ang0 = startAngle * (M_PI/180.0);	// convert degrees to radians
	ang1 = endAngle * (M_PI/180.0);
	glBegin(GL_LINE_STRIP);		// tell OpenGL to draw connected lines.
	for(angle=ang0; angle <= ang1+NU_ANGLESTEP; angle+= NU_ANGLESTEP)
		{
			vectorX = xctr + radius*cos(angle);	// find a line endpoint
			vectorY = yctr + radius*sin(angle);
			glVertex2d(vectorX,vectorY);	// plot that line endpoint,
		}
	glEnd();						// Finished drawing connected lines.
	glFlush();						// Finish any pending drawing commands
} 

//-----------------------------------------------------------------------------
// DrawPieArc (xctr, yctr, radius, startAngle, endAngle)
//
// Draw a circular arc and fill the pie-slice-shaped region between the arc 
// and its center point with the current pen color.  Default fill color is black, 
// but the SetFillColor() function can change it.
// Draws the shape using filled triangles.
// x,y, radius are measured in pixels
// Angles are measured in degrees

void DrawPieArc(double xctr, double yctr, double radius, double startAngle, double endAngle)
{
	double vectorX1,vectorY1;		// vector to a point on circle from its center
	double vectorX0,vectorY0;		// previous version of vectorX1,Y1;
	double angle,ang0,ang1;			// Angle in radians from circle start point.

	ang0 = startAngle * (M_PI/180.0);	// convert degrees to radians
	ang1 = endAngle * (M_PI/180.0);
	glBegin(GL_TRIANGLES);		// Tell OpenGL to draw a series of triangles
								// Start at the circle's rightmost point.
	vectorX1 = xctr + radius*cos(ang0);	
	vectorY1 = yctr + radius*sin(ang0);
	for(angle=ang0 + NU_ANGLESTEP;// step through all other points on circle;
		angle < ang1 + NU_ANGLESTEP; angle += NU_ANGLESTEP)			
	{								// (add to ang1 to ensure arcs can close)
		vectorX0 = vectorX1;		// save previous point's position,
		vectorY0 = vectorY1;
		vectorX1= xctr + radius*cos(angle);	// find a new point on the circle,
		vectorY1= yctr + radius*sin(angle);		
		glVertex2d(xctr,yctr);		// plot the points of a triangle (CCW order)
		glVertex2d(vectorX0,vectorY0);	// center-->old pt-->new pt.
		glVertex2d(vectorX1,vectorY1);
	}
	glEnd();						// finished drawing triangles.
	glFlush();						// Finish any pending drawing commands


	vectorY1=yctr;					// Set starting point
	vectorX1=xctr;
}

//-----------------------------------------------------------------------------
// DrawText2D(font, x0, y0, pString)
//
//  Write the text string stored at 'pString' to the display screen using the
//  current pen color and the specified font.  
//  It places the lower left corner of the first line of text 
//  at position x0,y0.  
//	Example:  DrawText2D(helv18, 10,20,"Hello!");
//
// Available fonts:
// rom10 (TIMES_ROMAN size 10)
// rom24 (TIMES_ROMAN size 24)
// helv10 (HELVETICA size 10)
// helv12 (HELVETICA size 12)
// helv18 (HELVETICA size 18)

void DrawText2D(void * font, double x0, double y0, const char* pString) 		
{
	int i, imax;							// counter for characters.
	int		lines;							// counter for each 'newline' char.

	lines = 0;
	glRasterPos2d(x0, y0);			// set text's lower-left corner position
	imax = 1000;						// limit the number of chars we print.
	for(i=0; pString[i] != '\0'; i++)		// for each char,
	{									
		if (pString[i] == '\n')						// is it a new-line?
		{
			lines++;								// count it, and
			glRasterPos2d(x0, y0-(lines * 18.0));	// move down to next line
		}
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pString[i]);
	}
}

//----------------------------------------------------------------------------
// SetDottedLines()
//
//  Change pen to draw dotted lines instead of solid lines. All lines drawn 
//  after this call will be drawn dotted until you call the 'DrawSolidLines()' 
//  function.

void SetDottedLines(void)
{
	glLineStipple(4, 0xAAAA);		// Set OpenGL's fill pattern bits,
	glEnable(GL_LINE_STIPPLE);		// enable it,
	glFlush();						// and finish any pending drawing commands
}

//-----------------------------------------------------------------------------
// SetSolidLines()
//
//  Change the pen back to the default of drawing solid lines.  All lines drawn
//  after this call will be drawn solid until you call the 'DrawDottedLines()' 
//  function.

void SetSolidLines(void)
{
	glDisable(GL_LINE_STIPPLE);		// disable the fill pattern
	glFlush();
}

//-----------------------------------------------------------------------------
// SetLineWidth(wide)
//
//  Change the width (in pixels) of the lines drawn by the pen.  Default value is 3.0;
//  Careful! if you set 'wide' to zero, all lines drawn are invisible!

void SetLineWidth(double wide)
{
	glLineWidth((GLfloat)wide);		// (cast to OpenGL's float type)
}

//-----------------------------------------------------------------------------
// ClearWindow()
//
//  Calls OpenGL 'clear screen' function; fills the screen with background color.
//  You can change that by calling the SetBackgndColor() function

void ClearWindow(void)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
}


//-----------------------------------------------------------------------------
// SetBackgndColor(red, green, blue)
//
//  Sets the color used by 'ClearWindow()' function, and then fills the entire
// window with that color.

void SetBackgndColor(double red, double green, double blue)
{
		glClearColor(red, green, blue, 0.0);	// Set the background color
		glClear(GL_COLOR_BUFFER_BIT);			// Clear window using that color
}
	                                                        
//-----------------------------------------------------------------------------
// Pause(milliseconds)
//
//  Pause for 'milliseconds' seconds.  For best animation, pause should about
//  1/60th = 166 milliseconds or less between each frame. 	                                                        

void Pause(int milliseconds)
{                         // uses Win32's timing utilities:
#ifndef WIN32
  usleep((unsigned long) milliseconds);
#else  
  Sleep((DWORD)milliseconds);// cast to Win32's 'DWORD' (long int) type
#endif
}
 
//-----------------------------------------------------------------------------
// myIdle is a function that allows for animation. This function is called
// by the InitGraphics(); Thus, it should not be called any place else.

void myIdle (void)
{
	glutSwapBuffers();
	glutPostRedisplay();
} 
