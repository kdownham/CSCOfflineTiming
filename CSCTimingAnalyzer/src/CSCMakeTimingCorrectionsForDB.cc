#include "CSCOfflineTiming/CSCTimingAnalyzer/interface/CSCMakeTimingCorrectionsForDB.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "CalibMuon/CSCCalibration/interface/CSCChannelMapperRecord.h"
#include "CalibMuon/CSCCalibration/interface/CSCChannelMapperBase.h"
#include "RecoLocalMuon/CSCRecHitD/src/CSCRecoConditions.h"
#include "CondFormats/DataRecord/interface/CSCChamberTimeCorrectionsRcd.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerRecord.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerBase.h"
#include "CalibMuon/CSCCalibration/interface/CSCIndexerPostls1.h"
#include "CondFormats/DataRecord/interface/CSCDBChipSpeedCorrectionRcd.h"
#include "CondFormats/CSCObjects/interface/CSCDBChipSpeedCorrection.h"
#include <iostream>
#include <fstream>

const unsigned int CSCMakeTimingCorrectionsForDB::NSTATIONS = 4;

CSCMakeTimingCorrectionsForDB::CSCMakeTimingCorrectionsForDB(const edm::ParameterSet& iConfig) :
    isLoaded_(false)
{
    heuristicCorrFileName = iConfig.getUntrackedParameter<std::string>("heuristicCorrFileName", "");
    cableLengthFileName = iConfig.getUntrackedParameter<std::string>("cableLengthFileName", "");
    outFileName = iConfig.getUntrackedParameter<std::string>("outFileName", "");
    outChipFileName = iConfig.getUntrackedParameter<std::string>("outChipFileName", "");
    //edm::ConsumesCollector iC = consumesCollector(); // Added this line for extra argumet for following line
    //recoConditions = new CSCRecoConditions(iConfig,iC);
    recoConditions = new CSCRecoConditions(iConfig,consumesCollector());
    updateOnlyNewChambers_ = iConfig.getUntrackedParameter<bool>("updateOnlyNewChambers", false);
}


CSCMakeTimingCorrectionsForDB::~CSCMakeTimingCorrectionsForDB()
{
    delete recoConditions;
}


//
// member functions
//

// ------------ method called for each event  ------------
void
CSCMakeTimingCorrectionsForDB::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    if (!isLoaded_)
    {
        recoConditions->initializeEvent(iSetup);
        iSetup.get<CSCChamberTimeCorrectionsRcd>().get(theChamberTimingCorrections);
        iSetup.get<CSCIndexerRecord>().get(indexer);
        iSetup.get<CSCChannelMapperRecord>().get(mapper);
        iSetup.get<CSCDBChipSpeedCorrectionRcd>().get(theChipCorrections);
        isLoaded_ = true;
    }


    FILE *chipfile = 0;
    if (!outChipFileName.empty()) chipfile = fopen(outChipFileName.c_str(), "w+");
    if (!chipfile)
    {
        std::cerr << "Failed to open file " << outChipFileName << " for writing." << std::endl;
    }

    // printf("index \t endcap \t station \t ring \t chamber \t CFEB length \t CFEB REV \t ALCT length \t ALCT REV \t skew delay \t timing corr \t cable delay \t anode BX offset\n");
    // printf("----------------------------------------------------------------------------------------------------------------------------------\n");
    // std::cout << "endcap \t station \t ring \t chamber \t chipsPerOnlineLayer \t chipsPerLayer \t chipStart" << std::endl;
    for (unsigned int ie = 1; ie <= 2; ie++)
    {
        for (unsigned int is = 1; is <= NSTATIONS; is++)
        {
            unsigned int nrings = indexer->offlineRingsInStation(is);
            for (unsigned int ir = 1; ir <= nrings; ir++)
            {
                unsigned int nchambers = indexer->chambersInRingOfStation(is, ir);
                for (unsigned int ic = 1; ic <= nchambers; ic++)
                {
                    CSCDetId id(ie, is, ir, ic);
                    CSCDetId idraw = mapper->rawCSCDetId(id);
                    int index = indexer->chamberIndex(idraw) - 1;
                    CSCChamberTimeCorrections::ChamberTimeCorrections c = theChamberTimingCorrections->item(index);
                    mid[id] = index;
                    mvals[index] = c;
                    // if (id.endcap() == 2 && !(id.station() == 1 && (id.ring() ==1 || id.ring() == 4)))
                    // {
                    //     printf("(index, endcap, station, ring, chamber), corr = (%d, %d, %d, %d, %d), %4.2f\n", index, id.endcap(), id.station(), id.ring(), id.chamber(),c.cfeb_timing_corr * 1./theChamberTimingCorrections->precision());
                    // }
                    unsigned int nlayers = 6;
                    for (unsigned int il = 1; il <= nlayers; il++)
                    {
                        CSCDetId detid(ie, is, ir, ic, il);
                        unsigned int nchips = indexer->chipsPerLayer(is, ir);
                        for (unsigned int ichip = 1; ichip <= nchips; ichip++)
                        {
                            unsigned int chip_index = indexer->chipIndex(detid, ichip) - 1;
                            double chipCorr = theChipCorrections->value(chip_index);
                            chipCorr /= theChipCorrections->scale();
                            if (is==1 && (ir==1 || ir==4)) chipCorr = -52.41;
                            if (ie==2 && is==1 && ir==3 && ic==3 && chipCorr < -60) chipCorr = -52.41;
                            if (chipfile)
                                fprintf(chipfile, "%d %2.2f\n", chip_index+1, chipCorr);
                            // chipCorr -= 0.5;
                            // chipCorr /= theChipCorrections->scale();
                            // if (is==1 && (ir==1 || ir==4)) chipCorr = -52.41;
                            // chipCorr = 170. - chipCorr;
                            // if (chipfile)
                            //     fprintf(chipfile, "%d %d %d %d %d %d %3.3f %f %d\n", index+1, ie, is, ir, ic, chip_index % 30, chipCorr, 1., 1);
                                // fprintf(chipfile, "%d %d %d %d %d %d %d %3.3f %f %d\n", index+1, ie, is, ir, ic, chip_index, chip_index % 30, chipCorr, 1., 1);
                        }
                    }
                }
            }
        }
    }

    fclose(chipfile);

    //
    // read heuristic corrections from file
    //
    std::ifstream infile(heuristicCorrFileName.c_str());
    if (!infile)
    {
        std::cout << "Failed to read heuristic corrections from file " << heuristicCorrFileName << std::endl;
        return;
    }
    bool is_first = true;
    std::string line;
    while (std::getline(infile, line))
    {
        if (is_first)
        {
            is_first = false;
            continue;
        }
        std::istringstream ss(line);
        int endcap, station, ring, chamber;
        double dcorr;
        ss >> endcap >> station >> ring >> chamber >> dcorr;
        int corr = 100*dcorr;
        CSCDetId id(endcap, station, ring, chamber);
        mcorr[id] = corr;
    }
    infile.close();

    //
    // read skewclear cable lengths from file
    //
    infile.open(cableLengthFileName.c_str());
    if (!infile)
    {
        std::cout << "Failed to read skewclear cable lengths from file " << cableLengthFileName << std::endl;
        return;
    }
    is_first = true;
    while (std::getline(infile, line))
    {
        if (is_first)
        {
            is_first = false;
            continue;
        }
        std::istringstream ss(line);
        int endcap, station, ring, chamber, clength, cdelay;
        char crev;
        ss >> endcap >> station >> ring >> chamber >> crev >> clength >> cdelay;
        CSCDetId id(endcap, station, ring, chamber);
        mcable[id] = clength;
        mcrev[id] = crev;
        mdelay[id] = cdelay;
    }

    FILE *outfile = 0;
    if (!outFileName.empty()) outfile = fopen(outFileName.c_str(), "w+");
    if (!outfile)
    {
        std::cerr << "Failed to open file " << outFileName << " for writing." << std::endl;
    }

    FILE *oldfile = fopen("timecorr_old_unsorted.dat", "w+");
    if (oldfile)
    {
        std::cerr << "Failed to open file old.txt for writing." << std::endl;
    }

    for (auto item : mid)
    {
        if (item.first.ring() == 4) continue;
        if (oldfile)
        {
            fprintf(oldfile, "%d %hd %c %hd %c %hd %hd %d %hd\n",
                    item.second+1,
                    mvals[item.second].cfeb_length, mvals[item.second].cfeb_rev,
                    mvals[item.second].alct_length, mvals[item.second].alct_rev,
                    mvals[item.second].cfeb_tmb_skew_delay,
                    mvals[item.second].anode_bx_offset,
                    mvals[item.second].cfeb_timing_corr,
                    mvals[item.second].cfeb_cable_delay);
        }
        int corr = mvals[item.second].cfeb_timing_corr;
        int clength = mvals[item.second].cfeb_length;
        int cdelay = mvals[item.second].cfeb_tmb_skew_delay;
        char crev = mvals[item.second].cfeb_rev;
        if (updateOnlyNewChambers_)
        {
            if ((mcorr.find(item.first) != mcorr.end()) && item.first.station() == 1 && (item.first.ring() == 1 || item.first.ring() == 4))
            {
                corr = mcorr[item.first];
            // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
            if ((mcorr.find(item.first) != mcorr.end()) && item.first.station() == 4 && item.first.ring() == 2 && (item.first.endcap() == 2 || (item.first.endcap() == 1 && (item.first.chamber() < 9 || item.first.chamber() > 13))))
            {
                corr = mcorr[item.first];
                // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
            if ((mcable.find(item.first) != mcable.end()) && item.first.station() == 1 && (item.first.ring() == 1 || item.first.ring() == 4))
            {
                clength = mcable[item.first];
                // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
            if ((mcrev.find(item.first) != mcrev.end()) && item.first.station() == 1 && (item.first.ring() == 1 || item.first.ring() == 4))
            {
                crev = mcrev[item.first];
                // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
            if ((mdelay.find(item.first) != mdelay.end()) && item.first.station() == 1 && (item.first.ring() == 1 || item.first.ring() == 4))
            {
                cdelay = mdelay[item.first];
                // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
        }
        else
        {
            if ((mcorr.find(item.first) != mcorr.end()))
            {
                corr = mcorr[item.first];
            // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
            if ((mcable.find(item.first) != mcable.end()))
            {
                clength = mcable[item.first];
                // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
            if ((mcrev.find(item.first) != mcrev.end()))
            {
                crev = mcrev[item.first];
                // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
            if ((mdelay.find(item.first) != mdelay.end()))
            {
                cdelay = mdelay[item.first];
                // printf("\tusing new correction for (index, endcap, station, ring, chamber) = (%d, %d, %d, %d, %d)\n", item.second, item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber());
            }
        }

        if (outfile)
        {
            fprintf(outfile, "%d %hd %c %hd %c %hd %hd %d %hd\n",
                    item.second+1,
                    clength, crev,
                    mvals[item.second].alct_length, mvals[item.second].alct_rev,
                    cdelay,
                    getUpdatedAnodeBXOffset(item.first),
                    corr,
                    mvals[item.second].cfeb_cable_delay);
        }
        printf("%u %d %d %d %d %hd %c %hd %c %hd %hd %d %hd\n",
               item.second+1,
               item.first.endcap(), item.first.station(), item.first.ring(), item.first.chamber(),
               clength, crev,
               mvals[item.second].alct_length, mvals[item.second].alct_rev,
               cdelay,
               getUpdatedAnodeBXOffset(item.first),
               corr,
               mvals[item.second].cfeb_cable_delay);
    }
    fclose(outfile);
    fclose(oldfile);
}

unsigned int CSCMakeTimingCorrectionsForDB::getUpdatedAnodeBXOffset (CSCDetId id)
{
    // if (id.station() == 1 && (id.ring() == 1 || id.ring() == 4)) return 830;
    // else if (id.station() > 1 && id.ring() == 1) return 820;
    // else if (id.ring() == 2 || id.ring() == 3) return 828;
    // return 820;

    // if (id.station() == 1 && id.ring() == 3) return 820;
    // else if (id.station() > 1 && id.ring() == 1) return 815;
    // else if (id.ring() == 2 || id.ring() == 1) return 822;
    // return 820;

    if (id.station() == 1 && id.ring() == 1) return 822;
    else if (id.station() > 1 && id.ring() == 1) return 815;
    else if (id.ring() == 2 || id.ring() == 3) return 820;
    return 820;
}

void
CSCMakeTimingCorrectionsForDB::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
CSCMakeTimingCorrectionsForDB::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
CSCMakeTimingCorrectionsForDB::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(CSCMakeTimingCorrectionsForDB);
