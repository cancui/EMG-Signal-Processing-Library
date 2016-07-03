#Encapsulation of a key function that some filters require
class Moving_Average(object):
    def __init__(self, length, return_int = False):
        self.data = []
        self.data_sum = -1
        #self.data_avg = -1
        self.length = length
        self.value = -1
        self.return_int = return_int

        #self.sample_frequency = sample_frequency               #in Hz
        #self.range_ = range_                                   #in seconds
        #self.scope = 1.0 * self.sample_frequency * self.range_       #in number of samples, limits the length of movingAvg    
        #self.sum_movingAvg = 0                                 #tracks the sum of the moving average
        #self.val_movingAvg = -1                                #the latest moving average value
        #self.movingAvg = []                                    #used to store the datapoints for taking a moving average

    def get_movingAvg (self, data):
        self.data.insert(0, data)
        self.data_sum += data

        if len(self.data) > self.length:
            self.data_sum -= self.data.pop()

        if self.return_int == True:
            self.value = int(self.data_sum / self.length) #preserves integer form
        else: 
            self.value = 1.0 * self.data_sum / self.length

        if len(self.data) < (self.length / 2):
            return -1
        else:
            return self.value

#Simplifies the creation of a moving average-based LPF 
class LPF(Moving_Average):
    def __init__(self, cutoff_frequency, sample_frequency, return_int = False):
        length = int(0.125 * sample_frequency / cutoff_frequency)
        Moving_Average.__init__(self, length, return_int = return_int)

    def filter(self, to_filter):
        if self.length < 2:
            return to_filter
        else:
            return self.get_movingAvg(to_filter) 

class Basic_Stats(object):
    def __init__(self, length):
        self.length = length
        self.data_points = []
        self.total_sum = -1
        self.average = -1
        self.stddev = -1
    
    def add_data(self, data):
        self.data_points.insert(0, data)
        self.total_sum += data

        if len(self.data_points) > self.length:
            self.total_sum -= self.data_points.pop()

    def get_average(self, data):
        self.add_data(data)
        self.average = 1.0 * self.total_sum / len(self.data_points)
        return self.average


class PkPk(object):
    def __init__(self, sample_frequency, min_frequency, max_frequency):
        self.for_pkpk = []
        self.min_frequency = min_frequency
        self.max_frequency = max_frequency
        self.min_pk_gap = sample_frequency / max_frequency
        self.max_pk_gap = sample_frequency / min_frequency

        #self.neutral = -1                                                   #signal is shifted so that point point is zero
        #self.min_EMG_frequency = min_EMG_frequency                          #signals below this frequency do not influence the peak to peak measurements
        #self.max_EMG_frequency = max_EMG_frequency                          #signals above this frequency do not influence the peak to peak measurements
        #self.min_pk_gap = 1.0/self.max_EMG_frequency * sample_frequency     #the minimun distance in data points that two registered peaks can be 
        #self.max_pk_gap = 1.0/self.min_EMG_frequency * sample_frequency     #the maximum distance two consecutive peaks can be without the calculated neutral-point shifting significantly 

        #self.pk_indices = [] 
        #self.pk_is_convex = []

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

print "End"