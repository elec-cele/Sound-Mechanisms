#include "sound.h"
const int sample_rate = 48000;

int main()
{
    // Example composition - start with a null sound
    stereo composition_space = Sound::empty_sound();
    //composition_space.reserve(1000000000);

    // Create a Crystal, add it to the composition
    mono frequencies = { 100, 1000, 1250 };
    mono pannings = { 0, -1, 1 };
    mono phases = { 0, 0, 0 };
    mono taus = { 30, 15, 20 };
    mono intensities = { 1, 1, 0.8 };
    Sound::Crystal this_crystal(frequencies, pannings, phases, taus, intensities);
    Sound::add_sounds(composition_space, this_crystal.sound, 0);
    
    // Create an Arb roughly simulating a siren, add it to the composition
    Sound::Harmony horn(500, Sound::ints(6));
    for (size_t q = 0; q < horn.frequencies.size(); q++) {
        double tq = static_cast<double>(q + 1);
        for (int i = 0; i < 2; i++) {
            Sound::Contour freq_line, env_line, pan_line;
            freq_line.trapezoid(0, 2, horn.frequencies[q] * (1 + (i * 0.2)), 6, 2, 200);
            env_line.trapezoid(0, 0.5, 0.2 / tq, 9, 0.5, 0);
            pan_line.trapezoid(Sound::d_flip(Sound::rd), 3, Sound::d_flip(Sound::rd), 3, 4, Sound::d_flip(Sound::rd));
            Sound::Arb this_arb({ freq_line }, { pan_line }, { env_line }, { 0 });
            Sound::add_sounds(composition_space, this_arb.sound, sample_rate);
        }
    }

    // Normalize the composition audio and write it to a wav file
    Sound::stereo_normalize(composition_space);
    Wav::write_stereo_wav("sound.wav", composition_space);
    
    return 0;
}
