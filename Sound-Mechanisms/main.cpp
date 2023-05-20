#include "sound.h"
#include "bitmap.h"
#include<chrono>
#include<string>
#include "pendulum.h"
#include "keyboard.h"
const int SAMPLE_RATE = 96000;

template <typename T>
double sign(T x) {
	if (x >= 0.0) { return 1.0; }
	return -1.0;
}

mono mute2(Sound::Contour cont, double ramp_time, double gate = 1E-3) {
	mono interp;
	for (int i = 0; i < cont.total_samples; i++) {
		interp.push_back(cont.interpolate(i));
	}
	mono mute;

	int k = static_cast<int>(ramp_time * SAMPLE_RATE);
	double accumulate = interp[0];
	int nmax = cont.total_samples;

	for (int n = 0; n < nmax; n++) {
		if (interp[n] <= gate) {
			accumulate = 0;
			mute.push_back(0);
		}
		else {
			int samps_after = std::min(nmax - n, k);
			int index_after = n + samps_after;
			double a = 1.0 / samps_after;
			double nextsum = a * std::reduce(interp.begin() + n, interp.begin() + index_after);
			if (nextsum <= accumulate) {
				accumulate = std::min(1.0, nextsum);
				mute.push_back(accumulate);
			}
			else {
				accumulate += a;
				mute.push_back(std::min(accumulate, 1.0));
			}
		}
	}
	return mute;
}



int main()
{
	auto start = std::chrono::high_resolution_clock::now();
	// initialize bitmap wavetables
	Bitmap wf1("wf1.bmp");
	wf1.array_setup();
	Bitmap wf2("wf2.bmp");
	wf2.array_setup();
	Bitmap wf3("wf3.bmp");
	wf3.array_setup();
	Bitmap wf4("wf4.bmp");
	wf4.array_setup();
	Bitmap wf5("wf5.bmp");
	wf5.array_setup();
	Bitmap wf6("wf6.bmp");
	wf6.array_setup();
	Bitmap wf7("wf7.bmp");
	wf7.array_setup();
	Bitmap wf8("wf8.bmp");
	wf8.array_setup();
	Bitmap wf9("wf9.bmp");
	wf9.array_setup();
	std::vector<Bitmap> bmp_wts = { wf1, wf2, wf3, wf4, wf5, wf6, wf6, wf7, wf8, wf9 };



	//initialize keyboard
	Keys::Keyboard k1("C3", 130.8128, Keys::SEMITONE_ET_12);
	mono kf = k1.notes_to_freqs({"G2", "B2", "C2", "E2", "F#2", "C3", "E3", "F#3", "G3", "A4", "B4", "C4", "D4", "E4", "F#4", "G4"});

	Bitmap freqwf("freq_cont_draw_8.bmp");
	freqwf.array_setup(false);
	freqwf.quantize(kf);

	Bitmap freqwf2("freq_cont_draw_8.bmp");
	freqwf2.array_setup();

	Bitmap freqwf3("freq_cont_draw_5.bmp");
	freqwf3.array_setup(false);
	Bitmap freqwf4("freq_cont_draw_6.bmp");
	freqwf4.array_setup(false);
	stereo composition_space = Sound::empty_sound();
	int offset = 0;
	double duration = 30.0;

	//x len freq contour to y seconds at SAMPLE_RATE
	//scale factor = (y*SAMPLE_RATE)/x


		
		Sound::Contour phs_line(freqwf4.get_coords(static_cast<int>((duration * SAMPLE_RATE) / freqwf4.width)));
		Sound::Contour freq_line(freqwf.get_coords(static_cast<int>((duration*SAMPLE_RATE)/freqwf.width), 0.0, 1024));

		mono freq_mute = mute2(freq_line, 0.02, 0.001);

		//Sound::Contour freq_line(freqwf.get_quant_coords(static_cast<int>((duration * SAMPLE_RATE) / freqwf.width)));
		Sound::Contour pan_line(freqwf2.get_coords(static_cast<int>((duration * SAMPLE_RATE) / freqwf2.width)));
		Sound::Contour env_line;//(freqwf2.get_coords(static_cast<int>((duration * SAMPLE_RATE) / freqwf2.width)));

		env_line.add_flat_line(freq_line.total_duration, 1.0);
		//pan_line.add_flat_line(freq_line.total_duration, 0.0);
		//phs_line.add_flat_line(freq_line.total_duration, 0.0);

		Sound::ArbSingle this_arb(freq_line, pan_line, phs_line, env_line);
		this_arb.make_arbsingle_wavetable(bmp_wts[6]);
		Sound::stereo_times_mono(this_arb.sound, freq_mute);

		Sound::add_sounds(composition_space, this_arb.sound, offset);
		std::cout << '.';
		//offset += 10.0 * SAMPLE_RATE;
	


	Sound::stereo_normalize(composition_space, true);

	Wav::write_stereo_wav("freq_draw_11a3q2.wav", composition_space, 1);

	auto total = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
	std::cout << "time: " << total << '\n';
	return 0;
}
