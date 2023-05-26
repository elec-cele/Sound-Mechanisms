#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
typedef std::vector<std::pair<int, double>> coordinates;
typedef std::vector<double> mono;
const extern int SAMPLE_RATE;
const extern double two_pi;
namespace Sound {

	class Contour {
	public:
		double total_duration = 0.0; //units of seconds
		int total_samples = 0;
		coordinates coords = {};
		double samp_duration = 1.0 / SAMPLE_RATE;
		double last_val = 0.0;
		double slope, intercept, segment_max_samples = 0.0;
		int offset = 0;
		mono interpolated;
		mono mute;

		Contour() {
			total_duration, last_val = 0.0;
			total_samples = 0;
			coords = { {0,0} };
		}

		Contour(coordinates start_coords) {
			coords = start_coords;
			offset = 0;
			solve_segment();
			update_duration();
		}

		Contour(mono time_vec, mono val_vec) {
			for (size_t n = 0; n < time_vec.size(); n++) {
				int time2sample = static_cast<int>(SAMPLE_RATE * time_vec[n]);
				coords.push_back({ time2sample, val_vec[n] });
			}
			offset = 0;
			solve_segment();
			update_duration();

		}

		// TODO: get rid of this
		void solve_segment(void) {
			double slope_num = std::get<double>(coords[offset + 1]) - std::get<double>(coords[offset]);
			double slope_den = std::get<int>(coords[offset + 1]) - std::get<int>(coords[offset]);
			slope = slope_num / slope_den;
			intercept = std::get<double>(coords[offset + 1]) - std::get<int>(coords[offset + 1]) * slope;
			segment_max_samples = std::get<int>(coords[offset + 1]);
			offset += 1;
		}

		//TODO: stop using this.just use interpolate_contour and work from that mono vector.
		// need to untangle from sound.h and anywhere else it's referenced
		double interpolate(int x_a) {
			double y_a = 0.0;

			if (x_a <= segment_max_samples) {
				y_a = slope * x_a + intercept;
			}
			else if (offset + 1 < coords.size()) {
				solve_segment();
				y_a = slope * x_a + intercept;
			}
			else if (x_a > total_samples) {
				y_a = 0.0;
			}
			return y_a;
		}

		//don't use this either. just use interpolate_contour and work from that mono vector.
		double interp_single(int x_a) {
			//linear interpolation of sample x_a within coords
			if (x_a > total_samples) { return -1.0; };

			bool found = false;
			int i = 0;
			while (!found) {
				int max_next = std::min(total_samples, i + 1);
				double x_n = static_cast<double>(std::get<int>(coords[i]));
				double y_n = std::get<double>(coords[i]);
				if (x_n == x_a) { return y_n; }

				double x_n_next = static_cast<double>(std::get<int>(coords[max_next]));

				if (x_n <= x_a && x_a <= x_n_next) {
					double y_n_next = std::get<double>(coords[max_next]);
					double slope_num = y_n_next - y_n;
					double slope_den = x_n_next - x_n;
					slope = slope_num / slope_den;
					intercept = y_n_next - x_n_next * slope;

					return slope * x_a + intercept;
				}

				else {
					i++;
				}
			}
		}


		void update_duration(void) {
			total_samples = std::get<int>(coords[coords.size() - 1]);
			last_val = std::get<double>(coords[coords.size() - 1]);
			total_duration = total_samples / static_cast<double>(SAMPLE_RATE);
		}

		void add_flat_line(double line_duration, double value) {
			double line_from = std::get<double>(coords[coords.size() - 1]);
			int duration_samples = static_cast<int>(line_duration * SAMPLE_RATE);
			if (line_from != value) {
				coords.push_back({ total_samples + 1, value });
			}
			coords.push_back({ total_samples + duration_samples, value });
			//int duration_samples = static_cast<int>(line_duration * SAMPLE_RATE);
			//if (coords.size() > 0) {
			//	coords.push_back({ total_samples + 1, value });
			//}
			//coords.push_back({ total_samples + duration_samples, value });
			update_duration();
		}

		void ramp_to(double ramp_duration, double ramp_to) {
			int duration_samples = static_cast<int>(ramp_duration * SAMPLE_RATE);
			coords.push_back({ total_samples + duration_samples, ramp_to });
			update_duration();
		}

		void flat_sine_mod(double duration, double mod_freq, double mod_depth) {
			int this_samp_freq = SAMPLE_RATE / 10;  // NOTE: limits modulation bandwidth to 10% of audio bandwidth to save memory.
			double freq_ratio = SAMPLE_RATE / (double)this_samp_freq;
			int num_samples = static_cast<int>(this_samp_freq * duration);
			double this_sample;
			double phase = 0;
			for (int i = 0; i < num_samples; i++) {
				this_sample = last_val + mod_depth * sin(two_pi * phase);
				phase += mod_freq / this_samp_freq;
				if (i == 0) {
					coords.push_back({ total_samples + 1, this_sample });
				}
				else if (i > 0) {
					coords.push_back({ total_samples + static_cast<int>(i * freq_ratio), this_sample });
				}
			} update_duration();
		}

		void flat_ramp_flat(double start_flat_duration,
			double start_val,
			double rise_time,
			double end_val,
			double end_flat_duration) {
			add_flat_line(start_flat_duration, start_val);
			ramp_to(rise_time, end_val);
			add_flat_line(end_flat_duration, end_val);
		}

		void trapezoid(double start_val,
			double rise_time,
			double peak_val,
			double peak_duration,
			double fall_time,
			double end_val) {
			coords.push_back({ total_samples + 1, start_val });
			ramp_to(rise_time, peak_val);
			ramp_to(peak_duration, peak_val);
			ramp_to(fall_time, end_val);
		}

		mono interpolate_contour() {
			// fill in the gaps between known (x0,y0), (x1,y1), ... (x_n-1, y_n-1), (x_n, y_n)
			interpolated.reserve(total_samples);
			for (size_t i = 0; i < coords.size() - 1; i++) {
				//solve this segment equation, push back necessary quantity of samples until next segment.
				int max_next = std::min(total_samples, static_cast<int>(i + 1));

				int x_n = std::get<int>(coords[i]);
				double y_n = std::get<double>(coords[i]);
				int x_n_next = std::get<int>(coords[max_next]);
				double y_n_next = std::get<double>(coords[max_next]);

				double slope = (y_n_next - y_n) / (x_n_next - x_n);
				double intercept = y_n_next - x_n_next * slope;

				int samps_in_segment = x_n_next - x_n;

				for (int j = 0; j < samps_in_segment; j++) {
					interpolated.push_back(slope * (i + j) + intercept);
				}
			}
			return interpolated;
		}

		mono mute_contour(double ramp_time, double gate = 1E-3) {
			if (interpolated.size() == 0) {
				interpolate_contour();
			}
			mute.reserve(total_samples);
			int k = static_cast<int>(ramp_time * SAMPLE_RATE);
			double accumulate = interpolated[0];
			int nmax = total_samples;

			for (int n = 0; n < nmax; n++) {
				int samps_after = std::min(nmax - n, k);
				int index_after = n + samps_after;
				double a = 1.0 / samps_after;

				if (interpolated[n] == 0.0) {
					accumulate -= a;
					if (accumulate < 0) { accumulate = 0; }
				}
				else {
					accumulate += a;
					if (accumulate > 1) { accumulate = 1; }
				}
				mute.push_back(accumulate);
			}
			return mute;
		}

	};
}