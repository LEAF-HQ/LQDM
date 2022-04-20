
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
import ROOT
from ROOT import gROOT, TFile, TCanvas, TLatex, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed, kAzure, kViolet, TGraphAsymmErrors, kFullCircle, kOpenCircle, kError
from tdrstyle_all import *

def CalculateSelectionEfficiencies(self, backgrounds, signals, num_and_denom):
    print green('  --> Producing selection efficiencies.')
    gROOT.SetBatch(1)
    ROOT.gErrorIgnoreLevel = kError
    setTDRStyle()
    # colors = [kRed+4, kRed+1, kViolet, kAzure-2, kOrange, kGreen-2]
    # colors = [kRed+1, kAzure-2, kOrange, kGreen-2, kViolet, kRed+4]

    # # legends = {
    #     "QCDHad": "QCD",
    #     "TT":      "t#bar{t}",
    #     "VV":      "VV",
    #     "ST":      "Single t",
    #     "DYJets":  "DY #rightarrow ll",
    #     "WJets":   "W+jets #rightarrow l#nu",
    #     # 'LQLQToBTau_MLQ1000_MPS117_MC1100_L1p0': 'LQ LQ #rightarrow b#tau b#tau',
    #     # 'LQLQToBTauPsiChi_MLQ1000_MPS117_MC1100_L1p0': 'LQ LQ #rightarrow b#tau #psi#chi_{1}',
    #     # 'LQLQToPsiChi_MLQ1000_MPS117_MC1100_L1p0': 'LQ LQ #rightarrow #psi#chi_{1} #psi#chi_{1}',
    #     # 'PsiPsiToLQChi_MLQ1000_MPS2221_MC12089_L1p0': '#psi #psi #rightarrow LQ#chi_{1} LQ#chi_{1}'
    # }
    processes = signals + backgrounds

    graphs = OrderedDict()
    for process in processes:
        infilename = os.path.join(self.inputpath, 'MC__'+process+'.root')
        infile = TFile(infilename, 'READ')
        print infilename


        h_pass = TH1D('h_num', ';Selection step;Selection efficiency', len(num_and_denom), 0, len(num_and_denom))
        h_total = TH1D('h_denom', ';Selection step;Selection efficiency', len(num_and_denom), 0, len(num_and_denom))
        for i, (n, d) in enumerate(num_and_denom):
            h_pass.GetXaxis().SetBinLabel(i+1,n)
            h_total.GetXaxis().SetBinLabel(i+1,n)

        histname = 'sumweights'
        for i, (num, denom) in enumerate(num_and_denom):
            histfoldername_num = '_'.join([num, 'General'])
            histfoldername_denom = '_'.join([denom, 'General'])
            h_num = infile.Get(os.path.join(histfoldername_num, histname))
            print os.path.join(histfoldername_num, histname)
            h_denom = infile.Get(os.path.join(histfoldername_denom, histname))
            h_pass.SetBinContent(i+1, float(h_num.GetBinContent(1)))
            h_total.SetBinContent(i+1, float(h_denom.GetBinContent(1)))
            h_pass.SetBinError(i+1, float(h_num.GetBinError(1)))
            h_total.SetBinError(i+1, float(h_denom.GetBinError(1)))

        g = h_pass.Clone('ratio')
        g.SetDirectory(0)
        g.Divide(h_total)
        graphs[process] = g

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
    haxis.SetMinimum(0.)
    haxis.Draw('AXIS')
    legend = tdrLeg(0.25,0.72,0.75,0.93, textSize=0.025)
    for i, process in enumerate(graphs):
        if process in backgrounds: # backgrounds
            lstyle = 1
            tdrDraw(graphs[process], "E", marker=kFullCircle, mcolor=self.samples[process].color, lcolor=self.samples[process].color, fstyle=0, lstyle=self.samples[process].linestyle)
        else: #signals
            mlq, mps, mch = get_mlq_mps_mch(process)
            lstyle = 2
            tdrDraw(graphs[process], "E", marker=kFullCircle, mcolor=self.samples[process].color, lcolor=self.samples[process].color, fstyle=0, lstyle=self.samples[process].linestyle)

        legtext = self.samples[process].legendtext
        legend.AddEntry(graphs[process], self.samples[process].legendtext, 'LP')

    plotoutname = '_'.join([self.plotprefix, 'SelectionEfficiencies.pdf'])
    c.SaveAs(os.path.join(self.plotoutput_path, plotoutname))

    haxis.SetMaximum(1E2)
    haxis.SetMinimum(1E-4)
    c.SetLogy()
    c.SaveAs(os.path.join(self.plotoutput_path, plotoutname.replace('.pdf', '_logy.pdf')))













def get_mlq_mps_mch(samplename):
    parts = samplename.split('_')
    for part in parts:
        if 'MLQ' in part:
            mlq = int(part[3:])
        if 'MPS' in part:
            mps = int(part[3:])
        if 'MCH' in part:
            mch = int(part[3:])
    return (mlq, mps, mch)
