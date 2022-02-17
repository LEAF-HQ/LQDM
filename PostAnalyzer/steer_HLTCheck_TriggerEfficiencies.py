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





    PreselectionRunner17 = ScriptRunner(year='2017', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionstage='Studies', selectionname='LQDMHLTCheck', backgrounds=['LQLQToBTau_MLQ1000_MPS117_MC1100_L1p0', 'LQLQToBTauPsiChi_MLQ1000_MPS117_MC1100_L1p0', 'LQLQToPsiChi_MLQ1000_MPS117_MC1100_L1p0', 'PsiPsiToLQChi_MLQ1000_MPS2221_MC12089_L1p0'], signals=[])
    # PreselectionRunner17.CalculateSelectionEfficiencies(num_and_denom=[('singlemu', 'input'), ('doublemu', 'input'), ('mutau', 'input'), ('singleele', 'input'), ('eletau', 'input'), ('elejet', 'input'), ('ditau', 'input'), ('met', 'input'), ('triggercategories', 'input')])
    PreselectionRunner17.CalculateSelectionEfficiencies(num_and_denom=[('muon', 'input'), ('electron', 'input'), ('tau', 'input'), ('MET', 'input'), ('combined', 'input')])





if __name__ == '__main__':
    main()
