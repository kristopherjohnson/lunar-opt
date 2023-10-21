// Translation of
// <http://www.cs.brandeis.edu/~storer/LunarLander/LunarLander/LunarLanderListing.jpg>
// by Jim Storer from FOCAL to C.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "lunar.h"

// Set this non-zero to get debug output
static int trace = 0;

// Global variables
//
// A - Altitude (miles)
// G - Gravity
// I - Intermediate altitude (miles)
// J - Intermediate velocity (miles/sec)
// K - Fuel rate (lbs/sec)
// L - Elapsed time (sec)
// M - Total weight (lbs)
// N - Empty weight (lbs, Note: M - N is remaining fuel weight)
// S - Time elapsed in current 10-second turn (sec)
// T - Time remaining in current 10-second turn (sec)
// V - Downward speed (miles/sec)
// W - Temporary working variable
// Z - Thrust per pound of fuel burned

static double A, G, I, J, K, L, M, N, S, T, V, W, Z;

static void update_lander_state();
static void apply_thrust();

// Given a sequence of input values, run the lunar simulation and return a
// score.
//
// For a safe landing, score is the positive amount of fuel remaining plus a
// bonus based on softness of landing.
//
// For a survivable landing that damages the LEM, score is zero.
//
// For a fatal crash, score is a negative value proportional to downward speed.
int lunar_score(int input_count, int input_value[])
{
    if (trace) printf("input_count=%d\n", input_count);

    int input_index = 0;

    // 01.20 in original FOCAL code
    // puts("FIRST RADAR CHECK COMING UP\n\n");
    // puts("COMMENCE LANDING PROCEDURE");
    // puts("TIME,SECS   ALTITUDE,MILES+FEET   VELOCITY,MPH   FUEL,LBS   FUEL RATE");

    A = 120;
    V = 1;
    M = 32500;
    N = 16500;
    G = .001;
    Z = 1.8;
    L = 0;

start_turn: // 02.10 in original FOCAL code
    if (trace) printf("%7.0f%16.0f%7.0f%15.2f%12.1f      ",
            L,
            trunc(A),
            5280 * (A - trunc(A)),
            3600 * V,
            M - N);

    {
        //fputs("K=:", stdout);
        int is_valid_input = (input_index < input_count);
        if (is_valid_input)
        {
            K = input_value[input_index++];
        }
        if (!is_valid_input || K < 0 || ((0 < K) && (K < 8)) || K > 200)
        {
            // For any invalid input, use zero instead.
            K = 0;
        }
        if (trace) printf("K=:%d\n", (int)K);
    }

    T = 10;

turn_loop:
    for (;;) // 03.10 in original FOCAL code
    {
        if (M - N < .001)
            goto fuel_out;

        if (T < .001)
            goto start_turn;

        S = T;

        if (N + S * K - M > 0)
            S = (M - N) / K;

        apply_thrust();

        if (I <= 0)
            goto loop_until_on_the_moon;

        if ((V > 0) && (J < 0))
        {
            for (;;) // 08.10 in original FOCAL code
            {
                // FOCAL-to-C gotcha: In FOCAL, multiplication has a higher
                // precedence than division.  In C, they have the same
                // precedence and are evaluated left-to-right.  So the
                // original FOCAL subexpression `M * G / Z * K` can't be
                // copied as-is into C: `Z * K` has to be parenthesized to
                // get the same result.
                W = (1 - M * G / (Z * K)) / 2;
                S = M * V / (Z * K * (W + sqrt(W * W + V / Z))) + 0.5;
                apply_thrust();
                if (I <= 0)
                    goto loop_until_on_the_moon;
                update_lander_state();
                if (-J < 0)
                    goto turn_loop;
                if (V <= 0)
                    goto turn_loop;
            }
        }

        update_lander_state();
    }

loop_until_on_the_moon: // 07.10 in original FOCAL code
    while (S >= .005)
    {
        S = 2 * A / (V + sqrt(V * V + 2 * A * (G - Z * K / M)));
        apply_thrust();
        update_lander_state();
    }
    goto on_the_moon;

fuel_out: // 04.10 in original FOCAL code
    //printf("FUEL OUT AT %8.2f SECS\n", L);
    S = (sqrt(V * V + 2 * A * G) - V) / G;
    V += G * S;
    L += S;

on_the_moon: // 05.10 in original FOCAL code
    //printf("ON THE MOON AT %8.2f SECS\n", L);
    W = 3600 * V;
    //printf("IMPACT VELOCITY OF %8.2f M.P.H.\n", W);
    //printf("FUEL LEFT: %8.2f LBS\n", M - N);
    if (W <= 1)
    {
        if (trace) puts("PERFECT LANDING !-(LUCKY)");
        return M - N + 16000;
    }
    else if (W <= 10)
    {
        if (trace) puts("GOOD LANDING-(COULD BE BETTER)");
        return M - N + 8000;
    }
    else if (W <= 22)
    {
        if (trace) puts("CONGRATULATIONS ON A POOR LANDING");
        return M - N;
    }
    else if (W <= 40)
    {
        if (trace) puts("CRAFT DAMAGE. GOOD LUCK");
        return 0;
    }
    else if (W <= 60)
    {
        if (trace) puts("CRASH LANDING-YOU'VE 5 HRS OXYGEN");
        return 0;
    }
    else
    {
        if (trace)
        {
            puts("SORRY,BUT THERE WERE NO SURVIVORS-YOU BLEW IT!");
            printf("IN FACT YOU BLASTED A NEW LUNAR CRATER %8.2f FT. DEEP\n", W * .277777);
        }
        return -W;
    }
}

// Subroutine at line 06.10 in original FOCAL code
void update_lander_state()
{
    L += S;
    T -= S;
    M -= S * K;
    A = I;
    V = J;
}

// Subroutine at line 09.10 in original FOCAL code
void apply_thrust()
{
    double Q = S * K / M;
    double Q_2 = pow(Q, 2);
    double Q_3 = pow(Q, 3);
    double Q_4 = pow(Q, 4);
    double Q_5 = pow(Q, 5);

    J = V + G * S + Z * (-Q - Q_2 / 2 - Q_3 / 3 - Q_4 / 4 - Q_5 / 5);
    I = A - G * S * S / 2 - V * S + Z * S * (Q / 2 + Q_2 / 6 + Q_3 / 12 + Q_4 / 20 + Q_5 / 30);
}
