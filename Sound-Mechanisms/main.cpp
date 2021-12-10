#include "sound.h"
const int sample_rate = 48000;

int main()
{
    Sound::Just_Interval_Ratios just_int;

    // Example composition - start with a null sound
    stereo composition_space = Sound::empty_sound();

    // Create a Crystal, add it to the composition
    mono frequencies = { 100, 1000, 1250 };
    mono pannings = { 0, -1, 1 };
    mono phases = { 0, 0, 0 };
    mono taus = { 30, 15, 20 };
    mono intensities = { 2, 2, 3 }; // this helps the crystal stand out after final normalization before saving file
    Sound::Crystal this_crystal(frequencies, pannings, phases, taus, intensities);
    Sound::add_sounds(composition_space, this_crystal.sound, 0);

    // Create an Arb roughly simulating a siren, add it to the composition
    Sound::Harmony siren_fundamentals(500, {just_int.U, just_int.m3});
    for (size_t q = 0; q < siren_fundamentals.frequencies.size(); q++) {
        Sound::Harmony siren_overtones(siren_fundamentals.frequencies[q], Sound::ints(12));
        for (int i = 0; i < siren_overtones.frequencies.size(); i++) {
            double tq = static_cast<double>(i + 1);
            Sound::Contour freq_line, env_line, pan_line;
            freq_line.trapezoid(0, 2, siren_overtones.frequencies[i], 2, 2, 200);
            env_line.trapezoid(0, 0.5, 1 / tq, 4, 0.5, 0);
            pan_line.trapezoid(Sound::d_flip(Sound::rd), 2, Sound::d_flip(Sound::rd), 2, 2, Sound::d_flip(Sound::rd));
            Sound::Arb this_arb({ freq_line }, { pan_line }, { env_line }, { 0 });
            Sound::add_sounds(composition_space, this_arb.sound, sample_rate);
        }
    }

    // Create an Arb of a nicer chord progression with expressive modulations
    mono maj9 = { just_int.U, just_int.M3, just_int.P5, just_int.M7, 2 * just_int.M2 }; // 1, 3, 5, 7, 9 maj9 chord
    mono min9 = { just_int.U, just_int.m3, just_int.P5, just_int.m7, 2 * just_int.M2 }; // 1 m3 5 m7 9 min9 chord
    double fundamental = 90;
    Sound::Harmony fundamentals(fundamental, maj9);

    int offset = 6 * sample_rate;
    int arb_length = 0;
    for (int c = 1; c < 7; c++) {
        for (size_t i = 0; i < maj9.size(); i++) {
            Sound::Harmony overtones(fundamentals.frequencies[i], Sound::ints(22));
            overtones.skew_overtones(0.005);
            for (size_t j = 0; j < overtones.frequencies.size(); j++) {
                Sound::Contour freq_line, env_line, pan_line;
                mono phs;
                double chord_len = 3;
                // start at the center frequency, then sine mod for the rest of the duration
                double flat_len = chord_len * Sound::pct(Sound::rd);
                
                freq_line.add_flat_line(flat_len, overtones.frequencies[j]);
                freq_line.flat_sine_mod(chord_len - flat_len, 2 * (0.1 + Sound::pct(Sound::rd)), freq_line.last_val * Sound::pct(Sound::rd) * 0.02);

                // trapezoidal envelope with maxval weighted toward lower frequencies 
                double t_rise = 1;
                double t_fall = 1;
                double top_len = chord_len - (t_rise + t_fall);
                env_line.trapezoid(0, t_rise, Sound::pct(Sound::rd) / (j + 1.0), top_len, t_fall, 0);

                // random speed and depth panning
                pan_line.flat_sine_mod(chord_len, c * Sound::pct(Sound::rd), Sound::pct(Sound::rd));

                // random initial phase
                phs.push_back(Sound::pct(Sound::rd));

                Sound::Arb this_arb({ freq_line }, { pan_line }, { env_line }, { phs });
                Sound::add_sounds(composition_space, this_arb.sound, offset);
                arb_length = this_arb.sound[0].size();
            }
        }
        offset += arb_length - sample_rate * 0.2;
        fundamentals.transpose_fundamental(just_int.M3);    // shift up a major third
        if (c % 2 == 0) {
            fundamentals.change_overtones(min9);
        }
        else {
            fundamentals.change_overtones(maj9); 
        }
    }

    //Melody of Crystals -- basis of percussion 
    Sound::Harmony melody(100, { just_int.U, just_int.m2, just_int.M3, just_int.P4, just_int.m6, just_int.M7 });
    int melody_offset = composition_space[0].size();

    // go through two octaves
    for (int i = 0; i < 2; i++) {
        for (size_t j = 0; j < melody.frequencies.size(); j++) {
            Sound::Harmony marimb(melody.frequencies[j] * (i + 1.0), Sound::struck_plate(10));
            mono frequencies, pannings, phases, taus, intensities;
            for (size_t k = 0; k < marimb.frequencies.size(); k++) {
                frequencies.push_back(marimb.frequencies[k]);
                pannings.push_back(Sound::pct(Sound::rd));
                phases.push_back(0);
                taus.push_back(10 * Sound::pct(Sound::rd));
                intensities.push_back(1.0 / (k + 1));
            }
            Sound::Crystal new_crystal(frequencies, pannings, phases, taus, intensities);
            Sound::add_sounds(composition_space, new_crystal.sound, melody_offset);
            melody_offset += 20E3;
        }
    }

    // Normalize the composition audio and write it to a wav file
    Sound::stereo_normalize(composition_space);
    Wav::write_stereo_wav("sound.wav", composition_space);
    
    return 0;
}
