
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from ROOT import gROOT, TFile, TCanvas, TLatex, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed, kAzure, TGraphAsymmErrors, kFullCircle, kOpenCircle
from tdrstyle_all import *

def CalculateLeptonIDEfficiencies(self):
    print green('  --> Producing lepton reconstruction efficiencies.')
    gROOT.SetBatch(1)

    histnametags_per_lepton = {
    'el': ['fromtau', 'fromhad', 'unmatched'],
    'mu': ['fromtau', 'fromhad', 'unmatched']
    }
    histclassname_per_lepton = {
    'el': ('Electrons', 'electron'),
    'mu': ('Muons', 'muon')
    }
    ids_per_lepton = {
    'mu': ['mvasoft', 'mvaloose', 'mvatight', 'cutsoft', 'cutloose', 'cuttight'],
    'el': ['cutveto', 'cutloose', 'cuttight', 'mvaisoloose', 'mvanonisoloose', 'mvaiso90', 'mvanoniso90']
    }
    colors = [kRed+4, kRed+1, kAzure-2, kOrange, kGreen-2]
    tag_denominator = 'iddenominator'

    for lep in ids_per_lepton.keys():
        for id in ids_per_lepton[lep]:
            for tag in histnametags_per_lepton[lep]:
                hists_total = []
                hists_pass  = []
                ratios      = []
                hists_n_total = []
                hists_n_pass  = []

                for signal in self.signals:
                    infilename = os.path.join(self.inputpath, 'MC__'+signal+'.root')
                    infile = TFile(infilename, 'READ')

                    hfn_den = '_'.join([tag_denominator, histclassname_per_lepton[lep][0]])
                    hfn_num = '_'.join([lep, id, histclassname_per_lepton[lep][0]])

                    # nmuonsfromtau
                    print '/'.join([hfn_den, histclassname_per_lepton[lep][1] + tag + 'pt_rebin'])
                    h_total = infile.Get('/'.join([hfn_den, histclassname_per_lepton[lep][1]+tag+'pt_rebin']))
                    h_total.SetDirectory(0)
                    hists_total.append(h_total)
                    h_pass = infile.Get('/'.join([hfn_num, histclassname_per_lepton[lep][1]+tag+'pt_rebin']))
                    h_pass.SetDirectory(0)
                    hists_pass.append(h_pass)
                    h_n_total = infile.Get('/'.join([hfn_den, 'n'+histclassname_per_lepton[lep][1]+'s'+tag]))
                    h_n_total.SetDirectory(0)
                    hists_n_total.append(h_n_total)
                    h_n_pass = infile.Get('/'.join([hfn_num, 'n'+histclassname_per_lepton[lep][1]+'s'+tag]))
                    h_n_pass.SetDirectory(0)
                    hists_n_pass.append(h_n_pass)
                    ratio = TGraphAsymmErrors(h_pass, h_total, 'cl=0.683 b(1,1) mode')
                    ratios.append(ratio)
                    print len(ratios)

                    c = tdrDiCanvas('c', hists_total[0].GetBinLowEdge(1), hists_total[0].GetBinLowEdge(hists_total[0].GetNbinsX()+1), 1E-2, 1E8, 0.3,     1.7, hists_total[0].GetXaxis().GetTitle(), hists_total[0].GetYaxis().GetTitle(), 'efficiency', True, 0, 11)
                    leg = tdrLeg(0.25,0.65,0.85,0.9, textSize=0.025)

                len(self.signals)
                for idx, signal in enumerate(self.signals):
                    mlq, mps, mch = get_mlq_mps_mch(signal)
                    c.cd(1).SetLogy()
                    tdrDraw(hists_total[idx], "E", marker=kOpenCircle, mcolor=colors[idx], lcolor=colors[idx], fstyle=0)
                    tdrDraw(hists_pass[idx], "E", marker=kFullCircle, mcolor=colors[idx], lcolor=colors[idx], fstyle=0)
                    npass = 0
                    ntotal = 0
                    for i in range(hists_n_pass[idx].GetNbinsX()):
                        npass += hists_n_pass[idx].GetBinContent(i+1) * hists_n_pass[idx].GetBinCenter(i+1)
                        ntotal += hists_n_total[idx].GetBinContent(i+1) * hists_n_total[idx].GetBinCenter(i+1)
                    total_eff = npass / ntotal
                    leg.AddEntry(hists_pass[idx], 'M_{#psi} = %i GeV, M_{DM} = %i GeV (tot. eff: %.2f)' % (mps, mch, total_eff), 'L')
                    c.cd(2)
                    tdrDraw(ratios[idx], "P", marker=kFullCircle, mcolor=colors[idx], lcolor=colors[idx], fstyle=0)

                tagstr = '' if tag == '' else '_' + tag
                c.SaveAs(os.path.join(self.plotoutput_path, 'IDEfficiency_%s_%s%s.pdf' % (lep, id, tagstr)))

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
