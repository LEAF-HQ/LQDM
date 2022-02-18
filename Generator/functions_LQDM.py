# Author: Arne Reimers
import os, sys
from os.path import isfile, join
from fnmatch import fnmatch
import subprocess
import time
import signal
import math
from math import sqrt, log, floor, ceil
from bisect import bisect_left
from ROOT import TFile
from utils import *
from constants import *
from tqdm import tqdm

from multiprocessing import Process, Queue


def are_masses_allowed(mlq, mps, mch, particle_produced, decaychannels):
    is_excluded = False
    if particle_produced == 'Psi':
        if mps > 13000./2.: #too heavy for 13TeV
            is_excluded = True

    if particle_produced == 'LQ':
        if mlq > 13000./2.: #too heavy for 13TeV
            is_excluded = True
        # now look at decay channels
        if 'PsiChi' in decaychannels:
            if mlq < mps + mch: #LQ cannot decay to psi+chi1
                is_excluded = True
    return not is_excluded



def get_all_combinations(preferred_configurations, mlq_stepsize=90, mch_exp_stepsize=0.03):
    all_combinations = {}
    mlq_min = min(preferred_configurations.keys())
    mlq_max = max(preferred_configurations.keys())
    mch_min = 999999999
    mch_max = 0
    for mlq in preferred_configurations.keys():
        for mch in preferred_configurations[mlq].keys():
            mch_min = min(mch_min, mch)
            mch_max = max(mch_max, mch)

    for mlq in range(mlq_min, mlq_max+mlq_stepsize, mlq_stepsize):
        all_combinations[mlq] = []
        for mch_exp in range(int(math.log(mch_min, 10)*100), int(math.log(mch_max, 10)*100) + int(mch_exp_stepsize*100), int(mch_exp_stepsize*100)):
            mch_exp = float(mch_exp / 100.)
            mch = int(round(10**(float(mch_exp))))
            all_combinations[mlq].append(mch)
    return all_combinations


def get_best_lambda(mlq):
    # calculate lambda and round to 3 decimal points
    return float('%.3f' % float(1.1/2. * (mlq/1000.)))


def get_config_list(preferred_configurations, processname):
    configs = []

    for mlq in preferred_configurations.keys():
        for mch in preferred_configurations[mlq].keys():
            config = {}
            config['mlq'] = mlq
            config['mch'] = mch
            if not is_config_excluded_for_process(config=config, processname=processname, preferred_configurations=preferred_configurations):
                configs.append(config)
    return configs

def get_mlq_mps_mch(preferred_configurations, config):
    return (config['mlq'], preferred_configurations[config['mlq']][config['mch']][0], config['mch'])

def is_config_excluded(excluded_configurations, config, processname):
    if not processname in excluded_configurations.keys(): # no config of this proc is excluded
        return False

    if config in excluded_configurations[processname]:
        return True
    else:
        return False
