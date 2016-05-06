#!/usr/bin/python
import os
from subprocess import check_output
import time

import numpy as np
import matplotlib.pyplot as plt

if not os.path.exists('./time_test_graphs'):
        os.makedirs('./time_test_graphs')

output = ''

N_series  = [50, 100, 1000, 10000, 50000, 100000]
NP_series = [1, 2, 3, 5, 10, 20]
experiment_n_min = 5

for idx, N in enumerate(N_series):
        results = []
        for NP in NP_series:
                times = []
                outputs = []
                for _ in xrange(max(50 / NP, experiment_n_min)): # More experiments for few NPsc

                        start = time.time()
                        output = check_output('mpirun -np %d ./e_calc %d' % (NP, N), shell=True)
                        end = time.time()
                        
                        outputs.append(float(output))
                        times.append((end - start) * 100) # Millisconds
                        
                results.append((NP, np.mean(times), np.std(times), np.mean(outputs)))

        # Plotting graphs
        fig, ax = plt.subplots()

        ax.errorbar(zip(*results)[0], zip(*results)[1], yerr=zip(*results)[2],
                    ecolor='Red', marker='o', mfc='Red', ms=5)
        ax.set_title('N = %d' % N)
        ax.set_xlabel('Number of processes')
        ax.set_ylabel('Mean time (ms)')
        ax.set_xticks(range(1, max(zip(*results)[0]) + 2))
        
        fig.savefig('./time_test_graphs/time_test_N_%d.png' % N)

        print 'Test #%d of %d: done.' % (idx + 1, len(N_series))
