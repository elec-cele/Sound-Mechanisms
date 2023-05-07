#include "sound.h"

#include "newpendulum.h"
#include<chrono>
#include<string>
auto start = std::chrono::high_resolution_clock::now();
const int sample_rate = 48000;



int main()
{
	//l, q, x_0, y_0
	Pendulum pend1(500.0, 300, deg2rad(-47), 0.0); 
	Pendulum pend2(500.0, 600, deg2rad(75), 0.02);

	RK4(pend1, 3600, 0.01, false);
	RK4(pend2, 3600, 0.01, false);
	states2txt(pend1.state_vec, pend2.state_vec, "data", true);




	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
	std::cout << "computation time: " << duration.count() / 1e6 << " seconds" << std::endl;
	return 0;
}
