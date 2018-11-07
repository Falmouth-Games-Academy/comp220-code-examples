#pragma once

#include <SDL.h>

//Basic Timer Class
class Timer
{
public:
	Timer();
	~Timer();

	//Start Timer, this will get the initial time, get the update frequency and setup last time
	void Start();
	//Update the current timer, this will calculate the delta time since last update
	void Update();

	//Get the Delta time
	float GetDeltaTime()
	{
		return DeltaTime;
	};

	//Get the Current time
	Uint64 GetCurrentTime()
	{
		return CurrentTime;
	};

private:
	Uint64 LastTime;
	Uint64 CurrentTime;
	Uint64 Frequency;
	float DeltaTime;
};