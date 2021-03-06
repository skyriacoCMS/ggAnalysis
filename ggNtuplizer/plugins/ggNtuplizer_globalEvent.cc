#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;

// (local) variables associated with tree branches
Int_t     run_;
Long64_t  event_;
Int_t     lumis_;
Bool_t    isData_;
Int_t     metFilters_;
Int_t     nVtx_;
Int_t     nTrksPV_;
float     vtx_;
float     vty_;
float     vtz_;
float     rho_;
ULong64_t HLTEleMuX_;
ULong64_t HLTPho_;
ULong64_t HLTJet_;
ULong64_t HLTEleMuXIsPrescaled_;
ULong64_t HLTPhoIsPrescaled_;
ULong64_t HLTJetIsPrescaled_;
ULong64_t HLT50ns_;
ULong64_t HLTIsPrescaled50ns_;
float     genMET_;
float     genMETPhi_;
float     pfMET_;
float     pfMETPhi_;
float     pfMETsumEt_;
float     pfMETmEtSig_;
float     pfMETSig_;
float     noHFMET_;
float     noHFMETPhi_;
float     noHFMETType1_;
float     noHF_T1JERUp_;
float     noHF_T1JERDo_;
float     noHF_T1JESUp_;
float     noHF_T1JESDo_;
float     noHF_T1MESUp_;
float     noHF_T1MESDo_;
float     noHF_T1EESUp_;
float     noHF_T1EESDo_;
float     noHF_T1TESUp_;
float     noHF_T1TESDo_;
float     noHF_T1UESUp_;
float     noHF_T1UESDo_;
float     noHF_T1Phi_;
float     noHF_T1Px_;
float     noHF_T1Py_;
float     noHF_T1SumEt_;
float     noHF_T1TxyPhi_;
float     noHF_T1TxyPt_;

void ggNtuplizer::branchesGlobalEvent(TTree* tree) {

  tree->Branch("run",     &run_);
  tree->Branch("event",   &event_);
  tree->Branch("lumis",   &lumis_);
  tree->Branch("isData",  &isData_);
  if(addFilterInfoAOD_ || addFilterInfoMINIAOD_){
    tree->Branch("metFilters", &metFilters_);
  }
  tree->Branch("nVtx",                 &nVtx_);
  tree->Branch("nTrksPV",              &nTrksPV_);
  tree->Branch("vtx",                  &vtx_); 
  tree->Branch("vty",                  &vty_); 
  tree->Branch("vtz",                  &vtz_); 
  tree->Branch("rho",                  &rho_);
  tree->Branch("HLTEleMuX",            &HLTEleMuX_);
  tree->Branch("HLTPho",               &HLTPho_);
  tree->Branch("HLTJet",               &HLTJet_);
  tree->Branch("HLTEleMuXIsPrescaled", &HLTEleMuXIsPrescaled_);
  tree->Branch("HLTPhoIsPrescaled",    &HLTPhoIsPrescaled_);
  tree->Branch("HLTJetIsPrescaled",    &HLTJetIsPrescaled_);
  tree->Branch("HLT50ns",              &HLT50ns_);
  tree->Branch("HLTIsPrescaled50ns",   &HLTIsPrescaled50ns_);
  if (doGenParticles_) {
    tree->Branch("genMET",      &genMET_);
    tree->Branch("genMETPhi",   &genMETPhi_);
  }
  tree->Branch("pfMET",       &pfMET_);
  tree->Branch("pfMETPhi",    &pfMETPhi_);
  tree->Branch("pfMETsumEt",  &pfMETsumEt_);
  tree->Branch("pfMETmEtSig", &pfMETmEtSig_);
  tree->Branch("pfMETSig",    &pfMETSig_);

   if (doNoHFMET_){
    tree->Branch("noHFMET",      &noHFMET_);
    tree->Branch("noHFMETPhi",   &noHFMETPhi_);
    tree->Branch("noHFMETType1", &noHFMETType1_);     
    tree->Branch("noHF_T1JERUp", &noHF_T1JERUp_);
    tree->Branch("noHF_T1JERDo", &noHF_T1JERDo_);
    tree->Branch("noHF_T1JESUp", &noHF_T1JESUp_);
    tree->Branch("noHF_T1JESDo", &noHF_T1JESDo_);
    tree->Branch("noHF_T1MESUp", &noHF_T1MESUp_);
    tree->Branch("noHF_T1MESDo", &noHF_T1MESDo_);
    tree->Branch("noHF_T1EESUp", &noHF_T1EESUp_);
    tree->Branch("noHF_T1EESDo", &noHF_T1EESDo_);
    tree->Branch("noHF_T1TESUp", &noHF_T1TESUp_);
    tree->Branch("noHF_T1TESDo", &noHF_T1TESDo_);
    tree->Branch("noHF_T1UESUp", &noHF_T1UESUp_);
    tree->Branch("noHF_T1UESDo", &noHF_T1UESDo_);
    tree->Branch("noHF_T1Phi",   &noHF_T1Phi_);
    tree->Branch("noHF_T1Px",    &noHF_T1Px_);
    tree->Branch("noHF_T1Py",    &noHF_T1Py_);
    tree->Branch("noHF_T1SumEt", &noHF_T1SumEt_);
    tree->Branch("noHF_T1TxyPhi", &noHF_T1TxyPhi_);
    tree->Branch("noHF_T1TxyPt", &noHF_T1TxyPt_);
   }

}

void ggNtuplizer::fillGlobalEvent(const edm::Event& e, const edm::EventSetup& es) {

  edm::Handle<double> rhoHandle;
  e.getByToken(rhoLabel_, rhoHandle);

  run_    = e.id().run();
  event_  = e.id().event();
  lumis_  = e.luminosityBlock();
  isData_ = e.isRealData();
  rho_    = *(rhoHandle.product());

  edm::Handle<reco::VertexCollection> vtxHandle;
  e.getByToken(vtxLabel_, vtxHandle);
  
  nVtx_ = -1;
  if (vtxHandle.isValid()) {
    nVtx_ = 0;
    
    for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
      //bool isFake = isAOD_ ? v->isFake() : (v->chi2() == 0 && v->ndof() == 0);
      //if (!isFake) {
      if (nVtx_ == 0) {
	nTrksPV_ = v->nTracks();
	vtx_     = v->x();
	vty_     = v->y();
	vtz_     = v->z();
      }
      nVtx_++;

      //}
    }
  } else
    edm::LogWarning("ggNtuplizer") << "Primary vertices info not unavailable";

  metFilters_ = 0;
  if (addFilterInfoAOD_ && isData_ ){
    
    edm::Handle<bool> hcalNoiseHandle;
    e.getByLabel("HBHENoiseFilterResultProducer","HBHENoiseFilterResultRun1", hcalNoiseHandle);
    bool HBHENoiseResult_ = *hcalNoiseHandle;
    
    edm::Handle<bool> cSCHandle;
    e.getByLabel("CSCTightHaloFilter","", cSCHandle);
    bool CSCHaloResult_ = *cSCHandle;
    
    edm::Handle<bool> eCALTPHandle;
    e.getByLabel("EcalDeadCellTriggerPrimitiveFilter","",eCALTPHandle);
    bool EcalDeadCellTFResult_ = *eCALTPHandle;

    edm::Handle<bool> bADSCHandle;
    e.getByLabel("eeBadScFilter","", bADSCHandle);
    bool EEBadSCResult_ = *bADSCHandle;

    //    edm::Handle<bool> gOODVertexHandle;
    //    e.getByLabel("primaryVertexFilter","GoodVertexFilter",gOODVertexHandle);
    //    goodVertexResult_ = *gOODVertexHandle;
     
    if ( !HBHENoiseResult_      ) metFilters_ += 1;
    if ( !CSCHaloResult_        ) metFilters_ += 2;
    //if ( !goodVertexResult_     ) metFilters_ += 4;
    if ( !EcalDeadCellTFResult_ ) metFilters_ += 8;
    if ( !EEBadSCResult_        ) metFilters_ += 16; 
  }

  if (addFilterInfoMINIAOD_ && isData_ ){
    string filterNamesToCheck[5] = {
      "Flag_HBHENoiseFilter",
      "Flag_CSCTightHaloFilter",
      "Flag_goodVertices",
      "Flag_eeBadScFilter",
      "Flag_EcalDeadCellTriggerPrimitiveFilter"
    };

    edm::Handle<edm::TriggerResults> patFilterResultsHandle;
    e.getByToken(patTrgResultsLabel_, patFilterResultsHandle);
    edm::TriggerResults const& patFilterResults = *patFilterResultsHandle;
    
    auto&& filterNames = e.triggerNames(patFilterResults);
    for (unsigned iF = 0; iF != 5; ++iF) {
      unsigned index = filterNames.triggerIndex(filterNamesToCheck[iF]);
      if ( index == filterNames.size() ) 
	edm::LogError("Unknown MET filter label") 
	  << filterNamesToCheck[iF] << " is missing, exiting";
      else {
	if ( !patFilterResults.accept(index) ) {
	  metFilters_ += pow(2, iF);
	}
      }
    }
  }

  // HLT treatment
  HLTEleMuX_            = 0;
  HLTPho_               = 0;
  HLTJet_               = 0;
  HLTEleMuXIsPrescaled_ = 0;
  HLTPhoIsPrescaled_    = 0;
  HLTJetIsPrescaled_    = 0;
  HLT50ns_              = 0;
  HLTIsPrescaled50ns_   = 0;

  edm::Handle<edm::TriggerResults> trgResultsHandle;
  e.getByToken(trgResultsLabel_, trgResultsHandle);

  bool cfg_changed = true;
  HLTConfigProvider hltCfg;
  hltCfg.init(e.getRun(), es, trgResultsProcess_, cfg_changed);

  const edm::TriggerNames &trgNames = e.triggerNames(*trgResultsHandle);

  for (size_t i = 0; i < trgNames.size(); ++i) {
    const string &name = trgNames.triggerName(i);

    // HLT name => bit correspondence
    // Electron or Muon or Cross triggers for 25 ns
    int bitEleMuX = -1;
    if      (name.find("HLT_Ele22_eta2p1_WPLoose_Gsf_v")                    != string::npos) bitEleMuX =  0; //bit0(lowest)
    else if (name.find("HLT_Ele22_eta2p1_WPTight_Gsf_v")                    != string::npos) bitEleMuX =  1; 
    else if (name.find("HLT_Ele27_eta2p1_WPLoose_Gsf_v")                    != string::npos) bitEleMuX =  2; 
    else if (name.find("HLT_Ele27_eta2p1_WPTight_Gsf_v")                    != string::npos) bitEleMuX =  3; 
    else if (name.find("HLT_Ele32_eta2p1_WPLoose_Gsf_v")                    != string::npos) bitEleMuX =  4; 
    else if (name.find("HLT_Ele32_eta2p1_WPTight_Gsf_v")                    != string::npos) bitEleMuX =  5; 
    else if (name.find("HLT_Ele23_WPLoose_Gsf_v")                           != string::npos) bitEleMuX =  6; 
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v")       != string::npos) bitEleMuX =  7; 
    else if (name.find("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v")       != string::npos) bitEleMuX =  8; 
    else if (name.find("HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v")           != string::npos) bitEleMuX =  9;
    else if (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v")             != string::npos) bitEleMuX = 20;
    else if (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v")           != string::npos) bitEleMuX = 21;
    else if (name.find("HLT_Mu27_TkMu8_v")                                  != string::npos) bitEleMuX = 22;
    else if (name.find("HLT_DoubleIsoMu17_eta2p1_v")                        != string::npos) bitEleMuX = 23;
    else if (name.find("HLT_IsoMu24_eta2p1_v")                              != string::npos) bitEleMuX = 24;
    else if (name.find("HLT_IsoMu27_v")                                     != string::npos) bitEleMuX = 25;
    else if (name.find("HLT_Mu45_eta2p1_v")                                 != string::npos) bitEleMuX = 26;
    else if (name.find("HLT_Mu55_v")                                        != string::npos) bitEleMuX = 27;
    else if (name.find("HLT_TripleMu_12_10_5_v")                            != string::npos) bitEleMuX = 28;
    else if (name.find("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v")  != string::npos) bitEleMuX = 41;
    else if (name.find("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") != string::npos) bitEleMuX = 42;
    else if (name.find("HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v")                != string::npos) bitEleMuX = 43;
    else if (name.find("HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v")                 != string::npos) bitEleMuX = 44;
    else if (name.find("HLT_Mu17_Photon30_CaloIdL_L1ISO_v")                 != string::npos) bitEleMuX = 45;
    else if (name.find("HLT_Mu17_Photon35_CaloIdL_L1ISO_v")                 != string::npos) bitEleMuX = 46;
    else if (name.find("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v")    != string::npos) bitEleMuX = 47;
    else if (name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v")              != string::npos) bitEleMuX = 48;

    // Photon triggers for 25 ns
    int bitPho    = -1;
    if      (name.find("HLT_Photon22_v")                    != string::npos) bitPho =  0; //bit0(lowest)
    else if (name.find("HLT_Photon30_v")                    != string::npos) bitPho =  1; 
    else if (name.find("HLT_Photon36_v")                    != string::npos) bitPho =  2; 
    else if (name.find("HLT_Photon50_v")                    != string::npos) bitPho =  3; 
    else if (name.find("HLT_Photon75_v")                    != string::npos) bitPho =  4; 
    else if (name.find("HLT_Photon90_v")                    != string::npos) bitPho =  5; 
    else if (name.find("HLT_Photon120_v")                   != string::npos) bitPho =  6; 
    else if (name.find("HLT_Photon175_v")                   != string::npos) bitPho =  7; 
    else if (name.find("HLT_Photon250_NoHE_v")              != string::npos) bitPho =  8; 
    else if (name.find("HLT_Photon300_NoHE_v")              != string::npos) bitPho =  9; 
    else if (name.find("HLT_Photon500_v")                   != string::npos) bitPho = 10; 
    else if (name.find("HLT_Photon600_v")                   != string::npos) bitPho = 11; 
    else if (name.find("HLT_Photon165_HE10_v")              != string::npos) bitPho = 12; 
    else if (name.find("HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v") != string::npos) bitPho = 13;
    else if (name.find("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v") != string::npos) bitPho = 14;
    else if (name.find("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v") != string::npos) bitPho = 15;
    else if (name.find("HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v") != string::npos) bitPho = 16;
    else if (name.find("HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v") != string::npos) bitPho = 17;
    else if (name.find("HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v") != string::npos) bitPho = 18;
    else if (name.find("HLT_DoublePhoton85_v") != string::npos) bitPho = 19;

    // Jet triggers for 25 ns
    int bitJet    = -1;
    if      (name.find("HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq200_v") != string::npos) bitJet =  0;
    else if (name.find("HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq460_v") != string::npos) bitJet =  1; 
    else if (name.find("HLT_QuadPFJet_DoubleBTagCSV_VBF_Mqq240_v") != string::npos) bitJet =  2; 
    else if (name.find("HLT_QuadPFJet_SingleBTagCSV_VBF_Mqq500_v") != string::npos) bitJet =  3; 
    else if (name.find("HLT_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v")                      != string::npos) bitJet =  4;
    else if (name.find("HLT_PFMETNoMu120_JetIdCleaned_PFMHTNoMu120_IDTight_v")                    != string::npos) bitJet =  5;
    else if (name.find("HLT_MonoCentralPFJet80_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v")   != string::npos) bitJet =  6;
    else if (name.find("HLT_MonoCentralPFJet80_PFMETNoMu120_JetIdCleaned_PFMHTNoMu120_IDTight_v") != string::npos) bitJet =  7;
    else if (name.find("HLT_PFMET170_NoiseCleaned_v")              != string::npos) bitJet =  8;
    else if (name.find("HLT_CaloJet500_NoJetID_v")                 != string::npos) bitJet =  9;

    //////////////////////////////triggers for 50ns////////////////////////////////////////    
    ///muon triggers
    Long64_t bit50ns = -1;
    if      (name.find("HLT_Mu50_v")                    != string::npos) bit50ns = 0;  // bit 0 (lowest)
    else if (name.find("HLT_Mu45_eta2p1_v" )  != string::npos) bit50ns = 1; 
    else if (name.find("HLT_IsoMu20_v" )  != string::npos) bit50ns = 2; 
    else if (name.find("HLT_IsoMu17_eta2p1_v" )  != string::npos) bit50ns = 3; 
    else if (name.find("HLT_IsoTkMu20_v" )  != string::npos) bit50ns = 4; 
    else if (name.find("HLT_IsoTkMu20_eta2p1_v" )  != string::npos) bit50ns = 5; 
    else if (name.find("HLT_Mu27_TkMu8_v" )  != string::npos) bit50ns = 6; 
    else if (name.find("HLT_Mu17_TrkIsoVVL_v" )  != string::npos) bit50ns = 7; 
    else if (name.find("HLT_Mu17_Photon30_CaloIdL_L1ISO_v" )  != string::npos) bit50ns = 8; 
    else if (name.find("HLT_DoubleIsoMu17_eta2p1_v" )  != string::npos) bit50ns = 9; 
    else if (name.find("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v" )  != string::npos) bit50ns = 10; 
    else if (name.find("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v" )  != string::npos) bit50ns = 11; 
    
    ///electron triggers
    else if (name.find("HLT_Ele12_CaloIdL_TrackIdL_IsoVL_v" )  != string::npos) bit50ns = 12; 
    else if (name.find("HLT_Ele105_CaloIdVT_GsfTrkIdT_v" )  != string::npos) bit50ns = 13; 
    else if (name.find("HLT_Ele27_eta2p1_WPLoose_Gsf_v" )  != string::npos) bit50ns = 14; 
    else if (name.find("HLT_Ele27_eta2p1_WPTight_Gsf_v" )  != string::npos) bit50ns = 15; 
    else if (name.find("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v" )  != string::npos) bit50ns = 16; 
    else if (name.find("HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL_v" )  != string::npos) bit50ns = 17; 
    else if (name.find("HLT_DoubleEle24_22_eta2p1_WPLoose_Gsf_v" )  != string::npos) bit50ns = 18; 
    else if (name.find("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_MW_v" )  != string::npos) bit50ns = 19; 
    else if (name.find("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v" )  != string::npos) bit50ns = 20; 

    ///photon triggers
    else if (name.find("HLT_Photon175_v")                    != string::npos) bit50ns = 21;  
    else if (name.find("HLT_Photon250_NoHE_v" )  != string::npos) bit50ns = 22; 
    else if (name.find("HLT_Photon165_HE10_v" )  != string::npos) bit50ns = 23;
    else if (name.find("HLT_Photon165_R9Id90_HE10_IsoM_v" )  != string::npos) bit50ns = 24; 
    else if (name.find("HLT_Photon22_R9Id90_HE10_Iso40_EBOnly_VBF_v" )  != string::npos) bit50ns = 25; 
    else if (name.find("HLT_DoublePhoton85_v" )  != string::npos) bit50ns = 26; 
    else if (name.find("HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v" )  != string::npos) bit50ns = 27; 
    else if (name.find("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v" )  != string::npos) bit50ns = 28; 
    else if (name.find("HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v" )  != string::npos) bit50ns = 29; 
    else if (name.find("HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v" )  != string::npos) bit50ns = 30; 
    else if (name.find("HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v" )  != string::npos) bit50ns = 31; 
    else if (name.find("HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v" )  != string::npos) bit50ns = 32;
    // tau triggers
    else if (name.find("HLT_Ele22_eta2p1_WPLoose_Gsf_LooseIsoPFTau20_v" )  != string::npos) bit50ns = 33; 
    else if (name.find("HLT_IsoMu17_eta2p1_LooseIsoPFTau20_v")  != string::npos) bit50ns = 34;
    // Tri-muon triggers
    else if (name.find("HLT_TripleMu_12_10_5_v" )  != string::npos) bit50ns = 35; 
    // cross triggers
    else if (name.find("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v" )  != string::npos) bit50ns = 36; 
    else if (name.find("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v" )  != string::npos) bit50ns = 37; 
    else if (name.find("HLT_Mu8_DiEle12_CaloIdL_TrackIdL_v" )  != string::npos) bit50ns = 38;
    else if (name.find("HLT_DiMu9_Ele9_CaloIdL_TrackIdL_v" )  != string::npos) bit50ns = 39;
    else if (name.find("HLT_Mu17_Photon30_CaloIdL_L1ISO_v" )  != string::npos) bit50ns = 40;
    else if (name.find("HLT_Mu17_Photon35_CaloIdL_L1ISO_v" )  != string::npos) bit50ns = 41;
    else if (name.find("HLT_Photon30_v"  )  != string::npos) bit50ns = 42;
    else if (name.find("HLT_Photon36_v"  )  != string::npos) bit50ns = 43;
    else if (name.find("HLT_Photon50_v"  )  != string::npos) bit50ns = 44;
    else if (name.find("HLT_Photon75_v"  )  != string::npos) bit50ns = 45;
    else if (name.find("HLT_Photon90_v"  )  != string::npos) bit50ns = 46;
    else if (name.find("HLT_Photon120_v"  )  != string::npos) bit50ns = 47;
    else if (name.find("HLT_Photon300_NoHE_v"  )  != string::npos) bit50ns = 48;
    else if (name.find("HLT_Photon500_v"  )  != string::npos) bit50ns = 49;
    else if (name.find("HLT_Photon600_v"  )  != string::npos) bit50ns = 50;

    // indicates prescaling and whether trigger was fired or not
    ULong64_t isPrescaled = (hltCfg.prescaleValue(e, es, name)!=1) ? 1 : 0;
    ULong64_t isFired     = (trgResultsHandle->accept(i)) ? 1 : 0;

    if (bitEleMuX >= 0) {
      HLTEleMuX_            |= (isFired << bitEleMuX);
      HLTEleMuXIsPrescaled_ |= (isPrescaled << bitEleMuX);
    }

    if (bitPho >= 0) {
      HLTPho_            |= (isFired << bitPho);
      HLTPhoIsPrescaled_ |= (isPrescaled << bitPho);
    }

    if (bitJet >= 0) {
      HLTJet_            |= (isFired << bitJet);
      HLTJetIsPrescaled_ |= (isPrescaled << bitJet);
    }

    if (bit50ns >= 0) {
      HLT50ns_            |= (isFired << bit50ns);
      HLTIsPrescaled50ns_ |= (isPrescaled << bit50ns);
    }

    //////////////////////////end of HLT 50ns menu////////////////////////////
    //cout<<"HLT : "<<i<<" "<<name<<" "<<isPrescaled<<endl;

  }

  edm::Handle<edm::View<pat::MET> > pfMETHandle;
  e.getByToken(pfMETlabel_, pfMETHandle);

  genMET_      = -99;
  genMETPhi_   = -99;
  pfMET_       = -99;
  pfMETPhi_    = -99;
  pfMETsumEt_  = -99;
  pfMETmEtSig_ = -99;
  pfMETSig_    = -99;

  if (pfMETHandle.isValid()) {
    const pat::MET *pfMET = 0;
    pfMET = &(pfMETHandle->front());

    pfMET_       = pfMET->et();
    pfMETPhi_    = pfMET->phi();
    pfMETsumEt_  = pfMET->sumEt();
    pfMETmEtSig_ = (pfMET->mEtSig() < 1.e10) ? pfMET->mEtSig() : 0;
    pfMETSig_    = (pfMET->significance() < 1.e10) ? pfMET->significance() : 0;;

    if (!isData_) {
      genMET_    = pfMET->genMET()->et();
      genMETPhi_ = pfMET->genMET()->phi();
    }

  } else
    edm::LogWarning("ggNtuplizer") << "MET info unavailable";

   if (doNoHFMET_){
    pat::METCollection newMet;
    pat::METCollection t1txyMet;

    edm::Handle < pat::METCollection > newMetHandle;
    edm::InputTag _newMetLabel("slimmedMETsNoHF","","ggKit");
    edm::InputTag _t1txyMetLabel("patPFMetT1TxyNoHF");
    e.getByLabel(_newMetLabel,newMetHandle);
    if (newMetHandle.isValid() ) newMet = *newMetHandle;

    edm::Handle < pat::METCollection > t1txyMetHandle;
    e.getByLabel(_t1txyMetLabel,t1txyMetHandle);
    if (t1txyMetHandle.isValid() ) t1txyMet = *t1txyMetHandle;
    if (t1txyMetHandle.isValid() && newMetHandle.isValid()){
      noHFMET_   = newMet[0].pt();
      noHFMETPhi_= newMet[0].phi();

      //alternate way to get the Type1 value
      noHFMETType1_  = newMet[0].shiftedPt(pat::MET::NoShift, pat::MET::Type1); //second argument is Type1 per default

      //Type1MET uncertainties =======================================
      noHF_T1JERUp_ = newMet[0].shiftedPt(pat::MET::JetResUp);
      noHF_T1JERDo_ = newMet[0].shiftedPt(pat::MET::JetResDown);
      noHF_T1JESUp_ = newMet[0].shiftedPt(pat::MET::JetEnUp);
      noHF_T1JESDo_ = newMet[0].shiftedPt(pat::MET::JetEnDown);
      noHF_T1MESUp_ = newMet[0].shiftedPt(pat::MET::MuonEnUp);
      noHF_T1MESDo_ = newMet[0].shiftedPt(pat::MET::MuonEnDown);
      noHF_T1EESUp_ = newMet[0].shiftedPt(pat::MET::ElectronEnUp);
      noHF_T1EESDo_ = newMet[0].shiftedPt(pat::MET::ElectronEnDown);
      noHF_T1TESUp_ = newMet[0].shiftedPt(pat::MET::TauEnUp);
      noHF_T1TESDo_ = newMet[0].shiftedPt(pat::MET::TauEnDown);
      noHF_T1UESUp_ = newMet[0].shiftedPt(pat::MET::UnclusteredEnUp);
      noHF_T1UESDo_ = newMet[0].shiftedPt(pat::MET::UnclusteredEnDown);

      //other functions to access the shifted MET variables =================
      noHF_T1Phi_ = newMet[0].shiftedPhi(pat::MET::NoShift);  //second argument is Type1 per default
      noHF_T1Px_  = newMet[0].shiftedPx(pat::MET::NoShift);  //second argument is Type1 per default
      noHF_T1Py_  = newMet[0].shiftedPy(pat::MET::NoShift);  //second argument is Type1 per default
      noHF_T1SumEt_ = newMet[0].shiftedSumEt(pat::MET::NoShift);  //second argument is Type1 per default


      //extra Txy stuff =======================================================
      noHF_T1TxyPhi_ = t1txyMet[0].phi();
      noHF_T1TxyPt_ = t1txyMet[0].pt();
    }else
    edm::LogWarning("ggNtuplizer") << "MET info unavailable";


  }




}
