#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

G4double PMDetectorConstruction::ConvertWavelengthToEnergy(G4double wavelength)
{
    const G4double h = 6.62607015e-34 * joule * s;
    const G4double c = 299792458 * m / s;

    G4double energy = (h * c) / (wavelength * nm);

    return energy / eV;
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *flaskMat = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4Material *waterMat = nist->FindOrBuildMaterial("G4_WATER");

    G4double photonEnergyMin = ConvertWavelengthToEnergy(800. * nm);
    G4double photonEnergyMax = ConvertWavelengthToEnergy(300. * nm);

    std::vector<G4double> photonEnergy = {photonEnergyMin * eV, photonEnergyMax * eV};
    std::vector<G4double> refIndexWorld = {1.0, 1.0};
    std::vector<G4double> refIndexWater = {1.33, 1.33};
    std::vector<G4double> refIndexFlask = {1.49, 1.49};
    std::vector<G4double> reflectivity = {0.9, 0.9};

    // Mirror surface
    G4OpticalSurface *mirrorSurface = new G4OpticalSurface("mirrorSurface");

    mirrorSurface->SetType(dielectric_metal);
    mirrorSurface->SetModel(unified);
    mirrorSurface->SetFinish(ground);
    mirrorSurface->SetSigmaAlpha(0.0001);

    // Material properties
    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", photonEnergy, refIndexWorld);
    worldMat->SetMaterialPropertiesTable(mptWorld);

    G4MaterialPropertiesTable *mptWater = new G4MaterialPropertiesTable();
    mptWater->AddProperty("RINDEX", photonEnergy, refIndexWater);
    waterMat->SetMaterialPropertiesTable(mptWater);

    G4MaterialPropertiesTable *mptFlask = new G4MaterialPropertiesTable();
    mptFlask->AddProperty("RINDEX", photonEnergy, refIndexFlask);
    flaskMat->SetMaterialPropertiesTable(mptFlask);

    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
    mptMirror->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);
    mirrorSurface->SetMaterialPropertiesTable(mptMirror);

    G4double xWorld = 0.5 * m;
    G4double yWorld = 0.5 * m;
    G4double zWorld = 0.5 * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // Water
    G4double waterLength = 10.0 * cm;
    G4double waterRadius = 5.0 * cm;

    G4Tubs *solidWater = new G4Tubs("solidDetector", 0.0, waterRadius, waterLength, 0., 360. * deg);
    G4LogicalVolume *logicWater = new G4LogicalVolume(solidWater, waterMat, "logicWater");
    G4VPhysicalVolume *physWater = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicWater, "physWater", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicWater->SetVisAttributes(detVisAtt);

    // Detector
    G4double PMTLength = 0.5 * cm;
    G4double PMTShift = waterLength + PMTLength;

    G4Tubs *solidPMT = new G4Tubs("solidPMT", 0.0, waterRadius, PMTLength, 0., 360. * deg);
    logicPMT = new G4LogicalVolume(solidPMT, waterMat, "logicPMT");
    G4VPhysicalVolume *physPMT = new G4PVPlacement(0, G4ThreeVector(0, 0, PMTShift), logicPMT, "physPMT", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *PMTVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    PMTVisAtt->SetForceSolid(true);
    logicPMT->SetVisAttributes(PMTVisAtt);

    // Flask
    G4double flaskLength = PMTShift + PMTLength;
    G4double flaskThickness = 0.5 * cm;
    G4double flaskRadius = waterRadius + flaskThickness;

    G4Tubs *solidFlask = new G4Tubs("solidPMT", waterRadius, flaskRadius, flaskLength, 0., 360. * deg);
    G4LogicalVolume *logicFlask = new G4LogicalVolume(solidFlask, flaskMat, "logicPMT");
    G4VPhysicalVolume *physFlask = new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * cm), logicFlask, "physFlask", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes *FlaskVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.5));
    FlaskVisAtt->SetForceSolid(true);
    logicFlask->SetVisAttributes(FlaskVisAtt);

    // Mirror surface
    G4LogicalBorderSurface *surfaceMirror = new G4LogicalBorderSurface("MirrorSurface", physWater, physFlask, mirrorSurface);

    // Endcaps
    G4double endcapLength = 0.25 * cm;
    G4double endcapShift = flaskLength + endcapLength;

    G4Tubs *solidEndcap = new G4Tubs("solidEndap", 0.0 * cm, flaskRadius, endcapLength, 0., 360. * deg);
    G4LogicalVolume *logicEndcap = new G4LogicalVolume(solidEndcap, flaskMat, "logicEndcap");
    G4VPhysicalVolume *physEndcap1 = new G4PVPlacement(0, G4ThreeVector(0, 0, endcapShift), logicEndcap, "physEndcap1", logicWorld, false, 0, checkOverlaps);
    G4VPhysicalVolume *physEndcap2 = new G4PVPlacement(0, G4ThreeVector(0, 0, -endcapShift), logicEndcap, "physEndcap2", logicWorld, false, 0, checkOverlaps);

    logicEndcap->SetVisAttributes(FlaskVisAtt);

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    PMSensitiveDetector *sensPMT = new PMSensitiveDetector("SensitivePMT");
    logicPMT->SetSensitiveDetector(sensPMT);

    G4SDManager::GetSDMpointer()->AddNewDetector(sensPMT);
}
