/* Kalonakis Xristos , AEM: 680
   11/12/2009
   This program demonstrates the use of the Glut library:
   -- It draws the board, the clock, the objects and the scores of the game "Bejeweled".
   -- It accepts and reacts to mouse input,
   -- It accepts and reacts to keyboard input.
*/


#include "introGlutLib.h"	        /* include the basic drawing library */
#include <time.h>		        /* for MS Windows time-finding functions */
#include <stdlib.h>		        /* for malloc and free */
#include <string.h>		        /* for strcpy and other string fcns. */


#define BUTTON_HEIGHT 35		/* the height of buttons */

#define QUIT_BUTTON_X 10		/* position of quit button */
#define QUIT_BUTTON_Y 18
#define QUIT_BUTTON_LENGTH 70	        /* length of quit button */
#define PAUSE_BUTTON_X 10               /* position of pause button */
#define PAUSE_BUTTON_Y 70
#define PAUSE_BUTTON_LENGTH 85          /* length of pause button */
#define START_BUTTON_X 608              /* position of start button */
#define START_BUTTON_Y 50
#define START_BUTTON_LENGTH 70          /* length of start button */
#define PLAY_AGAIN_BUTTON_X 580         /* position of play again button */
#define PLAY_AGAIN_BUTTON_Y 50
#define PLAY_AGAIN_BUTTON_LENGTH 120    /* length of play again button */
#define SAVE_BUTTON_X 10                /* position of save button */
#define SAVE_BUTTON_Y 174
#define SAVE_BUTTON_LENGTH 70           /* length of save button */
#define LOAD_BUTTON_X 10                /* position of load button */
#define LOAD_BUTTON_Y 122
#define LOAD_BUTTON_LENGTH 70           /* length of load button */
#define HIGH_SCORES_BUTTON_X 10         /* position of high score button */
#define HIGH_SCORES_BUTTON_Y 226
#define HIGH_SCORES_BUTTON_LENGTH 90    /* length of high score button */
#define REGISTER_HIGH_SCORE_BUTTON_X 520   /* position of register high score button */
#define REGISTER_HIGH_SCORE_BUTTON_Y 50
#define REGISTER_HIGH_SCORE_BUTTON_LENGTH 240    /* length of register high score button */
#define BACK_TO_THE_GAME_BUTTON_X 520    /* position of back to the game button */
#define BACK_TO_THE_GAME_BUTTON_Y 50
#define BACK_TO_THE_GAME_BUTTON_LENGTH 212    /* length of back to the game button */
#define SIZE 8



int bejeweled_board[SIZE][SIZE];
int score;
char sscore[7];
char slevel[4];
char sscore_to_next_level[7];
char high_scores_score[7];
char high_scores_level[4];
int *click1_click2;
int time_elapsed;
int level;
int level_score;
int level_boost;
int direction;
char qwerty[4][10];
char shift_querty_numbers[10];
char *name;
int spot;
FILE *pFile;

/* The states of the game */
typedef enum {START, PLAY, PAUSE, ANIMATE, GAMEOVER, REGISTER_HIGH_SCORE, HIGH_SCORES} STATE_OF_GAME;
STATE_OF_GAME state_of_game, temp_state_of_game;

/* The number of mouse click */
typedef enum {CLICK0, CLICK1, CLICK2} MOUSE_CLICKS;
MOUSE_CLICKS mouse_clicks;

/* The state of pause button */
typedef enum {PAUSE0, PAUSE1, PAUSE2} PAUSE_PRESSED;
PAUSE_PRESSED pause_pressed;

/* The stages of gameover */
typedef enum {GAMEOVER0, GAMEOVER1} STAGE_GAMEOVER;
STAGE_GAMEOVER stage_gameover;

/* The state of capslock button (not the keyboard's) */
typedef enum {CAPSLOCK0, CAPSLOCK1} CAPSLOCK_PRESSED;
CAPSLOCK_PRESSED capslock_pressed;

/* The state of shift button (not the keyboard's) */
typedef enum {SHIFT0, SHIFT1} SHIFT_PRESSED;
SHIFT_PRESSED shift_pressed;

/* The stage of registration */
typedef enum {NOT_REGISTERED, REGISTERED} REGISTRATION_STAGE;
REGISTRATION_STAGE registration_stage;

/* The struct COLORS, defining the objects */
typedef struct {
    double red;
    double green;
    double blue;
} COLORS;
COLORS colors[6];

/* The contains of a node */
typedef struct LIST {
    int level_high_score;
    int score_high_score;
    char name_high_score[34];
    struct LIST *next;
} list;
list *root;



/* ========================== Function prototypes:============================ */
void myDisplay();			/* the GLUT library will call 'myDisplay' when  it wants to re-draw the window. */
void myMouse(int button, int state, int x, int y);    /* the glut library will call 'myMouse' when a mouse button is pressed */
void myKeyboard(unsigned char key, int x, int y);     /* the glut library will call 'myKeyboard when a keyboard button is pressed */
void DrawQuitButton();                  /* a function used to draw the quit button */
void DrawPauseResumeButton();           /* a function used to draw the pause/resume button */
void DrawStartButton();                 /* a function used to draw the start button */
void DrawPlayAgainButton();             /* a function used to draw the play again button */
void DrawSaveButton();                  /* a function used to draw the save button */
void DrawLoadButton();                  /* a function used to draw the load button */
void DrawHighScoresButton();            /* a function used to draw the high scores button */
void DrawRegisterHighScoreButton();     /* a function used to draw the register high scores button */
void DrawBackToTheGameButton();         /* a function used to draw the back to the game button */
void InitializeObjects();               /* a function used to initialize all the values */
void addObjectsOnTheBejeweledBoard(int i, int j);   /* a function used to randomize the game board */
void DrawBejeweledBoard();              /* a function used to draw the game board, the level, the score to next level and the current score */
void addGraphicalObjects(int i, int j); /* a function used to call the function animate0bject(...) with directioni,j params equals to 0 and the param object equals to the current object in the i,j cell */
void animateObject(int i, int j, int directioni, int directionj, int object);   /* a function used to draw an object according to its cell, placement, number */
void ObjectClock();                     /* a function used to draw the game clock */
void drawQwertyKeyboard();              /* a function used to draw a qwerty keyboard setup in register high scores state */
void drawHighScores();                  /* a function used to draw the high scores of the game */
void swap();                            /* a function used to swap the current clicked objects according to the game rules */
int verifyMove(int i, int j, int destinationi, int destinationj);   /* a function used to define the game rules, returning 1 for correct choise, 0 for wrong */
void clearSameOjects();                 /* a function used to clear at least 3, or more, same objects on a same row/column */
void lookForSpaces();                   /* a function used to check for empty slots on the board after the call of clearSameObjects(...) */
void drawSelected();                    /* a function used to draw our current chosen objects */
void wait(double seconds);              /* a function used to add a delay */
void gameover();                        /* a function used to iniatialize all the values again after a game loss */
void levelUp();                         /* a function used to increase the values of level/score to next level after a certain score */
void paused();                          /* a function used to pause the game */
void addToString(char qwerty, int spot);/* a function used to add characters to the dynamic defined string, name[] */
void listInsertInitialization();        /* a function used to add a list node on top of the list */
void listInsertNewScore();              /* a function used to make a sorted list according to the field score */
void clearList();                       /* a function used to empty a list and free its allocated memory */
int checkScore();                       /* a function used specify the placement of a new node into a sorted list */




/* =================================Main:======================================= */				
int main() {
  
    srand ( time(NULL) );
    state_of_game = START;
    
    root = NULL;
    listInsertInitialization();
    listInsertInitialization();
    listInsertInitialization();
    listInsertInitialization();
    listInsertInitialization();         /* add 5 nodes in a list */
    
    name = (char *) malloc(sizeof(char));
    if(name == NULL) {
        printf("Not enough memory.\n");
        exit(0);
    }
    name[0] = '\0';
    
    InitializeObjects();                /* ininialize the game values */

    InitGraphics();			/* initialize GLUT/OpenGL */
    glutMainLoop();			/* keep drawing */
		
  return 0;
}



/**********************************************************************
 myMouse(button, state, x, y)

 GLUT CALLBACK: Don't call this function in your program--GLUT does it.

 button: Which button was clicked. 
			Possible values are GLUT_LEFT for the left button and GLUT_RIGHT for the right one
 state: Is the button clicked (GLUT_DOWN) or not (GLUT_UP)?
 x, y: The coordinates of the place where you clicked (in pixels)
***********************************************************************/
			
void myMouse(int button, int state, int x, int y) {
	
	list *current;
	int new_y = NU_SCREENHEIGHT-y, i, j;                            /* Even thought the normal output window has 
	                                                                   the origin point in the lower left corner, 
								           the mouse handling function assumes that
									   it's the upper left corner. 
									   So we change y to keep things consistent. */

	
	if (state == GLUT_DOWN) {
		
		/* A button is being pressed. Set the correct motion function */
		
		if (button==GLUT_LEFT) {
			
		     /* check if the quit button was pressed, and if so, exit. */
		     if(state_of_game == START || state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE || state_of_game == GAMEOVER) {
			if(x <= QUIT_BUTTON_X + QUIT_BUTTON_LENGTH  && x >= QUIT_BUTTON_X 
				&& new_y <= QUIT_BUTTON_Y + BUTTON_HEIGHT && new_y >= QUIT_BUTTON_Y) { 
			        free(click1_click2);
			        free(name);
			        clearList();			
				exit(1);
			}
		     }
		     
		     /* check if the pause button was pressed, and if so, pause or resume the game. */
		     if(state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE) {
			if(x <= PAUSE_BUTTON_X + PAUSE_BUTTON_LENGTH  && x >= PAUSE_BUTTON_X 
				&& new_y <= PAUSE_BUTTON_Y + BUTTON_HEIGHT && new_y >= PAUSE_BUTTON_Y) { 
				pause_pressed++;
			}
		     }
		     
		     /* check if the start button was pressed, and if so, start playing. */
		     if(state_of_game == START) {	
			if(x <= START_BUTTON_X + START_BUTTON_LENGTH  && x >= START_BUTTON_X 
				&& new_y <= START_BUTTON_Y + BUTTON_HEIGHT && new_y >= START_BUTTON_Y) { 
			        state_of_game = PLAY;
			}
		     }
		     
		     /* check if the play again button was pressed, and if so, restart. (case: no new score to be imported) */
		     if(state_of_game == GAMEOVER && checkScore() == 7) {
		       if(x <= PLAY_AGAIN_BUTTON_X + PLAY_AGAIN_BUTTON_LENGTH  && x >= PLAY_AGAIN_BUTTON_X 
				&& new_y <= PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT && new_y >= PLAY_AGAIN_BUTTON_Y) { 
				stage_gameover++;
			}
		     }
		     
		     /* check if the play again button was pressed, and if so, restart. (case: a new score got imported) */
		     if(state_of_game == GAMEOVER && checkScore() != 7 && registration_stage == REGISTERED) {
		       if(x <= PLAY_AGAIN_BUTTON_X + PLAY_AGAIN_BUTTON_LENGTH  && x >= PLAY_AGAIN_BUTTON_X 
				&& new_y <= PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT && new_y >= PLAY_AGAIN_BUTTON_Y) { 
				stage_gameover++;
			}
		     }
		     
		     /* check if the back to the game button was pressed, and if so, return to the game. (case: start) */
		     if(state_of_game == HIGH_SCORES && temp_state_of_game == START) {
		       if(x <= BACK_TO_THE_GAME_BUTTON_X + BACK_TO_THE_GAME_BUTTON_LENGTH  && x >= BACK_TO_THE_GAME_BUTTON_X 
				&& new_y <= BACK_TO_THE_GAME_BUTTON_Y + BUTTON_HEIGHT && new_y >= BACK_TO_THE_GAME_BUTTON_Y) { 
		                state_of_game = START;
			}
		     }
		     
		     /* check if the high scores button was pressed, and if so, display the high scores. */
		     if(state_of_game == START) {
		       if(x <= HIGH_SCORES_BUTTON_X + HIGH_SCORES_BUTTON_LENGTH  && x >= HIGH_SCORES_BUTTON_X 
				&& new_y <= HIGH_SCORES_BUTTON_Y + BUTTON_HEIGHT * 2 && new_y >= HIGH_SCORES_BUTTON_Y) {
			        temp_state_of_game = START;
			        state_of_game = HIGH_SCORES;
		       }
		     }
		     
		     /* check if the back to the game button was pressed, and if so, return to the game. (case: gameover) */
		     if(state_of_game == HIGH_SCORES && temp_state_of_game == GAMEOVER) {
		       if(x <= BACK_TO_THE_GAME_BUTTON_X + BACK_TO_THE_GAME_BUTTON_LENGTH  && x >= BACK_TO_THE_GAME_BUTTON_X 
				&& new_y <= BACK_TO_THE_GAME_BUTTON_Y + BUTTON_HEIGHT && new_y >= BACK_TO_THE_GAME_BUTTON_Y) { 
		                time_elapsed = 325;
		                registration_stage = REGISTERED;
			}
		     }
		     
		     /* check if the save button was pressed, and if so, save. */
		     if(state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE) {
		        if(x <= SAVE_BUTTON_X + SAVE_BUTTON_LENGTH  && x >= SAVE_BUTTON_X 
				&& new_y <= SAVE_BUTTON_Y + BUTTON_HEIGHT && new_y >= SAVE_BUTTON_Y) { 
			        pFile = fopen("bejeweled_save.txt","w");
	                        for(i = 0; i < SIZE; i++) {
                                    for(j = 0; j < SIZE; j++) {
                                        fprintf(pFile, "%d", bejeweled_board[i][j]);
		                        fprintf(pFile, " ");
                                    }
	                            fprintf(pFile, "\n");
	                        }
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "%d", score);
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "%d", level);
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "%d", level_score);
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "%d", level_boost);
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "\n");
	                        fprintf(pFile, "%d", time_elapsed);
	                        fclose(pFile);
				free(click1_click2);
				click1_click2 = NULL;
				mouse_clicks = CLICK0;
				state_of_game = START;
				direction = 0;
				spot = 0;
                                capslock_pressed = CAPSLOCK0;
                                shift_pressed = SHIFT0;
                                registration_stage = NOT_REGISTERED;
				pause_pressed = PAUSE0;
				stage_gameover = GAMEOVER0;  
			}
		     }
		     
		/* check if the load button was pressed, and if so, load. */
		if(state_of_game == START || state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE || state_of_game == GAMEOVER) {
		  if(fopen("bejeweled_save.txt","r") != NULL) {
		     if(x <= LOAD_BUTTON_X + LOAD_BUTTON_LENGTH  && x >= LOAD_BUTTON_X 
				&& new_y <= LOAD_BUTTON_Y + BUTTON_HEIGHT && new_y >= LOAD_BUTTON_Y) { 
		                pFile = fopen("bejeweled_save.txt","r");
	                        for(i = 0; i < SIZE; i++) {
                                    for(j = 0; j < SIZE; j++) {
                                        fscanf(pFile, "%d", &bejeweled_board[i][j]);
                                    }
	                        }
                                fscanf(pFile, "%d", &score);
	                        fscanf(pFile, "%d", &level);
	                        fscanf(pFile, "%d", &level_score);
	                        fscanf(pFile, "%d", &level_boost);
	                        fscanf(pFile, "%d", &time_elapsed);
	                        fclose (pFile);
				free(click1_click2);
				click1_click2 = NULL;
				mouse_clicks = CLICK0;
				state_of_game = START;
				direction = 0;
				spot = 0;
				capslock_pressed = CAPSLOCK0;
				shift_pressed = SHIFT0;
				registration_stage = NOT_REGISTERED;
				pause_pressed = PAUSE0;
				stage_gameover = GAMEOVER0;
		     }
		  }
		}
		
		     /* check if the two objects were chosen, and if so, call swap(). */
		     if(pause_pressed == PAUSE0 && state_of_game == PLAY) {
		       if(mouse_clicks == CLICK1) {
			for(i = 0; i <= 7; i++) {
			    for(j = 0; j <= 7; j++){
			        if(x <= 110 + 47 * (j + 1) && x >= 110 + 47 * j
			        && new_y <= 69 + 8 * 47 - 47 * i && new_y >= 69 + 7 * 47 - 47 * i) {  
				    click1_click2 = (int *) realloc(click1_click2, 4 * sizeof(int));
				    if(click1_click2 == NULL) {
				        printf("Not enough memory.\n");
				        exit(0);
				    }
				    click1_click2[2] = i;
				    click1_click2[3] = j;
				    mouse_clicks = CLICK2;
			        }
			    }
			}
		       }
		       
		       if(mouse_clicks == CLICK0) {
			for(i = 0; i <= 7; i++) {
			    for(j = 0; j <= 7; j++){
			        if(x <= 110 + 47 * (j + 1) && x >= 110 + 47 * j
			        && new_y <= 69 + 8 * 47 - 47 * i && new_y >= 69 + 7 * 47 - 47 * i) {
				    if(click1_click2 != NULL) {
				        free(click1_click2);
	                                click1_click2 = NULL;
				    }
				    click1_click2 = (int *) malloc(2 * sizeof(int));
				    if(click1_click2 == NULL) {
				        printf("Not enough memory.\n");
				        exit(0);
				    }
				    click1_click2[0] = i;
				    click1_click2[1] = j;
				    mouse_clicks = CLICK1;
			        }
			    }
			}
		       }
			
		       if(mouse_clicks == CLICK2) {
			    swap();
			    mouse_clicks = CLICK0;
		       }
		    }
		    
		    /* check if any qwerty keyboard designed button was pressed, and if so, add a character to the string or proceed to high scores. */
		    if(state_of_game == REGISTER_HIGH_SCORE) {
		       if(shift_pressed == SHIFT0) {
			for(i = 0; i <= 3; i++) {
			    for(j = 0; j <= 9; j++){
			        if(x <= 155 + 47 * (j + 1) && x >= 155 + 47 * j
			        && new_y <= 69 + 4 * 47 - 47 * i && new_y >= 69 + 3 * 47 - 47 * i) {
				    if(spot < 33) {
				        addToString(qwerty[i][j],spot);
				        spot++;
				    }
			        }
			    }
			}
		       }
		       
			if(shift_pressed == SHIFT1) {
			    for(j = 0; j <= 9; j++){
			        if(x <= 155 + 47 * (j + 1) && x >= 155 + 47 * j
			        && new_y <= 69 + 4 * 47 && new_y >= 69 + 3 * 47) {
				    if(spot < 33) {
				        addToString(shift_querty_numbers[j],spot);
				        spot++;
				    }
				}
			    }
			    if(x <= 155 + 47 * 8 && x >= 155 + 47 * 7
			        && new_y <= 69 + 47 && new_y >= 69) {
				    if(spot < 33) {
				        addToString('<', spot);
				        spot++;
				    }
			    }
			    if(x <= 155 + 47 * 9 && x >= 155 + 47 * 8
			        && new_y <= 69 + 47 && new_y >= 69) {
				    if(spot < 33) {
				        addToString('>', spot);
				        spot++;
				    }
			    }
			    if(x <= 155 + 47 * 10 && x >= 155 + 47 * 9
			        && new_y <= 69 + 47 && new_y >= 69) {
				    if(spot < 33) {
				        addToString('?', spot);
				        spot++;
				    }
			    }
			    if(x <= 155 + 47 * 10 && x >= 155 + 47 * 9
			        && new_y <= 69 + 2 * 47 && new_y >= 69 + 47) {
				    if(spot < 33) {
				        addToString(':', spot);
				        spot++;
				    }
			    }
			}
			
			if(x <= 155 + 47 * 8 && x >= 155 + 47 * 2
			        && new_y <= 68 && new_y >= 68 - 47) {
			    if(spot < 33) {
			        addToString(31, spot);
			        spot++;
			    }
			}
			
                        if(x <= 155 + 47 * 11 && x >= 155 + 47 * 10
			        && new_y <= 68 + 47 * 2 && new_y >= 68 + 47) {
			    if(spot < 33) {
			        if(shift_pressed == SHIFT0) {
			            addToString(39, spot);
			            spot++;
				}
				else {
				    addToString('"', spot);
			            spot++;
				}
			    }
			}
			
			if(x <= 155 + 47 * 11 && x >= 155 + 47 * 10
			        && new_y <= 68 + 47 * 4 && new_y >= 68 + 47 * 3) {
			    if(spot < 33) {
			        if(shift_pressed == SHIFT0) {
			            addToString('-', spot);
			            spot++;
				}
				else {
				    addToString('_', spot);
			            spot++;
				} 
			    }
			}
                        
			if(x <= 155 + 47 * 13 && x >= 155 + 47 * 11
			        && new_y <= 68 + 47 * 2 && new_y >= 68 + 47) {
			    if(name[0] != '\0') {
			        listInsertNewScore();
				state_of_game = HIGH_SCORES;
				
				pFile = fopen("high_scores.txt","w");
	                        for(current = root; current != NULL; current = current -> next) {
	                            fprintf(pFile, "%d", current -> level_high_score);
	                            fprintf(pFile, "\n");
	                            fprintf(pFile, "%d", current -> score_high_score);
	                            fprintf(pFile, "\n");
	                            fprintf(pFile, "%s", current -> name_high_score);
	                            fprintf(pFile, "\n");
	                            fprintf(pFile, "\n");  
				}
				fclose (pFile);
				
				name = (char *) realloc(name, sizeof(char));
				if(name == NULL) {
				    printf("Not enough memory.\n");
				    exit(0);
				}
			        name[0] = '\0';
			        spot = 0;
			    }
			}
		
			
			if(x <= 155 + 47 * 11 && x >= 155 + 47 * 10
			        && new_y <= 68 + 47 * 3 && new_y >= 68 + 47 * 2) {
			    if(spot < 33) {
			        if(shift_pressed == SHIFT0) {
			            addToString('[', spot);
			            spot++;
				}
				else {
				    addToString('{', spot);
			            spot++;
				} 
			    }
			}
			
			if(x <= 155 + 47 * 12 && x >= 155 + 47 * 11
			        && new_y <= 68 + 47 * 3 && new_y >= 68 + 47 * 2) {
			    if(spot < 33) {
			        if(shift_pressed == SHIFT0) {
			            addToString(']', spot);
			            spot++;
				}
				else {
				    addToString('}', spot);
			            spot++;
				}
			    }
			}
			
			if(x <= 155 + 47 * 13 && x >= 155 + 47 * 12
			        && new_y <= 68 + 47 * 3 && new_y >= 68 + 47 * 2) {
			    if(spot < 33) {
			        if(shift_pressed == SHIFT0) {
			            addToString(92, spot);
			            spot++;
				}
				else {
				    addToString('|', spot);
			            spot++;
				}
			    }
			}
			
			if((x <= 153 && x >= 153 - 40 * 2
			        && new_y <= 68 + 47 && new_y >= 68) || (x <= 155 + 47 * 12 && x >= 155 + 47 * 10 && new_y <= 68 + 47 && new_y >= 68)) {
			        if(shift_pressed == SHIFT0) {
				    shift_pressed = SHIFT1;
				}
				else {
				    shift_pressed = SHIFT0;
				}
			}
			
			if(x <= 153 && x >= 153 - 40 * 2
			        && new_y <= 68 + 47 * 2 && new_y >= 68 + 47) {
			     if(capslock_pressed == CAPSLOCK0) {
			         capslock_pressed = CAPSLOCK1;
			     }
			     else {
			         capslock_pressed = CAPSLOCK0;
			     }
			}
			
			
			if(x <= 155 + 47 * 12 && x >= 155 + 47 * 11
			        && new_y <= 68 + 47 * 4 && new_y >= 68 + 47 * 3) {
			    if(spot < 33) {
			        if(shift_pressed == SHIFT0) {
			            addToString('=', spot);
			            spot++;
				}
				else {
				    addToString('+', spot);
			            spot++;
				}
			    }
			}
			
			if(x <= 153 && x >= 153 - 47
			        && new_y <= 68 + 47 * 4 && new_y >= 68 + 47 * 3) {
			    if(spot < 33) {
			        if(shift_pressed == SHIFT0) {
			            addToString('`', spot);
			            spot++;
				}
				else {
				    addToString('~', spot);
			            spot++;
				}
			    }
			}
			
			if(x <= 155 + 47 * 13 && x >= 155 + 47 * 12
			        && new_y <= 68 + 47 * 4 && new_y >= 68 + 47 * 3) {
			    if(spot > 0) {
			        spot--;
			        addToString('\0', spot);
			    }
			}
		     }
		     
		     /* check if the register high score button was pressed, and if so, proceed on registering the score. */
		     if(state_of_game == GAMEOVER && checkScore() != 7 && registration_stage == NOT_REGISTERED) {
		       if(x <= REGISTER_HIGH_SCORE_BUTTON_X + REGISTER_HIGH_SCORE_BUTTON_LENGTH  && x >= REGISTER_HIGH_SCORE_BUTTON_X 
				&& new_y <= REGISTER_HIGH_SCORE_BUTTON_Y + BUTTON_HEIGHT && new_y >= REGISTER_HIGH_SCORE_BUTTON_Y) { 
				state_of_game = REGISTER_HIGH_SCORE;
		                temp_state_of_game = GAMEOVER;
		                time_elapsed = 326;
			}
		     }
		}
	}
	
} 


/**********************************************************************
 myKeyboard(key, x, y)

 GLUT CALLBACK: Don't call this function in your program--GLUT does it.
***********************************************************************/

void myKeyboard(unsigned char key, int x, int y) {
  int i, j;
  list *current;
  
	switch(key)  {
	case 27:	
	case 'q':	
	case 'Q':
	  /* check if Escape or q,Q button was pressed, and if so, exit. */
	  if(state_of_game == START || state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE || state_of_game == GAMEOVER) {
	      free(click1_click2);
	      free(name);
	      clearList();
	      exit(1);
	  }
	  break;
	case 'p':
	case 'P':
	  /* check if p,P button was pressed, and if so, pause/resume. */
	  if(state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE) {
	      pause_pressed++;
	  }
	  break;
	case 's':
	case 'S':
	  /* check if s,S button was pressed, and if so, save. */
	  if(state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE) {
	      pFile = fopen("bejeweled_save.txt","w");
	      for(i = 0; i < SIZE; i++) {
                  for(j = 0; j < SIZE; j++) {
                      fprintf(pFile, "%d", bejeweled_board[i][j]);
		      fprintf(pFile, " ");
                  }
	          fprintf(pFile, "\n");
	      }
	      fprintf(pFile, "\n");
	      fprintf(pFile, "%d", score);
	      fprintf(pFile, "\n");
	      fprintf(pFile, "\n");
	      fprintf(pFile, "%d", level);
	      fprintf(pFile, "\n");
	      fprintf(pFile, "\n");
	      fprintf(pFile, "%d", level_score);
	      fprintf(pFile, "\n");
	      fprintf(pFile, "\n");
	      fprintf(pFile, "%d", level_boost);
	      fprintf(pFile, "\n");
	      fprintf(pFile, "\n");
	      fprintf(pFile, "%d", time_elapsed);
	      fclose (pFile);
	      free(click1_click2);
	      click1_click2 = NULL;
	      mouse_clicks = CLICK0;
	      state_of_game = START;
	      direction = 0;
	      spot = 0;
	      capslock_pressed = CAPSLOCK0;
	      shift_pressed = SHIFT0;
	      registration_stage = NOT_REGISTERED;
	      pause_pressed = PAUSE0;
	      stage_gameover = GAMEOVER0;
	  }
	  break;
	case 'l':
	case 'L':
	  /* check if l,L button was pressed, and if so, load. */
	 if(state_of_game == START || state_of_game == PLAY || state_of_game == PAUSE || state_of_game == ANIMATE || state_of_game == GAMEOVER) {
	     if(fopen("bejeweled_save.txt","r") != NULL) {
	         pFile = fopen("bejeweled_save.txt","r");
	         for(i = 0; i < SIZE; i++) {
                     for(j = 0; j < SIZE; j++) {
                         fscanf(pFile, "%d", &bejeweled_board[i][j]);
                     }
	         }
                 fscanf(pFile, "%d", &score);
	         fscanf(pFile, "%d", &level);
	         fscanf(pFile, "%d", &level_score);
	         fscanf(pFile, "%d", &level_boost);
	         fscanf(pFile, "%d", &time_elapsed);
	         fclose (pFile);
	         free(click1_click2);
	         click1_click2 = NULL;
	         mouse_clicks = CLICK0;
	         state_of_game = START;
	         direction = 0;
		 spot = 0;
		 capslock_pressed = CAPSLOCK0;
		 shift_pressed = SHIFT0;
		 registration_stage = NOT_REGISTERED;
	         pause_pressed = PAUSE0;
	         stage_gameover = GAMEOVER0;  
	     }
	 }
	 break;
	case 'h':
	case 'H':
	 /* check if h,H button was pressed, and if so, display the high scores. */
	 if(state_of_game == START) {
	     temp_state_of_game = START;
	     state_of_game = HIGH_SCORES;
	 }
	 break;
	}
	
      if(state_of_game == REGISTER_HIGH_SCORE) {
      /* check if any of the querty keyboard button was pressed, and if so, add the character to the string or proceed to high scores. */
	switch(key)  {
	  case '`':
	    if(spot < 33) {
	        addToString('`', spot);
	        spot++;
	    }
	    break;
	  case '~':
	    if(spot < 33) {
	        addToString('~', spot);
	        spot++;
	    }
	    break;
	  case '1':
	    if(spot < 33) {
	        addToString('1', spot);
	        spot++;
	    }
	    break;
	  case '!':
	    if(spot < 33) {
	        addToString('!', spot);
	        spot++;
	    }
	    break;
	  case '2':
	    if(spot < 33) {
	        addToString('2', spot);
	        spot++;
	    }
	    break;
	  case '@':
	    if(spot < 33) {
	        addToString('@', spot);
	        spot++;
	    }
	    break;
	  case '3':
	    if(spot < 33) {
	        addToString('3', spot);
	        spot++;
	    }
	    break;
	  case '#':
	    if(spot < 33) {
	        addToString('#', spot);
	        spot++;
	    }
	    break;
	  case '4':
	    if(spot < 33) {
	        addToString('4', spot);
	        spot++;
	    }
	    break;
	  case '$':
	    if(spot < 33) {
	        addToString('$', spot);
	        spot++;
	    }
	    break;
	  case '5':
	    if(spot < 33) {
	        addToString('5', spot);
	        spot++;
	    }
	    break;
	  case '%':
	    if(spot < 33) {
	        addToString('%', spot);
	        spot++;
	    }
	    break;
	  case '6':
	    if(spot < 33) {
	        addToString('6', spot);
	        spot++;
	    }
	    break;
	  case '^':
	    if(spot < 33) {
	        addToString('^', spot);
	        spot++;
	    }
	    break;
	  case '7':
	    if(spot < 33) {
	        addToString('7', spot);
	        spot++;
	    }
	    break;
	  case '&':
	    if(spot < 33) {
	        addToString('&', spot);
	        spot++;
	    }
	    break;
	  case '8':
	    if(spot < 33) {
	        addToString('8', spot);
	        spot++;
	    }
	    break;
	  case '*':
	    if(spot < 33) {
	        addToString('*', spot);
	        spot++;
	    }
	    break;
	  case '9':
	    if(spot < 33) {
	        addToString('9', spot);
	        spot++;
	    }
	    break;
	  case '(':
	    if(spot < 33) {
	        addToString('(', spot);
	        spot++;
	    }
	    break;
	  case '0':
	    if(spot < 33) {
	        addToString('0', spot);
	        spot++;
	    }
	    break;
	  case ')':
	    if(spot < 33) {
	        addToString(')', spot);
	        spot++;
	    }
	    break;
	  case '-':
	    if(spot < 33) {
	        addToString('-', spot);
	        spot++;
	    }
	    break;
	  case '_':
	    if(spot < 33) {
	        addToString('_', spot);
	        spot++;
	    }
	    break;
	  case '=':
	    if(spot < 33) {
	       addToString('=', spot);
	       spot++;
	    }
	    break;
	  case '+':
	    if(spot < 33) {
	        addToString('+', spot);
	        spot++;
	    }
	    break;
	  case 8:
	    if(spot > 0) {
	        spot--;
		addToString('\0', spot);
	    }
	    break;
	  case 'Q':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('Q', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('Q', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'q':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('q', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('q', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'W':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('W', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('W', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'w':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('w', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('w', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'E':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('E', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('E', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'e':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('e', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('e', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'R':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('R', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('R', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'r':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('r', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('r', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'T':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('T', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('T', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 't':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('t', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('t', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'Y':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('Y', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('Y', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'y':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('y', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('y', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'U':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('U', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('U', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'u':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('u', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('u', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'I':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('I', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('I', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'i':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('i', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('i', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'O':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('O', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('O', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'o':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('o', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('o', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'P':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('P', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('P', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'p':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('p', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('p', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case '[':
	    if(spot < 33) {
	        addToString('[', spot);
	        spot++;
	    }
	    break;
	  case '{':
	    if(spot < 33) {
	        addToString('{', spot);
	        spot++;
	    }
	    break;
	  case ']':
	    if(spot < 33) {
	        addToString(']', spot);
	        spot++;
	    }
	  case '}':
	    if(spot < 33) {
	        addToString('}', spot);
	        spot++;
	    }
	    break;
	  case 92:
	    if(spot < 33) {
	        addToString(92, spot);
	        spot++;
	    }
	    break;
	  case '|':
	    if(spot < 33) {
	        addToString('|', spot);
	        spot++;
	    }
	    break;
	  case 'A':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('A', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('A', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'a':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('a', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('a', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'S':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('S', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('S', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 's':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('s', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('s', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'D':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('D', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('D', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'd':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('d', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('d', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'F':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('F', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('F', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'f':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('f', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('f', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'G':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('G', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('G', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'g':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('g', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('g', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'H':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('H', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('H', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'h':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('h', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('h', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'J':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('J', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('J', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'j':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('j', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('j', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'K':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('K', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('K', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'k':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('k', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('k', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'L':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('L', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('L', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'l':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('l', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('l', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case ';':
	    if(spot < 33) {
	       addToString(';', spot);
	       spot++;
	    }
	    break;
	  case ':':
	    if(spot < 33) {
	        addToString(':', spot);
	        spot++;
	    }
	    break;
	  case 39:
	    if(spot < 33) {
	       addToString(39, spot);
	       spot++;
	    }
	    break;
	  case '"':
	    if(spot < 33) {
	        addToString('"', spot);
	        spot++;
	    }
	    break;
	  case 13:
	    if(name[0] != '\0') {
	        listInsertNewScore();
		state_of_game = HIGH_SCORES;
				
		pFile = fopen("high_scores.txt","w");
                for(current = root; current != NULL; current = current -> next) {
		    fprintf(pFile, "%d", current -> level_high_score);
                    fprintf(pFile, "\n");
                    fprintf(pFile, "%d", current -> score_high_score);
		    fprintf(pFile, "\n");
		    fprintf(pFile, "%s", current -> name_high_score);
		    fprintf(pFile, "\n");
		    fprintf(pFile, "\n");  
		}
		fclose (pFile);
		
		name = (char *) realloc(name, sizeof(char));
		if(name == NULL) {
		    printf("Not enough memory.\n");
		    exit(0);
		}
                name[0] = '\0';
		spot = 0;
	    }
	    break;
	  case 'Z':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('Z', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('Z', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'z':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('z', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('z', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'X':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('X', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('X', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'x':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('x', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('x', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'C':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('C', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('C', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'c':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('c', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('c', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'V':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('V', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('V', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'v':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('v', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('v', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'B':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('B', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('B', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'b':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('b', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('b', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'N':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('N', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('N', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'n':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('n', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('n', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'M':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('M', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('M', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case 'm':
	    if(spot < 33) {
	        if(capslock_pressed == CAPSLOCK1) {
	            addToString('m', spot);
	            spot++;
		}
		else {
		    capslock_pressed = CAPSLOCK1;
		    addToString('m', spot);
	            spot++;
		    capslock_pressed = CAPSLOCK0;
		}
	    }
	    break;
	  case ',':
	    if(spot < 33) {
	       addToString(',', spot);
	       spot++;
	    }
	    break;
	  case '<':
	    if(spot < 33) {
	        addToString('<', spot);
	        spot++;
	    }
	    break;
	  case '.':
	    if(spot < 33) {
	       addToString('.', spot);
	       spot++;
	    }
	    break;
	  case '>':
	    if(spot < 33) {
	        addToString('>', spot);
	        spot++;
	    }
	    break;
	  case '/':
	    if(spot < 33) {
	       addToString('/', spot);
	       spot++;
	    }
	    break;
	  case '?':
	    if(spot < 33) {
	        addToString('?', spot);
	        spot++;
	    }
	    break;
	  case 32:
	    if(spot < 33) {
	       addToString(31, spot);
	       spot++;
	    }
	    break;
	}
      }
}


/***************************************************************
 myDisplay()

 GLUT CALLBACK: Don't call this function in your program--GLUT does it.

 ######################################################################
 Students: put your drawing commands/function calls in this function, 
 rather than in main().
 ######################################################################
***************************************************************/

void myDisplay(void)  {
  int i, j;
	
	if(state_of_game == START) { /* state start */
	    ClearWindow();
	    SetBackgndColor(0.0, 0.0, 0.25);
	    DrawQuitButton();
	    DrawStartButton();
	    DrawLoadButton();
	    DrawHighScoresButton();
            DrawBejeweledBoard();
	    ObjectClock();
	}
	
        if(pause_pressed == PAUSE1) { /* state pause */
	    state_of_game = PAUSE;
	    DrawPauseResumeButton();
	    paused();
	}
	
	if(pause_pressed == PAUSE2) { /* state resume */
            state_of_game = PLAY;
	    pause_pressed = PAUSE0;
	    free(click1_click2);
	    click1_click2 = NULL;
	    mouse_clicks = CLICK0;
	}
	
	if(time_elapsed == 325 && checkScore() == 7) { /* state gameover without a new score waiting to be imported */
	        wait(0.8);
	        ClearWindow();
	        SetBackgndColor(0.0, 0.0, 0.25);
	        DrawQuitButton();
	        DrawPlayAgainButton();
		DrawLoadButton();
                DrawBejeweledBoard();
                SetPenColor(0.9, 0.1, 0.1);
                DrawText2D(helv18, ((110 + 110 + 47 * 8) / 2) - 57, (70 + 70 + 47 * 8) / 2, "GAME OVER!");
		ObjectClock();
		state_of_game = GAMEOVER;
		free(click1_click2);
		click1_click2 = NULL;
		if(stage_gameover == GAMEOVER1) {
	            gameover();
		}
	}
	
	if(time_elapsed == 325 && checkScore() != 7 && registration_stage == REGISTERED) { /* state gameover with a new score already imported */
	        wait(0.8);
	        ClearWindow();
	        SetBackgndColor(0.0, 0.0, 0.25);
	        DrawQuitButton();
	        DrawPlayAgainButton();
		DrawLoadButton();
                DrawBejeweledBoard();
                SetPenColor(0.9, 0.1, 0.1);
                DrawText2D(helv18, ((110 + 110 + 47 * 8) / 2) - 57, (70 + 70 + 47 * 8) / 2, "GAME OVER!");
		ObjectClock();
		state_of_game = GAMEOVER;
		free(click1_click2);
		click1_click2 = NULL;
		if(stage_gameover == GAMEOVER1) {
	            gameover();
		}
	}
	
	if(time_elapsed == 325 && checkScore() != 7 && registration_stage == NOT_REGISTERED) { /* state gameover with a new score waiting to be imported */
	        wait(0.8);
	        ClearWindow();
	        SetBackgndColor(0.0, 0.0, 0.25);
	        DrawQuitButton();
	        DrawRegisterHighScoreButton();
		DrawLoadButton();
                DrawBejeweledBoard();
                SetPenColor(0.9, 0.1, 0.1);
                DrawText2D(helv18, ((110 + 110 + 47 * 8) / 2) - 57, (70 + 70 + 47 * 8) / 2, "GAME OVER!");
		ObjectClock();
		state_of_game = GAMEOVER;
		free(click1_click2);
		click1_click2 = NULL;
	}
	
	if(direction == 48) { /* return to state play after animation */
	    state_of_game = PLAY;
	    direction = 0;
	    free(click1_click2);
	    click1_click2 = NULL;
	}
	
	if(state_of_game == PLAY) { /* state play */
	    ClearWindow();
	    SetBackgndColor(0.0, 0.0, 0.25);
	    DrawQuitButton();
	    DrawPauseResumeButton();
	    DrawSaveButton();
	    DrawLoadButton();
	    DrawBejeweledBoard();
	    for(i = 0; i < SIZE; i++) {
                for(j = 0; j < SIZE; j++) {
                    addGraphicalObjects(i, j);
                }
	    }
	    
	    if(click1_click2 != NULL) {
                drawSelected();
	    }
	    clearSameOjects();
	    lookForSpaces();
	    

            if(score >= level_score) {
	        levelUp();
	    }
	    /*wait(0.6);*/
	    ObjectClock();
	    time_elapsed += 25;
	}
	
	
      if(state_of_game == REGISTER_HIGH_SCORE) {  /* state register high score */
          ClearWindow();
	  SetBackgndColor(0.0, 0.0, 0.25);
	  drawQwertyKeyboard();
      }
    
      if(state_of_game == HIGH_SCORES) {  /* state high score */
          ClearWindow();
	  SetBackgndColor(0.0, 0.0, 0.25);
	  drawHighScores();
	  DrawBackToTheGameButton();
      }	
	
      if(state_of_game == ANIMATE) { /* state animation */
	/* redrawing the frame of the chosen cells */
	SetLineWidth(2);
        SetPenColor(0.0, 0.0, 0.0);
        DrawBox(109 + 47 * click1_click2[1], 69 + 7 * 47 - 47 * click1_click2[0], 109 + 47 * (click1_click2[1] + 1), 69 + 8 * 47 - 47 * click1_click2[0]);
	DrawBox(109 + 47 * click1_click2[3], 69 + 7 * 47 - 47 * click1_click2[2], 109 + 47 * (click1_click2[3] + 1), 69 + 8 * 47 - 47 * click1_click2[2]);
	/* redrawing the interior of the chosen cells according to cell */
        if(click1_click2[0] == 0 || click1_click2[0] == 2 || click1_click2[0] == 4 || click1_click2[0] == 6) {
	    if(click1_click2[1] == 0 || click1_click2[1] == 2 || click1_click2[1] == 4 || click1_click2[1] == 6) {
            SetPenColor(0.6, 0.4, 0.3);
            DrawFillBox(110 + 47 * click1_click2[1], 70 + 7 * 47 - 47 * click1_click2[0], 108 + 47 * (click1_click2[1] + 1), 68 + 8 * 47 - 47 * click1_click2[0]);
	    SetPenColor(0.6, 0.5, 0.3);
            DrawFillBox(110 + 47 * click1_click2[3], 70 + 7 * 47 - 47 * click1_click2[2], 108 + 47 * (click1_click2[3] + 1), 68 + 8 * 47 - 47 * click1_click2[2]);
	    }
	    if(click1_click2[1] == 1 || click1_click2[1] == 3 || click1_click2[1] == 5 || click1_click2[1] == 7) {
            SetPenColor(0.6, 0.5, 0.3);
            DrawFillBox(110 + 47 * click1_click2[1], 70 + 7 * 47 - 47 * click1_click2[0], 108 + 47 * (click1_click2[1] + 1), 68 + 8 * 47 - 47 * click1_click2[0]);
	    SetPenColor(0.6, 0.4, 0.3);
            DrawFillBox(110 + 47 * click1_click2[3], 70 + 7 * 47 - 47 * click1_click2[2], 108 + 47 * (click1_click2[3] + 1), 68 + 8 * 47 - 47 * click1_click2[2]);
	    }
	}
	if(click1_click2[0] == 1 || click1_click2[0] == 3 || click1_click2[0] == 5 || click1_click2[0] == 7) {
	     if(click1_click2[1] == 0 || click1_click2[1] == 2 || click1_click2[1] == 4 || click1_click2[1] == 6) {
             SetPenColor(0.6, 0.5, 0.3);
             DrawFillBox(110 + 47 * click1_click2[1], 70 + 7 * 47 - 47 * click1_click2[0], 108 + 47 * (click1_click2[1] + 1), 68 + 8 * 47 - 47 * click1_click2[0]);
	     SetPenColor(0.6, 0.4, 0.3);
             DrawFillBox(110 + 47 * click1_click2[3], 70 + 7 * 47 - 47 * click1_click2[2], 108 + 47 * (click1_click2[3] + 1), 68 + 8 * 47 - 47 * click1_click2[2]);
	     }
	     if(click1_click2[1] == 1 || click1_click2[1] == 3 || click1_click2[1] == 5 || click1_click2[1] == 7) {
             SetPenColor(0.6, 0.4, 0.3);
             DrawFillBox(110 + 47 * click1_click2[1], 70 + 7 * 47 - 47 * click1_click2[0], 108 + 47 * (click1_click2[1] + 1), 68 + 8 * 47 - 47 * click1_click2[0]);
	     SetPenColor(0.6, 0.5, 0.3);
             DrawFillBox(110 + 47 * click1_click2[3], 70 + 7 * 47 - 47 * click1_click2[2], 108 + 47 * (click1_click2[3] + 1), 68 + 8 * 47 - 47 * click1_click2[2]);
	     }
	}
	drawSelected(); 
	
	/* first object selected moving up, second moving down */
	if(click1_click2[2] == click1_click2[0] - 1 && click1_click2[3] == click1_click2[1]) {
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {	    
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {	      
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {	      
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 5);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], -direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], direction, 0, 4);
          }
	  direction++;
	}

  
       /* first object selected moving down, second moving up */
       if(click1_click2[2] == click1_click2[0] + 1 && click1_click2[3] == click1_click2[1]) {
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 0);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 1);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 2);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 3);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 5);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 4);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], direction, 0, 5);
	      animateObject(click1_click2[0], click1_click2[1], -direction, 0, 4);
          }
	  direction++;
       }
    
    
       /* first object selected moving left, second moving right */
       if(click1_click2[2] == click1_click2[0] && click1_click2[3] == click1_click2[1] - 1) {
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 5);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, -direction, 4);
          }
	  direction++;
       }
      
       
       /* first object selected moving right, second moving left */
       if(click1_click2[2] == click1_click2[0] && click1_click2[3] == click1_click2[1] + 1) {
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 0 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 0);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 1 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 1);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 2 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 2);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 4);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 3 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 3);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 5);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 4 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 5) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 4);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 5);
          }
          if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 0) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 0);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 1) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 1);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 2) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 2);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 3) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 3);
          }
	  if(bejeweled_board[click1_click2[0]][click1_click2[1]] == 5 && bejeweled_board[click1_click2[2]][click1_click2[3]] == 4) {
	      animateObject(click1_click2[2], click1_click2[3], 0, -direction, 5);
	      animateObject(click1_click2[0], click1_click2[1], 0, direction, 4);
          }
	  direction++;
       }
       /*wait(0.01);*/
      }
      
	
}

/***************************************************************
    DrawQuitButton()
	This function draws a quit button at a location specified
	through #defined literals. 
***************************************************************/
void DrawQuitButton() {

    SetPenColor(0.6, 0.4, 0.3);		
    DrawFillBox(QUIT_BUTTON_X, QUIT_BUTTON_Y, QUIT_BUTTON_X + QUIT_BUTTON_LENGTH, QUIT_BUTTON_Y + BUTTON_HEIGHT);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, QUIT_BUTTON_X + 10, QUIT_BUTTON_Y + 10, "QUIT");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(QUIT_BUTTON_X + 3, QUIT_BUTTON_Y + 2,QUIT_BUTTON_X + QUIT_BUTTON_LENGTH - 2, QUIT_BUTTON_Y + BUTTON_HEIGHT - 3);
    
}

/***************************************************************
    DrawPauseResumeButton()
	This function draws a pause-resume button at a location specified
	through #defined literals. 
***************************************************************/
void DrawPauseResumeButton() {
  
    if(pause_pressed == PAUSE0) {
        SetPenColor(0.6, 0.4, 0.3);		
	DrawFillBox(PAUSE_BUTTON_X, PAUSE_BUTTON_Y, PAUSE_BUTTON_X + PAUSE_BUTTON_LENGTH, PAUSE_BUTTON_Y + BUTTON_HEIGHT);
	SetPenColor(0.9, 0.1, 0.1);
	DrawText2D(helv18, PAUSE_BUTTON_X + 12, PAUSE_BUTTON_Y + 10, "PAUSE");
	SetLineWidth(1);
	SetPenColor(0.9, 0.1, 0.1);
	DrawBox(PAUSE_BUTTON_X + 3, PAUSE_BUTTON_Y + 2, PAUSE_BUTTON_X + PAUSE_BUTTON_LENGTH - 2, PAUSE_BUTTON_Y + BUTTON_HEIGHT - 3);
    }
    
    if(pause_pressed == PAUSE1) {
        SetPenColor(0.6, 0.4, 0.3);		
	DrawFillBox(10, 70, 10 + 80, 70 + 35);
	SetPenColor(0.9, 0.1, 0.1);
	DrawText2D(helv18, PAUSE_BUTTON_X + 4, PAUSE_BUTTON_Y + 10, "RESUME");
	SetLineWidth(1);
	SetPenColor(0.9, 0.1, 0.1);
	DrawBox(PAUSE_BUTTON_X + 3, PAUSE_BUTTON_Y + 2, PAUSE_BUTTON_X + PAUSE_BUTTON_LENGTH - 2, PAUSE_BUTTON_Y + BUTTON_HEIGHT - 3);
    }
    
}

/***************************************************************
    DrawStartButton()
	This function draws a start button at a location specified
	through #defined literals. 
***************************************************************/
void DrawStartButton() {
  
    SetPenColor(0.6, 0.4, 0.3);		
    DrawFillBox(START_BUTTON_X, START_BUTTON_Y, START_BUTTON_X + START_BUTTON_LENGTH, START_BUTTON_Y + BUTTON_HEIGHT);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, START_BUTTON_X + 5, START_BUTTON_Y + 10, "START");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(START_BUTTON_X + 3, START_BUTTON_Y + 2, START_BUTTON_X + START_BUTTON_LENGTH - 2, START_BUTTON_Y + BUTTON_HEIGHT - 3);

}

/***************************************************************
    DrawPlayAgainButton()
	This function draws a play again button at a location specified
	through #defined literals. 
***************************************************************/
void DrawPlayAgainButton() {
  
    SetPenColor(0.6, 0.4, 0.3);		
    DrawFillBox(PLAY_AGAIN_BUTTON_X, PLAY_AGAIN_BUTTON_Y, PLAY_AGAIN_BUTTON_X + PLAY_AGAIN_BUTTON_LENGTH, PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, PLAY_AGAIN_BUTTON_X + 5, PLAY_AGAIN_BUTTON_Y + 10, "PLAY AGAIN");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(PLAY_AGAIN_BUTTON_X + 3, PLAY_AGAIN_BUTTON_Y + 2, PLAY_AGAIN_BUTTON_X + PLAY_AGAIN_BUTTON_LENGTH - 2, PLAY_AGAIN_BUTTON_Y + BUTTON_HEIGHT - 3);

}

/***************************************************************
    DrawSaveButton()
	This function draws a save button at a location specified
	through #defined literals. 
***************************************************************/
void DrawSaveButton() {
  
    SetPenColor(0.6, 0.4, 0.3);		
    DrawFillBox(SAVE_BUTTON_X, SAVE_BUTTON_Y, SAVE_BUTTON_X + SAVE_BUTTON_LENGTH, SAVE_BUTTON_Y + BUTTON_HEIGHT);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, SAVE_BUTTON_X + 9, SAVE_BUTTON_Y + 10, "SAVE");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(SAVE_BUTTON_X + 3, SAVE_BUTTON_Y + 2, SAVE_BUTTON_X + SAVE_BUTTON_LENGTH - 2, SAVE_BUTTON_Y + BUTTON_HEIGHT - 3);

}

/***************************************************************
    DrawLoadButton()
	This function draws a load button at a location specified
	through #defined literals. 
***************************************************************/
void DrawLoadButton() {
  
    SetPenColor(0.6, 0.4, 0.3);		
    DrawFillBox(LOAD_BUTTON_X, LOAD_BUTTON_Y, LOAD_BUTTON_X + LOAD_BUTTON_LENGTH, LOAD_BUTTON_Y + BUTTON_HEIGHT);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, LOAD_BUTTON_X + 10, LOAD_BUTTON_Y + 10, "LOAD");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(LOAD_BUTTON_X + 3, LOAD_BUTTON_Y + 2, LOAD_BUTTON_X + LOAD_BUTTON_LENGTH - 2, LOAD_BUTTON_Y + BUTTON_HEIGHT - 3);

}

/***************************************************************
    DrawHighScoresButton()
	This function draws a high scores button at a location specified
	through #defined literals. 
***************************************************************/
void DrawHighScoresButton() {
  
    SetPenColor(0.6, 0.4, 0.3);		
    DrawFillBox(HIGH_SCORES_BUTTON_X, HIGH_SCORES_BUTTON_Y, HIGH_SCORES_BUTTON_X + HIGH_SCORES_BUTTON_LENGTH, HIGH_SCORES_BUTTON_Y + BUTTON_HEIGHT * 2);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, HIGH_SCORES_BUTTON_X + 22, HIGH_SCORES_BUTTON_Y + 37, "HIGH");
    DrawText2D(helv18, HIGH_SCORES_BUTTON_X + 5, HIGH_SCORES_BUTTON_Y + 17, "SCORES");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(HIGH_SCORES_BUTTON_X + 3, HIGH_SCORES_BUTTON_Y + 2, HIGH_SCORES_BUTTON_X + HIGH_SCORES_BUTTON_LENGTH - 2, HIGH_SCORES_BUTTON_Y + BUTTON_HEIGHT * 2 - 3);
    
}

/***************************************************************
    DrawRegisterHighScoreButton()
	This function draws a register high scores button at a location specified
	through #defined literals. 
***************************************************************/
void DrawRegisterHighScoreButton() {
  
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillBox(REGISTER_HIGH_SCORE_BUTTON_X, REGISTER_HIGH_SCORE_BUTTON_Y, REGISTER_HIGH_SCORE_BUTTON_X + REGISTER_HIGH_SCORE_BUTTON_LENGTH, REGISTER_HIGH_SCORE_BUTTON_Y + BUTTON_HEIGHT);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, REGISTER_HIGH_SCORE_BUTTON_X + 13, REGISTER_HIGH_SCORE_BUTTON_Y + 10, "REGISTER HIGH SCORE");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(REGISTER_HIGH_SCORE_BUTTON_X + 3, REGISTER_HIGH_SCORE_BUTTON_Y + 2, REGISTER_HIGH_SCORE_BUTTON_X + REGISTER_HIGH_SCORE_BUTTON_LENGTH - 2, REGISTER_HIGH_SCORE_BUTTON_Y + BUTTON_HEIGHT - 3);
    
}

/***************************************************************
    DrawBackToTheGameButton()
	This function draws a back to the game button at a location specified
	through #defined literals. 
***************************************************************/
void DrawBackToTheGameButton() {
  
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillBox(BACK_TO_THE_GAME_BUTTON_X, BACK_TO_THE_GAME_BUTTON_Y, BACK_TO_THE_GAME_BUTTON_X + BACK_TO_THE_GAME_BUTTON_LENGTH, BACK_TO_THE_GAME_BUTTON_Y + BUTTON_HEIGHT);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, BACK_TO_THE_GAME_BUTTON_X + 13, BACK_TO_THE_GAME_BUTTON_Y + 10, "BACK TO THE GAME");
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawBox(BACK_TO_THE_GAME_BUTTON_X + 3, BACK_TO_THE_GAME_BUTTON_Y + 2, BACK_TO_THE_GAME_BUTTON_X + BACK_TO_THE_GAME_BUTTON_LENGTH - 2, BACK_TO_THE_GAME_BUTTON_Y + BUTTON_HEIGHT - 3);

}

/***************************************************************
    InitializeObjects()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function initializes the values of the game.
    Preconditions: The game just started running, or restarted after a gameover.
    Postconditions: All the values are initialized.
***************************************************************/
void InitializeObjects() {
  int i, j;
  list *current;

    click1_click2 = NULL;
    score = 0;
    time_elapsed = 0;
    level = 1;
    level_score = 500;
    level_boost = 0;
    direction = 0;
    spot = 0;
    mouse_clicks = CLICK0;
    pause_pressed = PAUSE0;
    stage_gameover = GAMEOVER0;
    capslock_pressed = CAPSLOCK0;
    shift_pressed = SHIFT0;
    registration_stage = NOT_REGISTERED;

    for(i = 0; i < SIZE; i++) {
         for(j = 0; j < SIZE; j++) {
             addObjectsOnTheBejeweledBoard(i, j);
         }
    }

    /* Purple */
    colors[0].red = 0.4;
    colors[0].green = 0.0;
    colors[0].blue = 0.4;
    /* Cyan */
    colors[1].red = 0.0;
    colors[1].green = 0.0;
    colors[1].blue = 1.0;
    /* Red */
    colors[2].red = 1.0;
    colors[2].green = 0.0;
    colors[2].blue = 0.0;
    /* Green */
    colors[3].red = 0.0;
    colors[3].green = 0.42;
    colors[3].blue = 0.0;
    /* Orange */
    colors[4].red = 1.0;
    colors[4].green = 0.4;
    colors[4].blue = 0.0;
    /* Yellow */
    colors[5].red = 1.0;
    colors[5].green = 1.0;
    colors[5].blue = 0.0;

    qwerty[0][0] = '1';
    qwerty[0][1] = '2';
    qwerty[0][2] = '3';
    qwerty[0][3] = '4';
    qwerty[0][4] = '5';
    qwerty[0][5] = '6';
    qwerty[0][6] = '7';
    qwerty[0][7] = '8';
    qwerty[0][8] = '9';
    qwerty[0][9] = '0';
    qwerty[1][0] = 'Q';
    qwerty[1][1] = 'W';
    qwerty[1][2] = 'E';
    qwerty[1][3] = 'R';
    qwerty[1][4] = 'T';
    qwerty[1][5] = 'Y';
    qwerty[1][6] = 'U';
    qwerty[1][7] = 'I';
    qwerty[1][8] = 'O';
    qwerty[1][9] = 'P';
    qwerty[2][0] = 'A';
    qwerty[2][1] = 'S';
    qwerty[2][2] = 'D';
    qwerty[2][3] = 'F';
    qwerty[2][4] = 'G';
    qwerty[2][5] = 'H';
    qwerty[2][6] = 'J';
    qwerty[2][7] = 'K';
    qwerty[2][8] = 'L';
    qwerty[2][9] = ';';
    qwerty[3][0] = 'Z';
    qwerty[3][1] = 'X';
    qwerty[3][2] = 'C';
    qwerty[3][3] = 'V';
    qwerty[3][4] = 'B';
    qwerty[3][5] = 'N';
    qwerty[3][6] = 'M';
    qwerty[3][7] = ',';
    qwerty[3][8] = '.';
    qwerty[3][9] = '/';

    shift_querty_numbers[0] = '!';
    shift_querty_numbers[1] = '@';
    shift_querty_numbers[2] = '#';
    shift_querty_numbers[3] = '$';
    shift_querty_numbers[4] = '%';
    shift_querty_numbers[5] = '^';
    shift_querty_numbers[6] = '&';
    shift_querty_numbers[7] = '*';
    shift_querty_numbers[8] = '(';
    shift_querty_numbers[9] = ')';

    if(fopen("high_scores.txt","r") == NULL) {
        pFile = fopen("high_scores.txt","w");
	for(current = root; current != NULL; current = current -> next) {
	    fprintf(pFile, "%d", current -> level_high_score);
	    fprintf(pFile, "\n");
	    fprintf(pFile, "%d", current -> score_high_score);
	    fprintf(pFile, "\n");
	    fprintf(pFile, "%s", current -> name_high_score);
	    fprintf(pFile, "\n");
	    fprintf(pFile, "\n");
	}
	 
	fclose (pFile);
    }

    if(fopen("high_scores.txt","r") != NULL) {
        pFile = fopen("high_scores.txt","r");
	for(current = root; current != NULL; current = current -> next) {
	    fscanf(pFile, "%d", &current -> level_high_score);
	    fscanf(pFile, "%d", &current -> score_high_score);
	    fscanf(pFile, "%s", current -> name_high_score);
	}
	 
	fclose (pFile);
    }
    
}

/***************************************************************
    swap()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function switches the value of the chosen cells if verifyMove(...) is 1 and proceeds to the animation.
    Preconditions: The 1st(current location) and 2nd(destination) cell were chosen.
    Postconditions: The cells were swaped/not swaped.
***************************************************************/
void swap() { 
  int temp;

    if(verifyMove(click1_click2[0], click1_click2[1], click1_click2[2], click1_click2[3]) == 1) {
	temp = bejeweled_board[click1_click2[2]][click1_click2[3]];
	bejeweled_board[click1_click2[2]][click1_click2[3]] = bejeweled_board[click1_click2[0]][click1_click2[1]];
	bejeweled_board[click1_click2[0]][click1_click2[1]] = temp;
	time_elapsed = 0;
	state_of_game = ANIMATE;
    }

}

/***************************************************************
    verifyMove(i, j, destinationi, destinationj)
    Inputs: i <- our current's cell row, j <- our current's cell column, destinationi <- our destination's row, destinationj <- our destination's column.
    Returns: 1 or 0.
    Purpose: This function returns 1 if the movement can be done(moving to a cell with 2 same objects around making a block of 3 together) or 0 in any other way.
    Preconditions: The 2 objects were chosen and swap() was called.
    Postconditions: The movement can be done/cannot be done.
***************************************************************/
int verifyMove(int i, int j, int destinationi, int destinationj) {
    
    /* Moving Up */
    if(destinationi == i - 1 && destinationj == j) {
        if(i > 2 && j > 1) {
            if((j < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-2][j]) && (bejeweled_board[i][j] == bejeweled_board[i-3][j])) || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+2])))) {
                return 1;
            }
            if((j == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-2][j]) && (bejeweled_board[i][j] == bejeweled_board[i-3][j])))) {
                return 1;
            }
            if((j > SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-2][j]) && (bejeweled_board[i][j] == bejeweled_board[i-3][j]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j-2])))) {
                return 1;
            }
        }
        
        if(i <= 2 && j > 1) {
            if((j < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+2])))) { 
                return 1;
            }   
            if((j == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+1])))) {
                return 1;
            }
            if((j > SIZE - 2) && ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j-2]))) {
                return 1;
            }
        }
        
        if(i > 2 && j <= 1) {
            if(j == 1 && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-2][j]) && (bejeweled_board[i][j] == bejeweled_board[i-3][j]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+2])))) {
                return 1;
            }
            if(j < 1 && (((bejeweled_board[i][j] == bejeweled_board[i-2][j]) && (bejeweled_board[i][j] == bejeweled_board[i-3][j]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+2])))) {
                return 1;
            }
        }
        
        if(i <= 2 && j < 1) {
            if((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+2])) {
                return 1;
            }
        }
        
        if(i <= 2 && j == 1) {
            if(((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-1][j+1]))) {
                return 1;
            }
        }
    }
    
    /* Moving Down */
    if(destinationi == i + 1 && destinationj == j) {    
        if(i < SIZE - 3 && j > 1) {
            if((j < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+2][j]) && (bejeweled_board[i][j] == bejeweled_board[i+3][j])) || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+2])))) {
                return 1;
            }
            if((j == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))
	    || ((bejeweled_board[i][j] == bejeweled_board[i+2][j]) && (bejeweled_board[i][j] == bejeweled_board[i+3][j])))){
                return 1;
            }
            if((j > SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i+2][j]) && (bejeweled_board[i][j] == bejeweled_board[i+3][j]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-2])))) {
                return 1;
            }
        }
        
        if(i >= SIZE - 3 && j > 1) {
            if((j < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j]  == bejeweled_board[i+1][j+2])))) {  
                return 1;
            }
            if((j == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1])))) {
                return 1;
            }
            if((j > SIZE - 2) && ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-2]))) {
                return 1;
            }
        }
        
        if(i < SIZE - 3 && j <= 1) {
            if(j == 1 && (((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+2][j]) && (bejeweled_board[i][j] == bejeweled_board[i+3][j]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+2])))) {
                return 1;
            }
            if(j < 1 && (((bejeweled_board[i][j] == bejeweled_board[i+2][j]) && (bejeweled_board[i][j] == bejeweled_board[i+3][j]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+2])))) {
                return 1;
            }
        }
        
        if(i >= SIZE - 3 && j < 1) {
            if((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+2])) {
                return 1;
            }
        }
        
        if(i >= SIZE - 3 && j == 1) {
            if(((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+2]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))) {
                return 1;
            }
        }
    }
    
    /* Moving left */
    if(destinationi == i && destinationj == j - 1) {
        if(i > 1 && j > 2) {
            if((i < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i][j-2]) && (bejeweled_board[i][j] == bejeweled_board[i][j-3])) || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j-1])))) {
                return 1;
            }
            if((i == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i][j-2]) && (bejeweled_board[i][j] == bejeweled_board[i][j-3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-1])))) {
                return 1;
            }
            if((i > SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i][j-2]) && (bejeweled_board[i][j] == bejeweled_board[i][j-3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j-1])))) {
                return 1;
            }
        }
        
        if(i <= 1 && j > 2) {
            if(i == 1 && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i][j-2]) && (bejeweled_board[i][j] == bejeweled_board[i][j-3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j-1])))) {
                return 1;
            }
            if(i < 1 && (((bejeweled_board[i][j] == bejeweled_board[i][j-2]) && (bejeweled_board[i][j] == bejeweled_board[i][j-3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j-1])))) {
                return 1;
            }
        }
            
        
        if(i > 1 && j <= 2) {
            if((i < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j-1])))) { 
                return 1; 
            }
            if((i == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-1])))) {
                return 1;
            }
            if((i > SIZE - 2) && ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j-1]))) {
                return 1;
            }
        }
        
        if(i < 1 && j <= 2) {
            if((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j-1])) {
                return 1;
            }
        }
        
        if(i == 1 && j <= 2) {
            if(((bejeweled_board[i][j] == bejeweled_board[i+1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j-1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j-1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j-1]))) {
                return 1;
            }
        }
    }
    
    /* Moving right */
    if(destinationi == i && destinationj == j + 1) {
        if(i > 1 && j < SIZE - 3) {
            if((i < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i][j+2]) && (bejeweled_board[i][j] == bejeweled_board[i][j+3])) || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j+1])))) {
                return 1;
            }
            if((i == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i][j+2]) && (bejeweled_board[i][j] == bejeweled_board[i][j+3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1])))) {
                return 1;
            }
            if((i > SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i][j+2]) && (bejeweled_board[i][j] == bejeweled_board[i][j+3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j+1])))) {
                return 1;
            }
        }
        
        if(i <= 1 && j < SIZE - 3) {
            if(i == 1 && (((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i][j+2]) && (bejeweled_board[i][j] == bejeweled_board[i][j+3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j+1])))) {
                return 1;
            }
            if(i < 1 && (((bejeweled_board[i][j] == bejeweled_board[i][j+2]) && (bejeweled_board[i][j] == bejeweled_board[i][j+3]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j+1])))) {
                return 1;
            }
        }
            
        
        if(i > 1 && j >= SIZE - 3) {
            if((i < SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j+1])))) {  
                return 1;
            }
            if((i == SIZE - 2) && (((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1])))) {
                return 1;
            }
            if((i > SIZE - 2) && ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i-2][j+1]))) {
                return 1;
            }
        }
        
        if(i < 1 && j >= SIZE - 3) {
            if((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j+1])) {
                return 1;
            }
        }
        
        if(i == 1 && j >= SIZE - 3) {
            if((((bejeweled_board[i][j] == bejeweled_board[i+1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j+1]))
            || ((bejeweled_board[i][j] == bejeweled_board[i-1][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i+1][j+1])))) {
                return 1;
            }
        }
    }
    
return 0;
}

/***************************************************************
    clearSameObjects()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function deletes at least a block of 3 or more same objects in a row or column and reward us with 5 points for each deleted cell.
    Preconditions: None.
    Postconditions: The blocks of 3 or more were deleted and the points for each deletion were counted.
***************************************************************/
void clearSameOjects() {
  int i, j, n[SIZE][SIZE]; 

    for (i = 0; i < SIZE; i++) {
        for(j = 0; j < SIZE; j++) {
	n[i][j] = 0; 
	}
    }

    for(i = 0; i < SIZE; i++) {
	for(j = 0; j < SIZE; j++) {
	    if((bejeweled_board[i][j] == bejeweled_board[i][j+1]) && (bejeweled_board[i][j] == bejeweled_board[i][j+2]) && (j < SIZE - 2)) {
                while((j < SIZE - 1) && (bejeweled_board[i][j] == bejeweled_board[i][j+1]) && bejeweled_board[i][j] != 7) {
		    n[i][j] = 1; 
	            j++;
		}
	    n[i][j] = 1;
	    }
	}
    }
	

    for(i = 0; i < SIZE; i++) {
	for(j = 0; j < SIZE; j++) {
	    if((bejeweled_board[i][j] == bejeweled_board[i+1][j]) && (bejeweled_board[i][j] == bejeweled_board[i+2][j]) && (i < SIZE - 2)) {
		while((i < SIZE - 1) && (bejeweled_board[i][j] == bejeweled_board[i+1][j]) && bejeweled_board[i][j] != 7) {
		    n[i][j] = 1;
		    i++;
		}
	    n[i][j] = 1;
	    }
	}
    }
    
	
    for(i = 0; i < SIZE; i++) {
	for (j = 0; j < SIZE; j++) {
	    if(n[i][j] == 1) {
	        bejeweled_board[i][j] = 7;
		time_elapsed = 0;
		score += 5;
	    }
        }
    }
    
}

/***************************************************************
    lookForSpaces()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function creates new objects(random) on top of the game board.
    Preconditions: None.
    Postconditions: All the empty cells were filled random.
***************************************************************/
void lookForSpaces() {
  int i, j, l;

    for(i = 0; i < SIZE; i++) {
	for(j = 0; j < SIZE; j++) {
	    if(bejeweled_board[i][j] == 7 && i == 0) {   /* checking if the empty cells are on top */
	        bejeweled_board[i][j] = rand()%6;
		time_elapsed = 0;
		continue;
	    }
	    if(bejeweled_board[i][j] == 7 && i > 0) {    /* if they are not on top, shift the cells, which are on top, down and continue with the first loop */ 
	        for(l = i; l > 0; l--) {
		    bejeweled_board[l][j] = bejeweled_board[l-1][j];
		}
		bejeweled_board[0][j] = rand()%6;
		time_elapsed = 0;
                continue;
	    }
	}
    }
    
}

/***************************************************************
    addObjectsOnTheBejeweledBoard(i, j)
    Inputs: i <- our cell's row, j <- our cell's column.
    Returns: Nothing.
    Purpose: This function randomizes the game board (3 same objects cant be placed together).
    Preconditions: InitializeObjects() is called.
    Postconditions: The game board was randomized.
***************************************************************/
void addObjectsOnTheBejeweledBoard(int i, int j) {
     
    bejeweled_board[i][j] = rand()%6;
    
    /* Checking the last 2 objects of the current column */
    while (i >= 2 && bejeweled_board[i][j] == bejeweled_board[i-1][j] && bejeweled_board[i][j] == bejeweled_board[i-2][j]) {
        bejeweled_board[i][j] = rand()%6;
    }
    /* Checking the last 2 objects of the current row */
    while (j >= 2 && bejeweled_board[i][j] == bejeweled_board[i][j-1] && bejeweled_board[i][j] == bejeweled_board[i][j-2]) {
	bejeweled_board[i][j] = rand()%6;
    }
    /* Checking if the first check was bypassed by the second and calls recursive addObjectsOnTheBejeweledBoard(...) */
    if(i >= 2 && bejeweled_board[i][j] == bejeweled_board[i-1][j] && bejeweled_board[i][j] == bejeweled_board[i-2][j]) {
        addObjectsOnTheBejeweledBoard(i, j);
    }
    
}

/***************************************************************
    addGraphicalObjects()
    Inputs: i <- our cell's row, j <- our cell's column.
    Returns: Nothing.
    Purpose: This function calls animateObject(...) with the default cell parameters and the correct number for each object.
    Preconditions: The game board must be filled.
    Postconditions: The correct object object for each cell is displayed.
***************************************************************/
void addGraphicalObjects(int i, int j) {
  
    if(bejeweled_board[i][j] == 0) {
        animateObject(i, j, 0, 0, 0); /* Object0 */
    }
    if(bejeweled_board[i][j] == 1) {
        animateObject(i, j, 0, 0, 1); /* Object1 */
    }
    if(bejeweled_board[i][j] == 2) {
        animateObject(i, j, 0, 0, 2); /* Object2 */
    }
    if(bejeweled_board[i][j] == 3) {
        animateObject(i, j, 0, 0, 3); /* Object3 */
    }
    if(bejeweled_board[i][j] == 4) {
        animateObject(i, j, 0, 0, 4); /* Object4 */
    }
    if(bejeweled_board[i][j] == 5) {
        animateObject(i, j, 0, 0, 5); /* Object5 */
    }
  
}

/***************************************************************
    drawSelected()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function draws a red/green frame around the selected 1st/2nd cell.
    Preconditions: At least one object must be selected.
    Postconditions: The frame(s) was(were) displayed.
***************************************************************/
void drawSelected() {

    SetLineWidth(2);  
    SetPenColor(0.0, 1.0, 0.0);
    DrawBox(109 + 47 * click1_click2[3], 69 + 7 * 47 - 47 * click1_click2[2], 109 + 47 * (click1_click2[3] + 1), 69 + 8 * 47 - 47 * click1_click2[2]);
    SetPenColor(1.0, 0.0, 0.0);
    DrawBox(109 + 47 * click1_click2[1], 69 + 7 * 47 - 47 * click1_click2[0], 109 + 47 * (click1_click2[1] + 1), 69 + 8 * 47 - 47 * click1_click2[0]);

}

/***************************************************************
    wait(seconds)
    Inputs: seconds <- the seconds that we want to add to delay.
    Returns: Nothing.
    Purpose: This function adds a delay during execution.
    Preconditions: None.
    Postconditions: The execution was delayed for 'seconds' seconds.
***************************************************************/
void wait(double seconds) {
  
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    while(clock() < endwait) {}
    
}

/***************************************************************
    levelUp()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function increases the level and the score to the next level.
    Preconditions: A certain score was reached.
    Postconditions: The level and score to the next level were increased.
***************************************************************/
void levelUp() {
  
    DrawBejeweledBoard();
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, ((110 + 110 + 47 * 8) / 2) - 47, (70 + 70 + 47 * 8) / 2, "LEVEL UP!");
    level++;
    level_boost++;
    level_score = level_score + 500 + 200 * level_boost;
    time_elapsed = 0;
    mouse_clicks = CLICK0;
    free(click1_click2);
    click1_click2 = NULL;
  
}

/***************************************************************
    gameover()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function reinitializes the values of the game after a game loss.
    Preconditions: The button play again was pressed.
    Postconditions: The game values were reinitialized.
***************************************************************/
void gameover() {

    state_of_game = PLAY;
    InitializeObjects();
    
}

/***************************************************************
    paused()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function pauses the game.
    Preconditions: The pause button or p,P were pressed for first time.
    Postconditions: The game is paused.
***************************************************************/
void paused() {
  
    DrawBejeweledBoard();
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, ((110 + 110 + 47 * 8) / 2) - 40, (70 + 70 + 47 * 8) / 2, "PAUSED!");
    
}

/***************************************************************
    ObjectClock()
    Inputs: None.
    Returns: Nothing.
    Purpose: This draws the game clock.
    Preconditions: The state of game is PLAY.
    Postconditions: The current time remaining is diplayed.
***************************************************************/
void ObjectClock() {
  
    SetPenColor(0.9, 0.1, 0.1);
    DrawFillCircle(130, 35, 25);
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillCircle(130, 35, 22);
    SetLineWidth(2);
    SetPenColor(0.0, 0.0, 0.0);
    DrawLine(130, 35 + 22, 130, 35 + 22 - 2);
    DrawLine(130, 35 - 22, 130, 35 - 22 + 2);
    DrawLine(130 - 22, 35, 130 -22 + 2, 35);
    DrawLine(130 + 22, 35, 130 + 22 - 2, 35);
    SetPenColor(0.0, 0.0, 0.25);
    DrawLine(130, 35, 130, 35 + 17);
    DrawLine(130, 35, 130 + 13, 35);
    SetPenColor(0.0, 0.0, 0.0);
    DrawFillCircle(130, 35, 3);
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillBox(163, 13, 488, 58);
    SetPenColor(0.9, 0.1, 0.1);
    DrawFillBox(163, 13, 488 - time_elapsed, 58);
    SetPenColor(0.0, 0.0 , 0.0);
    DrawBox(161, 13, 488, 58);
  
}

/***************************************************************
    DrawBejeweledBoard()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function draws the game board, the level, the score to the next level and the current score.
    Preconditions: None.
    Postconditions: The game board, the level, the score to the next level and the current score were displayed.
***************************************************************/
void DrawBejeweledBoard() {
  int i, j;
  
    for(i = 110; i < 110 + 47 * 8; i += 94) {
        for(j = 70; j < 70 + 47 * 8; j += 94) {
            SetPenColor(0.6, 0.5, 0.3);
            DrawFillBox(i, j, i + 45, j + 45);
	}
    }
    for(i = 110 + 47; i < 110 + 47 * 9; i += 94) {
        for(j = 70 + 47; j < 70 + 47 * 9; j += 94) {
            SetPenColor(0.6, 0.5, 0.3);
            DrawFillBox(i, j, i + 45, j + 45);
	}
    }
    for(i = 110; i < 110 + 47 * 8; i += 94) {
        for(j = 70 + 47; j < 70 + 47 * 8; j += 94) {
            SetPenColor(0.6, 0.4, 0.3);
            DrawFillBox(i, j, i + 45, j + 45);
	}
    }
    for(i = 110 + 47; i < 110 + 47 * 8; i += 94) {
        for(j = 70; j < 70 + 47 * 8; j += 94) {
            SetPenColor(0.6, 0.4, 0.3);
            DrawFillBox(i, j, i + 45, j + 45);
	}
    }
    SetLineWidth(2);
    SetPenColor(0.0, 0.0, 0.0);
    DrawBox(109,69,109 + 47 * 8, 69 + 47 * 8);
    for(i = 109; i <= 109 + 47 * 8; i += 47) {
        SetLineWidth(2);
        DrawLine(i, 69, i, 69 + 47 * 8);
    }
    for(j = 69; j <= 69 + 47 * 8; j += 47) {
        SetLineWidth(2);
        DrawLine(109, j, 109 + 47 * 8, j);
    }
    
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillBox(590, 260, 690, 290);
    DrawFillCircle(690, (260 + 290) / 2, 15);
    DrawFillCircle(590, (260 + 290) / 2, 15);
    SetPenColor(0.0, 0.0, 0.25);
    DrawFillCircle(698, 290 + 4, 8);
    DrawFillCircle(698, 260 - 4, 8);
    DrawFillCircle(582, 290 + 4, 8);
    DrawFillCircle(582, 260 - 4, 8);
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawLine(590 + 2, 260 + 2, 690 - 2, 260 + 2);
    DrawLine(590 + 2, 290 - 3, 690 - 2, 290 - 3);
    DrawArc(698 - 2, 290 - 2, 8, 180, 270);
    DrawArc(698 - 2, 260 + 2, 8, 90, 180);
    DrawArc(582 + 2, 290 - 2, 8, 270, 360);
    DrawArc(582 + 2, 260 + 2, 8, 0, 90);
    DrawLine(697, 260 + 10, 697, 290 - 10);
    DrawLine(584, 260 + 10, 584, 290 - 10);
    
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillBox(610, 355, 670, 385);
    DrawFillCircle(670, (355 + 385) / 2, 15);
    DrawFillCircle(610, (355 + 385) / 2, 15);
    SetPenColor(0.0, 0.0, 0.25);
    DrawFillCircle(678, 385 + 4, 8);
    DrawFillCircle(678, 355 - 4, 8);
    DrawFillCircle(602, 385 + 4, 8);
    DrawFillCircle(602, 355 - 4, 8);
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawLine(610 + 2, 355 + 2, 670 - 2, 355 + 2);
    DrawLine(610 + 2, 385 - 3, 670 - 2, 385 - 3);
    DrawArc(678 - 2, 385 - 2, 8, 180, 270);
    DrawArc(678 - 2, 355 + 2, 8, 90, 180);
    DrawArc(602 + 2, 385 - 2, 8, 270, 360);
    DrawArc(602 + 2, 355 + 2, 8, 0, 90);
    DrawLine(677, 355 + 10, 677, 385 - 10);
    DrawLine(604, 355 + 10, 604, 385 - 10);
    
    
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillBox(590, 165, 690, 195);
    DrawFillCircle(690, (165 + 195) / 2, 15);
    DrawFillCircle(590, (165 + 195) / 2, 15);
    SetPenColor(0.0, 0.0, 0.25);
    DrawFillCircle(698, 195 + 4, 8);
    DrawFillCircle(698, 165 - 4, 8);
    DrawFillCircle(582, 195 + 4, 8);
    DrawFillCircle(582, 165 - 4, 8);
    SetLineWidth(1);
    SetPenColor(0.9, 0.1, 0.1);
    DrawLine(590 + 2, 165 + 2, 690 - 2, 165 + 2);
    DrawLine(590 + 2, 195 - 3, 690 - 2, 195 - 3);
    DrawArc(698 - 2, 195 - 2, 8, 180, 270);
    DrawArc(698 - 2, 165 + 2, 8, 90, 180);
    DrawArc(582 + 2, 195 - 2, 8, 270, 360);
    DrawArc(582 + 2, 165 + 2, 8, 0, 90);
    DrawLine(697, 165 + 10, 697, 195 - 10);
    DrawLine(584, 165 + 10, 584, 195 - 10);
    
    SetLineWidth(2);
    SetPenColor(0.6, 0.4, 0.3);
    DrawLine(0, 309, 50, 309);
    DrawLine(50, 309, 50, 472);
    DrawLine(50, 472, 518, 472);
    DrawLine(518, 472, 518, 130);
    DrawLine(518, 130, 760, 130);
    DrawLine(760, 130, 760, 472);
    DrawLine(760, 472, 800, 472);
    
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(rom24, 618, 390, "Level");
    DrawText2D(rom24, 553, 295, "Score to Next Level");
    DrawText2D(rom24, 615, 200, "Score");
    sprintf(sscore, "%06d", score);
    sprintf(slevel, "%03d", level);
    sprintf(sscore_to_next_level, "%06d",  level_score);
    DrawText2D(rom24, (602 + 678) / 2 - 15, (355 + 385) / 2 - 6, slevel);
    DrawText2D(rom24, (582 + 698) / 2 - 30, (260 + 290) / 2 - 6, sscore_to_next_level);
    DrawText2D(rom24, (582 + 698) / 2 - 30, (165 + 195) / 2 - 6, sscore);
    
}

/***************************************************************
    animateObject(i ,j, directioni, directionj, object)
    Inputs: Inputs: i <- our current's cell row, j <- our current's cell column, 
    destinationi <- our moving speed and direction, destinationj <- our moving speed and direction, object <- the current object.
    Returns: Nothing.
    Purpose: This function draws the current object on the game board.
    Preconditions: The game board must be filled.
    Postconditions: The current object is displayed.
***************************************************************/
void animateObject(int i, int j, int directioni, int directionj, int object) {
  int x = ((110 + 47 * j + 110 + 47 * j + 47) / 2) + directionj;
  int y = ((70 + 8 * 47 - 47 * i + 70 + 7 * 47 - 47 * i) / 2) + directioni;
   
    SetPenColor(0.5, 0.5, 0.5);
    DrawFillCircle(x, y, 20);
    SetPenColor(colors[object].red, colors[object].green, colors[object].blue);
    DrawFillCircle(x, y, 20 - 2);
    SetLineWidth(2);
    SetPenColor(0.5, 0.5, 0.5);
    DrawLine(x, y + 14, x - 14, y - 8);
    DrawLine(x - 14, y - 8, x + 14, y - 8);
    DrawLine(x + 14, y - 8, x, y + 14);
    DrawLine(x, y - 14, x + 14, y + 8);
    DrawLine(x + 14, y + 8, x - 14, y + 8);
    DrawLine(x - 14, y + 8, x, y - 14);
    SetPenColor(0.5, 0.5, 0.5);
    DrawFillCircle(x - 0.12, y + 15.5, 1);
    DrawFillCircle(x + 0.12, y + 15.5, 1);
    DrawFillCircle(x - 0.12, y - 15.5, 1);
    DrawFillCircle(x + 0.12, y - 15.5, 1);
    DrawFillCircle(x - 14.6, y + 9.6, 1);
    DrawFillCircle(x - 15.8, y + 7.6, 1);
    DrawFillCircle(x + 14.6, y + 9.6, 1);
    DrawFillCircle(x + 15.8, y + 7.6, 1);
    DrawFillCircle(x + 14.6, y - 9.6, 1);
    DrawFillCircle(x + 15.6, y - 7.6, 1);
    DrawFillCircle(x - 14.6, y - 9.6, 1);
    DrawFillCircle(x - 15.6, y - 7.6, 1);
    
}

/***************************************************************
    drawQwertyKeyboard()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function draws qwerty styled keyboard.
    Preconditions: The score on gameover is able to be imported in the high scores.
    Postconditions: The qwerty styled keyboard is displayed.
***************************************************************/
void drawQwertyKeyboard() {
  int i,j;
  
  for(i = 155; i < 155 + 47 * 10; i += 47) {
        for(j = 70; j < 70 + 47 * 4; j += 47) {
            SetPenColor(0.6, 0.4, 0.3);
            DrawFillBox(i, j, i + 45, j + 45);
	}
  }
  DrawFillBox(173, 410, 253, 455);
  DrawFillBox(520, 410, 670, 455);
  DrawFillBox(155 + 47 * 2, 68 - 47, 153 + 47 * 8, 68);
  DrawFillBox(155 + 47 * 10, 70 + 47, 153 + 47 * 11, 68 + 47 * 2);
  DrawFillBox(155 + 47 * 10, 70 + 47 * 3, 153 + 47 * 11, 68 + 47 * 4);
  DrawFillBox(155 + 47 * 11, 70 + 47, 153 + 47 * 13, 68 + 47 * 2);
  DrawFillBox(155 + 47 * 11, 70 + 47 * 3, 153 + 47 * 12, 68 + 47 * 4);
  DrawFillBox(155 + 47 * 12, 70 + 47 * 3, 153 + 47 * 13, 68 + 47 * 4);
  DrawFillBox(153, 70 + 47, 153 - 40 * 2, 68 + 47 * 2);
  DrawFillBox(153, 70 + 47 * 3, 153 - 46, 68 + 47 * 4);
  DrawFillBox(153, 70, 153 - 40 * 2, 68 + 47);
  DrawFillBox(155 + 47 * 10, 70, 155 + 47 * 12, 68 + 47);
  DrawFillBox(155 + 47 * 10, 70 + 47 * 2, 153 + 47 * 11, 68 + 47 * 3);
  DrawFillBox(155 + 47 * 11, 70 + 47 * 2, 153 + 47 * 12, 68 + 47 * 3);
  DrawFillBox(155 + 47 * 12, 70 + 47 * 2, 153 + 47 * 13, 68 + 47 * 3);
  DrawFillBox(155 - 47, 280, 155 + 47 * 12, 280 + 45);
  SetLineWidth(2);
  DrawLine(0, 90, 40 , 90);
  DrawLine(40, 90, 40 , 400);
  DrawLine(40, 400, 755 , 400);
  DrawLine(755, 400, 755 , 290);
  DrawLine(755, 290, 800 , 290);
  
  SetPenColor(0.9, 0.1, 0.1);
  DrawText2D(helv18, (75 - 47 + 75 + 47 * 12) / 2, 350, "Enter your name!");
  DrawText2D(helv18, (50 - 47 + 50 + 47 * 6) / 2, 470, "Your level!");
  DrawText2D(helv18, (50 - 47 + 50 + 47 * 22) / 2, 470, "Your score!");
  sprintf(sscore, "%06d", score);
  sprintf(slevel, "%03d", level);
  DrawText2D(rom24, (173 + 253) / 2 - 15, (410 + 455) / 2 - 6, slevel);
  DrawText2D(rom24, (520 + 670) / 2 - 30, (410 + 455) / 2 - 6, sscore);
  if(name != NULL) {
      DrawText2D(helv18, 155 - 46, 295, name);
  }
  DrawText2D(helv18, (117 - 40 * 2 + 117) / 2, (63 + 47 + 63 + 47 * 2) / 2, "CAPS L.");
  DrawText2D(helv18, (125 - 40 * 2 + 125) / 2, (63 + 63 + 47) / 2, "SHIFT");
  DrawText2D(helv18, (128 + 47 * 10 + 128 + 47 * 12) / 2, (63 + 63 + 47) / 2, "SHIFT");
  DrawText2D(helv18, (153 - 47 + 153) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "`");
  DrawText2D(helv18, (153 - 47 + 153) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "~");
  DrawText2D(helv18, (127 + 47 * 2 + 127 + 47 * 8) / 2, (63 - 47 + 63) / 2, "SPACE");
  DrawText2D(helv18, (125 + 47 * 11 + 125 + 47 * 13) / 2, (63 + 47 + 63 + 47 * 2) / 2, "ENTER");
  DrawText2D(helv18, (150 + 47 * 10 + 150 + 47 * 11) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "-");
  DrawText2D(helv18, (150 + 47 * 10 + 150 + 47 * 11) / 2, (80 + 47 * 3 + 80 + 47 * 4) / 2, "_");
  DrawText2D(helv18, (150 + 47 * 10 + 150 + 47 * 11) / 2, (50 + 47 + 50 + 47 * 2) / 2, "'");
  DrawText2D(helv18, (150 + 47 * 10 + 150 + 47 * 11) / 2, (70 + 47 + 70 + 47 * 2) / 2, "\"");
  DrawText2D(helv18, (150 + 47 * 11 + 150 + 47 * 12) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "=");
  DrawText2D(helv18, (150 + 47 * 11 + 150 + 47 * 12) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "+");
  DrawText2D(helv18, (145 + 47 * 12 + 145 + 47 * 13) / 2, (68 + 47 * 3 + 68 + 47 * 4) / 2, "BS");
  DrawText2D(helv18, (145 + 47 * 12 + 145 + 47 * 13) / 2, (53 + 47 * 3 + 53 + 47 * 4) / 2, "<-");
  DrawText2D(helv18, (148 + 47 * 10 + 148 + 47 * 11) / 2, (53 + 47 * 2 + 53 + 47 * 3) / 2, "[");
  DrawText2D(helv18, (148 + 47 * 10 + 148 + 47 * 11) / 2, (73 + 47 * 2 + 73 + 47 * 3) / 2, "{");
  DrawText2D(helv18, (148 + 47 * 11 + 148 + 47 * 12) / 2, (53 + 47 * 2 + 53 + 47 * 3) / 2, "]");
  DrawText2D(helv18, (148 + 47 * 11 + 148 + 47 * 12) / 2, (73 + 47 * 2 + 73 + 47 * 3) / 2, "}");
  DrawText2D(helv18, (148 + 47 * 12 + 148 + 47 * 13) / 2, (53 + 47 * 2 + 53 + 47 * 3) / 2, "\\");
  DrawText2D(helv18, (148 + 47 * 12 + 148 + 47 * 13) / 2, (73 + 47 * 2 + 73 + 47 * 3) / 2, "|");
  DrawText2D(helv18, (150 + 150 + 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "1");
  DrawText2D(helv18, (152 + 152 + 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "!");
  DrawText2D(helv18, (150 + 47 + 150 + 2 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "2");
  DrawText2D(helv18, (148 + 47 + 148 + 2 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "@");
  DrawText2D(helv18, (150 + 47 * 2 + 150 + 3 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "3");
  DrawText2D(helv18, (152 + 47 * 2 + 152 + 3 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "#");
  DrawText2D(helv18, (150 + 47 * 3 + 150 + 4 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "4");
  DrawText2D(helv18, (152 + 47 * 3 + 152 + 4 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "$");
  DrawText2D(helv18, (150 + 47 * 4 + 150 + 5 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "5");
  DrawText2D(helv18, (148 + 47 * 4 + 148 + 5 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "%");
  DrawText2D(helv18, (150 + 47 * 5 + 150 + 6 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "6");
  DrawText2D(helv18, (152 + 47 * 5 + 152 + 6 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "^");
  DrawText2D(helv18, (150 + 47 * 6 + 150 + 7 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "7");
  DrawText2D(helv18, (152 + 47 * 6 + 152 + 7 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "&");
  DrawText2D(helv18, (150 + 47 * 7 + 150 + 8 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "8");
  DrawText2D(helv18, (152 + 47 * 7 + 152 + 8 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "*");
  DrawText2D(helv18, (150 + 47 * 8 + 150 + 9 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "9");
  DrawText2D(helv18, (152 + 47 * 8 + 152 + 9 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, "(");
  DrawText2D(helv18, (150 + 47 * 9 + 150 + 10 * 47) / 2, (50 + 47 * 3 + 50 + 47 * 4) / 2, "0");
  DrawText2D(helv18, (152 + 47 * 9 + 152 + 10 * 47) / 2, (70 + 47 * 3 + 70 + 47 * 4) / 2, ")");
  DrawText2D(helv18, (148 + 148 + 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "Q");
  DrawText2D(helv18, (148 + 47 + 148 + 2 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "W");
  DrawText2D(helv18, (148 + 47 * 2 + 148 + 3 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "E");
  DrawText2D(helv18, (148 + 47 * 3 + 148 + 4 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "R");
  DrawText2D(helv18, (148 + 47 * 4 + 148 + 5 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "T");
  DrawText2D(helv18, (148 + 47 * 5 + 148 + 6 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "Y");
  DrawText2D(helv18, (148 + 47 * 6 + 148 + 7 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "U");
  DrawText2D(helv18, (148 + 47 * 7 + 148 + 8 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "I");
  DrawText2D(helv18, (148 + 47 * 8 + 148 + 9 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "O");
  DrawText2D(helv18, (148 + 47 * 9 + 148 + 10 * 47) / 2, (63 + 47 * 2 + 63 + 47 * 3) / 2, "P");
  DrawText2D(helv18, (148 + 148 + 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "A");
  DrawText2D(helv18, (148 + 47 + 148 + 2 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "S");
  DrawText2D(helv18, (148 + 47 * 2 + 148 + 3 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "D");
  DrawText2D(helv18, (148 + 47 * 3 + 148 + 4 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "F");
  DrawText2D(helv18, (148 + 47 * 4 + 148 + 5 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "G");
  DrawText2D(helv18, (148 + 47 * 5 + 148 + 6 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "H");
  DrawText2D(helv18, (148 + 47 * 6 + 148 + 7 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "J");
  DrawText2D(helv18, (148 + 47 * 7 + 148 + 8 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "K");
  DrawText2D(helv18, (148 + 47 * 8 + 148 + 9 * 47) / 2, (63 + 47 + 63 + 47 * 2) / 2, "L");
  DrawText2D(helv18, (148 + 47 * 9 + 148 + 10 * 47) / 2, (53 + 47 + 53 + 47 * 2) / 2, ";");
  DrawText2D(helv18, (148 + 47 * 9 + 148 + 10 * 47) / 2, (73 + 47 + 73 + 47 * 2) / 2, ":");
  DrawText2D(helv18, (148 + 148 + 47) / 2, (63 + 63 + 47) / 2, "Z");
  DrawText2D(helv18, (148 + 47 + 148 + 2 * 47) / 2, (63 + 63 + 47) / 2, "X");
  DrawText2D(helv18, (148 + 47 * 2 + 148 + 3 * 47) / 2, (63 + 63 + 47) / 2, "C");
  DrawText2D(helv18, (148 + 47 * 3 + 148 + 4 * 47) / 2, (63 + 63 + 47) / 2, "V");
  DrawText2D(helv18, (148 + 47 * 4 + 148 + 5 * 47) / 2, (63 + 63 + 47) / 2, "B");
  DrawText2D(helv18, (148 + 47 * 5 + 148 + 6 * 47) / 2, (63 + 63 + 47) / 2, "N");
  DrawText2D(helv18, (148 + 47 * 6 + 148 + 7 * 47) / 2, (63 + 63 + 47) / 2, "M");
  DrawText2D(helv18, (150 + 47 * 7 + 150 + 8 * 47) / 2, (53 + 53 + 47) / 2, ",");
  DrawText2D(helv18, (150 + 47 * 7 + 150 + 8 * 47) / 2, (73 + 73 + 47) / 2, "<");
  DrawText2D(helv18, (150 + 47 * 8 + 150 + 9 * 47) / 2, (53 + 53 + 47) / 2, ".");
  DrawText2D(helv18, (150 + 47 * 8 + 150 + 9 * 47) / 2, (73 + 73 + 47) / 2, ">");
  DrawText2D(helv18, (150 + 47 * 9 + 150 + 10 * 47) / 2, (53 + 53 + 47) / 2, "/");
  DrawText2D(helv18, (150 + 47 * 9 + 150 + 10 * 47) / 2, (73 + 73 + 47) / 2, "?");
  
}

/***************************************************************
    drawHighScores()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function draws the sorted list of high scores.
    Preconditions: The high scores button was pressed or a score was just been imported to the list.
    Postconditions: The sorted list of high scores is displayed.
***************************************************************/
void drawHighScores() {
  list *current;
  int i;
  
    SetPenColor(0.6, 0.4, 0.3);
    DrawFillBox(90, 70 + 47 * 5, 90 - 40 * 2, 68 + 47 * 6);
    DrawFillBox(92, 70 + 47 * 5, 92 + 47 * 2, 68 + 47 * 6);
    DrawFillBox(94 + 47 * 2, 70 + 47 * 5, 94 + 47 * 15 - 10, 68 + 47 * 6);
    DrawFillBox(90, 70 + 47 * 4, 90 - 40 * 2, 68 + 47 * 5);
    DrawFillBox(92, 70 + 47 * 4, 92 + 47 * 2, 68 + 47 * 5);
    DrawFillBox(94 + 47 * 2, 70 + 47 * 4, 94 + 47 * 15 - 10, 68 + 47 * 5);
    DrawFillBox(90, 70 + 47 * 3, 90 - 40 * 2, 68 + 47 * 4);
    DrawFillBox(92, 70 + 47 * 3, 92 + 47 * 2, 68 + 47 * 4);
    DrawFillBox(94 + 47 * 2, 70 + 47 * 3, 94 + 47 * 15 - 10, 68 + 47 * 4);
    DrawFillBox(90, 70 + 47 * 2, 90 - 40 * 2, 68 + 47 * 3);
    DrawFillBox(92, 70 + 47 * 2, 92 + 47 * 2, 68 + 47 * 3);
    DrawFillBox(94 + 47 * 2, 70 + 47 * 2, 94 + 47 * 15 - 10, 68 + 47 * 3);
    DrawFillBox(90, 70 + 47, 90 - 40 * 2, 68 + 47 * 2);
    DrawFillBox(92, 70 + 47, 92 + 47 * 2, 68 + 47 * 2);
    DrawFillBox(94 + 47 * 2, 70 + 47, 94 + 47 * 15 - 10, 68 + 47 * 2);
    SetLineWidth(2);
    DrawLine(0, 32 + 47 * 7, 20, 32 + 47 * 7);
    DrawLine(20, 32 + 47 * 7, 20, 54 + 47 * 7);
    DrawLine(20, 54 + 47 * 7, 84, 54 + 47 * 7);
    DrawLine(84, 54 + 47 * 7, 84, 32 + 47 * 7);
    DrawLine(84, 32 + 47 * 7, 104, 32 + 47 * 7);
    DrawLine(104, 32 + 47 * 7, 104, 54 + 47 * 7);
    DrawLine(104, 54 + 47 * 7, 176, 54 + 47 * 7);
    DrawLine(176, 54 + 47 * 7, 176, 32 + 47 * 7);
    DrawLine(176, 32 + 47 * 7, 453, 32 + 47 * 7);
    DrawLine(453, 32 + 47 * 7, 453, 54 + 47 * 7);
    DrawLine(453, 54 + 47 * 7, 519, 54 + 47 * 7);
    DrawLine(519, 54 + 47 * 7, 519, 32 + 47 * 7);
    DrawLine(519, 32 + 47 * 7, 800, 32 + 47 * 7);
    DrawFillBox((90 - 40 * 2 + 90) / 2 + 290, (70 + 47 * 8 + 68 + 47 * 9) / 2 - 30, (90 - 40 * 2 + 90) / 2 + 410, (70 + 47 * 8 + 68 + 47 * 9) / 2 + 13);
    SetPenColor(0.9, 0.1, 0.1);
    DrawText2D(helv18, (90 - 40 * 2 + 90) / 2 + 300, (70 + 47 * 8 + 68 + 47 * 9) / 2 - 15, "High Scores");
    DrawText2D(helv18, (90 - 40 * 2 + 90) / 2 - 20, (70 + 47 * 6 + 68 + 47 * 7) / 2 - 15, "Level");
    DrawText2D(helv18, (92 + 92 + 47 * 2) / 2 - 24, (70 + 47 * 6 + 68 + 47 * 7) / 2 - 15, "Score");
    DrawText2D(helv18, (94 + 47 * 2 + 94 + 47 * 15) / 2 - 30, (70 + 47 * 6 + 68 + 47 * 7) / 2 - 15, "Name"); 
    for(current = root, i = 6; current != NULL && current -> level_high_score != 0; current = current -> next, i--) {
        sprintf(high_scores_score, "%06d", current -> score_high_score);
	sprintf(high_scores_level, "%03d", current -> level_high_score);
	DrawText2D(rom24, (90 - 40 * 2 + 90) / 2 - 15, (70 + 47 * (i - 1) + 68 + 47 * i) / 2 - 6, high_scores_level);
	DrawText2D(rom24, (92 + 92 + 47 * 2) / 2 - 30, (70 + 47 * (i - 1) + 68 + 47 * i) / 2 - 6, high_scores_score);
	DrawText2D(rom24, (94 + 47 * 2 + 94 + 47 * 15) / 2 - 302, (70 + 47 * (i - 1) + 68 + 47 * i) / 2 - 6, current -> name_high_score);                            
    }
    
}

/***************************************************************
    addToString(qwerty, spot)
    Inputs: qwerty <- a character from the qwerty keyboard, spot <- the character's placement on the string.
    Returns: Nothing.
    Purpose: This function adds characters on a dynamic defined string.
    Preconditions: spot cant be less that 0 or greater than 32.
    Postconditions: The characted was added to the string.
***************************************************************/
void addToString(char qwerty, int spot) {
  
    if(capslock_pressed == CAPSLOCK0 && qwerty >= 65 && qwerty <= 90) {
        name = (char *) realloc(name, (spot + 2) * sizeof(char));
	if(name == NULL) {
	    printf("Not enough memory.\n");
	    exit(0);
	}
        name[spot] = qwerty + 'a' - 'A';
        name[spot + 1] = '\0';
    }
    else {
        name = (char *) realloc(name, (spot + 2) * sizeof(char));
	if(name == NULL) {
	    printf("Not enough memory.\n");
	    exit(0);
	}
        name[spot] = qwerty;
        name[spot + 1] = '\0';
    }

}

/***************************************************************
    listInsertInitialization()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function adds a new node on top of the list.
    Preconditions: None.
    Postconditions: A node was added in the list.
***************************************************************/
void listInsertInitialization() {
  list *current;
   
    current = (list*) malloc(sizeof(list));
    if(current == NULL) {
        printf("Not enough memory.\n");
        exit(0);
    }
    current -> level_high_score = 0;
    current -> score_high_score = 0;
    current -> next = root;
    root = current;
    
}

/***************************************************************
    clearList()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function clears the list but restoring the allocated memory.
    Preconditions: None.
    Postconditions: The list was cleared.
***************************************************************/
void clearList() {
  list *current, *previous;
  
    while(root != NULL) {
        current = root;
        previous = current;
        current = current -> next;
        root = current;
        free(previous);
    }
    
}

/***************************************************************
    listInsertNewScore()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function adds a node on the list in its correct place in order to be sorted according to the field score and deletes
    the last node.
    Preconditions: The score must be able to be imported to the list.
    Postconditions: The score was added to the list.
***************************************************************/
void listInsertNewScore() {
  list *current, *new, *previous;
  int i;
  
    if(checkScore() == 1 || checkScore() == 2 || checkScore() == 3 || checkScore() == 4) {
        for(current = root, previous = NULL, i = 1; i <= checkScore(); previous = current, current = current -> next, i++){}
	
	new = (list*) malloc(sizeof(list));
	if(new == NULL) {
	    printf("Not enough memory.\n");
	    exit(0);
	}
	new -> level_high_score = level;
	new -> score_high_score = score;
	strcpy(new -> name_high_score, name);
	previous -> next = new;
	new -> next = current;
	
	for(current = root, previous = NULL, i = 0; i < 5; previous = current, current = current -> next, i++) {}
	
	previous -> next = NULL;
	free(current);
    }

    if(checkScore() == 0) {
        new = (list*) malloc(sizeof(list));
	if(new == NULL) {
	    printf("Not enough memory.\n");
	    exit(0);
	}
	new -> level_high_score = level;
	new -> score_high_score = score;
	strcpy(new -> name_high_score, name);
	new -> next = root;
	root = new;
	
	for(current = root, previous = NULL, i = 0; i < 5; previous = current, current = current -> next, i++) {}
	
	previous -> next = NULL;
	free(current);
    }  
    
}

/***************************************************************
    checkScore()
    Inputs: None.
    Returns: Nothing.
    Purpose: This function checks if the score on gameover can be imported in the sorted list and if yes, in which place.
    Preconditions: The game must be in the state gameover.
    Postconditions: The score was checked.
***************************************************************/
int checkScore() {
  list *current;
  
    current = root;
    
        if(score > current -> score_high_score) {
	    return 0;
	}
	
	current = current -> next;
	
	if(score > current -> score_high_score) {
	    return 1;
	}
	
	current = current -> next;

	if(score > current -> score_high_score) {
	    return 2;
	}
	
	current = current -> next;
	if(score > current -> score_high_score) {
	    return 3;
	}
	
	current = current -> next;
	
	if(score > current -> score_high_score) {
	    return 4;
	}
	
  return 7;
}
