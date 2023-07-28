
void plotranges() {
  TGraph PSTAR("data/PSTAR_ar18.dat");
  TGraph arbox_proton("output/proton/proton_stop.dat");
  TGraph arbox_muon("output/mu/mu_stop.dat");
  TGraph arbox_pip("output/pip/pip_stop.dat");
  TGraph arbox_pim("output/pim/pim_stop.dat");

  TCanvas c1("c1", "", 1200,1000);
  c1.SetLogx();
  c1.SetLogy();

  PSTAR.Draw("AL");
  PSTAR.SetTitle("PSTAR ^{18}Ar");
  PSTAR.GetXaxis()->SetTitle("T (MeV)");
  PSTAR.GetYaxis()->SetTitle("Range (g/cm2)");

  arbox_proton.SetTitle("edep-sim proton");
  arbox_proton.SetLineColor(kRed);
  arbox_proton.Draw("L");

  arbox_muon.SetTitle("edep-sim mu-");
  arbox_muon.SetLineColor(kBlue);
  arbox_muon.Draw("L");

  arbox_pip.SetTitle("edep-sim pi+");
  arbox_pip.SetLineColor(kGreen);
  arbox_pip.Draw("L");

  arbox_pim.SetTitle("edep-sim pi-");
  arbox_pim.SetLineColor(kCyan);
  arbox_pim.Draw("L");

  c1.BuildLegend();

  c1.Print("PSTAR.png");
}