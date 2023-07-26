#pragma cling load("/opt/edep-sim/3.2.0/lib/libedepsim_io.so")

#include "/opt/edep-sim/3.2.0/include/EDepSim/TG4Event.h"

double mass(int pdg) {
  switch (std::abs(pdg)) {
  case 13: {
    return 105.7;
  }
  case 2212: {
    return 938.27;
  }
  case 211: {
    return 139.570;
  }
  default: {
    std::cout << "unknown mass for pdg: " << pdg << std::endl;
    abort();
  }
  }
}

double TFromP(double mom_MeV, int pdg) {
  double mass_MeV = mass(pdg);
  return std::sqrt(std::pow(mom_MeV, 2) + std::pow(mass_MeV, 2)) - mass_MeV;
}

void stoppingdist(const char *infn, const char *outfn) {

  TFile *fin = TFile::Open(infn);
  /// Get the event tree.
  TTree *events = fin->Get<TTree>("EDepSimEvents");

  /// Set the branch address.
  TG4Event *event = NULL;
  events->SetBranchAddress("Event", &event);

  TFile *fout = TFile::Open(outfn, "RECREATE");
  TTree *tout = new TTree("sumtree", "");

  int NTraj, pdg;
  double PInit_MeV, TInit_MeV, TEnd_MeV;
  double PathLength_cm, Depth_cm;
  tout->Branch("NTraj", &NTraj);
  tout->Branch("pdg", &pdg);
  tout->Branch("TInit_MeV", &TInit_MeV);
  tout->Branch("PInit_MeV", &PInit_MeV);
  tout->Branch("TEnd_MeV", &TEnd_MeV);
  tout->Branch("PathLength_cm", &PathLength_cm);
  tout->Branch("Depth_cm", &Depth_cm);

  Long64_t ents = events->GetEntries();

  for (Long64_t e = 0; e < ents; ++e) {
    events->GetEntry(e);

    pdg = event->Trajectories.front().GetPDGCode();
    NTraj = event->Trajectories.size();

    auto init_mom = event->Trajectories.front().Points.front().GetMomentum();
    auto end_mom = event->Trajectories.front().Points.back().GetMomentum();

    auto init_pos =
        event->Trajectories.front().Points.front().GetPosition().Vect();
    auto end_pos =
        event->Trajectories.front().Points.back().GetPosition().Vect();

    PInit_MeV = init_mom.Mag();
    TInit_MeV = TFromP(init_mom.Mag(), pdg);
    TEnd_MeV = TFromP(end_mom.Mag(), pdg);

    Depth_cm = (end_pos[2] - init_pos[2]) * 1E-1;

    PathLength_cm = 0;

    for (int i = 1; i < event->Trajectories.front().Points.size(); ++i) {
      PathLength_cm += (event->Trajectories.front().Points[i].GetPosition() -
                        event->Trajectories.front().Points[i - 1].GetPosition())
                           .Vect()
                           .Mag();
    }

    tout->Fill();
  }
  fout->Write();
  fout->Close();
}