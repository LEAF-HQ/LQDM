
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
import ROOT
from ROOT import gROOT, TFile, TCanvas, TLatex, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed, kAzure, TGraphAsymmErrors, kFullCircle, kOpenCircle, kError
from tdrstyle_all import *

def CalculateLeptonReconstructionEfficiencies(self):
    print green('  --> Producing lepton reconstruction efficiencies.')
    gROOT.SetBatch(1)
    ROOT.gErrorIgnoreLevel = kError

    lep_flavors = ['vistau', 'el', 'mu']
    histnametags_per_lepton = {
    'el': ['', 'fromtau', 'fromhad'],
    'mu': ['', 'fromtau', 'fromhad'],
    'vistau': ['']
    }
    histclassname_per_lepton = {
    'el': 'electron',
    'mu': 'muon',
    'vistau': 'vistau'
    }

    ids_per_lepton = {
    'mu': ['iddenominator', 'pteta', 'mu_mvasoft', 'mu_pteta', 'mu_mvaloose', 'mu_mvatight', 'mu_cutsoft', 'mu_cutloose', 'mu_cuttight'],
    'el': ['iddenominator', 'pteta', 'el_cutveto', 'el_pteta', 'el_cutloose', 'el_cuttight', 'el_mvaisoloose', 'el_mvanonisoloose', 'el_mvaiso90', 'el_mvanoniso90'],
    'vistau': ['iddenominator', 'pteta', 'tau_vsjetvvvloose', 'tau_vsjetvvloose', 'tau_vsjetvloose', 'tau_vsjetloose', 'tau_vsjetmedium', 'tau_vsjettight', 'tau_vsjetvtight', 'tau_vsjetvvtight']
    }

    histfolders_denom = {
    'SAME':  'WrtAllGen',
    'pteta': 'WrtPtEta'
    }
    colors = [kRed+4, kRed+1, kAzure-2, kOrange, kGreen-2]


    for lep in lep_flavors:
        for id in ids_per_lepton[lep]:
            for tag in histnametags_per_lepton[lep]:
                for histfolder_denom in histfolders_denom:
                    hists_total = []
                    hists_pass  = []
                    ratios      = []
                    hists_n_total = []
                    hists_n_pass  = []

                    for signal in self.signals:
                        infilename = os.path.join(self.inputpath, 'MC__'+signal+'.root')
                        infile = TFile(infilename, 'READ')

                        # '_'.join([tag_denominator, histclassname_per_lepton[lep])
                        histfoldername_num = '_'.join([id, 'GenParticles'])
                        histfoldername_denom = histfoldername_num if histfolder_denom == 'SAME' else '_'.join([histfolder_denom, 'GenParticles'])

                        varnum = 'ptgen'+lep+tag+'matched_rebin2'
                        vardenom = 'ptgen'+lep+tag+'_rebin2' if histfolder_denom == 'SAME' else varnum
                        nnum = 'gen'+lep+'s'+tag+'matched'
                        ndenom = 'gen'+lep+'s'+tag+'total' if histfolder_denom == 'SAME' else nnum

                        h_total = infile.Get('/'.join([histfoldername_denom, vardenom]))
                        h_total.SetDirectory(0)
                        hists_total.append(h_total)
                        h_n_total = infile.Get('/'.join([histfoldername_denom, ndenom]))
                        h_n_total.SetDirectory(0)
                        hists_n_total.append(h_n_total)
                        h_pass = infile.Get('/'.join([histfoldername_num, varnum]))
                        h_pass.SetDirectory(0)
                        hists_pass.append(h_pass)
                        h_n_pass = infile.Get('/'.join([histfoldername_num, nnum]))
                        h_n_pass.SetDirectory(0)
                        hists_n_pass.append(h_n_pass)
                        ratio = TGraphAsymmErrors(h_pass, h_total, 'cl=0.683 b(1,1) mode')
                        ratios.append(ratio)

                    c = tdrDiCanvas('c', hists_total[0].GetBinLowEdge(1), hists_total[0].GetBinLowEdge(hists_total[0].GetNbinsX()+1), 1E-2, 1E8, 0.3,     1.7, hists_total[0].GetXaxis().GetTitle(), hists_total[0].GetYaxis().GetTitle(), 'efficiency', True, 0, 11)
                    leg = tdrLeg(0.25,0.65,0.85,0.9, textSize=0.025)

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
                    c.SaveAs(os.path.join(self.plotoutput_path, 'ReconstructionEfficiency_%s_%s_%s%s.pdf' % (lep, histfolders_denom[histfolder_denom], id, tagstr)))

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
