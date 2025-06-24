#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>

#define MAX_TEAMS 20
#define MAX_WEEKS 52

const char* header= "Date dd/mm/yyyy,Time HH:MM,Division,Home Team,Away Team,Venue,Pitch,Home Score,Away Score";

char division[PATH_MAX];
char starttime[PATH_MAX] = "20:00";
char teams[MAX_TEAMS][PATH_MAX];
char shortcodes[MAX_TEAMS][PATH_MAX];
char venues[MAX_TEAMS][PATH_MAX];
int lastteam = 0;
char weeks[MAX_WEEKS][PATH_MAX];
int lastweek = 0;

// ~/ownCloud/GDPL_Private/assets/Calendar/doc/IanBalancedRoundRobin_v5_1_pjh_6-24.xlsm

int round_robin_24[24][23] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16, -17,  18, -19,  20, -21,  22, -23,  24},
	{  -1,   4,  -6,   8, -10,  12, -14,  16, -18,  20, -22,  24,  -3,   5,  -7,   9, -11,  13, -15,  17, -19,  21, -23},
	{ -24,   1,   5,  -7,   9, -11,  13, -15,  17, -19,  21, -23,   2,  -4,   6,  -8,  10, -12,  14, -16,  18, -20,  22},
	{  23,  -2,  -1,   6,  -8,  10, -12,  14, -16,  18, -20,  22, -24,   3,  -5,   7,  -9,  11, -13,  15, -17,  19, -21},
	{ -22,  24,  -3,   1,   7,  -9,  11, -13,  15, -17,  19, -21,  23,  -2,   4,  -6,   8, -10,  12, -14,  16, -18,  20},
	{  21, -23,   2,  -4,  -1,   8, -10,  12, -14,  16, -18,  20, -22,  24,  -3,   5,  -7,   9, -11,  13, -15,  17, -19},
	{ -20,  22, -24,   3,  -5,   1,   9, -11,  13, -15,  17, -19,  21, -23,   2,  -4,   6,  -8,  10, -12,  14, -16,  18},
	{  19, -21,  23,  -2,   4,  -6,  -1,  10, -12,  14, -16,  18, -20,  22, -24,   3,  -5,   7,  -9,  11, -13,  15, -17},
	{ -18,  20, -22,  24,  -3,   5,  -7,   1,  11, -13,  15, -17,  19, -21,  23,  -2,   4,  -6,   8, -10,  12, -14,  16},
	{  17, -19,  21, -23,   2,  -4,   6,  -8,  -1,  12, -14,  16, -18,  20, -22,  24,  -3,   5,  -7,   9, -11,  13, -15},
	{ -16,  18, -20,  22, -24,   3,  -5,   7,  -9,   1,  13, -15,  17, -19,  21, -23,   2,  -4,   6,  -8,  10, -12,  14},
	{  15, -17,  19, -21,  23,  -2,   4,  -6,   8, -10,  -1,  14, -16,  18, -20,  22, -24,   3,  -5,   7,  -9,  11, -13},
	{ -14,  16, -18,  20, -22,  24,  -3,   5,  -7,   9, -11,   1,  15, -17,  19, -21,  23,  -2,   4,  -6,   8, -10,  12},
	{  13, -15,  17, -19,  21, -23,   2,  -4,   6,  -8,  10, -12,  -1,  16, -18,  20, -22,  24,  -3,   5,  -7,   9, -11},
	{ -12,  14, -16,  18, -20,  22, -24,   3,  -5,   7,  -9,  11, -13,   1,  17, -19,  21, -23,   2,  -4,   6,  -8,  10},
	{  11, -13,  15, -17,  19, -21,  23,  -2,   4,  -6,   8, -10,  12, -14,  -1,  18, -20,  22, -24,   3,  -5,   7,  -9},
	{ -10,  12, -14,  16, -18,  20, -22,  24,  -3,   5,  -7,   9, -11,  13, -15,   1,  19, -21,  23,  -2,   4,  -6,   8},
	{   9, -11,  13, -15,  17, -19,  21, -23,   2,  -4,   6,  -8,  10, -12,  14, -16,  -1,  20, -22,  24,  -3,   5,  -7},
	{  -8,  10, -12,  14, -16,  18, -20,  22, -24,   3,  -5,   7,  -9,  11, -13,  15, -17,   1,  21, -23,   2,  -4,   6},
	{   7,  -9,  11, -13,  15, -17,  19, -21,  23,  -2,   4,  -6,   8, -10,  12, -14,  16, -18,  -1,  22, -24,   3,  -5},
	{  -6,   8, -10,  12, -14,  16, -18,  20, -22,  24,  -3,   5,  -7,   9, -11,  13, -15,  17, -19,   1,  23,  -2,   4},
	{   5,  -7,   9, -11,  13, -15,  17, -19,  21, -23,   2,  -4,   6,  -8,  10, -12,  14, -16,  18, -20,  -1,  24,  -3},
	{  -4,   6,  -8,  10, -12,  14, -16,  18, -20,  22, -24,   3,  -5,   7,  -9,  11, -13,  15, -17,  19, -21,   1,   2},
	{   3,  -5,   7,  -9,  11, -13,  15, -17,  19, -21,  23,  -2,   4,  -6,   8, -10,  12, -14,  16, -18,  20, -22,  -1},
};

int round_robin_22[22][21] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16, -17,  18, -19,  20, -21,  22},
	{  -1,   4,  -6,   8, -10,  12, -14,  16, -18,  20, -22,   3,  -5,   7,  -9,  11, -13,  15, -17,  19, -21},
	{ -22,   1,   5,  -7,   9, -11,  13, -15,  17, -19,  21,  -2,   4,  -6,   8, -10,  12, -14,  16, -18,  20},
	{  21,  -2,  -1,   6,  -8,  10, -12,  14, -16,  18, -20,  22,  -3,   5,  -7,   9, -11,  13, -15,  17, -19},
	{ -20,  22,  -3,   1,   7,  -9,  11, -13,  15, -17,  19, -21,   2,  -4,   6,  -8,  10, -12,  14, -16,  18},
	{  19, -21,   2,  -4,  -1,   8, -10,  12, -14,  16, -18,  20, -22,   3,  -5,   7,  -9,  11, -13,  15, -17},
	{ -18,  20, -22,   3,  -5,   1,   9, -11,  13, -15,  17, -19,  21,  -2,   4,  -6,   8, -10,  12, -14,  16},
	{  17, -19,  21,  -2,   4,  -6,  -1,  10, -12,  14, -16,  18, -20,  22,  -3,   5,   7,   9, -11,  13, -15},
	{ -16,  18, -20,  22,  -3,   5,  -7,   1,  11, -13,  15, -17,  19, -21,   2,  -4,   6,  -8,  10, -12,  14},
	{  15, -17,  19, -21,   2,  -4,   6,  -8,  -1,  12, -14,  16, -18,  20, -22,   3,  -5,   7,  -9,  11, -13},
	{ -14,  16, -18,  20, -22,   3,  -5,   7,  -9,   1,  13, -15,  17, -19,  21,  -2,   4,  -6,   8, -10,  12},
	{  13, -15,  17, -19,  21,  -2,   4,  -6,   8, -10,  -1,  14, -16,  18, -20,  22,  -3,   5,  -7,   9, -11},
	{ -12,  14, -16,  18, -20,  22,  -3,   5,  -7,   9, -11,   1,  15, -17,  19, -21,   2,  -4,   6,  -8,  10},
	{  11, -13,  15, -17,  19, -21,   2,  -4,   6,  -8,  10, -12,  -1,  16, -18,  20, -22,   3,  -5,   7, -9},
	{ -10,  12, -14,  16, -18,  20, -22,   3,  -5,   7,  -9,  11, -13,   1,  17, -19,  21,  -2,   4,  -6,  8},
	{   9, -11,  13, -15,  17, -19,  21,  -2,   4,  -6,   8, -10,  12, -14,  -1,  18, -20,  22,  -3,   5, -7},
	{  -8,  10, -12,  14, -16,  18, -20,  22,  -3,   5,  -7,   9, -11,  13, -15,   1,  19, -21,   2,  -4,  6},
	{   7,  -9,  11, -13,  15, -17,  19, -21,   2,  -4,   6,  -8,  10, -12,  14, -16,  -1,  20, -22,   3, -5},
	{  -6,   8, -10,  12, -14,  16, -18,  20, -22,   3,  -5,   7,  -9,  11, -13,  15, -17,   1,  21,  -2,  4},
	{   5,  -7,   9, -11,  13, -15, -17,  19,  21,  -2,   4,  -6,   8, -10,  12, -14,  16, -18,  -1,  22, -3},
	{  -4,   6,  -8,  10, -12,  14, -16,  18, -20,  22,  -3,   5,  -7,   9, -11,  13, -15,  17, -19,   1,  2},
	{   3,  -5,   7,  -9,  11, -13,  15, -17,  19,  21,   2,  -4,   6,  -8,  10, -12,  14, -16,  18, -20, -1},
};

int round_robin_20[20][19] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16, -17,  18, -19,  20},
	{  -1,   4,  -6,   8, -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,  13, -15,  17, -19},
	{ -20,   1,   5,  -7,   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  14, -16,  18},
	{  19,  -2,  -1,   6,  -8,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13,  15, -17},
	{ -18,  20,  -3,   1,   7,  -9,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12, -14,  16},
	{  17, -19,   2,  -4,  -1,   8, -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,  13, -15},
	{ -16,  18, -20,   3,  -5,   1,   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  14},
	{  15, -17,  19,  -2,   4,  -6,  -1,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13},
	{ -14,  16, -18,  20,  -3,   5,  -7,   1,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12},
	{  13, -15,  17, -19,   2,  -4,   6,  -8,  -1,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11},
	{ -12,  14, -16,  18, -20,   3,  -5,   7,  -9,   1,  13, -15,  17, -19,   2,  -4,   6,  -8,  10},
	{  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  -1,  14, -16,  18, -20,   3,  -5,   7,  -9},
	{ -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,   1,  15, -17,  19,  -2,   4,  -6,   8},
	{   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  -1,  16, -18,  20,  -3,   5,  -7},
	{  -8,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13,   1,  17, -19,   2,  -4,   6},
	{   7,  -9,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12, -14,  -1,  18, -20,   3,  -5},
	{  -6,   8, -10,  12, -14,  16, -18,  20,  -3,   5,  -7,   9, -11,  13, -15,   1,  19,  -2,   4},
	{   5,  -7,   9, -11,  13, -15,  17, -19,   2,  -4,   6,  -8,  10, -12,  14, -16,  -1,  20,  -3},
	{  -4,   6,  -8,  10, -12,  14, -16,  18, -20,   3,  -5,   7,  -9,  11, -13,  15, -17,   1,   2},
	{   3,  -5,   7,  -9,  11, -13,  15, -17,  19,  -2,   4,  -6,   8, -10,  12, -14,  16, -18,  -1},
};

int round_robin_18[18][17] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16, -17,  18},
	{  -1,   4,  -6,   8, -10,  12, -14,  16, -18,   3,  -5,   7,  -9,  11, -13,  15, -17},
	{ -18,   1,   5,  -7,   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  12, -14,  16},
	{  17,  -2,  -1,   6,  -8,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11,  13, -15},
	{ -16,  18,  -3,   1,   7,  -9,  11, -13,  15, -17,   2,  -4,   6,  -8,  10, -12,  14},
	{  15, -17,   2,  -4,  -1,   8, -10,  12, -14,  16, -18,   3,  -5,   7,  -9,  11, -13},
	{ -14,  16, -18,   3,  -5,   1,   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  12},
	{  13, -15,  17,  -2,   4,  -6,  -1,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11},
	{ -12,  14, -16,  18,  -3,   5,  -7,   1,  11, -13,  15, -17,   2,  -4,   6,  -8,  10},
	{  11, -13,  15, -17,   2,  -4,   6,  -8,  -1,  12, -14,  16, -18,   3,  -5,   7,  -9},
	{ -10,  12, -14,  16, -18,   3,  -5,   7,  -9,   1,  13, -15,  17,  -2,   4,  -6,   8},
	{   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  -1,  14, -16,  18,  -3,   5,  -7},
	{  -8,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11,   1,  15, -17,   2,  -4,   6},
	{   7,  -9,  11, -13,  15, -17,   2,  -4,   6,  -8,  10, -12,  -1,  16, -18,   3,  -5},
	{  -6,   8, -10,  12, -14,  16, -18,   3,  -5,   7,  -9,  11, -13,   1,  17,  -2,   4},
	{   5,  -7,   9, -11,  13, -15,  17,  -2,   4,  -6,   8, -10,  12, -14,  -1,  18,  -3},
	{  -4,   6,  -8,  10, -12,  14, -16,  18,  -3,   5,  -7,   9, -11,  13, -15,   1,   2},
	{   3,  -5,   7,  -9,  11, -13,  15, -17,   2,  -4,   6,  -8,  10, -12,  14, -16,  -1},
};

int round_robin_16[16][15] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14, -15,  16},
	{  -1,   4,  -6,   8, -10,  12, -14,  16,  -3,   5,  -7,   9, -11,  13, -15},
	{ -16,   1,   5,  -7,   9, -11,  13, -15,   2,  -4,   6,  -8,  10, -12,  14},
	{  15,  -2,  -1,   6,  -8,  10, -12,  14, -16,   3,  -5,   7,  -9,  11, -13},
	{ -14,  16,  -3,   1,   7,  -9,  11, -13,  15,  -2,   4,  -6,   8, -10,  12},
	{  13, -15,   2,  -4,  -1,   8, -10,  12, -14,  16,  -3,   5,  -7,   9, -11},
	{ -12,  14, -16,   3,  -5,   1,   9, -11,  13, -15,   2,  -4,   6,  -8,  10},
	{  11, -13,  15,  -2,   4,  -6,  -1,  10, -12,  14, -16,   3,  -5,   7,  -9},
	{ -10,  12, -14,  16,  -3,   5,  -7,   1,  11, -13,  15,  -2,   4,  -6,   8},
	{   9, -11,  13, -15,   2,  -4,   6,  -8, - 1,  12, -14,  16,  -3,   5,  -7},
	{  -8,  10, -12,  14, -16,   3,  -5,   7,  -9,   1,  13, -15,   2,  -4,   6},
	{   7,  -9,  11, -13,  15,  -2,   4,  -6,   8, -10,  -1,  14, -16,   3,  -5},
	{  -6,   8, -10,  12, -14,  16,  -3,   5,  -7,   9, -11,   1,  15,  -2,   4},
	{   5,  -7,   9, -11,  13, -15,   2,  -4,   6,  -8,  10, -12,  -1,  16,  -3},
	{  -4,   6,  -8,  10, -12,  14, -16,   3,  -5,   7,  -9,  11, -13,   1,   2},
	{   3,  -5,   7,  -9,  11, -13,  15,  -2,   4,  -6,   8, -10,  12, -14,  -1},
};

int round_robin_14[14][13] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12, -13,  14},
	{  -1,   4,  -6,   8, -10,  12, -14,   3,  -5,   7,  -9,  11, -13},
	{ -14,   1,   5,  -7,   9, -11,  13,  -2,   4,  -6,   8, -10,  12},
	{  13,  -2,  -1,   6,  -8,  10, -12,  14,  -3,   5,  -7,   9, -11},
	{ -12,  14,  -3,   1,   7,  -9,  11, -13,   2,  -4,   6,  -8,  10},
	{  11, -13,   2,  -4,  -1,   8, -10,  12, -14,   3,  -5,   7,  -9},
	{ -10,  12, -14,   3,  -5,   1,   9, -11,  13,  -2,   4,  -6,   8},
	{   9, -11,  13,  -2,   4,  -6,  -1,  10, -12,  14,  -3,   5,  -7},
	{  -8,  10, -12,  14,  -3,   5,  -7,   1,  11, -13,   2,  -4,   6},
	{   7,  -9,  11, -13,   2,  -4,   6,  -8,  -1,  12, -14,   3,  -5},
	{  -6,   8, -10,  12, -14,   3,  -5,   7,  -9,   1,  13,  -2,   4},
	{   5,  -7,   9, -11,  13,  -2,   4,  -6,   8, -10,  -1,  14,  -3},
	{  -4,   6,  -8,  10, -12,  14,  -3,   5,  -7,   9, -11,   1,   2},
	{   3,  -5,   7,  -9,  11, -13,   2,  -4,   6,  -8,  10, -12,  -1},
};

int round_robin_12[12][11] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10, -11,  12},
	{  -1,   4,  -6,   8, -10,  12,  -3,   5,  -7,   9, -11},
	{ -12,   1,   5,  -7,   9, -11,   2,  -4,   6,  -8,  10},
	{  11,  -2,  -1,   6,  -8,  10, -12,   3,  -5,   7,  -9},
	{ -10,  12,  -3,   1,   7,  -9,  11,  -2,   4,  -6,   8},
	{   9, -11,   2,  -4,  -1,   8, -10,  12,  -3,   5,  -7},
	{  -8,  10, -12,   3,  -5,   1,   9, -11,   2,  -4,   6},
	{   7,  -9,  11,  -2,   4,  -6,  -1,  10, -12,   3,  -5},
	{  -6,   8, -10,  12,  -3,   5,  -7,   1,  11,  -2,   4},
	{   5,  -7,   9, -11,   2,  -4,   6,  -8,  -1,  12,  -3},
	{  -4,   6,  -8,  10, -12,   3,  -5,   7,  -9,   1,   2},
	{   3,  -5,   7,  -9,  11,  -2,   4,  -6,   8, -10,  -1},
};

int round_robin_10[10][9] = {
	{   2,  -3,   4,  -5,   6,  -7,   8,  -9,  10},
	{  -1,   4,  -6,   8, -10,   3,  -5,   7,  -9},
	{ -10,   1,   5,  -7,   9,  -2,   4,  -6,   8},
	{   9,  -2,  -1,   6,  -8,  10,  -3,   5,  -7},
	{  -8,  10,  -3,   1,   7,  -9,   2,  -4,   6},
	{   7,  -9,   2,  -4,  -1,   8, -10,   3,  -5},
	{  -6,   8, -10,   3,  -5,   1,   9,  -2,   4},
	{   5,  -7,   9,  -2,   4,  -6,  -1,  10,  -3},
	{  -4,   6,  -8,  10,  -3,   5,  -7,   1,   2},
	{   3,  -5,   7,  -9,   2,  -4,   6,  -8,  -1},
};

int round_robin_8[8][7] = {
	{  2, -3,  4, -5,  6, -7,  8},
	{ -1,  4, -6,  8, -3,  5, -7},
	{ -8,  1,  5, -7,  2, -4,  6},
	{  7, -2, -1,  6, -8,  3, -5},
	{ -6,  8, -3,  1,  7, -2,  4},
	{  5, -7,  2, -4, -1,  8, -3},
	{ -4,  6, -8,  3, -5,  1,  2},
	{  3, -5,  7, -2,  4, -6, -1},
};

int round_robin_6[6][5] = {
	{   2, -3,  4, -5,  6},
	{  -1,  4, -6,  3, -5},
	{  -6,  1,  5, -2,  4},
	{   5, -2, -1,  6, -3},
	{  -4,  6, -3,  1,  2},
	{   3, -5,  2, -4, -1},
};

struct round_robins{
	int max_teams;
	int** robins;
} round_robins[] = {
	{ 6, (int**)round_robin_6},
	{ 8, (int**)round_robin_8},
	{10, (int**)round_robin_10},
	{12, (int**)round_robin_12},
	{14, (int**)round_robin_14},
	{16, (int**)round_robin_16},
	{18, (int**)round_robin_18},
	{20, (int**)round_robin_20},
	{0}
};

void help(){
	printf("Usage: generate_schedule {[OPTIONS]} [FILE]\n");
	printf("Generate one, or more, round-robin schedules for a set of teams\n");
	printf("\n");
	printf("  -m, --human           output in human-readable format\n");
	printf("  -l, --league_republic output in csv format for LR\n");
	printf("  -h, --help            this help\n");
}

enum output_format{
	none,
	human,
	league_republic
};
struct output_format_str{
	enum output_format f;
	const char* s;
}output_format_str[] = {
	{none, "default"},
	{human, "human"},
	{league_republic, "league_republic"},
	{0, 0}
};
enum output_format output_format = none;
const char* config_file;

const char* format2str(enum output_format f){
	int i=0;
	while (output_format_str[i].s){
		if (output_format_str[i].f == f){
			return output_format_str[i].s;
		}
		++i;
	}
	return "unknown!";
}

enum output_format change_format(enum output_format new_format){
	if (output_format != none){
		fprintf(stderr, "Conflicting output formats, was '%s', now using '%s'\n", format2str(output_format), format2str(new_format));
	}
	return output_format = new_format;
}
int get_options(int argc, char** argv){

	while(1){
		int c;
		static struct option long_options[] = {
			{"help",                no_argument, 0, 'h'},
			{"human",               no_argument, 0, 'm'},
			{"league_republic",     no_argument, 0, 'l'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		if ( (c = getopt_long(argc, argv, "hml", long_options, &option_index)) == -1)
			break; // finished options
		switch (c){
			case 0:
				printf("long option %s", long_options[option_index].name);
				if (optarg)
					printf(" with arg %s", optarg);
				printf("\n");
				break;
			case 'h':
				printf("Help:\n");
				help();
				exit(EXIT_SUCCESS);
			case 'm': output_format = change_format(human); break;
			case 'l': output_format = change_format(league_republic); break;
			case '?': break;
			default:
				printf("Unrecognised option: %c\n", c);
				help();
				break;
		}

	}
	if (output_format == none){
		output_format = change_format(human);
	}

	if ((argc - optind) == 1){
		config_file = argv[optind];
	}else{
		fprintf(stderr, "Missing config file\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}


int readfile(const char* filename){
	FILE* fp;
	int weekno;
	char buf[PATH_MAX];

	if (!(fp = fopen(filename, "r"))){
		fprintf(stderr, "Cannot open/find %s\n", filename);
		exit(EXIT_FAILURE);
	}

	while (fgets(buf, sizeof buf, fp)){
		int teamnum;
		char left[PATH_MAX], right[PATH_MAX];
		sscanf(buf, "%[^:]:%[^\n]", left, right);
		for (char* p=left; *p; ++p) *p = tolower(*p);
		if (!strcasecmp(left, "division")){
			strncpy(division, right, sizeof division);
		}else if (!strcasecmp(left, "time")){
			strncpy(starttime, right, sizeof starttime);
		}else if ( (teamnum = atoi(left)) >0 && (teamnum <= MAX_TEAMS) ){
			char team[PATH_MAX], venue[PATH_MAX] = {0}, *scode=0;
			// printf("Trying [%s]\n", )
			if (sscanf(right, "%[^|]|%[^\n]", team, venue) > 0){
				if (teams[teamnum-1][0]){
					fprintf(stderr, "Duplicate team number %d: %s/%s\n", teamnum, teams[teamnum-1], team);
					exit(EXIT_FAILURE);
				}
				if ( (scode = strchr(team, '%')) ){
					*scode++ = 0;
					// printf("Got %s [%s]\n", team, scode);
					strncpy(&shortcodes[teamnum-1][0], scode, sizeof shortcodes[0]);
				}else{
					// printf("Got %s [---]\n", team, scode);
				}

				strncpy(&teams[teamnum-1][0], team, sizeof teams[0]);
				strncpy(&venues[teamnum-1][0], *venue?venue:team, sizeof venues[0]);
				if (teamnum > lastteam){
					lastteam = teamnum;
				}
			}
		}else if (sscanf(left, "week%d", &weekno)){
			if (weekno > 0 && weekno <= MAX_WEEKS){
				strncpy(weeks[weekno -1], right, sizeof weeks[0]);
				if (weekno > lastweek){
					lastweek = weekno;
				}
			}
		}else{
			// printf("Ignoring line: %s", buf);
		}
	}
	return 0;
}

int main(int argc, char** argv){
	int i, matchcount;

	if (argc < 2){
		fprintf(stderr, "generate_schedule <file>\n");
		exit(EXIT_FAILURE);
	}

	get_options(argc, argv);
	readfile(config_file);

	if (output_format == human){
		printf("Division - %s\n", division);
		printf("Start time - %s\n", starttime);
		printf("%d team: \n", lastteam);
		for(i=0; i < lastteam; ++i){
			printf("%d - %s[%s](%s)\n", i+1, teams[i], shortcodes[i], venues[i]);
		}
	}

	int success = 0;
	for(i=0; round_robins[i].robins; i++){
		if ((lastteam == round_robins[i].max_teams) || (lastteam == round_robins[i].max_teams-1)){
			// printf("lastteam:%d round_robins[i].max_teams:%d\n", lastteam, round_robins[i].max_teams);
			while (lastteam < round_robins[i].max_teams){
				lastteam++;
				strncpy(&teams[lastteam-1][0], "Bye", sizeof teams[0]);
				strncpy(&venues[lastteam-1][0], "Bye", sizeof venues[0]);
			}
			success = 1;
			break;
		}else{
			// printf("%d failed againt %d\n", lastteam, round_robins[i].max_teams);f
		}
	}
	if (!success){
		fprintf(stderr, "Can't find a matrix for %d teams (only have for 10,12,14,16,18,20)\n", lastteam);
		fprintf(stderr, "round_robins[0].robins = %p\n", round_robins[0].robins);
		exit(EXIT_FAILURE);
	}

	if (output_format == human){
		printf("Using matrix for %d teams\n", lastteam);
	}
	matchcount = lastteam-1;

	for(int league_week = 0; league_week < lastweek; league_week++){
		int robin = league_week / matchcount;
		for(int team = 0; team < lastteam; ++team){
			int team1, team2;
			// int home, away;

			team1 = team+1;
			switch (lastteam){
				case 6:
					team2 = round_robin_6[team][league_week % matchcount];
					break;
				case 8:
					team2 = round_robin_8[team][league_week % matchcount];
					break;
				case 10:
					team2 = round_robin_10[team][league_week % matchcount];
					break;
				case 12:
					team2 = round_robin_12[team][league_week % matchcount];
					break;
				case 14:
					team2 = round_robin_14[team][league_week % matchcount];
					break;
				case 16:
					team2 = round_robin_16[team][league_week % matchcount];
					break;
				case 18:
					team2 = round_robin_18[team][league_week % matchcount];
					break;
				case 20:
					team2 = round_robin_20[team][league_week % matchcount];
					break;
				case 22:
					team2 = round_robin_22[team][league_week % matchcount];
					break;
				case 24:
					team2 = round_robin_24[team][league_week % matchcount];
					break;
				default:
					fprintf(stderr, "Unknown lastteam: %d\n", lastteam);
					exit(EXIT_FAILURE);
			}

			if (robin%2 == 1){
				team2 *= -1; // swap venue
			}

			if (team2 > 0){
				static int first = 1;
				switch (output_format){
					case none:
					case human:
						printf("%d, [%s] %s(%d) v %s(%d) @ %s\n", league_week+1, weeks[league_week], teams[team1-1], team1, teams[team2-1], team2, venues[team1-1]);
						break;
					case league_republic:
						if (first){
							// "Date dd/mm/yyyy,Time HH:MM,Division,Home Team,Away Team,Venue,Pitch,Home Score,Away Score";
							first = 0;
							printf("%s\n", header);
						}
						if (shortcodes[team1-1][0] && shortcodes[team2-1][0]) {
							printf("%s,%s,,%s,%s,,,,\n",
								weeks[league_week],
								starttime,
								shortcodes[team1-1],
								shortcodes[team2-1]
								);
						}else{
							// skip matches with byes
						}
						break;
				}

			}
		}
	}
}

/** Format for input file

Division: <name of division>
Time: <time for matches, default 20:00>
1: <first team name>{|<venue name>}
2: <second team name>{|<venue name>}
3: <...>
...

Week1:dd/mm/yyy
Week2:dd/mm/yyy
Week3:dd/mm/yyy
...
Week19:dd/mm/yyy

*/
