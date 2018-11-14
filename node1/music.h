#pragma once

#include <avr/pgmspace.h>

#define C8	4186.01

#define B7	3951.07
#define As7	3729.31
#define Bb7	3729.31
#define A7	3520
#define Gs7	3322.44
#define Ab7	3322.44
#define G7	3135.96
#define Fs7	2959.96
#define Gb7	2959.96
#define F7	2793.83
#define E7	2637.02
#define Ds7	2489.02
#define Eb7	2489.02
#define D7	2349.32
#define Cs7	2217.46
#define Db7	2217.46
#define C7	2093

#define B6	1975.53
#define As6	1864.66
#define Bb6	1864.66
#define A6	1760
#define Gs6	1661.22
#define Ab6	1661.22
#define G6	1567.98
#define Fs6	1479.98
#define Gb6	1479.98
#define F6	1396.91
#define E6	1318.51
#define Ds6	1244.51
#define Eb6	1244.51
#define D6	1174.66
#define Cs6	1108.73
#define Db6	1108.73
#define C6	1046.5

#define B5	987.767
#define As5	932.328
#define Bb5 932.328
#define A5	880
#define Gs5	830.609
#define Ab5	830.609
#define G5	783.991
#define Fs5	739.989
#define Gb5	739.989
#define F5	698.456
#define E5	659.255
#define Ds5	622.254
#define Eb5	622.254
#define D5	587.33
#define Cs5	554.365
#define Db5	554.365
#define C5	523.251

#define B4	493.883
#define As4	466.164
#define Bb4	466.164
#define A4	440
#define Gs4	415.305
#define Ab4	415.305
#define G4	391.995
#define Fs4	369.994
#define Gb4	369.994
#define F4	349.228
#define E4	329.628
#define Ds4	311.127
#define Eb4	311.127
#define D4	293.665
#define Cs4	277.183
#define Db4	277.183
#define C4	261.626

#define B3	246.942
#define As3	233.082
#define Bb3	233.082
#define A3	220
#define Gs3	207.652
#define Ab3	207.652
#define G3	195.998
#define Fs3	184.997
#define Gb3	184.997
#define F3	174.614
#define E3	164.814
#define Ds3	155.563
#define Eb3	155.563
#define D3	146.832
#define Cs3	138.591
#define Db3	138.591
#define C3	130.813

#define B2	123.471
#define As2	116.541
#define Bb2	116.541
#define A2	110
#define Gs2	103.826
#define Ab2	103.826
#define G2	97.9989
#define Fs2	92.4986
#define Gb2	92.4986
#define F2	87.4069
#define E2	82.4069
#define Ds2	77.7817
#define Eb2	77.7817
#define D2	73.4162
#define Cs2	69.2957
#define Db2	69.2957
#define C2	65.4064

#define B1	61.7354
#define As1	58.2705
#define Bb1	58.2705
#define A1	55
#define Gs1	51.9131
#define Ab1	51.9131
#define G1	48.9994
#define Fs1	46.2493
#define Gb1	46.2493
#define F1	43.6535
#define E1	41.2034
#define Ds1	38.8909
#define Eb1	38.8909
#define D1	36.7081
#define Cs1	34.6478
#define Db1	34.6478
#define C1	32.7032

#define B0	30.8677
#define As0	29.1352
#define Bb0	29.1352
#define A0	27.5

const float PROGMEM notes[5][105] = {
	//Song 0 - Happy birthday (134)
	{G4, G4, A4, G4, C5, B4, G4, G4, A4, G4, D5, C5, G4, G4, G5, E5, C5, B4, A4, F5, F5, E5, C5, D5, C5},
	//Song 1 - Zelda: Mabe Village (60)
	{D4,G3,A3,B3,C4,D4,D4,G3,G3,A3,B3,C4,B3,D4,A4,G4,A4,G4,
		D4,C4,B3,A3,0,B3,C4,D4,E4,G3,Fs3,A3,E4,D4,D4,Fs3,Fs3,G3,G3,Fs3,
	E3,G3,B3,A3,G3,Eb3,D3,D4,D3,D4,D3,D4,D3,D4,D4,D5,D4,D5,D4,D5,D4,D5},
	//Song 2 - Mario: World 1 (61)
	{G3,C3,G3,A3,C3,A3,B3,C3,B3,A3,0,Ab3,A3,G3,C3,G3,A3,C3,A3,B3,0,
		C4,F3,C4,D4,F3,D4,E4,F3,E4,D4,0,Db4,D4,C4,F3,C4,D4,F3,D4,E4,0,
	G3,C3,G3,A3,C3,A3,B3,C3,B3,A3,0,Ab3,A3,C4,C4,0,C4,C4,0},
	//Song 3 - Korobeiniki (46)
	{
		E5, B4, C5, D5, C5, B4, A4, A4, C5, E5, D5, C5, B4, B4, C5, D5, E5, C5, A4, A4, A4, B4, C5,
		D5, D5, F5, A5, G5, F5, E5, E5, C5, E5, D5, C5, B4, B4, C5, D5, E5, C5, A4, A4, 0,
		
		E5, B4, C5, D5, C5, B4, A4, A4, C5, E5, D5, C5, B4, B4, C5, D5, E5, C5, A4, A4, A4, B4, C5,
		D5, D5, F5, A5, G5, F5, E5, E5, C5, E5, D5, C5, B4, B4, C5, D5, E5, C5, A4, A4, 0,
		
		E5, C5, D5, B4, C5, A4, Gs4, B4, 0, E5, C5, D5, B4, C5, E5, A5, Gs5, 0,
		
		E5, B4, C5, D5, C5, B4, A4, A4, C5, E5, D5, C5, B4, B4, C5, D5, E5, C5, A4, A4, A4, B4, C5,
		D5, D5, F5, A5, G5, F5, E5, E5, C5, E5, D5, C5, B4, B4, C5, D5, E5, C5, A4, A4, 0
	},
	//Song 4 - Imperial March (14)
	{A4, A4, A4, F4, F4, C5, A4, F4, F4, C5, A4, //Takt 1-2
		
	 E5, E5, E5, F5, F5, C5, Ab4, F4, F4, C5, A4,	//Takt 3-4
		
		A5, A4, A4, A4, A5, Ab5, Ab5, G5, Gb5, F5, Gb5, 0, Bb4, Eb5, D5, D5, Db5, // Takt 5-6
		
		C5, B4, C5, 0, F4, Ab4, F4, F4, A4, C5, A4, A4, C5, E5, // Takt 7-8
		
		A5, A4, A4, A4, A5, Ab5, Ab5, G5, Gb5, F5, Gb5, 0, Bb4, Eb5, D5, D5, Db5, // Takt 9-10
		
		C5, B4, C5, 0, F4, Ab4, F4, F4, A4, A4, F4, F4, C5, A4 // Takt 7-8
	}
};

const uint8_t PROGMEM notes_lengths[5][150] = {
	//Song 0 - Zelda: Fairy Fountain (134)
	{3, 3, 2, 2, 2, 1, 3, 3, 2, 2, 2, 1, 3, 3, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 1},
	//Song 1 - Zelda: Mabe Village (60)
	{2,2,7,4,4,3,3,3,3,7,4,4,3,3,7,3,3,3,
		3,4,4,2,6,4,4,4,2,2,2,3,3,3,3,3,3,7,4,4,
	3,3,7,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	//Song 2 - Mario: World 1 (61)
	{6,4,2,6,4,2,6,4,6,4,6,4,2,6,4,2,6,4,6,4,0,
		6,4,2,6,4,2,6,4,6,4,6,4,2,6,4,2,6,4,6,4,0,
	6,4,2,6,4,2,6,4,6,4,6,4,2,6,4,1,6,4,0},
	//Song 3 - Mario: Athletic (46)
	{
		2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 2, 2, 3, 3, 3, 3,
		2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 2, 2, 2, 2,
		
		2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 2, 2, 3, 3, 3, 3,
		2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 2, 2, 2, 2,

		1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1,
		
		2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 2, 2, 3, 3, 3, 3,
		2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 3, 3, 2, 2, 2, 2, 2, 2,
	},
	//Song 4 - Game Over (14)
	{2,  2,  2,  3,  4,  4,  2,  3,  4,  4,  1,	//Takt 1-2
		
	 2,  2,  2,  3,  4,  4,  2,  3,  4,  4,  1,	//Takt 3-4
		
	 2,  3,  4,  4,  2,  3,  4,  4,  4,  4,  3, 3,  3,  2,  3,  4,  4, // Takt 5-6
		
	 4, 4, 3, 3, 3, 2, 3, 4, 4, 2, 3, 4, 4, 1, // Takt 7-8
		
	 2,  3,  4,  4,  2,  3,  4,  4,  4,  4,  3, 3,  3,  2,  3,  4,  4, // Takt 9-10
		
	 4, 4, 3, 3, 3, 2, 3, 4, 4, 2, 3, 4, 4, 1, // Takt 11-12
	}

};