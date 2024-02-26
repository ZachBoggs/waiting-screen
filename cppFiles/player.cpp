#include "player.h"
#include "raylib.h"
#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>

using std::cout;
using std::string;
using std::vector;

player::player(string playerName, int currentPoints, Vector2 &windowDims,float *refSpeed,int seed)
    : name(playerName), points(currentPoints), windowSize(windowDims),speed(refSpeed) 
{
  initPlayer(seed);
}

player::player(string playerName, Vector2 &windowDims, float *refSpeed,int seed)
    : name(playerName), points(0), windowSize(windowDims), speed(refSpeed) 
{
  initPlayer(seed);
}

void player::initPlayer(int seed) 
{
  srand(seed);
  //srand(5);
  playerPattern = randomNumber(0,4);
  size = {randomNumber(1,15),randomNumber(1,15)};
  //size = {10,10};
  randomColor();
  
  int j = randomNumber(boarderSize+25,windowSize.x-(size.x*20) - (boarderSize*2)-5);
  int k = randomNumber(boarderSize+25,windowSize.y-(size.y*20) - (boarderSize*2)-5);
  position = {j,k};
  //cout << "Player: " << name << " spawned at x: " << position.x << ", y: " << position.y << "\n";
  //
  velocity = {0,0};
  while(velocity.x == 0 || (velocity.x < -20 || velocity.x > 20))
  {
     velocity.x = randomNumber(-20,20);
  }

  while(velocity.y == 0 || (velocity.y < -20 || velocity.y > 20))
  {
    velocity.y = randomNumber(-20,20);
  }
  /*
  velocity = Vector2{x,y};
  */

    
  // spawn the cube at random point
  // add a spawning animation?
  while (!IsAudioDeviceReady()) {
  }
  // this sound effect is from the youtube studio sound library
  Wave hitSoundEffectwav = LoadWave("pingpong.wav"); 
  // loading the audio seperately, so then the sounds can overlap
  hitSoundEffectX = LoadSoundFromWave(hitSoundEffectwav);
  hitSoundEffectY = LoadSoundFromWave(hitSoundEffectwav);
  SetSoundPitch(hitSoundEffectX, 1.7);
  SetSoundPitch(hitSoundEffectY, 1.7);
  UnloadWave(hitSoundEffectwav);
  cout << "successfully added new player!\n";
}

void player::nextColor() 
{
  currentColorIndex += 1;
  currentColorIndex %= currentColorList.size();
  currentColor = currentColorList[currentColorIndex];
}
void player::randomColor()
{
  int newIndex(randomNumber(0,currentColorList.size()));
  //cout << "new index: " << newIndex << "/" << currentColorList.size() << "\n";
  currentColorIndex = newIndex;
  currentColor = currentColorList[currentColorIndex];
}

bool player::hitCorner() 
{
  // checks all four coordinates in the window if the cube position is in the
  // collide position

  return false;
}

void player::drawPlayer() 
{
  
  //drawCheckeredPlayer(position, size, boarderSize, currentColor);
  playerPatternHolder[playerPattern](position, size, boarderSize, currentColor);
}

void player::update() 
{
  position.x += (velocity.x * *speed);
  position.y += (velocity.y * *speed);
  int loopCount(0);
  //if (position.x - 5 <= 0 || (position.x + (boarderSize * 2) + (size.x * 20)) + 5 > windowSize.x)
  if (position.x <= 0 || (position.x + (boarderSize * 2) + (size.x * 20))  >= windowSize.x)
  {
    if (IsSoundPlaying(hitSoundEffectX)) 
    {
      StopSound(hitSoundEffectX);
    }
    
    PlaySound(hitSoundEffectX);
    randomColor();
    velocity.x *= -1;
    while (position.x <= 0 || position.x + (boarderSize * 2) + (size.x * 20) >= windowSize.x) 
    {
      loopCount++;
      if(loopCount > 100)
      {
        cout << "loop 1 infinite loop:\n";
        printDebugStats();
        exit(1);
      }
        
      if (*speed < 0.0)
      {
         //position.x -= (velocity.x * *speed);
         position.x -= (velocity.x);
      } else 
      {
         //position.x += (velocity.x * *speed);
         position.x += (velocity.x);
      }
    }
    
  }

  if (position.y <= 0 || position.y + (boarderSize * 2) + (size.y * 20) >= windowSize.y)
  {
    if (IsSoundPlaying(hitSoundEffectY))
    {
      StopSound(hitSoundEffectY);
    }
    PlaySound(hitSoundEffectY);
    randomColor();
    velocity.y *= -1;

    while (position.y <= 0 || position.y + (boarderSize * 2) + (size.y * 20) >= windowSize.y)
    {
      loopCount++;
      if(loopCount > 100)
      {
        cout << "loop 2 infinite loop:\n";
        printDebugStats();
        exit(1);
      }
      
      if (*speed < 0.0)
      {
        //position.y += (velocity.y * *speed);
        position.y -= (velocity.y);
      } else
      {
        //position.y += (velocity.y * *speed);
        position.y += (velocity.y);
      }
    }
  }
}

player::~player()
{
  UnloadSound(hitSoundEffectX);
  UnloadSound(hitSoundEffectY);
}
void player::printDebugStats()
{
  cout << "Player: " << name << "\n";
  cout << "Size: " << size.x << ", y: " << size.y << "\n";
  cout << "Position: " << position.x << ", " << position.y << "\n";
  cout << "Velocity: " << velocity.x << ", " << velocity.y << "\n";
  cout << "Speed: " << *speed << "\n";
}

void drawSlimePlayer(Vector2 position, Vector2 dimensions, int boarderSize, Color currentColor) 
{
  DrawRectangle(position.x, position.y, (dimensions.x * 20) + (boarderSize * 2), (dimensions.y * 20) + (boarderSize * 2), currentColor);
  DrawRectangle(position.x+boarderSize, position.y+boarderSize, (dimensions.x * 20), (dimensions.y * 20), Fade(RAYWHITE, .5));

  int wWidth = (dimensions.x * 20);
  int wHeight = (dimensions.y * 20);

  DrawRectangle(position.x + (wWidth/8) + boarderSize, position.y + (wHeight/4) + boarderSize,wWidth/4,wHeight/4,Fade(BLACK,.4));
  DrawRectangle(position.x + (wWidth/8) + (wWidth/2) + boarderSize, position.y + (wHeight/4) + boarderSize,wWidth/4,wHeight/4,Fade(BLACK,.4));
  DrawRectangle(position.x + (wWidth/2) + boarderSize, position.y + (wHeight/2)+(wHeight/8) + boarderSize,wWidth/8,wHeight/8,Fade(BLACK,.4));
}

void drawShiftedPlayer(Vector2 position, Vector2 dimensions, int boarderSize, Color currentColor) 
{
  DrawRectangle(position.x, position.y, (dimensions.x * 20) + (boarderSize * 2), (dimensions.y * 20) + (boarderSize * 2), currentColor);
  DrawRectangle(position.x+boarderSize, position.y+boarderSize, (dimensions.x * 20) + (boarderSize), (dimensions.y * 20) + (boarderSize), Fade(RAYWHITE, .5));
}

void drawCheckeredPlayer(Vector2 position, Vector2 dimensions, int boarderSize, Color currentColor) 
{
  DrawRectangle(position.x, position.y, (dimensions.x * 20) + (boarderSize * 2), (dimensions.y * 20) + (boarderSize * 2), currentColor);
  DrawRectangle(position.x, position.y, (dimensions.x * 20) + (boarderSize * 2), (dimensions.y * 20) + (boarderSize * 2), Fade(RAYWHITE, .5));

 
  for(int y=0;y<dimensions.y;++y)
  {
    for(int x=0;x<dimensions.x;++x)
    {

      if(y % 2 == 0 && x % 2 == 0)
      {
        DrawRectangle(x*20+position.x+boarderSize,y*20+position.y+boarderSize,20,20,currentColor);
      }else if(y % 2 == 0 || x % 2 == 0)
      {
        DrawRectangle(x*20+position.x+boarderSize,y*20+position.y+boarderSize,20,20,BLACK);
      }else
      {
        DrawRectangle(x*20+position.x+boarderSize,y*20+position.y+boarderSize,20,20,currentColor);
      }

    }

  }
  
}



void drawSmileyPlayer(Vector2 position, Vector2 dimensions, int boarderSize, Color currentColor) 
{
  DrawRectangle(position.x, position.y, (dimensions.x * 20) + (boarderSize * 2), (dimensions.y * 20) + (boarderSize * 2), currentColor);
  DrawRectangle(position.x+boarderSize, position.y+boarderSize, (dimensions.x * 20), (dimensions.y * 20), Fade(RAYWHITE, .5));

  int wWidth = (dimensions.x * 20);
  int wHeight = (dimensions.y * 20);

  DrawRectangle(position.x + (wWidth/8) + boarderSize, position.y + (wHeight/4) + boarderSize,wWidth/4,wHeight/4,Fade(BLACK,.4));
  DrawRectangle(position.x + (wWidth/8) + (wWidth/2) + boarderSize, position.y + (wHeight/4) + boarderSize,wWidth/4,wHeight/4,Fade(BLACK,.4));
  //DrawRectangle(position.x + (wWidth/2) + boarderSize, position.y + (wHeight/2)+(wHeight/8),wWidth/8,wHeight/8,Fade(BLACK,.4));
  DrawRectangle(position.x + boarderSize + (wWidth/4),position.y + (wHeight/2) + (wHeight/4) + boarderSize,wWidth/2,wHeight/16,Fade(BLACK,.4));
  DrawRectangle(position.x + boarderSize + (wWidth/8),position.y + (wHeight/2) + (wHeight/4) - (wHeight/16) + boarderSize,wWidth/8,wHeight/16,Fade(BLACK,.4));
  DrawRectangle(position.x + boarderSize + (wWidth/4) + (wWidth/2),position.y + (wHeight/2) + (wHeight/4) - (wHeight/16) + boarderSize,wWidth/8,wHeight/16,Fade(BLACK,.4));
}



int randomNumber(int low, int high)
{
  int numberCalculated = ((rand() % (high - low )) + low); 
  return numberCalculated;
}
