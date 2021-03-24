import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
# import ROOT


class PostAnalyzer:
    def __init__(self, year, analysisname, t3_base_path, result_base_path, selectionname):
        self.year = year
        self.analysisname  = analysisname
        self.t3_base_path  = t3_base_path
        self.selectionname = selectionname
        self.user = os.environ['USER']

        self.finalselection_path = os.path.join(t3_base_path,     self.user, analysisname, year, 'Finalselection', selectionname)
        self.resultpath          = os.path.join(result_base_path, self.user, analysisname, year, 'Finalselection', selectionname)
        self.datacard_path       = os.path.join(self.resultpath, 'data', 'combineinput')

        print green('--> Set up PostAnalyzer for year %s:' % (str(year)))
        for key in self.__dict__.keys():
            print green('  --> %s: %s' % (str(key), str(self.__dict__[key])))

    def ProduceCombineHistograms(self):
        pass
