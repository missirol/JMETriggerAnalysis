#ifndef Analysis_JMETrigger_JMETriggerAnalysisDriver_h
#define Analysis_JMETrigger_JMETriggerAnalysisDriver_h

#include "Analysis/JMETrigger/interface/AnalysisDriverBase.h"

#include <vector>
#include <memory>
#include <map>

#include <TH1D.h>
#include <TH2D.h>

class JMETriggerAnalysisDriver : public AnalysisDriverBase {

 public:
  explicit JMETriggerAnalysisDriver(const std::string& tfile, const std::string& ttree, const std::string& outputFilePath="", const std::string& outputFileMode="recreate")
    : AnalysisDriverBase(tfile, ttree, outputFilePath, outputFileMode) {}
  ~JMETriggerAnalysisDriver() {}

  void init() override;
  void analyze() override;
  void write(TFile&) override;

  Long64_t eventsProcessed() const { return eventsProcessed_; }

 protected:

  class fillHistoDataJets {
   public:
    std::string jetCollection = "";
    float jetPtMin = -1.;

    struct Match {
      Match(const std::string& theLabel, const std::string& theJetCollection, const float theJetPtMin, const float theJetDeltaRMin)
        : label(theLabel), jetCollection(theJetCollection), jetPtMin(theJetPtMin), jetDeltaRMin(theJetDeltaRMin) {}
      std::string label;
      std::string jetCollection;
      float jetPtMin;
      float jetDeltaRMin;
    };
    std::vector<Match> matches;
  };

  class fillHistoDataMET {
   public:
    std::string metCollection = "";

    struct Match {
      Match(const std::string& theLabel, const std::string& theMETCollection)
      : label(theLabel), metCollection(theMETCollection) {}
      std::string label;
      std::string metCollection;
    };
    std::vector<Match> matches;
  };

  void fillHistograms_Jets(const std::string& dir, const fillHistoDataJets& fhDataJets);
  void fillHistograms_MET(const std::string& dir, const fillHistoDataMET& fhDataMET);

  void bookHistograms_Jets(const std::string& dir, const std::string& jetType, const std::vector<std::string>& matchLabels={});
  void bookHistograms_MET(const std::string& dir, const std::string& jetType, const std::vector<std::string>& matchLabels={});

  bool hasTH1D(const std::string& key) const { return (mapTH1D_.find(key) != mapTH1D_.end()); }
  bool hasTH2D(const std::string& key) const { return (mapTH2D_.find(key) != mapTH2D_.end()); }

  TH1D* H1(const std::string&);
  TH2D* H2(const std::string&);

  void addTH1D(const std::string&, const std::vector<float>&);
  void addTH2D(const std::string&, const std::vector<float>&, const std::vector<float>&);

  std::vector<std::string> stringTokens(const std::string&, const std::string&) const;
  float deltaPhi2(const float phi1, const float phi2) const;
  float deltaR2(const float eta1, const float phi1, const float eta2, const float phi2) const;

  Long64_t eventsProcessed_ = 0;

  int verbosity_ = 0;

  std::map<std::string, std::unique_ptr<TH1D>> mapTH1D_;
  std::map<std::string, std::unique_ptr<TH2D>> mapTH2D_;

  std::vector<std::string> outputKeys_;
};

#endif
