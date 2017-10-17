#include "CSCOfflineTiming/CSCTimingAnalyzer/plugins/CSCTimingAnalyzer.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include <iostream>
#include <fstream>

const double CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR = -235.8;
const double CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR = -217.;

CSCTimingAnalyzer::CSCTimingAnalyzer(const edm::ParameterSet& iConfig)
{
  makeChamberTimingCorrectionValueHists_ = iConfig.getUntrackedParameter<bool>("makeChamberTimingCorrectionValueHists" , false );
  makePlotsPerChamber_                   = iConfig.getUntrackedParameter<bool>("makePlotsPerChamber"                   , false );
  makePlotsPerLayer_                     = iConfig.getUntrackedParameter<bool>("makePlotsPerLayer"                     , false );
  printTimeCorrectionParametersToFile_   = iConfig.getUntrackedParameter<bool>("printTimeCorrectionParametersToFile"   , false );
  writeTimeCorrectionParametersToTree_   = iConfig.getUntrackedParameter<bool>("writeTimeCorrectionParametersToTree"   , false );
  verbose_                               = iConfig.getUntrackedParameter<bool>("verbose"                               , false );
  debug_                                 = iConfig.getUntrackedParameter<bool>("debug"                                 , false );
  applyUpdatedME11corrections_           = iConfig.getUntrackedParameter<bool>("applyUpdatedME11corrections"           , false );
  checkCSCstatus_                        = iConfig.getUntrackedParameter<bool>("checkCSCstatus"                        , false );
  checkDCSstatus_                        = iConfig.getUntrackedParameter<bool>("checkDCSstatus"                        , false );
  removeHeuristicCorrection_             = iConfig.getUntrackedParameter<bool>("removeHeuristicCorrection"             , false );
  applyNewHeuristicCorrectionByRing_     = iConfig.getUntrackedParameter<bool>("applyNewHeuristicCorrectionByRing"     , false );
  applyNewHeuristicCorrectionByChamber_  = iConfig.getUntrackedParameter<bool>("applyNewHeuristicCorrectionByChamber"  , false );
  makeNminus1hists_                      = iConfig.getUntrackedParameter<bool>("makeNminus1hists"                      , false );
  writeTimingStudyBabyTree_              = iConfig.getUntrackedParameter<bool>("writeTimingStudyBabyTree"              , false );
  removeMatchedStationsRequirement_      = iConfig.getUntrackedParameter<bool>("removeMatchedStationsRequirement"      , false );
  combineME11ab_                         = iConfig.getUntrackedParameter<bool>("combineME11ab"                         , true  );
  requireInsideOutMuons_                 = iConfig.getUntrackedParameter<bool>("requireInsideOutMuons"                 , true  );
  min_pt_                                = iConfig.getUntrackedParameter<double>("min_pt"                              , 10.   );
  max_dz_                                = iConfig.getUntrackedParameter<double>("max_dz"                              , 0.5   );
  min_run_                               = iConfig.getUntrackedParameter<int>("min_run"                                , 0     );
  max_run_                               = iConfig.getUntrackedParameter<int>("max_run"                                , 999999);
  outfileName_                           = iConfig.getUntrackedParameter<std::string>("outfileName", "histos.root");
  timeParamFileName_                     = iConfig.getUntrackedParameter<std::string>("timeParamFileName", "cscTimingParameters.root");
  timingStudyBabyFileName_               = iConfig.getUntrackedParameter<std::string>("timingStudyBabyFileName", "timingBaby.root");
  fpForHeuristicCorrByRing_              = iConfig.getParameter<edm::FileInPath>("fpForHeuristicCorrByRing");
  fpForHeuristicCorrByChamber_           = iConfig.getParameter<edm::FileInPath>("fpForHeuristicCorrByChamber");    
    
  fileForHeuristicCorrByRing_            = fpForHeuristicCorrByRing_.fullPath();
  fileForHeuristicCorrByChamber_         = fpForHeuristicCorrByChamber_.fullPath();
    
  histos_  = new CSCValHists();
  outfile_ = new TFile(outfileName_.c_str(), "RECREATE");

  if (writeTimingStudyBabyTree_)
  {
    babyFile = new TFile(timingStudyBabyFileName_.c_str(), "RECREATE");
    babyTree = new TTree("tree", "baby tree for timing studies");
  }
  else
  {
    babyFile = 0;
    babyTree = 0;
  }

  run_token                         = consumes<int>                                                            (edm::InputTag ("cscTimingBabyMaker" , "run"                        )) ;
  lumi_token                        = consumes<int>                                                            (edm::InputTag ("cscTimingBabyMaker" , "lumi"                       )) ;
  evt_token                         = consumes<unsigned long long>                                             (edm::InputTag ("cscTimingBabyMaker" , "evt"                        )) ;
  bfield_token                      = consumes<double>                                                         (edm::InputTag ("cscTimingBabyMaker" , "bfield"                     )) ;
  csc_status_token                  = consumes<bool>                                                           (edm::InputTag ("cscTimingBabyMaker" , "cscstatus"                  )) ;
  detector_status_token             = consumes<unsigned int>                                                   (edm::InputTag ("cscTimingBabyMaker" , "detectorstatus"             )) ;
  is_global_token                   = consumes<std::vector<bool> >                                             (edm::InputTag ("cscTimingBabyMaker" , "isglobal"                   )) ;
  is_tracker_token                  = consumes<std::vector<bool> >                                             (edm::InputTag ("cscTimingBabyMaker" , "istracker"                  )) ;
  is_sa_token                       = consumes<std::vector<bool> >                                             (edm::InputTag ("cscTimingBabyMaker" , "issa"                       )) ;
  is_pf_token                       = consumes<std::vector<bool> >                                             (edm::InputTag ("cscTimingBabyMaker" , "ispf"                       )) ;
  p4_token                          = consumes<std::vector<LorentzVector> >                                    (edm::InputTag ("cscTimingBabyMaker" , "p4"                         )) ;
  dz_token                          = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "dz"                         )) ;
  d0_token                          = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "d0"                         )) ;
  dz_bs_token                       = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "dzbs"                       )) ;
  d0_bs_token                       = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "d0bs"                       )) ;
  gfit_chi2_token                   = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "gfitchi2"                   )) ;
  gfit_ndof_token                   = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "gfitndof"                   )) ;
  maxOpeningAngleTrack_token        = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "maxOpeningAngleTrack"       )) ;
  maxOpeningAngleMuon_token         = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "maxOpeningAngleMuon"        )) ;
  time_token                        = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "time"                       )) ;
  timeErr_token                     = consumes<std::vector<double> >                                           (edm::InputTag ("cscTimingBabyMaker" , "timeErr"                    )) ;  
  type_token                        = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "type"                       )) ;
  valid_siLayers_token              = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "validsiLayers"              )) ;
  valid_pixelHits_token             = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "validpixelHits"             )) ;
  charge_token                      = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "charge"                     )) ;
  trk_charge_token                  = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "trkcharge"                  )) ;
  gfit_validSTAhits_token           = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "gfitvalidSTAhits"           )) ;
  gfit_validSiHits_token            = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "gfitvalidSiHits"            )) ;
  sta_validHits_token               = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "stavalidHits"               )) ;
  numMatchedStations_token          = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "numMatchedStations"         )) ;
  numMatchedCSCsegments_token       = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "numMatchedCSCsegments"      )) ;
  station_mask_token                = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "stationmask"                )) ;
  direction_token                   = consumes<std::vector<int> >                                              (edm::InputTag ("cscTimingBabyMaker" , "direction"                  )) ;
  distToChamberEdge_token           = consumes<std::vector<std::vector<double> > >                             (edm::InputTag ("cscTimingBabyMaker" , "distToChamberEdge"          )) ;
  distToChamberEdgeErr_token        = consumes<std::vector<std::vector<double> > >                             (edm::InputTag ("cscTimingBabyMaker" , "distToChamberEdgeErr"       )) ;
  numCSCsegmentsInChamber_token     = consumes<std::vector<std::vector<int> > >                                (edm::InputTag ("cscTimingBabyMaker" , "numCSCsegmentsInChamber"    )) ;
  arbitration_mask_token            = consumes<std::vector<std::vector<std::vector<unsigned int> > > >         (edm::InputTag ("cscTimingBabyMaker" , "arbitrationmask"            )) ;
  isSegmentAndTrackArbitrated_token = consumes<std::vector<std::vector<std::vector<bool> > > >                 (edm::InputTag ("cscTimingBabyMaker" , "isSegmentAndTrackArbitrated")) ;
  numRecHitsInSegment_token         = consumes<std::vector<std::vector<std::vector<int> > > >                  (edm::InputTag ("cscTimingBabyMaker" , "numRecHitsInSegment"        )) ;
  segmentTime_token                 = consumes<std::vector<std::vector<std::vector<double> > > >               (edm::InputTag ("cscTimingBabyMaker" , "segmentTime"                )) ;
  endcap_token                      = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "endcap"                     )) ;
  station_token                     = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "station"                    )) ;
  ring_token                        = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "ring"                       )) ;
  chamber_token                     = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "chamber"                    )) ;
  layer_token                       = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "layer"                      )) ;
  nstrips_token                     = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "nstrips"                    )) ;
  rhtime_token                      = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "rhtime"                     )) ;
  twire_token                       = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "twire"                      )) ;
  cfeb_cable_length_token           = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "cfebcablelength"            )) ;
  cfeb_skew_delay_token             = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "cfebskewdelay"              )) ;
  cfeb_timing_corr_token            = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "cfebtimingcorr"             )) ;
  cfeb_cable_delay_token            = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "cfebcabledelay"             )) ;
  precision_token                   = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "precision"                  )) ;
  cfeb_cable_rev_token              = consumes<std::vector<std::vector<std::vector<std::vector<char> > > > >   (edm::InputTag ("cscTimingBabyMaker" , "cfebcablerev"               )) ;
  anode_bx_offset_token             = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "anodebxoffset"              )) ;
  chipCorr_token                    = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "cfebCorr"                   )) ;
  cfebCorr_token                    = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "chipCorr"                   )) ;
  skewClearDelay_token              = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "skewClearDelay"             )) ;
  cfebCableDelay_token              = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "cfebCableDelay"             )) ;
  chamberCorr_token                 = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "chamberCorr"                )) ;
  new_skewClearDelay_token          = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "newskewClearDelay"          )) ;
  new_cfebCableDelay_token          = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "newcfebCableDelay"          )) ;
  new_chamberCorr_token             = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "newchamberCorr"             )) ;
  new_cfeb_cable_length_token       = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "newcfebcablelength"         )) ;
  new_cfeb_cable_rev_token          = consumes<std::vector<std::vector<std::vector<std::vector<char> > > > >   (edm::InputTag ("cscTimingBabyMaker" , "newcfebcablerev"            )) ;
  new_cfeb_skew_delay_token         = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "newcfebskewdelay"           )) ;
  new_cfeb_cable_delay_token        = consumes<std::vector<std::vector<std::vector<std::vector<int> > > > >    (edm::InputTag ("cscTimingBabyMaker" , "newcfebcabledelay"          )) ;
  new_cfebCorr_token                = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "newcfebCorr"                )) ;
  new_chipCorr_token                = consumes<std::vector<std::vector<std::vector<std::vector<double> > > > > (edm::InputTag ("cscTimingBabyMaker" , "newchipCorr"                )) ;
  hlt_bits_token                    = consumes<TBits>                                                          (edm::InputTag ("cscTimingBabyMaker" , "hltbits"                    )) ;
  hlt_trigNames_token               = consumes<std::vector<TString> >                                          (edm::InputTag ("cscTimingBabyMaker" , "hlttrigNames"               )) ;
  hlt_prescales_token               = consumes<std::vector<unsigned int> >                                     (edm::InputTag ("cscTimingBabyMaker" , "hltprescales"               )) ;
  hlt_trigObjs_id_token             = consumes<std::vector<std::vector<int> > >                                (edm::InputTag ("cscTimingBabyMaker" , "hlttrigObjsid"              )) ;
  hlt_trigObjs_p4_token             = consumes<std::vector<std::vector<LorentzVector> > >                      (edm::InputTag ("cscTimingBabyMaker" , "hlttrigObjsp4"              )) ;
}


CSCTimingAnalyzer::~CSCTimingAnalyzer()
{
  histos_->writeHists(outfile_);
  outfile_->Close();
}


//
// member functions
//

// ------------ method called for each event  ------------
void
CSCTimingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  loadBranches(iEvent);
  if (!run_h.isValid()) return;

   
  getVars();
  if (debug_)
    printf("\n\nrun, lumi, evt, csc_status: %d, %d, %llu, %d\n\n", run, lumi, evt, csc_status); 

  if (run < min_run_) return;
  if (run > max_run_) return;

  if (checkCSCstatus_ && !csc_status) return;
  if (checkDCSstatus_ && !(detector_status & (0xF << 24))) return;

  size_t nMuons = p4_h->size();
  for (size_t nmu = 0; nmu < nMuons; nmu++)
  {
    getVars(nmu);
    if (debug_)
      printf("\n\tMuon %zd: eta = %4.2f, is_global = %d, numMatchedStations = %d, numMatchedCSCsegments = %d\n", nmu, p4.eta(), is_global, numMatchedStations, numMatchedCSCsegments);

    if (p4.pt() < min_pt_) continue;
    if (requireInsideOutMuons_ && direction < 0) continue;
    if (makeNminus1hists_)
      CSCTimingAnalyzer::fillNm1hists();
    if (!CSCTimingAnalyzer::isPOGmuonTight()) continue;
    bool isGoodMuWithArbitratedSegment = false;
    size_t nChambers = numCSCsegmentsInChamber_h->at(nmu).size();
    for (size_t nch = 0; nch < nChambers; nch++)
    {
      getVars(nmu, nch);
      if (debug_)
        printf("\t\tchamber %zd: numCSCsegmentsInChamber = %d\n", nch, numCSCsegmentsInChamber);

      size_t nSegments = numRecHitsInSegment_h->at(nmu).at(nch).size();
      if (nSegments > 0)
      {
        histos_->fill1DHist(std::min(std::max(distToChamberEdge,-59.9),9.9), "hChamberEdgeDist", "muon quality", 140, -60, 10, "Muons");
        histos_->fill1DHist(std::min(std::max(distToChamberEdge/distToChamberEdgeErr,-2.99),0.99), "hChamberEdgeSigmas", "muon quality", 40, -3, 1, "Muons");
      }
      for (size_t nseg = 0; nseg < nSegments; nseg++)
      {
        getVars(nmu, nch, nseg);
        if (debug_)
          printf("\t\t\tsegment %zd: isSegmentAndTrackArbitrated = %d, numRecHitsInSegment = %d\n", nseg, isSegmentAndTrackArbitrated, numRecHitsInSegment);

        // only use arbitrated segments
        if (!isSegmentAndTrackArbitrated) continue;
        isGoodMuWithArbitratedSegment = true;                
                
        size_t nRecHits = station_h->at(nmu).at(nch).at(nseg).size();
        histos_->fill1DHist(numRecHitsInSegment, "hNumRHinSegment", "segment quality", 10, -0.5, 9.5, "Muons");
        std::vector<double> rhTimes_;
        std::vector<double> wireTimes_;
        for (size_t nrh = 0; nrh < nRecHits; nrh++)
        {
          getVars(nmu,nch,nseg,nrh);
          if (debug_)
          {
            printf("\t\t\t\trechit %zd: (endcap,station,ring,chamber) = (%d,%d,%d,%d) has (cathode,anode) time = (%4.2f,%4.2f)\n", nrh, endcap, station, ring, chamber, rhtime, twire);
            printf("\t\t\t\t\tnstrips = %d, chipCorr = %4.2f, cfebDelay = %4.2f, skewClearDelay = %4.2f, heuristic = %4.2f\n", nstrips, chipCorr, cfebCableDelay, skewClearDelay, chamberCorr-cfebCableDelay-skewClearDelay);
            printf("\t\t\t\t\tnstrips = %d, new_chipCorr = %4.2f, new_cfebDelay = %4.2f, new_skewClearDelay = %4.2f, heuristic = %4.2f\n", nstrips, new_chipCorr, new_cfebCableDelay, new_skewClearDelay, chamberCorr-cfebCableDelay-skewClearDelay);
          }

          if (makeChamberTimingCorrectionValueHists_ || printTimeCorrectionParametersToFile_ || writeTimeCorrectionParametersToTree_)
            fillMaps(CSCDetId(endcap, station, ring, chamber));

          CSCDetId id(endcap, station, ring, chamber, layer);                    
          double rhtime_corr = rhtime;
          double twire_corr = twire;
                    
          if (station == 1 && (ring == 1 || ring == 4))
          {
            if (applyUpdatedME11corrections_)
            {
              // use average chip correction
              // rhtime_corr += (new_chipCorr - chipCorr);
                            
              // update CFEB cable delay
              // rhtime_corr += (new_cfebCableDelay - cfebCableDelay);
                            
              // update skewclear delay
              rhtime_corr += (new_skewClearDelay - skewClearDelay);
                            
              if (removeHeuristicCorrection_)
                rhtime_corr -= (new_chamberCorr - new_cfebCableDelay - new_skewClearDelay);
              else if (applyNewHeuristicCorrectionByRing_)
              {
                rhtime_corr -= (new_chamberCorr - new_cfebCableDelay - new_skewClearDelay); // first remove old hueristic correction
                CSCDetId tmp_id(endcap,station,ring,0);
                if (m_new_heuristicCorr.find(tmp_id) != m_new_heuristicCorr.end())
                  rhtime_corr += m_new_heuristicCorr[tmp_id]; // now apply new hueristic correction
                else                                       
                  rhtime_corr += (station == 1 && (ring == 1 || ring == 4)) ? CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR : CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR; // now apply new hueristic correction                                            

                // if (endcap == 1 && station == 1 && ring == 3 && chamber == 16)
                // {
                //     rhtime_corr += 25.; // this affectively changes the cable delay from 1 to 0
                // }
              }
              else if (applyNewHeuristicCorrectionByChamber_)
              {
                if (station == 1 && (ring == 4))
                {
                  if (verbose_)
                  {
                    CSCDetId tmp_id(endcap,station,ring,chamber);
                    if (m_new_heuristicCorr.find(tmp_id) != m_new_heuristicCorr.end())
                      printf("correction for e=%d, c=%d is %4.2f\n", endcap, chamber, m_new_heuristicCorr[tmp_id]);
                    else                                       
                      printf("correction for ME+1/1/1 is %4.2f\n", CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR);
                  }
                }
                rhtime_corr -= (new_chamberCorr - new_cfebCableDelay - new_skewClearDelay); // first remove old hueristic correction
                CSCDetId tmp_id(endcap,station,ring,chamber);
                if (m_new_heuristicCorr.find(tmp_id) != m_new_heuristicCorr.end())
                  rhtime_corr += m_new_heuristicCorr[tmp_id]; // now apply new hueristic correction
                else                                       
                  rhtime_corr += (station == 1 && (ring == 1 || ring == 4)) ? CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR : CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR; // now apply new hueristic correction                               
              }
            }
            else if (removeHeuristicCorrection_)
            {
              rhtime_corr -= (chamberCorr - cfebCableDelay - skewClearDelay);
            }
            else if (applyNewHeuristicCorrectionByRing_)
            {
              rhtime_corr -= (chamberCorr - cfebCableDelay - skewClearDelay); // first remove old hueristic correction
              CSCDetId tmp_id(endcap,station,ring,0);
              if (m_new_heuristicCorr.find(tmp_id) != m_new_heuristicCorr.end())
                rhtime_corr += m_new_heuristicCorr[tmp_id]; // now apply new hueristic correction
              else                                       
                rhtime_corr += (station == 1 && (ring == 1 || ring == 4)) ? CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR : CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR; // now apply new hueristic correction
            }
            else if (applyNewHeuristicCorrectionByChamber_)
            {
              rhtime_corr -= (chamberCorr - cfebCableDelay - skewClearDelay); // first remove old hueristic correction
              CSCDetId tmp_id(endcap,station,ring,chamber);
              if (m_new_heuristicCorr.find(tmp_id) != m_new_heuristicCorr.end())
                rhtime_corr += m_new_heuristicCorr[tmp_id]; // now apply new hueristic correction
              else                                       
                rhtime_corr += (station == 1 && (ring == 1 || ring == 4)) ? CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR : CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR; // now apply new hueristic correction                               
            }
          }
          else if (removeHeuristicCorrection_)
          {
            rhtime_corr -= (chamberCorr - cfebCableDelay - skewClearDelay);
          }
          else if (applyNewHeuristicCorrectionByRing_)
          {
            rhtime_corr -= (chamberCorr - cfebCableDelay - skewClearDelay); // first remove old hueristic correction
            CSCDetId tmp_id(endcap,station,ring,0);
            if (m_new_heuristicCorr.find(tmp_id) != m_new_heuristicCorr.end())
              rhtime_corr += m_new_heuristicCorr[tmp_id]; // now apply new hueristic correction
            else                                       
              rhtime_corr += (station == 1 && (ring == 1 || ring == 4)) ? CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR : CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR; // now apply new hueristic correction
          }
          else if (applyNewHeuristicCorrectionByChamber_)
          {
            rhtime_corr -= (chamberCorr - cfebCableDelay - skewClearDelay); // first remove old hueristic correction
            CSCDetId tmp_id(endcap,station,ring,chamber);
            if (m_new_heuristicCorr.find(tmp_id) != m_new_heuristicCorr.end())
              rhtime_corr += m_new_heuristicCorr[tmp_id]; // now apply new hueristic correction
            else                                       
              rhtime_corr += (station == 1 && (ring == 1 || ring == 4)) ? CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR : CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR; // now apply new hueristic correction                               
          }

          if (endcap == 2 && station == 1 && ring == 3 && chamber == 3 && chipCorr < -60)
          {
            rhtime_corr -= (chipCorr + 52.41);
          }

          // Temporary fix for applying twire correction on the fly
          if (station == 1)   twire_corr += 2;
          else if (ring == 2) twire_corr += 1.75;
          else if (ring == 1) twire_corr += 1.50;
          else std::cout << "WARNING: has left over ring for wire time correction!!\n";

          rhTimes_.push_back(rhtime_corr);
          wireTimes_.push_back(twire_corr);
          rhtime_corr /= 50.;                    

          // fill histogram per station
          if (removeHeuristicCorrection_)
          {
            if (combineME11ab_ && station == 1 && ring == 4)
            {
              CSCDetId tmp_id(endcap, station, 1, chamber);
              histos_->fill1DHistByType(rhtime_corr, "hRHTiming", "recHit Timing", tmp_id, 200, 2, 6, "recHits");
              histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", tmp_id, 10, -0.5, 9.5, "recHits");
              histos_->fill1DHistByType(twire_corr, "hAnodeTiming", "anode Timing", tmp_id, 200, -25, 25, "recHits");
            }
            else
            {
              histos_->fill1DHistByType(rhtime_corr, "hRHTiming", "recHit Timing", id, 200, 2, 6, "recHits");
              histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", id, 10, -0.5, 9.5, "recHits");
              histos_->fill1DHistByType(twire_corr, "hAnodeTiming", "anode Timing", id, 200, -25, 25, "recHits");
            }
            if (makePlotsPerChamber_)
            {
              if (combineME11ab_ && station == 1 && ring == 4)
              {
                CSCDetId tmp_id(endcap, station, 1, chamber);
                histos_->fill1DHistByChamber(rhtime_corr, "hRHTiming", "recHit Timing", tmp_id, 200, 2, 6, "recHitsByChamber");
                histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", tmp_id, 10, -0.5, 9.5, "recHits");
                histos_->fill1DHistByChamber(twire_corr, "hAnodeTiming", "anode Timing", tmp_id, 200, -25, 25, "recHitsByChamber");
              }
              else
              {
                histos_->fill1DHistByChamber(rhtime_corr, "hRHTiming", "recHit Timing", id, 200, 2, 6, "recHitsByChamber");
                histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", id, 10, -0.5, 9.5, "recHits");
                histos_->fill1DHistByChamber(twire_corr, "hAnodeTiming", "anode Timing", id, 200, -25, 25, "recHitsByChamber");
              }
            }
            if (makePlotsPerLayer_)
            {
              histos_->fill1DHistByLayer(rhtime_corr, "hRHTiming", "recHit Timing", id, 200, 2, 6, "recHitsByLayer");
              histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", id, 10, -0.5, 9.5, "recHits");
            }
          }
          else
          {
            if (combineME11ab_ && station == 1 && ring == 4)
            {
              CSCDetId tmp_id(endcap, station, 1, chamber);                            
              histos_->fill1DHistByType(rhtime_corr, "hRHTiming", "recHit Timing", tmp_id, 200, -2, 2, "recHits");
              histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", tmp_id, 10, -0.5, 9.5, "recHits");
              histos_->fill1DHistByType(twire_corr, "hAnodeTiming", "anode Timing", tmp_id, 200, -25, 25, "recHits");
              histos_->fill1DHistByType(anode_bx_offset, "hAndoeBXOffset", "anode_bx_offset", tmp_id, 100, 800, 900, "recHits");
            }
            else
            {
              histos_->fill1DHistByType(rhtime_corr, "hRHTiming", "recHit Timing", id, 200, -2, 2, "recHits");
              histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", id, 10, -0.5, 9.5, "recHits");
              histos_->fill1DHistByType(twire_corr, "hAnodeTiming", "anode Timing", id, 200, -25, 25, "recHits");
              histos_->fill1DHistByType(anode_bx_offset, "hAndoeBXOffset", "anode_bx_offset", id, 100, 800, 900, "recHits");
            }
            if (makePlotsPerChamber_)
            {
              if (combineME11ab_ && station == 1 && ring == 4)
              {
                CSCDetId tmp_id(endcap, station, 1, chamber);                            
                histos_->fill1DHistByChamber(rhtime_corr, "hRHTiming", "recHit Timing", tmp_id, 200, -2, 2, "recHitsByChamber");
                histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", tmp_id, 10, -0.5, 9.5, "recHits");
                histos_->fill1DHistByChamber(twire_corr, "hAnodeTiming", "anode Timing", tmp_id, 200, -25, 25, "recHitsByChamber");
              }
              else
              {
                histos_->fill1DHistByChamber(rhtime_corr, "hRHTiming", "recHit Timing", id, 200, -2, 2, "recHitsByChamber");
                histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", id, 10, -0.5, 9.5, "recHits");
                histos_->fill1DHistByChamber(twire_corr, "hAnodeTiming", "anode Timing", id, 200, -25, 25, "recHitsByChamber");
              }
            }
            if (makePlotsPerLayer_)
            {
              histos_->fill1DHistByLayer(rhtime_corr, "hRHTiming", "recHit Timing", id, 200, -2, 2, "recHitsByLayer");
              histos_->fill1DHistByType(nstrips, "hRHNumStrips", "Number of cathod strips", id, 10, -0.5, 9.5, "recHits");
            }
          }

          if (writeTimingStudyBabyTree_)
          {
            b_rhtime = rhtime_corr ;
            babyTree->Fill();
          }

        } // loop over rechits

        int ring_ = ring;
        if (combineME11ab_ && station == 1 && ring == 4) ring_ = 1;
        CSCDetId segid(endcap, station, ring_, chamber, layer);                                    
        histos_->fill1DHistByType(segmentTime, "hSegTime", "CSC segment time", segid, 200, -100., 100., "Segments");

        double tseg = calculateSegmentTime(rhTimes_, wireTimes_);
        histos_->fill1DHistByType(tseg, "hNewSegTime", "new CSC segment time", segid, 200, -100., 100., "Segments");

        // for (auto wt : wireTimes_) {
        //   histos_->fill1DHistByChamber(wt, "hwireTimes", "Anode Timing after tail cut", id, 200, -10, 10, "recHitsByChamber");
        //   histos_->fill1DHistByType(wt, "hwireTimes", "Anode Timing", id, 200, -10, 10, "recHits");
        // }

        if (makePlotsPerChamber_)
        {
          histos_->fill1DHistByChamber(segmentTime, "hSegTime", "CSC segment time", segid, 200, -100., 100., "SegmentsByChamber");                
          histos_->fill1DHistByChamber(tseg, "hNewSegTime", "new CSC segment time", segid, 200, -100., 100., "SegmentsByChamber");
        }
      } // loop over segments
    } // loop over chambers


    if (isGoodMuWithArbitratedSegment)
    {
      histos_->fill1DHist(is_tracker                            , "hMuIsTracker"            , "muon quality" , 4   , -1.5 , 2.5 , "Muons");
      histos_->fill1DHist(is_sa                                 , "hMuIsSA"                 , "muon quality" , 4   , -1.5 , 2.5 , "Muons");
      histos_->fill1DHist(is_pf                                 , "hMuIsPF"                 , "muon quality" , 4   , -1.5 , 2.5 , "Muons");
      histos_->fill1DHist(std::max(maxOpeningAngleTrack ,2.501) , "hMuMaxOpeningAngleTrack" , "muon quality" , 70  , 2.5  , 3.2 , "Muons");
      histos_->fill1DHist(std::max(maxOpeningAngleMuon  ,0.001) , "hMuMaxOpeningAngleMuon"  , "muon quality" , 160 , 0    , 3.2 , "Muons");
      histos_->fill1DHist(numMatchedCSCsegments                 , "hMuNumMatchedSegments"   , "muon quality" , 10  , -0.5 , 9.5 , "Muons");
      histos_->fill1DHist(p4.eta()                              , "hMuEta"                  , "muon quality" , 50  , -2.5 , 2.5 , "Muons");
      histos_->fill1DHist(p4.phi()                              , "hMuPhi"                  , "muon quality" , 64  , -3.2 , 3.2 , "Muons");
      histos_->fill1DHist(time                                  , "hMuTime"                 , "muon quality" , 200 , -100., 100., "Muons");
      if (direction < 0)
        histos_->fill1DHist(time                                , "hMuTimeN"                , "muon quality" , 200 , -100., 100., "Muons");
      else
        histos_->fill1DHist(time                                , "hMuTimeP"                , "muon quality" , 200 , -100., 100., "Muons");        

      // histos_->fill1DHist(is_global             , "hMuIsGlobal"             , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(dz                    , "hMuDz"                   , "muon quality" , 300, -30 , 30 , "Muons");
      // histos_->fill1DHist(d0                    , "hMuD0"                   , "muon quality" , 100 , -5 , 5 , "Muons");
      // histos_->fill1DHist(gfit_chi2/gfit_ndof   , "hMuIsGfitNormChi2"       , "muon quality" , 100 , 0 , 25 , "Muons");
      // histos_->fill1DHist(type                  , "hMuType"                 , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(valid_siLayers        , "hMuSiLayers"             , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(valid_pixelHits       , "hMuPixelHits"            , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(charge                , "hMuCharge"               , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(trk_charge            , "hMuTrkCharge"            , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(gfit_validSTAhits     , "hMuGfitSAhits"           , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(gfit_validSiHits      , "hMuIsGfitSiHits"         , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(sta_validHits         , "hMuSAhits"               , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(numMatchedStations    , "hMuNumMatchedStations"   , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
      // histos_->fill1DHist(pt                    , "hMuPt"                   , "muon quality" , 4 , -1.5 , 2.5 , "Muons");
    }

  } // loop over muons

    // histos_->fill1DHist(nRecHits, "hRHnrechits", "recHits per event (all chambers)", 151, -0.5, 150.5, "recHits");    
}

void CSCTimingAnalyzer::fillMaps (CSCDetId id)
{
  m_cfebCableDelay[id]    = cfebCableDelay;
  m_cfebCorr[id]          = cfebCorr;
  m_chamberCorr[id]       = chamberCorr;
  m_skewClearDelay[id]    = skewClearDelay;
  m_cfeb_cable_delay[id]  = cfeb_cable_delay;
  m_cfeb_cable_length[id] = cfeb_cable_length;
  m_cfeb_skew_delay[id]   = cfeb_skew_delay;
  m_anode_bx_offset[id]   = anode_bx_offset;
  m_cfeb_cable_rev[id]    = cfeb_cable_rev;
    
  m_new_cfebCableDelay[id]    = new_cfebCableDelay;
  m_new_cfebCorr[id]          = new_cfebCorr;
  m_new_chamberCorr[id]       = new_chamberCorr;
  m_new_skewClearDelay[id]    = new_skewClearDelay;
  m_new_cfeb_cable_delay[id]  = new_cfeb_cable_delay;
  m_new_cfeb_cable_length[id] = new_cfeb_cable_length;
  m_new_cfeb_skew_delay[id]   = new_cfeb_skew_delay;
  m_new_cfeb_cable_rev[id]    = new_cfeb_cable_rev;
    
  m_chipCorr[id].insert(chipCorr);
  m_new_chipCorr[id].insert(new_chipCorr);
}

void CSCTimingAnalyzer::fill1DHistFromMapByType (std::map<CSCDetId, double>& m, std::string name, std::string title,
                                                 int binsx, float xmin, float xmax, std::string folder)
{
  for (const auto& item : m)
    histos_->fill1DHistByType(item.second, name, title, item.first, binsx, xmin, xmax, folder);
  return;
}


void CSCTimingAnalyzer::fill1DHistFromMapByType (std::map<CSCDetId, int>& m, std::string name, std::string title,
                                                 int binsx, float xmin, float xmax, std::string folder)
{
  for (const auto& item : m)
    histos_->fill1DHistByType(item.second, name, title, item.first, binsx, xmin, xmax, folder);
  return;    
}

void CSCTimingAnalyzer::fill1DHistFromMapByType (std::map<CSCDetId, std::set<double> >& m, std::string name, std::string title,
                                                 int binsx, float xmin, float xmax, std::string folder)
{
  for (const auto& item : m)
    for (const auto& val : item.second)
      histos_->fill1DHistByType(val, name, title, item.first, binsx, xmin, xmax, folder);
  return;    
}

void CSCTimingAnalyzer::fill1DHistFromMapByChamber (std::map<CSCDetId, double>& m, std::string name, std::string title,
                                                    int binsx, float xmin, float xmax, std::string folder)
{
  for (const auto& item : m)
    histos_->fill1DHistByChamber(item.second, name, title, item.first, binsx, xmin, xmax, folder);
  return;
}


void CSCTimingAnalyzer::fill1DHistFromMapByChamber (std::map<CSCDetId, int>& m, std::string name, std::string title,
                                                    int binsx, float xmin, float xmax, std::string folder)
{
  for (const auto& item : m)
    histos_->fill1DHistByChamber(item.second, name, title, item.first, binsx, xmin, xmax, folder);
  return;    
}

void CSCTimingAnalyzer::fill1DHistFromMapByChamber (std::map<CSCDetId, std::set<double> >& m, std::string name, std::string title,
                                                    int binsx, float xmin, float xmax, std::string folder)
{
  for (const auto& item : m)
    for (const auto& val : item.second)
      histos_->fill1DHistByChamber(val, name, title, item.first, binsx, xmin, xmax, folder);
  return;    
}

void CSCTimingAnalyzer::fill1DdiffHistFromMapsByType (std::map<CSCDetId, double>& m1, std::map<CSCDetId, double>& m2, std::string name, std::string title,
                                                      int binsx, float xmin, float xmax, std::string folder)
{
  std::map<CSCDetId, double> diff_map;    
  for (const auto& item: m1)
    diff_map[item.first] = item.second - m2[item.first];

  float minx = xmin;
  float maxx = xmax;
  if (abs(xmin-xmax) < 0.01)
  {
    minx = getMinHistValFromMap(diff_map);
    maxx = getMaxHistValFromMap(diff_map);
  }
    
  for (const auto& item: diff_map)
    histos_->fill1DHistByType(item.second, name, title, item.first, binsx, minx, maxx, folder);
  return;
}

void CSCTimingAnalyzer::fill1DdiffHistFromMapsByType (std::map<CSCDetId, int>& m1, std::map<CSCDetId, int>& m2, std::string name, std::string title,
                                                      int binsx, float xmin, float xmax, std::string folder)
{
  std::map<CSCDetId, int> diff_map;    
  for (const auto& item: m1)
    diff_map[item.first] = item.second - m2[item.first];

  float minx = xmin;
  float maxx = xmax;
  if (abs(xmin-xmax) < 0.01)
  {
    minx = getMinHistValFromMap(diff_map);
    maxx = getMaxHistValFromMap(diff_map);
  }
    
  for (const auto& item: diff_map)
    histos_->fill1DHistByType(item.second, name, title, item.first, binsx, minx, maxx, folder);
  return;
}    

void CSCTimingAnalyzer::fillHistosFromMaps ()
{    
  return;
}

double CSCTimingAnalyzer::getMinHistValFromMap (const std::map<CSCDetId, double>& m)
{
  double min_val = std::min_element(m.begin(), m.end(), &CSCTimingAnalyzer::compareMapValuesD)->second;
  return (min_val - 0.1*abs(min_val));
}

double CSCTimingAnalyzer::getMaxHistValFromMap (const std::map<CSCDetId, double>& m)
{
  double max_val = std::max_element(m.begin(), m.end(), &CSCTimingAnalyzer::compareMapValuesD)->second;
  return (max_val + 0.1*abs(max_val));
}

int CSCTimingAnalyzer::getMinHistValFromMap (const std::map<CSCDetId, int>& m)
{
  int min_val = std::min_element(m.begin(), m.end(), &CSCTimingAnalyzer::compareMapValuesI)->second;
  return (min_val - (int)(0.1*abs(min_val)));
}

int CSCTimingAnalyzer::getMaxHistValFromMap (const std::map<CSCDetId, int>& m)
{
  int max_val = std::max_element(m.begin(), m.end(), &CSCTimingAnalyzer::compareMapValuesI)->second;
  return (max_val + (int)(0.1*abs(max_val)));
}

double CSCTimingAnalyzer::getMinHistValFromMap (const std::map<CSCDetId, std::set<double> >& m)
{
  double min_val = *std::min_element(m.begin(), m.end(), &CSCTimingAnalyzer::compareMapValuesSD)->second.rbegin();
  return (min_val - (int)(0.1*abs(min_val)));
}

double CSCTimingAnalyzer::getMaxHistValFromMap (const std::map<CSCDetId, std::set<double> >& m)
{
  double max_val = *std::max_element(m.begin(), m.end(), &CSCTimingAnalyzer::compareMapValuesSD)->second.rbegin();
  return (max_val + (int)(0.1*abs(max_val)));
}

void CSCTimingAnalyzer::printMapToFile (const std::map<CSCDetId, double>& m, std::string filename)
{
  std::ofstream outfile;
  outfile.open(filename, std::ios::out);
  for (const auto item : m)
    outfile << item.first.endcap() << "\t" << item.first.station() << "\t" << item.first.ring() << "\t" << item.first.chamber() << "\t" << item.second << std::endl;
  outfile.close();
  return;
}

void CSCTimingAnalyzer::printMapToFile (const std::map<CSCDetId, int>& m, std::string filename)
{
  std::ofstream outfile;
  outfile.open(filename, std::ios::out);
  for (const auto item : m)
    outfile << item.first.endcap() << "\t" << item.first.station() << "\t" << item.first.ring() << "\t" << item.first.chamber() << "\t" << item.second << std::endl;
  outfile.close();
  return;
}

void CSCTimingAnalyzer::printMapToFile (const std::map<CSCDetId, std::set<double> >& m, std::string filename)
{
  std::ofstream outfile;
  outfile.open(filename, std::ios::out);
  for (const auto item : m)
  {
    outfile << item.first.endcap() << "\t" << item.first.station() << "\t" << item.first.ring() << "\t" << item.first.chamber();
    for (const auto val : item.second)
      outfile << "\t" << val;
    outfile << std::endl;
  }
  outfile.close();
  return;    
}

void CSCTimingAnalyzer::setTimingParamBabyBranches (TTree* tree)
{
  tree->Branch("cfebCableDelay"        , &b_cfebCableDelay        );
  tree->Branch("chipCorr"              , &b_chipCorr              );
  tree->Branch("cfebCorr"              , &b_cfebCorr              );
  tree->Branch("chamberCorr"           , &b_chamberCorr           );
  tree->Branch("new_cfebCableDelay"    , &b_new_cfebCableDelay    );
  tree->Branch("new_chipCorr"          , &b_new_chipCorr          );
  tree->Branch("new_cfebCorr"          , &b_new_cfebCorr          );
  tree->Branch("new_chamberCorr"       , &b_new_chamberCorr       );
  tree->Branch("new_skewClearDelay"    , &b_new_skewClearDelay    );
  tree->Branch("skewClearDelay"        , &b_skewClearDelay        );
  tree->Branch("cfeb_cable_delay"      , &b_cfeb_cable_delay      );
  tree->Branch("cfeb_cable_length"     , &b_cfeb_cable_length     );
  tree->Branch("cfeb_skew_delay"       , &b_cfeb_skew_delay       );
  tree->Branch("anode_bx_offset"       , &b_anode_bx_offset       );
  tree->Branch("chamber"               , &b_chamber               );
  tree->Branch("endcap"                , &b_endcap                );
  tree->Branch("new_cfeb_cable_delay"  , &b_new_cfeb_cable_delay  );
  tree->Branch("new_cfeb_cable_length" , &b_new_cfeb_cable_length );
  tree->Branch("new_cfeb_skew_delay"   , &b_new_cfeb_skew_delay   );
  tree->Branch("new_heuristicCorr"     , &b_new_heuristicCorr     );
  tree->Branch("ring"                  , &b_ring                  );
  tree->Branch("station"               , &b_station               );
  tree->Branch("cfeb_cable_rev"        , &b_cfeb_cable_rev        );
  tree->Branch("new_cfeb_cable_rev"    , &b_new_cfeb_cable_rev    );    
  return;
}

void CSCTimingAnalyzer::setTimingParamValues (const CSCDetId& id)
{
  b_cfebCableDelay        = m_cfebCableDelay[id];
  b_cfebCorr              = m_cfebCorr[id];
  b_chamberCorr           = m_chamberCorr[id];
  b_new_cfebCableDelay    = m_new_cfebCableDelay[id];
  b_new_cfebCorr          = m_new_cfebCorr[id];
  b_new_chamberCorr       = m_new_chamberCorr[id];
  b_new_skewClearDelay    = m_new_skewClearDelay[id];
  b_skewClearDelay        = m_skewClearDelay[id];
  b_cfeb_cable_delay      = m_cfeb_cable_delay[id];
  b_cfeb_cable_length     = m_cfeb_cable_length[id];
  b_cfeb_skew_delay       = m_cfeb_skew_delay[id];
  b_anode_bx_offset       = m_anode_bx_offset[id];
  b_new_cfeb_cable_delay  = m_new_cfeb_cable_delay[id];
  b_new_cfeb_cable_length = m_new_cfeb_cable_length[id];
  b_new_cfeb_skew_delay   = m_new_cfeb_skew_delay[id];
  b_new_heuristicCorr     = (m_new_heuristicCorr.find(id) != m_new_heuristicCorr.end()) ? m_new_heuristicCorr[id] : (station == 1 && (ring == 1 || ring == 4)) ? CSCTimingAnalyzer::ME11_AVG_HEURISTIC_CORR : CSCTimingAnalyzer::NON_ME11_AVG_HEURISTIC_CORR;

  b_cfeb_cable_rev = m_cfeb_cable_rev[id];
  b_new_cfeb_cable_rev = m_new_cfeb_cable_rev[id];
    
  b_chipCorr.clear();
  std::copy(m_chipCorr[id].begin(), m_chipCorr[id].end(), std::back_inserter(b_chipCorr));    

  b_new_chipCorr.clear();
  std::copy(m_new_chipCorr[id].begin(), m_new_chipCorr[id].end(), std::back_inserter(b_new_chipCorr));    

  b_endcap  = id.endcap();
  b_station = id.station();
  b_ring    = id.ring();
  b_chamber = id.chamber();

  return;
}

void CSCTimingAnalyzer::setTimingStudyBabyBranches (TTree* tree)
{
  tree->Branch("chamber"                     , &chamber                     );
  tree->Branch("endcap"                      , &endcap                      );
  tree->Branch("ring"                        , &ring                        );
  tree->Branch("station"                     , &station                     );
  tree->Branch("run"                         , &run                         );
  tree->Branch("lumi"                        , &lumi                        );
  tree->Branch("evt"                         , &evt                         );
  tree->Branch("p4"                          , &p4                          );
  tree->Branch("dz"                          , &dz                          );
  tree->Branch("d0"                          , &d0                          );
  tree->Branch("dz_bs"                       , &dz_bs                       );
  tree->Branch("d0_bs"                       , &d0_bs                       );
  tree->Branch("maxOpeningAngleTrack"        , &maxOpeningAngleTrack        );
  tree->Branch("maxOpeningAngleMuon"         , &maxOpeningAngleMuon         );
  tree->Branch("distToChamberEdge"           , &distToChamberEdge           );
  tree->Branch("distToChamberEdgeErr"        , &distToChamberEdgeErr        );
  tree->Branch("numCSCsegmentsInChamber"     , &numCSCsegmentsInChamber     );
  tree->Branch("arbitration_mask"            , &arbitration_mask            );
  tree->Branch("isSegmentAndTrackArbitrated" , &isSegmentAndTrackArbitrated );
  tree->Branch("layer"                       , &layer                       );
  tree->Branch("rhtime"                      , &b_rhtime                    );
  tree->Branch("twire"                       , &twire                       );
  tree->Branch("nstrips"                     , &nstrips                     );
  return;
}

// ------------ method called once each job just before starting event loop  ------------
void 
CSCTimingAnalyzer::beginJob()
{
  if (applyNewHeuristicCorrectionByChamber_ || applyNewHeuristicCorrectionByRing_)
    std::cout << "Reading heuristic corrections from file..." << std::endl;
  if (!CSCTimingAnalyzer::readHeuristicCorrectionsFromFile())
    std::cout << "Failed to read heuristic corrections from file." << std::endl;
  if (applyNewHeuristicCorrectionByChamber_ || applyNewHeuristicCorrectionByRing_)
    std::cout << "Done reading heuristic corrections from file..." << std::endl;
  if (writeTimingStudyBabyTree_)
    CSCTimingAnalyzer::setTimingStudyBabyBranches(babyTree);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CSCTimingAnalyzer::endJob() 
{
  if (makeChamberTimingCorrectionValueHists_)
    fillHistosFromMaps();

  if (printTimeCorrectionParametersToFile_)
  {
    printMapToFile(m_cfebCableDelay    , "cfebCableDelay.txt"    );
    printMapToFile(m_cfebCorr          , "cfebCorr.txt"          );
    printMapToFile(m_chipCorr          , "chipCorr.txt"          );
    printMapToFile(m_chamberCorr       , "chamberCorr.txt"       );
    printMapToFile(m_skewClearDelay    , "skewClearDelay.txt"    );
    printMapToFile(m_cfeb_cable_delay  , "cfeb_cable_delay.txt"  );
    printMapToFile(m_cfeb_cable_length , "cfeb_cable_length.txt" );
    printMapToFile(m_cfeb_skew_delay   , "cfeb_skew_delay.txt"   );
    printMapToFile(m_anode_bx_offset   , "anode_bx_offset.txt"   );

    printMapToFile(m_new_cfebCableDelay    , "new_cfebCableDelay.txt"    );
    printMapToFile(m_new_chipCorr          , "new_chipCorr.txt"          );
    printMapToFile(m_new_cfebCorr          , "new_cfebCorr.txt"          );
    printMapToFile(m_new_chamberCorr       , "new_chamberCorr.txt"       );
    printMapToFile(m_new_skewClearDelay    , "new_skewClearDelay.txt"    );
    printMapToFile(m_new_cfeb_cable_delay  , "new_cfeb_cable_delay.txt"  );
    printMapToFile(m_new_cfeb_cable_length , "new_cfeb_cable_length.txt" );
    printMapToFile(m_new_cfeb_skew_delay   , "new_cfeb_skew_delay.txt"   );        
  }

  if (writeTimeCorrectionParametersToTree_)
  {
    TFile* outfile = new TFile(timeParamFileName_.c_str(), "RECREATE");
    TTree* outtree = new TTree("TimingParameters", "timing parameters tree");
    setTimingParamBabyBranches(outtree);
    for (const auto item : m_cfebCorr)
    {
      const CSCDetId& id = item.first;
      setTimingParamValues(id);
      outtree->Fill();
    }
    outfile->cd();
    outtree->Write();
    outfile->Close();
  }

  if (writeTimingStudyBabyTree_)
  {
    babyFile->cd();
    babyTree->Write();
    babyFile->Close();
  }
}

void CSCTimingAnalyzer::loadBranches (const edm::Event& event)
{
  event.getByToken(run_token                        , run_h                        );                        
  event.getByToken(lumi_token                       , lumi_h                       );                       
  event.getByToken(evt_token                        , evt_h                        );                        
  event.getByToken(bfield_token                     , bfield_h                     );                     
  event.getByToken(csc_status_token                 , csc_status_h                 );                 
  event.getByToken(detector_status_token            , detector_status_h            );            
  event.getByToken(is_global_token                  , is_global_h                  );                  
  event.getByToken(is_tracker_token                 , is_tracker_h                 );                 
  event.getByToken(is_sa_token                      , is_sa_h                      );                      
  event.getByToken(is_pf_token                      , is_pf_h                      );                      
  event.getByToken(p4_token                         , p4_h                         );                         
  event.getByToken(dz_token                         , dz_h                         );                         
  event.getByToken(d0_token                         , d0_h                         );                         
  event.getByToken(dz_bs_token                      , dz_bs_h                      );                         
  event.getByToken(d0_bs_token                      , d0_bs_h                      );                         
  event.getByToken(gfit_chi2_token                  , gfit_chi2_h                  );                  
  event.getByToken(gfit_ndof_token                  , gfit_ndof_h                  );                  
  event.getByToken(maxOpeningAngleTrack_token       , maxOpeningAngleTrack_h       );       
  event.getByToken(maxOpeningAngleMuon_token        , maxOpeningAngleMuon_h        );
  event.getByToken(time_token                       , time_h                       );
  event.getByToken(timeErr_token                    , timeErr_h                    );
  event.getByToken(type_token                       , type_h                       );                       
  event.getByToken(valid_siLayers_token             , valid_siLayers_h             );             
  event.getByToken(valid_pixelHits_token            , valid_pixelHits_h            );            
  event.getByToken(charge_token                     , charge_h                     );                     
  event.getByToken(trk_charge_token                 , trk_charge_h                 );                 
  event.getByToken(gfit_validSTAhits_token          , gfit_validSTAhits_h          );          
  event.getByToken(gfit_validSiHits_token           , gfit_validSiHits_h           );           
  event.getByToken(sta_validHits_token              , sta_validHits_h              );              
  event.getByToken(numMatchedStations_token         , numMatchedStations_h         );         
  event.getByToken(numMatchedCSCsegments_token      , numMatchedCSCsegments_h      );      
  event.getByToken(station_mask_token               , station_mask_h               );
  event.getByToken(direction_token                  , direction_h                  );
  event.getByToken(distToChamberEdge_token          , distToChamberEdge_h          );          
  event.getByToken(distToChamberEdgeErr_token       , distToChamberEdgeErr_h       );       
  event.getByToken(numCSCsegmentsInChamber_token    , numCSCsegmentsInChamber_h    );    
  event.getByToken(arbitration_mask_token           , arbitration_mask_h           );           
  event.getByToken(isSegmentAndTrackArbitrated_token, isSegmentAndTrackArbitrated_h);
  event.getByToken(numRecHitsInSegment_token        , numRecHitsInSegment_h        );
  event.getByToken(segmentTime_token                , segmentTime_h                );
  event.getByToken(endcap_token                     , endcap_h                     );                     
  event.getByToken(station_token                    , station_h                    );                    
  event.getByToken(ring_token                       , ring_h                       );                       
  event.getByToken(chamber_token                    , chamber_h                    );                    
  event.getByToken(layer_token                      , layer_h                      );                      
  event.getByToken(nstrips_token                    , nstrips_h                    );
  event.getByToken(rhtime_token                     , rhtime_h                     );                     
  event.getByToken(twire_token                      , twire_h                      );                      
  event.getByToken(cfeb_cable_length_token          , cfeb_cable_length_h          );          
  event.getByToken(cfeb_skew_delay_token            , cfeb_skew_delay_h            );            
  event.getByToken(cfeb_timing_corr_token           , cfeb_timing_corr_h           );           
  event.getByToken(cfeb_cable_delay_token           , cfeb_cable_delay_h           );           
  event.getByToken(precision_token                  , precision_h                  );                  
  event.getByToken(cfeb_cable_rev_token             , cfeb_cable_rev_h             );             
  event.getByToken(anode_bx_offset_token            , anode_bx_offset_h            );            
  event.getByToken(chipCorr_token                   , cfebCorr_h                   );
  event.getByToken(cfebCorr_token                   , chipCorr_h                   );
  event.getByToken(skewClearDelay_token             , skewClearDelay_h             );             
  event.getByToken(cfebCableDelay_token             , cfebCableDelay_h             );             
  event.getByToken(chamberCorr_token                , chamberCorr_h                );                
  event.getByToken(new_skewClearDelay_token         , new_skewClearDelay_h         );         
  event.getByToken(new_cfebCableDelay_token         , new_cfebCableDelay_h         );         
  event.getByToken(new_chamberCorr_token            , new_chamberCorr_h            );            
  event.getByToken(new_cfeb_cable_length_token      , new_cfeb_cable_length_h      );      
  event.getByToken(new_cfeb_cable_rev_token         , new_cfeb_cable_rev_h         );         
  event.getByToken(new_cfeb_skew_delay_token        , new_cfeb_skew_delay_h        );        
  event.getByToken(new_cfeb_cable_delay_token       , new_cfeb_cable_delay_h       );       
  event.getByToken(new_cfebCorr_token               , new_cfebCorr_h               );
  event.getByToken(new_chipCorr_token               , new_chipCorr_h               );
  event.getByToken(hlt_bits_token                   , hlt_bits_h                   );
  event.getByToken(hlt_trigNames_token              , hlt_trigNames_h              );
  event.getByToken(hlt_prescales_token              , hlt_prescales_h              );
  event.getByToken(hlt_trigObjs_id_token            , hlt_trigObjs_id_h            );
  event.getByToken(hlt_trigObjs_p4_token            , hlt_trigObjs_p4_h            );    
  return;
}

void CSCTimingAnalyzer::getVars (size_t muon_index, size_t chamber_index, size_t segment_index, size_t rechit_index)
{
  initVars();

  //
  // event level variables
  //
  run                         = *run_h;
  lumi                        = *lumi_h;
  evt                         = *evt_h;
  bfield                      = *bfield_h ;
  csc_status                  = *csc_status_h;
  detector_status             = *detector_status_h;

  //
  // muon level variables
  //
  if (debug_)
    printf("requesting muon %zd of event %llu which has %zd muons\n", muon_index, evt, is_global_h->size());
  if (muon_index < is_global_h->size())
  {
    is_global                   = is_global_h->at(muon_index)  ;
    is_tracker                  = is_tracker_h->at(muon_index) ;
    is_sa                       = is_sa_h->at(muon_index)      ;
    is_pf                       = is_pf_h->at(muon_index)      ;
    p4                          = p4_h->at(muon_index) ;
    dz                          = dz_h->at(muon_index) ;
    d0                          = d0_h->at(muon_index) ;
    dz_bs                       = dz_h->at(muon_index) ;
    d0_bs                       = d0_h->at(muon_index) ;
    gfit_chi2                   = gfit_chi2_h->at(muon_index) ;
    gfit_ndof                   = gfit_ndof_h->at(muon_index) ;
    maxOpeningAngleTrack        = maxOpeningAngleTrack_h->at(muon_index) ;
    maxOpeningAngleMuon         = maxOpeningAngleMuon_h->at(muon_index) ;
    type                        = type_h->at(muon_index) ;
    valid_siLayers              = valid_siLayers_h->at(muon_index) ;
    valid_pixelHits             = valid_pixelHits_h->at(muon_index) ;
    charge                      = charge_h->at(muon_index) ;
    trk_charge                  = trk_charge_h->at(muon_index) ;
    gfit_validSTAhits           = gfit_validSTAhits_h->at(muon_index) ;
    gfit_validSiHits            = gfit_validSiHits_h->at(muon_index) ;
    sta_validHits               = sta_validHits_h->at(muon_index) ;
    numMatchedStations          = numMatchedStations_h->at(muon_index) ;
    numMatchedCSCsegments       = numMatchedCSCsegments_h->at(muon_index) ;
    station_mask                = station_mask_h->at(muon_index) ;
    time                        = time_h->at(muon_index);
    timeErr                     = timeErr_h->at(muon_index);
    direction                   = direction_h->at(muon_index);

    if (debug_)
      printf("requesting chamber %zd of muon %zd which has %zd chambers\n", chamber_index, muon_index, numCSCsegmentsInChamber_h->at(muon_index).size());
    if (chamber_index < numCSCsegmentsInChamber_h->at(muon_index).size())
    {
      distToChamberEdge           = distToChamberEdge_h->at(muon_index).at(chamber_index) ;
      distToChamberEdgeErr        = distToChamberEdgeErr_h->at(muon_index).at(chamber_index) ;
      numCSCsegmentsInChamber     = numCSCsegmentsInChamber_h->at(muon_index).at(chamber_index) ;

      if (debug_)
        printf("requesting segment %zd of muon %zd, chamber %zd which has %zd segments\n", segment_index, muon_index, chamber_index, numRecHitsInSegment_h->at(muon_index).at(chamber_index).size());
      if (segment_index < numRecHitsInSegment_h->at(muon_index).at(chamber_index).size())
      {
        arbitration_mask            = arbitration_mask_h->at(muon_index).at(chamber_index).at(segment_index) ;
        isSegmentAndTrackArbitrated = isSegmentAndTrackArbitrated_h->at(muon_index).at(chamber_index).at(segment_index) ;
        numRecHitsInSegment         = numRecHitsInSegment_h->at(muon_index).at(chamber_index).at(segment_index) ;
        segmentTime                 = segmentTime_h->at(muon_index).at(chamber_index).at(segment_index);

        if (debug_)
          printf("requesting rechit %zd of muon %zd, chamber %zd, segment %zd which has %zd rechits\n", rechit_index, muon_index, chamber_index, segment_index, endcap_h->at(muon_index).at(chamber_index).at(segment_index).size());
        if (rechit_index < endcap_h->at(muon_index).at(chamber_index).at(segment_index).size())
        {
          endcap                      = endcap_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)                ;
          station                     = station_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)               ;
          ring                        = ring_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)                  ;
          chamber                     = chamber_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)               ;
          layer                       = layer_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)                 ;
          nstrips                     = nstrips_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)               ;
          rhtime                      = rhtime_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)                ;
          twire                       = twire_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)                 ;
          cfeb_cable_length           = cfeb_cable_length_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)     ;
          cfeb_skew_delay             = cfeb_skew_delay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)       ;
          cfeb_timing_corr            = cfeb_timing_corr_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)      ;
          cfeb_cable_delay            = cfeb_cable_delay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)      ;
          precision                   = precision_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)             ;
          cfeb_cable_rev              = cfeb_cable_rev_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)        ;
          anode_bx_offset             = anode_bx_offset_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)       ;
          chipCorr                    = chipCorr_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)              ;
          cfebCorr                    = cfebCorr_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)              ;
          skewClearDelay              = skewClearDelay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)        ;
          cfebCableDelay              = cfebCableDelay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)        ;
          chamberCorr                 = chamberCorr_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)           ;
          new_skewClearDelay          = new_skewClearDelay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)    ;
          new_cfebCableDelay          = new_cfebCableDelay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)    ;
          new_chamberCorr             = new_chamberCorr_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)       ;
          new_cfeb_cable_length       = new_cfeb_cable_length_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index) ;
          new_cfeb_cable_rev          = new_cfeb_cable_rev_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)    ;
          new_cfeb_skew_delay         = new_cfeb_skew_delay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)   ;
          new_cfeb_cable_delay        = new_cfeb_cable_delay_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)  ;
          new_cfebCorr                = new_cfebCorr_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)          ;
          new_chipCorr                = new_chipCorr_h->at(muon_index).at(chamber_index).at(segment_index).at(rechit_index)          ;
        }
      }
    }
  }

  return;
}

void CSCTimingAnalyzer::initVars ()
{
  run                         = 0;
  lumi                        = 0;
  evt                         = 0;
  bfield                      = -999 ;
  csc_status                  = false;
  detector_status             = 0;
  is_global                   = false;
  is_tracker                  = false;
  is_sa                       = false ;
  is_pf                       = false;
  p4                         = LorentzVector(0,0,0,0);
  dz                          = -999;
  d0                          = -999;
  dz_bs                       = -999;
  d0_bs                       = -999;
  gfit_chi2                   = -999;
  gfit_ndof                   = -1;
  maxOpeningAngleTrack        = -999;
  maxOpeningAngleMuon         = -999;
  time                        = -999;
  timeErr                     = -999;
  type                        = 0;
  valid_siLayers              = 0;
  valid_pixelHits             = 0;
  charge                      = 0;
  trk_charge                  = 0;
  gfit_validSTAhits           = 0;
  gfit_validSiHits            = 0;
  sta_validHits               = 0;
  numMatchedStations          = 0;
  numMatchedCSCsegments       = 0;
  station_mask                = 0;
  direction                   = 0;
  distToChamberEdge           = -999;
  distToChamberEdgeErr        = -999;
  numCSCsegmentsInChamber     = 0;
  arbitration_mask            = 0;
  isSegmentAndTrackArbitrated = false;
  numRecHitsInSegment         = 0;
  endcap                      = 0;
  station                     = 0;
  ring                        = 0;
  chamber                     = 0;
  layer                       = 0;
  nstrips                     = 0;
  cfeb_cable_length           = 0;
  cfeb_skew_delay             = 0;
  cfeb_timing_corr            = 0;
  cfeb_cable_delay            = 0;
  precision                   = 0;
  new_cfeb_cable_length       = 0;
  new_cfeb_skew_delay         = 0;
  new_cfeb_cable_delay        = 0;
  rhtime                      = -999;
  twire                       = -999;
  anode_bx_offset             = -999;
  chipCorr                    = -999;
  cfebCorr                    = -999;
  skewClearDelay              = -999;
  cfebCableDelay              = -999;
  chamberCorr                 = -999;
  new_skewClearDelay          = -999;
  new_cfebCableDelay          = -999;
  new_chamberCorr             = -999;
  new_cfebCorr                = -999;
  new_chipCorr                = -999;

  hlt_trigNames.clear();
  hlt_prescales.clear();
  hlt_trigObjs_id.clear();
  hlt_trigObjs_p4.clear();
  hlt_bits.Clear();
  return;
}

bool CSCTimingAnalyzer::passesGlobalChi2 ()
{
  return (gfit_chi2/gfit_ndof < 10.);
}

bool CSCTimingAnalyzer::passesGlobalSAhits ()
{
  return (gfit_validSTAhits > 0);
}

bool CSCTimingAnalyzer::passesNumStations ()
{
  return (numMatchedStations > 1);
}

bool CSCTimingAnalyzer::passesD0 ()
{
  return (fabs(d0_bs) <= 0.2);
}

bool CSCTimingAnalyzer::passesDZ ()
{
  return (fabs(dz_bs) <= max_dz_);
}

bool CSCTimingAnalyzer::passesPixelHits ()
{
  return (valid_pixelHits > 0);
}

bool CSCTimingAnalyzer::passesSiLayers ()
{
  return (valid_siLayers > 5);
}


bool CSCTimingAnalyzer::isPOGmuonTight ()
{
  if (!is_pf) return false;
  if (!isGlobalMuon()) return false;
  if (!passesGlobalChi2()) return false;
  if (!passesGlobalSAhits()) return false;
  if (!passesNumStations() && !removeMatchedStationsRequirement_) return false;
  if (!passesD0()) return false;
  if (!passesDZ()) return false;
  if (!passesPixelHits()) return false;
  if (!passesSiLayers()) return false;
  return true;
}

bool CSCTimingAnalyzer::readHeuristicCorrectionsFromFile ()
{
  if (applyNewHeuristicCorrectionByChamber_)
  {
    std::ifstream infile(fileForHeuristicCorrByChamber_.c_str());
    if (!infile)
    {
      std::cout << "Failed to read heuristic corrections from file " << fileForHeuristicCorrByChamber_ << std::endl;
      return false;
    }
    if (verbose_)
      std::cout << "Successfully opened file " << fileForHeuristicCorrByChamber_ << std::endl;
    bool is_first = true;
    while (!infile.eof())
    {
      if (is_first)
      {
        is_first = false;
        continue;
      }
      std::string blah;
      getline(infile, blah);
      int endcap_, station_, ring_, chamber_;
      double corr_;
      infile >> endcap_ >> station_ >> ring_ >> chamber_ >> corr_;
      if (debug_)
        std::cout << endcap_ << "\t" << station_ << "\t" << ring_ << "\t" << chamber_ << "\t" << corr_ << std::endl;
      CSCDetId id_(endcap_, station_, ring_, chamber_);
      m_new_heuristicCorr[id_] = corr_;
      if (combineME11ab_ && station_ == 1 && ring_ == 1)
        m_new_heuristicCorr[CSCDetId(endcap_, station_, 4, chamber_)] = corr_;
    }
    infile.close();

    if (verbose_)
    {
      printf("\nprinting new corrections read into map from file...\n\n");
      for (auto item : m_new_heuristicCorr)
        printf("e = %d, s = %d, r = %d, c = %d, corr = %4.2f\n", item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber(), item.second);
    }
        
    return true;
  }
  else if (applyNewHeuristicCorrectionByRing_)
  {
    std::ifstream infile(fileForHeuristicCorrByRing_.c_str());
    if (!infile)
    {
      std::cout << "Failed to read heuristic corrections from file " << fileForHeuristicCorrByRing_ << std::endl;
      return false;
    }
    if (verbose_)
      std::cout << "Successfully opened file " << fileForHeuristicCorrByRing_ << std::endl;
    bool is_first = true;
    while (!infile.eof())
    {
      if (is_first)
      {
        is_first = false;
        continue;
      }
      int endcap_, station_, ring_, chamber_ = 0;
      double corr_;
      std::string blah;
      getline(infile, blah);
      infile >> endcap_ >> station_ >> ring_ >> corr_;
      if (debug_)
        std::cout << endcap_ << "\t" << station_ << "\t" << ring_ << "\t" << chamber_ << "\t" << corr_ << std::endl;
      CSCDetId id_(endcap_, station_, ring_, chamber_);
      m_new_heuristicCorr[id_] = corr_;            
      if (combineME11ab_ && station_ == 1 && ring_ == 1)
        m_new_heuristicCorr[CSCDetId(endcap_, station_, 4, chamber_)] = corr_;
    }
    infile.close();
    return true;
  }
  return (!(applyNewHeuristicCorrectionByRing_ || applyNewHeuristicCorrectionByChamber_));
}

void CSCTimingAnalyzer::fillNm1hists ()
{
  // cuts for N-1 plotting
  enum Cuts {IS_GLOBAL = 0, CHI2, STA_HITS, NSTATIONS, D0, DZ, NPIXELS, NLAYERS, NCUTS};
  const unsigned int NBINS[] = {4, 50, 40, 6, 120, 300, 6, 20};
  const double BIN_EDGE_LOW[] {-1.5, 0, -0.5, -0.5, -3, -30, -0.5, -0.5};
  const double BIN_EDGE_HIGH[] = {2.5, 10, 39.5, 5.5, 3, 30, 5.5, 19.5};
  const std::string HIST_NAMES[] = {"isGlobal", "globalChi2", "validSAhits", "nMatchedStations", "d0", "dz", "pixelHits", "siLayers"};

  double cutParams[100];
  cutParams[Cuts::IS_GLOBAL] = is_global;
  cutParams[Cuts::CHI2]      = gfit_chi2/gfit_ndof;
  cutParams[Cuts::STA_HITS]  = gfit_validSTAhits;
  cutParams[Cuts::NSTATIONS] = numMatchedStations;
  cutParams[Cuts::D0]        = d0_bs;
  cutParams[Cuts::DZ]        = dz_bs;
  cutParams[Cuts::NPIXELS]   = valid_pixelHits;
  cutParams[Cuts::NLAYERS]   = valid_siLayers;

  int cutDecisions[100];
  cutDecisions[Cuts::IS_GLOBAL] = isGlobalMuon();      
  cutDecisions[Cuts::CHI2]      = passesGlobalChi2();  
  cutDecisions[Cuts::STA_HITS]  = passesGlobalSAhits();
  cutDecisions[Cuts::NSTATIONS] = passesNumStations(); 
  cutDecisions[Cuts::D0]        = passesD0();          
  cutDecisions[Cuts::DZ]        = passesDZ();          
  cutDecisions[Cuts::NPIXELS]   = passesPixelHits();   
  cutDecisions[Cuts::NLAYERS]   = passesSiLayers();    

  // build mask of cuts passed
  unsigned int cuts_passed = 0;
  for (size_t idx = 0; idx < Cuts::NCUTS; idx++)
  {
    cuts_passed |= (cutDecisions[idx] << idx);
  }

  for (size_t idx = 0; idx < Cuts::NCUTS; idx++)
  {        
    unsigned int cuts_to_pass = (~(1 << idx) & ((1<<Cuts::NCUTS)-1));
    if ((cuts_passed & cuts_to_pass) == cuts_to_pass)
    {
      histos_->fill1DHist(std::max(std::min(cutParams[idx],BIN_EDGE_HIGH[idx]-0.01),BIN_EDGE_LOW[idx]+0.01), HIST_NAMES[idx].c_str(), "muon quality", NBINS[idx], BIN_EDGE_LOW[idx], BIN_EDGE_HIGH[idx], "MuonNM1Hists");
    }
  }
}

double CSCTimingAnalyzer::calculateSegmentTime (std::vector<double>& rhts, std::vector<double>& wts)
{
  float averageTime=0;
  std::vector<float> wireTimes;
  for (unsigned int idx = 0; idx < rhts.size(); idx++)
  {
    averageTime += rhts.at(idx);
    averageTime += wts.at(idx);
  }
  averageTime=averageTime/(2*rhts.size());
 
  //The wire times have a long tail that has to be pruned.  The strip times (tpeak) are fine
  bool modified = true;
  while (modified)
  {
    modified = false;
    double maxDiff = -1;
    unsigned int maxHit = 0;
    for (unsigned int idx = 0; idx < wts.size(); idx++)
    {
      float diff = fabs(wts.at(idx) - averageTime);
      if (diff > maxDiff)
      {
        maxDiff = diff;
        maxHit = idx;
      }
    }
    if (maxDiff > 26)
    {
      int N=rhts.size()+wts.size();
      averageTime=(averageTime*N-(wts.at(maxHit)))/(N-1);
      wts.erase(wts.begin()+maxHit);
      modified = true;
    }
  }
  return averageTime;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CSCTimingAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCTimingAnalyzer);
