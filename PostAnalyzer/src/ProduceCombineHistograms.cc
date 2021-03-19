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


  vector<TString> samples_base = {"LQLQToBTau_MLQ1000_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ1360_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ1810_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ2170_MPS117_MC1100_L1p0", "LQLQToBTau_MLQ2620_MPS117_MC1100_L1p0", "ST", "TT", "DYJets", "QCD", "WJets", "DATA"};

  TString histfolder_base = "FinalSelection";

  vector<TString> systematics = {"nominal"};

  TString dir_base = PostAnalysisTool::base_path + PostAnalysisTool::year + "/";
  vector<TString> region_tags = {"catA", "catB"};
  vector<TString> histinname_base = {"MLQ_rebinlimit", "ST_rebinlimit"};
  vector<TString> histoutname_base = {"MLQ", "ST"};
  // vector<TString> channel_tags = {"ech", "much"};
  vector<TString> channel_tags = {"srmu", "ttbar"};


  vector<TString> syst_shift = {"up", "down"};
  vector<TString> syst_shift_combine = {"Up", "Down"};


  TString filename_base = "uhh2.AnalysisModuleRunner.";

  TString outfilename = PostAnalysisTool::combine_path + "input/combine_histograms_mc_fullsyst_incl.root";
  TFile* f_out = new TFile(outfilename, "RECREATE");


  for(unsigned int region=0; region<region_tags.size(); region++){
    for(unsigned int channel=0; channel<channel_tags.size(); channel++){
      if(channel_tags[channel] == "ttbar" && region_tags[region] == "catA") continue;

      for(unsigned int k=0; k<systematics.size(); k++){
        TString syst = systematics[k];
        cout << "============ Syst: " << syst << endl;

        for(unsigned int m=0; m<syst_shift.size(); m++){
          TString dir_nom = dir_base;
          TString dir_scale = dir_base;
          TString dir_pdf = dir_base;
          TString dir_jerc = dir_base;

          if(syst == "nominal" && m > 0) break;
          dir_nom += PostAnalysisTool::final_tag + "NOMINAL/";
          dir_jerc += PostAnalysisTool::full_tag + syst + "_" + syst_shift[m] + "/";
          dir_pdf += PostAnalysisTool::final_tag + "pdf/";
          dir_scale += PostAnalysisTool::final_tag + "scale/";

          for(unsigned int i=0; i<samples.size(); i++){
            bool force_nominal = false;
            TString sample_in = samples[i];
            TString sample_out = samples[i];
            if(sample_out == "DATA") sample_out = "data_obs_" + PostAnalysisTool::yeartag;
            cout << "-- Sample: " << sample_in << endl;

            if(sample_in == "DATA" && syst != "nominal") force_nominal = true;
            if(!sample_in.Contains("TTbar") && syst == "scale_TTbar") force_nominal = true;
            if(!sample_in.Contains("SingleTop") && syst == "scale_SingleTop") force_nominal = true;
            if(!sample_in.Contains("DYJets") && syst == "scale_DYJets") force_nominal = true;
            if(!sample_in.Contains("Diboson") && syst == "scale_Diboson") force_nominal = true;
            if(!sample_in.Contains("TTV") && syst == "scale_TTV") force_nominal = true;
            if(!sample_in.Contains("WJets") && syst == "scale_WJets") force_nominal = true;

            TString type = "MC.";
            if(sample_in == "DATA") type = "DATA.";

            if(sample_in == "DATA"){
              if(channel_tags[channel] == "ech") sample_in += "_Electron";
              else sample_in += "_Muon";
              sample_in += "_" + PostAnalysisTool::yeartag;
            }

            TString filename = dir_nom + filename_base + type + sample_in + ".root";
            if(!force_nominal){
              if(syst.Contains("scale_")) filename = dir_scale + sample_in + ".root";
              if(syst == "pdf") filename = dir_pdf + sample_in + ".root";
              // else if((syst == "JEC" || syst == "JER") && (sample_in.Contains("TTbar") || sample_in.Contains("SingleTop"))){
              //
              //
              //   filename = dir_jerc + sample_in + "_" + channel_tags[channel] + "_" + region_tags[region] + ".root";
              //   // cout << "filename: " << filename << endl;
              //
              // }
            }
            TFile* f_in = new TFile(filename, "READ");

            TString histname_in = "";
            histname_in = histfolder_base + "_" + channel_tags[channel] + "_" + region_tags[region] + "_" + syst;
            if(force_nominal) histname_in = histfolder_base + "_" + channel_tags[channel] + "_" + region_tags[region] + "_nominal";
            if(syst != "nominal" && !force_nominal) histname_in += "_" + syst_shift[m];
            histname_in += "/" + histinname_base[region];
            // if(syst == "JEC" || syst == "JER") histname_in = histfolder_base + "_" + region_tags[region] + "/" + histinname_base[region];
            if(syst.Contains("scale") && !force_nominal) histname_in = histfolder_base + "_" + channel_tags[channel] + "_" + region_tags[region] + "_scale_" + syst_shift[m] +  "/" + histinname_base[region];

            TString histname_out =  histoutname_base[region] + "_" + channel_tags[channel] + "_" + region_tags[region] + "__" + sample_out;
            if(syst != "nominal") histname_out += "__" + syst + syst_shift_combine[m];

            TH1F* h_in = (TH1F*)f_in->Get(histname_in);
            if(sample_in.Contains("LQtoTMuM300") || sample_in.Contains("LQtoTMuM400")){
              h_in->Scale(1./10.);
            }
            h_in->SetName(histname_out);
            f_out->cd();
            h_in->Write();
            f_in->Close();
          }
        }
      }
    }
  }

  cout << "finished!" << endl;



}
