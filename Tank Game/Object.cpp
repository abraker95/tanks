#include "Object.h"

Object::Object(RenderWindow* _window, char* _filename, int _numframes)
{
 window = _window;
 numFrames = _numframes;
 currFrame = 0;

 Texture* texture = new Texture[numFrames];
 sprite = new Sprite[numFrames];

 for(int frame = 0; frame<numFrames; frame++)
 { 
  char tmp[2];
  _itoa_s(frame, tmp, 2);

  texture[frame].loadFromFile(string(_filename)+"_"+tmp+".png");
     PRINT_DEBUG(cout<<string(_filename)+"_"+tmp+".png"<<endl, HI_DEBUG);
  sprite[frame] = Sprite(texture[frame]);
 }
}

Object::~Object()
{

}

void Object::setPos(int _xpos, int _ypos)
{
 for(int frame = 0; frame<numFrames; frame++)
     sprite[frame].setPosition(_xpos, _ypos);
}

void Object::Render()
{
 if(++currFrame>=numFrames) currFrame = 0;
 window->draw(sprite[currFrame]);
}