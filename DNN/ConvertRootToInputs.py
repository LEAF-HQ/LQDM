from DNNTools.ConvertRootToInputsBase import *

class ConvertRootToInputs(ConvertRootToInputsBase):
    def __init__(self, inputdir, outdir, samples):
        chunksize = 200000
        treename = 'AnalysisTree'
        namebranch_weight = ('Events.GenEvent.Event.weight','event_weight')
        ConvertRootToInputsBase.__init__(self, inputdir, outdir, chunksize, treename, namebranch_weight)
        self.LoadDependancies('libLEAFLQDM.so')
        self.samples = samples

    def DefineSamples(self):
        return self.samples

    def DefineVariables(self):
        varnames_jets      = [('m_pt', 'jet_pt'), ('m_eta', 'jet_eta'), ('m_phi', 'jet_phi'), ('m_m', 'jet_m'), ('m_score_DeepFlavB', 'jet_score_DeepFlavB')]
        njets = 5
        varnames_muons     = [('m_pt', 'muon_pt'), ('m_eta', 'muon_eta'), ('m_phi', 'muon_phi'), ('m_m', 'muon_m'), ('m_dxy', 'muon_dxy'), ('m_dz', 'muon_dz'), ('m_dxy_err', 'muon_dxy_err'), ('m_dz_err', 'muon_dz_err'), ('m_iso_rel_04', 'muon_iso_rel_04')]
        nmuons = 2
        varnames_electrons = [('m_pt', 'electron_pt'), ('m_eta', 'electron_eta'), ('m_phi', 'electron_phi'), ('m_m', 'electron_m'), ('m_dxy', 'electron_dxy'), ('m_dz', 'electron_dz'), ('m_dxy_err', 'electron_dxy_err'), ('m_dz_err', 'electron_dz_err'), ('m_iso_rel_03', 'electron_iso_rel_03')]
        nelectrons = 2
        varnames_taus      = [('m_pt', 'tau_pt'), ('m_eta', 'tau_eta'), ('m_phi', 'tau_phi'), ('m_m', 'tau_m'), ('m_dxy', 'tau_dxy'), ('m_dz', 'tau_dz'), ('m_score_deeptau_vse', 'tau_score_deeptau_vse'), ('m_score_deeptau_vsmu', 'tau_score_deeptau_vsmu'), ('m_score_deeptau_vsjet', 'tau_score_deeptau_vsjet'), ('m_comb_iso_dr03', 'tau_comb_iso_dr03')]
        ntaus = 2
        varnames_general   = [('met.m_pt', 'met_pt'), ('met.m_phi', 'met_phi')]
        varnames = []
        colnames = []
        for v,n in varnames_jets:
            for i in range(njets):
                varnames.append(('Events.jets_ak4chs.%s[%i]' % (v, i), -99., 1))
                colnames.append('%s_%i' % (n, i))
        for v,n in varnames_muons:
            for i in range(nmuons):
                varnames.append(('Events.muons.%s[%i]' % (v, i), -99., 1))
                colnames.append('%s_%i' % (n, i))
        for v,n in varnames_electrons:
            for i in range(nelectrons):
                varnames.append(('Events.electrons.%s[%i]' % (v, i), -99., 1))
                colnames.append('%s_%i' % (n, i))
        for v,n in varnames_taus:
            for i in range(ntaus):
                varnames.append(('Events.taus.%s[%i]' % (v, i), -99., 1))
                colnames.append('%s_%i' % (n, i))
        for v,n in varnames_general:
            varnames.append('Events.%s' % (v))
            colnames.append('%s_%i' % (n, i))

        return varnames, colnames
