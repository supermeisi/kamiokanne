#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char **argv)
{
    G4UIExecutive *ui;

#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager;
#else
    G4RunManager *runManger = new G4RunManager;
#endif

    // Detector construction
    runManager->SetUserInitialization(new DetectorConstruction());

    // Physics list
    runManager->SetUserInitialization(new PhysicsList());

    // Action initialization
    runManager->SetUserInitialization(new ActionInitialization());

    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    if (ui)
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    return 0;
}
