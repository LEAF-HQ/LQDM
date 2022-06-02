from DNNTools.PlotterBase import *
from DNNTools.functions_dnn import list_to_str, classstring_from_label

class Plotter(PlotterBase):
    def __init__(self, inputdir, outdir, classes):
        PlotterBase.__init__(self, inputdir=inputdir, outdir=outdir)
        self.classes = classes

    # def DefineClasses(self):
    #     return self.classes

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
        self.LoadInputs()
        self.PlotBase(self.DefineStyle())
