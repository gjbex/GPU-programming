#!/usr/bin/env python

import argparse
import matplotlib.pyplot as plt
import numpy as np

def main() :
    parser = argparse.ArgumentParser()
    parser.add_argument('data', type=str,
                        help='Path to the data file')
    args = parser.parse_args()

    data = np.genfromtxt(args.data, delimiter=' ', names=True)
    figure, axes = plt.subplots(nrows=3, ncols=2, figsize=(10, 10))
    # kinetic energy
    axes[0, 0].plot(data['time'], data['kinetic_energy'])
    axes[0, 0].set_yscale('log')
    axes[0, 0].set_title('Kinetic energy')
    # potential energy
    axes[0, 1].plot(data['time'], np.abs(data['potential_energy']))
    axes[0, 1].set_yscale('log')
    axes[0, 1].set_title('Potential energy')
    # distance statistics
    axes[1, 0].plot(data['time'], data['dist_min'], label='min')
    axes[1, 0].plot(data['time'], data['dist_median'], label='median')
    axes[1, 0].plot(data['time'], data['dist_max'], label='max')
    axes[1, 0].set_yscale('log')
    axes[1, 0].legend()
    axes[1, 0].set_title('Distance statistics')
    # velocity statistics
    axes[1, 1].plot(data['time'], data['vel_min'], label='min')
    axes[1, 1].plot(data['time'], data['vel_median'], label='median')
    axes[1, 1].plot(data['time'], data['vel_max'], label='max')
    axes[1, 1].set_yscale('log')
    axes[1, 1].legend()
    axes[1, 1].set_title('Velocity statistics')
    # force statistics
    axes[2, 0].plot(data['time'], np.abs(data['force_min']), label='min')
    axes[2, 0].plot(data['time'], np.abs(data['force_median']), label='median')
    axes[2, 0].plot(data['time'], np.abs(data['force_max']), label='max')
    axes[2, 0].set_yscale('log')
    axes[2, 0].legend()
    axes[2, 0].set_title('Force statistics')
    axes[2, 1].axis('off')
    figure.tight_layout()
    plt.show()

if __name__ == '__main__' :
    main()
