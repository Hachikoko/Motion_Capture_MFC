//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.
#include "stdAfx.h"
#include "Timer.h"

/*****************************************************************************
 Init()

 If the hi-res timer is present, the tick rate is stored and the function
 returns true. Otherwise, the function returns false, and the timer should
 not be used.
*****************************************************************************/

Timer::Timer()
{
}
Timer::~Timer()
{
}

bool Timer::Init()
{
	if (!QueryPerformanceFrequency(&m_ticksPerSecond))
	{
		// system doesn't support hi-res timer
		return false;
	}
	else
	{
		QueryPerformanceCounter(&m_startTime);
		return true;
	}
} // end Init()


float Timer::GetElapsedSeconds()
{
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float seconds = (float)((double)(currentTime.QuadPart-s_lastTime.QuadPart)/(double)m_ticksPerSecond.QuadPart);
  // reset the timer
  s_lastTime = currentTime;

  return seconds;
} // end GetElapsedSeconds()


/***************************************************************************
 GetFPS()

 Returns the average frames per second over elapsedFrames, which defaults to
 one. If this is not called every frame, the client should track the number
 of frames itself, and reset the value after this is called.
***************************************************************************/
float Timer::GetFPS(unsigned long elapsedFrames)
{
  static LARGE_INTEGER s_lastTime = m_startTime;
  LARGE_INTEGER currentTime;

  QueryPerformanceCounter(&currentTime);

  float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

  // reset the timer
  s_lastTime = currentTime;

  return fps;
} // end GetFPS

