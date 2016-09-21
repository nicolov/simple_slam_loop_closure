#!/usr/bin/env python

import os
import sys
from IPython import embed

import matplotlib
matplotlib.use('GTKAgg')

import seaborn as sns
import matplotlib.pyplot as plt

import scipy.io as sio
import numpy as np

GROUND_TRUTH_PATH = os.path.expanduser(
    '~/bags/IJRR_2008_Dataset/Data/NewCollege/masks/NewCollegeGroundTruth.mat')

WORK_FOLDER = os.path.expanduser(
    '~/dev/simple_slam_loop_closure/out/')

if __name__ == "__main__":

    default_heatmap_kwargs = dict(
        xticklabels=False,
        yticklabels=False,
        square=True,
        cbar=False,)

    fig, (ax1, ax2) = plt.subplots(ncols=2)

    # Plot the ground truth
    gt_data = sio.loadmat(GROUND_TRUTH_PATH)['truth']
    sns.heatmap(gt_data[::2, ::2],
        ax=ax1,
        **default_heatmap_kwargs)
    ax1.set_title('Ground truth')

    # Plot the BoW results
    bow_data = np.loadtxt(os.path.join(
        WORK_FOLDER, 'confusion_matrix.txt'))
    # Take the lower triangle only
    bow_data = np.tril(bow_data, 0)
    sns.heatmap(bow_data,
        ax=ax2,
        vmax=0.2,
        **default_heatmap_kwargs)
    ax2.set_title('SURF + BoW')

    # plt.show()
    plt.tight_layout()
    plt.savefig(os.path.join(
        WORK_FOLDER, 'confusion_matrix.png'),
        bbox_inches='tight')
