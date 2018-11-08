#include "Timer.h"

Timer::Timer()
{
	DeltaTime = 0.0f;
	CurrentTime = 0;
	Frequency = 0;
	LastTime = 0;
	UpdatedTime = 0;
}

Timer::~Timer()
{

}

void Timer::Start()
{
	//Get the current value from high pref counter
	CurrentTime = SDL_GetPerformanceCounter();
	//Set the last time equal to the current time
	LastTime = CurrentTime;
	//Get the frequency of the high pref counter
	Frequency = SDL_GetPerformanceFrequency();
}

void Timer::Update()
{
	//Get current value from high pref counter
	CurrentTime = SDL_GetPerformanceCounter();
	//Calculate Delta time
	DeltaTime = (float)(CurrentTime - LastTime) / (float)Frequency;
	//Set the last time equal to the current time
	LastTime = CurrentTime;
	UpdatedTime += DeltaTime;

}