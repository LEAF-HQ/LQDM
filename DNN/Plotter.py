from DNNTools.PlotterBase import *
from DNNTools.functions_dnn import list_to_str, classstring_from_label

class Plotter(PlotterBase):
    def __init__(self, inputdir, outdir, classes):
        PlotterBase.__init__(self, inputdir=inputdir, outdir=outdir)
        self.classes = classes

    def DefineStyle(self):
        style = {
            0: {
                'color': 'C0',
                'label': classstring_from_label(self.classes, 0)
            },
            1: {
                'color': 'C1',
                'label': classstring_from_label(self.classes, 1)
            }
        }
        return style

    def PlotInputVariables(self):
        self.DefineCommonStyle()
        self.DefineStylePerVariable()
        self.LoadInputs()
        self.PlotBase(self.DefineStyle())

class PlotterPredictions(PlotterBase):
    def __init__(self, labeldir, predictiondir, outdir, classes):
        PlotterBase.__init__(self, inputdir=labeldir, outdir=outdir)
        self.classes = classes
        self.predictiondir = predictiondir

    def DefineStyle(self):
        style = {
            0: {
                'color': 'C0',
                'label': classstring_from_label(self.classes, 0)
            },
            1: {
                'color': 'C1',
                'label': classstring_from_label(self.classes, 1)
            }
        }
        return style

    def LoadInputs(self):
        print(blue('--> Loading'))
        inputdir = os.path.join(self.inputdir, classes_to_str(self.classes))
        preds = []
        for label in ['train', 'val', 'test']:
            pred   = pd.read_pickle(os.path.join(os.path.join(self.predictiondir), 'prediction_%s_%s.pkl' %(label,self.frac) ))
            pred['label'] = np.load(os.path.join(inputdir, 'label_%s_%s.npy' %(label,self.frac) )).tolist()
            pred['label'] = np.argmax(np.array(pred['label'].to_list()), axis = 1)
            pred['weights'] = pd.read_pickle(os.path.join(inputdir, 'weights_%s_%s.pkl' %(label,self.frac) ))
            preds.append(pred)
        self.df = pd.concat(preds)

    def PlotPredictions(self):
        self.DefineCommonStyle()
        self.DefineStylePerVariable()
        self.LoadInputs()
        self.PlotBase(self.DefineStyle())
