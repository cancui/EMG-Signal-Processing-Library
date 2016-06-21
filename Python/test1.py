import csv_generator, emg
from pprint import pprint

#f = emg.EMG_filter_basic()
#pprint (vars(f))
#pprint (vars(f.f))

def get_list(filename):
	csv = open(filename, "r")
	csv_list = []

	for line in csv:
		csv_list.append(int(line))

	return csv_list


'''

m = emg.Moving_Average(8)

pprint(vars(m))



for i in range(100):
	print m.get_movingAvg(i)

print " "


l = emg.LPF(0.125, 8)

pprint(vars(l))

for i in range(100):
	print l.filter(i)

print " "

'''
'''
b = emg.EMG_filter(sample_frequency = 80, range_ = 0.1, reference_available = True)

for i in range(100):
	print b.filter(i, 0)

print " "

pprint(vars(b))
'''

f = emg.EMG_filter(1000, 1.5)#(sample_frequency = 500, range_ = 0.2)

data = get_list("emgvalues1.csv")

#data2 = [i for i in range (100)]

for item in data:
	f.filter(item)

#print f.log

csv_generator.export(f.log)







