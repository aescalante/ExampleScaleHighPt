#include <iostream>
#include <map>

float GeneralizedEndpointPt(float MuonPt, int MuonCharge, float MuonEta, float MuonPhi, int Mode, int verbose=0){

  //Check the input format.
  if (fabs(MuonEta)>2.4) {
    cerr<<"ERROR: MuonEta = "<< MuonEta << ", outisde valide range = [-2.4,2.4] "<<endl;
    return 0;
  }
  if (fabs(MuonPhi)>180) {
    cerr<<"ERROR: MuonPhi = "<< MuonPhi << ", outisde valide range = [-180,180] "<<endl;
    return 0;
  }
  if (MuonCharge != 1 && MuonCharge != -1) {
    cerr<<"ERROR: Invalide Muon Charge = "<< MuonCharge << endl;
    return 0;
  }
  if (Mode != 0 && Mode != 1 && Mode != 2 && Mode != -1 && Mode != -2 && Mode != -3) {
    cerr<<"ERROR: Invalide Mode = "<< Mode << endl;
    cerr<<"\t: Valid modes: 0 = Nominal, 1 = SystematicUp and 2 = SystematicDown "<< endl;
    cerr<<"\t: Experimental mode: -1 = Nominal for Data, -2 = SystematicUp and -3 = SystematicDown "<< endl;
    return 0;
  }

  // Eta Binning
  unsigned int etaBINS = 7;
  unsigned int kEtaBin = etaBINS;
  double EtaBin[etaBINS+1];
  EtaBin[0]=-2.4; EtaBin[1]=-2.1; EtaBin[2]=-0.9; EtaBin[3]=0.;
  EtaBin[4]=0.9; EtaBin[5]=1.2; EtaBin[6]=2.1; EtaBin[7]=2.4;  

  // Phi Binning.
  unsigned int phiBINS =3;
  unsigned int kPhiBin = phiBINS;
  double PhiBin[phiBINS+1];
  PhiBin[0]=-180.; PhiBin[1]=-60.; PhiBin[2]=60.; PhiBin[3]=180.;
  

  for (unsigned int kbin=0; kbin<=etaBINS; ++kbin) {
    if (MuonEta<EtaBin[kbin+1]) {
      kEtaBin = kbin;
      break;
    }
  }

  for (unsigned int kbin=0; kbin<=phiBINS; ++kbin) {
    if (MuonPhi<PhiBin[kbin+1]) {
      kPhiBin = kbin;
      break;
    }
  }

  std::map<int,std::map<int,float>> Correction;
  std::map<int,std::map<int,float>> CorrectionError;

  //Corrections from 2D matrix in MuonPOG presentation in c/TeV.
  //[-2.4, -2.1]
  Correction[0][0] = -0.401522; CorrectionError[0][0] = 0.142292;
  Correction[0][1] =  0.800000; CorrectionError[0][1] = 0.266103; 
  Correction[0][2] =  -0.235869; CorrectionError[0][2] =0.087648;
  //[-2.1, -0.9]
  Correction[1][0] =  -0.173451; CorrectionError[1][0] = 0.043914;
  Correction[1][1] =  0.066351; CorrectionError[1][1] = 0.065401;
  Correction[1][2] =  -0.151637; CorrectionError[1][2] = 0.076572;
  //[-0.9, 0.]
  Correction[2][0] =  0.058729; CorrectionError[2][0] = 0.059396;
  Correction[2][1] =  -0.040742; CorrectionError[2][1] = 0.053842;
  Correction[2][2] =  -0.079811; CorrectionError[2][2] = 0.057861;
  //[-0., 0.9]
  Correction[3][0] =  -0.081190; CorrectionError[3][0] = 0.061906;
  Correction[3][1] =  0.169463; CorrectionError[3][1] = 0.060391;
  Correction[3][2] =  -0.007521; CorrectionError[3][2] = 0.058256;
  //[0.9, 2.1]
  Correction[4][0] =  0.095130; CorrectionError[4][0] = 0.071207;
  Correction[4][1] =  0.238078; CorrectionError[4][1] = 0.056428;
  Correction[4][2] =  0.028462; CorrectionError[4][2] = 0.057563;
  //[2.1, 2.4]
  Correction[5][0] =  -0.494724; CorrectionError[5][0] = 0.141278;
  Correction[5][1] =  -0.091591; CorrectionError[5][1] = 0.135156;
  Correction[5][2] =  0.183099;  CorrectionError[5][2] = 0.182376;

  float KappaBias=Correction[kEtaBin][kPhiBin];
  float KappaBiasError=CorrectionError[kEtaBin][kPhiBin];

  MuonPt = MuonPt/1000.; //GeV to TeV.
  MuonPt = MuonCharge*fabs(MuonPt); //Signed Pt.

  if (Mode==1) KappaBias = KappaBias+KappaBiasError; //Take bias + UpSystematic.
  if (Mode==2) KappaBias = KappaBias-KappaBiasError; //Takes bias - DownSystematic.

  /// Experimental ///
  if (Mode==-1) KappaBias = -1*KappaBias; //Reverse the sign to use it with data as first approximation. (this option has some non-trivial assumptions).
  if (Mode==-2) KappaBias = -1*(KappaBias+KappaBiasError); //Reverse the sign to use it with data as first approximation. (this option has some non-trivial assumptions).
  if (Mode==-3) KappaBias = -1*(KappaBias-KappaBiasError); //Reverse the sign to use it with data as first approximation. (this option has some non-trivial assumptions).
  //////

  if (verbose ==1) printf("eta bin %i, phi bin %i Correction %f +- %f pt %f\n", kEtaBin, kPhiBin, KappaBias, KappaBiasError,MuonPt);

  MuonPt = 1/MuonPt; //Convert to Curvature.
  MuonPt = MuonPt + KappaBias; //Apply the bias.
  MuonPt = 1/MuonPt;//Return to Pt.
  MuonPt = fabs(MuonPt)*1000.;//Return to Pt.

  if (verbose ==1) printf("NEW pt %f\n", MuonPt);

  return MuonPt;
};
