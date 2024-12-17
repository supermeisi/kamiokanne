#ifndef PRIMARYGENERATOR_HH
#define PRIMARYGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"

class PrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGenerator();
    ~PrimaryGenerator();

    virtual void GeneratePrimaries(G4Event *);

private:
    G4ParticleGun *fParticleGun;
};

#endif