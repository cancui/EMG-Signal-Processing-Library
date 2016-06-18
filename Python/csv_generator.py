import os, time, sys

def create_output_name(num = 1):
	potential_filename = "processed_signal" + str(num) + ".csv"
	if os.path.isfile(potential_filename):
		potential_filename = create_output_name(num + 1)
	return potential_filename 

def export(log):
	filename = create_output_name()
	file = open(filename, 'w')

	file.write("Created" + str(time.datetime.now()))

	row = 0
	for datapoint in log:
		file.write(str(row) + ", " + str(datapoint))
		row += 1

	file.close()
	print "CSV file created as", filename

def main(argv):
	export(argv[0])

	print "CSV script finished"


'''
if __name__ is "__main__":
	main(sys.argv[1:])
'''
