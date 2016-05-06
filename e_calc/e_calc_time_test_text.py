#!/usr/bin/python
import os
from subprocess import check_output
import time

import numpy as np

if not os.path.exists('./time_test_graphs'):
        os.makedirs('./time_test_graphs')

output = ''

N_series  = [50, 100, 1000, 10000, 50000, 100000]
NP_series = [1, 2, 3, 5, 10, 20]
experiment_n_min = 5

for idx, N in enumerate(N_series):
        results = {'N': 0,
                   'measures': []}
        for NP in NP_series:
                times = []
                outputs = []
                for _ in xrange(max(50 / NP, experiment_n_min)): # More experiments for few NPsc

                        start = time.time()
                        output = check_output('mpirun -np %d ./e_calc %d' % (NP, N), shell=True)
                        end = time.time()
                        
                        outputs.append(float(output))
                        times.append((end - start) * 100) # Millisconds
                
                results['measures'].append((NP, np.mean(times), np.std(times), np.mean(outputs)))
        
        results['N'] = N

        with open('./time_test_graphs/results.txt', 'a') as f:
                f.write(str(results) + '\n')

        print 'Test #%d of %d: done.' % (idx + 1, len(N_series))
