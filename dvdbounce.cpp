#include "raylib.h"
#include "player.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <time.h>
#include <math.h>

using namespace std;

void drawCheckeredBoard(int xOS,int yOS,int xWidth,int yWidth,int boarderSize,Color currentColor);

struct menuWindow
{
  const char* windowName;
  Vector2 currentCoords;
  Vector2 windowSize;
  int fontSize;
  Rectangle dragSquare;
  bool draggingWindow = false;
  Vector2 mouseDownPosition;
  bool devMode = false;

  menuWindow(const char* winName)
    : currentCoords{0,0},
      windowName(winName),
      windowSize{300,600},
      fontSize(30)
  {
  }

  menuWindow(const char* winName,float xCoord,float yCoord)
    : currentCoords{0,0},
      windowName(winName),
      windowSize{300,600},
      fontSize(30)
  {
  }
  menuWindow(const char* winName,float xCoord,float yCoord,float winXWidth,float winYWidth)
    : currentCoords{0,0},
      windowName(winName),
      windowSize{winXWidth,winYWidth},
      fontSize(30)
  {
  }
  menuWindow(const char* winName,float xCoord,float yCoord,float winXWidth,float winYWidth,int inFontSize)
    : currentCoords{0,0},
      windowName(winName),
      windowSize{winXWidth,winYWidth},
      fontSize(inFontSize)
  {
  }

  void drawFrame(Color currentColor,Vector2 coords)
  {
      dragSquare = {coords.x,coords.y,windowSize.x,50};
      DrawRectangle(coords.x,coords.y,windowSize.x,windowSize.y,currentColor);
      DrawRectangle(coords.x,coords.y,windowSize.x,windowSize.y,Fade(WHITE,.5));
      DrawRectangle(coords.x+5,coords.y+5,windowSize.x-10,windowSize.y-10,Fade(BLACK,.9));

      DrawText(windowName,coords.x+(windowSize.x/2)-(MeasureText(windowName,fontSize)/2),coords.y+10,fontSize,currentColor);
      DrawText(windowName,coords.x+(windowSize.x/2)-(MeasureText(windowName,fontSize)/2),coords.y+10,fontSize,Fade(WHITE,.2));

      // drag bar at the top of the menu
      if(devMode)
      {
        DrawRectangleRec(dragSquare,Fade(currentColor,.4));
      }

      if (CheckCollisionPointRec(GetMousePosition(), dragSquare))
      {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
          draggingWindow = true;
          mouseDownPosition = GetMousePosition(); 
          mouseDownPosition.x -= currentCoords.x; 
          mouseDownPosition.y -= currentCoords.y; 
        }
        if(devMode)
        {
          DrawRectangleRec(dragSquare,Fade(currentColor,.5));
        }

      }

      // if the user clicked down on the window and is dragging
      if(draggingWindow)
      {
        currentCoords = GetMousePosition();
        currentCoords.x -= mouseDownPosition.x;
        currentCoords.y -= mouseDownPosition.y;
        // finding the difference between my current coords and the mouse

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
        {
          draggingWindow = false;
        }
      }
   }
  

  void drawWindow(Color currentColor,Vector2 coords)
  {
    drawFrame(currentColor,coords);
  }
  void drawWindow(Color currentColor)
  {
    drawWindow(currentColor,currentCoords);
  }
};


struct menuDetail
{
  char* detailName;
  char* details;

  menuDetail(char* inDetailName,char* inDetails)
    :  detailName(inDetailName),
       details(inDetails)
  {
  }

  menuDetail()
    : detailName("CHANGEME"),
      details("CHANGE THESE DETAILS")
  {
  }
};

struct menuOption
{
  const char* optionName; 

  menuOption(const char* optName)
    : optionName(optName)
  {
  }

  virtual void displayMenu(Vector2 coords,int fontSize,Color currentColor)=0;
};

struct sliderOption: public menuOption
{
  using menuOption::menuOption;
  //using menuOption::displayMenu;
  int sliderVal = 0;
  bool moving = false;

  virtual void displayMenu(Vector2 coords,int fontSize, Color currentColor)
  {
    if(!moving)
    {
      DrawText(optionName,coords.x,coords.y,fontSize,currentColor);
    }else
    {
      DrawText(optionName,coords.x,coords.y,fontSize,WHITE);
      DrawText(optionName,coords.x,coords.y,fontSize,Fade(currentColor,.4));
    }

    Vector2 mouse = GetMousePosition();
    coords.x += 200;
    coords.y += 10;
    coords.x += (float)sliderVal;
    DrawLine(coords.x-100-sliderVal,coords.y,coords.x+100-sliderVal,coords.y,WHITE);
    if(!moving)
    {
      // this will render normally
      DrawCircle(coords.x,coords.y,5.0f,currentColor);
    }else
    {
      // this renders when moving
      //DrawLine(coords.x-100-sliderVal,coords.y,coords.x+100-sliderVal,coords.y,WHITE);
      DrawCircle(coords.x,coords.y,7.0f,WHITE);
      DrawCircle(coords.x,coords.y,7.0f,Fade(currentColor,.4));
    }

    if(CheckCollisionPointCircle(mouse, coords, 5.0f))
    {
      if(!moving)
      {
        DrawCircle(coords.x,coords.y,7.0f,currentColor);
      }
      //DrawCircle(coords.x,coords.y,5.0f,WHITE);
      if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
      {
        moving = true;
      }
    }
    coords.x -= (float)sliderVal;

    // calculating the slieder value
    if(moving)
    {
      sliderVal = (int)round(mouse.x-coords.x);
      if(IsMouseButtonUp(MOUSE_BUTTON_LEFT))
      {
        moving = false;
      }
    }

    if(sliderVal < -100)
    {
      sliderVal = -100;
    }else if(sliderVal > 100)
    {
      sliderVal = 100;
    }
    coords.y -= 10;
    if(!moving)
    {
      DrawText(to_string(sliderVal).c_str(),coords.x+120,coords.y,fontSize,currentColor);
    }else
    {
      DrawText(to_string(sliderVal).c_str(),coords.x+120,coords.y,fontSize,WHITE);
      DrawText(to_string(sliderVal).c_str(),coords.x+120,coords.y,fontSize,Fade(currentColor,.4));
    }

    // add a way to customize the lower and upper bounds of the slider, for instance a min of -5 and max of 5 to add mroe customizability, this would be part of the constructor that specifies the range of a sldier
    // have it so there is a default value in the constructor for the slider value
    // add a couple more manu option types
    // two of the types would be toggle buttons/check boxes and a button, these both will have a timestamp of when the button was last pressed so then if the mouse is pressing over it again, it will check if enough time has passed since last time to quantify it as a button push
    // have a league where people can add users to a leugue and they can all be assigned a random color and random direction when a first 5 to touch a corner tournament is started
    // have a joystick like menu where the user can change the direction of a cube
    // have a menu that can display a tournament
    // work on removing all of the warnings that the program keeps printing out when the program starts
    // seperate out the square render and update into a specific class so that I can make multiple square mroe easu and they also all can be managed easuier

  }

};

struct optionMenuWindow: public menuWindow
{
  using menuWindow::menuWindow; 
  vector<menuOption*> menuOptions;

  void drawWindow(Color currentColor)
  {
    drawFrame(currentColor,currentCoords);

    for(int i=0;i<menuOptions.size();++i)
    {
      menuOptions[i]->displayMenu({20+currentCoords.x,(i*40)+currentCoords.y+60},fontSize-10,currentColor);
    }
  }

  void addOption(menuOption* newOption)
  {
    menuOptions.push_back(newOption);
  }
};

struct detailMenuWindow: public menuWindow
{
  using menuWindow::menuWindow; 
  vector<menuDetail> menuOptions;


  void addOption(menuDetail newDetail)
  {
    menuOptions.push_back(newDetail);
  }

  void drawWindow(Color currentColor)
  {
    drawWindow(currentColor,currentCoords);
  }
  void drawWindow(Color currentColor,Vector2 coords)
  {
    drawFrame(currentColor,coords);

    // drawing the menu details
    for(int i=0;i<menuOptions.size();++i)
    {
      DrawText(menuOptions.at(i).detailName,coords.x + 20,coords.y + 40 + (i*70),25,Fade(currentColor,.8));
      DrawText(menuOptions.at(i).details,coords.x + 35,coords.y + 70 + (i*70),20,Fade(currentColor,.7));
    }
  }
};


void drawPlayerCoords(Vector2 coordinates,player* currentPlayer,int fontSize)
{
  DrawRectangle(coordinates.x - 5,coordinates.y - 2,90,50,Fade(currentPlayer->currentColor,.3));
  DrawText((currentPlayer->name).c_str(),coordinates.x,coordinates.y,fontSize,currentPlayer->currentColor);
  DrawText((to_string(currentPlayer->position.x)).c_str(),coordinates.x,coordinates.y+25,fontSize/3,currentPlayer->currentColor);
  DrawText((to_string(currentPlayer->position.y)).c_str(),coordinates.x,coordinates.y+35,fontSize/3,currentPlayer->currentColor);
      // override << operator on player so we can just print the player and we will get this: "X: 111, Y: 111"
}

int main()
{

  Vector2 screenDimensions{1920,1080};
  int screenWidth(1920);
  int screenHeight(1080);

  InitAudioDevice();
  InitWindow(screenWidth,screenHeight,"Waiting screen");  
  while(!IsWindowReady())
  {
  }

  SetTargetFPS(60);
  ToggleFullscreen();

  // setting up color variables
  vector<Color> currentColorList{RED,ORANGE,LIME,BLUE,MAGENTA};
  int currentColorIndex(0);
  Color currentColor = currentColorList.at(currentColorIndex);
  Vector2 mousePosition;

  int boarderSize(10);
  
  bool splatMode(true);
  bool menuToggle(false);
  bool playing(true);
  bool credits(true);
  bool keybindToggle(true);
  bool highlightCursor(false);
  bool listPlayers(false);

  detailMenuWindow keybindMenu("Keybinds:",500,500,400,500,30);
  keybindMenu.addOption(menuDetail("Arrow Keys","Moves the current window"));
  keybindMenu.addOption(menuDetail("K","Toggles keybinds menu"));
  keybindMenu.addOption(menuDetail("M","Toggles options menu"));
  keybindMenu.addOption(menuDetail("P","Pauses the squares movement"));
  keybindMenu.addOption(menuDetail("C","Shows the author credits"));
  keybindMenu.addOption(menuDetail("H","Highlights the cursors position"));

  optionMenuWindow testOptionWindow("options",500,500,400,500,30); 
  sliderOption* testOption = new sliderOption("option 1");
  testOptionWindow.addOption(testOption);
  for(int i=0;i<4;++i)
  {
    testOption = new sliderOption("option 2");
    testOptionWindow.addOption(testOption);
  }

  menuWindow menu("Menu:",0,0,300,600,30);
  
  Vector2 menuCoords{10,10};

  // this sound effect is from the youtube studio sound library
  const char* mainLabel = "Bouncing Cube";

  vector<player*> playerHolder;
  float speed = 1.00;
  srand(10);
  //srand(time(NULL));

  int playerCount(0);


  while(WindowShouldClose() == false)
  {
    playerCount = playerHolder.size();
    
    if(IsKeyDown(KEY_LEFT_SHIFT))
    {
      if(IsKeyDown(KEY_F))
      {
        for(int i=0;i<50;++i)
        {
          srand(playerHolder.size());
          playerHolder.push_back(new player(to_string(playerHolder.size()+1),0,screenDimensions,&speed, rand()));
        }
      }
    }else if(IsKeyPressed(KEY_F))
    {
      for(int i=0;i<50;++i)
      {
        srand(playerHolder.size());
        playerHolder.push_back(new player(to_string(playerHolder.size()+1),0,screenDimensions,&speed, rand()));
      }
    }
    if(IsKeyPressed(KEY_C))
    {
      currentColorIndex++;
      currentColorIndex %= currentColorList.size();
      currentColor = currentColorList[currentColorIndex];
    }

    if(IsKeyPressed(KEY_A))
    {
      //player* testplayer = new player("Jerry",0,screenDimensions);
      srand(playerHolder.size());
      playerHolder.push_back(new player(to_string(playerHolder.size()+1),0,screenDimensions,&speed, rand()));
    }else if(IsKeyDown(KEY_A) && IsKeyDown(KEY_LEFT_SHIFT))
    {
      cout << "adding new player!\n";
      playerHolder.push_back(new player(to_string(playerHolder.size()+1),0,screenDimensions,&speed, rand()));
    }
    if(IsKeyPressed(KEY_L))
    {
      listPlayers = !listPlayers;
    }
  
    if(IsKeyPressed(KEY_M))
    {
      menuToggle = !menuToggle;
      if(keybindToggle)
      {
        keybindToggle = false;
      }
    }
    
    if(IsKeyPressed(KEY_P))
    {
      playing = !playing;
    }
    if(IsKeyPressed(KEY_K))
    {
      keybindToggle = !keybindToggle;
      if(menuToggle)
      {
        menuToggle = false;
      }
      menuCoords = menu.currentCoords;
    }
    if(IsKeyPressed(KEY_C))
    {
      credits = !credits;
    }

    if(IsKeyDown(KEY_UP))
    {
      if(IsKeyDown(KEY_LEFT_SHIFT))
      {
        speed += .10;
      }else if(IsKeyDown(KEY_LEFT_ALT))
      {
        speed += .001;
      }else
      {
        speed += .01;
      }
    }
    if(IsKeyDown(KEY_DOWN))
    {
      if(IsKeyDown(KEY_LEFT_SHIFT))
      {
        speed -= .1;
      }else if(IsKeyDown(KEY_LEFT_ALT))
      {
        speed -= .001;
      }else
      {
        speed -= 0.01;
      }
    }
    if(IsKeyPressed(KEY_H))
    {
      highlightCursor = !highlightCursor;
    }
    if(IsKeyPressed(KEY_R))
    {
      for(int i=0;i<playerHolder.size();++i)
      {
        delete playerHolder.at(i);
      }
      playerHolder.clear();
    }

    BeginDrawing();


    if(splatMode)
    {
      DrawRectangle(boarderSize/2,boarderSize/2,screenWidth-(boarderSize*2)+boarderSize,screenHeight-(boarderSize*2)+boarderSize,Fade(BLACK,.9));
    }else
    {
      ClearBackground(currentColor);
      DrawRectangle(boarderSize/2,boarderSize/2,screenWidth-(boarderSize*2)+boarderSize,screenHeight-(boarderSize*2)+boarderSize,Fade(BLACK,.9));
    }

    if(credits)
    {
      DrawTextEx(GetFontDefault(),mainLabel,{float(1920/2)-(MeasureText(mainLabel,30)/2),1080/2},30,1,Fade(currentColor,.5));
      DrawText("By Zach",1920/2,1080/2+35,15,Fade(currentColor,.5));
    }else
    {
      DrawTextEx(GetFontDefault(),mainLabel,{float(1920/2)-(MeasureText(mainLabel,30)/2),1080/2},30,1,Fade(currentColor,.1));
      DrawText("By Zach",1920/2,1080/2+35,15,Fade(currentColor,.1));
    }

    //DrawRectangle(350,0,50,1080,WHITE); testing left axis accuracy
    //DrawRectangle(800,0,50,1080,WHITE);
    //DrawRectangle(0,220,1920,80,WHITE);
    //DrawRectangle(0,800,1920,80,WHITE);

    if(playing)
    {
      for(int i=0;i<playerHolder.size();++i)
      {
        playerHolder.at(i)->update();
        playerHolder.at(i)->drawPlayer();
      }
    }else
    {
      for(int i=0;i<playerHolder.size();++i)
      {
        playerHolder.at(i)->drawPlayer();
      }
      DrawText("Paused",screenWidth -300 ,100,50,Fade(RAYWHITE,.3));
    }

    
    if(menuToggle)
    {
      menu.drawWindow(currentColor);
    }
    if(keybindToggle)
    {
      keybindMenu.drawWindow(currentColor,menuCoords);
    }
    //testOptionWindow.drawWindow(currentColor);
    if(highlightCursor)
    {
      mousePosition = GetMousePosition();
      DrawCircleV(mousePosition, 40, Fade(RAYWHITE,.1));
      DrawLineEx({mousePosition.x,0},{mousePosition.x,screenHeight},2,currentColor);
      DrawLineEx({0,mousePosition.y},{screenWidth,mousePosition.y},2,currentColor);
    }
    DrawText("Player count: ",screenDimensions.x/2-100,100,30,WHITE);
    DrawText((to_string(playerHolder.size())).c_str(),screenDimensions.x/2-100,140,40,WHITE);
    DrawText("Speed: ",screenDimensions.x/2-100,200,30,WHITE);
    DrawText((to_string(speed)).c_str(),screenDimensions.x/2-100,240,40,WHITE);

    if(listPlayers)
    {
      if(playerCount > 400)
      {
        playerCount = 400;
      }
      if(playerCount % 20 == 0)
      {
        if(playerCount != 0)
        {
          DrawRectangle(50,25,20+(90*((playerCount/20))),(20*50)+25,currentColor);
          DrawRectangle(55,30,10+(90*((playerCount/20))),(20*50)+15,Fade(BLACK,.8));
        }
      }else
      {
        DrawRectangle(50,25,20+(90*((playerHolder.size()/20)+1)),(20*50)+25,currentColor);
        DrawRectangle(55,30,10+(90*((playerHolder.size()/20)+1)),(20*50)+15,Fade(BLACK,.8));
      }
      
      for(int i=0;i<playerCount;++i)
      {
        drawPlayerCoords({65 + (90*(i/20)),40+((i%20)*50)},playerHolder.at(i),30);
      }
    }

    EndDrawing();

  }
  CloseAudioDevice();
  CloseWindow();

  return 0;
}



