#Simple filter class assumes no low-frequency noise and data centred at 0. More complex filteres inherit from this.
class EMG_filter_basic(object):

    def __init__(self, sample_frequency = 200, range_ = 0.5):
        self.sample_frequency = sample_frequency               #in Hz
        self.range_ = range_                                   #in seconds
        self.scope = 1.0 * self.sample_frequency * self.range_       #in number of samples, limits the length of movingAvg    
        self.sum_movingAvg = 0                                 #tracks the sum of the moving average
        self.val_movingAvg = -1                                #the latest moving average value
        self.movingAvg = []                                    #used to store the datapoints for taking a moving average
        self.log = []                                          #used to store filtered data

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

    #takes the moving average within scope variable, or all of the data if that is less than scope
    def get_movingAvg(self, data):
        self.movingAvg.insert(0, data)
        self.sum_movingAvg += data

        if len(self.movingAvg) > self.scope:
            self.sum_movingAvg -= self.movingAvg.pop()

        self.val_movingAvg = self.sum_movingAvg / len(self.movingAvg)

        if len(self.movingAvg) < self.scope/2:
            return -1
        else:
            return self.val_movingAvg

    #this function is called to input raw data. It returns a filtered value if it has enough samples, and also logs it
    def filter(self, data):
        filtered_value = self.get_movingAvg(self.rectify(data))
        self.log_data(filtered_value)

        return filtered_value

    def test_print(self):
        print "TEST PRINT"

#extension of EMG_filter_basic that has tools to remove low-frequency noise and normalize the data
class EMG_filter(EMG_filter_basic):

    def __init__(self, sample_frequency = 200, range_ = 0.5, min_EMG_frequency = 25, max_EMG_frequency = 150):
        EMG_filter_basic.__init__(self, sample_frequency = sample_frequency, range_ = range_)

        self.for_pkpk = []

        self.neutral = -1                                                   #signal is shifted so that point point is zero
        self.min_EMG_frequency = min_EMG_frequency                          #signals below this frequency do not influence the peak to peak measurements
        self.max_EMG_frequency = max_EMG_frequency                          #signals above this frequency do not influence the peak to peak measurements
        self.min_pk_gap = 1.0/self.max_EMG_frequency * self.sample_frequency  #the minimun distance in data points that two registered peaks can be 
        self.max_pk_gap = 1.0/self.min_EMG_frequency * self.sample_frequency  #the maximum distance two consecutive peaks can be without the calculated neutral-point shifting significantly 
        self.pk_indices = [] 
        self.pk_is_convex = []
        #self.pkpk_stored = 10

    #returns dictionary of max, min, difference, and neutral values
    def get_pkpk(self, data):
        self.for_pkpk.insert(0, data)

        #discards any data beyond two periods of the lowest-frequency wave
        if len(self.for_pkpk) > (self.max_pk_gap * 2): 
            self.for_pkpk.pop()

        highest = max(self.for_pkpk)
        lowest = min(self.for_pkpk)
        self.neutral = (highest + lowest)/2

        to_return = {'max' : highest, 'min' : lowest, 'pkpk' : highest - lowest, 'neutral' : self.neutral}

        if len(self.for_pkpk) < self.min_pk_gap * 2:
            return {'max' : -1, 'min' : -1, 'pkpk' : -1, 'neutral' : -1}
        else:
            return to_return


    def find_peaks(self):
        #cannot find another peak of oposite concavity until min_pk_gap data points past? 
        pass

    def advanced_get_pkpk(self):
        pass

    #this function is called to input raw data and return a filtered value, accounting for low-frequency noise and un-normalized data
    def filter(self, data):
        pkpk = self.get_pkpk(data)
        #print pkpk
        return super(EMG_filter, self).filter(data - pkpk['neutral'])
        #return super(EMG_filter, self).filter(data)

    def test_print(self):
        print "TEST PRINT 222222"

    def test_print2(self):
        super(EMG_filter, self).test_print()

print "End of file"