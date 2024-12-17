#include "EventAction.hh"

EventAction::EventAction(RunAction *)
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event *event)
{
    G4cout << "Event number: " << event->GetEventID() << G4endl;
}

void EventAction::EndOfEventAction(const G4Event *event)
{
}