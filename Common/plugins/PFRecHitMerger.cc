#include <vector>

#include "FWCore/Framework/interface/global/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/transform.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"

class PFRecHitMerger : public edm::global::EDProducer<> {
public:
  explicit PFRecHitMerger(const edm::ParameterSet &iConfig);
  ~PFRecHitMerger() override = default;
  static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);

private:
  void produce(edm::StreamID, edm::Event &, const edm::EventSetup &) const override;

  std::vector<edm::EDGetTokenT<reco::PFRecHitCollection>> const pfRecHitTokens_;
};

PFRecHitMerger::PFRecHitMerger(const edm::ParameterSet &iConfig)
  : pfRecHitTokens_{edm::vector_transform(iConfig.getParameter<std::vector<edm::InputTag>>("src"), [this](edm::InputTag const& tag) { return consumes<reco::PFRecHitCollection>(tag); })} {
  produces<reco::PFRecHitCollection>();
}

void PFRecHitMerger::fillDescriptions(edm::ConfigurationDescriptions &descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<std::vector<edm::InputTag>>("src", {});
  descriptions.addWithDefaultLabel(desc);
}

void PFRecHitMerger::produce(edm::StreamID, edm::Event& iEvent, const edm::EventSetup&) const {
  auto recoPFRecHits = std::make_unique<reco::PFRecHitCollection>();

  for(auto const& pfRecHitToken : pfRecHitTokens_){
    auto const& pfRecHits = iEvent.get(pfRecHitToken);
    recoPFRecHits->insert(recoPFRecHits->end(), pfRecHits.begin(), pfRecHits.end());
  }
  std::cout<<"!!!!!!!!!!!!!!!!!!!!!!! " << recoPFRecHits->size() << std::endl;
  iEvent.put(std::move(recoPFRecHits));
}

DEFINE_FWK_MODULE(PFRecHitMerger);
