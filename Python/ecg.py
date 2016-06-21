import signal_utilities as su 

class ECG(object):
	def __init__(self, sample_frequency = 200, range_ = 0.1, reference_available = False):
		self.sample_frequency = sample_frequency
		self.range_ = range_
		self.reference_available = reference_available
		self.initialization_period = self.sample_frequency * 3

		self.time_between_beats = [] #sized two or three, averaged to get BPM
		self.beats = su.PkPk(sample_frequency = sample_frequency, min_frequency = 5, max_frequency = 20)
		self.BPM = 0

		self.data_received = 0
		self.initialization_data = su.Basic_Stats(self.initialization_period)
		self.average_pkpk = -1

	def initialize(self, data):
		current_pkpk = self.beats.get_pkpk(data)['pkpk']
		self.average_pkpk = self.initialization_data.get_average(current_pkpk)

	def get_BPM(self, data):

		if self.data_received < self.initialization_period:
			initialize(data)
			self.data_received += 1
		else:
			current_pkpk = self.beats.get_pkpk(data)['pkpk']
			if current_pkpk > average_pkpk * 1.5:
				#beat detected, disable detection for next ... seconds (fraction of time between most recent beat intervals)
