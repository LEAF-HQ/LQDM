#! /usr/bin/env python

import os, sys, math, time
import subprocess
from utils import *
from SampleSettings import *
from collections import OrderedDict
from PostAnalyzer import *
from ScriptRunner import *
import ROOT

# All constants to be used
analysisname     = 'LQDM'
# analyzer_path    = os.environ['ANALYZERPATH'] # set up by setup.sh
# lqdm_path        = os.path.join(analyzer_path, analysisname)
t3_base_path     = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user')
result_base_path = '/work'


def main():


    # # Psi Psi
    # ScriptRunnerUL17 = ScriptRunner(year='UL17', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Fullselection', selectionname='PsiPsi_03Jets2BTagLoose1LeptonCategories', plotprefix='PsiPsiToLQChi', backgrounds=['VV', 'ST', 'TT', 'DYJets', 'WJets', 'QCDHad'], signals=['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'])

    systematics17  = ['nominal', 'lumi', 'rate_tt', 'rate_dyjets', 'rate_st', 'rate_vv', 'rate_wjets', 'rate_qcd']
    backgrounds17  = ['TT', 'DYJets', 'ST', 'WJets', 'QCDHad', 'VV']
    signals17      = ['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0']
    channels17     = [None]
    histnames_in_out_per_category17 = {
        None: ('metpt', 'MET'),
        # 'cat1jets_0btagloose': ('mt_taumet_rebinlimit2', 'MT'),
        # 'cat1jets_1btagloose': ('mt_taumet_rebinlimit2', 'MT'),
        # 'cat0jets': ('mt_taumet_rebinlimit2', 'MT'),
    }
    # histnames_in_out_per_category17 = {'cat1': ('mt_mumet_rebinlimit2', 'MT')}
    limithisttag17 = 'General'

    processes_per_systematic17 = {'lumi': 'all', 'rate_tt': 'TT', 'rate_dyjets': 'DYJets', 'rate_st': 'ST', 'rate_wjets': 'WJets', 'rate_qcd': 'QCD_Had', 'rate_vv': 'VV', 'pu': 'all', 'pdf': 'all', 'muid': 'all', 'muiso': 'all', 'mutrigger': 'all', 'eleid': 'all', 'elereco': 'all', 'btag_bc': 'all', 'btag_udsg': 'all', 'scale_tt': 'TT', 'scale_dyjets': 'DYJets', 'scale_st': 'ST'}
    pdf_per_systematic17 = {'lumi': 'lnN', 'pu': 'shape', 'pdf': 'shape', 'muid': 'shape', 'muiso': 'shape', 'mutrigger': 'shape', 'eleid': 'shape', 'elereco': 'shape', 'btag_bc': 'shape', 'btag_udsg': 'shape', 'rate_tt': 'lnN', 'rate_dyjets': 'lnN', 'rate_st': 'lnN', 'rate_vv': 'lnN', 'rate_wjets': 'lnN', 'rate_qcd': 'lnN', 'scale_tt': 'shape', 'scale_dyjets': 'shape', 'scale_st': 'shape'}
    value_per_systematic17 = {'lumi': 1.025, 'pu': 1, 'pdf': 1, 'muid': 1, 'muiso': 1, 'mutrigger': 1, 'eleid': 1, 'elereco': 1, 'btag_bc': 1, 'btag_udsg': 1, 'rate_tt': 1.056, 'rate_dyjets': 1.1, 'rate_st': 1.1, 'rate_vv': 1.2, 'rate_wjets': 1.25, 'rate_qcd': 1.5, 'scale_tt': 1, 'scale_dyjets': 1, 'scale_st': 1}

    Analyzer17 = PostAnalyzer('UL17', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Fullselection', selectionname='PsiPsi_03Jets2BTagLoose1LeptonCategories', systematics=systematics17, backgrounds=backgrounds17, signals=signals17, channels=channels17, histnames_in_out_per_category=histnames_in_out_per_category17, limithisttag=limithisttag17, processes_per_systematic=processes_per_systematic17, pdf_per_systematic=pdf_per_systematic17, value_per_systematic=value_per_systematic17)

    Analyzer17.ProduceCombineHistograms(signal_scaled_by=1.)
    # Analyzer17.CreateDatacards()
    # Analyzer17.CombineChannels()
    # Analyzer17.ExecuteCombineCombination()
    # Analyzer17.PlotLimits(signal_scaled_by=1., draw_theory=True)

if __name__ == '__main__':
    main()
