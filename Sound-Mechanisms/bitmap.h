#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <numeric>
const extern double two_pi;
const extern int SAMPLE_RATE;
typedef std::vector<double> mono;

// idea is for drawing wavetables as 1 bit per pixel bitmaps

std::vector<char> read_bytes(char const* filename)
{
	std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	if (pos == 0) {
		return std::vector<char>{};
	}

	std::vector<char>  result(pos);

	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], pos);

	return result;
}

int32_t BE_bytes_to_int(std::vector<char> bytes) {
	return static_cast<int32_t>(
		(bytes[3]) |
		(bytes[2] << 8) |
		(bytes[1] << 16) |
		(bytes[0] << 24));
}

int32_t LE_bytes_to_int(std::vector<char> bytes) {
	return static_cast<int32_t>(
		(bytes[0]) |
		(bytes[1] << 8) |
		(bytes[2] << 16) |
		(bytes[3] << 24));
}

std::string LE_bytes_to_str(std::vector<char> bytes) {
	return std::bitset<32>(
		(bytes[0]) |
		(bytes[1] << 8) |
		(bytes[2] << 16) |
		(bytes[3] << 24)).to_string();
}

std::vector<char> read_some_bytes(std::vector<char> data, int offset, int num_bytes) {
	std::vector<char> out(num_bytes);
	for (int n = 0; n < num_bytes; n++) {
		//std::cout << "read byte, offset = " << offset << " , val= " << std::bitset<8>(data[offset + n]).to_string() << '\n';
		out[n] = data[offset + n];
	}
	return out;
}

struct Bitmap {
private:
	const int data_start_offset = 0XA;
	const int width_offset = 0x12;
	const int height_offset = 0x16;
public:

	signed long int width, height, data_offset;
	std::vector<std::string> bmp_bin_array = {};
	std::vector<char> data = {};
	std::vector<double> vert_vals = {};	// average height of pixels in each column, ranged to 0.0-1.0
	std::vector<double> quant_verts = {};

	Bitmap(char const* filename) {
		data = read_bytes(filename);
		std::vector<char> width_bytes = read_some_bytes(data, width_offset, 4);
		std::vector<char> height_bytes = read_some_bytes(data, height_offset, 4);
		std::vector<char> offset_bytes = read_some_bytes(data, data_start_offset, 4);
		width = LE_bytes_to_int(width_bytes);
		height = LE_bytes_to_int(height_bytes);
		data_offset = LE_bytes_to_int(offset_bytes);
	}

	void array_setup(bool ac = true) {
		make_array();
		correct_bg();
		calc_verticals(ac);
	}

	void make_array() {
		//idea: starting at data_offset, make rows that are as long as the image is wide
		// for each row, convert each byte to binary, then concatenate the binary
		std::vector<std::vector<char>> holding{};
		int bytes_in_row = width / 8;
		for (int32_t i = 0; i < height; i++) {
			std::string this_row = "";
			for (int n = 0; n < bytes_in_row; n++) {
				this_row += std::bitset<8>(data[data_offset + n]).to_string();
			}
			bmp_bin_array.push_back(this_row);
			data_offset += bytes_in_row;
		}
	}

	void correct_bg() {
		// expected input files show thin lines on a background that's mostly empty
		// therefore the value that shows up most in the matrix is the background.
		// bmp color tables may be referenced such that the bg color shows up most often.
		// to standardize this, make sure the 0s and 1s are set up such that 0 is the bg (most common)
		// and 1 is the fg (least common)

		int zeros = 0, ones = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (bmp_bin_array[i][j] == '0') {
					zeros += 1;
				}
				else {
					ones += 1;
				}
			}
		}

		//std::cout << "zeros, ones = " << zeros << ", " << ones << "\n";
		//if the matrix is already ok, exit the function
		if (zeros >= ones) { return; }

		//otherwise, go through the rows and replace the characters
		else {
			//std::cout << "inverting bits ";
			for (int i = 0; i < height; i++) {
				std::bitset<32> this_row(bmp_bin_array[i]);
				bmp_bin_array[i] = this_row.flip().to_string();
			}
		}
	}

	void calc_verticals(bool ac = true) {
		// for each column, check for a 1 on each row
		// if 1 => +1 to num_vert, +rowindex to sum_vert
		for (int i = 0; i < width; i++) {
			double this_num_vert = 0.0;
			double this_sum_vert = 0.0;
			for (int j = 0; j < height; j++) {
				if (bmp_bin_array[j][i] == '1') {
					this_num_vert += 1;
					this_sum_vert += j;
				}
			}
			double this_val = 0.0;
			if (this_num_vert > 0) {
				this_val = this_sum_vert / this_num_vert;
			}
			vert_vals.push_back(this_val);
		}

		if (ac) {
			//remove DC offset => find average value, subtract from all elements 
			double DC = std::reduce(vert_vals.begin(), vert_vals.end(), 0.0) / vert_vals.size();
			for (auto& element : vert_vals)
				element -= DC;
		}
		// after figuring out basic values, normalize
		double max_val = mono_abs_max(vert_vals);
		double reciporical = 1.0 / max_val;

		for (size_t i = 0; i < vert_vals.size(); i++) {
			vert_vals[i] = reciporical * vert_vals[i];
		}


		// ensure first and last samples are 0.0
		//vert_vals[0] = 0.0;
		//vert_vals[vert_vals.size() - 1] = 0.0;
	}

	double mono_abs_max(mono& vec) {
		double maxval = *max_element(vec.begin(), vec.end());
		double minval = abs(*min_element(vec.begin(), vec.end()));
		return minval > maxval ? minval : maxval;
	}

	coordinates get_coords(int scale = 1, double min_add = 0.0, double mult_by = 1.0) {
		coordinates x;
		for (size_t n = 0; n < vert_vals.size(); n++) {
			x.push_back(std::pair(scale * n, mult_by * (min_add + vert_vals[n])));
		}
		return x;
	}


	coordinates get_quant_coords(int scale = 1) {
		coordinates x;
		for (size_t n = 0; n < quant_verts.size(); n++) {
			x.push_back(std::pair(scale * n, quant_verts[n]));
		}
		return x;
	}

	void quantize(mono quants) {
		int num_q = static_cast<int>(quants.size());
		int h = height / num_q;
		//assuming array setup with ac=false s.t. vert_vals is all positive, 0-1.0
		// 0.0 is the bottom of the bottom bin, 1.0 is the top of the top bin
		// each bin is wavetable.height / num_q tall
		for (size_t n = 0; n < vert_vals.size(); n++) {
			// go through the vert_vals vector, figure out which vertical bin it should be in
			double this_val = vert_vals[n] * (height - 1);
			if (this_val == 0.0) { quant_verts.push_back(0.0); }

			for (int j = 0; j < num_q; j++) {
				if (this_val > (j * h) && this_val < ((j + 1) * h)) {
					quant_verts.push_back(quants[j]);
				}
			}
		}

	}

};