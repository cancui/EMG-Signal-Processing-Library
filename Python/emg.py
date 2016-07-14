import signal_utilities as su

#Simple filter class assumes no low-frequency noise and data centred at 0. More complex filteres inherit from this.
class EMG_filter_basic(object):
    def __init__(self, sample_frequency = 200, range_ = 0.1, reference_available = False):
        self.log = []                                          #used to store filtered data
        self.reference_available = reference_available
        self.MA = su.Moving_Average(length = sample_frequency * range_, return_int = True)

        if reference_available == True:
            self.LPF_data = su.LPF(cutoff_frequency = 150, sample_frequency = sample_frequency)
            self.LPF_reference = su.LPF(cutoff_frequency = 150, sample_frequency = sample_frequency)

    #input filtered data
    def log_data(self, to_log):
        self.log.insert(0, to_log)

    #pop and return either one data point or a set of data points
    def pop_log(self, num_to_pop = 1):
        if len(self.log) < 1 or num_to_pop > len(self.log):
            return
        elif num_to_pop <= 1:
            return self.log.pop()
        elif num_to_pop > 1 and num_to_pop <= len(self.log):
            to_return = self.log[-num_to_pop:]
            self.log = self.log[:-num_to_pop]
            return to_return

    #rectifies data, assuming low frequency noise already removed (reference point of 0)
    def rectify(self, to_rectify):
        return abs(to_rectify)

    #this function is called to input raw data. It returns a filtered value if it has enough samples, and also logs it
    def filter(self, data, reference_data = 0):
        if not self.reference_available:
            filtered_value = self.MA.get_movingAvg(self.rectify(data))
            self.log_data(filtered_value)
            return filtered_value

        else:
            clean_data = self.LPF_data.filter(data)
            clean_reference = self.LPF_reference.filter(reference_data)
            filtered_value = self.MA.get_movingAvg(self.rectify(clean_data - clean_reference))
            self.log_data(filtered_value)
            #print "Reference used"
            return filtered_value

#extension of EMG_filter_basic that has tools to remove low-frequency noise and normalize the data
class EMG_filter(EMG_filter_basic):

    def __init__(self, sample_frequency = 200, range_ = 0.5, min_EMG_frequency = 25, max_EMG_frequency = 150, reference_available = False):
        EMG_filter_basic.__init__(self, sample_frequency = sample_frequency, range_ = range_, reference_available = reference_available)
        #self.reference_available = reference_available
        self.PkPk = su.PkPk(sample_frequency = sample_frequency, min_frequency = min_EMG_frequency, max_frequency = max_EMG_frequency)

    #this function is called to input raw data and return a filtered value, accounting for low-frequency noise and un-normalized data
    def filter(self, data, reference_data = 0):
        if self.reference_available == True:
            clean_data = self.LPF_data.filter(data)
            clean_reference = self.LPF_reference.filter(reference_data)
            data = clean_data - clean_reference
        
        neutral_value = self.PkPk.get_pkpk(data)['neutral']
        filtered_value = self.MA.get_movingAvg(self.rectify(data - neutral_value))
        self.log_data(filtered_value)
        return filtered_value

        #return super(EMG_filter, self).filter(data - neutral_value)

#print "End of EMG module"
