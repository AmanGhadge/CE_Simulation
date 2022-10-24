#include <iostream>
#include "generator.hh"
using namespace std;

MyPrimaryGenerator::MyPrimaryGenerator(/* args */)
{
    //primary vertex per event
    fParticleGun = new G4ParticleGun(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) 
{
    G4ParticleTable *particleTable = G4ParticleTable:: GetParticleTable();
    G4String particleName = "electron";
    G4ParticleDefinition *particle = particleTable->FindParticle("e-");
    //0.0575*m
    G4ThreeVector pos(0.,0.,4.3*cm);
    //G4float phi = 3.14/180.0 * (-30-rand()%120);
    G4float phi = 3.14/180.0 * (rand()%360);
    //G4float theta = 3.14/180.0 * (- rand()%181);
    G4float theta = 3.14/180.0 * (rand()%360);
    G4ThreeVector mom(cos(theta)*cos(phi),cos(theta)*sin(phi),sin(theta));

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    /*Energy of Particle = 100 keV*/
    //553.1726674*keV
    fParticleGun->SetParticleMomentum(300*keV);
    fParticleGun->SetParticleDefinition(particle);

    fParticleGun->GeneratePrimaryVertex(anEvent); 
}