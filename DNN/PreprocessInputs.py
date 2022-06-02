from DNNTools.PreprocessInputsBase import *
from sklearn.compose import ColumnTransformer

class PreprocessInputs(PreprocessInputsBase):
    def __init__(self, inputdir, outdir, samples, classes, runonfraction):
        PreprocessInputsBase.__init__(self, inputdir=inputdir, outdir=outdir, runonfraction=runonfraction)
        self.samples = samples
        self.classes = classes

    def DefineSamples(self):
        samplenames = list(dict.fromkeys([self.samples[key].samplename for key in self.samples]))
        return samplenames

    def DefineClasses(self):
        return self.classes

    def RemoveNanInf(self):
        self.df.replace([np.inf, -np.inf], np.nan, inplace=True)
        self.df.dropna()

    def FitScalers(self):
        self.scalers = {}
        self.scalers['standard'] = ColumnTransformer(list((col+'st',preprocessing.StandardScaler(), [col]) for col in  self.inputs['train'].columns.tolist()))
        self.scalers['standard'].fit(self.inputs['train'])
