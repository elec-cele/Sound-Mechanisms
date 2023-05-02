#include <iostream>
#include <vector>    // Apache 2.0
#include <fstream>

namespace ArbSingleTest {


	void testfunc() {


		Sound::Contour freq;
		Sound::Contour pan;
		Sound::Contour phs;
		Sound::Contour env;

		freq.add_flat_line(0, 256);
		freq.ramp_to(5, 1000);
		pan.add_flat_line(0, -1);
		pan.ramp_to(5, 1);
		phs.add_flat_line(0, 0.5);
		phs.flat_sine_mod(5, 20, 0.5);
		env.add_flat_line(5, 1);


		Sound::ArbSingle test(freq, pan, phs, env);

		Wav::write_stereo_wav("arbsingletest.wav", test.sound, 1);
	}

};

