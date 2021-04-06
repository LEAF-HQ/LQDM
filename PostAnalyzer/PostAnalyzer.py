import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
import functions.ProduceCombineHistograms
import functions.PlotLimits
import functions.Combine
# import ROOT


class PostAnalyzer:
    def __init__(self, year, analysisname, t3_base_path, result_base_path, selectionname, systematics, backgrounds, signals, channels, histnames_in_out_per_category, limithisttag, processes_per_systematic, pdf_per_systematic, value_per_systematic):
        self.year = year
        self.analysisname  = analysisname
        self.t3_base_path  = t3_base_path
        self.selectionname = selectionname
        self.user = os.environ['USER']
        self.systematics = systematics
        self.backgrounds = backgrounds
        self.signals = signals
        self.channels = channels
        self.histnames_in_out_per_category = histnames_in_out_per_category
        self.limithisttag = limithisttag
        self.processes_per_systematic = processes_per_systematic
        self.pdf_per_systematic = pdf_per_systematic
        self.value_per_systematic = value_per_systematic

        self.crosssection_path   = os.path.join(os.environ['GENERATORPATH'], 'crosssections', 'ChiPsi')
        self.finalselection_path = os.path.join(t3_base_path,     self.user, analysisname, year, 'Finalselection', selectionname)
        self.resultpath          = os.path.join(result_base_path, self.user, analysisname, year, 'Finalselection', selectionname)
        self.combineinput_path   = os.path.join(self.resultpath, 'data', 'combineinput')
        self.combineoutput_path  = os.path.join(self.resultpath, 'data', 'combineoutput')
        self.plotoutput_path     = os.path.join(self.resultpath, 'plots')
        ensureDirectory(self.combineinput_path)
        ensureDirectory(self.combineoutput_path)

        # now bind functions
        PostAnalyzer.ProduceCombineHistograms  = functions.ProduceCombineHistograms.ProduceCombineHistograms
        PostAnalyzer.CreateDatacards           = functions.Combine.CreateDatacards
        PostAnalyzer.CombineChannels           = functions.Combine.CombineChannels
        PostAnalyzer.ExecuteCombineCombination = functions.Combine.ExecuteCombineCombination
        PostAnalyzer.PlotLimits                = functions.PlotLimits.PlotLimits
        print green('--> Set up PostAnalyzer for year %s:' % (str(year)))
        for key in self.__dict__.keys():
            print green('  --> %s: %s' % (str(key), str(self.__dict__[key])))
