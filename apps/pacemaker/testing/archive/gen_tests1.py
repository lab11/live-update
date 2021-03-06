import random

def gen_normal_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('./tests/test_cases1.h', mode='w') as test_file:
        test_name = "NORMAL_HEART_TEST"
        test_trace = "N,0;"
        for _ in range(200):
            test_trace += "V" + "," + str(TLRI-TAVI-10) + ";"
            test_trace += "A" + "," + str(TAVI-15) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia7_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('./tests/test_cases1.h', mode='a') as test_file:
        test_name = "ARR7_HEART_TEST"
        test_trace = "N,0;"
        for _ in range(350):
            test_trace += "N" + "," + str(TLRI-TAVI-10) + ";"
            test_trace += "A" + "," + str(TAVI-15) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia8_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('./tests/test_cases1.h', mode='a') as test_file:
        test_name = "ARR8_HEART_TEST"
        test_trace = "N,0;"
        for _ in range(350):
            test_trace += "V" + "," + str(TLRI-TAVI) + ";"
            test_trace += "N" + "," + str(TAVI) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def gen_arrhythmia9_heart_test_str():
    TLRI = 1000
    TAVI = 150
    with open('./tests/test_cases1.h', mode='a') as test_file:
        test_name = "ARR9_HEART_TEST"
        test_trace = "N,0;"
        for _ in range(350):
            test_trace += "N" + "," + str(TLRI-TAVI) + ";"
            test_trace += "N" + "," + str(TAVI) + ";"
        test_file.write("char " + test_name + "[] = \"" + test_trace + "\";")
        test_file.write("\n")

def main():
    gen_normal_heart_test_str()
    gen_arrhythmia7_heart_test_str()
    gen_arrhythmia8_heart_test_str()
    gen_arrhythmia9_heart_test_str()

if __name__ == "__main__":
    main()
