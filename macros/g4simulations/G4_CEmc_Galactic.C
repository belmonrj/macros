int Min_cemc_layer = 1;
int Max_cemc_layer = 1;

#include <iostream>

// just a dummy parameter used by the tilted plate geom
void
CEmcInit(const int nslats = 1)
{
	  Min_cemc_layer = 1;
		  Max_cemc_layer = 1;
}


//! EMCal main setup macro
double
CEmc(PHG4Reco* g4Reco, double radius, const int crossings,
		    const int absorberactive = 0)
{
	double emc_inner_radius = 100.; // emc approx shower max 

	// swallow all particles that hit the emc
  PHG4CylinderSubsystem *galactic = new PHG4CylinderSubsystem("CEMC", 1);
  galactic->SuperDetector("CEMC");
  galactic->SetRadius(emc_inner_radius); // add 10 cm
  galactic->SetLengthViaRapidityCoverage(true);
	galactic->SetMaterial("G4_Galactic");
  galactic->SetThickness(0.1); // it needs some thickness
  galactic->SetActive(); // always see what leaks out
  galactic->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(galactic);
	// second layer of larger radius to trick as abosrber, though no hits in it.
	PHG4CylinderSubsystem *galactic = new PHG4CylinderSubsystem("ABSORBER_CEMC",1);
	galactic->SuperDetector("ABSORBER_CEMC");
  galactic->SetRadius(emc_inner_radius + 1); // add 10 cm
  galactic->SetLengthViaRapidityCoverage(true);
	galactic->SetMaterial("G4_Galactic");
  galactic->SetThickness(0.1); // it needs some thickness
  galactic->SetActive(); // always see what leaks out
  galactic->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(galactic);

}

void
CEMC_Cells(int verbosity = 0)
{
	gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
	PHG4CylinderCellReco *cemc_cells = new PHG4CylinderCellReco("CEMCCYLCELLRECO");
	cemc_cells->Detector("CEMC");
  cemc_cells->Verbosity(verbosity);
  for (int i = Min_cemc_layer; i <= Max_cemc_layer; i++)
	{	
		// cemc_cells->etaphisize(i, 0.024, 0.024);
		const double radius = 100;
		cemc_cells->cellsize(i, 2 * TMath::Pi() / 256. * radius,
				2 * TMath::Pi() / 256. * radius);
	}
	cemc_cells->set_timing_window_size(60);
	se->registerSubsystem(cemc_cells);
}

void
CEMC_Towers(int verbosity = 0)
{
	gSystem->Load("libfun4all.so");
	gSystem->Load("libg4detectors.so");
	Fun4AllServer *se = Fun4AllServer::instance();

	RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
	TowerBuilder->Detector("CEMC");
	TowerBuilder->set_sim_tower_node_prefix("SIM");
	TowerBuilder->Verbosity(verbosity);
	TowerBuilder->set_tower_energy_src(RawTowerBuilder::kEnergyDeposition);
	se->registerSubsystem(TowerBuilder);

	double sampling_fraction = 1;

	static const double photoelectron_per_GeV = 1; //500 photon per total GeV deposition

	RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer(
			"EmcRawTowerDigitizer");
	TowerDigitizer->Detector("CEMC");
	TowerDigitizer->Verbosity(verbosity);
	TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitalization);
	se->registerSubsystem(TowerDigitizer);

	/*
	RawTowerCalibration *TowerCalibration = new RawTowerCalibration(
			"EmcRawTowerCalibration");
	TowerCalibration->Detector("CEMC");
	TowerCalibration->Verbosity(verbosity);
	TowerCalibration->set_calib_algorithm(
			  RawTowerCalibration::kSimple_linear_calibration);
	TowerCalibration->set_calib_const_GeV_ADC(1. / sampling_fraction);
	TowerCalibration->set_pedstal_ADC(0);
	se->registerSubsystem(TowerCalibration);
	*/

	//
	// The RawTowerCalibrationSmear module does the same thing as the 
	// RawTowerCalibration module but includes a smearing in energy
	//
	RawTowerCalibrationSmear *TowerCalibration = new RawTowerCalibrationSmear(
			"EmcRawTowerCalibration");
	TowerCalibration->Detector("CEMC");
	TowerCalibration->Verbosity(verbosity);
	// set the width of em particle smear
	TowerCalibration->set_em_sigma_p0(0.11);
	TowerCalibration->set_em_sigma_p1(0.005);
	// set the width of hadron smear
	TowerCalibration->set_h_sigma_p0(0.175);
	TowerCalibration->set_h_sigma_p1(0.09);
	TowerCalibration->set_spread_e_thresh(1);
	TowerCalibration->set_zero_suppression_GeV(0.01);
	se->registerSubsystem(TowerCalibration);

	return;
}

void
CEMC_Clusters(int verbosity = 0)
{

	gSystem->Load("libfun4all.so");
	gSystem->Load("libg4detectors.so");
	Fun4AllServer *se = Fun4AllServer::instance();

	RawClusterBuilder* ClusterBuilder = new RawClusterBuilder(
			"EmcRawClusterBuilder");
	ClusterBuilder->Detector("CEMC");
	ClusterBuilder->Verbosity(verbosity);
	se->registerSubsystem(ClusterBuilder);

	return;
}

void
CEMC_Eval(std::string outputfile, int verbosity = 0)
{

	gSystem->Load("libfun4all.so");
	gSystem->Load("libg4eval.so");
	Fun4AllServer *se = Fun4AllServer::instance();

	CaloEvaluator* eval = new CaloEvaluator("CEMCEVALUATOR", "CEMC",
			outputfile.c_str());
	eval->Verbosity(verbosity);
	se->registerSubsystem(eval);

	return;
}
