import emg

f = emg.EMG_filter_basic(20, 2)

print f.sample_frequency
print f.range_
print f.scope
print f.movingAvg
print f.log

print "Test Logging"

for i in range(100):
	f.filter(-i - 1)

print f.log
print f.pop_log()
print f.pop_log(5)
print f.log

for i in range(10):
	f.filter(i + 100)

print f.log
