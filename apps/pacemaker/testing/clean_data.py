# This function will take as input an output trace file.
# It will first remove the first and last lines of the file
# Then it will remove the "00> " from the beginning of each line
# Then it will remove any duplicate lines (two subsequent pacing events in a row, for example)
# Then it will compute the difference between subsequent 
import matplotlib.pyplot as plt
import numpy as np
import math
import os

dir_path = "./output_traces_new1/"

def clean(file_name):
    with open(dir_path+file_name, "r") as input:
        with open(dir_path + "/clean_"+file_name, "w") as output: 
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
    atrial_ventricle_intervals = []

    last_ventricle = None
    last_atrial = None

    with open(dir_path+file_name, "r") as input:
        for line in input:
            time = int(line[3:])
            if line[0] == 'V':
                if last_ventricle != None:
                    ventricle_intervals.append(abs(time - last_ventricle))
                last_ventricle = time
                if last_atrial != None:
                    ventricle_atrial_intervals.append(abs(last_ventricle-last_atrial))
            if line[0] == 'A':
                if last_atrial != None:
                    atrial_intervals.append(abs(time - last_atrial))
                last_atrial = time
                if last_ventricle != None:
                    atrial_ventricle_intervals.append(abs(last_atrial-last_ventricle))

    return ventricle_intervals, atrial_intervals, ventricle_atrial_intervals, atrial_ventricle_intervals

def reject_outliers_2(data, m=2.):
    d = np.abs(data - np.median(data))
    mdev = np.median(d)
    s = d / (mdev if mdev else 1.)
    return data[s < m]

def plot_histogram(trace, type, seen_intervals):
    # N_points = 100000
    # n_bins = 20

    fig, axs = plt.subplots(1, sharey=True, tight_layout=False)
    if type == "Ventricle" or type == "Atrial":
        axs.set_title('Measured Intervals Between Subsequent ' + type + ' Events', y=1.05)
    else:
        types = type.split("-")
        first = types[0]
        second = types[1]
        axs.set_title('Measured Intervals Between ' + first + ' and ' + second + ' Events', y=1.05)
    axs.set_ylabel('Count')
    axs.set_xlabel("Interval (microseconds)")

    # errors = np.abs(np.array(seen_intervals) - np.array(exp_intervals))
    errors = np.array(seen_intervals)
    print(trace + " " + type + " errors: ", errors)
    errors = reject_outliers_2(errors, 10)
    # print("erorrrs", errors)
    iqr = np.subtract(*np.percentile(errors, [75, 25]))
    # print("iqr", iqr)
    h = 2 * iqr /(len(errors) ** (1./3))
    n = math.ceil((max(errors) - min(errors))/h)
    # print("n", n)

    n = min(n,40)
    plt.hist(errors, color = "darkgray", bins=n)
    
    min_xlim, max_xlim = plt.xlim()
    min_ylim, max_ylim = plt.ylim()

    axs.annotate('Mean: {:.2f} us'.format(np.mean(errors)), xy=(0.02, .97), xycoords='axes fraction', fontsize=10,
                horizontalalignment='left', verticalalignment='top')
    axs.annotate('Std Dev: {:.2f} us'.format(np.std(errors)), xy=(0.02, .93), xycoords='axes fraction', fontsize=10,
                horizontalalignment='left', verticalalignment='top')

    plt.axvline(np.mean(errors), color='k', linestyle='dashed', linewidth=1)

    # plt.show()
    if not os.path.exists(dir_path + "output_graphs"):
        os.makedirs(dir_path + "output_graphs")

    plt.savefig(dir_path + "output_graphs/"+type+"_"+trace+".png")


   
def main():
    # traces = ["normal_trace", "arr7_trace", "arr8_trace", "arr9_trace"]
    traces = ["arr7_trace", "arr8_trace", "arr9_trace"]

    for trace in traces:
        filename = trace + ".log"
        clean(filename)
        ventricle_intervals, atrial_intervals, ventricle_atrial_intervals, atrial_ventricle_intervals = compute_event_intervals("clean_"+filename)

        if trace in ("normal_trace", "arr9_trace"):
            plot_histogram(trace, "Ventricle", ventricle_intervals)
            plot_histogram(trace, "Atrial", atrial_intervals)
        elif trace in ("arr7_trace"):
            plot_histogram(trace, "Ventricle-Atrial", ventricle_atrial_intervals)
        elif trace in ("arr8_trace"):
            plot_histogram(trace, "Atrial-Ventricle", atrial_ventricle_intervals)


if __name__ == "__main__":
    main()