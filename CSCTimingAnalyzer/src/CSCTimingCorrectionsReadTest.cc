#include "CSCOfflineTiming/CSCTimingAnalyzer/interface/CSCTimingCorrectionsReadTest.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "CalibMuon/CSCCalibration/interface/CSCChannelMapperRecord.h"
#include "CalibMuon/CSCCalibration/interface/CSCChannelMapperBase.h"
#include "CondFormats/DataRecord/interface/CSCChamberTimeCorrectionsRcd.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerRecord.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerBase.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerPostls1.h"
#include <iostream>
#include <fstream>

CSCTimingCorrectionsReadTest::CSCTimingCorrectionsReadTest(const edm::ParameterSet& iConfig) :
    isLoaded_(false)
{
    outfileName_ = iConfig.getUntrackedParameter<std::string>("outfileName", "test.dat");
}


CSCTimingCorrectionsReadTest::~CSCTimingCorrectionsReadTest()
{
}


//
// member functions
//

// ------------ method called for each event  ------------
void
CSCTimingCorrectionsReadTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    if (!isLoaded_)
    {
        iSetup.get<CSCChamberTimeCorrectionsRcd>().get(theChamberTimingCorrections);
        iSetup.get<CSCIndexerRecord>().get(indexer);
        iSetup.get<CSCChannelMapperRecord>().get(mapper);
        isLoaded_ = true;
    }
    
    FILE *outfile = fopen(outfileName_.c_str(), "w+");
    if (outfile)
    {        
        std::cerr << "Failed to open file test.dat for writing." << std::endl;
    }

    const unsigned int NCHAMBERS = indexer->maxChamberIndex();
    for (unsigned int ic = 1; ic <= NCHAMBERS; ic++)
    {
        CSCDetId id = indexer->detIdFromChamberIndex(ic);
        CSCDetId idraw = mapper->rawCSCDetId(id);
        unsigned int index = indexer->chamberIndex(idraw) - 1;
        if (ic != index+1)
            printf("ic = %d, index = %d\n, id=(%d,%d,%d,%d)\n", ic, index, id.endcap(), id.station(), id.ring(), id.chamber());
        double corr = theChamberTimingCorrections->item(index).cfeb_timing_corr*1./theChamberTimingCorrections->precision();
        int clength = theChamberTimingCorrections->item(index).cfeb_length;
        char crev = theChamberTimingCorrections->item(index).cfeb_rev;
        fprintf(outfile, "%d %d %d %d %d %c %d %4.2f\n", ic, id.endcap(), id.station(), id.ring(), id.chamber(), crev, clength, corr);
        // printf("%d %d %d %d %d %4.2f\n", ic, id.endcap(), id.station(), id.ring(), id.chamber(), corr);
    }

    fclose(outfile);
}
void 
CSCTimingCorrectionsReadTest::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CSCTimingCorrectionsReadTest::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CSCTimingCorrectionsReadTest::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCTimingCorrectionsReadTest);
