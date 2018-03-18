from __future__ import print_function, division
import Python.Filters.signal_utilities as su


# PLAY AROUND WITH MIN_FREQUENCY UNTIL IT WORKS
class ECG(object):
    def __init__(self, sample_frequency=200, pkpk_threshold_ratio=2.0, reference_available=False,
                 autodetect_threshold=True):  # range_ = 0.1,
        self.sample_frequency = sample_frequency
        # self.range_ = range_
        self.reference_available = reference_available
        self.initialization_period = self.sample_frequency * 3

        self.samples_between_beats = su.BasicStats(length=3)  # sized two or three, averaged to get BPM
        # self.samples_between_beats.add_data(0)
        self.signal_tracker = su.PeakToPeak(sample_frequency=sample_frequency, min_frequency=self.sample_frequency / 20,
                                            max_frequency=self.sample_frequency / 8)  # PLAY AROUND WITH MIN_FREQUENCY UNTIL IT WORKS
        self.BPM = 0

        self.data_points_received = 0
        self.initialization_data = su.BasicStats(length=self.initialization_period)
        self.average_pkpk = -1
        self.pkpk_threshold_ratio = 2.0
        self.autodetect_ratio = 0.2
        self.data_samples_since_beat = 0
        self.first_beat = True
        # self.just_detected_beat = False #if true, prevents another beat from being detected for 1/2 of last beat-to-beat time

        self.autodetect_threshold = autodetect_threshold
        self.init_maxs = su.MovingAverage(length=sample_frequency / 40, return_int=True)
        init_maxs_average = 0;

    # self.init_mins = su.Moving_Average(length = sample_frequency / 40, return_int = True)
    # init_mins_average = 0;

    def initialize(self, data, reference_data=0):
        if self.reference_available == True:
            data = data - reference_data
        current_pkpk = self.signal_tracker.get_pkpk(data)['pkpk']
        if self.average_pkpk == -1:
            self.average_pkpk = current_pkpk
        else:
            self.average_pkpk = self.initialization_data.get_average(current_pkpk)

        if self.autodetect_threshold == True:
            if len(self.init_maxs.data) == 0 or data >= self.init_maxs.data[-1]:
                self.init_maxs_average = self.init_maxs.get_ma(data)

            # if len(self.init_mins.data) == 0 or data <= self.init_mins.data[-1]:
            #	self.init_mins_average = self.init_mins.get_ma(data)

            if self.data_points_received == self.initialization_period - 1:
                self.pkpk_threshold_ratio = self.autodetect_ratio * self.init_maxs_average / self.average_pkpk  # be able to set both thresholds
            # print("CALCULATED THRESHOLD: {}".format(self.pkpk_threshold_ratio))

    def get_bpm(self, data, reference_data=0):
        if self.reference_available == True:
            data = data - reference_data
        average_delay = 0
        if self.data_points_received < self.initialization_period:
            self.initialize(data)
            self.data_points_received += 1
        else:
            # print("not initializing anymore")
            current_pkpk = self.signal_tracker.get_pkpk(data)['pkpk']
            self.data_samples_since_beat += 1
            if (current_pkpk > self.average_pkpk * self.pkpk_threshold_ratio) and (
                    self.first_beat == True or self.data_samples_since_beat > 0.75 *
                self.samples_between_beats.data_points[0]):
                # self.just_detected_beat = True
                # if self.first_beat == True:
                #	self.samples_between_beats.add_data(self.data_samples_since_beat)
                #	self.first_beat = False

                average_delay = self.samples_between_beats.get_average(self.data_samples_since_beat)

                # beat detected, disable detection for next ... seconds (fraction of time between most recent beat intervals)
                if self.first_beat == True:
                    self.BPM = 60.0 * self.sample_frequency / self.data_samples_since_beat
                    self.first_beat = False
                else:
                    self.BPM = 60.0 * self.sample_frequency / average_delay

                self.data_samples_since_beat = 0

        return self.BPM

print("End of ECG module")
