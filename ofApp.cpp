#include "ofApp.h"
int FPS_BASE = 30;
int score=0;
int GRAVITY = 15;
int numberOfPlats=10;
int platCount=0;
int i=0;
int u = 0;
int u2 = 0;
int u3 = 0;
int d = 0;
int d2 = 0;
int d3 = 0;
int timerForPlayerSpawn=0;
int coinCount=0;
int finalCoinCount=-1;
int resetTimer=0;
bool gameWin=false;
bool gameWinPrompt=false;
int finalScore;
int highScore;
int laserSpeed=10;
ofSoundPlayer jumpSound;
ofSoundPlayer bgMusic;

Laser laser;


//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(FPS_BASE);
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    player.gravity=GRAVITY;
    player.jumping=false;
    player.dead=true;
    resetGame();

    bgMusic.load("bgmusic.mp3");
    bgMusic.play();
    bgMusic.setLoop(true);

    //initiate laser
    laser.x=-100;
    laser.y=500;
    laser.x_vel=laserSpeed;
    laser.laserWidth=100;
    laser.laserHeight=15;

}

//--------------------------------------------------------------
void ofApp::update(){
    //for text purposes only
    if(!gameWin){
        finalScore=score;

    }

    //if player collects all coins, run win method
    if(score==finalCoinCount){
        winGame();
    }
    //tick reset button timer to avoid crashes
    if (resetTimer!=31){
        resetTimer++;
    }

//--------------------------------------------------------------
    //tick player spawn timer (band-aid solution for a bug)
    if (timerForPlayerSpawn<=3){
        player.x=0;
        player.y=0;
        timerForPlayerSpawn++;
    }
    // spawn starting platforms 2 frames later (band-aid solution for a bug)
    if (timerForPlayerSpawn==2){

        startPlatform[0].x= ofGetWindowWidth()/2-75;
        startPlatform[0].y=ofGetWindowHeight()/4;

        startPlatform[1].x= 150;
        startPlatform[1].y= 450;

        startPlatform[2].x= 700;
        startPlatform[2].y=500;
    }
    //spawn player 3 frames later (band-aid solution for a bug)
    if(timerForPlayerSpawn==4){
        player.x = 500;
        player.y = 0;
        timerForPlayerSpawn++;
    }
//--------------------------------------------------------------
    //player controls and rules
    if (!player.dead && !gameWin){
        if(keys['s'] && player.jumping) player.y_vel = 5.0;
        if(keys['w'] && !player.jumping){
            player.y_vel = -50.0;
            player.jumping=true;
            playSound("jump");
        }
        if(keys['d']) player.x_vel = 5.0;
        if(keys['a']) player.x_vel = -5.0;

    }
    if(keys['r']) resetGame();


//--------------------------------------------------------------
    //apply gravity to player
    player.y+=player.gravity;


//--------------------------------------------------------------
   //Create coins at random coordinates and update them and check collisions
   while (coinCount <= 9){
                coins[coinCount].x=ofRandom(100,924);
                coins[coinCount].y=ofRandom(100,668);
                coinCount++;
            }
   if (coinCount>9){
       finalCoinCount=coinCount;
   }
   while(u3<=10 && !gameWin){
       coins[u3].updateObject();

       //check for collision with coin and add score
       if (coins[u3].collide(player) && coins[u3].interactable){
           score++;
           coins[u3].interactable=false;
           }
       u3++;
         }







//--------------------------------------------------------------
    //Create any number of platforms at random coordinates
    while (platCount <= numberOfPlats+1){
            newPlatform[platCount].x = ofRandom(24,1000);
            newPlatform[platCount].y = ofRandom(68,700);
            platCount++;

    }
//--------------------------------------------------------------
    //update player object
    if (!player.dead){
        player.updateObject();
    }

//--------------------------------------------------------------
    while (u2<=3 && !gameWin){
    //update starting platforms and check for collision
    startPlatform[u2].updateObject();

    //check for collision with platform and apply a displacement to
    //retain player on top of platform
    if (startPlatform[u2].collide(player)){
        player.y=startPlatform[u2].y-startPlatform[u2].platformHeight;
        player.jumping=false;
        player.y_vel=0;
        player.gravity=0;
    }
    else{
        player.gravity=GRAVITY;
    }
    u2++;
    }
//--------------------------------------------------------------
    //update random platforms and check for collisions
    while (u<=numberOfPlats+1  && !gameWin){
        newPlatform[u].updateObject();


        if (newPlatform[u].collide(player)){
            player.y=newPlatform[u].y-newPlatform[u].platformHeight;
            player.jumping=false;
            player.y_vel=0;
            player.gravity=0;
        }

        else{
            player.gravity=GRAVITY;
        }

        u++;
    }

//--------------------------------------------------------------
    //check if player outside window boundaries, if true run kill player method
    if (player.y > ofGetWindowHeight()){
        player.playerDead();
    }

//--------------------------------------------------------------

    if (gameWin){   //necessary updates for win game screen
        startPlatform[0].updateObject();
        startPlatform[0].collide(player);
        if (startPlatform[0].collide(player)){
            player.y=startPlatform[0].y-startPlatform[0].platformHeight;
            player.jumping=false;
            player.y_vel=0;
            player.gravity=0;
        }
        else{
            player.gravity=GRAVITY;
        }
    }
//--------------------------------------------------------------
    //laser
    //reset laser X coordinate and generate a new Y coordinate when finished traveling through the screen
    if(laser.x>ofGetWindowWidth()){
        laser.x=-100;
        laser.y=ofRandom(0,ofGetWindowHeight());
    }
    if(gameWin){
        laser.x=-100;
        laser.y=-100;
    }
    //check collision and kill player if true
    laser.updateObject();
    if (laser.collide(player)){
        player.playerDead();
    }


}// end of update method
//--------------------------------------------------------------
void ofApp::resetGame(){
    //crash prevention: check if a second has passed since last reset (30 frames = 1 sec)
    if (resetTimer>30){
        int x=0;
        //reset "Interactable" variable on all coins to true so they are drawn again
        while(x<coinCount){
            coins[x].interactable=true;
            x++;
        }



        //reset all game variables
        player.dead=false;
        player.x=500;
        player.y=0;
        platCount=0;
        coinCount=0;
        timerForPlayerSpawn=0;
        score=0;
        gameWin=false;
        player.dead=false;
        resetTimer=0;
        ofSetBackgroundColor(0,0,0);
        i=0;
        u = 0;
        u2 = 0;
        u3 = 0;
        d = 0;
        d2 = 0;
        d3 = 0;

        //initiate laser
        laser.x=-100;
        laser.y=500;
        laser.x_vel=laserSpeed;
        laser.laserWidth=100;
        laser.laserHeight=15;
        bgMusic.load("bgmusic.mp3");
        bgMusic.play();
        bgMusic.setLoop(true);

    }

}
//--------------------------------------------------------------
void ofApp::winGame(){
    finalScore=score;
    if (finalScore>highScore){
        highScore=finalScore;
    }
    bgMusic.load("winmusic.mp3");
    bgMusic.setLoop(false);
    bgMusic.play();
    score=0;
    gameWin=true;
    gameWinPrompt=true;
    player.dead=false;
    player.y_vel=5;
    player.x_vel=0;
    ofSetBackgroundColor(0,175,0);
    player.gravity=GRAVITY;
    player.x=500;
    player.y=0;
    startPlatform[0].x=475;
    startPlatform[0].y=500;
    gameWinPrompt=false;

}

//--------------------------------------------------------------
void ofApp::draw(){
 //draw controls text
    if(!gameWin){
        ofDrawBitmapString("Controls:\nW A S D = Movement\nR = Restart",30,30);
    }


 //draw player
 if (!player.dead){
     player.drawObject();
 }
 //draw start platforms
 while(d2<3 && !gameWin){
      startPlatform[d2].drawObject();
      d2++;
 }

 //draw every random platform
 while (d<=numberOfPlats+1 && !gameWin){
     newPlatform[d].drawObject();
     d++;
 }
 //draw every coin that hasnt been picked up
 while (d3<=10 && !gameWin){
     if (coins[d3].interactable){
         coins[d3].drawObject();
     }

     d3++;
 }

 //if all platforms drawn and updated in the current frame, reset counters
 if (d>numberOfPlats){
     d=0;
     u=0;
 }
 // reset starter platform counters
 if (u2>3){
     u2=0;
     d2=0;
 }
// reset coin counters
 if (u3>10){
     u3=0;
     d3=0;
 }

 if (gameWin){ //draw victory graphics
   ofDrawBitmapString("VICTORY!",500,375);
   ofDrawBitmapString("Press R to restart",475,415);
   startPlatform[0].drawObject();
 }


 ofDrawBitmapString(finalScore ,(player.x), (player.y-30));
 ofDrawBitmapString("High Score: "+std::to_string(highScore) ,(900), (30));

 //laser draw
 laser.drawObject();


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    keys[key] = 1;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    keys[key] = 0;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
//--------------------------------------------------------------
GameObject::GameObject() {

}
//--------------------------------------------------------------
GameObject::~GameObject() {}
//--------------------------------------------------------------
Platform::Platform(){
}
//--------------------------------------------------------------
Platform::~Platform(){}
//--------------------------------------------------------------
Laser::Laser(){}
//--------------------------------------------------------------
Laser::~Laser(){}
//--------------------------------------------------------------

bool Laser::collide(Player p){
    //get player positions
    float bot = p.y+p.characterSize;
    float top =p.y;
    float leftside = p.x;
    float rightside=p.x+p.characterWidth;

    //check if player touches laser
    if ((top>y-30) && (bot<y+laserHeight+30) && (x<leftside+20) && (x+laserWidth>rightside+20)){
        return true;
    }

    else{
        return false;
    }
};
//--------------------------------------------------------------#
void Laser::drawObject(){
    ofSetColor(255,0,0);
    ofDrawRectangle(x,y,laserWidth,laserHeight);
    ofSetColor(255,255,255);
}
//--------------------------------------------------------------
Coin::Coin(){

}
//--------------------------------------------------------------
Coin::~Coin(){}
//--------------------------------------------------------------
bool Coin::collide(Player p){
    bool intersects=false;

    //player
    float pTop=p.y-15;
    float pBot=p.y+p.characterSize+15;
    float pLeft=p.x-15;
    float pRight=p.x+15+p.characterSize;


    //coin
    float top=y;
    float bot=y+coinSize;
    float left=x;
    float right=x+coinSize;


    //check if middle of player intersects with middle of coin
    if (pTop <bot
            && pBot > top
            && pRight > left
            && pLeft < right
            ){
        intersects=true;
    }


    if (intersects){
            return true;
    }
    else{
        return false;
    }
}
//--------------------------------------------------------------
void Coin::drawObject(){
    ofSetColor(255,255,0);
    ofDrawCircle(x,y,coinSize);
    ofSetColor(255,255,255);
};
//--------------------------------------------------------------
void GameObject::updateObject(){

    //apply forces and update position
    x_vel *= drag;
    y_vel *= drag;
    x += x_vel;
    y += y_vel;





}
//--------------------------------------------------------------
void Player::playerDead(){
    dead=true;
    x=0;
    y=1500;
    if(finalScore>highScore){
        highScore=finalScore;
    }
    bgMusic.stop();
    ofSetBackgroundColor(175,0,0);
}

//--------------------------------------------------------------
void GameObject::drawObject(){

}
//--------------------------------------------------------------
bool Platform::collide(Player p) {


    //get player positions
    float bot = p.y;
    float leftside = p.x;
    float rightside=p.x+p.characterWidth;

    //check if player is conforming to platform boundaries
    if ((bot>y-platformHeight+10) && (bot<y) && (x<leftside+20) && (x+platformWidth>rightside-20)){
        return true;
    }

    else{
        return false;
    }
}


//--------------------------------------------------------------
Player::Player(){
    x_vel = 0;
    y_vel = 0;
    drag = 0.9;
};
//--------------------------------------------------------------
Player::~Player() {
}

//--------------------------------------------------------------
void Player::drawObject() {

    //check if player is outside boundaries,
    //if true draw a circle to represent the player's position in relation to the window
    //else draw regular player character
    if (y<0){
        ofDrawCircle(x+characterWidth/2,0,7);
    }

    else if (x<0){
        ofDrawCircle(0,y,7);
    }

    else if (x>ofGetWindowWidth()){
        ofDrawCircle(ofGetWindowWidth(),y,7);
    }
    else{
        ofDrawRectangle(x,y,characterWidth,characterSize);
    }

}



//--------------------------------------------------------------
void Platform::drawObject(){
    ofDrawRectangle(x,y,platformWidth,platformHeight);
}

//--------------------------------------------------------------
void ofApp::playSound(string sound){
    if (sound=="jump"){
        jumpSound.load("jump.mp3");
        jumpSound.play();
    }

}
//--------------------------------------------------------------




