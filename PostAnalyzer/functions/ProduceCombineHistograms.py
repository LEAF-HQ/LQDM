
import os, sys, math, time
import subprocess
from utils import *
from collections import OrderedDict
from ROOT import TFile, TH1D

def ProduceCombineHistograms(self, signal_scaled_by=1.):
    print green('  --> Producing histograms for combine.')

    samples = self.signals + self.backgrounds + ['DATA']
    syst_shifts = [('up', 'Up'), ('down', 'Down')]

    ensureDirectory(self.combineinput_path)
    outfilename = os.path.join(self.combineinput_path, 'combine_histograms.root')
    outfile = TFile(outfilename, 'RECREATE')

    # loop
    for category in self.histnames_in_out_per_category:
        for channel in self.channels:
            for syst in self.systematics:
                print green('  --> Systematic: %s' % (syst))
                for shiftidx, syst_shift in enumerate(syst_shifts):
                    dir_nom = self.finalselection_path
                    dir_scale = os.path.join(self.finalselection_path, 'scale')
                    dir_pdf = os.path.join(self.finalselection_path, 'pdf')

                    if syst == 'nominal' and shiftidx > 0: continue
                    for sample in samples:
                        type = 'DATA__' if sample == 'DATA' else 'MC__'

                        force_nominal = False
                        sample_out = sample
                        if sample_out == 'DATA': sample_out = 'data_obs_' + self.year
                        else:                    sample_out += '_' + self.year
                        print green('    --> Sample: %s' % (sample))

                        if sample == 'DATA' and not syst == 'nominal': force_nominal = True
                        if not sample in self.processes_per_systematic.keys() and not syst == 'nominal': raise ValueError('When producing combine histograms: found systematic in systematic list that is not covered by \'processes_per_systematic\' dictionary. Please add it there and in the other dictionaries.')
                        if not syst == 'nominal' and not sample == self.processes_per_systematic[syst]: force_nominal = True
                        # if not sample == 'TT' and syst == 'scale_TT':  force_nominal = True
                        # if not sample == 'ST' and syst == 'scale_ST':  force_nominal = True
                        # if not sample == 'DYJets' and syst == 'scale_DYJets':  force_nominal = True
                        # if not sample == 'WJets' and syst == 'scale_WJets':    force_nominal = True

                        if sample == 'DATA': # choose correct PD
                            if channel == 'much' or channel == 'elmuch': sample += '_SingleMuon'
                            elif channel == 'elch': sample += '_SingleElectron'
                            else: sample += '_Tau'

                        infilename = os.path.join(dir_nom, type+sample+'.root')
                        if not force_nominal:
                            if 'scale_' in syst: infilename = os.path.join(dir_scale, sample_in+'.root')
                            if syst == 'pdf':    infilename = os.path.join(dir_pdf, sample_in+'.root')
                        infile = TFile(infilename, 'READ')

                        histname_in = '_'.join([channel, category, syst, self.limithisttag])
                        if force_nominal: histname_in = '_'.join([channel, category, 'nominal', self.limithisttag])
                        if not syst == 'nominal' and not force_nominal: histname_in = '_'.join([histname_in, syst_shift[0]])
                        histname_in += '/' + self.histnames_in_out_per_category[category][0]
                        # think this is not needed, but not 100% sure
                        # if(syst.Contains("scale") && !force_nominal) histname_in = self.channels[channel] + "_" + histfolder_base + "_" + category_tags[category] + "_scale_" + syst_shift[m] +  "/" + histinname_base[category];

                        histname_out = '_'.join([self.histnames_in_out_per_category[category][1], channel, category]) + '__' + sample_out
                        if not syst == 'nominal': histname_out += '__' + syst + syst_shift[1]
                        h_in = infile.Get(histname_in)
                        h_in.SetName(histname_out)
                        h_in.Scale(signal_scaled_by)
                        outfile.cd()
                        h_in.Write()
                        infile.Close()

    print green('  --> Finished producing the combine histograms in \'%s\'.' % (outfilename))
