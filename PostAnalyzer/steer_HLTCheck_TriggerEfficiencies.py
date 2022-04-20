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


    # LQ -> b tau
    # PreselectionRunner17 = ScriptRunner(year='UL17', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Studies', selectionname='LQDMHLTCheck', plotprefix='LQLQToBTau', backgrounds=['VV', 'ST', 'TT', 'DYJets', 'WJets', 'QCDHad'], signals=['LQLQToBTau_MLQ1000_MCH100_MPS117_L1p0', 'LQLQToBTau_MLQ1360_MCH100_MPS117_L1p0', 'LQLQToBTau_MLQ1810_MCH100_MPS117_L1p0', 'LQLQToBTau_MLQ2170_MCH100_MPS117_L1p0', 'LQLQToBTau_MLQ2620_MCH100_MPS117_L1p0'])

    # Psi Psi
    PreselectionRunner17 = ScriptRunner(year='UL17', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Studies', selectionname='LQDMHLTCheck', plotprefix='PsiPsiToLQChi', backgrounds=['VV', 'ST', 'TT', 'DYJets', 'WJets', 'QCDHad'], signals=['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0'])

    # LQ -> Psi Chi
    # PreselectionRunner17 = ScriptRunner(year='UL17', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Studies', selectionname='LQDMHLTCheck', plotprefix='LQLQToPsiChi', backgrounds=['VV', 'ST', 'TT', 'DYJets', 'WJets', 'QCDHad'], signals=['LQLQToPsiChi_MLQ1360_MCH100_MPS117_L1p0', 'LQLQToPsiChi_MLQ1360_MCH457_MPS513_L1p0', 'LQLQToPsiChi_MLQ1810_MCH100_MPS117_L1p0', 'LQLQToPsiChi_MLQ1810_MCH457_MPS508_L1p0', 'LQLQToPsiChi_MLQ2620_MCH100_MPS117_L1p0', 'LQLQToPsiChi_MLQ2620_MCH457_MPS506_L1p0'])



    PreselectionRunner17.CalculateSelectionEfficiencies(num_and_denom=[('muon', 'input'), ('electron', 'input'), ('tau', 'input'), ('MET', 'input'), ('combined', 'input')])
    # PreselectionRunner17.CalculateSelectionEfficiencies(num_and_denom=[('singlemu', 'input'), ('doublemu', 'input'), ('mutau', 'input'), ('singleele', 'input'), ('eletau', 'input'), ('elejet', 'input'), ('ditau', 'input'), ('met', 'input'), ('triggercategories', 'input')])




if __name__ == '__main__':
    main()
