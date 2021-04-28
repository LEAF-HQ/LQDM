#! /usr/bin/env python

import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from PostAnalyzer import *
from ScriptRunner import *


# All constants to be used
analysisname     = 'LQDM'
# analyzer_path    = os.environ['ANALYZERPATH'] # set up by setup.sh
# lqdm_path        = os.path.join(analyzer_path, analysisname)
t3_base_path     = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user')
result_base_path = '/work'


def main():

    systematics17  = ['nominal', 'lumi', 'rate_tt', 'rate_dyjets', 'rate_st']
    # backgrounds17  = ['TT', 'DYJets', 'ST', 'WJets', 'QCD_Had']
    backgrounds17  = ['TT', 'DYJets', 'ST']
    signals17      = ['LQLQToBTau_MLQ1000_MPS117_MC1100_L1p0', 'LQLQToBTau_MLQ1360_MPS117_MC1100_L1p0', 'LQLQToBTau_MLQ1810_MPS117_MC1100_L1p0', 'LQLQToBTau_MLQ2170_MPS117_MC1100_L1p0', 'LQLQToBTau_MLQ2620_MPS117_MC1100_L1p0']
    channels17     = ['elch', 'much', 'elmuch', 'tach']
    histnames_in_out_per_category17 = {'cat1': ('stmet3', 'STMET'), 'cat2': ('stmet3', 'STMET')}
    limithisttag17 = 'Limits'

    processes_per_systematic17 = {'lumi': 'all', 'rate_tt': 'TT', 'rate_dyjets': 'DYJets', 'rate_st': 'ST', 'rate_wjets': 'WJets', 'rate_qcd': 'QCD_Had', 'pu': 'all', 'pdf': 'all', 'muid': 'all', 'muiso': 'all', 'mutrigger': 'all', 'eleid': 'all', 'elereco': 'all', 'btag_bc': 'all', 'btag_udsg': 'all', 'scale_tt': 'TT', 'scale_dyjets': 'DYJets', 'scale_st': 'ST'}
    pdf_per_systematic17 = {'lumi': 'lnN', 'pu': 'shape', 'pdf': 'shape', 'muid': 'shape', 'muiso': 'shape', 'mutrigger': 'shape', 'eleid': 'shape', 'elereco': 'shape', 'btag_bc': 'shape', 'btag_udsg': 'shape', 'rate_tt': 'lnN', 'rate_dyjets': 'lnN', 'rate_st': 'lnN', 'rate_wjets': 'lnN', 'rate_qcd': 'lnN', 'scale_tt': 'shape', 'scale_dyjets': 'shape', 'scale_st': 'shape'}
    value_per_systematic17 = {'lumi': 1.025, 'pu': 1, 'pdf': 1, 'muid': 1, 'muiso': 1, 'mutrigger': 1, 'eleid': 1, 'elereco': 1, 'btag_bc': 1, 'btag_udsg': 1, 'rate_tt': 1.056, 'rate_dyjets': 1.1, 'rate_st': 1.1, 'rate_wjets': 1.25, 'rate_qcd': 1.5, 'scale_tt': 1, 'scale_dyjets': 1, 'scale_st': 1}




    StudiesRunner17 = ScriptRunner(year='2017', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Studies', selectionname='PsiPsi_LeptonIDEfficiencies', backgrounds=[], signals=['PsiPsiToLQChi_MLQ1000_MPS117_MC1100_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS244_MC1214_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS567_MC1457_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS1051_MC1977_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS2221_MC12089_L1p0'])
    # StudiesRunner17.CalculateLeptonReconstructionEfficiencies()
    StudiesRunner17.CalculateLeptonIDEfficiencies()

    PreselectionRunner17 = ScriptRunner(year='2017', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Preselection', selectionname='PsiPsi_Initial', backgrounds=['TT', 'DYJets', 'ST', 'WJets', 'QCD_Had'], signals=['PsiPsiToLQChi_MLQ1000_MPS117_MC1100_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS244_MC1214_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS567_MC1457_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS1051_MC1977_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS2221_MC12089_L1p0'])
    # PreselectionRunner17.CalculateSelectionEfficiencies(num_and_denom=[('pteta1muon', 'iddenominator'), ('pteta1electron', 'iddenominator'), ('pteta1tau', 'iddenominator'), ('pteta1lepton', 'iddenominator'), ('mva1muon', 'iddenominator'), ('mva1electron', 'iddenominator'), ('mva1lepton', 'iddenominator'), ('mvaip1muon', 'iddenominator'), ('mvaip1electron', 'iddenominator'), ('mvaip1lepton', 'iddenominator'), ('tauvsmu1tau', 'iddenominator'), ('tauvsmuvse1tau', 'iddenominator'), ('tauvsmuvsevsjet1tau', 'iddenominator'), ('final', 'iddenominator')])



    # Analyzer17 = PostAnalyzer('2017', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionname='TauTau_ElTau_MuTau_ElMu_2Jets_12LooseBTag_met50', systematics=systematics17, backgrounds=backgrounds17, signals=signals17, channels=channels17, histnames_in_out_per_category=histnames_in_out_per_category17, limithisttag=limithisttag17, processes_per_systematic=processes_per_systematic17, pdf_per_systematic=pdf_per_systematic17, value_per_systematic=value_per_systematic17)

    # Analyzer17.ProduceCombineHistograms(signal_scaled_by=0.01)
    # Analyzer17.CreateDatacards()
    # Analyzer17.CombineChannels()
    # Analyzer17.ExecuteCombineCombination()
    # Analyzer17.PlotLimits(signal_scaled_by=0.01, draw_observed=False)















if __name__ == '__main__':
    main()
