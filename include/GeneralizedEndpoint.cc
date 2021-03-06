#include "GeneralizedEndpoint.h"
#include <iostream>
#include <math.h>
using std::cout;
using std::cerr;
using std::endl;

GeneralizedEndpoint::GeneralizedEndpoint(){
  //Corrections from 2D matrix in MuonPOG presentation in c/TeV for tracker assignment.
  //[-2.4, -2.1]
  _Correction[0][0] = -0.401522; _CorrectionError[0][0] = 0.142292; //-180,-60 
  _Correction[0][1] =  0.800000; _CorrectionError[0][1] = 0.266103; //-60,60
  _Correction[0][2] =  -0.235869; _CorrectionError[0][2] = 0.087648; //60,180
  //[-2.1, -0.9]                                                                                           
  _Correction[1][0] =  -0.173451; _CorrectionError[1][0] = 0.043914;
  _Correction[1][1] =  0.066351; _CorrectionError[1][1] = 0.065401;
  _Correction[1][2] =  -0.151637; _CorrectionError[1][2] = 0.076572;
  //[-0.9, 0.]
  _Correction[2][0] =  0.058729; _CorrectionError[2][0] = 0.059396;
  _Correction[2][1] =  -0.040742; _CorrectionError[2][1] = 0.053842;
  _Correction[2][2] =  -0.079811; _CorrectionError[2][2] = 0.057861;
  //[-0., 0.9]
  _Correction[3][0] =  -0.081190; _CorrectionError[3][0] = 0.061906;
  _Correction[3][1] =  0.169463; _CorrectionError[3][1] = 0.060391;
  _Correction[3][2] =  -0.007521; _CorrectionError[3][2] = 0.058256;
  //[0.9, 2.1]
  _Correction[4][0] =  0.095130; _CorrectionError[4][0] = 0.071207;
  _Correction[4][1] =  0.238078; _CorrectionError[4][1] = 0.056428;
  _Correction[4][2] =  0.028462; _CorrectionError[4][2] = 0.057563;
  //[2.1, 2.4]
  _Correction[5][0] =  -0.494724; _CorrectionError[5][0] = 0.141278;
  _Correction[5][1] =  -0.091591; _CorrectionError[5][1] = 0.135156;
  _Correction[5][2] =  0.183099;  _CorrectionError[5][2] = 0.182376;

  // By default not to merge the bins
  _MergeBins = false;
  // By defatult not to smear central values
  _SmearCentralValue = false;
};

GeneralizedEndpoint::~GeneralizedEndpoint()
{
};

float GeneralizedEndpoint::GeneralizedEndpointPt(float MuonPt, int MuonCharge, float MuonEta, float MuonPhi, int Mode, int verbose){

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
  unsigned int etaBINS = 6;
  unsigned int kEtaBin = etaBINS;
  double EtaBin[etaBINS+1];
  EtaBin[0]=-2.4; EtaBin[1]=-2.1; EtaBin[2]=-1.2; EtaBin[3]=0.;
  EtaBin[4]=1.2; EtaBin[5]=2.1; EtaBin[6]=2.4;  

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

  float KappaBias=_Correction[kEtaBin][kPhiBin];
  float KappaBiasError=_CorrectionError[kEtaBin][kPhiBin];


  /// Insert simplifaction and average eta and phi bins.  
  if ( _MergeBins == true){
    KappaBias = 0.;
    if (fabs(MuonEta) < 1.2 ) KappaBiasError = 0.03;
    if (fabs(MuonEta) > 1.2 ) KappaBiasError = 0.04;
    // if (KappaBias > 0.03) KappaBias = 0.03;
    // if (KappaBias < -0.03) KappaBias = -0.03;
    if (verbose ==1) printf("Warning merged values are used for this correction, eta %f correction %f\n", MuonEta, KappaBias);
  }

  //Smear correction by the uncertainty. 
  if (_SmearCentralValue == true) {
    if (verbose ==1) printf("Before Smearing bias is %f +- %f \n", KappaBias, KappaBiasError);
    KappaBias = KappaBias + _RandomNumbers.Gaus(0, KappaBiasError);
    if (verbose ==1) printf("After Smearing bias is %f +- %f  \n",  KappaBias, KappaBiasError);
  }

  if (Mode==1) KappaBias = KappaBias+KappaBiasError; //Take bias + UpSystematic.
  if (Mode==2) KappaBias = KappaBias-KappaBiasError; //Takes bias - DownSystematic.

  /// Experimental ///
  if (Mode==-1) KappaBias = -1*KappaBias; //Reverse the sign to use it with data as first approximation. (this option has some non-trivial assumptions).
  if (Mode==-2) KappaBias = -1*(KappaBias+KappaBiasError); //Reverse the sign to use it with data as first approximation. (this option has some non-trivial assumptions).
  if (Mode==-3) KappaBias = -1*(KappaBias-KappaBiasError); //Reverse the sign to use it with data as first approximation. (this option has some non-trivial assumptions).
  //////

  if (verbose ==1) printf("eta bin %i, phi bin %i Correction %f +- %f pt %f\n", kEtaBin, kPhiBin, KappaBias, KappaBiasError,MuonCharge*MuonPt);

  MuonPt = MuonPt/1000.; //GeV to TeV.
  MuonPt = MuonCharge*fabs(MuonPt); //Signed Pt.
  MuonPt = 1/MuonPt; //Convert to Curvature.
  MuonPt = MuonPt + KappaBias; //Apply the bias.
  if (fabs(MuonPt) < 0.14 && verbose ==1)  printf("WARNING: Very small curvature after correction!(is this expected?) eta = %.2f, phi = %.2f \n", MuonEta, MuonPhi);
  if (fabs(MuonPt) < 0.14) MuonPt = KappaBiasError; //To avoid a division by set the curvature to its error if after the correction the pt is larger than 7 TeV.
  MuonPt = 1/MuonPt;//Return to Pt.
  MuonPt = fabs(MuonPt);//returns unsigned Pt, any possible sign flip due to the curvature is absorbed here.
  MuonPt = MuonPt*1000.;//Return to Pt in GeV.

  if (verbose ==1) printf("NEW pt %f\n", MuonPt);

  return MuonPt;
};
