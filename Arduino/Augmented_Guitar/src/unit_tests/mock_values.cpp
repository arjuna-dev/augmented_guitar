#include <cstdlib>
#include "mock_values.h"

// GuitarStrings
const int string_input_pins_mock[NUM_OF_STRINGS] = {34, 35, 36, 37, 38, 39};
const char open_string_notes_mock[NUM_OF_STRINGS] = {'E', 'A', 'D', 'G', 'B', 'e'};
const int max_amplitudes_mock[NUM_OF_STRINGS] = {540, 510, 560, 580, 620, 600};
const int min_thresholds_mock[NUM_OF_STRINGS] = {15, 18, 20, 20, 15, 23};
const int max_wave_periods_mock[NUM_OF_STRINGS] = {15, 10, 8, 6, 4, 3};


int mock_mock_array[1] = {0};

// Left hand
  // TeensyTouch
int mux_pins_mock[NUM_OF_MUX_PINS] = {mux_1_pin, mux_2_pin};
int* ptr_mux_pins_mock = mux_pins_mock;
int mux_ch_mock = 0;
int* ptr_mux_ch_mock = &mux_ch_mock;
int analog_values_mock[NUM_OF_NOTES] = {0};
int* ptr_analog_values_mock = analog_values_mock;
int reference_analog_values_mock[NUM_OF_NOTES] = {0};
int* ptr_reference_analog_values_mock = reference_analog_values_mock;
void selectMuxChannelMock(int mux_ch) {}
int* ptr_mock_TSI_values = mock_mock_array;

// Right hand
int* ptr_mock_amplitude_values = mock_mock_array;
int analogReadMock(){
  int analog_value = *ptr_mock_amplitude_values;
  ptr_mock_amplitude_values++;
  return analog_value;
};

const int amplitude_mock_values_e_empty[1000] = {0};

const int amplitude_mock_values_e[1000] = {158, 190, 221, 245, 260, 272, 280, 287, 293, 301, 307, 311, 309, 303, 300, 295, 292, 290, 289, 288, 286, 280, 274, 273, 272, 269, 290, 333, 399, 466, 483, 483, 483, 482, 482, 482, 482, 481, 481, 481, 481, 480, 480, 480, 479, 477, 231, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 51, 74, 93, 110, 125, 141, 158, 173, 187, 195, 201, 202, 201, 205, 206, 209, 212, 217, 219, 217, 217, 223, 226, 239, 259, 304, 376, 453, 501, 501, 501, 501, 501, 501, 500, 500, 500, 499, 499, 499, 499, 498, 496, 493, 146, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 38, 61, 81, 93, 104, 118, 136, 154, 171, 183, 189, 191, 195, 196, 200, 205, 209, 215, 217, 214, 217, 222, 227, 243, 275, 321, 398, 475, 519, 519, 518, 518, 519, 518, 518, 518, 517, 517, 517, 517, 515, 513, 511, 509, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 14, 46, 73, 91, 105, 118, 130, 143, 157, 172, 184, 191, 195, 199, 202, 206, 213, 219, 228, 228, 229, 233, 240, 245, 260, 293, 347, 429, 500, 535, 535, 535, 534, 534, 534, 534, 534, 533, 533, 533, 533, 531, 528, 526, 525, 54, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 26, 59, 85, 103, 116, 128, 142, 157, 169, 179, 187, 193, 200, 206, 212, 217, 223, 232, 240, 239, 241, 247, 253, 260, 278, 312, 368, 451, 523, 550, 550, 550, 550, 550, 548, 548, 548, 548, 547, 548, 548, 547, 545, 542, 541, 17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 17, 53, 82, 106, 123, 135, 146, 161, 176, 190, 202, 209, 211, 213, 213, 218, 222, 234, 242, 247, 250, 257, 263, 268, 278, 297, 334, 395, 480, 551, 564, 564, 564, 564, 564, 563, 563, 562, 562, 561, 562, 561, 561, 559, 557, 555, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 29, 64, 90, 113, 126, 139, 151, 167, 181, 198, 209, 216, 223, 228, 230, 232, 234, 241, 257, 260, 263, 270, 273, 280, 293, 313, 353, 420, 505, 574, 578, 578, 577, 578, 577, 576, 576, 576, 576, 575, 575, 574, 572, 574, 569, 569, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 45, 77, 107, 125, 141, 153, 163, 178, 194, 209, 222, 228, 232, 235, 241, 243, 247, 253, 257, 270, 275, 283, 288, 297, 309, 332, 373, 442, 524, 589, 590, 590, 590, 590, 589, 589, 589, 588, 588, 588, 588, 588, 586, 586, 582, 581, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 17, 58, 92, 117, 136, 149, 159, 172, 186, 201, 218, 228, 237, 244, 249, 254, 256, 261, 269, 277, 278, 282, 291, 300, 308, 323, 350, 396, 465, 547, 602, 602, 602, 602, 602, 601, 601, 601, 600, 600, 599, 600, 599, 598, 595, 593, 580, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 41, 75, 110, 135, 152, 164, 174, 186, 198, 213, 229, 236, 245, 249, 256, 260, 265, 271, 283, 288, 294, 301, 302, 310, 326, 333, 366, 415, 488, 562, 612, 613, 613, 612, 612, 612, 612, 611, 611, 610, 610, 610, 609, 607, 605, 603, 477, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 40, 87, 108, 137, 154, 167, 179, 188, 202, 219, 232, 242, 250, 254, 261, 263, 268, 277, 288, 293, 302, 309, 317, 322, 328, 355, 373, 431, 511, 579, 623, 621, 623, 622, 622, 621, 622, 621, 621, 620, 620, 620, 619, 617, 615, 613, 390, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 21, 59, 92, 124, 139, 154, 167, 182, 191, 208, 224, 235, 248, 255, 260, 267, 269, 273, 284, 294, 303, 311, 319, 331, 338, 349, 361, 398, 448, 524, 590, 632, 632, 632, 632, 632, 631, 631, 630, 630, 630, 630, 629, 628, 626, 623, 623, 325, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 34, 75, 108, 133, 153, 166, 171, 181, 190, 209, 227, 243, 254, 259, 267, 272, 275, 281, 293, 300, 309, 317, 327, 340, 351, 364, 381, 414, 475, 553, 608, 640, 640, 640, 640, 640, 640, 640, 639, 639, 639, 638, 638, 637, 635, 632, 631, 275, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 52, 92, 123, 149, 166, 178, 190, 193, 201, 214, 227, 241, 253, 263, 271, 275, 282, 290, 301, 310, 320, 327, 338, 349, 360, 373, 392, 430, 493, 570, 627, 649, 649, 648, 648, 646, 647, 648, 648, 647, 647, 647, 646, 644, 643, 640, 639, 215, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 11, 56, 95, 128, 152, 170, 184, 190};

const int amplitude_mock_values_e_note_off[1000] = {158, 190, 221, 245, 260, 272, 280, 287, 293, 301, 307, 311, 309, 303, 300, 295, 292, 290, 289, 288, 286, 280, 274, 273, 272, 269, 290, 333, 399, 466, 483, 483, 483, 482, 482, 482, 482, 481, 481, 481, 481, 480, 480, 480, 479, 477, 231, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 51, 74, 93, 110, 125, 141, 158, 173, 187, 195, 201, 202, 201, 205, 206, 209, 212, 217, 219, 217, 217, 223, 226, 239, 259, 304, 376, 453, 501, 501, 501, 501, 501, 501, 500, 500, 500, 499, 499, 499, 499, 498, 496, 493, 146, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 38, 61, 81, 93, 104, 118, 136, 154, 171, 183, 189, 191, 195, 196, 200, 205, 209, 215, 217, 214, 217, 222, 227, 243, 275, 321, 398, 475, 519, 519, 518, 518, 519, 518, 518, 518, 517, 517, 517, 517, 515, 513, 511, 509, 103, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 14, 46, 73, 91, 105, 118, 130, 143, 157, 172, 184, 191, 195, 199, 202, 206, 213, 219, 228, 228, 229, 233, 240, 245, 260, 293, 347, 429, 500, 535, 535, 535, 534, 534, 534, 534, 534, 533, 533, 533, 533, 531, 528, 526, 525, 54, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 26, 59, 85, 103, 116, 128, 142, 157, 169, 179, 187, 193, 200, 206, 212, 217, 223, 232, 240, 239, 241, 247, 253, 260, 278, 312, 368, 451, 523, 550, 550, 550, 550, 550, 548, 548, 548, 548, 547, 548, 548, 547, 545, 542, 541, 17, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 17, 53, 82, 106, 123, 135, 146, 161, 176, 190, 202, 209, 211, 213, 213, 218, 222, 234, 242, 247, 250, 257, 263, 268, 278, 297, 334, 395, 480, 551, 564, 564, 564, 564, 564, 563, 563, 562, 562, 561, 562, 561, 561, 559, 557, 555, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 29, 64, 90, 113, 126, 139, 151, 167, 181, 198, 209, 216, 223, 228, 230, 232, 234, 241, 257, 260, 263, 270, 273, 280, 293, 313, 353, 420, 505, 574, 578, 578, 577, 578, 577, 576, 576, 576, 576, 575, 575, 574, 572, 574, 569, 569, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};