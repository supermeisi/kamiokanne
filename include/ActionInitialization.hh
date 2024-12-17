#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include "PrimaryGenerator.hh"
#include "RunAction.hh"
#include "EventAction.hh"

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
