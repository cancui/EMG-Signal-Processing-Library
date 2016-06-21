def get_list(filename):
	csv = open(filename, "r")
	csv_list = []

	for line in csv:
		csv_list.append(int(line))

	return csv_list

