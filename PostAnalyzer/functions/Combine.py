import os
import sys
import subprocess
from utils import *

def CreateDatacards(self, selectiontag=None):

    for signal in self.signals:
        for cat in self.histnames_in_out_per_category:
            for chan in self.channels:
                create_datacard(self.year, signal=signal, variable=self.histnames_in_out_per_category[cat][1], category=cat, channel=chan, backgrounds=self.backgrounds, systematics=self.systematics, procs_per_syst=self.processes_per_systematic, pdf_per_syst=self.pdf_per_systematic, value_per_syst=self.value_per_systematic, combineinput_path=self.combineinput_path, rootfilename='combine_histograms%s.root'%('' if selectiontag is None else selectiontag), selectiontag=selectiontag)

def CombineChannels(self, selectiontag=None):
    combine_dir = os.getenv('COMBINEPATH')
    if not os.path.exists(combine_dir):
        raise RuntimeError('Combine not set-up where expected: %s.' % (combine_dir))
    processes = []
    for signal in self.signals:
        datacards = []
        finalname_base = os.path.join(self.combineinput_path, 'COMB')
        finalname = finalname_base
        for chan in self.channels:
            finalname = '_'.join([x for x in [finalname, chan] if x is not None])
            for cat in self.histnames_in_out_per_category.keys():
                indatacardname = '_'.join([x for x in [self.histnames_in_out_per_category[cat][1], chan, cat, signal] if x is not None]) + '%s.txt' % ('' if selectiontag is None else selectiontag)
                datacards.append(os.path.join(self.combineinput_path, indatacardname))
                # print finalname, cat
                finalname = '_'.join([x for x in [finalname, cat] if x is not None])
        finalname = '_'.join([finalname, signal]) + '%s.txt' % ('' if selectiontag is None else selectiontag)
        command = [combine_dir + '/scripts/combineCards.py']
        command += datacards
        print command
        f = open(finalname, 'w')
        processes.append(subprocess.Popen(command, stdout=f))

    for p in processes:
        p.wait()

def ExecuteCombineCombination(self, selectiontag=None):
    cwd = os.getcwd()
    if not os.path.exists(self.combineoutput_path):
        raise RuntimeError('Combine output directory not where expected: %s.' % (self.combineoutput_path + '/output'))
    os.chdir(self.combineoutput_path)
    combine_dir = os.getenv('COMBINEPATH')
    processes = []
    signalmasses = []
    commands = []
    for signal in self.signals:
        combcard_base = os.path.join(self.combineinput_path, 'COMB')
        combcard = combcard_base
        for chan in self.channels:
            combcard = '_'.join([x for x in [combcard, chan] if x is not None])
            for cat in self.histnames_in_out_per_category.keys():
                combcard = '_'.join([x for x in [combcard, cat] if x is not None])
        combcard = '_'.join([combcard, signal]) + '%s.txt' % ('' if selectiontag is None else selectiontag)
        
        parts = signal.split('_')
        signaltag = signal
        signalmass = -1
        for part in parts:
            if self.signalmass_identifier in part:
                signalmass = part[len(self.signalmass_identifier):]
                break
            # if not self.signalmass_identifier in part:
            #     if signaltag == '': signaltag = part
            #     else: signaltag = '_'.join([signaltag, part])
            # else:
            #     signalmass = part[len(self.signalmass_identifier):]
            #     break

        signalmasses.append(signalmass)
        sourceprecommands = '. /cvmfs/cms.cern.ch/${SCRAM_ARCH}/external/gcc/7.0.0-omkpbe2/etc/profile.d/init.sh; . /cvmfs/cms.cern.ch/${SCRAM_ARCH}/lcg/root/6.12.07-gnimlf5/bin/thisroot.sh;. /cvmfs/cms.cern.ch/${SCRAM_ARCH}/external/gsl/2.2.1-omkpbe2/etc/profile.d/init.sh;. /cvmfs/cms.cern.ch/${SCRAM_ARCH}/external/tbb/2018_U1-omkpbe2/etc/profile.d/init.sh;. /cvmfs/cms.cern.ch/${SCRAM_ARCH}/cms/vdt/0.4.0-gnimlf/etc/profile.d/init.sh;. /cvmfs/cms.cern.ch/${SCRAM_ARCH}/external/boost/1.63.0-gnimlf/etc/profile.d/init.sh;. /cvmfs/cms.cern.ch/${SCRAM_ARCH}/external/pcre/8.37-omkpbe2/etc/profile.d/init.sh;. /cvmfs/cms.cern.ch/${SCRAM_ARCH}/external/eigen/64060da8461a627eb25b5a7bc0616776068db58b/etc/profile.d/init.sh; cd $CMSSW_COMBINE_BASE/src; eval `scramv1 runtime -sh`; cd -;'
        command = '%s combine -M AsymptoticLimits --run blind -n %s -m %s %s' % (sourceprecommands, '%s' % (signaltag if selectiontag is None else signaltag+selectiontag), signalmass, combcard)
        print command
        commands.append(command)
    execute_commands_parallel(commands=commands)

    for p in processes:
        p.wait()

    os.chdir(cwd)








def write_lines(filename, lines):
    outfile = open(filename, 'w')
    for line in lines:
        outfile.write(line)
        outfile.write('\n')
    outfile.close()

def get_lines_datacard_header(variable,category, channel, signal, backgrounds):
    lines = []
    lines.append('# Datacard for variable %s in channel %s, category %s, for signal %s \n' % (variable, channel, category, signal))
    lines.append('# HEADER')
    lines.append('imax 1')
    lines.append('jmax %i' % (len(backgrounds)))
    lines.append('kmax *')
    return lines

def get_lines_datacard_channels(varcat):
    lines = []
    lines.append('# CHANNELS')
    lines.append('bin %s' % (varcat))
    lines.append('observation -1')
    return lines

def get_lines_datacard_input(rootfilename, year):
    lines = []
    #     shapes * * input/combine_histograms_mc_fullsyst_much.root $CHANNEL__$PROCESS_2016v3 $CHANNEL__$PROCESS_2016v3__$SYSTEMATIC
    lines.append('# INPUT')
    lines.append('shapes * * %s $CHANNEL__$PROCESS_%s $CHANNEL__$PROCESS_%s__$SYSTEMATIC' % (rootfilename, year, year))
    return lines

def get_lines_datacard_processes(category, varcat, signal, backgrounds, hists_empty):
    lines = []
    lines.append('# PROCESSES')

    line = 'bin        '
    for i in range(len(backgrounds) + 1):
        line += varcat + '  '
    lines.append(line)

    line = 'process    ' + '%s' % (signal) + '  '
    for bkg in backgrounds:
        line += bkg + '  '
    lines.append(line)

    line = 'process    0  '
    idx = 1
    for bkg in backgrounds:
        line += str(idx) + '  '
        idx += 1
    lines.append(line)

    line = 'rate       ' + '%s' % ('-1' if not hists_empty[signal] else '0') + '  '
    for bkg in backgrounds:
        line += '%s  ' % ('-1' if not hists_empty[bkg] else '0')
    lines.append(line)

    return lines




def get_lines_datacard_systematics(category, systematics, procs_per_syst, pdf_per_syst, value_per_syst, backgrounds, signal):
    lines = []
    lines.append('# SYSTEMATICS')
    for syst in systematics:
        if syst == 'nominal': continue
        if not syst in procs_per_syst.keys():
            raise RuntimeError('Systematic %s not in constant dictionaries.' % (syst))
        print 'syst: %s' % (syst)
        line = syst + '  ' + pdf_per_syst[syst] + '  '
        # first for signal:
        if procs_per_syst[syst] == 'all' or procs_per_syst[syst] == signal:
            line += str(value_per_syst[syst]) + '  '
        else:
            line += '-  '

        for bkg in backgrounds:
            if procs_per_syst[syst] == 'all' or procs_per_syst[syst] == bkg:
                line += str(value_per_syst[syst]) + '  '
            else:
                line += '-  '
        lines.append(line)

    return lines

def get_lines_datacard_statistics():
    lines = []
    lines.append('# STATISTICS')
    lines.append('* autoMCStats 1 0 1')
    return lines


def is_hist_empty(process, rootfilename, histname):

    histogramfile = ROOT.TFile(rootfilename, 'READ')
    h = histogramfile.Get(histname)
    is_empty = False
    if int(h.GetEntries()) == 0:
        # print histname, h.GetEntries()
        is_empty = True
    histogramfile.Close()
    return is_empty



def create_datacard(year, signal, variable, category, channel, backgrounds, systematics, procs_per_syst, pdf_per_syst, value_per_syst, combineinput_path, rootfilename, selectiontag):

    if not os.path.exists(combineinput_path):
        raise RuntimeError('Path %s does not exist.' % (combineinput_path))
    if not os.path.exists(os.path.join(combineinput_path, rootfilename)):
        raise RuntimeError('Rootfile %s does not exist.' % (os.path.join(combineinput_path, rootfilename)))

    filename_datacard = '_'.join([x for x in [variable, channel, category, signal] if x is not None]) + '%s.txt' % ('' if selectiontag is None else selectiontag)
    # print filename_datacard
    varcat = '_'.join([x for x in [variable, channel, category] if x is not None])
    separator = ['-----------------------------\n']

    ### Check for empty histograms in signal and backgrounds.
    # signal_hist_empty = is_hist_empty(process=signal, rootfilename=os.path.join(combineinput_path, rootfilename), histname='_'.join([x for x in [variable, channel, category] if x is not None]) + '__' + signal + '_%s' % (year))
    hists_empty = {bkg: is_hist_empty(process=signal, rootfilename=os.path.join(combineinput_path, rootfilename), histname='_'.join([x for x in [variable, channel, category] if x is not None]) + '__' + bkg + '_%s' % (year)) for bkg in backgrounds}
    hists_empty[signal] = is_hist_empty(process=signal, rootfilename=os.path.join(combineinput_path, rootfilename), histname='_'.join([x for x in [variable, channel, category] if x is not None]) + '__' + signal + '_%s' % (year))
    # print signal_hist_empty
    # print background_hists_empty
    # if signal_hist_empty:
        # signal = None
    # backgrounds = [b for b in backgrounds if not hists_empty[b]]

    lines_header = get_lines_datacard_header(variable, category, channel, signal, backgrounds) + separator
    lines_channels = get_lines_datacard_channels(varcat) + separator
    lines_input = get_lines_datacard_input(rootfilename, year)
    lines_processes = get_lines_datacard_processes(category, varcat, signal, backgrounds, hists_empty)
    lines_systematics = get_lines_datacard_systematics(category, systematics, procs_per_syst, pdf_per_syst, value_per_syst, backgrounds, signal)
    lines_statistics = get_lines_datacard_statistics()

    lines = lines_header + lines_channels + lines_input + lines_processes + lines_systematics + lines_statistics
    write_lines(os.path.join(combineinput_path, filename_datacard), lines)
    print green('  --> Created datacard for signal %s in channel %s and category %s. ' % (signal, channel, category))
