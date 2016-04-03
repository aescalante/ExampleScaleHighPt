#include "include/GeneralizedEndpointPt.h"

void ExampleScaleCorrection(){

  //Example muon.

  float MuonPt= 500.; //Input pt in GeV
  int MuonCharge = 1; // Input charge +1 or -1.
  float MuonEta=0.9; //Muon Eta Signed [-2.4, 2.4] 
  float MuonPhi=150.; //Muon phi in degrees (-180, 180), Reminder if Radians use: MuonPhi = (MuonPhiInRad*180)/Pi.
  int mode=0; //0 Nominal, 1 Systematic Up, 2 systematic Down.


  /// To be applied on simulation only.

  //Get Nominal Correction.
  GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, mode);
  //Get SystematicUp Correction.
  GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, 1);
  //Get SystematicDown Correction.
  GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, 2);


  //For test.
  // GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, 220., mode);
  // GeneralizedEndpointPt(MuonPt, MuonCharge, 3.5, MuonPhi, mode);
  // GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, -2);
  // GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, -1);
  GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, 0, 1);

}
