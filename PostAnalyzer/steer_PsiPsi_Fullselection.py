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
    'PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', color=ROOT.kRed+4, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=0.12 TeV, light LQ)'),

    'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', color=ROOT.kRed+1, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=0.12 TeV, heavy LQ)'),

    'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', color=ROOT.kViolet, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=0.57 TeV, light LQ)'),

    'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', color=ROOT.kAzure-2, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=0.50 TeV, heavy LQ)'),

    'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', color=ROOT.kOrange, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=2.22 TeV, light LQ)'),

    'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0', color=ROOT.kGreen-2, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=2.45 TeV, heavy LQ)'),

    'VV':          SampleSettings(name='VV', color=ROOT.kViolet, linestyle=1, legendtext='VV'),

    'ST':          SampleSettings(name='ST', color=ROOT.kAzure-2, linestyle=1, legendtext='ST'),

    'TT':          SampleSettings(name='TT', color=ROOT.kRed+1, linestyle=1, legendtext='TT'),

    'DYJets':          SampleSettings(name='DYJets', color=ROOT.kOrange, linestyle=1, legendtext='DYJets'),

    'WJets':          SampleSettings(name='WJets', color=ROOT.kGreen-2, linestyle=1, legendtext='WJets'),

    'QCDHad':          SampleSettings(name='QCDHad', color=429, linestyle=1, legendtext='QCDHad'),

}


def main():


    # Psi Psi
    # ScriptRunnerUL17 = ScriptRunner(year='UL17', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Fullselection', selectionname='PsiPsi_03Jets2BTagLoose1LeptonCategories', plotprefix='PsiPsiToLQChi', backgrounds=['VV', 'ST', 'TT', 'DYJets', 'WJets', 'QCDHad'], signals=['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'])

    ScriptRunnerUL17 = ScriptRunner(year='UL17', analysisname=analysisname, input_base_path=input_base_path, result_base_path=result_base_path, selectionstage='Fullselection', selectionname='PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories', plotprefix='PsiPsiToLQChi', samples=samples)



    ScriptRunnerUL17.CalculateSelectionEfficiencies(backgrounds=['VV', 'ST', 'TT', 'DYJets', 'WJets', 'QCDHad'], signals=['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'], num_and_denom=[('input', 'input'), ('jets1_nominal', 'input'), ('jets2_nominal', 'input'), ('bjettight1_nominal', 'input'), ('dphi_j1_met_nominal', 'input'), ('dphi_j2_met_nominal', 'input'), ('ptratio_j2_met_nominal', 'input'), ('sphericity_nominal', 'input'), ('tach_nominal', 'input'), ('tach_addmu_nominal', 'input'), ('tach_addel_nominal', 'input'), ('tach_nolep_nominal', 'input'), ('much_nominal', 'input'), ('elch_nominal', 'input'), ('noch_nominal', 'input'), ('nominal', 'input')])




if __name__ == '__main__':
    main()
