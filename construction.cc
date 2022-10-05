#include "construction.hh"
#include "MagLensTabulatedField3D.hh"
#include<cmath>

MyDetectorConstruction::MyDetectorConstruction() 
{
  fField.Put(0);
}
MyDetectorConstruction::~MyDetectorConstruction() 
{}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
    //Dataset of Material
    G4NistManager *nist = G4NistManager::Instance();
    
    //Mother Volume
    //Vacuum material
    G4Material *vacuum = nist->FindOrBuildMaterial("G4_Galactic");

    //Defining structure of Material
    G4Box *solidWorld = new G4Box("solidWorld",0.3*m,0.3*m,0.4*m);

    //Integrating structure and material
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,vacuum,"logicWorld");

    //Creating the Physical Object
    //G4PVPlacement(rotation,location,logicVolume,name,MotherVolume,boolean-operation,no of extra copies,overlay-error checks if two volume intersect)
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicWorld,"physWorld",0,false,0,true);
    
    //Target
    G4Element *Pb = nist->FindOrBuildElement("Pb");
    G4Material *PbTarget = new G4Material("PbTarget",11.3*g/cm3,1);
    PbTarget->AddElement(Pb,100*perCent);
    G4Tubs *target = new G4Tubs("target",0.*cm,0.35*cm,0.1*cm,0.*deg,360.*deg);
    G4LogicalVolume *logicTarget = new G4LogicalVolume(target,PbTarget, "logicTarget");
    //G4VPhysicalVolume *physTarget = new G4PVPlacement(0,G4ThreeVector(0.,0.,5.75*cm),logicTarget,"physTarget",logicWorld,false,0,true);

    //gamma Shield
    G4Element *Ta = nist->FindOrBuildElement("Ta");
    G4Element *W = nist->FindOrBuildElement("W");
    G4Material *gammaShield = new G4Material("gammaShield",16.8*g/cm3,2);
    gammaShield->AddElement(Ta,97.5*perCent);
    gammaShield->AddElement(W,2.5*perCent);
    G4Cons *shield = new G4Cons("shield",0.3*cm,1*cm,0.3*cm,0.5*cm,1.25*cm,0.*deg,360.*deg);
    G4LogicalVolume *logicShield = new G4LogicalVolume(shield,gammaShield,"logicShield");
    G4VPhysicalVolume *physShield = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicShield,"physShield",logicWorld,false,0,true);

    //X-Ray Shield-Sn
    G4Element *Sn = nist->FindOrBuildElement("Sn");
    G4Material *xrayShield1 = new G4Material("xrayShield1",5.75*g/cm3,1);
    xrayShield1->AddElement(Sn, 100*perCent);
    G4Tubs *xrayVol1 = new G4Tubs("xrayVol1",0.3*cm,1*cm,0.2*cm,0.,360.*deg);
    G4LogicalVolume *logicxrayShield1 = new G4LogicalVolume(xrayVol1,xrayShield1,"logicxrayShield1");
    G4VPhysicalVolume *physxrayShield1 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-1.45*cm),logicxrayShield1,"physxrayShield1",logicWorld,false,0,true);
    
    //X-Ray Shield-Cu
    G4Element *Cu = nist->FindOrBuildElement("Cu");
    G4Material *xrayShield2 = new G4Material("xrayShield2",8.96*g/cm3,1);
    xrayShield2->AddElement(Cu, 100*perCent);
    G4Tubs *xrayVol2 = new G4Tubs("xrayVol2",0.3*cm,1*cm,0.05*cm,0.,360.*deg);
    G4LogicalVolume *logicxrayShield2 = new G4LogicalVolume(xrayVol2,xrayShield2,"logicxrayShield2");
    G4VPhysicalVolume *physxrayShield2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-1.5*cm),logicxrayShield2,"physxrayShield2",logicWorld,false,0,true);


    //Detector
    G4Element *Si = nist->FindOrBuildElement("Si");
    G4Material *detectorMat = new G4Material("detectorMat",2.32*g/cm3,1);
    detectorMat->AddElement(Si, 100*perCent);
    G4Tubs* solidDetector = new G4Tubs("solidDetector", 0.*cm, 1.1*cm,0.1*cm,0.,360.*deg); //Box to Tub
    G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

    for(G4int i=0; i<5; i++)
    { 
      G4float phi = i*(2*M_PI/5);
     G4VPhysicalVolume* physDetector = new G4PVPlacement(0,G4ThreeVector(1.9*cos(phi)*cm,1.9*sin(phi)*cm,-5.75*cm ),
               logicDetector, "physDetector",logicWorld, false,i,true);
    }
    
       // for (G4int i = 0; i < 4; i++) {
         //   G4float r = (((0.01 + (i + 1) * 0.08) + (0.01 + i * 0.08)) / 2);
           //  for (G4int j = 0; j < 8; j++) {
             //    G4float phi = (M_PI/8 + (j * M_PI/4));

               //G4VPhysicalVolume* physDetector = new G4PVPlacement(0, G4ThreeVector(0,0,-5.75*cm ),
             //  logicDetector, "physDetector",logicWorld, false, i*10 + j, true);
            // }
        // } 

       

    return physWorld;
}

/*
void MyDetectorConstruction::ConstructSDandField() {

    MySensitiveDetector* sensDet = new MySensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
}*/

void MyDetectorConstruction::ConstructSDandField() {
  if (fField.Get() == 0) {
    G4MagneticField* MagneticLens = new MagLensTabulatedField3D("/Users/rocky/np/tproto/data120_20_40.TABLE");

    fField.Put(MagneticLens);

    G4FieldManager* FieldMgr = 
	  G4TransportationManager::GetTransportationManager()->GetFieldManager();

    FieldMgr->SetDetectorField(fField.Get());
    FieldMgr->CreateChordFinder(fField.Get());
  }
}