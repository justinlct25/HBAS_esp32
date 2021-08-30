#include "checkbool.h"

bool isperson = false;
bool isbrake = false;
bool isalert = false;

//limit parameter
int lim_distance = 35;
int lim_angle = 40; //ref: ~ 47  ref: ~105

void person()
{
  if ( (distance <= lim_distance && distance >= 0) && distance <= 818 )
  {
    isperson = true;
  }
  else
  {
    isperson = false;
    if(distance == -1) isperson = true;
  }
}

void checkrot2()
{
  if (stickrot < lim_angle)
  {
    isbrake = false;
    if (!isperson && !isbrake)
    {
      isalert = true;
      if(!istof && !isintmpu && !isextmpu) isalert = false;
    }
    else
    {
      isalert = false;
    }
  }
  else
  {
    isbrake = true;
    isalert = false;
  }
}
