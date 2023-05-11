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

struct Pendulum {
public:

	//theta pendulum parameters
	double l = 1.0;		 // pendulum length
	double q = 1.0;		 // quality (higher = less decay)
	double alpha = 1.0;	     // attenuation, = -1/q
	double g = 9.81;

	mono state_vec;
	mono step_vec;
	mono theta_vec;
	mono omega_vec;
	mono dist_vec;
	mono atten_vec; // 1/dist^2
	double endval = 1e-3;

	Pendulum(
		double length,
		double quality,
		double init_theta,
		double init_omega) {
		l = length;
		q = quality;
		state_vec = { init_theta, init_omega };
		alpha = -1.0 / q;
	}

	mono G(mono state) {
		double theta = state[0];
		double omega = state[1];

		double dthetadt = omega;
		double domegadt = (alpha * omega) - ((g / l) * sin(theta));

		return { dthetadt, domegadt };
	}

	void record_state(mono state, double t) {
		step_vec.push_back(t);
		theta_vec.push_back(state[0]);
		omega_vec.push_back(state[1]);
	}

	void calc_dist_from_origin(double h0) {
		for (size_t n = 0; n < step_vec.size(); n++) {
			double theta_dist = pow(l * sin(theta_vec[n]), 2);
			double omega_dist = pow(h0 - l * cos(theta_vec[n]), 2);
			double d = theta_dist + omega_dist;
			dist_vec.push_back(sqrt(d));
			atten_vec.push_back(1.0 / pow(d, 2.0));
		}
	}

	bool truncate_check() {
		if (abs(fmod(state_vec[0],two_pi)) < endval && abs(state_vec[1]) < endval) { return true; }
		return false;
	}

	void states2txt(std::string filename, bool header = false) {
		std::ofstream textlog(filename + ".txt");
		if (header) {
			textlog << "time" << '\t' << "theta" << '\t' << "omega" << '\t' << '\n';
		}
		for (size_t i = 0; i < step_vec.size(); i++) {
			textlog << step_vec[i] << '\t' << theta_vec[i] << '\t' << omega_vec[i] << '\n';
		}
	}



};

struct SphericalPendulum {
public:
	double l = 1.0;		 // pendulum length
	double q = 1;		 // quality (higher = less decay)
	double alpha;	     // attenuation, = -1/q
	double g = 9.81;
	double init_theta = 0.0; // pendulum initial phase (RADIANS)
	double init_phi = 0.0; // pendulum initial velocity

	mono state_vec;
	mono time_vec;
	mono dist_vec;
	mono atten_vec; // 1/dist^2

	mono dtheta_vec, dphi_vec, theta_vec, phi_vec;

	double endval = 1e-3;
	SphericalPendulum(double len, mono init_state) {
		l = len;
		state_vec = init_state;
	}

	mono G(mono state_vec) {
		double dtheta = state_vec[0];
		double dphi = state_vec[1];
		double theta = state_vec[2];
		double phi = state_vec[3];

		double dd_theta = pow(dphi, 2.0) * cos(theta) * sin(theta) - (g / l) * sin(theta);
		double dd_phi = -2.0 * dtheta * dphi / tan(theta);
		return { dd_theta, dd_phi, dtheta, dphi };
	}

	void states2txt(std::string filename, int down_sample = 1, bool header = false) {
		std::ofstream textlog(filename + ".txt");
		//if down_sample = 10, only get every 10th item, etc.
		for (size_t i = 0; i < dtheta_vec.size() / down_sample; i++) {
			int i_corr = static_cast<int>(down_sample * i);
			textlog << time_vec[i_corr] << '\t' << dtheta_vec[i_corr] << '\t' << dphi_vec[i_corr] << '\t' << theta_vec[i_corr] << '\t' << phi_vec[i_corr] << '\n';
		}
	}

	void record_state(mono state, double t) {
		time_vec.push_back(t);
		dtheta_vec.push_back(state_vec[0]);
		dphi_vec.push_back(state_vec[1]);
		theta_vec.push_back(state_vec[2]);
		phi_vec.push_back(state_vec[3]);
	}

	bool truncate_check() {
		if (abs(fmod(state_vec[0], two_pi)) < endval && abs(state_vec[1]) < endval) { return true; }
		return false;
	}

};

template <typename T>
void RK4(T& p, double sim_duration, double h, int rec_every = 1, bool cut_at_endval = true) {
	int num_steps;
	num_steps = static_cast<int>(sim_duration / h);

	mono k1, k2, k3, k4;
	double t = 0.0;

	double hdiv2 = h / 2.0;
	double hdiv6 = h / 6.0;
	double twice_hdiv6 = h / 3.0;
	for (long int n = 0; n < num_steps; n++) {

		// record state at start of step to include t=0.0
		if (((n + rec_every) % rec_every) == 0) {
			p.record_state(p.state_vec, t);
		}

		k1 = p.G(p.state_vec);
		k2 = p.G(Sound::add_monos(p.state_vec, Sound::mono_times_scalar(k1, hdiv2)));
		k3 = p.G(Sound::add_monos(p.state_vec, Sound::mono_times_scalar(k2, hdiv2)));
		k4 = p.G(Sound::add_monos(p.state_vec, Sound::mono_times_scalar(k3, h)));

		Sound::add_many_monos_ref(p.state_vec, 
			{Sound::mono_times_scalar(k1, hdiv6), 
			Sound::mono_times_scalar(k2, twice_hdiv6), 
			Sound::mono_times_scalar(k3, twice_hdiv6), 
			Sound::mono_times_scalar(k4, hdiv6)});


		t += h;

		if (cut_at_endval) {
			if (p.truncate_check()) { return; }
		}
	}
}