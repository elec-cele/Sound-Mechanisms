#include "sound.h"
#include "keyboard.h"
#include "canada_goose.h"
#include <string>
#include <regex>
#include <map>
#include "pendulum.h"
#include "arbsingle_test.h"

const int sample_rate = 48000;

std::random_device rdv;
std::mt19937 mt(rdv());

std::uniform_int_distribution<int> harmonics(3, 25);
std::uniform_real_distribution<double> pct(0, 1);
std::uniform_real_distribution<double> freq_range(10, 40);
std::uniform_real_distribution<double> d_flip(-1, 1);

std::gamma_distribution<double> gamma(2, 1);
typedef std::vector<std::vector<std::string>> progression;


int main()
{
	//Pendulum::generate_pendulum_data();
	//Pendulum::generate_pendulum_sound();
	ArbSingleTest::testfunc();

	return 0;
}
