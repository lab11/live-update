import numpy as np
import matplotlib.pyplot as plt 
import csv
import sys

filename = sys.argv[1]
savename = sys.argv[2]

# Reading <data>.txt file
data = []

with open(filename) as textfile:
	line = True
	while (line):
		line = textfile.readline()
		if (line[:3] == "Res"):
			data += [float(line[10:-5])]

print(data)

indata = input("Write to CSV? (y/n) ")
if (indata == 'n'):
	exit()

# Write data into a csv file

with open(savename, mode='w') as csvfile:
	file_writer = csv.writer(csvfile, delimiter=',')
	file_writer.writerow(data)
