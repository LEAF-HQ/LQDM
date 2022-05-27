from DNNTools.ConvertRootToInputsBase import *

class ConvertRootToInputs(ConvertRootToInputsBase):
    def __init__(self, inputdir, outdir, samples):
        chunksize = 200000
        treename = 'AnalysisTree'
        namebranch_weight = 'Events.GenEvent.Event.weight'
        ConvertRootToInputsBase.__init__(self, inputdir, outdir, chunksize, treename, namebranch_weight)
        self.LoadDependancies('libLEAFLQDM.so')
        self.samples = samples

    def DefineSamples(self):
        return self.samples

    def DefineVariables(self):
        varnames_jets      = ['m_pt', 'm_eta', 'm_phi', 'm_m', 'm_score_DeepFlavB']
        njets = 5
        varnames_muons     = ['m_pt', 'm_eta', 'm_phi', 'm_m', 'm_dxy', 'm_dz', 'm_dxy_err', 'm_dz_err', 'm_iso_rel_04']
        nmuons = 2
        varnames_electrons = ['m_pt', 'm_eta', 'm_phi', 'm_m', 'm_dxy', 'm_dz', 'm_dxy_err', 'm_dz_err', 'm_iso_rel_03']
        nelectrons = 2
        varnames_taus      = ['m_pt', 'm_eta', 'm_phi', 'm_m', 'm_dxy', 'm_dz', 'm_score_deeptau_vse', 'm_score_deeptau_vsmu', 'm_score_deeptau_vsjet', 'm_comb_iso_dr03']
        ntaus = 2
        varnames_general   = ['met.m_pt', 'met.m_phi']
        varnames = []
        for v in varnames_jets:
            for i in range(njets):
                varnames.append(('Events.jets_ak4chs.%s[%i]' % (v, i), -99., 1))
        for v in varnames_muons:
            for i in range(nmuons):
                varnames.append(('Events.muons.%s[%i]' % (v, i), -99., 1))
        for v in varnames_electrons:
            for i in range(nelectrons):
                varnames.append(('Events.electrons.%s[%i]' % (v, i), -99., 1))
        for v in varnames_taus:
            for i in range(ntaus):
                varnames.append(('Events.taus.%s[%i]' % (v, i), -99., 1))
        for v in varnames_general:
            varnames.append('Events.%s' % (v))

        return varnames
