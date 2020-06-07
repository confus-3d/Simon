//SIMON SAYS
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
enum gameStates {INS, R, Y, B, G, O, M, MASTER, GAME, END};  //tile states
byte gameState = INS;
int numNeighbors;
int brightness = 100; 
int sendingSwitch = 0;
int sendingSwitch2 = 0;
int MAX_COLOURS = 0;
int COLOUR;
int SHOWCOLOUR;

const int MAX_LEVEL = 50;
int sequence[MAX_LEVEL];
int your_sequence[MAX_LEVEL];
int level = 1;

int velocity = 1000;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  
  switch (gameState) {
    case INS:
      installLoop();
      break;
  case R:
      RLoop();
      gameDisplayLoop();
      break;
  case Y:
      YLoop();
      gameDisplayLoop();
      break;
  case B:
      BLoop();
      gameDisplayLoop();
      break;
  case G:
      GLoop();
      gameDisplayLoop();
      break;
  case O:
      OLoop();
      gameDisplayLoop();
      break;
  case M:
      MLoop();
      gameDisplayLoop();
      break;
  case GAME:
      gameLoop();
      gameDisplayLoop();
      break;
  case MASTER:
      masterLoop();
      gameDisplayLoop();
      break;
  }

  
//RESET GAME
  if (buttonLongPressed()) {
    sendingSwitch = 0;
    sendingSwitch2 = 0;
    MAX_COLOURS = 0;
    level = 1;
    gameState = INS;
  }

}

void installLoop() { //Setup acording to number of blinks, from 2 to 6.

setColorOnFace( dim( RED,  brightness  ), 0);
setColorOnFace( dim( MAGENTA,  brightness  ), 1);
setColorOnFace( dim( BLUE,  brightness  ), 2);
setColorOnFace( dim( GREEN,  brightness  ), 3);
setColorOnFace( dim( YELLOW,  brightness  ), 4);
setColorOnFace( dim( ORANGE,  brightness  ), 5);

numNeighbors = 0;
setValueSentOnAllFaces(gameState);
  FOREACH_FACE(f) {
    if ( !isValueReceivedOnFaceExpired( f ) ) {
      byte neighborGameState(getLastValueReceivedOnFace(f));
      numNeighbors++;
      if (numNeighbors == 1 && buttonDoubleClicked()) {
        gameState = R;
      }
      if (neighborGameState == R) {
        gameState = Y;
        MAX_COLOURS = 2;
      }
      if (neighborGameState == Y) {
        gameState = B;
        MAX_COLOURS = 3;
      }
      if (neighborGameState == B) {
        gameState = G;
        MAX_COLOURS = 4;
      }
      if (neighborGameState == G) {
        gameState = O;
        MAX_COLOURS = 5;
      }
      if (neighborGameState == O) {
        gameState = M;
        MAX_COLOURS = 6;
      }
    }
  }

}

void RLoop() {
COLOUR = 0;
sendingLoop();
detectLoop();
}

void YLoop() {
  COLOUR = 1;
sendingLoop();
detectLoop();
    if (buttonDoubleClicked()){
      gameState = MASTER;
    }
}

void BLoop() {
  COLOUR = 2;
sendingLoop();
detectLoop();
    if (buttonDoubleClicked()){
      gameState = MASTER;
    }
}

void GLoop() {
  COLOUR = 3;
sendingLoop();
detectLoop();
    if (buttonDoubleClicked()){
      gameState = MASTER;
    }
}

void OLoop() {
  COLOUR = 4;
sendingLoop();
detectLoop();
    if (buttonDoubleClicked()){
      gameState = MASTER;
    }
}

void MLoop() {
  COLOUR = 5;
sendingLoop();
detectLoop();
    if (buttonDoubleClicked()){
      gameState = MASTER;
    }

}


void masterLoop() {
  sendingLoop2();
  if (level == 1)
  generate_sequence();
  show_sequence();
  get_sequence();
}

void gameLoop() {
  sendingLoop2();

  FOREACH_FACE(f) {
    if ( !isValueReceivedOnFaceExpired( f ) ) {
      SHOWCOLOUR = (getLastValueReceivedOnFace(f));
      setValueSentOnAllFaces(SHOWCOLOUR);
      _delay_ms(200);
  }
  }
    if (buttonSingleClicked()){
      setValueSentOnAllFaces(COLOUR);
    }
    
  if (SHOWCOLOUR == COLOUR || SHOWCOLOUR == 7){
    brightness = 255;
  }
  if (SHOWCOLOUR == 6) {
    brightness = 100;
  } 
}

void gameDisplayLoop() {
switch (COLOUR) {
  case 0:
      setColor( dim( RED ,  brightness  ) ); 
      break;    
  case 1:
      setColor( dim( YELLOW ,  brightness  ) );  
      break;
  case 2:
      setColor( dim( BLUE ,  brightness  ) );
      break;
  case 3:
      setColor( dim( GREEN ,  brightness  ) );
      break;
  case 4:
      setColor( dim( ORANGE ,  brightness  ) );
      break;
  case 5:
      setColor( dim( MAGENTA ,  brightness  ) );
      break;
}
}

void sendingLoop() {
  if (sendingSwitch == 0){
    setValueSentOnAllFaces(gameState);
    sendingSwitch++;
  }
}
void sendingLoop2() {
  if (sendingSwitch2 == 0){
    setValueSentOnAllFaces(gameState);
    sendingSwitch2++;
  }
}

void detectLoop() {
  FOREACH_FACE(f) {
    if ( !isValueReceivedOnFaceExpired( f ) ) {
      byte neighborGameState(getLastValueReceivedOnFace(f));
      if (neighborGameState == MASTER) {
        gameState = GAME;
        sendingSwitch = 0;
      }
      if (neighborGameState == GAME) {
        gameState = GAME;
        sendingSwitch = 0;
      }
    }
  }
}


void generate_sequence()
{

for (int i = 0; i < MAX_LEVEL; i++)
{
sequence[i] = random(MAX_COLOURS);
}
}

void show_sequence()
{
for (int i = 0; i < level; i++)
{
setValueSentOnAllFaces(sequence[i]);
if (COLOUR == sequence[i]){
  brightness = 255;
  gameDisplayLoop();
  _delay_ms(velocity);
  brightness = 100;
  gameDisplayLoop();
}
if (COLOUR != sequence[i]){
  _delay_ms(velocity);
}
setValueSentOnAllFaces(6);
_delay_ms(200);
}
}

void get_sequence()
{
int flag = 0; //this flag indicates if the sequence is correct
  for (int i = 0; i < level; i++)
  {
  flag = 0;
    while(flag == 0){
      FOREACH_FACE(f) {
        if ( !isValueReceivedOnFaceExpired( f ) ) {
          your_sequence[i] = getLastValueReceivedOnFace(f);
          flag = 1;
          _delay_ms(200);
          setValueSentOnAllFaces(6);
          if (your_sequence[i] != sequence[i])
            {
            wrong_sequence();
            return;
            }
        }
      }
    }
  }
right_sequence();
}

void wrong_sequence()
{
  
for (int i = 0; i < 3; i++)
{

setValueSentOnAllFaces(7);
_delay_ms(250);

setValueSentOnAllFaces(6);
_delay_ms(250);
}
level = 1;
velocity = 1000;
}

void right_sequence()
{
setValueSentOnAllFaces(7);
_delay_ms(1000);

setValueSentOnAllFaces(6);
_delay_ms(250);


if (level < MAX_LEVEL){
 level++; 
}

if (level == MAX_LEVEL){
  setValueSentOnAllFaces(7);
  _delay_ms(1000);
  level = 1;
  velocity = 1000;
  
}

velocity -= 50; //increase difficulty until level 10
if (velocity < 500){
 velocity = 500;
}

}

//COMMUNICATION
