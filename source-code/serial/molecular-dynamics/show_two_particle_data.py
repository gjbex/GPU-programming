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
    figure, axes = plt.subplots(nrows=3, ncols=3, figsize=(10, 10))
    # kinetic energy
    axes[0, 0].plot(data['time'], data['kinetic_energy'])
    axes[0, 0].set_title('Kinetic energy')
    # potential energy
    axes[0, 1].plot(data['time'], data['potential_energy'])
    axes[0, 1].set_title('Potential energy')
    axes[0, 2].plot(data['time'], data['kinetic_energy'] + data['potential_energy'])
    axes[0, 2].set_title('Total energy')
    # positions, distance, center of mass
    axes[1, 0].plot(data['time'], data['x_0'], label='$x_0$')
    axes[1, 0].plot(data['time'], data['x_1'], label='$x_1$')
    axes[1, 0].legend()
    axes[1, 0].set_title('Positions')
    axes[1, 1].plot(data['time'], np.abs(data['x_0'] - data['x_1']))
    axes[1, 1].set_title('Distance')
    axes[1, 2].plot(data['time'], 0.5*(data['x_0'] + data['x_1']))
    axes[1, 2].set_title('Center of mass')
    # velocity statistics
    axes[2, 0].plot(data['time'], data['v_0'], label='$v_0$')
    axes[2, 0].plot(data['time'], data['v_1'], label='$v_1$')
    axes[2, 0].legend()
    axes[2, 0].set_title('Velocities')
    # no more plots
    axes[2, 1].axis('off')
    axes[2, 2].axis('off')
    figure.tight_layout()
    plt.show()

if __name__ == '__main__' :
    main()
