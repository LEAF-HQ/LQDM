from DNNTools.PlotterBase import *
from DNNTools.functions_dnn import list_to_str, classstring_from_label
from DNNTools.DNNutils import LoadPandas
from DNNTools.PlottingFunctions import plot_losses, plot_rocs
import pickle
from utils import ensureDirectory
import ROOT as rt

def DefineUniformStyle(classes):
    style = {
        0: {
            'color': 'C0',
            'rootcolor': rt.TColor.GetColor('#95BBD9'),
            'label': classstring_from_label(classes, 0)
        },
        1: {
            'color': 'C1',
            'rootcolor': rt.TColor.GetColor('#FFC08C'),
            'label': classstring_from_label(classes, 1)
        },
        2: {
            'color': 'C2',
            'rootcolor': rt.TColor.GetColor('#9CCF94'),
            'label': classstring_from_label(classes, 2)
        },
        3: {
            'color': 'C3',
            'rootcolor': rt.TColor.GetColor('#ED9895'),
            'label': classstring_from_label(classes, 3)
        },
        4: {
            'color': 'C4',
            'rootcolor': rt.TColor.GetColor('#CAB2DE'),
            'label': classstring_from_label(classes, 4)
        },
        5: {
            'color': 'C5',
            'rootcolor': rt.TColor.GetColor('#C6ABA5'),
            'label': classstring_from_label(classes, 5)
        },
        6: {
            'color': 'C6',
            'rootcolor': rt.TColor.GetColor('#F2BCDF'),
            'label': classstring_from_label(classes, 6)
        },
        7: {
            'color': 'C7',
            'rootcolor': rt.TColor.GetColor('#000000'),
            'label': classstring_from_label(classes, 7)
        },
        8: {
            'color': 'C8',
            'rootcolor': rt.TColor.GetColor('#000000'),
            'label': classstring_from_label(classes, 8)
        },
        9: {
            'color': 'C9',
            'rootcolor': rt.TColor.GetColor('#000000'),
            'label': classstring_from_label(classes, 9)
        }
    }
    return style

class Plotter(PlotterBase):
    def __init__(self, modelpath, classes):
        PlotterBase.__init__(self)
        self.classes = classes
        self.modelpath = modelpath

    def DefineStyle(self):
        result = DefineUniformStyle(self.classes)
        return result

    def PlotPerformance(self, inputs, predictions, outdir, modelname='finalmodel'):
        print(blue('--> Plotting performance'))
        # load history
        with open(os.path.join(self.modelpath, '%s_history.pkl' % (modelname)), 'r') as f:
            history = pickle.load(f)

        # loss
        plot_losses(history, mode='loss', name=os.path.join(outdir, 'training_loss'))
        plot_losses(history, mode='categorical_accuracy',  name=os.path.join(outdir, 'training_acc'))

        # for class i, the score of the i'th node should be used: 'score_%i' % (i) for the "summary ROC curve".
        self.PlotROCSummary(df=predictions, outdir=outdir)

        for colname in predictions.columns:
            if not 'score' in colname: continue
            self.PlotROCSingleVariable(df=predictions, variable_name=colname, outdir=outdir)

        self.PlotROCSingleVariable(df=inputs, variable_name='met_pt_1', outdir=outdir)


        print(green('--> Plotted performance'))
