
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from ROOT import gROOT, TFile, TCanvas, TH1D, TTree, TGraph, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed
from array import array
from tdrstyle_all import *
import constants

def CompareLimits(self, xaxistitle, yaxistitle, selectiontags, colors, signal_scaled_by=1., draw_theory=False):
    print green('  --> comparing %i expected limits.' % (len(selectiontags)))
    # Expects all signals to belong to the same process!

    gROOT.SetBatch(1)

    graphs_per_selectiontag = {}
    numbers_per_selectiontag = {}
    for selectiontag in selectiontags:
        limits_and_quantiles_per_mass = {}
        for signal in self.signals:
            parts = signal.split('_')
            signaltag = ''
            signalmass = -1
            for part in parts:
                if not self.signalmass_identifier in part:
                    if signaltag == '': signaltag = part
                    else: signaltag = '_'.join([signaltag, part])
                else:
                    signalmass = part[len(self.signalmass_identifier):]
                    break

            limits_and_quantiles_per_mass[int(signalmass)] = {}
            combinefilename = os.path.join(self.combineoutput_path, 'higgsCombine%s.AsymptoticLimits.mH%s.root' % ('%s' % (signaltag if selectiontag is None else signaltag+selectiontag), signalmass))
            rootfile = TFile(combinefilename, 'READ')
            tree = rootfile.Get('limit')
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                limits_and_quantiles_per_mass[int(signalmass)][tree.quantileExpected] = tree.limit
            rootfile.Close()
        numbers_per_selectiontag[selectiontag] = limits_and_quantiles_per_mass

        g_expected = TGraph()
        g_68 = TGraphAsymmErrors()
        g_95 = TGraphAsymmErrors()
        npoints_expected = npoints_68 = npoints_95 = 0
        for mass in sorted(limits_and_quantiles_per_mass):
            expected = low_68 = high_68 = low_95 = high_95 = obs = -1.
            for quant in limits_and_quantiles_per_mass[mass]:
                if quant < 0: obs = limits_and_quantiles_per_mass[mass][quant] * signal_scaled_by
                elif quant < 0.0251: low_95 = limits_and_quantiles_per_mass[mass][quant] * signal_scaled_by
                elif quant < 0.161: low_68 = limits_and_quantiles_per_mass[mass][quant] * signal_scaled_by
                elif quant < 0.51: expected = limits_and_quantiles_per_mass[mass][quant] * signal_scaled_by
                elif quant < 0.85: high_68 = limits_and_quantiles_per_mass[mass][quant] * signal_scaled_by
                elif quant < 0.98: high_95 = limits_and_quantiles_per_mass[mass][quant] * signal_scaled_by

            g_expected.SetPoint(npoints_expected, mass, expected)
            g_68.SetPoint(npoints_68, mass, expected)
            g_95.SetPoint(npoints_95, mass, expected)
            g_68.SetPointError(npoints_68, 0., 0., expected-low_68, high_68-expected)
            g_95.SetPointError(npoints_95, 0., 0., expected-low_95, high_95-expected)
            npoints_expected += 1
            npoints_68 += 1
            npoints_95 += 1
        graphs_per_selectiontag[selectiontag] = {
            'exp': g_expected,
            '68': g_68,
            '95': g_95,
        }

    # get theory prediction
    print signaltag
    if draw_theory:
        if signaltag.startswith('LQLQ'):
            xsecinfilename = os.path.join(self.crosssection_path, 'Crosssections_LQLQ.root')
            crosssectiongraphname = '_'.join(['LQLQ', self.signals[0].split('_')[3], self.signals[0].split('_')[4]])
        elif signaltag.startswith('LQTChannel'):
            xsecinfilename = os.path.join(self.crosssection_path, 'Crosssections_%s.root' % (signaltag))
            crosssectiongraphname = '_'.join([signaltag, 'MC1107', 'L1p0'])
        elif signaltag.startswith('PsiPsiToLQChi_MLQ1000'):
            print self.crosssection_path
            xsecinfilename = os.path.join(self.crosssection_path, 'Crosssections_PsiPsi_vs_MPS.root')
            print xsecinfilename
            crosssectiongraphname = '_'.join(['PsiPsi', 'L1', 'MLQ1000'])
            print crosssectiongraphname

        else:
            raise ValueError('PlotLimits() can only plot LQLQ and t-channel cross section. This seems to be a different process (maybe PsiPsi?) and needs implementation.')
        xsecinfile = TFile(xsecinfilename, 'READ')
        g_theory = xsecinfile.Get(crosssectiongraphname)
        g_theory_realistic = xsecinfile.Get(crosssectiongraphname)

    # hack some theory lines for illustration
    if signaltag.startswith('LQTChannel'):
        g_theory_equal = g_theory_realistic.Clone()
        g_theory_realistic_L2p5 = g_theory_realistic.Clone()
        for i in range(g_theory_equal.GetN()):
            g_theory_equal.GetY()[i] *= 17.7 # equal couplings (both = 0.7) instead of (1 and 0.12) --> 0.7^4 / (1^2 * 0.12^2) = 17.7
            g_theory_equal.GetEYhigh()[i] *= 17.7 # equal couplings (both = 0.7) instead of (1 and 0.12) --> 0.7^4 / (1^2 * 0.12^2) = 17.7
            g_theory_equal.GetEYlow()[i] *= 17.7 # equal couplings (both = 0.7) instead of (1 and 0.12) --> 0.7^4 / (1^2 * 0.12^2) = 17.7
        for i in range(g_theory_realistic_L2p5.GetN()):
            g_theory_realistic_L2p5.GetY()[i] *= 39 # lambda = 2.5 instead of 1.0 --> * 2.5^4 = 39
            g_theory_realistic_L2p5.GetEYhigh()[i] *= 39 # lambda = 2.5 instead of 1.0 --> * 2.5^4 = 39
            g_theory_realistic_L2p5.GetEYlow()[i] *= 39 # lambda = 2.5 instead of 1.0 --> * 2.5^4 = 39

    # yaxis_title = '#sigma (pp #rightarrow %s) [pb]' % (signaltag)
    # c = tdrCanvas(canvName='c', x_min=min(limits_and_quantiles_per_mass), x_max=max(limits_and_quantiles_per_mass), y_min=1E-5, y_max=1E4, nameXaxis='M_{LQ} [GeV]', nameYaxis=yaxis_title, square=True, iPeriod=0, iPos=11)
    c = tdrCanvas(canvName='c', x_min=min(numbers_per_selectiontag[selectiontags[0]]), x_max=max(numbers_per_selectiontag[selectiontags[0]]), y_min=1E-2, y_max=1E4, nameXaxis=xaxistitle, nameYaxis=yaxistitle, square=True, iPeriod=0, iPos=11)
    c.SetLogy()
    leg = tdrLeg(0.5,0.65,0.85,0.9, textSize=0.042)
    tdrHeader(leg, '95% CL upper limits')
    if draw_theory:
        leg_theory = tdrLeg(0.5,0.45,0.85,0.6, textSize=0.042)
        tdrHeader(leg_theory, 'Theory prediction')
    # tdrDraw(g_95, "3 SAME", mcolor=kOrange, lcolor=kOrange, fcolor=kOrange)
    # tdrDraw(g_68, "3 SAME", mcolor=kGreen+1, lcolor=kGreen+1, fcolor=kGreen+1)
    for i, selectiontag in enumerate(graphs_per_selectiontag.keys()):
        color = colors[i] if i < len(colors) else colors[-1]+ i-len(colors) + 1
        tdrDraw(graphs_per_selectiontag[selectiontag]['exp'], "L SAME", mcolor=color, lcolor=color, fcolor=color, lstyle=2)
        leg.AddEntry(graphs_per_selectiontag[selectiontag]['exp'], 'expected %s' % (selectiontag), 'L')
    if draw_theory:
        if signaltag.startswith('LQLQ'):
            tdrDraw(g_theory, "3L SAME", mcolor=kRed+2, lcolor=kRed+2, fcolor=kRed+2, lstyle=1, alpha=0.4)
            leg_theory.AddEntry(g_theory, 'LQ pair production', 'LF')
        elif signaltag.startswith('LQTChannel'):
            tdrDraw(g_theory_realistic, "3L SAME", mcolor=kRed+2, lcolor=kRed+2, fcolor=kRed+2, lstyle=1, alpha=0.4)
            leg_theory.AddEntry(g_theory_realistic, 'Realistic cpl., #lambda = 1.0', 'LF')
            tdrDraw(g_theory_equal, "3L SAME", mcolor=kAzure-2, lcolor=kAzure-2, fcolor=kAzure-2, lstyle=1, alpha=0.4)
            leg_theory.AddEntry(g_theory_equal, 'Equal cpl., #lambda = 1.0', 'LF')
            tdrDraw(g_theory_realistic_L2p5, "3L SAME", mcolor=kGreen-2, lcolor=kGreen-2, fcolor=kGreen-2, lstyle=1, alpha=0.4)
            leg_theory.AddEntry(g_theory_realistic_L2p5, 'Realistic cpl., #lambda = 2.5', 'LF')
        if signaltag.startswith('PsiPsiToLQChi_MLQ1000'):
            tdrDraw(g_theory, "3L SAME", mcolor=kRed+2, lcolor=kRed+2, fcolor=kRed+2, lstyle=1, alpha=0.4)
            leg_theory.AddEntry(g_theory, '#psi pair production', 'LF')
    # leg.AddEntry(g_68, '68% expected', 'F')
    # leg.AddEntry(g_95, '95% expected', 'F')
    leg.Draw('SAME')
    if draw_theory:
        leg_theory.Draw('SAME')
    c.RedrawAxis()
    outname = os.path.join(self.plotoutput_path, 'LimitComparison.pdf')
    c.SaveAs(outname)
    if draw_theory:
        xsecinfile.Close()
