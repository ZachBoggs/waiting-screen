#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include "raylib.h"

using std::string;
using std::vector;

struct player
{
  string name;
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Vector2& windowSize;
  float* speed;
  int boarderSize = 10;
  int points;
  int playerPattern; 
  

  Sound hitSoundEffectX;
  Sound hitSoundEffectY;

  vector<Color> currentColorList{RED,ORANGE,LIME,BLUE,MAGENTA};
  int currentColorIndex = 0;
  Color currentColor = currentColorList[currentColorIndex];

  
  player(string,int,Vector2&,float*,int);
  player(string,Vector2&,float*,int);

  void initPlayer(int);
  void drawPlayer();
  void nextColor();
  void randomColor();
  void printDebugStats();

  bool hitCorner();
  void update();
  ~player();
};

void drawSlimePlayer(Vector2,Vector2,int,Color);
void drawShiftedPlayer(Vector2,Vector2,int,Color);
void drawCheckeredPlayer(Vector2,Vector2,int,Color);
void drawSmileyPlayer(Vector2,Vector2,int,Color);
typedef void (*playerPatterns)(Vector2, Vector2, int,Color);
constexpr playerPatterns playerPatternHolder[4] = {&drawSlimePlayer,&drawShiftedPlayer,&drawCheckeredPlayer,&drawSmileyPlayer};
int randomNumber(int,int);

#endif

