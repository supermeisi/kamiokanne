#include "PhysicsList.hh"

PhysicsList::PhysicsList()
{
    // EM physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Optical physics
    RegisterPhysics(new G4OpticalPhysics());
}

PhysicsList::~PhysicsList()
{
}
