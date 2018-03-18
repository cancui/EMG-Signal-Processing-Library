import pytest
import Python.Filters.signal_utilities as su

__author__ = 'Jeremy Malloch'


def is_close(a, b, rel_tol=1e-09, abs_tol=0.0):
    """
    Determines if two floats are 'close', so that it can be used in place of the comparison operator
    Is a default function in the Python 3.5 math library, but we're using Python 2 for the project  -_____-
    """
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)


class TestEMADefaults:
    """
    Testing the ExpMovingAverage class using the default parameters
    """
    @classmethod
    def setup_class(cls):
        self.Two = su.ExpMovingAverage()
        self.Zero = su.ExpMovingAverage()
        for _ in range (self.Two.window + 5):
            self.Two.get_EMA(2)
            self.Zero.get_EMA(0)

    def test_constant_value(self):
        assert isclose(self.Two.get_EMA(2), 2)

    def test_zero_value(self):
        assert isclose(self.Zero.get_EMA(0), 0)

    def test_one_nonzero_term(self):
        assert isclose(self.Zero.get_EMA(250), 125)

    def test_two_nonzero_term(self):
        assert  isclose(self.Zero.get_EMA(250), 187.5)

    def test_zeros_after_nonzero_values(self):
        for _ in range(self.Two.window - 1):
            self.Two.get_EMA(0)
        assert isclose(self.Two.get_EMA(0), 0)


class TestEMADifferentWindow:
    """
    Testing the ExpMovingAverage class using varying window values
    """
    @classmethod
    def setup_class(cls):
        self.Window1 = su.ExpMovingAverage(window_ = 1)
        self.Window100 = su.ExpMovingAverage(window_ = 100)

    def test_window_length_1(self):
        assert isclose(self.Window1.get_EMA(1), 1)
        assert isclose(self.Window1.get_EMA(2), 1)

    def test_window_length_100(self):
        for _ in range(99):
            self.Window100.get_EMA(10)
        assert isclose(self.Window100.get_EMA(10), 10)

    def test_single_zero_value(self):
        assert isclose(self.Window100.get_EMA(0), 5)


class TestEMADifferentExponentialFactors:
    """
    Testing with varying Exponential Factors
    """
    @classmethod
    def setup_class(cls):
        self.FactorQuarter = su.ExpMovingAverage(expFactor_= 0.25)
        self.FactorFourFifths = su.ExpMovingAverage(expFactor_= 0.8)

    def test_constant_value_four_fifths(self):
        for _ in range(self.FactorFourFifths.window - 1):
            self.FactorFourFifths.get_EMA(10)
        assert isclose(self.FactorQuarter.get_EMA(10), 10)

    def test_constant_value_quarter(self):
        for _ in range(self.FactorFourFifths.window - 1):
            self.FactorQuarter.get_EMA(10)
        assert isclose(self.FactorFourFifths.get_EMA(10), 10)


if __name__ == '__main__':
    pytest.main()
