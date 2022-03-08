#! /usr/bin/env python

import os, sys, math
from os.path import isfile, join
import subprocess
import time
import parse
from operator import itemgetter
import importlib
from utils import *
from functions import *
from constants import *
from collections import OrderedDict

import ROOT
from ROOT import gROOT, gStyle, gPad, TLegend, TFile, TCanvas, Double, TF1, TH2D, TGraph, TGraph2D, TGraphAsymmErrors, TLine,\
                 kBlack, kRed, kBlue, kAzure, kCyan, kGreen, kGreen, kYellow, kOrange, kMagenta, kViolet,\
                 kSolid, kDashed, kDotted
from math import sqrt, log, floor, ceil
from array import array

from preferred_configurations import *
from tdrstyle_all import *
import tdrstyle_all as TDR

from CrossSectionRunner import *
from GensimRunner import *

processes = ['LQLQToBTau']
mlqs     = [1000, 1400, 1800, 2200, 2600]
mchs_exp = [2.0] # to reasonable precision, the LQ->btau decay kinematics do not depend on DM/Psi masses
lambdas  = [1.0, 'best']


general_settings = {
'UL17':{
    'BWCUTOFF': 15,
    'PDF':      315200
    }
}

individual_settings = []
for lamb in lambdas:
    for mlq in mlqs:
        prefmlq = find_closest(sorted(preferred_configurations.keys()), mlq)
        for exp in mchs_exp:
            mch = find_closest(sorted(preferred_configurations[prefmlq].keys()), 10**exp)
            mps = preferred_configurations[prefmlq][mch][0]

            dict = OrderedDict()
            # dict keys: what to replace in cards
            # dict value tuples: (value, "name of parameter in samplename")
            dict['MLQ']    = (prefmlq, 'MLQ')
            dict['MCH']    = (mch, 'MCH')
            dict['MPS']    = (mps, 'MPS')
            dict['MC2']    = (int(round(2*mps - mch)), None) # delta_c2 = 2 * delta_ps => mc2 = 2mps - mch --> As done in paper, but value of mc2 doesn't matter for anomalies or relic abundance. Maybe increase to suppress this channel
            dict['MZP']    = (int(round(prefmlq/math.sqrt(2))), None) # as done in the paper, but doesn't really affect anomalies or relic abundance
            dict['LAMBDA'] = (lamb, 'L')
            individual_settings.append(dict)


tag = ''                # tags are auto-formatted to '_XXXX'
maxindex        = 100   # Number of samples per configuration
nevents         = 1000  # Events per sample



username       = os.environ['USER']
arch_tag       = 'slc7_amd64_gcc700'
cmssw_tag_gp  = 'CMSSW_10_6_19' # from gridpack_generation.sh
cmssw_tag_sim  = 'CMSSW_10_6_28'
cmssw_tag_hlt  = 'CMSSW_9_4_14_UL_patch1'
campaign       = 'UL17'
sampletype     = 'ChiPsi'


workarea       = os.path.join('/work', username)
workdir_slurm  = os.path.join(workarea, 'workdir_slurm')
mgfolder       = os.path.join(workarea, cmssw_tag_sim, 'src', 'genproductions', 'bin', 'MadGraph5_aMCatNLO')
mgfolder_local = os.path.join(workarea, 'MG5_aMC_v2_7_2')
basefolder     = os.environ['LEAFPATH']
thisgenerator  = os.path.join(basefolder, 'LQDM', 'Generator')
generatorfolder= os.environ['GENERATORPATH']
gridpackfolder = os.path.join(thisgenerator, 'gridpacks', sampletype)
cardfolder     = os.path.join(thisgenerator, 'cards', sampletype)
crosssecfolder = os.path.join(thisgenerator, 'crosssections', sampletype)
psetfolder     = os.path.join(generatorfolder, 'PSets', campaign)
T2_director      = 'gsiftp://storage01.lcg.cscs.ch/'
T2_director_root = 'root://storage01.lcg.cscs.ch/'
T3_director      = 'root://t3dcachedb03.psi.ch/'
T2_path          = os.path.join('/pnfs/lcg.cscs.ch/cms/trivcat/store/user', username)
T3_path          = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user', username)




folderstructure = {
    'GENSIM': {
        'pset':            psetfolder+'/pset_01_gensim.py',
        'cmsswtag':        cmssw_tag_sim,
        'jobnametag':      'gensim',
        'outfilenamebase': 'GENSIM',
        'pathtag':         'GENSIM/' + sampletype
    },
    'DR': {
        'pset':            psetfolder+'/pset_Summer20_03_dr.py',
        'cmsswtag':        cmssw_tag_sim,
        'jobnametag':      'dr',
        'outfilenamebase': 'DR',
        'infilepathtag':   'GENSIM/' + sampletype,
        'infilenamebase':  'GENSIM',
        'pathtag':         'DR/' + sampletype
    },
    'HLT': {
        'pset':            psetfolder+'/pset_04_hlt.py',
        'cmsswtag':        cmssw_tag_hlt,
        'jobnametag':      'hlt',
        'outfilenamebase': 'HLT',
        'infilepathtag':   'DR/' + sampletype,
        'infilenamebase':  'DR',
        'pathtag':         'HLT/' + sampletype
    },
    'AOD': {
        'pset':            psetfolder+'/pset_05_aod.py',
        'cmsswtag':        cmssw_tag_sim,
        'jobnametag':      'aod',
        'outfilenamebase': 'AOD',
        'infilepathtag':   'HLT/' + sampletype,
        'infilenamebase':  'HLT',
        'pathtag':         'AOD/' + sampletype
    },
    'MINIAODv2': {
        'pset':            psetfolder+'/pset_06_miniaodv2.py',
        'cmsswtag':        cmssw_tag_sim,
        'jobnametag':      'miniaod',
        'outfilenamebase': 'MINIAOD',
        'infilepathtag':   'AOD/' + sampletype,
        'infilenamebase':  'AOD',
        'pathtag':         'MINIAODv2/' + sampletype
    }

}

ensureDirectory(workdir_slurm)


submit = True



EventGenerator = GensimRunner(processnames=processes, tag=tag, individual_settings=individual_settings, general_settings=general_settings[campaign], workdir_slurm=workdir_slurm, workarea=workarea, basefolder=basefolder, cardfolder=cardfolder, mgfolder=mgfolder, generatorfolder=generatorfolder, gridpackfolder=gridpackfolder, arch_tag=arch_tag, cmssw_tag_gp=cmssw_tag_gp, T2_director=T2_director, T2_path=T2_path, T2_director_root=T2_director_root, T3_director=T3_director, T3_path=T3_path, campaign=campaign, folderstructure=folderstructure, maxindex=maxindex, nevents=nevents, submit=submit)
# EventGenerator.ProduceCards()
# EventGenerator.SubmitGridpacks(runtime=(3,00,00))
# EventGenerator.SubmitGenerationStep(generation_step='GENSIM', ncores=2, runtime=(3,00,00), mode='new')
# EventGenerator.SubmitGenerationStep(generation_step='GENSIM', ncores=8, runtime=(3,00,00), mode='resubmit')
# EventGenerator.SubmitGenerationStep(generation_step='DR', ncores=8, runtime=(10,00,00), mode='new')
# EventGenerator.SubmitGenerationStep(generation_step='DR', ncores=8, runtime=(10,00,00), mode='resubmit')
# EventGenerator.SubmitGenerationStep(generation_step='HLT', ncores=8, runtime=(10,00,00), mode='new')
# EventGenerator.SubmitGenerationStep(generation_step='HLT', ncores=8, runtime=(10,00,00), mode='resubmit')
# EventGenerator.RemoveSamples(generation_step='DR')
EventGenerator.SubmitGenerationStep(generation_step='AOD', ncores=4, runtime=(10,00,00), mode='new')
# EventGenerator.SubmitGenerationStep(generation_step='AOD', ncores=4, runtime=(10,00,00), mode='resubmit')
# EventGenerator.SubmitGenerationStep(generation_step='MINIAODv2', ncores=2, runtime=(5,00,00), mode='new')
# EventGenerator.SubmitGenerationStep(generation_step='MINIAODv2', ncores=2, runtime=(5,00,00), mode='resubmit')


#
