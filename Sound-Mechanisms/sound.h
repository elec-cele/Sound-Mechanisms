#pragma once
#include <fstream>   // Apache 2.0
#include <iostream>  // Apache 2.0
#include <vector>    // Apache 2.0
#include <iostream>  // Apache 2.0
#include <random>    // Apache 2.0
#include <map>       // Apache 2.0
#include "contour.h" // GPLv3
#include <sstream>
#include <string>

const extern int sample_rate;
double two_pi = 6.28318530717959;
//TYPES
typedef std::vector<double> mono;
typedef std::vector<mono> stereo;
typedef std::vector<stereo> param_table;
typedef std::vector<Sound::Contour> contour_table;

namespace Sound
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> d_flip(-1, 1);
    std::uniform_real_distribution<double> pct(0, 1);

    // Common vector functions
    double mono_abs_max(mono& vec) {
        double maxval = *max_element(vec.begin(), vec.end());
        double minval = abs(*min_element(vec.begin(), vec.end()));
        return minval > maxval ? minval : maxval;
    }

    void stereo_normalize(stereo& vec) {
        for (int k = 0; k < 2; k++) {
            double maxval = mono_abs_max(vec[k]);
            double reciporical = 1.0 / maxval;
            // (don't normalize sounds that are intentionally less than max volume)
            if (maxval > 1) {
                for (size_t i = 0; i < vec[k].size(); i++) {
                    vec[k][i] = vec[k][i] * reciporical;
                }
            }
        }
    }

    void sum_stereo_vectors(stereo& a, stereo& b) {
        // redundant with add_sounds, only works if both inputs are same size
        for (int i = 0; i < 2; i++) {
            transform(a[i].begin(), a[i].end(), b[i].begin(), a[i].begin(), std::plus<double>());
        }
    }

    void scale_stereo(stereo& a, double b) {
        for (int i = 0; i < 2; i++) {
            for (size_t j = 0; j < a[i].size(); j++) {
                a[i][j] *= b;
            }
        }
    }

    stereo empty_sound(int samps = 0) {
        stereo vec;
        vec.resize(2);
        vec[0].reserve(samps);
        vec[1].reserve(samps);
        for (size_t k = 0; k < samps; k++) {
            vec[0].push_back(0);
            vec[1].push_back(0);
        }
        return vec;
    }

    int longest_env(param_table envelopes) {
        int samples = static_cast<int>(envelopes[0][envelopes[0].size() - 1][0]);
        for (size_t i = 1; i < envelopes.size(); i++) {
            int this_samples = static_cast<int>(envelopes[i][envelopes[i].size() - 1][0]);
            if (this_samples > samples) {
                samples = this_samples;
            }

        }
        return samples;
    }

    int longest_contour(contour_table envelopes) {
        int samples = static_cast<int>(envelopes[0].total_samples);
        for (size_t i = 1; i < envelopes.size(); i++) {
            int this_samples = static_cast<int>(envelopes[i].total_samples);
            if (this_samples > samples) {
                samples = this_samples;
            }

        }
        return samples;
    }


    void add_sounds(stereo& sound_a, stereo& sound_b, int offset_samps) {
        // Adds arbitrary sound_b onto/into sound_a
        for (int i = 0; i < 2; i++) {
            int sound_len_pre = static_cast<int>(sound_a[i].size());
            int this_sound_len = static_cast<int>(sound_b[i].size());
            int gap_samples = offset_samps - sound_len_pre;
            if (gap_samples > 0) {
                for (size_t j = 0; j < gap_samples; j++) {
                    sound_a[i].push_back(0);
                }
                for (size_t j = 0; j < this_sound_len; j++) {
                    sound_a[i].push_back(sound_b[i][j]);
                }
            }
            else if (gap_samples <= 0) {
                int index = std::min(sound_len_pre, this_sound_len + offset_samps);
                int tailing_samples = (offset_samps + this_sound_len) - sound_len_pre;
                int tail_start_index = this_sound_len - tailing_samples;

                for (size_t j = offset_samps; j < index; j++) {
                    sound_a[i][j] += sound_b[i][j - offset_samps];
                }
                if (tailing_samples > 0) {
                    for (int j = tail_start_index; j < this_sound_len; j++) {
                        sound_a[i].push_back(sound_b[i][j]);
                    }
                }
            }
        }
    }
    // end common vector functions

    // Harmony stuff
    typedef enum class intvls {U, m2, M2, m3, M3, P4, A4, TT, d5, P5, m6, M6, m7, M7};

    struct Just_Interval_Ratios {
        double U = 1.0;
        double m2 = 16 / 15.0;
        double M2 = 9 / 8.0;
        double m3 = 6 / 5.0;
        double M3 = 5 / 4.0;
        double P4 = 4 / 3.0;
        double TT = 7 / 5.0;
        double P5 = 3 / 2.0;
        double m6 = 8 / 5.0;
        double M6 = 5 / 3.0;
        double m7 = 9 / 5.0;
        double M7 = 15 / 8.0;

    };

    
    mono ints(int num) {
        mono out;
        for (int i = 1; i <= num; i++) {
            out.push_back((static_cast<double>(i)));
        }
        return out;
    }

    mono evens(int num) {
        mono out;
        out.push_back(1.0);
        for (int i = 1; i < num; i++) {
            out.push_back(static_cast<double>(i) * 2.0);
        }
        return out;
    }

    mono odds(int num) {
        mono out;
        for (int i = 0; i < num; i++) {
            out.push_back(1.0 + ((static_cast<double>(i) * 2.0)));
        }
        return out;
    }

    mono struck_plate(int num) {
        mono out;
        mono master_list = { 1.0, 3.89, 8.72, 2.08, 5.95, 11.75, 3.41, 8.28, 15.06, 5.00, 10.87, 18.63, 6.82, 13.71, 22.47 };
        if (num >= master_list.size()) {
            out = master_list;
        }
        else if (num < master_list.size()) {
            for (int i = 0; i < num; i++) {
                out.push_back(master_list[i]);
            }
        }
        return out;
    }
    mono struck_rigid_membrane(int num) {
        mono out;
        mono master_list = { 1, 1.594, 2.136, 2.296, 2.653, 2.918, 3.156, 3.501, 3.600, 3.652, 4.060, 4.154 };
        if (num >= master_list.size()) {
            out = master_list;
        }
        else if (num < master_list.size()) {
            for (int i = 0; i < num; i++) {
                out.push_back(master_list[i]);
            }
        }
        return out;
    }

    mono marimboid(int num) {
        mono out;
        mono master_list = { 1.0, 4.0, 9.0, 10.0, 12.0, 20.0, 25.0, 25.6, 29.8, 31.9 };
        if (num >= master_list.size()) {
            out = master_list;
        }
        else if (num < master_list.size()) {
            for (int i = 0; i < num; i++) {
                out.push_back(master_list[i]);
            }
        }
        return out;
    }

    class Harmony {
        // General container for frequency patterns like scales, overtone structures, and harmonies
    public:
        double fundamental;
        mono harmonic_structure;
        mono frequencies;

        Harmony() {
            fundamental = 0;
            harmonic_structure = { 0 };
            frequencies = { 0 };
        }

        Harmony(double a, mono b) {
            fundamental = a;
            harmonic_structure = b;
            frequencies = harmonic_structure;
            expand_overtones();
        }

        void expand_overtones(void) {
            for (size_t i = 0; i < harmonic_structure.size(); i++) {
                frequencies[i] = fundamental * harmonic_structure[i];
            }
        }

        void set_fundamental(double new_fund) {
            fundamental = new_fund;
            expand_overtones();
        }

        void transpose_fundamental(double transpose_ratio) {
            fundamental *= transpose_ratio;
            expand_overtones();
        }

        void change_overtones(mono new_overtones) {
            harmonic_structure = new_overtones;
            expand_overtones();
        }

        void skew_overtones(double max_depth) {
            for (size_t i = 1; i < harmonic_structure.size(); i++) {
                harmonic_structure[i] = harmonic_structure[i] * (1.0 + (max_depth * d_flip(rd)));
            }
            expand_overtones();
        }

    };
    // end harmony stuff


    // Crystal structure and sound creation
    class Crystal
    {
        // Crystals are a summation of exponentially decaying sine waves with fixed parameters
        // TODO: nonzero tau clicks at start of exp decay. 
        //       need to add steep rise to start of exp curve to avoid envelope discontinuity

    public:
        mono frequencies;
        mono pannings;
        mono phases;
        mono taus;
        mono intensities;
        stereo sound;

        Crystal(mono freqs, mono pans, mono phs, mono ts, mono intns)
        {
            frequencies = freqs;
            pannings = pans;
            phases = phs;
            taus = ts;
            intensities = intns;
            sound = empty_sound(0);//make_crystal();
        }

        stereo make_crystal(void)
        {
            // Creates a stereo vector of the sound defined by the crystal parameters
            int samples = static_cast<int>(
                *std::max_element(taus.begin(), taus.end()) * sample_rate / 10.0);
            stereo this_sound = empty_sound(samples);

            for (size_t i = 0; i < frequencies.size(); i++) {
                stereo particle = empty_sound(samples);
                double sound_samp, env_samp, pan_factor = 0.0;
                double phase = phases[i];
                double phase_increment = frequencies[i] / sample_rate;

                for (int n = 0; n < samples; n++) {
                    sound_samp = sin(two_pi * phase);
                    phase = fmod((phase + phase_increment), 1);
                    env_samp = exp(-n / (0.18 * taus[i] * sample_rate / 10.0));
                    sound_samp = sound_samp * env_samp * intensities[i];
                    pan_factor = (pannings[i] + 1) * two_pi / 8.0;
                    particle[0][n] = (sound_samp * cos(pan_factor));
                    particle[1][n] = (sound_samp * sin(pan_factor));
                }
                sum_stereo_vectors(this_sound, particle);
            }
            return this_sound;
            std::cout << ".";
        }

        stereo make_smooth_crystal(mono rise_time)
        {
            // Creates a stereo vector of the sound defined by the crystal parameters
            mono lengths;
            for (size_t i = 0; i < frequencies.size(); i++) {
                int rise_samps = rise_time[i] * sample_rate;
                int decay_samps = taus[i] * sample_rate / 10.0;
                lengths.push_back(rise_samps + decay_samps);
            }

            int samples = static_cast<int>(*std::max_element(lengths.begin(), lengths.end()));
            stereo this_sound = empty_sound(samples);

            for (size_t i = 0; i < frequencies.size(); i++) {
                stereo particle = empty_sound(samples);
                double sound_samp, env_samp, pan_factor = 0.0;
                double phase = phases[i];
                double phase_increment = frequencies[i] / sample_rate;

                for (int n = 0; n < samples; n++) {
                    sound_samp = sin(two_pi * phase);
                    phase = fmod((phase + phase_increment), 1);
                    //=IF(samp<=rise_samps,samp/rise_samps,EXP(-decay_tau*(samp-rise_samps)))
                    int rise_samps = rise_time[i] * sample_rate;
                    int decay_samps = taus[i] * sample_rate / 10.0;
                    if (n <= rise_samps) {
                        env_samp = n / static_cast<double>(rise_samps);
                    }
                    else {
                        int corrected_samp = n - rise_samps;
                        env_samp = exp(-corrected_samp / (0.18 * taus[i] * sample_rate / 10.0));
                    }

                    //env_samp = exp(-n / (0.18 * taus[i] * sample_rate / 10.0));
                    sound_samp = sound_samp * env_samp * intensities[i];
                    pan_factor = (pannings[i] + 1) * two_pi / 8.0;
                    particle[0][n] = (sound_samp * cos(pan_factor));
                    particle[1][n] = (sound_samp * sin(pan_factor));
                }
                sum_stereo_vectors(this_sound, particle);
            }
            return this_sound;
            std::cout << ".";
        }
    };
    // end crystal stuff

    // Arb stuff
    class Arb {
    // Sounds with arbitrary definition. Parameters are defined with interpolated points in time

    public:
        contour_table frequencies, pannings, envelopes;
        mono init_phases;
        stereo sound;

        Arb(contour_table freqs, contour_table pans, contour_table envs, mono phs) {
            frequencies = freqs;
            pannings = pans;
            envelopes = envs;
            init_phases = phs;
            sound = make_arb();
        }
        stereo make_arb() {
            int samples = longest_contour(envelopes);
            stereo all_sound = empty_sound(samples);

            for (size_t i = 0; i < frequencies.size(); i++) {
                double sound_samp, env_samp, pan_factor = 0.0;
                double phase = init_phases[i];

                stereo this_particle = empty_sound(0);
                this_particle[0].reserve(samples);
                this_particle[1].reserve(samples);

                for (int n = 0; n < samples; n++) {
                    sound_samp = sin(two_pi * phase);
                    phase = fmod((phase + (frequencies[i].interpolate(n) / sample_rate)), 1);
                    env_samp = envelopes[i].interpolate(n);

                    if (env_samp < 1E-5) {
                        this_particle[0].push_back(0);
                        this_particle[1].push_back(0);
                    }
                    else if (env_samp != 0) {
                        sound_samp = sound_samp * env_samp;
                        pan_factor = (pannings[i].interpolate(n) + 1) * two_pi / 8;
                        this_particle[0].push_back(sound_samp * cos(pan_factor));
                        this_particle[1].push_back(sound_samp * sin(pan_factor));
                    }
                }
                sum_stereo_vectors(all_sound, this_particle);
            }
            stereo_normalize(all_sound);
            return all_sound;
        }
    };
    //end arb stuff
}   // end Sound namespace


// wav output 
namespace Wav
{
    //https://cplusplus.com/forum/beginner/166954/
    //https://en.cppreference.com/w/cpp/language/range-for
    template <typename Word>
    std::ostream& write_word_little_endian(std::ostream& outs, Word value, unsigned size = sizeof(Word))
    {
        for (; size; --size, value >>= 8)
            outs.put(static_cast<char> (value & 0xFF));
        return outs;
    }

    void write_stereo_wav(const std::string& filename, stereo sound_out, int auto_play = 1) {
        int32_t sxtn = 0b00010000;
        short int one = 0b01;
        short int two = 0b10;
        short int four = 0b0100;
        int32_t samp_rate = sample_rate;   // can't reinterpret cast of const
        int32_t total_rate = 4 * samp_rate;
        int32_t bit_rate = sample_rate * 4;
        constexpr double max_amplitude = 32760;  // for scaling [-1, 1] to [-32760, 32760] PCM format int range
        std::ofstream out_stream(filename, std::ios::binary);

        // RIFF WAVE header 
        out_stream << "RIFF";
        out_stream << "----"; // file size added at end
        out_stream << "WAVEfmt ";
        Wav::write_word_little_endian(out_stream, 16, 4);     // Length of format data
        Wav::write_word_little_endian(out_stream, 1, 2); // PCM format
        Wav::write_word_little_endian(out_stream, 2, 2); // Stereo format
        Wav::write_word_little_endian(out_stream, sample_rate, 4);  // samples per second (Hz)
        Wav::write_word_little_endian(out_stream, total_rate, 4); // bit rate
        Wav::write_word_little_endian(out_stream, 4, 2);    // data block size
        Wav::write_word_little_endian(out_stream, 16, 2);  // bits per sample

        out_stream << "data";
        size_t data_size_loc = out_stream.tellp();
        out_stream << "----";  // data size added later

        // write the audio data
        size_t num_samples = sound_out[0].size();
        for (int i = 0; i < num_samples; i++) {
            int32_t left = static_cast<int>(max_amplitude * sound_out[0][i]);
            int32_t right = static_cast<int>(max_amplitude * sound_out[1][i]);
            Wav::write_word_little_endian(out_stream, left, 2);
            Wav::write_word_little_endian(out_stream, right, 2);
        }

        // fill in the blanks
        size_t data_size = out_stream.tellp();
        out_stream.seekp(data_size_loc);
        data_size = data_size - data_size_loc + 4;
        Wav::write_word_little_endian(out_stream, data_size, 4);
        out_stream.seekp(4);

        int32_t file_size = data_size - 8;
        Wav::write_word_little_endian(out_stream, file_size, 4);
        out_stream.close();
        if (auto_play) {
            system(("explorer " + filename).c_str());
        }

    }
}
//end wav output


//// Sequencer idea
//{
//    // one number per beat, 1 = sound, = 0 no sound
//    mono samp_seq = { 1, 0, 1, 0, 1, 1, 0, 0 };
//    int num_repeats = 4;
//    int beat_count = 0;
//    double time_divider = 1 / 8.0; //eighth notes
//    for (size_t i = 0; i < samp_seq.size() * num_repeats; i++) {
//        // Crystal(mono freqs, mono pans, mono phs, mono ts, mono intns)
//        
//        if (samp_seq[beat_count % samp_seq.size()] == 1) {
//            Sound::Crystal beat_sound({ 50.0 * (beat_count + 1) }, { 0 }, { 0 }, { 1 }, { 1 });
//            stereo this_beat = beat_sound.make_crystal();
//            int samps_from_start = beat_count * samp_per_beat * time_divider;
//            Sound::add_sounds(composition_space, this_beat, samps_from_start);
//        }
//        beat_count += 1;
//    }
//}