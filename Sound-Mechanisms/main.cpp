#include "sound.h"
#include "bitmap.h"
#include<chrono>


const int SAMPLE_RATE = 96000;

int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	// initialize bitmap wavetable
	Bitmap wf("drawings/wfs/wf6.bmp");
	wf.array_setup();

	Bitmap freqwf("drawings/composition/c1.bmp");
	freqwf.array_setup(false);


	stereo composition_space = Sound::empty_sound();
	int offset = 0;
	double duration = 4.0;

	Sound::Contour freq_line(freqwf.get_raw_coords(static_cast<int>((duration * SAMPLE_RATE) / freqwf.width)));
	freq_line.mute_contour(0.02, 1e-3);

	Sound::Contour freq_lineb(freqwf.get_zeroless_scaled_coords(static_cast<int>((duration * SAMPLE_RATE) / freqwf.width), 40.0, 5120.0));

	Sound::Contour pan_line;
	Sound::Contour env_line;
	Sound::Contour phs_line;

	env_line.add_flat_line(freq_line.total_duration, 1.0);
	pan_line.add_flat_line(freq_line.total_duration, 0.0);
	phs_line.add_flat_line(freq_line.total_duration, 0.0);

	Sound::ArbSingle this_arb(freq_lineb, pan_line, phs_line, env_line);
	this_arb.make_arbsingle_wavetable(wf);
	Sound::stereo_times_mono(this_arb.sound, freq_line.mute);

	Sound::add_sounds(composition_space, this_arb.sound, offset);
	std::cout << '.';



	Sound::stereo_normalize(composition_space, true);

	Wav::write_stereo_wav("temp/test.wav", composition_space, 0);

	auto total = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
	std::cout << "time: " << total << '\n';
	return 0;
}
