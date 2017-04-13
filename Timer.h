//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.

/************************************************
*************************************************
*****************HOW TO USE TIMER****************
******Document Written by Ehsan Kamrani***********

Files:
Timer.h
Timer.cpp

Usage:
1)Include the header file:
   #include "Timer.h"

2)Declare a timer object:
   	TIMER *timer;

3)In your initialization function, initialize timer:
    timer = new Timer;
	if( !timer->Init() )
	{
        //Report an error message
		MessageBox( NULL, "Cannot initialize the timer", "ERROR", MB_OK | MB_ICONINFORMATION );
		return 0; 
	}

4)In your rendering function:
    float elapsedTime;
	elapsedTime = timer->GetElapsedSeconds( 1 ) ;

5)In your release code, write the following code:
   delete timer;
   timer = NULL;

/*************************************************************
*************************************************************/

#pragma once

class Timer
{
public:
  Timer();
  ~Timer();

  bool Init();
  float GetElapsedSeconds();
  float GetFPS(unsigned long elapsedFrames = 1);

private:
  LARGE_INTEGER   m_startTime;
  LARGE_INTEGER   m_ticksPerSecond;
};


