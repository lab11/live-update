import csv

def gen_normal_heart_test_csv():
    with open('tests/normal_heart_test.csv', mode='w') as test_file:
        test_file_writer = csv.writer(test_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

        TLRI = 1000
        TAVI = 150
        for i in range(500):
            test_file_writer.writerow(['V', "" + str(TLRI-TAVI)])
            test_file_writer.writerow(['A', "" + str(TAVI)])

def gen_normal_heart_test_str():
    TLRI = 1000
    TAVI = 150
    time_elapsed = 0
    with open('test_cases1.h', mode='w') as test_file:
        test_name = "NORMAL_HEART_TEST_cum"
        test_trace = ""
        for _ in range(50):
            test_trace += "V" + "," + str(time_elapsed + TLRI-TAVI) + ";"
            time_elapsed = time_elapsed + TLRI-TAVI
            test_trace += "A" + "," + str(time_elapsed + TAVI) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")

def gen_arrhythmia_heart_test_str():
    TLRI = 1000
    TAVI = 200
    time_elapsed = 0
    with open('test_cases2.h', mode='w') as test_file:
        test_name = "ARRHYTHMIA_HEART_TEST"
        test_trace = ""
        for _ in range(50):
            test_trace += "V" + "," + str(time_elapsed + TLRI-TAVI) + ";"
            time_elapsed = time_elapsed + TLRI-TAVI
            test_trace += "A" + "," + str(time_elapsed + TAVI) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")

def gen_sample_heart_test_str():
    with open('test_cases1.h', mode='w') as test_file:
        test_name = "SAMPLE_HEART_TEST_cum"
        test_trace = ""
        for _ in range(50):
            test_trace += "V" + "," + str(1000) + ";"
            test_trace += "A" + "," + str(1000) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
# gen_normal_heart_test()
# gen_normal_heart_test_str()
# gen_arrhythmia_heart_test_str()
# gen_sample_heart_test_str()
gen_arrhythmia_heart_test_str()