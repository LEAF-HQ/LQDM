import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
import functions.CalculateLeptonReconstructionEfficiencies
import functions.CalculateLeptonIDEfficiencies
import functions.CalculateSelectionEfficiencies
import functions.CalculateXPDFScaleFactors


class ScriptRunner:
    def __init__(self, year, analysisname, t3_base_path, result_base_path, selectionstage, selectionname, backgrounds, signals):
        self.year = year
        self.analysisname  = analysisname
        self.t3_base_path  = t3_base_path
        self.selectionstage = selectionstage
        self.selectionname = selectionname
        self.user = os.environ['USER']
        self.backgrounds = backgrounds
        self.signals = signals

        self.crosssection_path   = os.path.join(os.environ['GENERATORPATH'], 'crosssections', 'ChiPsi')
        self.inputpath           = os.path.join(t3_base_path,     self.user, analysisname, year, selectionstage, selectionname)
        self.resultpath          = os.path.join(result_base_path, self.user, analysisname, year, selectionstage, selectionname)
        self.plotoutput_path     = os.path.join(self.resultpath, 'plots')
        self.dataoutput_path     = os.path.join(os.environ['ANALYZERPATH'], analysisname, 'data', year)
        ensureDirectory(self.plotoutput_path)

        # now bind functions
        ScriptRunner.CalculateLeptonReconstructionEfficiencies = functions.CalculateLeptonReconstructionEfficiencies.CalculateLeptonReconstructionEfficiencies
        ScriptRunner.CalculateLeptonIDEfficiencies = functions.CalculateLeptonIDEfficiencies.CalculateLeptonIDEfficiencies
        ScriptRunner.CalculateSelectionEfficiencies = functions.CalculateSelectionEfficiencies.CalculateSelectionEfficiencies
        ScriptRunner.CalculateXPDFScaleFactors = functions.CalculateXPDFScaleFactors.CalculateXPDFScaleFactors
        print green('--> Set up ScriptRunner for year %s:' % (str(year)))
        for key in self.__dict__.keys():
            print green('  --> %s: %s' % (str(key), str(self.__dict__[key])))
