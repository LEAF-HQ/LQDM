#!/usr/bin/env python

import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict

import ROOT
from DNNTools.SampleSettings import *
from DNNRunner import *
from ConvertRootToInputs import ConvertRootToInputs
from DNNTools.functions_dnn import parameters_to_tag


# All constants to be used
analysisname     = 'LQDM'
user             = 'areimers'
input_base_path  = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user', user, 'LQDM', 'UL17', 'Fullselection', 'PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories')
output_base_path = os.path.join('/work', user, 'LQDM', 'UL17', 'Fullselection', 'PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories')

samples = {

    # 'PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0':   SampleSettings(filename='MC__PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0.root',   samplename='PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0', category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0': SampleSettings(filename='MC__PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0.root', samplename='PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0', category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0':   SampleSettings(filename='MC__PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0.root',   samplename='PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0', category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0':   SampleSettings(filename='MC__PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0.root',   samplename='PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0', category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0':  SampleSettings(filename='MC__PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0.root',  samplename='PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0', category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0.root',    samplename='PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0':  SampleSettings(filename='MC__PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0.root',  samplename='PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0.root',    samplename='PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0.root',    samplename='PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0':   SampleSettings(filename='MC__PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0.root',   samplename='PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0.root',    samplename='PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0':  SampleSettings(filename='MC__PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0.root',  samplename='PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0.root',    samplename='PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0.root',    samplename='PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0':   SampleSettings(filename='MC__PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0.root',   samplename='PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0.root',    samplename='PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0':  SampleSettings(filename='MC__PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0.root',  samplename='PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0.root',    samplename='PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0':    SampleSettings(filename='MC__PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0.root',    samplename='PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0',  category='PsiPsiToLQChi'),
    # 'PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0':   SampleSettings(filename='MC__PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0.root',   samplename='PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0',  category='PsiPsiToLQChi'),


    # 'VV':          SampleSettings(filename='MC__VV.root',     samplename='VV',     category='VV'),
    'ST':          SampleSettings(filename='MC__ST.root',     samplename='ST',     category='ST'),
    # 'TT':          SampleSettings(filename='MC__TT.root',     samplename='TT',     category='TT'),
    # 'DYJets':      SampleSettings(filename='MC__DYJets.root', samplename='DYJets', category='DYJets'),
    # 'WJets':       SampleSettings(filename='MC__WJets.root',  samplename='WJets',  category='WJets'),
    # 'QCDHad':      SampleSettings(filename='MC__QCDHad.root', samplename='QCDHad', category='QCDHad'),
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
dnnparameters['classes'] = OrderedDict([('PsiPsiToLQChi', 0), ('QCDHad', 1), ('VV', 2), ('ST', 1)])
dnnparameters['regularization_method'] =  'dropout'
dnnparameters['regularization_rate'] =  0.000000
dnnparameters['batchnorm'] =  False
dnnparameters['epochs'] =  50
dnnparameters['learningrate'] =  0.00050
dnnparameters['runonfraction'] =  1.00
dnnparameters['eqweight'] = False



def main():

    # # # Classifier = DNNRunner(dnnparameters=dnnparameters, year='UL17', plotprefix='PsiPsiToLQChi', samples=samples, analysisname=analysisname, input_base_path=input_base_path, output_base_path=output_base_path, selectionstage='Fullselection', selectionname='PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories')
    Classifier = DNNRunner(dnnparameters=dnnparameters, year='UL17', samples=samples, analysisname=analysisname, input_base_path=input_base_path, output_base_path=output_base_path)
    Classifier.ConvertRootToInputs.Convert()
    # Classifier.PreprocessInputs.Process()
    # Classifier.Plotter.PlotInputVariables()
    # Classifier.Training.Train()
    # Classifier.PlotterPredictions.PlotPredictions()
    # Classifier.MakePrediction(filepostfix='')
    # Classifier.PlotPerformance(filepostfix='', plotfoldername='DNNPerformancePlots')


    # ExportModel(parameters, inputfolder='input/', outputfolder='output/', use_best_model=True)
    # RankNetworks(outputfolder='output/')







if __name__ == '__main__':
    main()
