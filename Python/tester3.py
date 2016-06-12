import emg

f = emg.EMG_filter()

print f.sample_frequency
print f.range_
print f.scope
print f.sum_movingAvg
print f.val_movingAvg
print f.movingAvg
print f.log
print f.min_EMG_frequency
print f.max_EMG_frequency
print f.for_pkpk
print "!!!!!!!"
print f.max_pk_gap

for i in range(300):
	print f.filter(i)
		

print f.log
