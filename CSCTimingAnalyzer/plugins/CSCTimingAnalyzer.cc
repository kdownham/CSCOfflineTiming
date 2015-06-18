#include "CSCOfflineTiming/CSCTimingAnalyzer/plugins/CSCTimingAnalyzer.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include <iostream>
#include <fstream>

CSCTimingAnalyzer::CSCTimingAnalyzer(const edm::ParameterSet& iConfig)
{
    makeChamberTimingCorrectionValueHists_ = iConfig.getUntrackedParameter<bool>("makeChamberTimingCorrectionValueHists" , false);
    makePlotsPerChamber_                   = iConfig.getUntrackedParameter<bool>("makePlotsPerChamber"                   , false);
    makePlotsPerLayer_                     = iConfig.getUntrackedParameter<bool>("makePlotsPerLayer"                     , false);
    printTimeCorrectionParametersToFile_   = iConfig.getUntrackedParameter<bool>("printTimeCorrectionParametersToFile"   , false);
    writeTimeCorrectionParametersToTree_   = iConfig.getUntrackedParameter<bool>("writeTimeCorrectionParametersToTree"   , false);
    verbose_                               = iConfig.getUntrackedParameter<bool>("verbose"                               , false);
    debug_                                 = iConfig.getUntrackedParameter<bool>("debug"                                 , false);
    outfileName_                           = iConfig.getUntrackedParameter<std::string>("outfileName", "histos.root");
    timeParamFileName_                     = iConfig.getUntrackedParameter<std::string>("timeParamFileName", "cscTimingParameters.root");

    histos_  = new CSCValHists();
    outfile_ = new TFile(outfileName_.c_str(), "RECREATE");
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

    getVars();
    if (verbose_)
        printf("\n\nrun, lumi, evt, csc_status: %d, %d, %llu, %d\n\n", run, lumi, evt, csc_status); 

    size_t nMuons = p4_h->size();
    for (size_t nmu = 0; nmu < nMuons; nmu++)
    {
        getVars(nmu);
        if (verbose_)
            printf("\n\tMuon %zd: eta = %4.2f, is_global = %d, numMatchedStations = %d, numMatchedCSCsegments = %d\n", nmu, p4.eta(), is_global, numMatchedStations, numMatchedCSCsegments);

        size_t nChambers = numCSCsegmentsInChamber_h->at(nmu).size();
        for (size_t nch = 0; nch < nChambers; nch++)
        {
            getVars(nmu, nch);
            if (verbose_)
                printf("\t\tchamber %zd: numCSCsegmentsInChamber = %d\n", nch, numCSCsegmentsInChamber);

            size_t nSegments = numRecHitsInSegment_h->at(nmu).at(nch).size();
            for (size_t nseg = 0; nseg < nSegments; nseg++)
            {
                getVars(nmu, nch, nseg);
                if (verbose_)
                    printf("\t\t\tsegment %zd: isSegmentAndTrackArbitrated = %d, numRecHitsInSegment = %d\n", nseg, isSegmentAndTrackArbitrated, numRecHitsInSegment);

                size_t nRecHits = station_h->at(nmu).at(nch).at(nseg).size();
                for (size_t nrh = 0; nrh < nRecHits; nrh++)
                {
                    getVars(nmu,nch,nseg,nrh);
                    if (verbose_)
                    {
                        printf("\t\t\t\trechit %zd: (endcap,station,ring,chamber) = (%d,%d,%d,%d) has (cathode,anode) time = (%4.2f,%4.2f)\n", nrh, endcap, station, ring, chamber, rhtime, twire);
                        printf("\t\t\t\t\tnstrips = %d, chipCorr = %4.2f, cfebDelay = %4.2f, skewClearDelay = %4.2f, heuristic = %4.2f\n", nstrips, chipCorr, cfebCableDelay, skewClearDelay, chamberCorr-cfebCableDelay-skewClearDelay);
                        printf("\t\t\t\t\tnstrips = %d, new_chipCorr = %4.2f, new_cfebDelay = %4.2f, new_skewClearDelay = %4.2f, heuristic = %4.2f\n", nstrips, new_chipCorr, new_cfebCableDelay, new_skewClearDelay, chamberCorr-cfebCableDelay-skewClearDelay);
                    }
                }
            }
        }
    }

/*    
    size_t nRecHits = station_h->size();
    for (size_t nrh = 0; nrh < nRecHits; nrh++)
    {
        getVars(nrh);

        // if (!status_) continue;

        if (onlyCruzetData_ && run_ > 238340) continue;
        if (onlyCraftData_ && run_ < 238361) continue;
        
        bool fill_chamber_maps = (makeChamberTimingCorrectionValueHists_ | printTimeCorrectionParametersToFile_ | writeTimeCorrectionParametersToTree_);
        CSCDetId detid(endcap_, station_, ring_, chamber_, fill_chamber_maps ? 1 : layer_);
        CSCDetId detid_with_layer(endcap_, station_, ring_, chamber_, layer_);

        if (fill_chamber_maps)
            fillMaps(detid);

        double rh_time = rhtime_;
        double rh_time_corr = 0.;
        if (station_ == 1 && (ring_ == 1 || ring_ == 4) && (run_ <= 237595 || run_ >= 238335))
        {
            if (useAvgChipSpeedCorrForME11_)
                rh_time_corr += (new_chipCorr_ - chipCorr_);
            if (flipPhaseForME11_ && run_ <= 237595)
                rh_time_corr += (new_phaseCorr_ - phaseCorr_);
            if (setCfebCableDelayToZeroForME11_)
                rh_time_corr += (new_cfebCableDelay_ - cfebCableDelay_);
            if (useNewSkewClearDelaysForME11_)
                rh_time_corr += (new_skewClearDelay_ - skewClearDelay_);
        }

        if (applyNoChipCorrections_)
            rh_time_corr -= chipCorr_;

        if (applyNoCfebCableDelay_)
            rh_time_corr -= cfebCableDelay_;

        if (applyNoSkewClearDelay_)
            rh_time_corr -= skewClearDelay_;
        
        rh_time += rh_time_corr;
        rh_time /= 50.;

        // fill histogram
        histos_->fill1DHistByType(rh_time, "hRHTiming", "recHit Timing", detid, 200, -10, 10, "recHits");
        histos_->fill1DHistByType(tofCorr_, "hTOF", "muon TOF", detid, 300, 15, 45, "tofValues");
        histos_->fill1DHistByType(yCorr_, "hYCorr", "pulse propagation delay", detid, 100, -2, 2, "yCorrValues");
        if (makePlotsPerChamber_)
        {
            histos_->fill1DHistByChamber(rh_time, "hRHTiming", "recHit Timing", detid, 200, -10, 10, "recHitsByChamber");
            histos_->fill1DHistByChamber(tofCorr_, "hTOF", "muon TOF", detid, 300, 15, 45, "tofValuesByChamber");
            histos_->fill1DHistByChamber(yCorr_, "hYCorr", "pulse propagation delay", detid, 100, -2, 2, "yCorrValuesByChamber");
        }
        if (makePlotsPerLayer_)
        {
            histos_->fill1DHistByLayer(rh_time, "hRHTiming", "recHit Timing", detid_with_layer, 200, -10, 10, "recHitsByLayer");
            histos_->fill1DHistByLayer(tofCorr_, "hTOF", "muon TOF", detid_with_layer, 300, 15, 45, "tofValuesByLayer");
            histos_->fill1DHistByLayer(yCorr_, "hYCorr", "pulse propagation delay", detid_with_layer, 100, -2, 2, "yCorrValuesByLayer");
        }
    }

    histos_->fill1DHist(nRecHits, "hRHnrechits", "recHits per event (all chambers)", 151, -0.5, 150.5, "recHits");  
*/  
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
    
    m_new_cfebCableDelay[id]    = new_cfebCableDelay;
    m_new_cfebCorr[id]          = new_cfebCorr;
    m_new_chamberCorr[id]       = new_chamberCorr;
    m_new_skewClearDelay[id]    = new_skewClearDelay;
    m_new_cfeb_cable_delay[id]  = new_cfeb_cable_delay;
    m_new_cfeb_cable_length[id] = new_cfeb_cable_length;
    m_new_cfeb_skew_delay[id]   = new_cfeb_skew_delay;

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
    tree->Branch("chamber"               , &b_chamber               );
    tree->Branch("endcap"                , &b_endcap                );
    tree->Branch("new_cfeb_cable_delay"  , &b_new_cfeb_cable_delay  );
    tree->Branch("new_cfeb_cable_length" , &b_new_cfeb_cable_length );
    tree->Branch("new_cfeb_skew_delay"   , &b_new_cfeb_skew_delay   );
    tree->Branch("ring"                  , &b_ring                  );
    tree->Branch("station"               , &b_station               );
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
    b_new_cfeb_cable_delay  = m_new_cfeb_cable_delay[id];
    b_new_cfeb_cable_length = m_new_cfeb_cable_length[id];
    b_new_cfeb_skew_delay   = m_new_cfeb_skew_delay[id];

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

// ------------ method called once each job just before starting event loop  ------------
void 
CSCTimingAnalyzer::beginJob()
{
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
}

void CSCTimingAnalyzer::loadBranches (const edm::Event& event)
{
    event.getByLabel("cscTimingBabyMaker" , "run"                         , run_h                        );                        
    event.getByLabel("cscTimingBabyMaker" , "lumi"                        , lumi_h                       );                       
    event.getByLabel("cscTimingBabyMaker" , "evt"                         , evt_h                        );                        
    event.getByLabel("cscTimingBabyMaker" , "bfield"                      , bfield_h                     );                     
    event.getByLabel("cscTimingBabyMaker" , "cscstatus"                   , csc_status_h                 );                 
    event.getByLabel("cscTimingBabyMaker" , "detectorstatus"              , detector_status_h            );            
    event.getByLabel("cscTimingBabyMaker" , "isglobal"                    , is_global_h                  );                  
    event.getByLabel("cscTimingBabyMaker" , "istracker"                   , is_tracker_h                 );                 
    event.getByLabel("cscTimingBabyMaker" , "issa"                        , is_sa_h                      );                      
    event.getByLabel("cscTimingBabyMaker" , "ispf"                        , is_pf_h                      );                      
    event.getByLabel("cscTimingBabyMaker" , "p4"                          , p4_h                         );                         
    event.getByLabel("cscTimingBabyMaker" , "dz"                          , dz_h                         );                         
    event.getByLabel("cscTimingBabyMaker" , "d0"                          , d0_h                         );                         
    event.getByLabel("cscTimingBabyMaker" , "gfitchi2"                    , gfit_chi2_h                  );                  
    event.getByLabel("cscTimingBabyMaker" , "gfitndof"                    , gfit_ndof_h                  );                  
    event.getByLabel("cscTimingBabyMaker" , "maxOpeningAngleTrack"        , maxOpeningAngleTrack_h       );       
    event.getByLabel("cscTimingBabyMaker" , "maxOpeningAngleMuon"         , maxOpeningAngleMuon_h        );        
    event.getByLabel("cscTimingBabyMaker" , "type"                        , type_h                       );                       
    event.getByLabel("cscTimingBabyMaker" , "validsiLayers"               , valid_siLayers_h             );             
    event.getByLabel("cscTimingBabyMaker" , "validpixelHits"              , valid_pixelHits_h            );            
    event.getByLabel("cscTimingBabyMaker" , "charge"                      , charge_h                     );                     
    event.getByLabel("cscTimingBabyMaker" , "trkcharge"                   , trk_charge_h                 );                 
    event.getByLabel("cscTimingBabyMaker" , "gfitvalidSTAhits"            , gfit_validSTAhits_h          );          
    event.getByLabel("cscTimingBabyMaker" , "gfitvalidSiHits"             , gfit_validSiHits_h           );           
    event.getByLabel("cscTimingBabyMaker" , "stavalidHits"                , sta_validHits_h              );              
    event.getByLabel("cscTimingBabyMaker" , "numMatchedStations"          , numMatchedStations_h         );         
    event.getByLabel("cscTimingBabyMaker" , "numMatchedCSCsegments"       , numMatchedCSCsegments_h      );      
    event.getByLabel("cscTimingBabyMaker" , "stationmask"                 , station_mask_h               );               
    event.getByLabel("cscTimingBabyMaker" , "distToChamberEdge"           , distToChamberEdge_h          );          
    event.getByLabel("cscTimingBabyMaker" , "distToChamberEdgeErr"        , distToChamberEdgeErr_h       );       
    event.getByLabel("cscTimingBabyMaker" , "numCSCsegmentsInChamber"     , numCSCsegmentsInChamber_h    );    
    event.getByLabel("cscTimingBabyMaker" , "arbitrationmask"             , arbitration_mask_h           );           
    event.getByLabel("cscTimingBabyMaker" , "isSegmentAndTrackArbitrated" , isSegmentAndTrackArbitrated_h);
    event.getByLabel("cscTimingBabyMaker" , "numRecHitsInSegment"         , numRecHitsInSegment_h        );        
    event.getByLabel("cscTimingBabyMaker" , "endcap"                      , endcap_h                     );                     
    event.getByLabel("cscTimingBabyMaker" , "station"                     , station_h                    );                    
    event.getByLabel("cscTimingBabyMaker" , "ring"                        , ring_h                       );                       
    event.getByLabel("cscTimingBabyMaker" , "chamber"                     , chamber_h                    );                    
    event.getByLabel("cscTimingBabyMaker" , "layer"                       , layer_h                      );                      
    event.getByLabel("cscTimingBabyMaker" , "nstrips"                     , nstrips_h                    );
    event.getByLabel("cscTimingBabyMaker" , "rhtime"                      , rhtime_h                     );                     
    event.getByLabel("cscTimingBabyMaker" , "twire"                       , twire_h                      );                      
    event.getByLabel("cscTimingBabyMaker" , "cfebcablelength"             , cfeb_cable_length_h          );          
    event.getByLabel("cscTimingBabyMaker" , "cfebskewdelay"               , cfeb_skew_delay_h            );            
    event.getByLabel("cscTimingBabyMaker" , "cfebtimingcorr"              , cfeb_timing_corr_h           );           
    event.getByLabel("cscTimingBabyMaker" , "cfebcabledelay"              , cfeb_cable_delay_h           );           
    event.getByLabel("cscTimingBabyMaker" , "precision"                   , precision_h                  );                  
    event.getByLabel("cscTimingBabyMaker" , "cfebcablerev"                , cfeb_cable_rev_h             );             
    event.getByLabel("cscTimingBabyMaker" , "anodebxoffset"               , anode_bx_offset_h            );            
    event.getByLabel("cscTimingBabyMaker" , "cfebCorr"                    , cfebCorr_h                   );
    event.getByLabel("cscTimingBabyMaker" , "chipCorr"                    , chipCorr_h                   );
    event.getByLabel("cscTimingBabyMaker" , "skewClearDelay"              , skewClearDelay_h             );             
    event.getByLabel("cscTimingBabyMaker" , "cfebCableDelay"              , cfebCableDelay_h             );             
    event.getByLabel("cscTimingBabyMaker" , "chamberCorr"                 , chamberCorr_h                );                
    event.getByLabel("cscTimingBabyMaker" , "newskewClearDelay"           , new_skewClearDelay_h         );         
    event.getByLabel("cscTimingBabyMaker" , "newcfebCableDelay"           , new_cfebCableDelay_h         );         
    event.getByLabel("cscTimingBabyMaker" , "newchamberCorr"              , new_chamberCorr_h            );            
    event.getByLabel("cscTimingBabyMaker" , "newcfebcablelength"          , new_cfeb_cable_length_h      );      
    event.getByLabel("cscTimingBabyMaker" , "newcfebcablerev"             , new_cfeb_cable_rev_h         );         
    event.getByLabel("cscTimingBabyMaker" , "newcfebskewdelay"            , new_cfeb_skew_delay_h        );        
    event.getByLabel("cscTimingBabyMaker" , "newcfebcabledelay"           , new_cfeb_cable_delay_h       );       
    event.getByLabel("cscTimingBabyMaker" , "newcfebCorr"                 , new_cfebCorr_h               );
    event.getByLabel("cscTimingBabyMaker" , "newchipCorr"                 , new_chipCorr_h               );               
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
    gfit_chi2                   = -999;
    gfit_ndof                   = -1;
    maxOpeningAngleTrack        = -999;
    maxOpeningAngleMuon         = -999;
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
    return;
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
