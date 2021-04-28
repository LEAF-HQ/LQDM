
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
import ROOT
from ROOT import gROOT, TFile, TCanvas, TLatex, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed, kAzure, TGraphAsymmErrors, kFullCircle, kOpenCircle, kError
from tdrstyle_all import *

def CalculateSelectionEfficiencies(self, num_and_denom):
    print green('  --> Producing selection efficiencies.')
    gROOT.SetBatch(1)
    ROOT.gErrorIgnoreLevel = kError
    setTDRStyle()
    colors = [kRed+4, kRed+1, kAzure-2, kOrange, kGreen-2]

    legends = {
        "QCD_Had": "QCD",
        "TT":      "t#bar{t}",
        "ST":      "Single t",
        "DYJets":  "DY #rightarrow ll",
        "WJets":   "W+jets #rightarrow l#nu"
    }
    processes = self.signals + self.backgrounds

    graphs = OrderedDict()
    for signal in processes:
        infilename = os.path.join(self.inputpath, 'MC__'+signal+'.root')
        infile = TFile(infilename, 'READ')


        h_pass = TH1D('h_num', ';;Selection efficiency', len(num_and_denom), 0, len(num_and_denom))
        h_total = TH1D('h_denom', ';;Selection efficiency', len(num_and_denom), 0, len(num_and_denom))
        for i, (n, d) in enumerate(num_and_denom):
            h_pass.GetXaxis().SetBinLabel(i+1,n)
            h_total.GetXaxis().SetBinLabel(i+1,n)

        histname = 'sumweights'
        for i, (num, denom) in enumerate(num_and_denom):
            histfoldername_num = '_'.join([num, 'General'])
            histfoldername_denom = '_'.join([denom, 'General'])
            h_num = infile.Get(os.path.join(histfoldername_num, histname))
            h_denom = infile.Get(os.path.join(histfoldername_denom, histname))
            h_pass.SetBinContent(i+1, float(h_num.GetBinContent(1)))
            h_total.SetBinContent(i+1, float(h_denom.GetBinContent(1)))
            h_pass.SetBinError(i+1, float(h_num.GetBinError(1)))
            h_total.SetBinError(i+1, float(h_denom.GetBinError(1)))

        g = h_pass.Clone('ratio')
        g.SetDirectory(0)
        g.Divide(h_total)
        graphs[signal] = g

    c = TCanvas('c', 'c', 50, 50, 600, 600)
    c.SetFillColor(0)
    c.SetBorderMode(0)
    c.SetFrameFillStyle(0)
    c.SetFrameBorderMode(0)
    c.SetTopMargin(0.05)
    c.SetBottomMargin(0.14)
    c.SetLeftMargin(0.16)
    c.SetRightMargin(0.07)

    haxis = graphs[graphs.keys()[0]]
    haxis.SetMaximum(1.2)
    haxis.Draw('AXIS')
    legend = tdrLeg(0.45,0.65,0.95,0.9, textSize=0.025)
    idx_sig = 0
    idx_bkg = 0
    for i, signal in enumerate(graphs):
        if signal in legends.keys(): # backgrounds
            legtext = legends[signal]
            lstyle = 1
            tdrDraw(graphs[signal], "E", marker=kFullCircle, mcolor=colors[idx_bkg], lcolor=colors[idx_bkg], fstyle=0, lstyle=lstyle)
            idx_bkg += 1
        else: #signals
            mlq, mps, mch = get_mlq_mps_mch(signal)
            legtext = 'M_{#psi} = %i GeV, M_{DM} = %i GeV' % (mps, mch)
            lstyle = 2
            tdrDraw(graphs[signal], "E", marker=kFullCircle, mcolor=colors[idx_sig], lcolor=colors[idx_sig], fstyle=0, lstyle=lstyle)
            idx_sig += 1
        legend.AddEntry(graphs[signal], legtext, 'LP')

    c.SaveAs(os.path.join(self.plotoutput_path, 'SelectionEfficiencies.pdf'))












def get_mlq_mps_mch(samplename):
    parts = samplename.split('_')
    for part in parts:
        if 'MLQ' in part:
            mlq = int(part[3:])
        if 'MPS' in part:
            mps = int(part[3:])
        if 'MC1' in part:
            mch = int(part[3:])
    return (mlq, mps, mch)
