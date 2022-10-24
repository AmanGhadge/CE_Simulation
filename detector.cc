#include"detector.hh"
#include <fstream>
#include <iostream>
#include <string>

int countDetect = 0;

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{
   std::cout << "Hello" << std::endl;
   std::ofstream outfile;
   outfile.open("/Users/rocky/np/fourproto/outputlog.txt");
   outfile << countDetect << std::endl;
   outfile.close();
}


G4bool MySensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *)
{

   G4Track *track = step->GetTrack();
   track->SetTrackStatus(fStopAndKill);

   G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
   G4int copyNo = touchable->GetVolume(0)->GetCopyNo();

   G4double edep = step->GetTotalEnergyDeposit();  
   //G4cout << "Energy is:" << edep << G4endl;
   countDetect += 1;
   G4cout << countDetect << G4endl;
   return true;
}
