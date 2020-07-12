import urllib
import os
import contextlib

def get_file(url, filename):
    with open(filename, 'wb') as out_file:
        with contextlib.closing(urllib.request.urlopen(url)) as fp:
            block_size = 1024 * 8
            while True:
                block = fp.read(block_size)
                if not block:
                    break
                out_file.write(block)

if not os.path.exists('tmp'):
    os.makedirs('tmp')
get_file("https://archive.ics.uci.edu/ml/machine-learning-databases/adult/Index", filename="tmp/Index")
get_file("https://archive.ics.uci.edu/ml/machine-learning-databases/adult/adult.data", filename="tmp/adult.data")
get_file("https://archive.ics.uci.edu/ml/machine-learning-databases/adult/adult.names", filename="tmp/adult.names")
get_file("https://archive.ics.uci.edu/ml/machine-learning-databases/adult/adult.test", filename="tmp/adult.test")

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import itertools

df = pd.read_csv('tmp/adult.data', 
                  names=['age','workclass',
                        'fnlwgt','education',
                        'education-num','marital-status',
                        'occupation','relationship',
                        'race','sex',
                        'capital-gain','capital-loss',
                        'hours-per-week','native-country',
                        'label'])

hpw_v_i_le50k = df[df['label'] == " <=50K"]
                  .groupby(['hours-per-week', 'label'])
                  .size()
                  .reset_index()
hpw_v_i_gt50k = df[df['label'] == " >50K"]
                  .groupby(['hours-per-week', 'label'])
                  .size()
                  .reset_index()