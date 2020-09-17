import argparse
from copy import deepcopy
import json
import networkx as nx
import os
import pprint
import random
import re
import serial
import struct
import sys
import time

import binascii

from intelhex import IntelHex

MAGIC = 0xfdfdfdfd

'''
TEST_DATA = {
        'time': 0,
        'type': 3,
        'insulin': 0,
        'duration': 0,
        'rate': 0.425,
        'glucose': 0,
    }
'''

TEST_DATA = [{
    'time': 0,
    'type': 3,
    'insulin': 0,
    'duration': 0,
    'rate': 0.425,
    'glucose': 0
},
{
    'time': 9000,
    'type': 3,
    'insulin': 0,
    'duration': 0,
    'rate': 0.35,
    'glucose': 0
},
{
    'time': 12324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 80
},
{
    'time': 12335,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 12624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 80
},
{
    'time': 12924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 79
},
{
    'time': 13224,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 77
},
{
    'time': 13524,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 80
},
{
    'time': 13539,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 13825,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 13907,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 14124,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 14424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 14724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 14735,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 14936,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 93
},
{
    'time': 15026,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 86
},
{
    'time': 15035,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 15324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 15624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 15924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 15935,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 16224,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 16236,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 16524,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 85
},
{
    'time': 16824,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 88
},
{
    'time': 17124,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 90
},
{
    'time': 17424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 90
},
{
    'time': 17724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 92
},
{
    'time': 18024,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 97
},
{
    'time': 18324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 104
},
{
    'time': 18338,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.2,
    'glucose': 0
},
{
    'time': 18624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 108
},
{
    'time': 18924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 110
},
{
    'time': 19224,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 110
},
{
    'time': 19524,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 112
},
{
    'time': 19537,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.15,
    'glucose': 0
},
{
    'time': 19824,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 113
},
{
    'time': 19837,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1,
    'glucose': 0
},
{
    'time': 20124,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 116
},
{
    'time': 20137,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.825,
    'glucose': 0
},
{
    'time': 20424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 120
},
{
    'time': 20437,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.8,
    'glucose': 0
},
{
    'time': 20724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 124
},
{
    'time': 20738,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.1,
    'glucose': 0
},
{
    'time': 21024,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 129
},
{
    'time': 21037,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.15,
    'glucose': 0
},
{
    'time': 21324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 134
},
{
    'time': 21338,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.2,
    'glucose': 0
},
{
    'time': 21600,
    'type': 3,
    'insulin': 0,
    'duration': 0,
    'rate': 0.4,
    'glucose': 0
},
{
    'time': 21624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 139
},
{
    'time': 21924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 143
},
{
    'time': 22224,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 146
},
{
    'time': 22469,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.1,
    'glucose': 0
},
{
    'time': 22524,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 147
},
{
    'time': 22770,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.625,
    'glucose': 0
},
{
    'time': 22824,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 147
},
{
    'time': 23070,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.525,
    'glucose': 0
},
{
    'time': 23124,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 146
},
{
    'time': 23370,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.45,
    'glucose': 0
},
{
    'time': 23424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 145
},
{
    'time': 23670,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.2,
    'glucose': 0
},
{
    'time': 23724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 143
},
{
    'time': 23969,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.15,
    'glucose': 0
},
{
    'time': 24024,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 142
},
{
    'time': 24270,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.225,
    'glucose': 0
},
{
    'time': 24324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 141
},
{
    'time': 24556,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.15,
    'glucose': 0
},
{
    'time': 24624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 137
},
{
    'time': 24870,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 24924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 133
},
{
    'time': 25224,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 128
},
{
    'time': 25524,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 123
},
{
    'time': 25824,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 118
},
{
    'time': 26069,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 26124,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 113
},
{
    'time': 26424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 109
},
{
    'time': 26724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 106
},
{
    'time': 27024,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 102
},
{
    'time': 27269,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 27324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 99
},
{
    'time': 27624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 96
},
{
    'time': 27924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 91
},
{
    'time': 28224,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 88
},
{
    'time': 28469,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 28524,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 86
},
{
    'time': 28824,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 84
},
{
    'time': 29124,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 29424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 29669,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 29724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 84
},
{
    'time': 30024,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 30324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 30624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 30924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 31224,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 80
},
{
    'time': 31524,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 31824,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 32124,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 32400,
    'type': 3,
    'insulin': 0,
    'duration': 0,
    'rate': 0.35,
    'glucose': 0
},
{
    'time': 32424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 32724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 79
},
{
    'time': 32969,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 33023,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 78
},
{
    'time': 33324,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 78
},
{
    'time': 33569,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 33623,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 77
},
{
    'time': 33924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 76
},
{
    'time': 34223,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 76
},
{
    'time': 34523,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 77
},
{
    'time': 34824,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 78
},
{
    'time': 35123,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 77
},
{
    'time': 35423,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 75
},
{
    'time': 35669,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 35724,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 76
},
{
    'time': 35970,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 36023,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 75
},
{
    'time': 36323,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 76
},
{
    'time': 36623,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 76
},
{
    'time': 36923,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 75
},
{
    'time': 37169,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 37223,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 74
},
{
    'time': 37523,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 73
},
{
    'time': 37823,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 73
},
{
    'time': 38123,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 77
},
{
    'time': 38369,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 38423,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 79
},
{
    'time': 38723,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 75
},
{
    'time': 39023,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 76
},
{
    'time': 39270,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 39323,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 78
},
{
    'time': 39569,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 39623,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 76
},
{
    'time': 39924,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 74
},
{
    'time': 40183,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 40223,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 75
},
{
    'time': 40483,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 40523,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 78
},
{
    'time': 40823,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 80
},
{
    'time': 41123,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 80
},
{
    'time': 41424,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 41723,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 42023,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 42323,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 42624,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 42869,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 42923,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 43170,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 43223,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 43523,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 43823,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 44070,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.075,
    'glucose': 0
},
{
    'time': 44123,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 44370,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 44423,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 44622,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 86
},
{
    'time': 44723,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 44734,
    'type': 2,
    'insulin': 2.1,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 45025,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 83
},
{
    'time': 45270,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.1,
    'glucose': 0
},
{
    'time': 45323,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 45570,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.05,
    'glucose': 0
},
{
    'time': 45623,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 45870,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 45923,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 46223,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 81
},
{
    'time': 46522,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 92
},
{
    'time': 46822,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 101
},
{
    'time': 47070,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.2,
    'glucose': 0
},
{
    'time': 47122,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 112
},
{
    'time': 47422,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 124
},
{
    'time': 47722,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 132
},
{
    'time': 48022,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 136
},
{
    'time': 48270,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.2,
    'glucose': 0
},
{
    'time': 48322,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 145
},
{
    'time': 48622,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 155
},
{
    'time': 48922,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 163
},
{
    'time': 49222,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 170
},
{
    'time': 49470,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.2,
    'glucose': 0
},
{
    'time': 49522,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 168
},
{
    'time': 49822,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 174
},
{
    'time': 50122,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 167
},
{
    'time': 50370,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 50400,
    'type': 3,
    'insulin': 0,
    'duration': 0,
    'rate': 0.4,
    'glucose': 0
},
{
    'time': 50422,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 168
},
{
    'time': 50670,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 50722,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 167
},
{
    'time': 51022,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 162
},
{
    'time': 51322,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 157
},
{
    'time': 51622,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 152
},
{
    'time': 51870,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 51922,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 148
},
{
    'time': 52222,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 143
},
{
    'time': 52823,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 130
},
{
    'time': 53071,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 53123,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 122
},
{
    'time': 53423,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 116
},
{
    'time': 53722,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 109
},
{
    'time': 54022,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 103
},
{
    'time': 54272,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 54274,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 54322,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 95
},
{
    'time': 54571,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 54622,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 88
},
{
    'time': 54922,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 82
},
{
    'time': 55222,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 75
},
{
    'time': 55522,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 72
},
{
    'time': 55708,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 63
},
{
    'time': 55774,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 55822,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 69
},
{
    'time': 56122,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 68
},
{
    'time': 56422,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 72
},
{
    'time': 56509,
    'type': 2,
    'insulin': 0.475,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 56722,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 79
},
{
    'time': 56971,
    'type': 1,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 57022,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 90
},
{
    'time': 57322,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 103
},
{
    'time': 57336,
    'type': 2,
    'insulin': 1,
    'duration': 0,
    'rate': 0,
    'glucose': 0
},
{
    'time': 57622,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 117
},
{
    'time': 57869,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.2,
    'glucose': 0
},
{
    'time': 57922,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 130
},
{
    'time': 58222,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 142
},
{
    'time': 58523,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 150
},
{
    'time': 58823,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 153
},
{
    'time': 59123,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 158
},
{
    'time': 59131,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 1.2,
    'glucose': 0
},
{
    'time': 59423,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 157
},
{
    'time': 59723,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 151
},
{
    'time': 60023,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 147
},
{
    'time': 60031,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.875,
    'glucose': 0
},
{
    'time': 60323,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 146
},
{
    'time': 60332,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.15,
    'glucose': 0
},
{
    'time': 60623,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 142
},
{
    'time': 60632,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0.125,
    'glucose': 0
},
{
    'time': 60877,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 60922,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 136
},
{
    'time': 61222,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 129
},
{
    'time': 61522,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 120
},
{
    'time': 61822,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 114
},
{
    'time': 62122,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 104
},
{
    'time': 62132,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 62422,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 88
},
{
    'time': 62723,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 75
},
{
    'time': 63022,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 66
},
{
    'time': 63277,
    'type': 1,
    'insulin': 0,
    'duration': 30,
    'rate': 0,
    'glucose': 0
},
{
    'time': 63322,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 59
},
{
    'time': 63460,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 60
},
{
    'time': 63622,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 54
},
{
    'time': 63922,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 60
},
{
    'time': 64222,
    'type': 0,
    'insulin': 0,
    'duration': 0,
    'rate': 0,
    'glucose': 58
}]

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Glucose test script.')
    parser.add_argument('dev', help='Serial port')
    parser.add_argument('--baud', help='Baud rate', type=int, default=115200)
    parser.add_argument('--timeout', help='Timeout', type=int, default=2)
    parser.add_argument('--dry_run', help='Does not attempt serial communication', action='store_true', default=False)
    args = parser.parse_args()

    if not args.dry_run:
        with serial.Serial(args.dev, args.baud, timeout=args.timeout) as ser:

            for t in TEST_DATA:
                payload = struct.pack('IIQffff',
                    MAGIC,
                    t['type'],
                    t['time'],
                    t['insulin'],
                    t['duration'],
                    t['rate'],
                    t['glucose'])
                size = len(payload)

                print('sending payload...')
                ser.write(payload)
                print('device should have received', size, 'bytes')

                time.sleep(2)
    else:
        chunks = []
        for t in TEST_DATA:
            payload = struct.pack('!IIQffff',
                MAGIC,
                t['type'],
                t['time'],
                t['insulin'],
                t['duration'],
                t['rate'],
                t['glucose'])
            hexstr = str(binascii.hexlify(payload))[2:][:-1]

            chunks += [(hexstr[i:i+8]) for i in range(0, len(hexstr), 8)]

        for c in chunks:
            print('0x' + c, end=', ')

