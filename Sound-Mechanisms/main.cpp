#include "sound.h"
#include "bitmap.h"
#include<chrono>
#include<string>
#include "canada_goose.h"
#include "keyboard.h"
const int SAMPLE_RATE = 96000;



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
	std::vector<Bitmap> bmp_wts = {wf1, wf2, wf3, wf4, wf5, wf6, wf6, wf7, wf8, wf9};


	//initialize keyboard
	Keys::Keyboard test_keyboard("C3", 130.8128, Keys::SEMITONE_ET_12);

	stereo composition_space = Sound::empty_sound();
	int offset = 0;

	for (int i = 0; i < 2; i++) {
		// for each of the two progressions, make and arb of each chord in succsession
		// the second progression resets time s.t. it overlaps the first
		offset = 0;
		progression this_prog = cg_a_top;
		if (i == 1) {
			this_prog = cg_a_bottom;
		}

		for (size_t j = 0; j < this_prog.size(); j++) {
			//figure out how many frequencies in this chord
			mono these_freqs = test_keyboard.notes_to_freqs(this_prog[j]);
			double chord_duration = 2;

			for (size_t k = 0; k < these_freqs.size(); k++) {
				// for each frequency in the chord, make an arb and stick them all on top of each other
				// (note: for chords with interval structures on each frequency, add another loop over the intervals)
				Sound::Contour freq_line, env_line, pan_line, phs;


				double ramp_time = (chord_duration * (Sound::pct(Sound::rd) + 0.5) / 5.0);
				double top_time = (chord_duration) - 2.0 * ramp_time;

				double this_phs = Sound::d_flip(Sound::rd);
				freq_line.add_flat_line(chord_duration, these_freqs[k]);
				env_line.trapezoid(0, ramp_time, Sound::pct(Sound::rd), top_time, ramp_time, 0);
				pan_line.trapezoid(Sound::d_flip(Sound::rd), ramp_time, Sound::d_flip(Sound::rd), top_time, ramp_time, Sound::d_flip(Sound::rd));
				phs.add_flat_line(chord_duration, 0.0);


				Sound::ArbSingle this_arb(freq_line, pan_line, phs, env_line );
				this_arb.make_arbsingle_wavetable(wf9);
				//bmp_wts[static_cast<int>(Sound::pct(Sound::rd) * bmp_wts.size())]


				Sound::add_sounds(composition_space, this_arb.sound, offset);
				//std::cout << ".";
			}
			//after sticking the chord together on top of itself, move the offset over
			offset += (chord_duration - 0.5) * SAMPLE_RATE;
			//std::cout << "-";
		}
		//std::cout << "=";
	}

	Sound::stereo_normalize(composition_space, true);

	Wav::write_stereo_wav("canada_goose_wt_alpha_4_wf9.wav", composition_space, 1);

	auto total = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
	std::cout << "time: " << total << '\n';
	return 0;
}
