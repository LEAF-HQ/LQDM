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

    # allchannels = [['jets1'], ['jets2'], ['bjettight1'], ['dphi_j1_met'], ['dphi_j2_met'], ['ptratio_j2_met'], ['sphericity'], ['tach', 'much', 'elch', 'noch'], ['tach_addmu', 'tach_addel', 'tach_nolep', 'much', 'elch', 'noch']]
    # tags     = ['_metpt_1j', '_metpt_2j', '_metpt_2j1bt', '_metpt_2j1btdphi', '_metpt_2j1btdphi12', '_metpt_2j1btdphi12ptratio', '_metpt_2j1btdphi12ptratiosphericity', '_metpt_tachmuchelchnoch', '_metpt_tachsub3muchelchnoch']
    allchannels = [['sphericity']]
    tags     = ['_metpt_2j1btdphi12ptratiosphericity']
    signalgroups = [
        # ['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0'],
        # ['PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0'],
        # ['PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0'],
        ['PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0']
    ]


    for (channels, tag) in zip(allchannels, tags):
        Analyzer = run_limits(signalgroups, channels, tag)

    # Analyzer.CompareLimits(xaxistitle='M_{#psi} [GeV]', yaxistitle='#sigma(pp#rightarrow#psi#psi) [pb]', selectiontags=tags, colors=[ROOT.kRed+4, ROOT.kRed+1, ROOT.kViolet, ROOT.kAzure-2, ROOT.kOrange], signal_scaled_by=1., draw_theory=True)



def run_limits(signalgroups, channels, tag):
    systematics17  = ['nominal', 'lumi', 'rate_tt', 'rate_dyjets', 'rate_st', 'rate_vv', 'rate_wjets', 'rate_qcd']
    backgrounds17  = ['TT', 'DYJets', 'ST', 'WJets', 'QCDHad', 'VV']
    signalmass_identifier = 'MPS'
    # signals17      = ['PsiPsiToLQChi_MLQ1000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH457_MPS567_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH977_MPS1051_L1p0', 'PsiPsiToLQChi_MLQ1000_MCH2089_MPS2221_L1p0']
    # signals17      = ['PsiPsiToLQChi_MLQ3970_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH2089_MPS2551_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH457_MPS505_L1p0', 'PsiPsiToLQChi_MLQ3970_MCH977_MPS1106_L1p0']
    # signals17      = ['PsiPsiToLQChi_MLQ7030_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH2089_MPS2445_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ7030_MCH977_MPS1071_L1p0']
    # signals17      = ['PsiPsiToLQChi_MLQ10000_MCH100_MPS117_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH2089_MPS2342_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH214_MPS244_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH457_MPS504_L1p0', 'PsiPsiToLQChi_MLQ10000_MCH977_MPS1052_L1p0']



    channels17     = channels
    histnames_in_out_per_category17 = {
        None: ('metpt', 'MET')
    }
    limithisttag17 = 'General'
    selectiontag17 = tag

    processes_per_systematic17 = {'lumi': 'all', 'rate_tt': 'TT', 'rate_dyjets': 'DYJets', 'rate_st': 'ST', 'rate_wjets': 'WJets', 'rate_qcd': 'QCDHad', 'rate_vv': 'VV'}
    pdf_per_systematic17 = {'lumi': 'lnN', 'rate_tt': 'lnN', 'rate_dyjets': 'lnN', 'rate_st': 'lnN', 'rate_vv': 'lnN', 'rate_wjets': 'lnN', 'rate_qcd': 'lnN'}
    value_per_systematic17 = {'lumi': 1.025, 'rate_tt': 1.056, 'rate_dyjets': 1.1, 'rate_st': 1.1, 'rate_vv': 1.2, 'rate_wjets': 1.25, 'rate_qcd': 1.5}


    for signals in signalgroups:
        Analyzer17 = PostAnalyzer('UL17', analysisname=analysisname, input_base_path=input_base_path, result_base_path=result_base_path, selectionstage='Fullselection', selectionname='PsiPsi_05_Jets2_BTagTight1_DPhiJet1Met_DPhiJet2Met_PtratioJet2Met_LeptonCategories', systematics=systematics17, backgrounds=backgrounds17, signals=signals, signalmass_identifier=signalmass_identifier, channels=channels17, histnames_in_out_per_category=histnames_in_out_per_category17, limithisttag=limithisttag17, processes_per_systematic=processes_per_systematic17, pdf_per_systematic=pdf_per_systematic17, value_per_systematic=value_per_systematic17, crosssection_path='/work/areimers/CMSSW_10_6_28/src/LEAF/LQDM/Generator/crosssections/ChiPsi')

        Analyzer17.ProduceCombineHistograms(selectiontag=selectiontag17, signal_scaled_by=1.0)
        Analyzer17.CreateDatacards(selectiontag=selectiontag17)
        Analyzer17.CombineChannels(selectiontag=selectiontag17)
        Analyzer17.ExecuteCombineCombination(selectiontag=selectiontag17)
        Analyzer17.PlotLimits(xaxistitle='M_{#psi} [GeV]', yaxistitle='#sigma(pp#rightarrow#psi#psi) [pb]', selectiontag=selectiontag17, signal_scaled_by=1.0, draw_observed=False, draw_theory=True)

    # Analyzer17.PlotLimits2d(signalgroups=signalgroups, xsecvar='MPS', xsecconst='MLQ', xaxisdef = ('MLQ', 'M_{LQ} [GeV]'), yaxisdef = ('MCH', 'M_{#chi_{1}} [GeV]'), zaxistitle='Expected upper limit [pb]', selectiontag=selectiontag17, signal_scaled_by=1.0, draw_exclusion=True)

    return Analyzer17

if __name__ == '__main__':
    main()
