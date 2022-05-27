from DNNTools.PreprocessInputsBase import *
from sklearn.compose import ColumnTransformer

class PreprocessInputs(PreprocessInputsBase):
    def __init__(self, inputdir, outdir, samples, classes):
        PreprocessInputsBase.__init__(self, None, inputdir=inputdir, outdir=outdir)
        self.samples = samples
        self.classes = classes

    def DefineSamples(self):
        return self.samples

    def DefineClasses(self):
        return self.classes

    def RemoveNanInf(self):
        self.df.replace([np.inf, -np.inf], np.nan, inplace=True)
        self.df.dropna()

    def FitScalers(self):
        self.scalers = {}
        self.scalers['standard'] = ColumnTransformer(list((col+'st',preprocessing.StandardScaler(), [col]) for col in  self.input_train.columns.tolist()))
        self.scalers['standard'].fit(self.input_train)
