import csv
import datetime
import os.path
import post_filter_data_normalization as data_norm
import matplotlib.pyplot as plt
import Python.Filters.emg as emg

plt.style.use("ggplot")

# Define the sample frequency as a global variable to make filtering consistent between filter methods, as well as
# allow a labeled x-axis for the plot
sample_frequency = 1000

# Open the sample emg signal file
basepath = os.path.dirname(__file__)
filepath = os.path.abspath(os.path.join(basepath, '..', '..', 'Sample Signals', 'emg_raw_values.csv'))
inputFile = open(filepath)
inputReader = csv.reader(inputFile)

# Set up the list of filters to test out
filters = []
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.5))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.75))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.9))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.925))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.95))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.975))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.985))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.99))
filters.append(emg.EMGFilterEMA(sample_frequency=1000, exponential_factor_=0.999))
filters.append(emg.EMGFilter(sample_frequency=1000))

filter_names = ["Unfiltered Data", "EMA 0.5", "EMA 0.75", "EMA 0.9", "EMA 0.925", "EMA 0.95", "EMA 0.975", "EMA 0.985",
                "EMA 0.99", "EMA 0.999", "MA"]


def call_filter(filter_object, unfiltered_value):
    return_value_ = filter_object.filter(unfiltered_value)
    return return_value_


def filtered_values(filters, unfiltered_value):
    """
    Takes a list of EMG filter objects from filter parameter, passes the latest value from the unfiltered data into the
    objects filter method, and then returns a list of the return values of all of the filters. The first entry in the
    list returned is the unfiltered value so that it can be compared to the original value
    """
    return_value = [call_filter(filters[x], unfiltered_value) for x in range(len(filters))]
    return_value.insert(0, unfiltered_value)
    return return_value

# Create list of the values being filtered so they can be plotted
values_to_plot = []
for row in inputReader:
    value = float(row[0])
    values_to_plot.append(filtered_values(filters, value))
# Transpose the values to be plotted
values_to_plot = list(zip(*values_to_plot))

print("Filtered data has been successfully input")

# Trim first 100 entries from file as the moving average based filters are building up a window, and then normalize the
# data
for count, dataset in enumerate(values_to_plot):
    dataset = dataset[100:]
    values_to_plot[count] = data_norm.normalize(dataset)

print("Data has been trimmed and normalized")

x_axis = [x/sample_frequency for x in range(len(values_to_plot[0]))]

num_plots = len(values_to_plot)

fig, axarr = plt.subplots(num_plots, sharex=True)

for i in range(num_plots):
    axarr[i].set_title(filter_names[i], fontsize=24)
    axarr[i].scatter(x_axis, values_to_plot[i], s=125)
    axarr[i].set_xlim([0, 7.5])
    axarr[i].set_ylim([-4, 4])

fig.suptitle('EMG Filter Comparison', fontsize=36, fontweight='bold')

axarr[num_plots-1].set_xlabel('Time (s)')
fig.set_size_inches(80, num_plots*5)
figure_path = os.path.abspath(os.path.join(basepath, '..', '..', 'Sample Signals', 'Plotted Filters',
                                           'Filter Comparison ' + str(datetime.datetime.now()) + '.png'))
fig.savefig(figure_path, bbox_inches='tight', dpi=100)

print("Plot is finished and saved")
