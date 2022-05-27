#!/usr/bin/env python

import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict

import ROOT
from DNNTools.SampleSettings import *
from DNNRunner import *
from ConvertRootToInputs import ConvertRootToInputs


# All constants to be used
analysisname     = 'LQDM'
user             = 'areimers'
input_base_path  = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user', user, 'LQDM', 'UL17', 'Fullselection', 'PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories')
output_base_path = os.path.join('/work', user, 'LQDM', 'UL17', 'Fullselection', 'PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories')

samples = {

    'PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0':   SampleSettings(name='PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0',   category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0'),
    'PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0': SampleSettings(name='PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0', category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0'),
    'PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0':   SampleSettings(name='PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0',   category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0'),
    'PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0':   SampleSettings(name='PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0',   category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0'),
    'PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0':  SampleSettings(name='PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0',  category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0'),
    'PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0'),
    'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0':  SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0',  category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0'),
    'PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0'),
    'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0'),
    'PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0':   SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0',   category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0'),
    'PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0'),
    'PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0':  SampleSettings(name='PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0',  category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0'),
    'PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0'),
    'PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0'),
    'PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0':   SampleSettings(name='PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0',   category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0'),
    'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0'),
    'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0':  SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0',  category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'),
    'PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0'),
    'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0':    SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0',    category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0'),
    'PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0':   SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0',   category='PsiPsiToLQChi', label=6, color=ROOT.kBlack, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0'),


    # 'VV':          SampleSettings(name='VV',     category='VV',     label=5, color=ROOT.kViolet,  linestyle=1, legendtext='VV'),
    # 'ST':          SampleSettings(name='ST',     category='ST',     label=4, color=ROOT.kGreen-2, linestyle=1, legendtext='ST'),
    # 'TT':          SampleSettings(name='TT',     category='TT',     label=3, color=ROOT.kRed+1,   linestyle=1, legendtext='TT'),
    # 'DYJets':      SampleSettings(name='DYJets', category='DYJets', label=2, color=ROOT.kOrange,  linestyle=1, legendtext='DYJets'),
    # 'WJets':       SampleSettings(name='WJets',  category='WJets',  label=1, color=ROOT.kGreen-2, linestyle=1, legendtext='WJets'),
    'QCDHad':      SampleSettings(name='QCDHad', category='QCDHad', label=0, color=ROOT.kAzure-2, linestyle=1, legendtext='QCDHad'),
}

dnnparameters = OrderedDict()
dnnparameters['DenseLayer'] = [128]
dnnparameters['batch_size'] =  128
# dnnparameters['classes'] = OrderedDict([(0, ['QCDHad']), (1, ['WJets']), (2, ['DYJets']), (3, ['TT']), (4, ['ST']), (5, ['VV']), (6, ['PsiPsiToLQChi'])])
# dnnparameters['classes'] = OrderedDict([('QCDHad', 0), ('WJets', 1), ('DYJets', 2), ('TT', 3), ('ST', 4), ('VV', 5), ('PsiPsiToLQChi', 6)])
# dnnparameters['classes'] = OrderedDict([('QCDHad', 0), ('WJets', 1), ('DYJets', 2), ('TT', 3), ('ST', 4), ('VV', 5), ('PsiPsiToLQChi', 6)])
# dnnparameters['classes'] = OrderedDict([('QCDHad', 0), ('WJets', 1), ('DYJets', 1), ('TT', 1), ('ST', 1), ('VV', 1), ('PsiPsiToLQChi', 2)])
# dnnparameters['classes'] = OrderedDict([('PsiPsiToLQChi', 0), ('QCDHad', 1), ('VV', 2)])
# dnnparameters['classes'] = OrderedDict([('PsiPsiToLQChi', 0), ('QCDHad', 1), ('VV', 1)])
dnnparameters['classes'] = OrderedDict([('PsiPsiToLQChi', 0), ('QCDHad', 1)])
dnnparameters['regmethod'] =  'dropout'
dnnparameters['regrate'] =  0.000000
dnnparameters['batchnorm'] =  False
dnnparameters['epochs'] =  50
dnnparameters['learningrate'] =  0.00050
dnnparameters['runonfraction'] =  1.00
dnnparameters['eqweight'] = False

### From Andrea
dnnparameters['activation'] = 'relu'
dnnparameters['kernel_initializer'] = 'glorot_normal'
dnnparameters['bias_initializer'] = 'ones'
dnnparameters['activation_last'] = 'softmax'
dnnparameters['optimizer'] = 'adam' #'adamax'
dnnparameters['metrics'] = ['accuracy']
dnnparameters['dropoutRate'] = 0.01



def main():

    # # # Classifier = DNNRunner(dnnparameters=dnnparameters, year='UL17', plotprefix='PsiPsiToLQChi', samples=samples, analysisname=analysisname, input_base_path=input_base_path, output_base_path=output_base_path, selectionstage='Fullselection', selectionname='PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories')
    Classifier = DNNRunner(dnnparameters=dnnparameters, year='UL17', plotprefix='PsiPsiToLQChi', samples=samples, analysisname=analysisname, input_base_path=input_base_path, output_base_path=output_base_path)
    # Classifier.ConvertRootToInputs.Convert()
    Classifier.PreprocessInputs.Process()
    # Classifier.Plotter.Plot()
    Classifier.Training.Train()
    # Classifier.MakePrediction(filepostfix='')
    # Classifier.PlotPerformance(filepostfix='', plotfoldername='DNNPerformancePlots')


    # ExportModel(parameters, inputfolder='input/', outputfolder='output/', use_best_model=True)
    # RankNetworks(outputfolder='output/')







if __name__ == '__main__':
    main()
