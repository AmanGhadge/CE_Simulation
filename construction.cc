#include "construction.hh"
#include "MagLensTabulatedField3D.hh"
#include<cmath>
#include"detector.hh"

 #define MAG 1

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
    G4Cons *shield = new G4Cons("shield",0.3*cm,2.42*cm,0.3*cm,1.16*cm,1.5*cm,0.*deg,360.*deg);
    G4LogicalVolume *logicShield = new G4LogicalVolume(shield,gammaShield,"logicShield");
    G4VPhysicalVolume *physShield = new G4PVPlacement(0,G4ThreeVector(0.,0.,0.),logicShield,"physShield",logicWorld,false,0,true);

    //X-Ray Shield-Sn
    G4Element *Sn = nist->FindOrBuildElement("Sn");
    G4Material *xrayShield1 = new G4Material("xrayShield1",5.75*g/cm3,1);
    xrayShield1->AddElement(Sn, 100*perCent);
    G4Tubs *xrayVol1 = new G4Tubs("xrayVol1",0.3*cm,1*cm,0.05*cm,0.,360.*deg);
    G4LogicalVolume *logicxrayShield1 = new G4LogicalVolume(xrayVol1,xrayShield1,"logicxrayShield1");
    G4VPhysicalVolume *physxrayShield1 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-1.45*cm),logicxrayShield1,"physxrayShield1",logicWorld,false,0,true);
    
    //X-Ray Shield-Cu
    G4Element *Cu = nist->FindOrBuildElement("Cu");
    G4Material *xrayShield2 = new G4Material("xrayShield2",8.96*g/cm3,1);
    xrayShield2->AddElement(Cu, 100*perCent);
    G4Tubs *xrayVol2 = new G4Tubs("xrayVol2",0.3*cm,1*cm,0.05*cm,0.,360.*deg);
    G4LogicalVolume *logicxrayShield2 = new G4LogicalVolume(xrayVol2,xrayShield2,"logicxrayShield2");
    G4VPhysicalVolume *physxrayShield2 = new G4PVPlacement(0,G4ThreeVector(0.,0.,-1.6*cm),logicxrayShield2,"physxrayShield2",logicWorld,false,0,true);


    //Detector
    G4Element *Si = nist->FindOrBuildElement("Si");
    G4Material *detectorMat = new G4Material("detectorMat",2.32*g/cm3,1);
    detectorMat->AddElement(Si, 100*perCent);
    G4Tubs* solidDetector = new G4Tubs("solidDetector", 0.8*cm, 4.8*cm,0.1*cm,0.,360.*deg); //Box to Tub
    logicDetector = new G4LogicalVolume(solidDetector, detectorMat, "logicDetector");

  
     G4VPhysicalVolume* physDetector = new G4PVPlacement(0,G4ThreeVector(0.*cm,0.*cm,-7.2*cm ),
               logicDetector, "physDetector",logicWorld, false,0,true);
    
      

    return physWorld;
}



void MyDetectorConstruction::ConstructSDandField() {
#if MAG
  if (fField.Get() == 0) {
    G4MagneticField* MagneticLens = new MagLensTabulatedField3D("/Users/rocky/np/fourproto/data120_20_40.TABLE");

    fField.Put(MagneticLens);

    G4FieldManager* FieldMgr = 
	  G4TransportationManager::GetTransportationManager()->GetFieldManager();

    FieldMgr->SetDetectorField(fField.Get());
    FieldMgr->CreateChordFinder(fField.Get());
  }
#endif
  MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

  logicDetector->SetSensitiveDetector(sensDet);
  
  }