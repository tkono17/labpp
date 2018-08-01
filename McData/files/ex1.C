.L sw/lib/libMcData.so
TFile *_file0 = TFile::Open("bb.root")
TTree* t = dynamic_cast<TTree*>(_file0->Get(""));
PJEvent event;
event.setBranchAddress(t);
t->GetEntries()
t->GetEntry(0)
event.nParticles()
event.nJets()
