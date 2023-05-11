#include "sound.h"
#include "bitmap.h"
#include "pendulum.h"
#include<chrono>
#include<string>
const int SAMPLE_RATE = 48000;


int main()
{

	//std::string read_test = read_binary("test4.bmp");

	SphericalPendulum test(100, { Sound::pct(Sound::rd), Sound::pct(Sound::rd), deg2rad(45.0 + Sound::pct(Sound::rd)*45.0), 0.0});
	
	//Pendulum test(1.0, 10, 1.0, 0.0);
	RK4(test, 1200, 0.0002, 100, true);
	test.states2txt("pend2test.txt");
	return 0;
}
