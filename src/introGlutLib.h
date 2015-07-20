/******************************************************************************
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_

			Introductory Graphics Library using GLUT/OpenGL 
			Northwestern University CS110 'Intro to Programming'

11/10/2003: Created --Andrew William Proksel
11/17/2003: Formatted, put fcn bodies in .c files, fixed order dependence, 
			changed floats to doubles, fixed float calls to int GL fcns,
			commented function internals, added more drawing and animation 
			prototypes. --J. Tumblin
12/05/2003: Created function bodies for the prototypes, tested --A.W. Proksel
12/12/2003: Cleanup comments, revise disclaimer, added internal comments to
			all new functions measured angles from X axis, fixed sin/cos swap, 
			cleanup for consistent formatting, changed all floats to doubles 
			(again!), made #define for all angular steps in the .h file,
			re-test, (#define FINAL_GRADE==A) --J. Tumblin
04/12/2004:  Small clean-up by Prasun Choudhury. To be used in CS110 projects 
			for Spring 2004.
10/06/2004: Small clean-up. Removed SetFillColor() because it was 
			identical to SetPenColor() -- V. Doufexi
08/20/2005: Added mouse callback functions myMouse()
			Added font parameter to DrawText
			Added font #definitions to be used by DrawText() 
			--V. Doufexi
 ===============================================================================
  DISCLAIMER:
 introGlutLib.c and  introGlutLib.h originated as a student project written by
 Andrew William Proksel at Northwestern University, Evanston IL, as part of his
 classwork in the "Introduction to Computer Programming" course for non-majors 
 (CS110) taught by Jack Tumblin in Fall 2003.  It applies the wonderfully
  platform-independent 'GLUT' library written Mark Kilgard, ported to Win32 
 (Windows 95,98,Me,NT,2000,XP) by Nate Robins, and we thank them both for the
 great service their software provides freely to everyone; to obtain it, visit
	http://www.xmission.com/~nate/glut.html

  You are welcome to use and modify this code in any way you wish, but:
	1) Please include this disclaimer in any code that includes our work, and 
	please mention us in the credits or bibliography of published work
		(we're academics--publicity for our work is the best reward).
	2) You must hold us harmless for any use you make of this code. Said in  
	plain English, this means you cannot sue us or Northwestern University for 
	any mistakes we made in this software, no matter how you decide to use it 
	or change it, an no matter how badly you were hurt because of it.
	3) This code is provided 'as-is'. While we will gladly answer reasonable
	questions about it, you cannot expect us to maintain the code for you,
	nor can we act as a customer-service desk that helps you debug your code.
******************************************************************************/

/*  Microsoft Visual C++ Preparations:  
  1) Create a Win32 'console' project,
  2) Get these files: glut.h, glut.lib, and glut32.dll
  (from Blackboard, or from http://www.xmission.com/~nate/glut.html
					
  2) Move them to these directories:	
	glut.h			-- C:\Program Files\Microsoft Visual Studio\VC98\include
	glut.lib		-- C:\Program Files\Microsoft Visual Studio\VC98\lib
	glut32.dll		-- put this in C:\WINNT\system32

  3) Microsoft Visual C++  settings:
	 a)From the Visual C++ main menu, select "Project | Settings...",
	 b) Select "Link" tab from dialog box, and set Category to 'General' 
		(the default).
	 c) In the "Objects/Library" window you'll see other text; click on
		the text, scroll to the end of it, and add in these 3 libraries:
				opengl32.lib glut32.lib glu32.lib
		(exactly as shown here--without commas or quotes)
=============================================================================*/

/*The 'pragma' below will hide the 'console window' when the program runs.  */
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) 

// include the GLUT/OpenGL library files
#ifdef __APPLE__
	#include <OpenGL/GL.h>  // OpenGL 3D graphics rendering functions
	#include <OpenGL/Glu.h> // OpenGL utilties functions
	#include <GLUT/glut.h>  // GLUT functions for windows, keybd, mouse
#endif
#ifdef linux
	#include<GL/gl.h>
	#include<GL/glu.h>
	#include<GL/glut.h>
#endif

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>					// for printf(), scanf(), etc.
#include <math.h>					// for all math, some used in circle, etc
#include <time.h>					// read computers' clock for animation

#define NU_SCREENWIDTH		800		// Drawing window's width, height in pixels
#define NU_SCREENHEIGHT		500
#define NU_SCREEN_XPOS		100		// Drawing window position, in pixels.
#define NU_SCREEN_YPOS		150

#if !defined(M_PI)						// If we don't have PI defined already,
#define M_PI				3.1415926535897932384626433832795
#endif									// (PI from MS 'Calculator' app...)
#define NU_ANGLESTEP		M_PI/180.0	// draw curves in 1-degree increments

/* font options */
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

/*-----------------------------------------------------
Function Prototypes for startup/shutdown/housekeeping
------------------------------------------------------*/
void InitGraphics(void);
void CloseGraphics(void);
void setWindow(double left, double right, double bottom, double top);
void setViewport(int left, int right, int bottom, int top);

/*------------------------------------------------------------------------------
Function prototypes for GLUT callbacks.  A 'callback' is a function that YOU 
write, but you never call; instead, something else calls it, such as another 
program, the operating system, or a collection of ready-made functions.
The GLUT library uses callbacks.  Its functions take care of all the pesky 
details of putting your drawings in a proper on-screen Window, complete with
a frame, title bar, little buttons at the corner, and drag-able corners for 
resizing.  When it needs your program to re-draw the screen, it calls the
'myDisplay()' function. When GLUT has nothing more to do, it calls 'myIdle()'.
GLUT has other callbacks for when the user presses a key on the keyboard,
pushes down a mouse button, moves the mouse, or releases a mouse button, etc.,
we aren't using them here, but you're welcome to do so.  To find out how,
search the web for a GLUT tutorial.  You will need to 'register' the name
of your callback function, and make sure your function has the right arguments
and return type.
------------------------------------------------------------------------------*/
void myIdle(void);				// GLUT callback--do not call this yourself!;
void myDisplay(void);			// function prototype.
void myKeyboard(unsigned char key, int x, int y);	// for key-pressed function 
void myMouse(int button, int state, int x, int y);	// for mouse-click function

/*==============================================================================
		Function Prototypes for on-screen drawing and animation.
		WE WILL USE *ONLY* THESE FUNCTIONS for DRAWING on the SCREEN.
==============================================================================*/
//------------------------------------------------------------------------------
// DrawLine(x0, y0, x1, y1)
// 
// Draw a line from (x0,y0) and ending at (x1,y1). 
// (doesn't use relative addressing).
// x,y are measured in pixels.

void DrawLine(double x0, double y0, double x1, double y1);

//-----------------------------------------------------------------------------
// DrawBox (x0, y0, x1, y1)
// 
// Draw a box using the current pen color with lower left corner at (x0,y0)
// and upper right corner at x1, y1.
// x,y are measured in pixels.

void DrawBox(double x0, double y0, double x1, double y1);

//------------------------------------------------------------------------------
// DrawFillBox (x0, y0, x1, y1)
// 
// Draw a box and fill the region inside it using the current pen color. 
// You can specify the pen color with the SetPenColor() function.
// The default color is black.
// x,y are measured in pixels.

void DrawFillBox(double x0, double y0, double x1, double y1);


//------------------------------------------------------------------------------
// DrawFillTriangle(x0, y0, x1, y1, x2, y2)
//
// Draw a triangle and fill the region inside it using the current pen color
// Default fill color is black, but you can change it with the SetPenColor() function.
// x,y are measured in pixels.

void DrawFillTriangle(double x0, double y0, double x1, double y1, double x2, double y2);


//-----------------------------------------------------------------------------
// DrawEllipse(xctr, yctr, radiusX, radiusY)
//
// Draw the outline of an ellipse centered at (xctr,yctr), 
// with width  'radius_x'  and height 'radius_y' 
// using the current pen color and line width. Default pen color is black, but 
// the 'SetPenColor()' function can change it.
// Draws the ellipse as a sequence of very short,straight lines. 
// x,y, radius are measured in pixels.

void DrawEllipse(double xctr, double yctr, double radiusX, double radiusY);
void DrawFillEllipse(double xctr, double yctr, double radiusX, double radiusY);

//-----------------------------------------------------------------------------
// DrawCircle(xctr, yctr, radius)
//
// Draw the outline of a circle centered at (xctr,yctr) and width of 'radius'. 
// using the current pen color and line width. Default pen color is black, but 
// the 'SetPenColor()' function can change it.
// Draws the circle as a sequence of very short,straight lines. 
// x,y, radius are measured in pixels.

void DrawCircle(double xctr, double yctr, double radius);


//----------------------------------------------------------------------------
// DrawFillCircle(xctr, yctr, radius)
//
// Draw a circle and fill the region inside it using the the current pen color.  
// Default pen color is black, but the SetFillColor() function will change it.
// Draws the shape using filled triangles.
// x,y, radius are measured in pixels

void DrawFillCircle(double xctr, double yctr, double radius);


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

void DrawArc (double xctr, double yctr, double radius, 
			  double startAngle, double endAngle);


//-----------------------------------------------------------------------------
// DrawPieArc (xctr, yctr, radius, startAngle, endAngle)
//
// Draw a circular arc and fill the pie-slice-shaped region between the arc 
// and its center point with the current pen color.  Default fill color is black, 
// but the SetFillColor() function can change it.
// Draws the shape using filled triangles.
// x,y, radius are measured in pixels
// Angles are measured in degrees

void DrawPieArc(double xctr, double yctr, double radius, 
				  double startAngle, double endAngle);


//-----------------------------------------------------------------------------
// DrawText2D(font, x0, y0, pString)
//
//  Write the text string stored at 'pString' to the display screen using the
//  current pen color and the specified font.  
//  It places the lower left corner of the first line of text 
//  at position x0,y0.  
//	Example:  DrawText2D(helv18, 10,20,"Hello!");
//
// See #defined values at the top of this file for a list of available fonts


void DrawText2D(void *font, double x0, double y0, const char* pString);

//-----------------------------------------------------------------------------
// ClearWindow()
//
//  Calls OpenGL 'clear screen' function; fills the screen with background color.
//  You can change that by calling the SetBackgndColor() function

void ClearWindow(void);				

//------------------------------------------------------------------------------
// SetPenColor(red, green, blue)
//
// Set the drawing color. red, green, blue range from 0.0 to 1.0.
//   Commonly used colors:
//		(0.0,0.0,0.0) = black		(1.0,1.0,1.0) = white
//		(1.0,0.0,0.0) = red			(0.0,1.0,1.0) = cyan
//		(0.0,1.0,0.0) = green		(1.0,0.0,1.0) = magenta
//		(0.0,0.0,1.0) = blue		(1.0,1.0,0.0) = yellow
//		(0.5,0.5,0.5) = grey		(1.0,0.5,0.0) = orange
//		(0.5,0.0,0.5) = purple		(0.5,0.25,0.0) = brown
//		(0.0,0.25,0.0) = forest green
//		(0.0,0.0,0.25) = midnight blue
//
//   Try fractional amounts of red,green,blue, for other, more subtle colors.
//   You have 2^24 = 24 Million different color combinations available!	   */

void SetPenColor(double red, double green, double blue);


//-----------------------------------------------------------------------------
// SetBackgndColor(red, green, blue)
//
//  Sets the color used by 'ClearWindow()' function, and then fills the entire
// window with that color.

void SetBackgndColor(double red, double green, double blue);


//----------------------------------------------------------------------------
// SetDottedLines()
//
//  Change pen to draw dotted lines instead of solid lines. All lines drawn 
//  after this call will be drawn dotted until you call the 'DrawSolidLines()' 
//  function.

void SetDottedLines(void);


//-----------------------------------------------------------------------------
// SetSolidLines()
//
//  Change the pen back to the default of drawing solid lines.  All lines drawn
//  after this call will be drawn solid until you call the 'DrawDottedLines()' 
//  function.

void SetSolidLines(void);


//-----------------------------------------------------------------------------
// SetLineWidth(wide)
//
//  Change the width (in pixels) of the lines drawn by the pen.  Default value is 3.0;
//  Careful! if you set 'wide' to zero, all lines drawn are invisible!

void SetLineWidth(double wide);


//-----------------------------------------------------------------------------
// Pause(milliseconds)
//
//  Pause for 'milliseconds' seconds.  For best animation, pause should be about
//  1/60th = 166 milliseconds or less between each frame. 	

void Pause(int milliseconds);




