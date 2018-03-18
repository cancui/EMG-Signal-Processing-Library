from __future__ import print_function, division
import Python.Filters.signal_utilities as su
from collections import deque


class EMGFilterBasic(object):
    """
    Simple moving average filter class that assumes no low-frequency noise and data centred at 0.
    More complex filters inherit from this.
    """

    def __init__(self, sample_frequency=200, range_=0.1, reference_available=False):
        self.log = deque([])  # Used to store filtered data
        self.reference_available = reference_available
        self.MA = su.MovingAverage(length=sample_frequency * range_, return_int=False)

        if reference_available:
            self.LPF_data = su.LowPassFilter(cutoff_frequency=150, sample_frequency=sample_frequency)
            self.LPF_reference = su.LowPassFilter(cutoff_frequency=150, sample_frequency=sample_frequency)

    def log_data(self, to_log):
        """
        Input filtered data
        """
        self.log.appendleft(to_log)

    def pop_log(self, num_to_pop=1):
        """
        Since you can't slice in a deque list a list, when popping more than one entries in a list you must repeatedly
        call the pop deque method.
        """
        if num_to_pop_ema == 1:
            return self.log.pop()
        to_return = []
        for _ in range(min(num_to_pop, len(self.log))):
            to_return.append(self.log.pop())
        return to_return

    def rectify(self, to_rectify):
        """
        Rectifies data, assuming low frequency noise already removed (reference point of 0)
        """
        return abs(to_rectify)

    def filter(self, data, reference_data=0):
        """
        This function is called to input raw data. It returns a filtered value if it has enough samples, and also logs it
        """
        if not self.reference_available:
            filtered_value = self.MA.get_ma(self.rectify(data))
            if filtered_value:  # If get_ma returned a value, log it and return it
                self.log_data(filtered_value)
                return filtered_value
            return

        else:
            clean_data = self.LPF_data.filter(data)
            clean_reference = self.LPF_reference.filter(reference_data)
            filtered_value = self.MA.get_ma(self.rectify(clean_data - clean_reference))
            self.log_data(filtered_value)
            # ("Reference used")
            return filtered_value


class EMGFilter(EMGFilterBasic):
    """
    Extension of EMGFilterBasic that has tools to remove low-frequency noise and normalize the data
    """

    def __init__(self, sample_frequency=200, range_=0.5, min_EMG_frequency=25, max_EMG_frequency=150,
                 reference_available=False):
        EMGFilterBasic.__init__(self, sample_frequency=sample_frequency, range_=range_,
                                reference_available=reference_available)
        # self.reference_available = reference_available
        self.PkPk = su.PeakToPeak(sample_frequency=sample_frequency, min_frequency=min_EMG_frequency,
                                  max_frequency=max_EMG_frequency)

    def filter(self, data, reference_data=0):
        """
        This function is called to input raw data and return a filtered value, accounting for low-frequency noise and un-normalized data
        """
        if self.reference_available == True:
            clean_data = self.LPF_data.filter(data)
            clean_reference = self.LPF_reference.filter(reference_data)
            data = clean_data - clean_reference

        neutral_value = self.PkPk.get_pkpk(data)['neutral']
        filtered_value = self.MA.get_ma(self.rectify(data - neutral_value))
        self.log_data(filtered_value)
        return filtered_value

        # return super(EMG_filter, self).filter(data - neutral_value)


class EMGFilterBasicEMA(object):
    """
    Simple exponential moving average filter class that assumes no low-frequency noise and data centred at 0.
    """

    def __init__(self, sample_frequency=200, range_=0.1, reference_available=False, exponential_factor_=0.5):
        self.reference_available = reference_available
        self.ema = su.ExpMovingAverage(frame_length=sample_frequency * range_, exponential_factor=exponential_factor_)

        if reference_available:
            self.LPF_data = su.LowPassFilter(cutoff_frequency=150, sample_frequency=sample_frequency)
            self.LPF_reference = su.LowPassFilter(cutoff_frequency=150, sample_frequency=sample_frequency)

    def pop_log(self, num_to_pop=1):
        """
        Pop and return either one data point or a set of data points
        """
        return self.ema.pop_log(num_to_pop_ema=num_to_pop)

    def rectify(self, to_rectify):
        """
        Rectifies data, assuming low frequency noise already removed (reference point of 0)
        """
        return abs(to_rectify)

    def filter(self, data, reference_data=0):
        """
        This function is called to input raw data. It returns a filtered value if it has enough samples, and also logs
        it
        """
        if not self.reference_available:
            return self.ema.get_ema(self.rectify(data))

        else:
            clean_data = self.LPF_data.filter(data)
            clean_reference = self.LPF_reference.filter(reference_data)
            return self.ema.get_ema(self.rectify(clean_data - clean_reference))


class EMGFilterEMA(EMGFilterBasicEMA):
    """
    Extension of EMGFilterBasic that has tools to remove low-frequency noise and normalize the data
    """

    def __init__(self, sample_frequency=200, range_=0.5, min_EMG_frequency=25, max_EMG_frequency=150,
                 reference_available_=False, exponential_factor=0.5):
        EMGFilterBasicEMA.__init__(self, sample_frequency=sample_frequency, range_=range_,
        reference_available=reference_available, exponential_factor_=exponential_factor))
        self.reference_available = reference_available_
        self.PkPk = su.PeakToPeak(sample_frequency=sample_frequency, min_frequency=min_EMG_frequency,
                                  max_frequency=max_EMG_frequency)

    def filter(self, data, reference_data=0):
        """
        This function is called to input raw data and return a filtered value, accounting for low-frequency noise and
        un-normalized data
        """
        if self.reference_available:
            clean_data = self.LPF_data.filter(data)
            clean_reference = self.LPF_reference.filter(reference_data)
            data = clean_data - clean_reference

        neutral_value = self.PkPk.get_pkpk(data)['neutral']
        return self.ema.get_ema(self.rectify(data - neutral_value))


print("End of EMG module")
