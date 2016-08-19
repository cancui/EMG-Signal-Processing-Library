import csv
import os.path
import matplotlib.pyplot as plt
import Python.Filters.emg as emg

plt.style.use("ggplot")

# Define the sample frequency as a global variable to make filtering consistent between filter methods, as well as
# allow a labeled x-axis for the plot
sample_frequency = 1000

# Open the sample emg signal file
basepath = os.path.dirname(__file__)
filepath = os.path.abspath(os.path.join(basepath, '..', 'Sample Signals', 'emg_values1.csv'))
inputFile = open(filepath)
inputReader = csv.reader(inputFile)

# Set up the list of filters to test out
filters = []
filters.append(emg.EMGFilterBasicEMA(sample_frequency=1000))
filters.append(emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.75))
filters.append(emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.85))
filters.append(emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.9))
filters.append(emg.EMGFilterBasic(sample_frequency=1000))

# EMA_filter_05 = emg.EMGFilterBasicEMA(sample_frequency=1000)
# EMA_filter_075 = emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.75)
# EMA_filter_085 = emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.85)
# EMA_filter_09 = emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.9)
# MA_filter = emg.EMGFilterBasic(sample_frequency=1000)


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

print("Filtered data has been saved")

# values_to_plot = []
# for row in inputReader:
#     values_to_plot.append(float(row[0]))

# x_axis = [x/sample_frequency for x in range(len(values_to_plot))]
x_axis = [x/sample_frequency for x in range(len(values_to_plot[0]))]

num_plots = len(values_to_plot)

fig, axarr = plt.subplots(num_plots, sharex=True)

for i in range(num_plots):
    axarr[i].set_title()
    axarr[i].plot(x_axis, values_to_plot[i])
    axarr[i].set_xlim([0.1, 7.5])
    axarr[i].set_ylim([0, 700])

fig.suptitle('EMG Filter Comparison', fontsize=36, fontweight='bold')

axarr[num_plots-1].set_xlabel('Time (s)')
fig.set_size_inches(20, num_plots*5)
fig.savefig("figure.png", bbox_inches='tight', dpi=100)

print("Plot is finished and saved")