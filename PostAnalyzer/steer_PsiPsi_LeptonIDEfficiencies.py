#! /usr/bin/env python

import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from PostAnalyzer import *
from ScriptRunner import *


# All constants to be used
analysisname     = 'LQDM'
user = 'areimers'
# analyzer_path    = os.environ['ANALYZERPATH'] # set up by setup.sh
# lqdm_path        = os.path.join(analyzer_path, analysisname)
input_base_path     = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user', user)
result_base_path = os.path.join('/work', user)

# colors = [ROOT.kRed+4, ROOT.kRed+1, ROOT.kViolet, ROOT.kAzure-2, ROOT.kOrange, ROOT.kGreen-2]
samples = {
    'PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', color=ROOT.kRed+4, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0'),

    'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', color=ROOT.kRed+1, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0'),

    'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', color=ROOT.kViolet, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0'),

    'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', color=ROOT.kAzure-2, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0'),

    'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', color=ROOT.kOrange, linestyle=2, legendtext='PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0'),

    'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0', color=ROOT.kGreen-2, linestyle=2, legendtext='PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'),

    'VV':          SampleSettings(name='VV', color=ROOT.kViolet, linestyle=1, legendtext='VV'),

    'ST':          SampleSettings(name='ST', color=ROOT.kGreen-2, linestyle=1, legendtext='ST'),

    'TT':          SampleSettings(name='TT', color=ROOT.kRed+1, linestyle=1, legendtext='TT'),

    'DYJets':          SampleSettings(name='DYJets', color=ROOT.kOrange, linestyle=1, legendtext='DYJets'),

    'WJets':          SampleSettings(name='WJets', color=ROOT.kGreen-2, linestyle=1, legendtext='WJets'),

    'QCDHad':          SampleSettings(name='QCDHad', color=ROOT.kAzure-2, linestyle=1, legendtext='QCDHad'),

}

def main():


    # Psi Psi
    PreselectionRunner17 = ScriptRunner(year='UL17', analysisname=analysisname, input_base_path=input_base_path, result_base_path=result_base_path, selectionstage='Studies', selectionname='PsiPsi_LeptonIDEfficiencies', plotprefix='PsiPsiToLQChi', samples=samples)

    # PreselectionRunner17.CalculateLeptonReconstructionEfficiencies(backgrounds=[], signals=['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'])
    # PreselectionRunner17.CalculateLeptonIDEfficiencies(backgrounds=[], signals=['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'])





    # LQ -> Psi Chi
    # PreselectionRunner17 = ScriptRunner(year='UL17', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Studies', selectionname='LQDMHLTCheck', plotprefix='LQLQToPsiChi', backgrounds=['VV', 'ST', 'TT', 'DYJets', 'WJets', 'QCDHad'], signals=['LQLQToPsiChi_MLQ1360_MCH100_MPS117_L1p0', 'LQLQToPsiChi_MLQ1360_MCH457_MPS513_L1p0', 'LQLQToPsiChi_MLQ1810_MCH100_MPS117_L1p0', 'LQLQToPsiChi_MLQ1810_MCH457_MPS508_L1p0', 'LQLQToPsiChi_MLQ2620_MCH100_MPS117_L1p0', 'LQLQToPsiChi_MLQ2620_MCH457_MPS506_L1p0'])




if __name__ == '__main__':
    main()
