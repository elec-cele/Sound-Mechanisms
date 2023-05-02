#include <iostream>
#include <vector>    // Apache 2.0
#include <fstream>

namespace Pendulum {

	mono data_log(int samps = 0) {
		mono vec;
		vec.reserve(samps);
		for (size_t k = 0; k < samps; k++) {
			vec.push_back(0);
		}
		return vec;
	}

	coordinates coordinates_log(int samps = 0) {
		coordinates coords;
		coords.reserve(samps);
		for (size_t k = 0; k < samps; k++) {
			coords.push_back({ 0, 0 });
		}
		return coords;
	}

	int duration = 60;
	int sim_sample_rate = 48; //nominal 48000/1000
	double time = 0.0;
	int num_samps = duration * sim_sample_rate;
	

	mono log = data_log(num_samps);
	mono xpos = data_log(num_samps);
	mono ypos = data_log(num_samps);
	mono dist = data_log(num_samps);

	coordinates dist_contour = coordinates_log(num_samps);
	coordinates pan_contour = coordinates_log(num_samps);
	mono phs;

	void generate_pendulum_data() {

		//https://ximera.osu.edu/ode/main/simplePendulum/simplePendulum
		const double e = 2.7182818284;
		const double two_pi = 2 * 3.14159265358;

		double theta_0 = 60; //initial pendulum angle from horizontal
		double theta = 0.0;
		double a = 1; //'drag'
		double l = 50; //pendulum length
		double g = 9.8; //gravity
		double z = 0.0001;
		double b = sqrt(4 * l * g - a * a) / (2 * l);
		double decay = (-1 * a) / (2 * l);
		double h0 = 54; // height of top of pendulum above (0,0)



		const double deg_to_rad = 0.017453;


		double phase = 0.0;
		for (int n = 0; n < num_samps; n++) {
			double phase_increment = b / sim_sample_rate;
			double time_increment = 1.0 / sim_sample_rate;

			theta = pow(e, decay * time) * (theta_0 * cos(phase) + z * sin(phase));
			log[n] = theta;

			phase = fmod((phase + phase_increment), two_pi);
			time += time_increment;

			xpos[n] = l * sin(deg_to_rad * theta);
			ypos[n] = h0 - l * cos(deg_to_rad * theta);


			dist[n] = sqrt(pow(xpos[n], 2) + pow(ypos[n], 2));
			int samp_converted = (48000 / sim_sample_rate) * n;
			//contours for sound.h need to be in form std::vector<std::pair<int, double>> coordinates
			//where int is the sample rate in the sound.h application, typ 48000
			dist_contour[n] = { samp_converted, 1 - (dist[n] / (l))  };
			pan_contour[n] = { samp_converted, xpos[n] / l };
			phs.push_back(0);
		}
		
	}


void generate_pendulum_sound() {
		stereo composition_space = Sound::empty_sound();

		// idea: for number of samples, use distance and panning to control a basic sine wave
		//        Arb(contour_table freqs, contour_table pans, contour_table envs, mono phs) {
		Sound::Contour freq;
		freq.add_flat_line(duration, 256*1.5);
		contour_table freq_tbl = { freq };
		
		Sound::Contour env(dist_contour);
		contour_table env_tbl = { env };

		Sound::Contour panc(pan_contour);
		contour_table pan_tbl = { panc };

		Sound::Arb this_arb(freq_tbl, pan_tbl, env_tbl, phs);
		Sound::add_sounds(composition_space, this_arb.sound, 0);

		// Normalize the composition audio and write it to a wav file
		Sound::stereo_normalize(composition_space);
		std::string filename = "pendulum4.wav";
		Wav::write_stereo_wav(filename, composition_space, 1);
	}

};

