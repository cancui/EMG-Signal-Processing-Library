# EMG/ECG Signal Processing Library Python Documentation

- This is used to prototype new filter types and experiment with different parameters, such as frame length for moving average based filters
- Raw filters are implemented in the Filters/signal_utilities.py
- The filters are then combined together to work in emg.py and ecg.py, building off on another
- The library is compatible with Python 2 & 3
- The only dependencies for this project are Matplotlib for visualizing the effectiveness of filters, and Pytest for running unit tests

Creating a new filter:
----------------------

##### 1) Write the filter as a class in the signal_utilities.py file
##### 2) Test your filter, and verifying it performs correctly
##### 3) Implement your filter in the emg.py and ecg.py files
##### 4) Edit filter_prototype_test.py to test how your filter performs on raw EMG data compared to the pre-existing filters
##### 5) If your filter performs well, save the Matplotlib figure as *your_filter_name.png*


