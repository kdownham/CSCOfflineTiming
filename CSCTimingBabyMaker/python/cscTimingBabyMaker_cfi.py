import FWCore.ParameterSet.Config as cms

cscTimingBabyMaker = cms.EDProducer('CSCTimingBabyMaker',
                                    readBadChannels = cms.bool(True),
                                    readBadChambers = cms.bool(True),
                                    CSCUseTimingCorrections = cms.bool(True),
                                    CSCUseGasGainCorrections = cms.bool(True),
                                    muonTag = cms.InputTag('muons'),
                                    vtxTag = cms.InputTag('offlinePrimaryVertices'),
                                    trkTag = cms.InputTag('generalTracks'),
                                    bsTag = cms.InputTag('offlineBeamSpot'),
                                    trgResTag = cms.InputTag('TriggerResults','','RECO'),
                                    verbose = cms.bool(False))
