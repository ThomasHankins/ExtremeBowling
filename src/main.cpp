#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif



#ifndef MAIN
#define MAIN

#include <chrono> // for more accurate time checking
#include <map>
#include <random>

#include "misc/camera.h"
#include "misc/direction_controller.h"
#include "misc/level.h"

#include "menu/menu.h"
#include "menu/startMenu.h"
#include "menu/instructionsMenu.h"
#include "menu/pauseMenu.h"
#include "menu/endMenu.h"
#include "menu/hudInterface.h"

using namespace std;
using namespace std::chrono;

#define DEFAULT_SIZE_X 800
#define DEFAULT_SIZE_Y 600

int refreshRate;
float windowX = DEFAULT_SIZE_X; // need to initialize this first here for menu parameters
float windowY = DEFAULT_SIZE_Y;

bool pauseStatus;
bool startStatus;
bool instructionsStatus;
bool endStatus;

bool showFPS;
bool showHitbox;

int frameTime;
int frameCount;
int time_past;

int prevX;
int prevY;

Camera ballCam(40);

StartMenu startMenu(windowX, windowY);
InstructionsMenu instructionsMenu(windowX, windowY);
PauseMenu pauseMenu(windowX, windowY);
EndMenu endMenu(windowX, windowY);
HudInterface hudInterface(windowX, windowY);

DirectionController arrowKeys;

Level currentLevel;

void keyboard(unsigned char key, int _x, int _y) {

    if (key == '`'){
        showFPS = !showFPS;
    }

    if (key  == ' ' && !startStatus){
        pauseStatus = !pauseStatus;
    }

    if (key == 'r' && !pauseStatus){ 
        currentLevel.ballReset();
    }

    if (key == '~'){
        showHitbox = !showHitbox;
    }

    if (key == 'w')
        arrowKeys.toggleUp(true);
    else if (key == 's')
        arrowKeys.toggleDown(true);
    else if (key == 'd')
        arrowKeys.toggleRight(true);
    else if (key == 'a')
        arrowKeys.toggleLeft(true);

    glutPostRedisplay();
}


void keyboardUp(unsigned char key, int _x, int _y) {
    if (key == 'w')
        arrowKeys.toggleUp(false);
    else if (key == 's')
        arrowKeys.toggleDown(false);
    else if (key == 'd')
        arrowKeys.toggleRight(false);
    else if (key == 'a')
        arrowKeys.toggleLeft(false);

    glutPostRedisplay();
}



void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (startStatus) {
            if (startMenu.instructionsClicked(x, y)) { // switch to instructions menu
                startStatus = false;
                instructionsStatus = true;
            } else if (startMenu.quitClicked(x, y)) { // quit program
                exit(0);
            } else if (startMenu.level1Clicked(x, y)) {
                startStatus = false;
                pauseStatus = false;
                currentLevel.init("src/levels/map1.json");
            } 
            else if (startMenu.level2Clicked(x, y)) {
                startStatus = false;
                pauseStatus = false;
                currentLevel.init("src/levels/map2.json");
            }
            else if (startMenu.level3Clicked(x, y)) {
                startStatus = false;
                pauseStatus = false;
                currentLevel.init("src/levels/map3.json");
            }
        } else if (instructionsStatus) {
            if (instructionsMenu.backClicked(x, y)) {
                instructionsStatus = false;
                startStatus = true;
            }
        } else if (endStatus) {
            if (endMenu.backToStartClicked(x, y)) {
                endStatus = false;
                startStatus = true;
            }
        } else { 
            currentLevel.ballJump();
        }
    }
    prevX = x;
    prevY = y;
    glutPostRedisplay();
}

void motion(int x, int y){
    if(!pauseStatus){
        ballCam.orbit(x - prevX, y - prevY);
    }
    if (x > windowX || x < 0 || y > windowY || y < 0) {
        pauseStatus = true; // if mouse outside window, game pauses
    } 
    prevX = x;
    prevY = y; 
}

void passive(int x, int y){
    if(!pauseStatus){
        ballCam.orbit(x - prevX, y - prevY);

        arrowKeys.setForward(ballCam.getForward());
        arrowKeys.setRightward(ballCam.getRight());
    }
    if (x > windowX || x < 0 || y > windowY || y < 0) {
        pauseStatus = true; // if mouse outside window, game pauses
    }
    prevX = x;
    prevY = y;
}

void windowEntry(int state){
    if (state == GLUT_LEFT)
        pauseStatus = true;
}


void special(int key, int _x, int _y){
    if (key == GLUT_KEY_UP)
        arrowKeys.toggleUp(true);
    else if (key == GLUT_KEY_DOWN)
        arrowKeys.toggleDown(true);
    else if (key == GLUT_KEY_RIGHT)
        arrowKeys.toggleRight(true);
    else if (key == GLUT_KEY_LEFT)
        arrowKeys.toggleLeft(true);

    glutPostRedisplay();
}

void specialUp(int key, int _x, int _y){
    if (key == GLUT_KEY_UP)
        arrowKeys.toggleUp(false);
    else if (key == GLUT_KEY_DOWN)
        arrowKeys.toggleDown(false);
    else if (key == GLUT_KEY_RIGHT)
        arrowKeys.toggleRight(false);
    else if (key == GLUT_KEY_LEFT)
        arrowKeys.toggleLeft(false);

    glutPostRedisplay();
}

void FPS (int val){
    int time_current = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    int d_time = time_current - time_past;
    frameTime = d_time;
    time_past = time_current; 

    if(!pauseStatus){
        currentLevel.ballMove(arrowKeys.getDirection());
        currentLevel.runLevel(d_time);
        ballCam.changePosition(currentLevel.getBallX(),currentLevel.getBallY(),currentLevel.getBallZ());

        if(currentLevel.getEnded()){
            endStatus = true;
            pauseStatus = true;
            float currentScore = currentLevel.getScore();
            float highScore = currentLevel.getHighScore();

            if(currentLevel.endLevel()){
                if(currentScore > highScore){
                    // Victory Screen call congrats new high score
                    endMenu.setText("STRIKE! New High Score!", currentScore);
                } else {
                    //Victory Screen
                    endMenu.setText("Level Complete!", currentScore);
                }
            } else {
                //Call game over menu
                endMenu.setText("GUTTER BALL! You Lose!", currentScore);
            }
            currentLevel = Level();
        }
    
    }

    glutPostRedisplay();


    d_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - time_past;
    
    frameCount += 1;
    if(d_time > 1000 / refreshRate){
        glutTimerFunc(1, FPS, 0);
        
    } else {
        glutTimerFunc(1000 / refreshRate, FPS, 0);
    }
}


void displayFPS(){

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, windowX, 0.0, windowY);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glPushMatrix();
        glDisable(GL_LIGHTING);
        
        glColor3f(0.0, 1.0, 0.0);
        glRasterPos2i(10, windowY - 15);

        string s = to_string(1000 / (frameTime+1));
        void * font = GLUT_BITMAP_9_BY_15;
        for (string::iterator i = s.begin(); i != s.end(); ++i)
        {
            char c = *i;
            glutBitmapCharacter(font, c);
        }

        glEnable(GL_LIGHTING);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

void display(void)
{
    if (startStatus) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pauseStatus = true; // prevent movement of other things
        startMenu.display();
    } else if (instructionsStatus) {
        pauseStatus = true; // prevent movement of other things
        instructionsMenu.display();
    } else if (endStatus) {
        pauseStatus = true;
        endMenu.display();
    } else if (pauseStatus) {
        if(currentLevel.getBlend()){
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }
        pauseMenu.display();
    } else {
        if(currentLevel.getBlend()){
            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70, windowX/windowY, 1, 1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        gluLookAt(
        ballCam.getX(),    ballCam.getY(),    ballCam.getZ(),
        currentLevel.getBallX(),currentLevel.getBallY(),currentLevel.getBallZ(),
        0,1,0
        );
                  
        currentLevel.displayAssets(showHitbox);
        if(showFPS){
            displayFPS();     
        }
        hudInterface.display(currentLevel.getLives(), currentLevel.getTime());
    }
    

    glFlush();
    glutSwapBuffers();

}

void init(){
    pauseStatus = true;
    windowX = DEFAULT_SIZE_X;
    windowY = DEFAULT_SIZE_Y;
    refreshRate = 120;
    frameTime = 0;
    showFPS = false;
    startStatus = true;
    showHitbox = false;

    time_past = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void handleReshape(int w, int h) {
    windowX = w;
    windowY = h;
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glColor4f(1.0f,1.0f,1.0f,0.7f);		// Full Brightness, 70% Alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);	// blending function for translucency based on source alpha value
    
    glViewport(0, 0, (GLint)w, (GLint)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70, windowX/windowY, 1, 1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}


/* main function - program entry point */
int main(int argc, char** argv)
{

    glutInit(&argc, argv);        //starts up GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    init();

    glutInitWindowSize(windowX, windowY);
    glutInitWindowPosition(300,300);
    glutCreateWindow("Extreme Bowling");    //creates the window

    //callbacks
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);


    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passive);
    glutEntryFunc(windowEntry);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutReshapeFunc(handleReshape);
    glutDisplayFunc(display);    //registers "display" as the display callback function

    
    glutTimerFunc(1000/refreshRate,FPS, 0);

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Graphics::loadTextures();

    glutMainLoop();

    return 0;                    
}





#endif