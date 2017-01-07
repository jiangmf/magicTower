/*
    Mingfei Jiang
    1320376
    COMPSCI 3GC3 Final Project
    Magic Tower
*/

#include <vector>
#include <string>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "material.h"
// import all the materials from material.h
extern Material playerMaterial;
extern Material wallMaterial;
extern Material floorMaterial;
extern Material yellowMaterial;
extern Material blueMaterial;
extern Material redMaterial;
extern Material yellowKeyMaterial;
extern Material blueKeyMaterial;
extern Material redKeyMaterial;
extern Material defaultMaterial;
#include "model.h"
// import all the models from model.h
extern Model cubeModel;
extern Model keyModel;
extern Model potionModel;
extern Model gemModel;
extern Model stairModel;
extern Model monster1Model;
extern Model monster2Model;
extern Model monster3Model;
extern Model monster4Model;
extern Model monster5Model;
extern Model monster6Model;
extern Model monster7Model;
extern Model monster8Model;
extern Model swordModel;
extern Model shieldModel;
#include "texture.h"
// import textures
extern GLuint myTex[20];
// import global variables
extern int itemRotateAngle;
extern int currentFloor;
extern vector<string> messageStack;

#ifdef _WIN32
    // define the to_string function if on windows
    // as MinGW is missing this functionality
    std::string to_string(int i)
    {
        std::stringstream ss;
        ss << i;
        return ss.str();
    }
#endif

using namespace std;

// the base class for all objects in a floor
class Entity {
private:
    int type;
public:
    bool rotates = false;
    Entity(int t) {
        type = t;
    }

    int getType() {
        return type;
    }

    void setType(int t) {
        type = t;
    }
    // the interact function lets the player interact with this entity
    // and will return a boolean value for if the player is allowed
    // to move to this entity's location
    virtual bool interact() = 0;
    virtual void draw() = 0;
};

// The Player is a subclass of Entity
class Player : public Entity {
public:
    // the player holds a number of different colored keys
    int yellowKeys = 0;
    int blueKeys = 0;
    int redKeys = 0;
    // the player's stats
    int hp = 1000;
    int att = 10;
    int def = 10;
    // how much gold the player has
    int gold = 0;

    Player(int t = 5) : Entity(t) {}
    
    bool interact() {}

    // player gets key
    void receiveKey(char key){
        switch(key){
            case 'y': yellowKeys += 1; break;
            case 'b': blueKeys += 1; break;
            case 'r': redKeys += 1; break;
        }
    }
    // player uses key
    void useKey(char key){
        switch(key){
            case 'y': yellowKeys -= 1; break;
            case 'b': blueKeys -= 1; break;
            case 'r': redKeys -= 1; break;
        }
    }
    // player gets potion
    void receivePotion(char pot){
        switch(pot){
            case 's': hp += 300; break;
            case 'm': hp += 500; break;
            case 'l': hp += 1; break;
        }
    }
    // player gets gem or key or shield
    void receiveGem(char gem){
        switch(gem){
            case 'b': def += 3; break;
            case 'r': att += 3; break;
            case 's': att += 10; break;
            case 'S': def += 10; break;
        }
    }
    // player takes damage
    void receiveHit(int dmg){
        hp -= dmg;
    }
    // player gets gold
    void receiveGold(int g){
        gold += g;
    }
    // returns player hp
    int getHp(void){
        return hp;
    }
    // sets player hp
    int setHp(int v){
        hp = v;
    }
    // draw the player
    void draw() {
        glBindTexture(GL_TEXTURE_2D, 0);
        playerMaterial.set();
        glTranslatef(0.5,0.5,0.5);
        glutSolidSphere(0.5, 100, 100);
    }
};
// initialize the player
Player* player = new Player();

// blocks are subclasses of Entities
class Block : public Entity {
private:
    int blockId;
public:
    Block(int id, int t = 0) : Entity(t) {
        blockId = id;
    }
    
    bool interact() {
        if (blockId == 0) {
            // wall, collides with player
            return true;
        }
        else if (blockId == 1) {
            // floor, does not collide with player
            return false;
        }
    }

    void draw() {
        if (blockId == 0) {
            // draw the wall
            glBindTexture(GL_TEXTURE_2D, myTex[0]);
            wallMaterial.set();
            glTranslatef(0.5,0.5,0.5);
            cubeModel.draw();
        }
        else if (blockId == 1) {
            // draw the floor
            glBindTexture(GL_TEXTURE_2D, myTex[1]);
            floorMaterial.set();
            glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glNormal3f(0,1,0);
                glVertex3f(0,0,0);
                glTexCoord2f(1,0);
                glNormal3f(0,1,0);
                glVertex3f(1,0,0);
                glTexCoord2f(1,1);
                glNormal3f(0,1,0);
                glVertex3f(1,0,1);
                glTexCoord2f(0,1);
                glNormal3f(0,1,0);
                glVertex3f(0,0,1);
            glEnd();
        }
    }
};

// stairs are subclasses of entities
class Stair : public Entity {
private:
    // stairs have an orientation (which way it's pointing)
    // and a direction, (up or down)
    char orientation;
    char direction;
public:
    Stair(char o, char d, int t = 0) : Entity(t) {
        orientation = o;
        direction = d;
    }
    
    bool interact() {
        if(direction == 'u'){
            // go up a floor
            currentFloor++;
        } else {
            // go down a floor
            currentFloor--;
        }
        return true;
    }

    void draw() {
        // draw the stars
        glBindTexture(GL_TEXTURE_2D, 0);
        wallMaterial.set();
        // translate the stairs down if it's a down redirection stair
        if(direction == 'u'){
            glTranslatef(0.5,0,0.5);
        } else {
            glTranslatef(0.5,-1,0.5);
        }
        // rotate the stair base
        if(orientation == 'w'){
            glRotatef(90,0,1,0);
        }
        if(orientation == 'e'){
            glRotatef(270,0,1,0);
        }
        if(orientation == 's'){
            glRotatef(180,0,1,0);
        }
        stairModel.draw();
    }
};
// items are subclasses of entities
class Item : public Entity {
private:
    int itemId;
    // items can be hidden
    // usually after interacting with them
    bool hidden = false;
public:
    Item(int id, int t = 1) : Entity(t) {
        itemId = id;
        switch(itemId){
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 20:
            case 21:
            case 10:
            case 11:
                rotates = true;
                break;
        }
    }
    
    bool interact() {
        switch(itemId){
            // yellow door
            case 2:
                if (hidden){
                    // if it's hidden(opened), does not collide with player
                    return false;
                } else if(player->yellowKeys > 0){
                    // use yellow key and hide the door
                    player->useKey('y');
                    hidden = true;
                    messageStack.push_back("You use a yellow key to open this door");
                    return false;
                }
                // if the player does not have a yellow key, collides with player
                messageStack.push_back("You don't have any yellow keys");
                return true;
                break;
            // blue door
            case 3:
                if (hidden){
                    // if it's hidden(opened), does not collide with player
                    return false;
                } else if(player->blueKeys > 0){
                    // use blue key and hide the door
                    player->useKey('b');
                    hidden = true;
                    messageStack.push_back("You use a blue key to open this door");
                    return false;
                }
                // if the player does not have a blue key, collides with player
                messageStack.push_back("You don't have any blue keys");
                return true;
                break;
            // red door
            case 4:
                if (hidden){
                    // if it's hidden(opened), does not collide with player
                    return false;
                } else if(player->redKeys > 0){
                    // use red key and hide the door
                    player->useKey('r');
                    hidden = true;
                    messageStack.push_back("You use a red key to open this door");
                    return false;
                }
                // if the player does not have a red key, collides with player
                messageStack.push_back("You don't have any red keys");
                return true;
                break;
            // yellow key
            case 5:
                if (!hidden){
                    // add key to player and hide it
                    player->receiveKey('y');
                    hidden = true;
                    messageStack.push_back("You found a yellow key! You can use this to open yellow doors.");
                }
                return false;
                break;
            // blue key
            case 6:
                if (!hidden){
                    // add key to player and hide it
                    player->receiveKey('b');
                    hidden = true;
                    messageStack.push_back("You found a blue key! You can use this to open blue doors.");
                }
                return false;
                break;
            // red key
            case 7:
                if (!hidden){
                    // add key to player and hide it
                    player->receiveKey('r');
                    hidden = true;
                    messageStack.push_back("You found a red key! You can use this to open red doors.");
                }
                return false;
                break;
            // small Potion
            case 8:
                if (!hidden){
                    // add hp to player and hide it
                    player->receivePotion('s');
                    hidden = true;
                    messageStack.push_back("You found a small potion! HP +300!");
                }
                return false;
                break;
            // medium Potion
            case 9:
                if (!hidden){
                    // add hp to player and hide it
                    player->receivePotion('m');
                    hidden = true;
                    messageStack.push_back("You found a medium potion! HP +500!");
                }
                return false;
                break;
            //sword
            case 10:
                if (!hidden){
                    // add stats to player and hide it
                    player->receiveGem('s');
                    hidden = true;
                    messageStack.push_back("You found a sword! ATT + 10!");
                }
                return false;
                break;
            //shield
            case 11:
                if (!hidden){
                    // add stats to player and hide it
                    player->receiveGem('S');
                    hidden = true;
                    messageStack.push_back("You found a shield! DEF + 10!");
                }
                return false;
                break;
            // blue gem
            case 20:
                if (!hidden){
                    // add stats to player and hide it
                    player->receiveGem('b');
                    hidden = true;
                    messageStack.push_back("You found a blue gem! DEF +3!");
                }
                return false;
                break;
            // red gem
            case 21:
                if (!hidden){
                    // add stats to player and hide it
                    player->receiveGem('r');
                    hidden = true;
                    messageStack.push_back("You found a red gem! ATT +3!");
                }
                return false;
                break;
        }
    }

    // draw the items
    void draw() {
        // draw the floor below the item
        glBindTexture(GL_TEXTURE_2D, myTex[1]);
        floorMaterial.set();
        glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glNormal3f(0,1,0);
                glVertex3f(0,0,0);
                glTexCoord2f(1,0);
                glNormal3f(0,1,0);
                glVertex3f(1,0,0);
                glTexCoord2f(1,1);
                glNormal3f(0,1,0);
                glVertex3f(1,0,1);
                glTexCoord2f(0,1);
                glNormal3f(0,1,0);
                glVertex3f(0,0,1);
            glEnd();
        if(!hidden){
            // rotate the item by the rotate angle if the item rotates
            if(rotates){
                glTranslatef(0.5,0,0.5);
                glRotatef(itemRotateAngle, 0, 1, 0);
                glTranslatef(-0.5,0,-0.5);
            }
            switch(itemId){
                // draw yellow door
                case 2:
                    glBindTexture(GL_TEXTURE_2D, myTex[16]);
                    yellowMaterial.set();
                    glTranslatef(0.5,0.5,0.5);
                    cubeModel.draw();
                    break;
                // draw blue door
                case 3:
                    glBindTexture(GL_TEXTURE_2D, myTex[17]);
                    blueMaterial.set();
                    glTranslatef(0.5,0.5,0.5);
                    cubeModel.draw();
                    break;
                // draw red door
                case 4:
                    glBindTexture(GL_TEXTURE_2D, myTex[18]);
                    redMaterial.set();
                    glTranslatef(0.5,0.5,0.5);
                    cubeModel.draw();
                    break;
                // draw yellow key
                case 5:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    yellowMaterial.set();
                    glTranslatef(0.5,0.5,0.5);
                    keyModel.draw();
                    break;
                // draw blue key
                case 6:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    blueMaterial.set();
                    glTranslatef(0.5,0.5,0.5);
                    keyModel.draw();
                    break;
                // draw red key
                case 7:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    redMaterial.set();
                    glTranslatef(0.5,0.5,0.5);
                    keyModel.draw();
                    break;
                // draw small potion
                case 8:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    redMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    potionModel.draw();
                    break;
                //draw medium potion
                case 9:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    blueMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    potionModel.draw();
                    break;
                // draw sword
                case 10:
                    glBindTexture(GL_TEXTURE_2D, myTex[10]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    glScalef(0.3,0.3,0.3);
                    swordModel.draw();
                    break;
                // draw shield
                case 11:
                    glBindTexture(GL_TEXTURE_2D, myTex[11]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    glScalef(1.2,1.2,1.2);
                    shieldModel.draw();
                    break;
                // draw blue gem
                case 20:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    blueMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    gemModel.draw();
                    break;
                // draw red gem
                case 21:
                    glBindTexture(GL_TEXTURE_2D, 0);
                    redMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    gemModel.draw();
                    break;
            }
        }
    }
};
// enemies are subclasses of entities
class Enemy : public Entity {
private:
    int enemyId;
    bool hidden=false;
public:
    //enemy stats
    int hp;
    int att;
    int def;
    int gold;
    string name;

    Enemy(int id, int t = 6) : Entity(t) {
        enemyId = id;
        //initialize enemy stats based on id
        switch(enemyId){
            case 1: hp = 50 ; att = 20 ; def = 2 ; gold = 1; name="Tanemon"; break;
            case 2: hp = 80 ; att = 30 ; def = 5 ; gold = 2; name="Yokomon"; break;
            case 3: hp = 60 ; att = 58 ; def = 3 ; gold = 3; name="Demiveemon"; break;
            case 4: hp = 100; att = 42 ; def = 5 ; gold = 4; name="Bakemon"; break;
            case 5: hp = 118; att = 38 ; def = 8 ; gold = 4; name="Tsunomon"; break;
            case 6: hp = 180; att = 66 ; def = 15; gold = 5; name="Elecmon"; break;
            case 7: hp = 320; att = 55 ; def = 24; gold = 6; name="Extyranomon"; break;
            case 8: hp = 420; att = 140; def = 12; gold = 8; name="Dragomon"; break;
        }
    }

    bool interact() {
        if(!hidden){
            // get the projected damage
            int projectedDmg = getProjectedDamage();
            // if the player is unable to defeat the enemy, prevent them from engaging combat
            if(projectedDmg == -1 or projectedDmg > player->getHp()){
                messageStack.push_back("You cannot defeat " + name + "!");
                return true;
            } else {
                // engage in combat
                while(hp>0){
                    int damageRecieved = max(player->att - def, 0);
                    hp = hp - damageRecieved;
                    messageStack.push_back("You dealt " + to_string(damageRecieved) + " damage to " + name);
                    if(hp>0){
                        int damgeDealt = max(att - player->def, 0);
                        player->receiveHit(damgeDealt);
                        messageStack.push_back(name + " dealt " + to_string(damgeDealt) + " damage to you.");
                    }
                }
                // receive gold
                player->receiveGold(gold);
                hidden = true;
                messageStack.push_back("You have slain " + name + " and found " + to_string(gold) + " gold!");
                if(enemyId == 8){
                    messageStack.push_back("Congratulations you have completed the game!");
                }
                return false;
            }
        }
        return false;
    }
    // calculate the damage the player would take if combat is engaged
    int getProjectedDamage() {
        // if the enemy defense is greater or equal to the player's attack
        // then the projected damage is infinity
        if(player->att <= def){
            return -1;
        }
        int dmg = 0;
        int hpCopy = hp;
        while(hpCopy>0){
            hpCopy = hpCopy - (player->att - def);
            if(hpCopy>0){
                dmg += max((att - player->def), 0);
            }
        }
        return dmg;
    }

    // draw the enemies
    void draw() {
        // draw the floor below the enemy
        glBindTexture(GL_TEXTURE_2D, myTex[1]);
        floorMaterial.set();
        glBegin(GL_POLYGON);
                glTexCoord2f(0,0);
                glNormal3f(0,1,0);
                glVertex3f(0,0,0);
                glTexCoord2f(1,0);
                glNormal3f(0,1,0);
                glVertex3f(1,0,0);
                glTexCoord2f(1,1);
                glNormal3f(0,1,0);
                glVertex3f(1,0,1);
                glTexCoord2f(0,1);
                glNormal3f(0,1,0);
                glVertex3f(0,0,1);
            glEnd();
        if(!hidden){
            switch(enemyId){
                // set texture, material, transformations and draw the enemies
                case 1:
                    glBindTexture(GL_TEXTURE_2D, myTex[2]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    monster1Model.draw();
                    break;
                case 2:
                    glBindTexture(GL_TEXTURE_2D, myTex[3]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    monster2Model.draw();
                    break;
                case 3:
                    glBindTexture(GL_TEXTURE_2D, myTex[4]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    monster3Model.draw();
                    break;
                case 4:
                    glBindTexture(GL_TEXTURE_2D, myTex[5]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    monster4Model.draw();
                    break;
                case 5:
                    glBindTexture(GL_TEXTURE_2D, myTex[6]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    glScalef(2.3,2.3,2.3);
                    monster5Model.draw();
                    break;
                case 6:
                    glBindTexture(GL_TEXTURE_2D, myTex[7]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    monster6Model.draw();
                    break;
                case 7:
                    glBindTexture(GL_TEXTURE_2D, myTex[8]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    glScalef(0.7,0.7,0.7);
                    monster7Model.draw();
                    break;
                case 8:
                    glBindTexture(GL_TEXTURE_2D, myTex[9]);
                    defaultMaterial.set();
                    glTranslatef(0.5,0,0.5);
                    glScalef(0.05,0.05,0.05);
                    monster8Model.draw();
                    break;
            }
        }
    }
};