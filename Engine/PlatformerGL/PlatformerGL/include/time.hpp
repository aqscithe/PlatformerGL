#pragma once

//target frame rate
#define TFR 60.f

class Time
{
private:
	float			lastTime = 0.f;

	float			ts = 1.f;					//time scale

	float			dt = 0.f;					//delta time

	float			targetFrameRate = TFR;
	float			fdt = 1 / targetFrameRate;	//fixed delta time
	int				f = 1;						//fix count

	//cannot create any Time object
	Time() {};

public:
	static Time& getInstance()
	{
		static Time instance;

		return instance;
	}

	//cannot create any Time object
	Time(Time const&) = delete;
	void operator=(Time const&) = delete;

	static float&	timeScale();
	static float	deltaTime();

	static float	fixedDeltaTime();
	static int		fixing();
	static void		resetLastTime();

	void			update();
};