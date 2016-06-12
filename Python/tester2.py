import emg

f = emg.EMG_filter_basic()

a = [-5,3,-5,3,-5,3,-5,3,-5,3,-5,3,-5,3,-5,3]
b = [10, 20, 40, 80, 200, 500, 1500]

for item in a:
	f.filter(item)
for item in b:
	f.filter(item)

print f.log
