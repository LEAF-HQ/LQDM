
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
import ROOT
from ROOT import gROOT, TFile, TCanvas, TLatex, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed, kAzure, TGraphAsymmErrors, kFullCircle, kOpenCircle, kError
from tdrstyle_all import *

def CalculateLeptonIDEfficiencies(self):
    print green('  --> Producing lepton ID efficiencies.')
    gROOT.SetBatch(1)
    ROOT.gErrorIgnoreLevel = kError

    signalcategory = 'fromtau'
    backgroundcategories = ['fromhad', 'unmatched']
    histnametags_per_lepton = {
    'el': [''] + [signalcategory] + backgroundcategories,
    'mu': [''] + [signalcategory] + backgroundcategories
    }
    histclassname_per_lepton = {
    'el': ('Electrons', 'electron'),
    'mu': ('Muons', 'muon')
    }
    ids_per_lepton = {
    'mu': ['pteta', 'mvasoft', 'mvaloose', 'mvatight', 'cutsoft', 'cutloose', 'cuttight'],
    'el': ['pteta', 'cutveto', 'cutloose', 'cuttight', 'mvaisoloose', 'mvanonisoloose', 'mvaiso90', 'mvanoniso90']
    }
    colors = [kRed+4, kRed+1, kAzure-2, kOrange, kGreen-2]
    tags_denominator = {
    'iddenominator': 'WrtAllReco',
    'pteta': 'WrtPtEta',
    }

    for lep in ids_per_lepton.keys():
        eff_sig_per_id = {}
        eff_bkg_per_id = {}
        eff_had_per_id = {}
        eff_unm_per_id = {}
        for id in ids_per_lepton[lep]:
            eff_sig_per_denomtag = {}
            eff_bkg_per_denomtag = {}
            eff_had_per_denomtag = {}
            eff_unm_per_denomtag = {}
            for denomtag in tags_denominator.keys():
                eff_sig_per_signal = {}
                eff_bkg_per_signal = {}
                eff_had_per_signal = {}
                eff_unm_per_signal = {}
                npass_sig_per_signal = {}
                npass_bkg_per_signal = {}
                npass_had_per_signal = {}
                npass_unm_per_signal = {}
                ntot_sig_per_signal = {}
                ntot_bkg_per_signal = {}
                ntot_had_per_signal = {}
                ntot_unm_per_signal = {}
                for signal in self.signals:
                    npass_sig_per_signal[signal] = 0
                    ntot_sig_per_signal[signal]  = 0
                    npass_bkg_per_signal[signal] = 0
                    ntot_bkg_per_signal[signal]  = 0
                    npass_had_per_signal[signal] = 0
                    ntot_had_per_signal[signal]  = 0
                    npass_unm_per_signal[signal] = 0
                    ntot_unm_per_signal[signal]  = 0
                for tag in histnametags_per_lepton[lep]:
                    is_signal = tag == signalcategory
                    is_background = tag in backgroundcategories
                    is_had = tag == 'fromhad'
                    is_unmatched = tag == 'unmatched'
                    hists_total = []
                    hists_pass  = []
                    ratios      = []
                    hists_n_total = []
                    hists_n_pass  = []

                    for signal in self.signals:
                        infilename = os.path.join(self.inputpath, 'MC__'+signal+'.root')
                        infile = TFile(infilename, 'READ')

                        hfn_den = '_'.join([denomtag, histclassname_per_lepton[lep][0]])
                        hfn_num = '_'.join([lep, id, histclassname_per_lepton[lep][0]])

                        h_total = infile.Get('/'.join([hfn_den, histclassname_per_lepton[lep][1]+tag+'pt_rebin2']))
                        h_total.SetDirectory(0)
                        hists_total.append(h_total)
                        h_pass = infile.Get('/'.join([hfn_num, histclassname_per_lepton[lep][1]+tag+'pt_rebin2']))
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
                        if is_signal:
                            npass_sig_per_signal[signal] += float(npass)
                            ntot_sig_per_signal[signal]  += float(ntotal)
                        if is_background:
                            npass_bkg_per_signal[signal] += float(npass)
                            ntot_bkg_per_signal[signal]  += float(ntotal)
                        if is_had:
                            npass_had_per_signal[signal] += float(npass)
                            ntot_had_per_signal[signal]  += float(ntotal)
                        if is_unmatched:
                            npass_unm_per_signal[signal] += float(npass)
                            ntot_unm_per_signal[signal]  += float(ntotal)

                        leg.AddEntry(hists_pass[idx], 'M_{#psi} = %i GeV, M_{DM} = %i GeV (tot. eff: %.2f)' % (mps, mch, total_eff), 'L')
                        c.cd(2)
                        tdrDraw(ratios[idx], "P", marker=kFullCircle, mcolor=colors[idx], lcolor=colors[idx], fstyle=0)

                    tagstr = '' if tag == '' else '_' + tag
                    c.SaveAs(os.path.join(self.plotoutput_path, 'IDEfficiency_%s_%s_%s%s.pdf' % (lep, tags_denominator[denomtag], id, tagstr)))

                for signal in self.signals:
                    eff_sig_per_signal[signal] = npass_sig_per_signal[signal]/ntot_sig_per_signal[signal]
                    eff_bkg_per_signal[signal] = npass_bkg_per_signal[signal]/ntot_bkg_per_signal[signal]
                    eff_had_per_signal[signal] = npass_had_per_signal[signal]/ntot_had_per_signal[signal]
                    eff_unm_per_signal[signal] = npass_unm_per_signal[signal]/ntot_unm_per_signal[signal]
                eff_sig_per_denomtag[denomtag] = eff_sig_per_signal
                eff_bkg_per_denomtag[denomtag] = eff_bkg_per_signal
                eff_had_per_denomtag[denomtag] = eff_had_per_signal
                eff_unm_per_denomtag[denomtag] = eff_unm_per_signal
            eff_sig_per_id[id] = eff_sig_per_denomtag
            eff_bkg_per_id[id] = eff_bkg_per_denomtag
            eff_had_per_id[id] = eff_had_per_denomtag
            eff_unm_per_id[id] = eff_unm_per_denomtag


        for signal in self.signals:
            for denomtag in tags_denominator.keys():
                graphs_bkg = []
                graphs_had = []
                graphs_unm = []
                legends_bkg = []
                legends_had = []
                legends_unm = []
                for id in ids_per_lepton[lep]:

                    graph_bkg = TGraph()
                    graph_bkg.SetPoint(0, eff_sig_per_id[id][denomtag][signal], eff_bkg_per_id[id][denomtag][signal])
                    legends_bkg.append(id)
                    graphs_bkg.append(graph_bkg)

                    graph_had = TGraph()
                    graph_had.SetPoint(0, eff_sig_per_id[id][denomtag][signal], eff_had_per_id[id][denomtag][signal])
                    legends_had.append(id)
                    graphs_had.append(graph_had)

                    graph_unm = TGraph()
                    graph_unm.SetPoint(0, eff_sig_per_id[id][denomtag][signal], eff_unm_per_id[id][denomtag][signal])
                    legends_unm.append(id)
                    graphs_unm.append(graph_unm)
                cgrbkg = tdrCanvas('cgrbkg', 0, 1, 1E-3, 1, '%s from #tau efficiency' % (histclassname_per_lepton[lep][1]), 'other %s efficiency' % (histclassname_per_lepton[lep][1]), True, 0, 11)
                legend_bkg = tdrLeg(0.25,0.65,0.85,0.9, textSize=0.025)
                for i in range(len(graphs_bkg)):
                    g = graphs_bkg[i]
                    l = legends_bkg[i]
                    tdrDraw(g, "P", marker=kFullCircle, mcolor=i+1, lcolor=i+1, fstyle=0)
                    legend_bkg.AddEntry(g, l, 'P')
                cgrbkg.SetLogy()
                cgrbkg.SaveAs(os.path.join(self.plotoutput_path, 'IDEfficiencyVsBackground_%s_%s_%s.pdf' % (lep, signal, tags_denominator[denomtag])))

                cgrhad = tdrCanvas('cgrhad', 0, 1, 1E-3, 1, '%s from #tau efficiency' % (histclassname_per_lepton[lep][1]), '%s from had. efficiency' % (histclassname_per_lepton[lep][1]), True, 0, 11)
                legend_had = tdrLeg(0.25,0.65,0.85,0.9, textSize=0.025)
                for i in range(len(graphs_had)):
                    g = graphs_had[i]
                    l = legends_had[i]
                    tdrDraw(g, "P", marker=kFullCircle, mcolor=i+1, lcolor=i+1, fstyle=0)
                    legend_had.AddEntry(g, l, 'P')
                cgrhad.SetLogy()
                cgrhad.SaveAs(os.path.join(self.plotoutput_path, 'IDEfficiencyVsHadronic_%s_%s_%s.pdf' % (lep, signal, tags_denominator[denomtag])))

                cgrunm = tdrCanvas('cgrunm', 0, 1, 1E-3, 1, '%s from #tau efficiency' % (histclassname_per_lepton[lep][1]), 'unmatched %s efficiency' % (histclassname_per_lepton[lep][1]), True, 0, 11)
                legend_unm = tdrLeg(0.25,0.65,0.85,0.9, textSize=0.025)
                for i in range(len(graphs_unm)):
                    g = graphs_unm[i]
                    l = legends_unm[i]
                    tdrDraw(g, "P", marker=kFullCircle, mcolor=i+1, lcolor=i+1, fstyle=0)
                    legend_unm.AddEntry(g, l, 'P')
                cgrunm.SetLogy()
                # os.path.join(self.plotoutput_path, 'ReconstructionEfficiency_%s_%s_%s%s.pdf' % (lep, histfolders_denom[histfolder_denom], id, tagstr))
                cgrunm.SaveAs(os.path.join(self.plotoutput_path, 'IDEfficiencyVsUnmatched_%s_%s_%s.pdf' % (lep, signal, tags_denominator[denomtag])))











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
