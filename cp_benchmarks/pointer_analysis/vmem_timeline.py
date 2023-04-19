import pandas as pd
import re
import numpy as np 
from itertools import islice
def replace_whitespace(string):
    return re.sub(r'\s+', ' ', string)

# read the files
file1 = open('graph_mem_trace', 'r')
file2 = open('graph_pagefault_trace', 'r')

# Create a timeline of all virtual addresses.
# Each timeline contains loads where the value
# of the address was loaded into a register. 
# and a pagefault of the same address. 
# Time complexity O(loads) space O(pagefaults)

pagefault_trace = file2.readlines()
count = 0
vmem_timelines = dict()
prev_address = 0
for pagefault in pagefault_trace:
    
    trace = pagefault.split()
    i = trace[6]
    time = trace[3]
    print(trace)
    address = int(i[8:], 16)
    # Major pagefaults might show up twice in the trace.
    if address == prev_address:
        continue
    row = []
    row.append(time[:time.index('.')])
    row.append(time[time.index('.') + 1: time.index(':')])
    row.append(1)
    if address in vmem_timelines:
       vmem_timelines[address].append(row)
    else:
        vmem_timelines[address] = []
        vmem_timelines[address].append(row)
    prev_address = address
    count += 1
 
#mem_trace = file1.readlines()
count=0
for trace in file1:
    address = '0x0'
    if '.' not in trace:
        continue

    if 'Reg' in trace:
        address1 = trace[trace.index('Reg') + 6 :-1]
    #elif 'R' in trace:
    #    address = trace[trace.index('R') + 4:-1]
    else:
        continue
    address = int(address1, 16) & ~((1<<12) - 1)
    if int(address) in vmem_timelines:
        sec = trace[:trace.index('.')]
        nsec = trace[trace.index('.') + 1: trace.index(' ')]
        row = []
        row.append(sec)
        row.append(nsec)
        row.append(0)
        vmem_timelines[int(address)].append(row)

print("Starting analysis on timeline")
   
n = 100
preceeding_same_second=0
not_preeceding = 0
for idx, (k,v) in enumerate(vmem_timelines.items()):
    sorted_timeline = sorted(v, key = lambda x: (x[0], x[1]))
    #Assume pagefault can't be first value, that should not be possible.
    for i in range(len(sorted_timeline)):
        if sorted_timeline[i][2] == 1: 
            if sorted_timeline[i-1][2] == 0 and (int(sorted_timeline[i-1][0]) == int(sorted_timeline[i][0]) or int(sorted_timeline[i-1][0]) == int(sorted_timeline[i][0]) - 1 or int(sorted_timeline[i-1][0]) == int(sorted_timeline[i][0]) + 1):
                preceeding_same_second += 1
            else:
                not_preeceding += 1
print(preceeding_same_second, not_preeceding)
