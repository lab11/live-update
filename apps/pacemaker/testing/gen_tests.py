# import csv
import random
# def gen_normal_heart_test_csv():
#     with open('tests/normal_heart_test.csv', mode='w') as test_file:
#         test_file_writer = csv.writer(test_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

#         TLRI = 1000
#         TAVI = 150
#         for i in range(500):
#             test_file_writer.writerow(['V', "" + str(TLRI-TAVI)])
#             test_file_writer.writerow(['A', "" + str(TAVI)])

def gen_normal_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('test_cases1.h', mode='a') as test_file:
        test_name = "NORMAL_HEART_TEST"
        test_trace = "NONE,0;"
        for _ in range(50):
            test_trace += "V" + "," + str(TLRI-TAVI) + ";"
            test_trace += "A" + "," + str(TAVI) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia1_heart_test_str():
    TLRI = 1000
    TAVI = 100
    with open('test_cases1.h', mode='a') as test_file:
        test_name = "ARR1_HEART_TEST"
        test_trace = "NONE,0;"
        for _ in range(50):
            test_trace += "V" + "," + str(TLRI-TAVI) + ";"
            test_trace += "A" + "," + str(TAVI) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia2_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('test_cases1.h', mode='a') as test_file:
        test_name = "ARR2_HEART_TEST"
        test_trace = "NONE,0;"
        for _ in range(50):
            rand_val = random.randint(-10,10)
            test_trace += "V" + "," + str(TLRI-TAVI+rand_val) + ";"
            rand_val = random.randint(-10,10)
            test_trace += "A" + "," + str(TAVI+rand_val) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia3_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('test_cases1.h', mode='a') as test_file:
        test_name = "ARR3_HEART_TEST"
        test_trace = "NONE,0;"
        for _ in range(50):
            rand_val = random.randint(0,10)
            test_trace += "V" + "," + str(TLRI-TAVI+rand_val) + ";"
            rand_val = random.randint(0,10)
            test_trace += "A" + "," + str(TAVI+rand_val) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia4_heart_test_str():
    TLRI = 1000
    TAVI = 100
    with open('test_cases1.h', mode='a') as test_file:
        test_name = "ARR4_HEART_TEST"
        test_trace = "NONE,0;"
        for _ in range(50):
            test_trace += "V" + "," + str(TLRI-TAVI) + ";"
            test_trace += "A" + "," + str(TAVI+100) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia5_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('test_cases1.h', mode='a') as test_file:
        test_name = "ARR5_HEART_TEST"
        test_trace = "NONE,0;"
        for _ in range(250):
            test_trace += "NONE" + "," + str(TLRI-TAVI) + ";"
            test_trace += "A" + "," + str(TAVI) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

# def gen_sample_heart_test_str():
#     with open('test_cases1.h', mode='w') as test_file:
#         test_name = "SAMPLE_HEART_TEST_cum"
#         test_trace = ""
#         for _ in range(50):
#             test_trace += "V" + "," + str(1000) + ";"
#             test_trace += "A" + "," + str(1000) + ";"
#         test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")


# gen_normal_heart_test_str()
# gen_arrhythmia1_heart_test_str()
# gen_arrhythmia2_heart_test_str()
# gen_arrhythmia3_heart_test_str()
# gen_arrhythmia4_heart_test_str()
gen_arrhythmia5_heart_test_str()