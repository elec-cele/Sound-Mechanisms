#include <iostream>
#include <vector>    // Apache 2.0
#include <fstream>

namespace ArbSingleTest {


	void testfunc() {


		Sound::Contour freq;
		Sound::Contour pan;
		Sound::Contour phs;
		Sound::Contour env;

		freq.add_flat_line(0, 512);
		freq.flat_sine_mod(5, 64, 256);
		pan.add_flat_line(0, 0);
		pan.flat_sine_mod(5, 1, 1);
		phs.add_flat_line(0, 0.5);
		phs.flat_sine_mod(5, 32, 100);
		env.trapezoid(0, 1, 1, 3, 1, 0);


		Sound::ArbSingle test(freq, pan, phs, env);

		Wav::write_stereo_wav("arbsingletestb.wav", test.sound, 1);
	}

};

