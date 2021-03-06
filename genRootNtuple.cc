// main92.cc is a part of the PYTHIA event generator.
// Copyright (C) 2017 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

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

int main(int argc, char** argv) {
  // Parse args
  string cmndFile = "genRootNtuple.cmnd";
  string usage = "Usage: ./genRootNtuple [COMMANDFILE] (default = genRootNtuple.cmnd)\n";
  if (argc == 1) {
  } else if(argc == 2) { 
      cmndFile = argv[1]; // Command file to be read in by Pythia
  } else { // Incorrect number of arguments
      cout << usage; 
      return 0;
  }
  
  // Pythia instance
  Pythia pythia;
  pythia.settings.addWord("Main:outputFileName", "myNtuple.root"); 

  // Read in settings & intialize
  pythia.readFile( cmndFile, 0);
  pythia.init();

  // Program options
  int nEvents = pythia.mode("Main:numberOfEvents");
  if (nEvents <= 0) nEvents = 1000000000; // should be large enough...

  const string outName = pythia.word("Main:outputFileName");

  // Branch variable declaration
  int eventNumber;
  int lha_id1, lha_id2;
  int lha_id1pdf, lha_id2pdf;
  int lha_strategy;
  int mc_n;
  
  float lha_s;
  float lha_x1, lha_x2;
  float lha_x1pdf, lha_x2pdf;
  float lha_pdf1, lha_pdf2;
  float lha_QFac;
  float lha_alphaS, lha_alphaEM;
  float lha_scalup;
  float lha_weight;
  float lha_y, lha_tau;

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
  T->Branch("eventNumber", &eventNumber);

 // Begin event loop. Generate event; skip if generation aborted.
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next()){
        if (info->atEndOfFile()) break;
        continue;
    } 
    // Clear values
    eventNumber = iEvent+1;
    lha_id1 = lha_id2 = 0;
    lha_id1pdf = lha_id2pdf = 0;
    lha_strategy = 0;
    mc_n = 0;
    lha_s = 0.;
    lha_x1 = lha_x2 = 0.;
    lha_x1pdf = lha_x2pdf = 0.;
    lha_pdf1 = lha_pdf2 = 0.;
    lha_QFac = 0.;
    lha_alphaS = lha_alphaEM = 0.;
    lha_scalup = 0.;
    lha_weight = 0.;
    lha_y = lha_tau = 0.;
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
    lha_id1 = info->id1(), lha_id2 = info->id2();
    lha_id1pdf = info->id1pdf(), lha_id2pdf = info->id2pdf();
    lha_strategy = info->lhaStrategy();
    lha_s = info->s();
    lha_x1 = info->x1(), lha_x2 = info->x2();
    lha_x1pdf = info->x1pdf(), lha_x2pdf = info->x2pdf();
    lha_pdf1 = info->pdf1(), lha_pdf2 = info->pdf2();
    lha_QFac = info->QFac();
    lha_alphaS = info->alphaS(), lha_alphaEM = info->alphaEM();
    lha_scalup = info->scalup();
    lha_weight = info->weight();
    lha_y = info->y(), lha_tau = info->tau();
    
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
  T->Write();

  cout << "done"<< endl;

  // Done.
  return 0;
}


