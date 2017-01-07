/*
    Mingfei Jiang
    1320376
    COMPSCI 3GC3 Final Project
    Magic Tower
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <math.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
using namespace std;

#include "layout.h"
#include "entity.h"

/*
    GLOBALS
*/
// Position of the Player relative to the floor
int playerPos[] = {6,0,1};
// Width/Height of the floors
int floorWidth = 13;
int floorHeight = 13;
// the floor the player is currently on
int currentFloor = 0;
// used to animate rotating objects
int itemRotateAngle = 0;
// price of the shop
int shopPrice = 20;
// used to store messages to be shown in the console
vector<string> messageStack;
// the player object
extern Player* player;
// the layout of the floors stored as strings
extern vector<vector<string>> floors;
// the actual floors containing the different entities
vector<vector<shared_ptr<Entity>>> floorVectors;
// the currently selected enemy, used to show stats on the top right
shared_ptr<Enemy> selectedEnemy;
// camera distance
float camDistance = 10;
// draws text on the screen
void drawStrokeText(string s, int x, int y, bool rightAlign=false)
{
    // subtract the width of the text from x if it is right aligned
    if(rightAlign){
        x = x - 9*s.length();
    }
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT));
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            glLoadIdentity();
            glColor3f(0,1,0);
            glRasterPos2i(x, y);
            void * font = GLUT_BITMAP_9_BY_15;
            for (std::string::iterator i = s.begin(); i != s.end(); ++i)
            {
                char c = *i;
                glutBitmapCharacter(font, c);
            }
            glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// project a ray from where the cursor clicked to find the corresponding world coordinate
Vertex getWorldCoordinates(int x, int y){
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    
    Vertex ret = {posX, posY, posZ};
    return ret;
}

// initialize all the entities based on the layout
void initializeFloors(){
    printf("INITIALIZING FLOORS...\n");
    // iterate though the floors
    for (int floorNum=0; floorNum<floors.size(); floorNum++){
        vector<shared_ptr<Entity>> floor;
        // iterate through the rows of each floor
        for (int row = 0; row<floorHeight; row++) {
            // iterate through the columns of each row
            for (int col=0;col<floorWidth;col++){
                // create an Entity based on what character the layout specified
                switch(floors[floorNum][row][col]){
                    case '.': floor.push_back(shared_ptr<Entity>(new Block(0))); break;
                    case ' ': floor.push_back(shared_ptr<Entity>(new Block(1))); break;
                    case 'y': floor.push_back(shared_ptr<Entity>(new Item(2))); break;
                    case 'b': floor.push_back(shared_ptr<Entity>(new Item(3))); break;
                    case 'r': floor.push_back(shared_ptr<Entity>(new Item(4))); break;
                    case 'Y': floor.push_back(shared_ptr<Entity>(new Item(5))); break;
                    case 'B': floor.push_back(shared_ptr<Entity>(new Item(6))); break;
                    case 'R': floor.push_back(shared_ptr<Entity>(new Item(7))); break;
                    case '!': floor.push_back(shared_ptr<Entity>(new Item(8))); break;
                    case '@': floor.push_back(shared_ptr<Entity>(new Item(9))); break;
                    case 'd': floor.push_back(shared_ptr<Entity>(new Item(20))); break;
                    case 'a': floor.push_back(shared_ptr<Entity>(new Item(21))); break;
                    case '1': floor.push_back(shared_ptr<Entity>(new Enemy(1))); break;
                    case '2': floor.push_back(shared_ptr<Entity>(new Enemy(2))); break;
                    case '3': floor.push_back(shared_ptr<Entity>(new Enemy(3))); break;
                    case '4': floor.push_back(shared_ptr<Entity>(new Enemy(4))); break;
                    case '5': floor.push_back(shared_ptr<Entity>(new Enemy(5))); break;
                    case '6': floor.push_back(shared_ptr<Entity>(new Enemy(6))); break;
                    case '7': floor.push_back(shared_ptr<Entity>(new Enemy(7))); break;
                    case '8': floor.push_back(shared_ptr<Entity>(new Enemy(8))); break;
                    case 's': floor.push_back(shared_ptr<Entity>(new Item(10))); break;
                    case 'S': floor.push_back(shared_ptr<Entity>(new Item(11))); break;
                    case 'U': {
                        switch(floorNum){
                            case 0:  floor.push_back(shared_ptr<Entity>(new Stair('w', 'u'))); break;
                            case 1:  floor.push_back(shared_ptr<Entity>(new Stair('e', 'u'))); break;
                            case 2:  floor.push_back(shared_ptr<Entity>(new Stair('w', 'u'))); break;
                            case 3:  floor.push_back(shared_ptr<Entity>(new Stair('s', 'u'))); break;
                            case 4:  floor.push_back(shared_ptr<Entity>(new Stair('w', 'u'))); break;
                            case 5:  floor.push_back(shared_ptr<Entity>(new Stair('s', 'u'))); break;
                            case 6:  floor.push_back(shared_ptr<Entity>(new Stair('s', 'u'))); break;
                            case 7:  floor.push_back(shared_ptr<Entity>(new Stair('n', 'u'))); break;
                            case 8:  floor.push_back(shared_ptr<Entity>(new Stair('n', 'u'))); break;
                            case 9:  floor.push_back(shared_ptr<Entity>(new Stair('s', 'u'))); break;
                        }
                        break;
                    } 
                    case 'D': {
                        switch(floorNum){
                            case 0:  floor.push_back(shared_ptr<Entity>(new Stair('w', 'd'))); break;
                            case 1:  floor.push_back(shared_ptr<Entity>(new Stair('w', 'd'))); break;
                            case 2:  floor.push_back(shared_ptr<Entity>(new Stair('w', 'd'))); break;
                            case 3:  floor.push_back(shared_ptr<Entity>(new Stair('s', 'd'))); break;
                            case 4:  floor.push_back(shared_ptr<Entity>(new Stair('s', 'd'))); break;
                            case 5:  floor.push_back(shared_ptr<Entity>(new Stair('e', 'd'))); break;
                            case 6:  floor.push_back(shared_ptr<Entity>(new Stair('n', 'd'))); break;
                            case 7:  floor.push_back(shared_ptr<Entity>(new Stair('n', 'd'))); break;
                            case 8:  floor.push_back(shared_ptr<Entity>(new Stair('s', 'd'))); break;
                            case 9:  floor.push_back(shared_ptr<Entity>(new Stair('e', 'd'))); break;
                        }
                        break;
                    }
                }
            }
        }
        floorVectors.push_back(floor);
    }
}

// glut functions
void keyboard(unsigned char key, int xIn, int yIn)
{
    int mod = glutGetModifiers();
    int entityIndex;
    bool collition = true;
    switch (key)
    {
        // esc or q to exit the game
        case 'q':
        case 27:
            exit(0);
            break;
        // wasd to interact with an entity next to the player
        // the interact function returns a boolean value "collision"
        // which tells the player if it's okay to move onto that block
        case 'd':
            entityIndex = playerPos[0] + 1 + (playerPos[2])*floorWidth;
            collition = floorVectors[currentFloor][entityIndex]->interact();
            if(collition){
            }else{
                playerPos[0] += 1;
            }
            break;
        case 'a':
            entityIndex = playerPos[0] - 1 + (playerPos[2])*floorWidth;
            collition = floorVectors[currentFloor][entityIndex]->interact();
            if(collition){
            }else{
                playerPos[0] -= 1;
            }
            break;
        case 's':
            entityIndex = playerPos[0] + (playerPos[2]+1)*floorWidth;
            collition = floorVectors[currentFloor][entityIndex]->interact();
            if(collition){
            }else{
                playerPos[2] += 1;
            }
            break;
        case 'w':
            entityIndex = playerPos[0] + (playerPos[2]-1)*floorWidth;
            collition = floorVectors[currentFloor][entityIndex]->interact();
            if(collition){
            }else{
                playerPos[2] -= 1;
            }
            break;
        // r to reset the game
        case 'r':
            // reset all variables
            currentFloor = 0;
            shopPrice = 20;
            player = new Player();
            floorVectors.clear();
            // reinitialize the floors
            initializeFloors();
            playerPos[0] = 6;
            playerPos[2] = 1;
            // clear the message stack
            messageStack.clear();
            messageStack.push_back("Game has been reset.");
            break;
    }
}

void special(int key, int x, int y) {
    switch (key) {
        // zoom the camera in and out
        case GLUT_KEY_PAGE_UP:
            if(camDistance>5){
                camDistance-=0.2;
            }
            break;
        case GLUT_KEY_PAGE_DOWN:
            if (camDistance<20){
                camDistance+=0.2;
            }
            break;
    }
}

// glut init function
void init(void)
{
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);

    glEnable(GL_LIGHTING); 
    
    initializeFloors();

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);
}

// glut display function
void display(void)
{
    // reset the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // calculate the camPos and the cam target
    float camPos[3] = {playerPos[0],1.5*camDistance,playerPos[2]+camDistance};
    float camTarget[3] = {playerPos[0],playerPos[1],playerPos[2]};

    // enable lighting
    // 5 lights are used (with 1 main light and 4 support ones)
    // to ensure a well lit scene
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    // set the ambient diffuse and specular for the main and support lights
    float amb0[4]  = {0,0,0,1}; float diff0[4] = {1, 1, 1, 1}; float spec0[4] = {1, 1, 1, 1}; 
    float amb1[4]  = {0,0,0,1}; float diff1[4] = {0.5, 0.5, 0.5, 1}; float spec1[4] = {0,0,0,1}; 
    // set the lighting positions
    float lightPos0[4] = {playerPos[0],playerPos[1]+5,playerPos[2], 1};
    float lightPos1[4] = {playerPos[0]+10,playerPos[1]+5,playerPos[2]+10, 1};
    float lightPos2[4] = {playerPos[0]+10,playerPos[1]+5,playerPos[2]-10, 1};
    float lightPos3[4] = {playerPos[0]-10,playerPos[1]+5,playerPos[2]+10, 1};
    float lightPos4[4] = {playerPos[0]-10,playerPos[1]+5,playerPos[2]-10, 1};
    // set the light properties
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1); 
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1); 
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);
    glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
    glLightfv(GL_LIGHT2, GL_AMBIENT, amb1); 
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diff1); 
    glLightfv(GL_LIGHT2, GL_SPECULAR, spec1);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
    glLightfv(GL_LIGHT3, GL_AMBIENT, amb1); 
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diff1); 
    glLightfv(GL_LIGHT3, GL_SPECULAR, spec1);
    glLightfv(GL_LIGHT4, GL_POSITION, lightPos4);
    glLightfv(GL_LIGHT4, GL_AMBIENT, amb1); 
    glLightfv(GL_LIGHT4, GL_DIFFUSE, diff1); 
    glLightfv(GL_LIGHT4, GL_SPECULAR, spec1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camPos[0], camPos[1], camPos[2], camTarget[0], camTarget[1], camTarget[2], 0,1,0);

    // increment the rotate angle for rotate animation
    itemRotateAngle++;

    // iterate through the floors
    for(int row=0; row<floorHeight; row++){
       for(int col=0; col<floorWidth; col++){
            glPushMatrix();
            // translate the entity to their corresponding location on the floor
            glTranslatef(col, 0, row);
                glPushMatrix();
                    // draw the entity
                    floorVectors[currentFloor][row*floorWidth+col]->draw();
                glPopMatrix();
            glPopMatrix();
        }
    }

    // reset the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // translate the draw the player at its location
    glPushMatrix();
    glTranslatef(playerPos[0], 0, playerPos[2]);
    player->draw();  
    glPopMatrix();

    // set the text color
    float mat_emission[] ={0,1,0,1};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
    // get the window width/height
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);

    // write floor/player info
    drawStrokeText("FLOOR  "       + to_string(currentFloor+1),     10, windowHeight-20);
    drawStrokeText("HP: "          + to_string(player->hp),         10, windowHeight-60);
    drawStrokeText("Attack: "      + to_string(player->att),        10, windowHeight-80);
    drawStrokeText("Defense: "     + to_string(player->def),        10, windowHeight-100);

    drawStrokeText("Yellow Keys: " + to_string(player->yellowKeys), 10, windowHeight-140);
    drawStrokeText("Blue Keys: "   + to_string(player->blueKeys),   10, windowHeight-160);
    drawStrokeText("Red Keys: "    + to_string(player->redKeys),    10, windowHeight-180);
    drawStrokeText("Gold: "        + to_string(player->gold),       10, windowHeight-200);

    // write enemy info if one is selected
    if(selectedEnemy){
        drawStrokeText(selectedEnemy->name, windowWidth-10, windowHeight-20, true);
        drawStrokeText("HP: "               + to_string(selectedEnemy->hp),  windowWidth-10, windowHeight-40, true);
        drawStrokeText("Attack: "           + to_string(selectedEnemy->att), windowWidth-10, windowHeight-60, true);
        drawStrokeText("Defense: "          + to_string(selectedEnemy->def), windowWidth-10, windowHeight-80, true);
        drawStrokeText("Gold: "             + to_string(selectedEnemy->gold), windowWidth-10, windowHeight-100, true);
        drawStrokeText("Projected Damage: " + to_string(selectedEnemy->getProjectedDamage()),  windowWidth-10, windowHeight-120, true);
    }

    // write the last 10 items in the message stack
    int h =10;
    for(int i=messageStack.size()-1; i>=max(0, (int)messageStack.size()-10); i--){
        drawStrokeText(messageStack[i], 10, h);
        h+=20;
    }

    //switch our buffers for a smooth animation
    glutSwapBuffers();
}
// glut mouse function
void mouse(int btn, int state, int x, int y){
    // left click selects an object
    if (btn == GLUT_LEFT_BUTTON){
        if (state == GLUT_UP){
            // find the world coordinate
            Vertex v = getWorldCoordinates(x, y);
            // if the coordinate is within the floor
            if(v.x>0 && v.z>0 && v.x<floorWidth && v.z<floorHeight){
                // and the selected entity is an enemy
                if(floorVectors[currentFloor][(int)v.x+floorWidth*(int)v.z]->getType()==6){
                    // set the selected enemy to the one clicked on by the user
                    selectedEnemy = dynamic_pointer_cast<Enemy>(floorVectors[currentFloor][(int)v.x+floorWidth*(int)v.z]);
                }
            }
        }
    }
}

// glut reshape function
void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)((w+0.0f)/h), 1, 100);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

//glut timer function
void FPS(int val){
    glutPostRedisplay();
    glutTimerFunc(17, FPS, 0); // 1sec = 1000, 60fps = 1000/60 = ~17
}

void callBackInit(){
    glutDisplayFunc(display);   //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, FPS, 0);
}

// functionality for the shop
void menuProc(int value){
    if (player->gold >= shopPrice){
        // buy hp
        if (value == 1){
            player->setHp(player->getHp() + 500);
            messageStack.push_back("You bought 500HP");
        // buy attack
        } else if (value == 2){
            player->att+=5;
            messageStack.push_back("You bought 5 ATT");
        // buy defense
        } else if (value == 3){
            player->def+=5;
            messageStack.push_back("You bought 5 DEF");
        }
        //subtract player gold
        player->gold -= shopPrice;
    } else {
        messageStack.push_back("Not Enough Gold!");
    }
}

// create the shop
void createOurMenu(){
    int main_id = glutCreateMenu(menuProc);
    glutAddMenuEntry("+500 HP (20 Gold)", 1);
    glutAddMenuEntry("+5 ATT (20 Gold)", 2);
    glutAddMenuEntry("+5 DEF (20 Gold)", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/* main function - program entry point */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);      //starts up GLUT
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    printf("\n\n========================== MAGIC TOWER INSTRUCTIONS ==========================\n\n");
    printf("  Magic tower is a puzzle RPG game where you navigate through a dangerous tower\n");
    printf("  in hopes of finding and defeating the legendary dragomon \n\n");
    printf("  Find gems and equipment scatter around the tower to upgrade your character\n");
    printf("  Kill monsters for gold that you can spend in the shop\n");
    printf("  Find colored keys used to open doors of the matching color\n");
    printf("  Find potions to heal\n\n");
    printf("  This tower consists of 10 floors and should be relatively easy to complete\n\n");
    printf("  CONTROLS:\n");
    printf("  W,A,S,D to move the character \n");
    printf("  LEFT CLICK on an enemy to view its stats\n");
    printf("  RIGHT CLICK to open the shop\n");
    printf("  PAGE UP and PAGE DOWN to zoom the camera in and out\n");
    printf("  Press r to reset the game.\n");
    printf("  Press q or esc to exit the program.\n\n");
    printf("  HINTS:\n");
    printf("  Upgrade your character whenever you have enough gold to do so.\n");
    printf("  Remember to backtrack for keys, potions, and gold.\n\n");
    printf("================================================================================\n\n");

    cout << "Press Enter to Continue...";
    cin.ignore();



    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Magic Tower");    //creates the window

    callBackInit();
    init();
    createOurMenu();
    loadTextures();



    glutMainLoop();             //starts the event glutMainLoop

    return(0);                  //return may not be necessary on all compilers
}
