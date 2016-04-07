int Fun4All_G4_sPHENIX_FastSim(
			       const int jobNum = 0,
			       const int nEvents = 1
			       )
{

  //gSystem->Load("libfastsim_emcal_check.so");
  gSystem->Load("libSimpleTrackingAnalysis.so");

  TTimeStamp time;

  //  HIJING AuAu200
  const char* inputFile = Form("in/hijing_%05i.txt.bz2",jobNum);
  //  Full sim hits
  //const char* inputFile = "/sphenix/sim/sim01/production/2016-02-01/spacal1d/fieldmap/G4Hits_sPHENIX_pi-_eta0_10GeV-0000.root";
  // output
  const char* outputFile = Form("out/embedded_pi-_e-_HIJING_fastsim_%i_%05i.root",time.GetDate(),jobNum);
  //===============
  // Input options
  //===============

  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  const bool readhits = false;
  // Or:
  // read files in HepMC format (typically output from event generators like hijing or pythia)
  //const bool readhepmc = false; // don't read HepMC files
  const bool readhepmc = true; // read HepMC files
  // Or:
  // Use particle generator
  const bool runpythia = false;

  //======================
  // What to run
  //======================

  bool dopion = true;
  //bool doelectron = true;
  bool doelectron = false;


  bool do_bbc = true;

  bool do_pipe = true;

  bool do_svtx = true;
  bool do_svtx_cell = true;
  bool do_svtx_track = true;
  bool do_svtx_eval = false;

  bool do_preshower = true;

  bool do_cemc = true;
  bool do_cemc_cell = true;
  bool do_cemc_twr = true;
  bool do_cemc_cluster = true;
  bool do_cemc_eval = false;

  bool do_hcalin = true;
  bool do_hcalin_cell = true;
  bool do_hcalin_twr = true;
  bool do_hcalin_cluster = true;
  bool do_hcalin_eval = false;

  bool do_magnet = false;

  bool do_hcalout = true;
  bool do_hcalout_cell = true;
  bool do_hcalout_twr = true;
  bool do_hcalout_cluster = true;
  bool do_hcalout_eval = false;

  //bool do_global = true;
  bool do_global = false;
  bool do_global_fastsim = false;

  bool do_jet_reco = false;
  bool do_jet_eval = false;

  bool do_dst_compress = false;

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  bool do_DSTReader = false;
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libphhepmc.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4hough.so");
  gSystem->Load("libcemc.so");
  gSystem->Load("libg4eval.so");

  // establish the geometry and reconstruction setup
  //gROOT->LoadMacro("G4Setup_sPHENIX_og.C");

  gROOT->LoadMacro("G4Setup_sPHENIX_BH.C");
  //if ( doelectron ) G4FastSimFlag(1); // emc is a black hole
  if ( dopion ) G4FastSimFlag(2); // emc is galactic medium, inner hcal is black hole
  G4Init(do_svtx,do_preshower,do_cemc,do_hcalin,do_magnet,do_hcalout,do_pipe);

  int absorberactive = 1; // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = "/phenix/upgrades/decadal/fieldmaps/sPHENIX.2d.root"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const float magfield_rescale = 1.4/1.5; // scale the map to a 1.4 T field

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You ca neither set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  //rc->set_IntFlag("RANDOMSEED", 12345);

  //-----------------
  // Event generation
  //-----------------

  if (readhits)
    {
      // Get the hits from a file
      // The input manager is declared later
    }
  else if (readhepmc)
    {
      // cout << "reading the hepmc file..." << endl;
      // // this module is needed to read the HepMC records into our G4 sims
      // // but only if you read HepMC input files
      // // toss low multiplicity dummy events
      // PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      // if ( doelectron ) gen->add_particles("e-",2); // mu+,e+,proton,pi+,Upsilon
      // if ( dopion ) gen->add_particles("pi-",2); // mu+,e+,proton,pi+,Upsilon
      // //gen->add_particles("pi+",10); // mu-,e-,anti_proton,pi-
      // //gen->set_reuse_existing_vertex(true);
      // //gen->set_existing_vertex_offset_vector(0.0,0.0,0.0);
      // gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
      //   				    PHG4SimpleEventGenerator::Uniform,
      //   				    PHG4SimpleEventGenerator::Uniform);
      // gen->set_vertex_distribution_mean(0.0,0.0,0.0);
      // gen->set_vertex_distribution_width(0.0,0.0,5.0);
      // gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
      // gen->set_vertex_size_parameters(0.0,0.0);
      // gen->set_eta_range(0., 0.1);
      // gen->set_phi_range(0, 0.1);
      // //gen->set_phi_range(-1.0*TMath::Pi(), 1.0*TMath::Pi());
      // gen->set_pt_range(10, 10.1);
      // gen->Embed(1);
      // gen->Verbosity(0);
      // se->registerSubsystem(gen);
      // HepMCNodeReader *hr = new HepMCNodeReader();
      // se->registerSubsystem(hr);
      HepMCNodeReader *hr = new HepMCNodeReader();
      TRandom3* rnd = new TRandom3(0);
      hr->VertexPosition( 0, 0, rnd->Uniform(-10,10) );
      se->registerSubsystem(hr);

      // toss low multiplicity dummy events
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      // gen->add_particles("e+",5); // mu+,e+,proton,pi+,Upsilon
      // gen->add_particles("pi+",5); // mu-,e-,anti_proton,pi-
      // gen->add_particles("e-",5); // mu+,e+,proton,pi+,Upsilon
      // gen->add_particles("pi-",5); // mu-,e-,anti_proton,pi-
      if ( doelectron) gen->add_particles("e-",2); // mu+,e+,proton,pi+,Upsilon
      if ( doelectron) gen->add_particles("e+",2); // mu+,e+,proton,pi+,Upsilon
      if ( dopion ) gen->add_particles("pi-",2); // mu+,e+,proton,pi+,Upsilon
      if ( dopion ) gen->add_particles("pi+",2); // mu+,e+,proton,pi+,Upsilon
      // if ( dopion ) gen->add_particles("pi-",1); // mu+,e+,proton,pi+,Upsilon
      // if ( dopion ) gen->add_particles("pi+",1); // mu+,e+,proton,pi+,Upsilon
      if (readhepmc) {
	gen->set_reuse_existing_vertex(true);
	gen->set_existing_vertex_offset_vector(0.0,0.0,0.0);
      } else {
	gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
					       PHG4SimpleEventGenerator::Uniform,
					       PHG4SimpleEventGenerator::Uniform);
	gen->set_vertex_distribution_mean(0.0,0.0,0.0);
	gen->set_vertex_distribution_width(0.0,0.0,5.0);
      }
      gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
      gen->set_vertex_size_parameters(0.0,0.0);
      gen->set_eta_range(-1.0, 1.0);
      gen->set_phi_range(-1.0*TMath::Pi(), 1.0*TMath::Pi());
      //gen->set_pt_range(0.1, 10.0);
      gen->set_pt_range(2.0, 10.0); // maybe better for fast sim?
      gen->Embed(1);
      gen->Verbosity(0);
      se->registerSubsystem(gen);
    }
  else if (runpythia)
    {
      gSystem->Load("libPHPythia8.so");

      PHPythia8* pythia8 = new PHPythia8();
      // see coresoftware/generators/PHPythia8 for example config
      pythia8->set_config_file("phpythia8.cfg");
      se->registerSubsystem(pythia8);

      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);
    }
  else
    {
      // toss low multiplicity dummy events
      cout << "tossing test particles..." << endl;
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      // only small hits in the outer hcal...
      if ( doelectron) gen->add_particles("e-",2); // mu+,e+,proton,pi+,Upsilon
      if ( doelectron) gen->add_particles("e+",2); // mu+,e+,proton,pi+,Upsilon
      if ( dopion ) gen->add_particles("pi-",2); // mu+,e+,proton,pi+,Upsilon
      if ( dopion ) gen->add_particles("pi+",2); // mu+,e+,proton,pi+,Upsilon
      //gen->add_particles("e-",10); // mu-,e-,anti_proton,pi-
      //gen->add_particles("pi-",10); // mu-,e-,anti_proton,pi-
      gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
					    PHG4SimpleEventGenerator::Uniform,
					    PHG4SimpleEventGenerator::Uniform);
      gen->set_vertex_distribution_mean(0.0,0.0,0.0);
      gen->set_vertex_distribution_width(0.0,0.0,5.0);
      gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
      gen->set_vertex_size_parameters(0.0,0.0);
      gen->set_eta_range(0., 0.1);
      //gen->set_phi_range(0, 0.1);
      gen->set_phi_range(-1.0*TMath::Pi(), 1.0*TMath::Pi());
      gen->set_pt_range(1, 26);
      gen->Embed(1);
      gen->Verbosity(0);
      se->registerSubsystem(gen);
    }

  if (!readhits)
    {
      //---------------------
      // Detector description
      //---------------------

      G4Setup(absorberactive, magfield, TPythia6Decayer::kAll,
	      do_svtx, do_preshower, do_cemc, do_hcalin, do_magnet, do_hcalout, do_pipe, magfield_rescale);
    }

  //---------
  // BBC Reco
  //---------

  if (do_bbc)
    {
      gROOT->LoadMacro("G4_Bbc.C");
      BbcInit();
      Bbc_Reco();
    }
  //------------------
  // Detector Division
  //------------------

  if (do_svtx_cell) Svtx_Cells();

  if (do_cemc_cell) CEMC_Cells();

  if (do_hcalin_cell) HCALInner_Cells();

  if (do_hcalout_cell) HCALOuter_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------

  if (do_cemc_twr) CEMC_Towers();
  if (do_cemc_cluster) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------

  if (do_hcalin_twr) HCALInner_Towers();
  if (do_hcalin_cluster) HCALInner_Clusters();

  if (do_hcalout_twr) HCALOuter_Towers();
  if (do_hcalout_cluster) HCALOuter_Clusters();

  if (do_dst_compress) ShowerCompress();

  //--------------
  // SVTX tracking
  //--------------

  if (do_svtx_track) Svtx_Reco();

  //-----------------
  // Global Vertexing
  //-----------------

  if (do_global)
    {
      gROOT->LoadMacro("G4_Global.C");
      Global_Reco();
    }

  else if (do_global_fastsim)
    {
      gROOT->LoadMacro("G4_Global.C");
      Global_FastSim();
    }

  //---------
  // Jet reco
  //---------

  if (do_jet_reco)
    {
      gROOT->LoadMacro("G4_Jets.C");
      Jet_Reco();
    }
  //----------------------
  // Simulation evaluation
  //----------------------

  if (do_svtx_eval) Svtx_Eval("g4svtx_eval.root");

  if (do_cemc_eval) CEMC_Eval("g4cemc_eval.root");

  if (do_hcalin_eval) HCALInner_Eval("g4hcalin_eval.root");

  if (do_hcalout_eval) HCALOuter_Eval("g4hcalout_eval.root");

  if (do_jet_eval) Jet_Eval("g4jet_eval.root");

  //fastsim_emcal_check *perchk = new fastsim_emcal_check();
  //perchk->caloname = "HCALIN";

  cout << "Starting analysis module..." << endl;
  SimpleTrackingAnalysis *checker = new SimpleTrackingAnalysis();
  checker->set_nlayers(7); // nLayers is set in G4Setup_sPHENIX.C
  checker->set_verbosity(-10); // higher for testing, lower for running
  se->registerSubsystem(checker);

  //--------------
  // IO management
  //--------------

  if (readhits)
    {
      // Hits file
      Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
      hitsin->fileopen(inputFile);
      se->registerInputManager(hitsin);
    }
  if (readhepmc)
    {
      Fun4AllInputManager *in = new Fun4AllHepMCInputManager( "DSTIN");
      se->registerInputManager( in );
      se->fileopen( in->Name().c_str(), inputFile );
    }
  else
    {
      // for single particle generators we just need something which drives
      // the event loop, the Dummy Input Mgr does just that
      Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
      se->registerInputManager( in );
    }

  if (do_DSTReader)
    {
      //Convert DST to human command readable TTree for quick poke around the outputs
      gROOT->LoadMacro("G4_DSTReader.C");

      G4DSTreader( outputFile, //
          /*int*/ absorberactive ,
          /*bool*/ do_svtx ,
          /*bool*/ do_preshower ,
          /*bool*/ do_cemc ,
          /*bool*/ do_hcalin ,
          /*bool*/ do_magnet ,
          /*bool*/ do_hcalout ,
          /*bool*/ do_cemc_twr ,
          /*bool*/ do_hcalin_twr ,
          /*bool*/ do_magnet  ,
          /*bool*/ do_hcalout_twr
          );
    }

  // Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
  // if (do_dst_compress) DstCompress(out);
  // se->registerOutputManager(out);

  //-----------------
  // Event processing
  //-----------------
  if (nEvents < 0)
    {
      return;
    }
  // if we run the particle generator and use 0 it'll run forever
  if (nEvents == 0 && !readhits && !readhepmc)
    {
      cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
      cout << "it will run forever, so I just return without running anything" << endl;
      return;
    }

  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->dumpHistos(outputFile);

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
}
