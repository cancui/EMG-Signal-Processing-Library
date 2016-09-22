import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import csv
plt.style.use('seaborn-colorblind')

inputFile = open("emg_raw_values.csv")
inputReader = csv.reader(inputFile)
x_values = []
for row in inputReader:
    x_values.append(float(row[0]))
mu = np.mean(x_values)
sigma = np.std(x_values)

# the histogram of the data
n, bins, patches = plt.hist(x_values, 100, normed=True)

# add a 'best fit' line
y = mlab.normpdf(bins, mu, sigma)
l = plt.plot(bins, y, linewidth=1.5)

plt.xlabel('Intensity of Signal')
plt.ylabel('Probability')
plt.title("Distribution of Data Collected From an EMG Sensor")
plt.grid(True)

plt.savefig("normality_test.png", dpi=300)
