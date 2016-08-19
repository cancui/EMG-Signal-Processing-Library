import numpy as np


def normalize_mean(dataset):
    """
    Modifies a dataset so that the mean is 0.  Dataset is a list, return value is a numpy array.
    """
    normalized_dataset = np.array(dataset)
    return normalized_dataset - np.mean(normalized_dataset)


def normalize_standard_deviation(dataset):
    """
    Modifies a dataset so that the standard deviation is 1, so that 99.7% of data should be between -3 and 3,
    assuming that normalize_mean has been called first.  Dataset is assumed to be a numpy array since the mean should
    be normalized before the standard deviation is.  Return value is a numpy array.
    """
    return dataset*(1/np.std(dataset))


def normalize(dataset):
    """
    Normalizes a dataset's mean (makes it equal to zero), normalizes the standard deviation (sets it equal to one)
    Dataset is assumed to be an interable, and the return value is a numpy array
    """
    return normalize_standard_deviation(normalize_mean(dataset))