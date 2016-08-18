from __future__ import print_function, division
from collections import deque


class MovingAverage(object):
    """
    Encapsulation of a key function that some filters require
    Note: MA stands for moving average
    """
    def __init__(self, length, return_int=False):
        self.data = deque([])
        self.data_sum = -1
        self.length = length
        self.value = -1
        self.return_int = return_int

    def get_ma(self, data):
        self.data.appendleft(data)
        self.data_sum += data

        if len(self.data) > self.length:
            self.data_sum -= self.data.pop()

        if self.return_int:
            self.value = self.data_sum // self.length  # preserves integer form
        else:
            self.value = 1.0 * self.data_sum / self.length

        if len(self.data) < (self.length / 2):
            return -1
        else:
            return self.value


class ExpMovingAverage(object):
    """
    An exponential moving average filter with variable exponentiating factors.
    Does not return an exponential moving average while the log is being populated at first (it would be too
    computationally expensive calculate the sum for each step to the end of the frame in order to have the weighting
    factors sum to 1)
    Note: As above, MA stands for moving average, and EMA stands for Exponential Moving Average
    """
    def __init__(self, exponential_factor=0.5, frame_length=50):
        """
        expFactor is what each term is multiplied by each time it is shift right in the current frame
        window is the number of samples that are taken into account when calculating the EMA
        In order for this to work, two conditions must be met:
        0 < expFactor < 1
        1 < window
        """
        if 0 < exponential_factor < 1:
            self.expFactor = exponential_factor
        else:
            raise ValueError("expFactor must be between 0 and 1")
        if frame_length <= 1:
            raise ValueError("window must be greater than 1")
        self.window = int(frame_length)  # Number of samples that are factored into calculation
        self.sumEMA = 0  # Initialize the sum of the EMA as zero
        self.EMA = deque([], maxlen=self.window)  # Initialize an empty deque iterable to contain the EMA calculated
        # for the past
        self.log = deque([], maxlen=self.window)  # Initialize an empty deque iterable of the unfiltered datapoints
        self.finalFactor = self.expFactor ** self.window  # Precomputed weighting factor applied to the last entry in the
        # EMA sum
        self.weightFactor = sum(self.expFactor ** x for x in range(1, self.window)) * 1.0  # Value sumEMA is divided
        # by so that sum of EMA weights is normalized to 1

    def get_ema(self, data_point):
        """
        Calculates the exponential moving average for the most recent points
        """
        if len(self.log) < self.window - 1:
            # If the log will not be as long as the full window after the new dataPoint is added, no EMA will be
            # calculated or returned
            self.sumEMA = self.expFactor * (data_point + self.sumEMA)
            self.log.appendleft(data_point)
            return
        else:
            self.sumEMA = ((self.sumEMA - self.finalFactor * self.log[-1]) + data_point) * self.expFactor
            self.log.appendleft(data_point)
            return_value = self.sumEMA / self.weightFactor
            self.EMA.appendleft(return_value)
            return return_value

    def pop_log(self, num_to_pop_ema=1):
        """
        Since you can't slice in a deque list a list, when popping more than one entries in a list you must repeatedly
        call the pop deque method.
        """
        if num_to_pop_ema == 1:
            return self.log.pop()
        to_return = []
        for _ in range(min(num_to_pop_ema, len(self.log))):
            to_return.append(self.log.pop())
        return to_return


class LowPassFilter(MovingAverage):
    """
    Simplifies the creation of a moving average-based low pass filter
    """

    def __init__(self, cutoff_frequency, sample_frequency, return_int=False):
        length = int(0.125 * sample_frequency / cutoff_frequency)
        MovingAverage.__init__(self, length, return_int=return_int)

    def filter(self, to_filter):
        if self.length < 2:
            return to_filter
        else:
            return self.get_ma(to_filter)


class BasicStats(object):
    def __init__(self, length):
        self.length = length
        self.data_points = deque([])
        self.total_sum = -1
        self.average = -1
        self.stddev = -1

    def add_data(self, data):
        self.data_points.appendleft(data)
        self.total_sum += data

        if len(self.data_points) > self.length:
            self.total_sum -= self.data_points.pop()

    def get_average(self, data):
        self.add_data(data)
        self.average = 1.0 * self.total_sum / len(self.data_points)
        return self.average


class PeakToPeak(object):
    """
    Peak to peak filter
    """

    def __init__(self, sample_frequency, min_frequency, max_frequency):
        self.for_pkpk = deque([])
        self.min_frequency = min_frequency
        self.max_frequency = max_frequency
        self.min_pk_gap = sample_frequency / max_frequency
        self.max_pk_gap = sample_frequency / min_frequency

        # self.neutral = -1                                                   #signal is shifted so that point point is zero
        # self.min_EMG_frequency = min_EMG_frequency                          #signals below this frequency do not influence the peak to peak measurements
        # self.max_EMG_frequency = max_EMG_frequency                          #signals above this frequency do not influence the peak to peak measurements
        # self.min_pk_gap = 1.0/self.max_EMG_frequency * sample_frequency     #the minimun distance in data points that two registered peaks can be
        # self.max_pk_gap = 1.0/self.min_EMG_frequency * sample_frequency     #the maximum distance two consecutive peaks can be without the calculated neutral-point shifting significantly

        # self.pk_indices = deque([])
        # self.pk_is_convex = deque([])

    def get_pkpk(self, data):
        self.for_pkpk.appendleft(data)

        # discards any data beyond two periods of the lowest-frequency wave
        if len(self.for_pkpk) > (self.max_pk_gap * 2):
            self.for_pkpk.pop()

        highest = max(self.for_pkpk)
        lowest = min(self.for_pkpk)
        self.neutral = (highest + lowest) / 2

        to_return = {'max': highest, 'min': lowest, 'pkpk': highest - lowest, 'neutral': self.neutral}

        if len(self.for_pkpk) < self.min_pk_gap * 2:
            return {'max': -1, 'min': -1, 'pkpk': -1, 'neutral': -1}
        else:
            return to_return

    def find_peaks(self):
        # cannot find another peak of oposite concavity until min_pk_gap data points past?
        pass

    def advanced_get_pkpk(self):
        pass


print("End")
