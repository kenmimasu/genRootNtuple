// main92.cc is a part of the PYTHIA event generator.
// Copyright (C) 2017 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// Modified by Rene Brun and Axel Naumann to put the Pythia::event
// into a TTree.


#include <iostream>
// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"
using namespace std;
using namespace Pythia8;

int main() {

  // Pythia instance
  Pythia pythia;
  pythia.settings.addWord("Main:outputFileName", "myNtuple.root"); 

  // Read in settings & intialize
  pythia.readFile( "genRootNtuple.cmnd", 0);
  pythia.init();

  // Program options
  int nEvents = pythia.mode("Main:numberOfEvents");
  if (nEvents < 0) nEvents = 1e9;

  const string outName = pythia.word("Main:outputFileName");

  // Branch variable declaration
  int eventNumber;
  int lha_id1;
  int lha_id2;
  int lha_id1pdf;
  int lha_id2pdf;
  int lha_strategy;
  int mc_n;
  
  float lha_s;
  float lha_x1;
  float lha_x2;
  float lha_x1pdf;
  float lha_x2pdf;
  float lha_pdf1;
  float lha_pdf2;
  float lha_QFac;
  float lha_alphaS;
  float lha_alphaEM;
  float lha_scalup;
  float lha_weight;
  float lha_y;
  float lha_tau;

  vector<int> *mc_pdgId = new vector<int>;
  vector<int> *mc_status = new vector<int>;
  vector<int> *mc_charge = new vector<int>;
  
  vector<float> *mc_pt = new vector<float>;
  vector<float> *mc_eta = new vector<float>;
  vector<float> *mc_phi = new vector<float>;
  vector<float> *mc_m = new vector<float>;

  vector< vector<int> > *mc_parent_index= new vector< vector<int> >;
  vector< vector<int> > *mc_child_index = new vector< vector<int> >;

  // PYTHIA info instance
  Info *info = &pythia.info;
  
  // Set up the ROOT TFile and TTree.
  TFile *file = TFile::Open(outName.c_str(), "recreate");

  TTree *T = new TTree("T", "Hadronised Event Tree");

  T->Branch("lha_id1",&lha_id1);
  T->Branch("lha_id2",&lha_id2);
  T->Branch("lha_id1pdf",&lha_id1pdf);
  T->Branch("lha_id2pdf",&lha_id2pdf);
  T->Branch("lha_strategy",&lha_strategy);
  
  T->Branch("lha_s",&lha_s);
  T->Branch("lha_x1",&lha_x1);
  T->Branch("lha_x2",&lha_x2);
  T->Branch("lha_x1pdf",&lha_x1pdf);
  T->Branch("lha_x2pdf",&lha_x2pdf);
  T->Branch("lha_pdf1",&lha_pdf1);
  T->Branch("lha_pdf2",&lha_pdf2);
  T->Branch("lha_QFac",&lha_QFac);
  T->Branch("lha_alphaS",&lha_alphaS);
  T->Branch("lha_alphaEM",&lha_alphaEM);
  T->Branch("lha_scalup",&lha_scalup);
  T->Branch("lha_weight",&lha_weight);
  T->Branch("lha_y",&lha_y);
  T->Branch("lha_tau",&lha_tau);
  
  T->Branch("mc_n", &mc_n);
  T->Branch("mc_pt", &mc_pt);
  T->Branch("mc_m", &mc_m);
  T->Branch("mc_eta", &mc_eta);
  T->Branch("mc_phi", &mc_phi);
  T->Branch("mc_status", &mc_status);
  T->Branch("mc_pdgId", &mc_pdgId);
  T->Branch("mc_charge", &mc_charge);
  T->Branch("mc_parent_index", &mc_parent_index);
  T->Branch("mc_child_index", &mc_child_index);
  T->Branch("EventNumber", &eventNumber);

 // Begin event loop. Generate event; skip if generation aborted.
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next()){
        if (info->atEndOfFile()) break;
        continue;
    } 
    // Clear values
    eventNumber=0;
    lha_id1=0;
    lha_id2=0;
    lha_id1pdf=0;
    lha_id2pdf=0;
    lha_strategy=0;
    mc_n=0;
    lha_s=0;
    lha_x1=0;
    lha_x2=0;
    lha_x1pdf=0;
    lha_x2pdf=0;
    lha_pdf1=0;
    lha_pdf2=0;
    lha_QFac=0;
    lha_alphaS=0;
    lha_alphaEM=0;
    lha_scalup=0;
    lha_weight=0;
    lha_y=0;
    lha_tau=0;
    mc_pt->clear();
    mc_eta->clear();
    mc_phi->clear();
    mc_m->clear();
    mc_pdgId->clear();
    mc_status->clear();
    mc_charge->clear();
    mc_parent_index->clear();
    mc_child_index->clear();

    // Assign LHA event info values
    lha_id1=info->id1();
    lha_id2=info->id2();
    lha_id1pdf=info->id1pdf();
    lha_id2pdf=info->id2pdf();
    lha_strategy=info->lhaStrategy();
    lha_s=info->s();
    lha_x1=info->x1();
    lha_x2=info->x2();
    lha_x1pdf=info->x1pdf();
    lha_x2pdf=info->x2pdf();
    lha_pdf1=info->pdf1();
    lha_pdf2=info->pdf2();
    lha_QFac=info->QFac();
    lha_alphaS=info->alphaS();
    lha_alphaEM=info->alphaEM();
    lha_scalup=info->scalup();
    lha_weight=info->weight();
    lha_y=info->y();
    lha_tau=info->tau();
    
    // Assign particle info values 
    mc_n=pythia.event.size();
    
    for(int i =0; i<mc_n; i++){
      mc_pt->push_back( pythia.event[i].pT());
      mc_eta->push_back( pythia.event[i].eta());
      mc_phi->push_back( pythia.event[i].phi());
      mc_m->push_back( pythia.event[i].m());

      mc_pdgId->push_back( pythia.event[i].id());
      mc_status->push_back( pythia.event[i].status());
      mc_charge->push_back( pythia.event[i].charge());
      
      mc_parent_index->push_back(pythia.event[i].motherList());
      mc_child_index->push_back(pythia.event[i].daughterList());
    }

    T->Fill();

  // End event loop.
  }

  // Statistics on event generation.
  // pythia.stat();

  //  Write tree.
  // cout << "printing tree"<< endl;
  // T->Print();
  // cout << "writing tree"<< endl;
  T->Write();
  // cout << "deleting file"<< endl;
  // delete file;
  cout << "done"<< endl;

  // Done.
  return 0;
}


// main13.cc is a part of the PYTHIA event generator.
// Copyright (C) 2017 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It illustrates how two Les Houches Event Files can be combined in PYTHIA,
// just like in main12.cc, but here with the difference that information is
// stored in main13.cmnd and read out using the subruns possibility.

/*#include "Pythia8/Pythia.h"
using namespace Pythia8;
int main() {

  // Book histogram.
  Hist nCharged("charged particle multiplicity",100,-0.5,399.5);

  // Generator.
  Pythia pythia;


  // Extract data to be used in main program. Set counters.
  int nSubrun = pythia.mode("Main:numberOfSubruns");
  int nAbort  = pythia.mode("Main:timesAllowErrors");
  int iAbort  = 0;

  // Begin loop over subruns.
  for (int iSubrun = 1; iSubrun <= nSubrun; ++iSubrun) {

    // Read in subrun-specific data from main13.cmnd.
    pythia.readFile( "main13.cmnd", iSubrun);

    // Initialize generator.
    pythia.init();

    // Begin infinite event loop - to be exited at end of file.
    for (int iEvent = 0; ; ++iEvent) {

      // Generate next event.
      if (!pythia.next()) {

        // Leave event loop if at end of file.
        if (pythia.info.atEndOfFile()) break;

        // First few failures write off as "acceptable" errors, then quit.
        if (++iAbort < nAbort) continue;
        break;
      }

    // End of event loop.
    }

  // End of subrun loop.
  }

  // Give statistics. Print histogram.
  pythia.stat();
  cout << nCharged;

  // Done.
  return 0;
}*/
