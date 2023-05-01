#include "sound.h"
#include "keyboard.h"
#include <string>
#include <regex>
#include <map>
const int sample_rate = 48000;

std::random_device rdv;
std::mt19937 mt(rdv());

std::uniform_int_distribution<int> harmonics(3, 25);
std::uniform_real_distribution<double> pct(0, 1);
std::uniform_real_distribution<double> freq_range(10, 40);
std::uniform_real_distribution<double> d_flip(-1, 1);

std::gamma_distribution<double> gamma(2, 1);

mono fixed_ratio_list(int num, double ratio)
{
    mono list;
    for (int i = 0; i < num; i++)
    {
        list.push_back(pow(ratio, i));
    }
    return list;
}

class NoteName
{
public:
    std::string note_letter;
    std::string note_mods;
    int note_number;

    NoteName()
    {
        note_letter = "C";
        note_mods = "";
        note_number = 3;
    }

    NoteName(const std::string a, int b)
    {
        split_note_letter(a);
        split_note_mods(a);
        note_number = b;
    }

    NoteName(const std::string a)
    {
        separate_string(a);
    }

private:
    void separate_string(const std::string string)
    {
        // string is of format C#3, F0, Bb2, etc

        split_note_letter(string);
        split_note_mods(string);
        split_note_number(string);
    }

    void split_note_letter(const std::string string)
    {
        // this gets the letter: C, F, B, etc
        std::regex rgx("[A-G]");
        std::smatch matches;

        if (std::regex_search(string, matches, rgx))
        {
            note_letter = matches[0];
        }
        else
        {
            std::cout << "ERROR: No valid note letter in: " << string << std::endl;
        }
    }
    void split_note_number(const std::string string)
    {
        // this gets the number: 3, 0, 2, etc.
        std::regex rgx("[0-9]+");
        std::smatch matches;

        if (std::regex_search(string, matches, rgx))
        {
            note_number = std::stoi(matches[0]);
        }
        else
        {
            std::cout << "ERROR: No valid note number in: " << string << std::endl;
        }
    }

    void split_note_mods(const std::string string)
    {
        // this get the accidentals: #, '', b, etc
        std::regex rgx("[b#]");
        std::smatch matches;

        if (std::regex_search(string, matches, rgx))
        {
            note_mods = matches[0];
        }
        else
        {
            note_mods = "";
        }
    }
};

int main()
{
    Keys::Keyboard test_keyboard("C3", 130.8128, Keys::SEMITONE_ET_12);

    typedef std::vector<std::vector<std::string>> progression;

    progression cg_a_bottom = {{"C3", "E3", "G3", "C4", "E4", "G4"},
                               {"C3", "E3", "G3", "C4", "E4", "G4"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"C3", "E3", "G3", "C4", "E4", "G4"},
                               {"C3", "E3", "G3", "C4", "E4", "G4"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"E3", "G#3", "B3", "E4", "G#4", "B4"},
                               {"E3", "G#3", "B3", "E4", "G#4", "B4"},
                               {"E3", "G#3", "C4", "E4", "G#4", "C5"},
                               {"E3", "G#3", "C4", "E4", "G#4", "C#5"},
                               {"F3", "A3", "C4", "F4", "A4", "C5"},
                               {"F3", "A3", "C4", "F4", "A4", "C5"},
                               {"F3", "A3", "C#4", "F4", "A4", "C#5"},
                               {"F3", "A3", "D4", "F4", "A4", "D5"},
                               {"F3", "G#3", "C4", "F4", "G#4", "C5"},
                               {"F3", "G#3", "C4", "F4", "G#4", "C5"},
                               {"F3", "G#3", "C#4", "F4", "G#4", "C#5"},
                               {"F3", "G#3", "D4", "F4", "G#4", "D5"},
                               {"E3", "G#3", "B3", "E4", "G#4", "B4"},
                               {"E3", "G#3", "B3", "E4", "G#4", "B4"},
                               {"E3", "G#3", "C4", "E4", "G#4", "C5"},
                               {"E3", "G#3", "C4", "E4", "G#4", "C#5"},
                               {"F3", "A3", "C4", "F4", "A4", "C5"},
                               {"F3", "A3", "C4", "F4", "A4", "C5"},
                               {"F3", "A3", "C#4", "F4", "A4", "C#5"},
                               {"F3", "A3", "D4", "F4", "A4", "D5"},
                               {"F3", "G#3", "C4", "F4", "G#4", "C5"},
                               {"F3", "G#3", "C4", "F4", "G#4", "C5"},
                               {"F3", "G#3", "C#4", "F4", "G#4", "C#5"},
                               {"F3", "G#3", "D4", "F4", "G#4", "D5"},
                               {"E3", "G#3", "B3", "E4", "G#4", "B4"},
                               {"E3", "G#3", "B3", "E4", "G#4", "B4"},
                               {"E3", "G#3", "C4", "E4", "G#4", "C5"},
                               {"E3", "G#3", "C4", "E4", "G#4", "C#5"},
                               {"C3", "E3", "G3", "G4", "C5", "E5"},
                               {"C3", "E3", "G3", "G4", "C5", "E5"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"C3", "E3", "G3", "G4", "C5", "E5"},
                               {"C3", "E3", "G3", "G4", "C5", "E5"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"F3", "G#3", "B3", "F4", "G#4", "B4"},
                               {"C3", "F3", "G3", "C4", "F4", "G4"},
                               {"C3", "F3", "G3", "C4", "F4", "G4"},
                               {"C3", "E3", "G3", "C4", "E4", "G4"},
                               {"C3", "E3", "G3", "C4", "E4", "G4"},
                               // B section
                               {"C3", "E3", "G3", "B3", "C4", "E4"},
                               {"C3", "E3", "G3", "B3", "C4", "E4"},
                               {"C3", "F3", "G#3", "C4", "C#4", "G4"},
                               {"C3", "F3", "G#3", "C4", "C#4", "G4"},
                               {"C3", "E3", "G3", "B3", "C4", "E4"},
                               {"C3", "E3", "G3", "B3", "C4", "E4"},
                               {"C3", "F3", "G#3", "C4", "C#4", "G4"},
                               {"C3", "F3", "G#3", "C4", "C#4", "G4"},
                               {"D#3", "G3", "A#3", "C4", "D#4", "F4"},
                               {"D#3", "G3", "A#3", "C4", "D#4", "F4"},
                               {"C#3", "G3", "A#3", "C4", "D#4", "G#4"},
                               {"C#3", "G3", "A#3", "C4", "D#4", "G4"},
                               {"C3", "D#3", "G#3", "D#4", "C5"},
                               {"C3", "D#3", "G#3", "D#4", "C5"},
                               {"G3", "A#3", "C4", "C#4", "F4"},
                               {"G3", "A#3", "C4", "C#4", "F4"},
                               {"G3", "A#3", "C4", "C#4", "G4"},
                               {"G3", "A#3", "C4", "C#4", "D#4"},
                               {"C3", "D#3", "G#3", "D#4", "C5"},
                               {"C3", "D#3", "G#3", "D#4", "C5"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C3", "D#3", "G3", "C4", "D#4", "G4"},
                               {"C3", "D#3", "G3", "C4", "D#4", "G4"},
                               {"C3", "E3", "G#3", "C4", "E4", "G#4"},
                               {"C3", "E3", "G#3", "C4", "E4", "A#4"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C#3", "F3", "G#3", "C4", "D#4", "F4"},
                               {"C3", "D#3", "G3", "C4", "D#4", "G4"},
                               {"C3", "D#3", "G3", "C4", "D#4", "G4"},
                               {"C3", "E3", "G#3", "C4", "E4", "G#4"},
                               {"C3", "E3", "G#3", "C4", "E4", "A#4"},
                               {"C#3", "F3", "G#3", "C#4", "F4", "G#4"},
                               {"C#3", "F3", "G#3", "C#4", "F4", "G#4"}};

    progression cg_a_top = {{"C4", "E4", "G4", "C5", "E5", "G5"},
                            {"C4", "E4", "G4", "C5", "E5", "G5"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"C4", "E4", "G4", "C5", "E5", "G5"},
                            {"C4", "E4", "G4", "C5", "E5", "G5"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"E4", "G#4", "B4", "E5", "G#5", "B5"},
                            {"E4", "G#4", "B4", "E5", "G#5", "B5"},
                            {"E4", "G#4", "C5", "E5", "G#5", "C6"},
                            {"E4", "G#4", "C#5", "E5", "G#5", "C#6"},
                            {"F4", "A4", "C5", "F5", "A5", "C6"},
                            {"F4", "A4", "C5", "F5", "A5", "C6"},
                            {"F4", "A4", "C#5", "F5", "A5", "C#6"},
                            {"F4", "A4", "D5", "F5", "A5", "D6"},
                            {"F4", "G#4", "C5", "F5", "G#5", "C6"},
                            {"F4", "G#4", "C5", "F5", "G#5", "C6"},
                            {"F4", "G#4", "C#5", "F5", "G#5", "C#6"},
                            {"F4", "G#4", "D5", "F5", "G#5", "D6"},
                            {"E4", "G#4", "B4", "E5", "G#5", "B5"},
                            {"E4", "G#4", "B4", "E5", "G#5", "B5"},
                            {"E4", "G#4", "C5", "E5", "G#5", "C6"},
                            {"E4", "G#4", "C#5", "E5", "G#5", "C#6"},
                            {"F4", "A4", "C5", "F5", "A5", "C6"},
                            {"F4", "A4", "C5", "F5", "A5", "C6"},
                            {"F4", "A4", "C#5", "F5", "A5", "C#6"},
                            {"F4", "A4", "D5", "F5", "A5", "D6"},
                            {"F4", "G#4", "C5", "F5", "G#5", "C6"},
                            {"F4", "G#4", "C5", "F5", "G#5", "C6"},
                            {"F4", "G#4", "C#5", "F5", "G#5", "C#6"},
                            {"F4", "G#4", "D5", "F5", "G#5", "D6"},
                            {"E4", "G#4", "B4", "E5", "G#5", "B5"},
                            {"E4", "G#4", "B4", "E5", "G#5", "B5"},
                            {"E4", "G#4", "C5", "E5", "G#5", "C6"},
                            {"E4", "G#4", "C#5", "E5", "G#5", "C#6"},
                            {"C4", "E4", "G4", "G5", "C6", "E6"},
                            {"C4", "E4", "G4", "G5", "C6", "E6"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"C4", "E4", "G4", "G5", "C6", "E6"},
                            {"C4", "E4", "G4", "G5", "C6", "E6"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"F4", "G#4", "B4", "F5", "G#5", "B5"},
                            {"C4", "F4", "G4", "C5", "F5", "G5"},
                            {"C4", "F4", "G4", "C5", "F5", "G5"},
                            {"C4", "E4", "G4", "C5", "E5", "G5"},
                            {"C4", "E4", "G4", "C5", "E5", "G5"},
                            // B section
                            {"C4", "E4", "G4", "B4", "C5", "E5"},
                            {"C4", "E4", "G4", "B4", "C5", "E5"},
                            {"C4", "F4", "G#4", "C5", "C#5", "G5"},
                            {"C4", "F4", "G#4", "C5", "C#5", "G5"},
                            {"C4", "E4", "G4", "B4", "C5", "E5"},
                            {"C4", "E4", "G4", "B4", "C5", "E5"},
                            {"C4", "F4", "G#4", "C5", "C#5", "G5"},
                            {"C4", "F4", "G#4", "C5", "C#5", "G5"},
                            {"D#4", "G#4", "A#4", "C5", "D#5", "F5"},
                            {"D#4", "G#4", "A#4", "C5", "D#5", "F5"},
                            {"C#4", "G4", "A#4", "C5", "D#5", "G#5"},
                            {"C#4", "G4", "A#4", "C5", "D#5", "G5"},
                            {"G#3", "C4", "D#4", "G#4", "D#5", "C6"},
                            {"G#3", "C4", "D#4", "G#4", "D#5", "C6"},
                            {"D#3", "G4", "A#4", "C5", "C#5", "F5"},
                            {"D#3", "G4", "A#4", "C5", "C#5", "F5"},
                            {"D#3", "G4", "A#4", "C5", "C#5", "G5"},
                            {"D#3", "G4", "A#4", "C5", "C#5", "D#5"},
                            {"G#3", "C4", "D#4", "G#4", "D#5", "C6"},
                            {"G#3", "C4", "D#4", "G#4", "D#5", "C6"},
                            {"C#4", "F4", "G#4", "C5", "D#5", "F5"},
                            {"C#4", "F4", "G#4", "C5", "D#5", "F5"},
                            {"C4", "D#4", "G4", "C5", "D#5", "G5"},
                            {"C4", "D#4", "G4", "C5", "D#5", "G5"},
                            {"C4", "E4", "G#4", "C5", "E5", "G#5"},
                            {"C4", "E4", "G#4", "C5", "E5", "A#5"},
                            {"C#4", "F4", "G#4", "C5", "D#5", "F5"},
                            {"C#4", "F4", "G#4", "C5", "D#5", "F5"},
                            {"C#4", "F4", "G#4", "C5", "D#5", "F5"},
                            {"C#4", "F4", "G#4", "C5", "D#5", "F5"},
                            {"C4", "D#4", "G4", "C5", "D#5", "G5"},
                            {"C4", "D#4", "G4", "C5", "D#5", "G5"},
                            {"C4", "E4", "G#4", "C5", "E5", "G#5"},
                            {"C4", "E4", "G#4", "C5", "E5", "A#5"},
                            {"C#4", "F4", "G#4", "C#5", "F5", "G#5"},
                            {"C#4", "F4", "G#4", "C#5", "F5", "G#5"}};

    mono sts = {Keys::SEMITONE_ET_12, Keys::SEMITONE_ET_12 * 1.001};
    std::uniform_int_distribution<std::size_t> distribution(0, sts.size() - 1);
    stereo composition_space = Sound::empty_sound();


 

    double chord_duration = 1;
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

            for (size_t k = 0; k < these_freqs.size(); k++) {
                // for each frequency in the chord, make an arb and stick them all on top of each other
                // (note: for chords with interval structures on each frequency, add another loop over the intervals)
                Sound::Contour freq_line, env_line, pan_line;
                mono phs;
                double ramp_time = (chord_duration * (pct(rdv) + 0.02) / 5.0);
                double silence_time = ramp_time * (pct(rdv) + 0.02) / 5.0;
                double top_time = chord_duration - 2.0 * ramp_time;

                freq_line.add_flat_line(chord_duration, these_freqs[k]);
                env_line.trapezoid(0, ramp_time, pct(rdv), top_time, ramp_time, 0);
                pan_line.add_flat_line(chord_duration, Sound::d_flip(Sound::rd));
                phs.push_back(pct(rdv));


                Sound::Arb this_arb({ freq_line }, { pan_line }, { env_line }, { phs });

                Sound::add_sounds(composition_space, this_arb.sound, offset);
                std::cout << ".";
            }
            //after sticking the chord together on top of itself, move the offset over
            offset += (chord_duration - 0.1) * sample_rate;
            std::cout << "-";
        }
        std::cout << "=";
    }



    // Normalize the composition audio and write it to a wav file
    Sound::stereo_normalize(composition_space);
    std::string filename = "canada_goose_alpha_cleanedb.wav";
    //Wav::write_stereo_wav(filename, composition_space, 1);
    Wav::write_stereo_wav_fixed(filename, composition_space, 1);

    return 0;
}
