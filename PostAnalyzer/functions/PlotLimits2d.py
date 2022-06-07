
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from ROOT import gROOT, TFile, TCanvas, TH1D, TTree, TGraph, TGraph2D, TGraphAsymmErrors, kBlack, kOrange, kGreen, kRed
from array import array
from tdrstyle_all import *
import constants

def PlotLimits2d(self, signalgroups, xsecvar, xsecconst, xaxisdef, yaxisdef, zaxistitle, selectiontag=None, signal_scaled_by=1., draw_exclusion=False):
    print green('  --> Plotting 2d-limits.')
    # Expects all signals to belong to the same process!

    gROOT.SetBatch(1)

    limits_and_quantiles_per_xmass_ymass = {}
    xsecconstvalues = []
    g_expected_1ds = {}
    g_1slo_1ds = {}
    g_1shi_1ds = {}
    for signals in signalgroups:
        for signal in signals:
            parts = signal.split('_')
            signaltag = signal
            massx = -1
            massy = -1
            signalmass = -1
            for part in parts:
                if xaxisdef[0] in part:
                    massx = part[len(xaxisdef[0]):]
                if yaxisdef[0] in part:
                    massy = part[len(yaxisdef[0]):]
                if self.signalmass_identifier in part:
                    signalmass = part[len(self.signalmass_identifier):]
            constval = get_value_by_tag_from_string(string=signal, tag=xsecconst)
            if not constval in g_expected_1ds.keys():
                g_expected_1ds[constval] = TGraph()
            if not constval in g_1slo_1ds.keys():
                g_1slo_1ds[constval] = TGraph()
            if not constval in g_1shi_1ds.keys():
                g_1shi_1ds[constval] = TGraph()
            if not constval in xsecconstvalues:
                xsecconstvalues.append(constval)

            print signaltag, massx, massy
            limits_and_quantiles_per_xmass_ymass[(massx, massy)] = {}
            combinefilename = os.path.join(self.combineoutput_path, 'higgsCombine%s.AsymptoticLimits.mH%s.root' % ('%s' % (signaltag if selectiontag is None else signaltag+selectiontag), signalmass))
            rootfile = TFile(combinefilename, 'READ')

            tree = rootfile.Get('limit')
            for i in range(tree.GetEntries()):
                tree.GetEntry(i)
                limits_and_quantiles_per_xmass_ymass[(massx, massy)][tree.quantileExpected] = tree.limit
            rootfile.Close()

    print xsecconstvalues

    g_expected = TGraph2D()
    # g_68 = TGraphAsymmErrors()
    # g_95 = TGraphAsymmErrors()
    npoints_expected = npoints_68 = npoints_95 = 0
    for (massx, massy) in limits_and_quantiles_per_xmass_ymass:
        expected = low_68 = high_68 = low_95 = high_95 = obs = -1.
        for quant in limits_and_quantiles_per_xmass_ymass[(massx, massy)]:
            if quant < 0: obs = limits_and_quantiles_per_xmass_ymass[(massx, massy)][quant] * signal_scaled_by
            elif quant < 0.0251: low_95 = limits_and_quantiles_per_xmass_ymass[(massx, massy)][quant] * signal_scaled_by
            elif quant < 0.161: low_68 = limits_and_quantiles_per_xmass_ymass[(massx, massy)][quant] * signal_scaled_by
            elif quant < 0.51: expected = limits_and_quantiles_per_xmass_ymass[(massx, massy)][quant] * signal_scaled_by
            elif quant < 0.85: high_68 = limits_and_quantiles_per_xmass_ymass[(massx, massy)][quant] * signal_scaled_by
            elif quant < 0.98: high_95 = limits_and_quantiles_per_xmass_ymass[(massx, massy)][quant] * signal_scaled_by

        if  xaxisdef[0] == xsecconst:
            g_expected_1ds[massx].SetPoint(g_expected_1ds[massx].GetN(), float(massy), float(expected))
            g_1slo_1ds[massx].SetPoint(g_1slo_1ds[massx].GetN(), float(massy), float(low_68))
            g_1shi_1ds[massx].SetPoint(g_1shi_1ds[massx].GetN(), float(massy), float(high_68))
            print float(massy), float(expected)
        elif yaxisdef[0] == xsecconst:
            g_expected_1ds[massy].SetPoint(g_expected_1ds[massy].GetN(), float(massx), float(expected))
            g_1slo_1ds[massy].SetPoint(g_1slo_1ds[massy].GetN(), float(massx), float(low_68))
            g_1shi_1ds[massy].SetPoint(g_1shi_1ds[massy].GetN(), float(massx), float(high_68))
            print float(massx), float(expected)
        else:
            raise ValueError('In PlotLimits2d(): The constant variable %s for the cross section is not one of the axes of the 2d plot.' % (xsecconst))

        g_expected.SetPoint(npoints_expected, float(massx), float(massy), float(expected))

        # g_68.SetPoint(npoints_68, mass, expected)
        # g_95.SetPoint(npoints_95, mass, expected)
        # g_68.SetPointError(npoints_68, 0., 0., expected-low_68, high_68-expected)
        # g_95.SetPointError(npoints_95, 0., 0., expected-low_95, high_95-expected)
        npoints_expected += 1
        # npoints_68 += 1
        # npoints_95 += 1

    # get theory prediction
    if draw_exclusion:
        if signaltag.startswith('PsiPsiToLQChi'):
            xsecinfilename = os.path.join(self.crosssection_path, 'Crosssections_PsiPsi_vs_MPS.root')



        else:
            raise ValueError('PlotLimits2d() can only plot PsiPsi exclusion line. This seems to be a different process and needs implementation.')
        xsecinfile = TFile(xsecinfilename, 'READ')
        crosssectiongraphname = '_'.join(['PsiPsi', 'L1', '%s'%(xsecconst)])
        exclusions_per_xsecconstvalue = {}
        g_exclusion_1d = TGraph()
        g_exclusion1slo_1d = TGraph()
        g_exclusion1shi_1d = TGraph()
        n_exclusionpoints = n_exclusionpoints_1slo = n_exclusionpoints_1shi = 0
        for val in xsecconstvalues:
            g_theory = xsecinfile.Get(crosssectiongraphname + '%s' % val)
            print get_intersection(g_theory, g_expected_1ds[val])
            if get_intersection(g_theory, g_expected_1ds[val]) is not None:
                g_exclusion_1d.SetPoint(n_exclusionpoints, float(val), float(get_intersection(g_theory, g_expected_1ds[val])))
                n_exclusionpoints += 1
                if get_intersection(g_theory, g_1slo_1ds[val]) is not None:
                    g_exclusion1slo_1d.SetPoint(n_exclusionpoints_1slo, float(val), float(get_intersection(g_theory, g_1slo_1ds[val])))
                    n_exclusionpoints_1slo += 1
                if get_intersection(g_theory, g_1shi_1ds[val]) is not None:
                    g_exclusion1shi_1d.SetPoint(n_exclusionpoints_1shi, float(val), float(get_intersection(g_theory, g_1shi_1ds[val])))
                    n_exclusionpoints_1shi += 1

    c = tdrCanvas2d(canvName='c', square=True)
    c.SetLogy()
    c.SetLogz()
    xmin = ymin = zmin = 1E-19
    xmax = g_expected.GetXaxis().GetXmax()
    ymax = g_expected.GetYaxis().GetXmax()*1.5
    xmin = g_expected.GetXaxis().GetXmin()
    ymin = max(g_expected.GetYaxis().GetXmin(), ymin)
    tdrDraw2d(g_expected, 'COLZ', 500, xmin, xmax, 500, ymin, ymax, 150, 1E-2, 1E3, [xaxisdef[1], yaxisdef[1], 'Expected cross section upper limit [pb]'])

    if draw_exclusion:
        leg = tdrLeg(0.25,0.75,0.8,0.95, textSize=0.042)
        tdrHeader(leg, '95% CL exclusion')
        tdrDraw(g_exclusion_1d, "L SAME", mcolor=kBlack, lcolor=kBlack, fcolor=kBlack, lstyle=2, alpha=0.0)
        tdrDraw(g_exclusion1slo_1d, "L SAME", mcolor=kBlack, lcolor=kBlack, fcolor=kBlack, lstyle=3, alpha=0.0)
        tdrDraw(g_exclusion1shi_1d, "L SAME", mcolor=kBlack, lcolor=kBlack, fcolor=kBlack, lstyle=3, alpha=0.0)
        leg.AddEntry(g_exclusion_1d, 'Median expected', 'L')
        leg.AddEntry(g_exclusion1slo_1d, '68% expected', 'L')
    c.RedrawAxis()
    outname = os.path.join(self.plotoutput_path, 'Limits2d%s.pdf' % ('' if selectiontag is None else selectiontag))
    c.SaveAs(outname)
    if draw_exclusion:
        xsecinfile.Close()

def get_value_by_tag_from_string(string, tag, separator='_'):
    parts = string.split(separator)
    for p in parts:
        if tag in p:
            return p[len(tag):]
    raise ValueError('tag %s not found in string %s' % (tag, string))


def get_intersection(g1, g2):

    result = None

    #find highest and lowest x:
    xmin = max(g1.GetX()[0], g2.GetX()[0] )
    xmax = min(g1.GetX()[g1.GetN()-1], g2.GetX()[g2.GetN()-1])

    # iteratively find intersection, by evaluating graphs at xmin, xmax, moving one of them, and repeating
    # ====================================================================================================
    y1lo = g1.Eval(xmin)
    y2lo = g2.Eval(xmin)
    y1hi = g1.Eval(xmax)
    y2hi = g2.Eval(xmax)
    # print y1lo, y2lo, y1hi, y2hi
    if (y1lo < y2lo and y1hi < y2hi) or (y1lo > y2lo and y1hi > y2hi):
        # raise ValueError('graphs 1 and 2 either have no or multiple intersections')
        return None

    # find higher graph at xmin
    g1_from_top = True if y1lo > y2lo else False

    xmid = xmin + (xmax-xmin)/2.
    while math.fabs(xmin-xmax) > 1:
        y1mid = g1.Eval(xmid)
        y2mid = g2.Eval(xmid)
        if g1_from_top:
            if y1mid > y2mid:
                xmin = xmid
            else:
                xmax = xmid
        elif not g1_from_top:
            if y1mid > y2mid:
                xmax = xmid
            else:
                xmin = xmid
        xmid = xmin + (xmax-xmin)/2.

    return xmid
