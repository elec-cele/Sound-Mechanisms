#include <iostream>
#include <tuple>
#include "sound.h"
const extern double two_pi; 
double deg2rad(double deg) {
	return fmod(deg * two_pi / 360.0, two_pi);
}
double rad2deg(double rad) {
	return fmod(rad * 360.0 / two_pi, 360.0);
}

//state_vec type holds t, x, y
typedef std::tuple<double, double, double> triple;
typedef std::vector<std::tuple<double, double, double>> state_type;

struct Pendulum {
public:

	//x pendulum parameters
	double l = 1.0;		 // pendulum length
	double q = 1;		 // quality (higher = less decay)
	double alpha;	     // attenuation, = -1/q
	double g = 9.81;
	double init_x = 0.0; // pendulum initial phase (RADIANS)
	double init_y = 0.0; // pendulum initial velocity

	state_type state_vec;	// holds {{t, x, y},...} of solution

	double endval = 1e-3;

	Pendulum(
		double a,
		double b,
		double c,
		double d) {
		l = a;
		q = b;
		init_x = c;
		init_y = d;
		alpha = -1.0 / q;
	}
	//x = theta, y=velocity
	double dxdt(double x, double y) { return y; };
	double dydt(double x, double y) { return (alpha * y) - ((g / l) * sin(x)); };


};

void state2txt(state_type& st, std::string filename) {
	std::ofstream textlog(filename + ".txt");
	for (size_t i = 0; i < st.size(); i++) {
		textlog << std::get<0>(st[i]) << '\t' << std::get<1>(st[i]) << '\t' << std::get<2>(st[i]) << '\n';

		textlog << std::get<0>(st[i]) << '\t' << std::get<1>(st[i]) << '\t' << std::get<2>(st[i]) << '\n';
	}
}

//used for combining data from two pendulums into one file. skips the time column from the second state vec
// must be equal length
void states2txt(state_type& st1, state_type& st2, std::string filename, bool header=false) {
	std::ofstream textlog(filename + ".txt");
			if (header) {
			textlog << "time" << '\t' << "pend1 theta" << '\t' << "pend1 vel" << '\t' << "pend2 theta" << '\t' << "pend2 vel" << '\n';
		}
	for (size_t i = 0; i < st1.size(); i++) {

		textlog << std::get<0>(st1[i]) << '\t' << std::get<1>(st1[i]) << '\t' << std::get<2>(st1[i]) << '\t' << std::get<1>(st2[i]) << '\t' << std::get<2>(st2[i]) << '\n';
	}
}

void record_state(state_type& sv, triple st) {
	sv.push_back(st);
}

template <typename T>
void RK4(T& eqn, double sim_duration, double h, bool cut_at_endval=true) {
	int num_steps;
	num_steps = static_cast<int>(sim_duration / h);

	double kx1, kx2, kx3, kx4;
	double ky1, ky2, ky3, ky4;
	double t = 0.0;
	double x = eqn.init_x;
	double y = eqn.init_y;
	//eqn.record_state({ t, x, y });
	record_state(eqn.state_vec, { t, x, y });
	double hdiv2 = h / 2.0;
	double hdiv6 = h / 6.0;
	for (long int n = 0; n < num_steps; n++) {

		kx1 = eqn.dxdt(x, y);
		ky1 = eqn.dydt(x, y);

		kx2 = eqn.dxdt(x + hdiv2 * kx1, y + hdiv2 * ky1);
		ky2 = eqn.dydt(x + hdiv2 * kx1, y + hdiv2 * ky1);

		kx3 = eqn.dxdt(x + hdiv2 * kx2, y + hdiv2 * ky2);
		ky3 = eqn.dydt(x + hdiv2 * kx2, y + hdiv2 * ky2);

		kx4 = eqn.dxdt(x + h * kx3, y + h * ky3);
		ky4 = eqn.dydt(x + h * kx3, y + h * ky3);

		x += hdiv6 * (kx1 + 2.0 * kx2 + 2.0 * kx3 + kx4);
		y += hdiv6 * (ky1 + 2.0 * ky2 + 2.0 * ky3 + ky4);
		t += h;

		record_state(eqn.state_vec, { t, x, y });

		//make this optional?
		// stop when the result gets adequately small 
		if (cut_at_endval) {
			if (abs(x) < eqn.endval && abs(y) < eqn.endval) { return; }
		}
	}
}