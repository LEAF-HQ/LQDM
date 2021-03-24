#! /usr/bin/env python

import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from PostAnalyzer import *


# All constants to be used
analysisname     = 'LQDM'
leaf_path        = os.environ['LEAFPATH'] # set up by setup.sh
analyzer_path    = os.environ['ANALYZERPATH'] # set up by setup.sh
lqdm_path        = os.path.join(analyzer_path, analysisname)

t3_base_path     = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user')
result_base_path = '/work'

#
# cout << green << "--> Fullselection path:  " << m_fullselection_path << reset << endl;
# cout << green << "--> Finalselection path: " << m_fullselection_path << reset << endl;
# cout << green << "--> Output path:         " << m_output_path << reset << endl;


def main():
    Analyzer17 = PostAnalyzer('2017', analysisname=analysisname, t3_base_path=t3_base_path, result_base_path=result_base_path, selectionname='TauTau_ElTau_MuTau_ElMu_2Jets_12LooseBTag_met50')
    Analyzer17.ProduceCombineHistograms()



















if __name__ == '__main__':
    main()
