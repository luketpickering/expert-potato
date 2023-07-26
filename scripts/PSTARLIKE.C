void PSTARLIKE(const char *infn, const char *reffn, const char *ofn) {

  TFile *fin = TFile::Open(infn);
  TTree *tin = fin->Get<TTree>("sumtree");

  double TInit_MeV;
  double Depth_cm;

  tin->SetBranchAddress("TInit_MeV", &TInit_MeV);
  tin->SetBranchAddress("Depth_cm", &Depth_cm);

  TGraph PSTAR(reffn);

  std::vector<double> Tinit_Bins;

  for (int i = 1; i < PSTAR.GetN(); ++i) {
    double x1 = PSTAR.GetPointX(i - 1);
    double x2 = PSTAR.GetPointX(i);

    if (!Tinit_Bins.size()) {
      Tinit_Bins.push_back(x1 - ((x2 - x1) / 2.0));
    }
    Tinit_Bins.push_back((x2 + x1) / 2.0);
  }
  Tinit_Bins.push_back(2.0 * Tinit_Bins.back() -
                       Tinit_Bins[Tinit_Bins.size() - 2]);

  TAxis ax(Tinit_Bins.size() - 1, Tinit_Bins.data());

  std::vector<std::vector<double>> Ranges;
  Ranges.resize(Tinit_Bins.size() - 1);

  Long64_t ents = tin->GetEntries();
  for (Long64_t i = 0; i < ents; ++i) {
    tin->GetEntry(i);

    int bin = ax.FindFixBin(TInit_MeV) - 1;
    if (bin < (Tinit_Bins.size() - 1)) {
      Ranges[bin].push_back(Depth_cm);
    }
  }

  std::vector<double> RangeMeans;
  for (int i = 0; i < Ranges.size(); ++i) {
    double sum = 0;
    for (double const &r : Ranges[i]) {
      sum += r;
    }
    RangeMeans.push_back(sum > 0 ? (sum / double(Ranges[i].size())) : 0);
  }

  std::ofstream fout(ofn);

  for (int i = 0; i < Ranges.size(); ++i) {
    if (RangeMeans[i]) {
      fout << ax.GetBinCenter(i + 1) << " " << (RangeMeans[i] * 0.0178)
           << std::endl;
    }
  }
}