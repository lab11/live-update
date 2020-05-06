# This function will take as input an output trace file.
# It will first remove the first and last lines of the file
# Then it will remove the "00> " from the beginning of each line
# Then it will remove any duplicate lines (two subsequent pacing events in a row, for example)
# Then it will compute the difference between subsequent 
import matplotlib.pyplot as plt
import numpy as np
import math

def clean(file_name):
    with open("./output_traces/"+file_name, "r") as input:
        with open("./output_traces/clean_"+file_name, "w") as output: 
            prev_event = None
            for line in input:
                if line.find("00> ") != -1:
                    line = line[3:]
                if "Initiating input trace..." in line or "Finished trace." in line:
                    continue
                event = line[:3]
                if event == prev_event:
                    continue
                else:
                    prev_event = event
                output.write(line.lstrip())

def compute_event_intervals(file_name):
    ventricle_intervals = []
    atrial_intervals = []
    ventricle_atrial_intervals = []

    last_ventricle = None
    last_atrial = None

    with open("./output_traces/"+file_name, "r") as input:
        for line in input:
            
            time = int(line[3:])
            if line[0] == 'V':
                if last_ventricle != None:
                    ventricle_intervals.append(time - last_ventricle)
                last_ventricle = time
            if line[0] == 'A':
                if last_atrial != None:
                    atrial_intervals.append(time - last_atrial)
                last_atrial = time
    # print("ventricle_intervals: ", ventricle_intervals)
    # print("atrial_intervals: ", atrial_intervals)
    return ventricle_intervals, atrial_intervals

def reject_outliers_2(data, m=2.):
    d = np.abs(data - np.median(data))
    mdev = np.median(d)
    s = d / (mdev if mdev else 1.)
    return data[s < m]

def plot_histogram(trace, type, seen_intervals, exp_intervals):
    # N_points = 100000
    # n_bins = 20

    fig, axs = plt.subplots(1, sharey=True, tight_layout=True)
    axs.set_ylabel('Count')
    axs.set_xlabel("Deviation from Expected Interval Between Subsequent " + type + " Events (microseconds)")

    errors = np.abs(np.array(seen_intervals) - np.array(exp_intervals))
    errors = reject_outliers_2(errors, 5)

    w = 15
    n = math.ceil((max(errors) - min(errors))/w)
    
    plt.hist(errors, bins=n)

    min_ylim, max_ylim = plt.ylim()
    plt.axvline(np.mean(errors), color='k', linestyle='dashed', linewidth=1)
    plt.text(np.mean(errors)*1.1, max_ylim*0.9, 'Mean: {:.2f}'.format(np.mean(errors)))
    # plt.show()
    plt.savefig("./output_graphs/"+type+"_"+trace+".png")


   
def main():
    traces = ["arr6_trace"]

    for trace in traces:
        filename = trace + ".log"
        clean(filename)
        ventricle_intervals, atrial_intervals = compute_event_intervals("clean_"+filename)
        exp_ventricle_intervals = [(ventricle_intervals[i]/10000)*10000 for i in range(len(ventricle_intervals))]
        exp_atrial_intervals = [(atrial_intervals[i]/10000)*10000 for i in range(len(atrial_intervals))]
        plot_histogram(trace, "ventricle", ventricle_intervals, exp_ventricle_intervals)
        # plot_histogram(trace, "atrial", ventricle_intervals, exp_ventricle_intervals)
        # plot_histogram(trace, "atrial_ventricle", ventricle_intervals, exp_ventricle_intervals)



if __name__ == "__main__":
    main()