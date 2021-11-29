
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
import ROOT
from ROOT import gROOT, TFile, TCanvas, TLatex, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed, kAzure, TGraphAsymmErrors, kFullCircle, kOpenCircle, kError
from tdrstyle_all import *

def CalculateXPDFScaleFactors(self, processes_source, processes_target):
    print green('  --> Producing x * PDF scale factors.')
    gROOT.SetBatch(1)
    ROOT.gErrorIgnoreLevel = kError

    if not len(processes_source) == len(processes_target):
        raise AttributeError('In CalculateXPDFScaleFactors(): The lists of source and target processes must be of the same length and ordered correctly.')

    sfs = []
    for i, ps in enumerate(processes_source):
        pt = processes_target[i]

        # can safely prepend it with "MC__", since these SFs are only done for MC anyway
        targetfilename = os.path.join(self.inputpath, 'MC__'+pt+'.root')
        sourcefilename = os.path.join(self.inputpath, 'MC__'+ps+'.root')

        ft = TFile(targetfilename, 'READ')
        fs = TFile(sourcefilename, 'READ')

        ht = ft.Get('genmet_GenInfo/geninfoxpdfproduct_rebin4')
        hs = fs.Get('genmet_GenInfo/geninfoxpdfproduct_rebin4')

        sf = ht.Clone()
        sf.Divide(hs)
        sf.SetName(ps)
        sf.SetDirectory(0)
        sf.GetXaxis().SetTitle('x1*PDF1 * x2*PDF2')
        sfs.append(sf)

        ft.Close()
        fs.Close()

    outfilename = os.path.join(self.dataoutput_path, 'XPDFScaleFactors.root')
    fo = TFile(outfilename, 'RECREATE')
    for sf in sfs:
        print type(sf)

        sf.Write()
    fo.Close()
    print green('  --> Created file \'%s\'' % (outfilename))



    return True
