"""
Defines Python wrapper for the lunar_score() function in lunar.c
"""

cdef extern from "lunar.c":
    int lunar_score(int input_count, int input_value[])

def py_lunar_score(int input_count, int[:] input_value):
    return lunar_score(input_count, &input_value[0])
