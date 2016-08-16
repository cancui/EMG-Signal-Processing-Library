import csv
import Python.emg as emg

__author__ = 'Jeremy Malloch'

# Open an output file to write the filtered data to
outputFile = open('Sample Signals/ema_emgvalues.csv', 'w')
outputWriter = csv.writer(outputFile)
outputWriter.writerow(["Unfiltered Data", "EMA Filtered Data 0.5", "EMA Filtered Data 0.75", "EMA Filtered Data 0.85",
                       "EMA Filtered Data 0.90", "MA Filtered Data"])

# Open the sample emg signal file
inputFile = open('Sample Signals/emg_values1.csv')
inputReader = csv.reader(inputFile)

# Set up the EMG Filter using the Exponential moving average
EMA_filter_05 = emg.EMGFilterBasicEMA(sample_frequency=1000)
EMA_filter_075 = emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.75)
EMA_filter_085 = emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.85)
EMA_filter_09 = emg.EMGFilterBasicEMA(sample_frequency=1000, exponential_factor_=0.9)
MA_filter = emg.EMGFilterBasic(sample_frequency=1000)

def filtered_values(value):
   return  [value, EMA_filter_05.filter(value), EMA_filter_075.filter(value), EMA_filter_085.filter(value),
            EMA_filter_09.filter(value), MA_filter.filter(value)]

for count, row in enumerate(inputReader):
    value = float(row[0])
    filtered_values(value)
    if count > 100:
        outputWriter.writerow(filtered_values(value))