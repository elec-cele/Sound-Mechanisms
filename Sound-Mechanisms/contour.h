#pragma once
#include "sound.h"
#include <vector>
typedef std::vector<std::pair<int, double>> coordinates;
typedef std::vector<double> mono;
const extern int SAMPLE_RATE;

namespace Sound {

    class Contour {
    public:
        double total_duration = 0.0; //units of seconds
        int total_samples = 0;
        coordinates coords = {};
        double samp_duration = 1 / (SAMPLE_RATE);
        double last_val = 0.0;
        double slope, intercept, segment_max_samples = 0.0;
        int offset = 0;

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

        void solve_segment(void) {
            double slope_num = std::get<double>(coords[offset + 1]) - std::get<double>(coords[offset]);
            double slope_den = std::get<int>(coords[offset + 1]) - std::get<int>(coords[offset]);
            slope = slope_num / slope_den;
            intercept = std::get<double>(coords[offset + 1]) - std::get<int>(coords[offset + 1]) * slope;
            segment_max_samples = std::get<int>(coords[offset + 1]);
            offset += 1;
        }

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
            coords.push_back({total_samples + duration_samples, value });
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
                this_sample = last_val + mod_depth * sin(6.28318530717959 * phase);
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

    };
}