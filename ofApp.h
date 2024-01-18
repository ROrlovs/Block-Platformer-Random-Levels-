#pragma once

#include "ofMain.h"

class GameObject {
public:
    GameObject();
    ~GameObject();
    int id;
    float x,y;
    float x_vel, y_vel;
    float drag = 1.0;
    void updateObject();
    void drawObject();
};

class Player : public GameObject {
public:
    Player();
    ~Player();
    float characterWidth = 20.0;
    float characterSize = 20.0;
    bool jumping;
    float gravity = 15;
    void drawObject();
    bool dead;
    void playerDead();

};

class Platform : public GameObject {
public:
    Platform();
    ~Platform();
    bool collide(Player p);
    float platformHeight = 25;
    float platformWidth = 100;
    void drawObject();
};

class Laser : public GameObject {
public:
    Laser();
    ~Laser();
    bool collide(Player p);
    float laserHeight = 100;
    float laserWidth = 15;
    void drawObject();
};

class Coin : public GameObject {
public:
    Coin();
    ~Coin();
    bool collide(Player p);
    bool interactable=true;
    float coinSize = 10;
    void drawObject();
};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void resetGame();
    void winGame();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void playSound(string sound);
    Coin coins[10];
    Platform startPlatform[3];
    Platform newPlatform[10];
    Player player;
    int keys[128];

};

