#pragma once

#include <TString.h>
#include <TH1F.h>
#include <map>
#include <TTreeReader.h>
#include <TROOT.h>
#include "LQDM/Analyzer/include/LQDMEvent.h"
#include "Analyzer/include/BaseHists.h"
#include "Analyzer/include/Muon.h"
#include "Analyzer/include/useful_functions.h"

using namespace std;

class LQDMGenParticleOriginHists : public BaseHists{

public:
  // Constructors, destructor
  LQDMGenParticleOriginHists(TString dir_);
  LQDMGenParticleOriginHists(const LQDMGenParticleOriginHists &) = default;
  LQDMGenParticleOriginHists & operator = (const LQDMGenParticleOriginHists &) = default;
  ~LQDMGenParticleOriginHists() = default;

  // Main functions
  void fill(const LQDMEvent & event);


protected:

  shared_ptr<TH1D>

  hgenmuorigin, hgenmu1origin, hgenmu2origin,

  hgenmusfromtautotal, hgenmusfromtaumatched, hptgenmufromtau, hptgenmufromtau_rebin, hptgenmufromtau_rebin2, hptgenmufromtaumatched, hptgenmufromtaumatched_rebin, hptgenmufromtaumatched_rebin2, hdrgendimufromtau, hdphigendimufromtau, hdetagendimufromtau, hdrgenmufromtauvistau, hdphigenmufromtauvistau, hdetagenmufromtauvistau, hdrgenmufromtaue, hdphigenmufromtaue, hdetagenmufromtaue,

  hgenmusfromhadtotal, hgenmusfromhadmatched, hptgenmufromhad, hptgenmufromhad_rebin, hptgenmufromhad_rebin2, hptgenmufromhadmatched, hptgenmufromhadmatched_rebin, hptgenmufromhadmatched_rebin2, hdrgendimufromhad, hdphigendimufromhad, hdetagendimufromhad, hdrgenmufromhadvistau, hdphigenmufromhadvistau, hdetagenmufromhadvistau, hdrgenmufromhade, hdphigenmufromhade, hdetagenmufromhade,

  hgenelorigin, hgenel1origin, hgenel2origin,

  hgenelsfromtautotal, hgenelsfromtaumatched, hptgenelfromtau, hptgenelfromtau_rebin, hptgenelfromtau_rebin2, hptgenelfromtaumatched, hptgenelfromtaumatched_rebin, hptgenelfromtaumatched_rebin2, hdrgendiefromtau, hdphigendiefromtau, hdetagendiefromtau, hdrgenefromtauvistau, hdphigenefromtauvistau, hdetagenefromtauvistau, hdrgenefromtaumu, hdphigenefromtaumu, hdetagenefromtaumu,

  hgenelsfromhadtotal, hgenelsfromhadmatched, hptgenelfromhad, hptgenelfromhad_rebin, hptgenelfromhad_rebin2, hptgenelfromhadmatched, hptgenelfromhadmatched_rebin, hptgenelfromhadmatched_rebin2, hdrgendiefromhad, hdphigendiefromhad, hdetagendiefromhad, hdrgenefromhadvistau, hdphigenefromhadvistau, hdetagenefromhadvistau, hdrgenefromhadmu, hdphigenefromhadmu, hdetagenefromhadmu,

  hgenborigin, hgenb1origin, hgenb2origin,

  hgenbsfromhardtotal, hgenbsfromhardmatched, hptgenbfromhard, hptgenbfromhard_rebin, hptgenbfromhard_rebin2, hptgenbfromhardmatched, hptgenbfromhardmatched_rebin, hptgenbfromhardmatched_rebin2, hdrgendibfromhard, hdphigendibfromhard, hdetagendibfromhard, hdrgenbfromhardvistau, hdphigenbfromhardvistau, hdetagenbfromhardvistau, hdrgenbfromhardmu, hdphigenbfromhardmu, hdetagenbfromhardmu, hdrgenbfromharde, hdphigenbfromharde, hdetagenbfromharde,

  hgenbsnotfromhardtotal, hgenbsnotfromhardmatched, hptgenbnotfromhard, hptgenbnotfromhard_rebin, hptgenbnotfromhard_rebin2, hptgenbnotfromhardmatched, hptgenbnotfromhardmatched_rebin, hptgenbnotfromhardmatched_rebin2, hdrgendibnotfromhard, hdphigendibnotfromhard, hdetagendibnotfromhard, hdrgenbnotfromhardvistau, hdphigenbnotfromhardvistau, hdetagenbnotfromhardvistau, hdrgenbnotfromhardmu, hdphigenbnotfromhardmu, hdetagenbnotfromhardmu, hdrgenbnotfromharde, hdphigenbnotfromharde, hdetagenbnotfromharde;






};
