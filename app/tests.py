"""
Unit tests
"""

import unittest

import numpy as np

# The lunar_wrapper module must be built using the setup.py script in the parent
# directory.
from .lunar_wrapper import py_lunar_score

class LunarWrapperTest(unittest.TestCase):
    """
    Tests of the lunar_wrapper module and lunar_score() C function.
    """

    def test_zeroes(self):
        # COMMENCE LANDING PROCEDURE
        # TIME,SECS   ALTITUDE,MILES+FEET   VELOCITY,MPH   FUEL,LBS   FUEL RATE
        #       0             120      0        3600.00     16000.0      K=:0
        #      10             109   5016        3636.00     16000.0      K=:0
        #      20              99   4224        3672.00     16000.0      K=:0
        #      30              89   2904        3708.00     16000.0      K=:0
        #      40              79   1056        3744.00     16000.0      K=:0
        #      50              68   3960        3780.00     16000.0      K=:0
        #      60              58   1056        3816.00     16000.0      K=:0
        #      70              47   2904        3852.00     16000.0      K=:0
        #      80              36   4224        3888.00     16000.0      K=:0
        #      90              25   5016        3924.00     16000.0      K=:0
        #     100              15      0        3960.00     16000.0      K=:0
        #     110               3   5016        3996.00     16000.0      K=:0
        # ON THE MOON AT   113.55 SECS
        # IMPACT VELOCITY OF  4008.79 M.P.H.
        # FUEL LEFT: 16000.00 LBS
        # SORRY,BUT THERE WERE NO SURVIVORS-YOU BLEW IT!
        # IN FACT YOU BLASTED A NEW LUNAR CRATER  1113.55 FT. DEEP
        input_value = np.array([0], dtype=np.intc)
        input_count = len(input_value)
        result = py_lunar_score(input_count, input_value)
        self.assertEqual(result, -4008)

    def test_poor_landing(self):
        # COMMENCE LANDING PROCEDURE
        # TIME,SECS   ALTITUDE,MILES+FEET   VELOCITY,MPH   FUEL,LBS   FUEL RATE
        #       0             120      0        3600.00     16000.0      K=:0
        #      10             109   5016        3636.00     16000.0      K=:0
        #      20              99   4224        3672.00     16000.0      K=:0
        #      30              89   2904        3708.00     16000.0      K=:0
        #      40              79   1056        3744.00     16000.0      K=:0
        #      50              68   3960        3780.00     16000.0      K=:0
        #      60              58   1056        3816.00     16000.0      K=:0
        #      70              47   2904        3852.00     16000.0      K=:170
        #      80              37   1474        3539.86     14300.0      K=:200
        #      90              27   5247        3140.80     12300.0      K=:200
        #     100              19   4537        2710.41     10300.0      K=:200
        #     110              12   5118        2243.83      8300.0      K=:200
        #     120               7   2285        1734.97      6300.0      K=:200
        #     130               3   1990        1176.06      4300.0      K=:200
        #     140               0   5040         556.96      2300.0      K=:170
        #     150               0   1040         -21.20       600.0      K=:0
        #     160               0   1087          14.80       600.0      K=:0
        #     170               0    606          50.80       600.0      K=:30
        #     180               0    436         -27.90       300.0      K=:0
        #     190               0    581           8.10       300.0      K=:8
        #     200               0    425          13.17       220.0      K=:10
        #     210               0    253          10.30       120.0      K=:9
        #     220               0     95          11.11        30.0      K=:100
        # FUEL OUT AT   220.30 SECS
        # ON THE MOON AT   226.11 SECS
        # IMPACT VELOCITY OF    21.35 M.P.H.
        # FUEL LEFT:     0.00 LBS
        # CONGRATULATIONS ON A POOR LANDING
        input_value = np.array(
            [0, 0, 0, 0, 0, 0, 0, 170, 200, 200, 200, 200, 200, 200, 170, 0, 0, 30, 0, 8, 10, 9, 100],
            dtype=np.intc)
        input_count = len(input_value)
        result = py_lunar_score(input_count, input_value)
        self.assertEqual(result, 0)

    def test_good_landing(self):
        # COMMENCE LANDING PROCEDURE
        # TIME,SECS   ALTITUDE,MILES+FEET   VELOCITY,MPH   FUEL,LBS   FUEL RATE
        #       0             120      0        3600.00     16000.0      K=:0
        #      10             109   5016        3636.00     16000.0      K=:0
        #      20              99   4224        3672.00     16000.0      K=:0
        #      30              89   2904        3708.00     16000.0      K=:0
        #      40              79   1056        3744.00     16000.0      K=:0
        #      50              68   3960        3780.00     16000.0      K=:0
        #      60              58   1056        3816.00     16000.0      K=:0
        #      70              47   2904        3852.00     16000.0      K=:180
        #      80              37   1626        3518.79     14200.0      K=:200
        #      90              28    438        3118.26     12200.0      K=:200
        #     100              20     71        2686.18     10200.0      K=:200
        #     110              13   1022        2217.65      8200.0      K=:200
        #     120               7   3868        1706.49      6200.0      K=:200
        #     130               3   4011        1144.83      4200.0      K=:200
        #     140               1   2263         522.40      2200.0      K=:130
        #     150               0   3000          91.49       900.0      K=:15
        #     160               0   1805          71.39       750.0      K=:15
        #     170               0    908          50.80       600.0      K=:15
        #     180               0    317          29.70       450.0      K=:14
        #     190               0     11          11.96       310.0      K=:30
        # ON THE MOON AT   190.93 SECS
        # IMPACT VELOCITY OF     4.53 M.P.H.
        # FUEL LEFT:   282.04 LBS
        # GOOD LANDING-(COULD BE BETTER)
        input_value = np.array(
            [0, 0, 0, 0, 0, 0, 0, 180, 200, 200, 200, 200, 200, 200, 130, 15, 15, 15, 14, 30],
            dtype=np.intc)
        input_count = len(input_value)
        result = py_lunar_score(input_count, input_value)
        self.assertEqual(result, 8000 + 282)
