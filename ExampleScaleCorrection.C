#include "include/GeneralizedEndpoint.h"

int main () {
  //Random example muon.

  float MuonPt= 500.; //Input pt in GeV
  int MuonCharge = 1; // Input charge +1 or -1.
  float MuonEta=0.9; //Muon Eta Signed [-2.4, 2.4] 
  float MuonPhi=150.; //Muon phi in degrees (-180, 180), Reminder if Radians use: MuonPhi = (MuonPhiInRad*180)/Pi.
  int mode=0; //0 Nominal, 1 Systematic Up, 2 systematic Down.

  /// To be applied on simulation only.
  GeneralizedEndpoint ScaleSystematic;

  //Get Nominal Correction.
  ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, mode);
  //Get SystematicUp Correction.
  ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, 1);
  //Get SystematicDown Correction.
  ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, 2);  
  //Get Nominal Correction with verbose =1.
  ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, 0, 1);

  // //For test.
  // ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, 220., mode);
  // ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, 3.5, MuonPhi, mode);
  // ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, -2);
  // ScaleSystematic.GeneralizedEndpointPt(MuonPt, MuonCharge, MuonEta, MuonPhi, -1);

}
