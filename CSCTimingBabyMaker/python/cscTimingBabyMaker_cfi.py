import FWCore.ParameterSet.Config as cms

cscTimingBabyMaker = cms.EDFilter('CSCTimingBabyMaker',
                                  readBadChannels = cms.bool(True),
                                  readBadChambers = cms.bool(True),
                                  CSCUseTimingCorrections = cms.bool(True),
                                  CSCUseGasGainCorrections = cms.bool(True),
                                  muonTag = cms.InputTag('muons'),
                                  vtxTag = cms.InputTag('offlinePrimaryVertices'),
                                  trkTag = cms.InputTag('generalTracks'),
                                  bsTag = cms.InputTag('offlineBeamSpot'),
                                  trgResTag = cms.InputTag("TriggerResults::HLT"),
                                  trgEvtTag = cms.InputTag("hltTriggerSummaryAOD::HLT"),
                                  useMuonSegmentMatcher = cms.bool(False),
                                  debug = cms.bool(False),
                                  verbose = cms.bool(False),
                                  MatchParameters = cms.PSet(
                                      DTsegments = cms.InputTag("dt4DSegments"),
                                      DTradius = cms.double(0.1),
                                      CSCsegments = cms.InputTag("cscSegments"),
                                      TightMatchDT = cms.bool(False),
                                      TightMatchCSC = cms.bool(True)
                                  ),
                                  ServiceParameters = cms.PSet(
                                      Propagators = cms.untracked.vstring('SteppingHelixPropagatorAny'),
                                      RPCLayers = cms.bool(True)
                                  ),
                                  fillTriggerObjects = cms.untracked.bool(True),
                                  prunedTriggerNames = cms.untracked.vstring(
                                      "HLT*Mu*")
)
