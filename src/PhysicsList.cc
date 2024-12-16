#include "PMPhysicsList.hh"

PMPhysicsList::PMPhysicsList()
{
    // EM physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Optical physics
    RegisterPhysics(new G4OpticalPhysics());
}

PMPhysicsList::~PMPhysicsList()
{
}
