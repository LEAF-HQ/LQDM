#include <TString.h>
#include <iostream>
#include <TStyle.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TText.h>
#include <TPaveText.h>
#include <TGaxis.h>
#include <TGraph.h>
#include <TStyle.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TROOT.h>
#include <TKey.h>
#include <TLatex.h>
#include <TClass.h>
#include <fstream>

#include "Analyzer/LQDM/PostAnalyzer/include/PostAnalysisTool.h"

using namespace std;

void PostAnalysisTool::ProduceCombineHistograms(){


  vector<TString> samples = {"LQLQToBTau_MLQ1000_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ1360_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ1810_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ2170_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ2620_MPS117_MC1100_L1p0", "ST", "TT", "DYJets", "QCD_Had", "WJets", "DATA"};

  TString histfolder_base = "Limits";

  vector<TString> systematics = {"nominal"};

  TString dir_base = PostAnalysisTool::m_finalselection_path;
  vector<TString> category_tags = {"cat1", "cat2"};
  vector<TString> histinname_base = {"stmet2", "stmet2"};
  vector<TString> histoutname_base = {"STMET", "STMET"};
  vector<TString> channel_tags = {"elch", "much", "elmuch", "tach"};


  vector<TString> syst_shift = {"up", "down"};
  vector<TString> syst_shift_combine = {"Up", "Down"};

  string command = "mkdir -p " + (string)m_datacard_path;
  system(command.c_str());

  TString outfilename = PostAnalysisTool::m_datacard_path + "combine_histograms.root";

  unique_ptr<TFile> outfile;
  outfile.reset(new TFile(outfilename, "RECREATE"));


  for(unsigned int category=0; category<category_tags.size(); category++){
    // cout << "category: " << category_tags[category] << endl;
    for(unsigned int channel=0; channel<channel_tags.size(); channel++){
      // cout << "channel: " << channel_tags[channel] << endl;

      for(unsigned int k=0; k<systematics.size(); k++){
        TString syst = systematics[k];
        cout << green << "  --> Syst: " << syst << reset << endl;

        for(unsigned int m=0; m<syst_shift.size(); m++){
          // cout << "syst_shift: " << syst_shift[m] << endl;
          TString dir_nom = dir_base;
          TString dir_scale = dir_base;
          TString dir_pdf = dir_base;

          if(syst == "nominal" && m > 0) break;
          dir_pdf += "pdf/";
          dir_scale += "scale/";

          for(unsigned int i=0; i<samples.size(); i++){
            // cout << "sample: " << samples[i] << endl;
            bool force_nominal = false;
            TString sample_in = samples[i];
            TString sample_out = samples[i];
            if(sample_out == "DATA") sample_out = "data_obs_" + PostAnalysisTool::m_year;
            cout << green << "    --> Sample: " << sample_in << reset << endl;

            if(sample_in == "DATA" && syst != "nominal") force_nominal = true;
            if(!(sample_in == "TT") && syst == "scale_TT") force_nominal = true;
            if(!(sample_in == "SingleTop") && syst == "scale_ST") force_nominal = true;
            if(!(sample_in == "DYJets") && syst == "scale_DYJets") force_nominal = true;
            if(!(sample_in == "WJets") && syst == "scale_WJets") force_nominal = true;

            TString type = "MC__";
            if(sample_in == "DATA") type = "DATA__";

            if(sample_in == "DATA"){
              if(channel_tags[channel] == "much" || channel_tags[channel] == "elmuch") sample_in += "_SingleMuon";
              else if(channel_tags[channel] == "elch") sample_in += "_SingleElectron";
              else sample_in += "_Tau";
            }

            TString filename = dir_nom + type + sample_in + ".root";
            if(!force_nominal){
              if(syst.Contains("scale_")) filename = dir_scale + sample_in + ".root";
              if(syst == "pdf") filename = dir_pdf + sample_in + ".root";
            }
            TFile* infile = new TFile(filename, "READ");

            TString histname_in = "";
            histname_in = channel_tags[channel] + "_" + category_tags[category] + "_" + syst + "_" + histfolder_base;
            if(force_nominal) histname_in = channel_tags[channel] + "_" + channel_tags[channel] + "_" + category_tags[category] + "_nominal" + "_" + histfolder_base;
            if(syst != "nominal" && !force_nominal) histname_in += "_" + syst_shift[m];
            histname_in += "/" + histinname_base[category];
            // think this is not needed, but not 100% sure
            // if(syst.Contains("scale") && !force_nominal) histname_in = channel_tags[channel] + "_" + histfolder_base + "_" + category_tags[category] + "_scale_" + syst_shift[m] +  "/" + histinname_base[category];

            TString histname_out =  histoutname_base[category] + "_" + channel_tags[channel] + "_" + category_tags[category] + "__" + sample_out;
            if(syst != "nominal") histname_out += "__" + syst + syst_shift_combine[m];

            // cout << "histname_in: " << histname_in << endl;
            TH1F* h_in = (TH1F*)infile->Get(histname_in);
            // if(sample_in.Contains("LQtoTMuM300") || sample_in.Contains("LQtoTMuM400")){
            //   h_in->Scale(1./10.);
            // }
            h_in->SetName(histname_out);
            outfile->cd();
            h_in->Write();
            infile->Close();
          }
        }
      }
    }
  }

  cout << green << "--> Finished producing the combine histograms in '" << outfilename << "'."  << reset << endl;



}
