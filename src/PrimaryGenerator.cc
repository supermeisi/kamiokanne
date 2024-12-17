#include "PrimaryGenerator.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    // Particle position
    G4double x = 0. * m;
    G4double y = 0. * m;
    G4double z = -12. * cm;

    G4ThreeVector pos(x, y, z);

    // Particle direction
    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 1.;

    G4ThreeVector mom(px, py, pz);

    // Particle type
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("mu+");

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(50. * MeV);
    fParticleGun->SetParticleDefinition(particle);
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}