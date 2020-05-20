from datetime import datetime

SIZE = 4152
type_dict = {'BG Check': 'BG_Check', 'Temp Basal': 'Temp_Basal', 'Correction Bolus': 'Correction_Bolus', 'Scheduled Basal': 'Scheduled_Basal'}
init_time = None

def parse_value(value):
    try:
        v =  float(value)
        if int(v) == v:
            return str(int(v))
        return str(v)
    except ValueError:
        return '0'

output = open('data.h','w')

output.write('#include <time.h>\n')
output.write('#define SIZE ' + str(SIZE) + '\n')
i = 0
for k in type_dict:
    output.write('#define ' + type_dict[k] + ' ' + str(i) + '\n')
    i += 1
output.write('struct Data {\n')
output.write('time_t time; int type; float insulin, duration, rate, glucose;')
output.write('};\n')

output.write('struct Data data[SIZE] = {\n')
init_time = None

with open('combined_data_edited.csv') as file_in:
    for line in file_in:
        parsed_info = line.split(',')
        datetime_object = datetime.strptime(parsed_info[0], '%Y-%m-%dT%H:%M:%SZ')
        if not init_time:
            init_time = datetime_object
        curr_diff = str((datetime_object - init_time).seconds)
        curr_type = type_dict[parsed_info[1]]
        curr_insulin = parse_value(parsed_info[2])
        curr_duration = parse_value(parsed_info[3])
        curr_rate = parse_value(parsed_info[4])
        curr_glucose = parse_value(parsed_info[5])
        output.write('{' + ','.join((curr_diff, curr_type, curr_insulin, curr_duration, curr_rate, curr_glucose)) + '}, \n')

output.write('};')
output.close()
