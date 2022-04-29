#! /usr/bin/env python

import os, sys, math, time
import subprocess
from utils import *
from SampleSettings import *
from collections import OrderedDict
from PostAnalyzer import *
from ScriptRunner import *
import ROOT

# All constants to be used
analysisname     = 'LQDM'
user = 'areimers'
# analyzer_path    = os.environ['ANALYZERPATH'] # set up by setup.sh
# lqdm_path        = os.path.join(analyzer_path, analysisname)
input_base_path     = os.path.join('/pnfs/psi.ch/cms/trivcat/store/user', user)
result_base_path = os.path.join('/work', user)

# colors = [ROOT.kRed+4, ROOT.kRed+1, ROOT.kViolet, ROOT.kAzure-2, ROOT.kOrange, ROOT.kGreen-2]
samples = {
    'PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', color=ROOT.kRed+4, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=0.12 TeV, light LQ)'),
    'PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0', color=ROOT.kRed+1, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=0.21 TeV, light LQ)'),
    'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0':          SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', color=ROOT.kViolet, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=0.57 TeV, light LQ)'),
    'PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0':         SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0', color=ROOT.kAzure-2, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=1.05 TeV, light LQ)'),
    'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0':        SampleSettings(name='PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0', color=ROOT.kOrange, linestyle=2, legendtext='#psi #psi #rightarrow LQ#chi LQ#chi (M_{#psi}=2.22 TeV, light LQ)'),
}


def main():


    systematics17  = ['nominal', 'lumi', 'rate_tt', 'rate_dyjets', 'rate_st', 'rate_vv', 'rate_wjets', 'rate_qcd']
    backgrounds17  = ['TT', 'DYJets', 'ST', 'WJets', 'QCDHad', 'VV']
    signalmass_identifier = 'MPS'
    signals17      = ['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0']
    channels17     = ['tach_addmu', 'tach_addel', 'tach_nolep', 'much', 'elch', 'noch']
    histnames_in_out_per_category17 = {
        None: ('metpt', 'MET')
    }
    limithisttag17 = 'General'
    selectiontag17 = '_metpt_tachsub3muchelchnoch'

    processes_per_systematic17 = {'lumi': 'all', 'rate_tt': 'TT', 'rate_dyjets': 'DYJets', 'rate_st': 'ST', 'rate_wjets': 'WJets', 'rate_qcd': 'QCDHad', 'rate_vv': 'VV'}
    pdf_per_systematic17 = {'lumi': 'lnN', 'rate_tt': 'lnN', 'rate_dyjets': 'lnN', 'rate_st': 'lnN', 'rate_vv': 'lnN', 'rate_wjets': 'lnN', 'rate_qcd': 'lnN'}
    value_per_systematic17 = {'lumi': 1.025, 'rate_tt': 1.056, 'rate_dyjets': 1.1, 'rate_st': 1.1, 'rate_vv': 1.2, 'rate_wjets': 1.25, 'rate_qcd': 1.5}

    Analyzer17 = PostAnalyzer('UL17', analysisname=analysisname, input_base_path=input_base_path, result_base_path=result_base_path, selectionstage='Fullselection', selectionname='PsiPsi_04_Jets2_BTagTight1_DPhiJet1Met_PtratioJet2Met', systematics=systematics17, backgrounds=backgrounds17, signals=signals17, signalmass_identifier=signalmass_identifier, channels=channels17, histnames_in_out_per_category=histnames_in_out_per_category17, limithisttag=limithisttag17, processes_per_systematic=processes_per_systematic17, pdf_per_systematic=pdf_per_systematic17, value_per_systematic=value_per_systematic17, crosssection_path='/work/areimers/CMSSW_10_6_28/src/LEAF/LQDM/Generator/crosssections/ChiPsi')


    Analyzer17.ProduceCombineHistograms(selectiontag=selectiontag17, signal_scaled_by=1.)
    Analyzer17.CreateDatacards(selectiontag=selectiontag17)
    Analyzer17.CombineChannels(selectiontag=selectiontag17)
    Analyzer17.ExecuteCombineCombination(selectiontag=selectiontag17)
    Analyzer17.PlotLimits(xaxistitle='M_{#psi} [GeV]', yaxistitle='#sigma(pp#rightarrow#psi#psi) [pb]', selectiontag=selectiontag17, signal_scaled_by=1., draw_observed=False, draw_theory=True)
    Analyzer17.CompareLimits(xaxistitle='M_{#psi} [GeV]', yaxistitle='#sigma(pp#rightarrow#psi#psi) [pb]', selectiontags=['_metpt_1j', '_metpt_2j', '_metpt_2j1bt', '_metpt_2j1btdphi', '_metpt_2j1btdphiptratio', '_metpt_2j1btdphiptrationoch', '_metpt_muchelchnoch', '_metpt_tachmuchelchnoch', '_metpt_tachsub3muchelchnoch'], colors=[ROOT.kRed+4, ROOT.kRed+1, ROOT.kViolet, ROOT.kAzure-2, ROOT.kOrange], signal_scaled_by=1., draw_theory=True)

if __name__ == '__main__':
    main()
