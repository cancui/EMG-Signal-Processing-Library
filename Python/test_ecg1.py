import ecg
from pprint import pprint
from random import randint

f = ecg.ECG(sample_frequency = 200)

pprint(vars(f))

signal = []

for beats in range(15):
	for data_points in range(180):
		#add very stable data, but random
		signal.append(randint(200,210))
	for extra_data in range(randint(1,10)):
		#add very stable data, but random
		signal.append(randint(200,210))
	#add obvious peak data (one high, one low)
	signal.append(randint(400,410))
	signal.append(randint(1,10))

'''
for i in range (551):
	signal.append(randint(50,60))
'''
for point in signal:
	f.get_BPM(point)
	print f.BPM

print "Average peak to peak: ", f.average_pkpk
print "Average beats per minute: ", f.BPM

print "end of test2.py"