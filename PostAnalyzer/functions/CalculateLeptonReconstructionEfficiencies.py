
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from ROOT import gROOT, TFile, TCanvas, TLatex, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed, kAzure, TGraphAsymmErrors, kFullCircle, kOpenCircle
from tdrstyle_all import *

def CalculateLeptonReconstructionEfficiencies(self):
    print green('  --> Producing lepton reconstruction efficiencies.')
    gROOT.SetBatch(1)

    lep_flavors = ['vistau', 'el', 'mu']
    # lep_flavors = ['el', 'mu']
    histnametags_per_lepton = {
    'el': ['', 'fromtau'],
    'mu': ['', 'fromtau'],
    'vistau': ['']
    }
    selsteps = ['input', 'hadcleaner', 'jettaucleaner', 'muon_mvasoft', 'muon_mvaloose', 'muon_mvatight', 'muon_cutsoft', 'muon_cutloose', 'muon_cuttight', 'electron_cutveto', 'electron_cutloose', 'electron_cuttight', 'electron_mvaisoloose', 'electron_mvanonisoloose', 'electron_mvaiso90', 'electron_mvanoniso90']
    colors = [kRed+4, kRed+1, kAzure-2, kOrange, kGreen-2]

    for lep in lep_flavors:
        for step in selsteps:
            for tag in histnametags_per_lepton[lep]:
                hists_total = []
                hists_pass  = []
                ratios      = []
                hists_n_total = []
                hists_n_pass  = []

                for signal in self.signals:
                    infilename = os.path.join(self.inputpath, 'MC__'+signal+'.root')
                    infile = TFile(infilename, 'READ')
                    histfoldername = step+'_GenParticles'
                    h_total = infile.Get('/'.join([histfoldername, 'ptgen'+lep+tag+'_rebin2']))
                    h_total.SetDirectory(0)
                    hists_total.append(h_total)
                    h_n_total = infile.Get('/'.join([histfoldername, 'gen'+lep+'s'+tag+'total']))
                    h_n_total.SetDirectory(0)
                    hists_n_total.append(h_n_total)
                    h_pass = infile.Get('/'.join([histfoldername, 'ptgen'+lep+tag+'matched_rebin2']))
                    h_pass.SetDirectory(0)
                    hists_pass.append(h_pass)
                    h_n_pass = infile.Get('/'.join([histfoldername, 'gen'+lep+'s'+tag+'matched']))
                    h_n_pass.SetDirectory(0)
                    hists_n_pass.append(h_n_pass)
                    ratio = TGraphAsymmErrors(h_pass, h_total, 'cl=0.683 b(1,1) mode')
                    ratios.append(ratio)

                print len(ratios)

                c = tdrDiCanvas('c', hists_total[0].GetBinLowEdge(1), hists_total[0].GetBinLowEdge(hists_total[0].GetNbinsX()+1), 1E-2, 1E8, 0.3,     1.7, hists_total[0].GetXaxis().GetTitle(), hists_total[0].GetYaxis().GetTitle(), 'efficiency', True, 0, 11)
                leg = tdrLeg(0.25,0.65,0.85,0.9, textSize=0.025)

                # c.cd(0)
                for idx, signal in enumerate(self.signals):
                    mlq, mps, mch = get_mlq_mps_mch(signal)
                    c.cd(1).SetLogy()
                    tdrDraw(hists_total[idx], "E", marker=kOpenCircle, mcolor=colors[idx], lcolor=colors[idx], fstyle=0)
                    tdrDraw(hists_pass[idx], "E", marker=kFullCircle, mcolor=colors[idx], lcolor=colors[idx], fstyle=0)
                    total_eff = hists_n_pass[idx].GetBinContent(1) / hists_n_total[idx].GetBinContent(1)
                    leg.AddEntry(hists_pass[idx], 'M_{#psi} = %i GeV, M_{DM} = %i GeV (tot. eff: %.2f)' % (mps, mch, total_eff), 'L')

                    c.cd(2)
                    tdrDraw(ratios[idx], "P", marker=kFullCircle, mcolor=colors[idx], lcolor=colors[idx], fstyle=0)

                tagstr = '' if tag == '' else '_' + tag
                c.SaveAs(os.path.join(self.plotoutput_path, 'ReconstructionEfficiency_%s_%s%s.pdf' % (step, lep, tagstr)))

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
