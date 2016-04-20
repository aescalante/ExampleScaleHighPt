// * GeneralizedEndpoint.h
#include <iostream>
#include <map>
#include "TRandom3.h"

#ifndef GENERALIZEDENDPOINT_H_
#define GENERALIZEDENDPOINT_H_

class GeneralizedEndpoint {
 public:
   GeneralizedEndpoint();
   virtual ~GeneralizedEndpoint();
   float GeneralizedEndpointPt(float MuonPt, int MuonCharge, float MuonEta, float MuonPhi, int Mode, int verbose=0);
   bool MergeBins(){return _MergeBins=true;}; //if the set to true, average bins in eta <1.2 and phi.
 private:
   std::map<int,std::map<int,float> > _Correction;
   std::map<int,std::map<int,float> > _CorrectionError;   
   TRandom3 _RandomNumbers;
   bool _MergeBins;
};
#endif /* GENERALIZEDENDPOINT_H_ */
