import pytest
import Python.Filters.signal_utilities as su

__author__ = 'Jeremy Malloch'


def is_close(a, b, rel_tol=1e-09, abs_tol=0.0):
    """
    Determines if two floats are 'close', so that it can be used in place of the comparison operator
    Is a default function in the Python 3.5 math library, but we're using Python 2 for the project  -_____-
    """
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)


class TestMovingAverageReturnInt:
    """
    Testing the moving average algorithm returning an integer
    """
    @classmethod
    def setup_class(cls):
        su.MovingAverage





