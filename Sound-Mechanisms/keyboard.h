#pragma once
#include <map>
#include <string>
#include <iostream>  // Apache 2.0

namespace Keys {
	std::map<std::string, int> note_to_keynum = {
	{"C0", 0},
	{"C#0", 1},
	{"Db0", 1},
	{"D0", 2},
	{"D#0", 3},
	{"Eb0", 3},
	{"E0", 4},
	{"F0", 5},
	{"F#0", 6},
	{"Gb0", 6},
	{"G0", 7},
	{"G#0", 8},
	{"Ab0", 8},
	{"A0", 9},
	{"A#0", 10},
	{"Bb0", 10},
	{"B0", 11},
	{"C1", 12},
	{"C#1", 13},
	{"Db1", 13},
	{"D1", 14},
	{"D#1", 15},
	{"Eb1", 15},
	{"E1", 16},
	{"F1", 17},
	{"F#1", 18},
	{"Gb1", 18},
	{"G1", 19},
	{"G#1", 20},
	{"Ab1", 20},
	{"A1", 21},
	{"A#1", 22},
	{"Bb1", 22},
	{"B1", 23},
	{"C2", 24},
	{"C#2", 25},
	{"Db2", 25},
	{"D2", 26},
	{"D#2", 27},
	{"Eb2", 27},
	{"E2", 28},
	{"F2", 29},
	{"F#2", 30},
	{"Gb2", 30},
	{"G2", 31},
	{"G#2", 32},
	{"Ab2", 32},
	{"A2", 33},
	{"A#2", 34},
	{"Bb2", 34},
	{"B2", 35},
	{"C3", 36},
	{"C#3", 37},
	{"Db3", 37},
	{"D3", 38},
	{"D#3", 39},
	{"Eb3", 39},
	{"E3", 40},
	{"F3", 41},
	{"F#3", 42},
	{"Gb3", 42},
	{"G3", 43},
	{"G#3", 44},
	{"Ab3", 44},
	{"A3", 45},
	{"A#3", 46},
	{"Bb3", 46},
	{"B3", 47},
	{"C4", 48},
	{"C#4", 49},
	{"Db4", 49},
	{"D4", 50},
	{"D#4", 51},
	{"Eb4", 51},
	{"E4", 52},
	{"F4", 53},
	{"F#4", 54},
	{"Gb4", 54},
	{"G4", 55},
	{"G#4", 56},
	{"Ab4", 56},
	{"A4", 57},
	{"A#4", 58},
	{"Bb4", 58},
	{"B4", 59},
	{"C5", 60},
	{"C#5", 61},
	{"Db5", 61},
	{"D5", 62},
	{"D#5", 63},
	{"Eb5", 63},
	{"E5", 64},
	{"F5", 65},
	{"F#5", 66},
	{"Gb5", 66},
	{"G5", 67},
	{"G#5", 68},
	{"Ab5", 68},
	{"A5", 69},
	{"A#5", 70},
	{"Bb5", 70},
	{"B5", 71},
	{"C6", 72},
	{"C#6", 73},
	{"Db6", 73},
	{"D6", 74},
	{"D#6", 75},
	{"Eb6", 75},
	{"E6", 76},
	{"F6", 77},
	{"F#6", 78},
	{"Gb6", 78},
	{"G6", 79},
	{"G#6", 80},
	{"Ab6", 80},
	{"A6", 81},
	{"A#6", 82},
	{"Bb6", 82},
	{"B6", 83},
	{"C7", 84},
	{"C#7", 85},
	{"Db7", 85},
	{"D7", 86},
	{"D#7", 87},
	{"Eb7", 87},
	{"E7", 88},
	{"F7", 89},
	{"F#7", 90},
	{"Gb7", 90},
	{"G7", 91},
	{"G#7", 92},
	{"Ab7", 92},
	{"A7", 93},
	{"A#7", 94},
	{"Bb7", 94},
	{"B7", 95},
	{"C8", 96},
	{"C#8", 97},
	{"Db8", 97},
	{"D8", 98},
	{"D#8", 99},
	{"Eb8", 99},
	{"E8", 100},
	{"F8", 101},
	{"F#8", 102},
	{"Gb8", 102},
	{"G8", 103},
	{"G#8", 104},
	{"Ab8", 104},
	{"A8", 105},
	{"A#8", 106},
	{"Bb8", 106},
	{"B8", 107}
	};

	std::map<std::string, int>::iterator it = note_to_keynum.begin();


	std::map<int, std::string> keynum_to_note = {
		{0, "C0"},
		{1, "C#0/Db0"},
		{2, "D0"},
		{3, "D#0/Eb0"},
		{4, "E0"},
		{5, "F0"},
		{6, "F#0/Gb0"},
		{7, "G0"},
		{8, "G#0/Ab0"},
		{9, "A0"},
		{10, "A#0/Bb0"},
		{11, "B0"},
		{12, "C1"},
		{13, "C#1/Db1"},
		{14, "D1"},
		{15, "D#1/Eb1"},
		{16, "E1"},
		{17, "F1"},
		{18, "F#1/Gb1"},
		{19, "G1"},
		{20, "G#1/Ab1"},
		{21, "A1"},
		{22, "A#1/Bb1"},
		{23, "B1"},
		{24, "C2"},
		{25, "C#2/Db2"},
		{26, "D2"},
		{27, "D#2/Eb2"},
		{28, "E2"},
		{29, "F2"},
		{30, "F#2/Gb2"},
		{31, "G2"},
		{32, "G#2/Ab2"},
		{33, "A2"},
		{34, "A#2/Bb2"},
		{35, "B2"},
		{36, "C3"},
		{37, "C#3/Db3"},
		{38, "D3"},
		{39, "D#3/Eb3"},
		{40, "E3"},
		{41, "F3"},
		{42, "F#3/Gb3"},
		{43, "G3"},
		{44, "G#3/Ab3"},
		{45, "A3"},
		{46, "A#3/Bb3"},
		{47, "B3"},
		{48, "C4"},
		{49, "C#4/Db4"},
		{50, "D4"},
		{51, "D#4/Eb4"},
		{52, "E4"},
		{53, "F4"},
		{54, "F#4/Gb4"},
		{55, "G4"},
		{56, "G#4/Ab4"},
		{57, "A4"},
		{58, "A#4/Bb4"},
		{59, "B4"},
		{60, "C5"},
		{61, "C#5/Db5"},
		{62, "D5"},
		{63, "D#5/Eb5"},
		{64, "E5"},
		{65, "F5"},
		{66, "F#5/Gb5"},
		{67, "G5"},
		{68, "G#5/Ab5"},
		{69, "A5"},
		{70, "A#5/Bb5"},
		{71, "B5"},
		{72, "C6"},
		{73, "C#6/Db6"},
		{74, "D6"},
		{75, "D#6/Eb6"},
		{76, "E6"},
		{77, "F6"},
		{78, "F#6/Gb6"},
		{79, "G6"},
		{80, "G#6/Ab6"},
		{81, "A6"},
		{82, "A#6/Bb6"},
		{83, "B6"},
		{84, "C7"},
		{85, "C#7/Db7"},
		{86, "D7"},
		{87, "D#7/Eb7"},
		{88, "E7"},
		{89, "F7"},
		{90, "F#7/Gb7"},
		{91, "G7"},
		{92, "G#7/Ab7"},
		{93, "A7"},
		{94, "A#7/Bb7"},
		{95, "B7"},
		{96, "C8"},
		{97, "C#8/Db8"},
		{98, "D8"},
		{99, "D#8/Eb8"},
		{100, "E8"},
		{101, "F8"},
		{102, "F#8/Gb8"},
		{103, "G8"},
		{104, "G#8/Ab8"},
		{105, "A8"},
		{106, "A#8/Bb8"},
		{107, "B8"}
	};
	std::map<int, std::string>::iterator itb = keynum_to_note.begin();

	const mono SEMITONES_ET_12 = { 1,
							   1.059463,
							   1.122462,
							   1.189207,
							   1.259921,
							   1.33484,
							   1.414214,
							   1.498307,
							   1.587401,
							   1.681793,
							   1.781797,
							   1.887749
	};

	const double SEMITONE_ET_12 = 1.05946309436; //2^(1/12)


	//C3 is nominally 130.81 Hz
	class Keyboard {
	public:
		std::string initial_notename;
		int num_keys = 108;
		mono key_freqs;
		double semitone;
		int initial_keynum;
		double root_freq;



		Keyboard(const std::string init_note, double root, double st) {
			//idea: from the initial note, fill in the frequencies up and down based on the key maps
			initial_notename = init_note;
			initial_keynum = Keys::note_to_keynum[init_note];
			root_freq = root;
			semitone = st;
			calculate_frequencies();
		}

		void calculate_frequencies() {
			mono calc_freqs(108);
			calc_freqs[initial_keynum] = root_freq;
			// filling in going up from the note
			for (int i = initial_keynum + 1; i < num_keys; i++) {
				calc_freqs[i] = calc_freqs[i - 1] * semitone;
			}
			// filling in going down from the note
			for (int i = initial_keynum - 1; i >= 0; i--) {
				calc_freqs[i] = calc_freqs[i + 1] / semitone;
			}
			key_freqs = calc_freqs;
		}

		double note_to_freq(std::string string) {
			int this_keynum = Keys::note_to_keynum[string];
			return key_freqs[this_keynum];
		}

		mono notes_to_freqs(std::vector<std::string> notes) {
			mono freqs;
			for (size_t i = 0; i < notes.size(); i++){
				int this_keynum = Keys::note_to_keynum[notes[i]];
				freqs.push_back(key_freqs[this_keynum]);
			}
			return freqs;
		}
		
		void change_semitone(double new_st) {
			semitone = new_st;
			calculate_frequencies();
		}

		void skew_semitone(double skew) {
			semitone *= skew;
			calculate_frequencies();
		}

	};

}