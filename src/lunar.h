#ifndef LUNAR_H
#define LUNAR_H

// Given a sequence of input values, run the lunar simulation and return a
// score.
//
// For a safe landing, score is the positive amount of fuel remaining plus a
// bonus based on softness of landing.
//
// For a survivable landing that damages the LEM, score is zero.
//
// For a fatal crash, score is a negative value proportional to downward speed.
extern int lunar_score(int input_count, int input_value[]);

#endif
