#include "include/MMVII_all.h"

using namespace NS_SymbolicDerivative ;


namespace MMVII
{

/* ============================================= */
/*      cDataMapCalcSymbDer<Type>                */
/* ============================================= */


template <class Type,const int DimIn,const int DimOut> 
  void  cDataMapCalcSymbDer<Type,DimIn,DimOut>::CheckDim(tCalc  * aCalc,bool Derive)
{
   MMVII_INTERNAL_ASSERT_strong(DimIn==aCalc->NbUk(),"Input dim in calculator");
   MMVII_INTERNAL_ASSERT_strong(DimOut==aCalc->NbElem(),"Output dim in calculator");
   MMVII_INTERNAL_ASSERT_strong(aCalc->WithDer()==Derive,"Derive dim in calculator");
}

template <class Type,const int DimIn,const int DimOut> 
  void  cDataMapCalcSymbDer<Type,DimIn,DimOut>::SetObs(const std::vector<Type> & aVObs)
{
   mVObs = aVObs;
}

template <class Type,const int DimIn,const int DimOut> 
  cDataMapCalcSymbDer<Type,DimIn,DimOut>::cDataMapCalcSymbDer
  (
         tCalc  * aCalcVal,
         tCalc  * aCalcDer,
         const std::vector<Type> & aVObs
  ) :
     mCalcVal  (aCalcVal),
     mCalcDer  (aCalcDer),
     mVObs     (aVObs)
{
    CheckDim(mCalcVal,false);
    CheckDim(mCalcDer,true);
    // mCalcVal.NbUk()
}


template <class Type,const int DimIn,const int DimOut> 
  const typename cDataMapCalcSymbDer<Type,DimIn,DimOut>::tVecOut &
      cDataMapCalcSymbDer<Type,DimIn,DimOut>::Values(tVecOut & aRes,const tVecIn & aVecIn) const
{
   aRes.clear();
   std::vector<Type> aVUk(DimIn);

   MMVII_INTERNAL_ASSERT_strong(mCalcVal->NbInBuf()==0,"Buff not empty");
   tU_INT4 aSzBuf = mCalcVal->SzBuf();
   for (tU_INT4 aK0=0 ;aK0<aVecIn.size() ; aK0+=aSzBuf)
   {
       tU_INT4 aK1 = std::min(tU_INT4(aVecIn.size()),aK0+aSzBuf);
       for (tU_INT4 aK=aK0 ; aK<aK1 ; aK++)
       {
           for (int aD=0 ; aD<DimIn ; aD++)
               aVUk[aD] = aVecIn[aK][aD];
           mCalcVal->PushNewEvals(aVUk,mVObs);
       }
       mCalcVal->EvalAndClear();
       for (tU_INT4 aK=aK0 ; aK<aK1 ; aK++)
       {
           tPtOut aPRes;
           for (int aD=0 ; aD<DimOut ; aD++)
           {
               aPRes[aD] = mCalcVal->ValComp(aK-aK0,aD);
           }
           aRes.push_back(aPRes);
       }
   }
   return aRes;
}

template <class Type,const int DimIn,const int DimOut> 
  typename cDataMapCalcSymbDer<Type,DimIn,DimOut>::tCsteResVecJac
      cDataMapCalcSymbDer<Type,DimIn,DimOut>::Jacobian(tResVecJac aRes,const tVecIn & aVecIn) const 
{
   tVecOut * aVecOut=aRes.first;
   tVecJac * aVecJac=aRes.second;

   std::vector<Type> aVUk(DimIn);

   MMVII_INTERNAL_ASSERT_strong(mCalcDer->NbInBuf()==0,"Buff not empty");
   tU_INT4 aSzBuf = mCalcDer->SzBuf();
   for (tU_INT4 aK0=0 ;aK0<aVecIn.size() ; aK0+=aSzBuf)
   {
       tU_INT4 aK1 = std::min(tU_INT4(aVecIn.size()),aK0+aSzBuf);
       for (tU_INT4 aK=aK0 ; aK<aK1 ; aK++)
       {
           for (int aD=0 ; aD<DimIn ; aD++)
               aVUk[aD] = aVecIn[aK][aD];
           mCalcDer->PushNewEvals(aVUk,mVObs);
       }

       mCalcDer->EvalAndClear();
       for (tU_INT4 aK=aK0 ; aK<aK1 ; aK++)
       {
           tPtOut aPRes;
           for (int aDOut=0 ; aDOut<DimOut ; aDOut++)
           {
               tPtIn aLineJac;
               aPRes[aDOut] = mCalcDer->ValComp(aK-aK0,aDOut);
               for (int aDIn=0 ; aDIn<DimIn ; aDIn++)
               {
                  aLineJac[aDIn] = mCalcDer->DerComp(aK-aK0,aDOut,aDIn);
               }
               SetLine(aDOut,(*aVecJac)[aK],aLineJac);
               // SetCol((*aVecJac)[aK],aDOut,aLineJac);
           }
           aVecOut->push_back(aPRes);
       }
   }
   return tCsteResVecJac(aVecOut,aVecJac);
}

template class cDataMapCalcSymbDer<tREAL8,2,2> ;
 // cDataIIMFromMap<Type,Dim>;


/*
template <class Type,const int Dim> class  cDataDistCloseId : cDataIterInvertMapping<Type,Dim>
{
     public :
       cDataDistCloseId(
     private :
};
*/




/* ============================================= */
/*      cDataMapCalcSymbDer<Type>                */
/* ============================================= */
cRandInvertibleDist::~cRandInvertibleDist()
{
   delete mEqVal;
   delete mEqDer;
}

cRandInvertibleDist::cRandInvertibleDist(const cPt3di & aDeg,double aRhoMax,double aProbaNotNul,double aTargetSomJac) :
   mRhoMax  (aRhoMax),
   mDeg     (aDeg),
   mEqVal   (EqDist(aDeg,false,1+RandUnif_N(50))),  // Random for size buf
   mEqDer   (EqDist(aDeg,true,1+RandUnif_N(50))),
   mNbParam (mEqVal->NbObs()),
   mVParam  (mNbParam,0.0),
   mVecDesc (DescDist(mDeg))
{
   // 1- Initialize, without precautions

   double aSomJac=0.0;  //  sum of jacobian
   while (aSomJac==0.0)
   {
      for (int aKPar=0 ; aKPar<mNbParam ;aKPar++)
      {
         double aMajNorm =  mVecDesc.at(aKPar).MajNormJacOfRho(mRhoMax);
         double aV = RandUnif_C() * (RandUnif_0_1() < aProbaNotNul) /aMajNorm;
         mVParam[aKPar] = aV;
         aSomJac += std::abs(aV) * aMajNorm;
      }
   }
   aSomJac = std::max(aSomJac,1e-5) ; // dont divide 0 if allmost null

   for (int aKPar=0 ; aKPar<mNbParam ;aKPar++)
   {
       mVParam[aKPar] *=  aTargetSomJac / aSomJac;
       if (false) // Print params
          std::cout << "KPar " << mEqVal->NamesObs().at(aKPar) << " : "  << mVParam[aKPar] << "\n";
   }
}

cDataMapCalcSymbDer<double,2,2> * cRandInvertibleDist::MapDerSymb()
{
   return new cDataMapCalcSymbDer<double,2,2>(mEqVal,mEqDer,mVParam);
}

const std::vector<double> & cRandInvertibleDist::VParam() const
{
   return mVParam;
}


cCalculator<double> &  cRandInvertibleDist::EqVal() {return *mEqVal;}
cCalculator<double> &  cRandInvertibleDist::EqDer() {return *mEqDer;}


/* ============================================= */
/*                   TEST                        */
/* ============================================= */

bool BUGINVMAP =false;

void TestJacob(cDataMapCalcSymbDer<double,2,2> * aMCS,const cPt2dr & aP)
{
double aEps= 1e-5;
// cPt2dr aP0 = aMCS->cDataMapping<double,2,2>::Value(aP) ;
   cPt2dr aPPx = aMCS->Value(aP +cPt2dr(aEps,0.0)) ;
   cPt2dr aPmx = aMCS->Value(aP +cPt2dr(-aEps,0.0)) ;
   cPt2dr aPPy = aMCS->Value(aP +cPt2dr(0.0,aEps)) ;
   cPt2dr aPmy = aMCS->Value(aP +cPt2dr(0.0,-aEps)) ;

double aRho = Norm2(aP);
StdOut() << " GX: " << (aPPx-aPmx)/(2*aEps)
         << " GY: " << (aPPy-aPmy)/(2*aEps)
         << " 7R6 " << 7 * pow(aRho,6)
         << "\n";
}


void BenchSymDerMap(cParamExeBench & aParam)
{
   cPt3di aDeg(3,1,1);
   // const std::vector<cDescOneFuncDist>  & aVecD =  DescDist(aDeg);

   for (int aKTest=0 ; aKTest<100 ; aKTest++)
   {
       double aRhoMax = 5 * (0.01 +  RandUnif_0_1());
       double aProbaNotNul = 0.1 + (0.4 *RandUnif_0_1());
       double aTargetSomJac = 0.2;
       cRandInvertibleDist aRID(aDeg,aRhoMax,aProbaNotNul,aTargetSomJac) ;
       cDataMapCalcSymbDer<double,2,2> * aMCS = aRID.MapDerSymb();
       cMapping<double,2,2>            aMapCS(aMCS);

       auto aDId = new cMappingIdentity<double,2> ;
       cMapping<double,2,2>            aMapId(aDId);

       cDataIIMFromMap<double,2> aIMap(aMapCS,aMapId,1e-6,15);

       // Generate in VIn a random set, with random size, of points in disk of radius aRhoMax
       int aNbPts  = 1+RandUnif_N(100);
       std::vector<cPt2dr> aVIn;
       for (int aKPts=0 ; aKPts<aNbPts ; aKPts++)
       {
           aVIn.push_back(cPt2dr::PRandInSphere() * aRhoMax );
       }

       // Check that Map(Map-1(P)) = P
       const std::vector<cPt2dr> & aVInv = aIMap.cDataInvertibleMapping<double,2>::Inverses(aVIn);
       double aMaxD=0.0;  //  Max | Map(Map-1(P)) -P|
       double aMaxDisto=0.0;  // Max | Map-1(aP) -P| , in case we need to evaluate the dist
       for (int aKPts=0 ; aKPts<int(aVIn.size()) ; aKPts++)
       {
           cPt2dr aDif =  aVIn[aKPts] - aMCS->Value(aVInv[aKPts]) ;
           aMaxD = std::max(aMaxD,Norm2(aDif));
           aMaxDisto = std::max(aMaxDisto,Norm2(aVIn[aKPts]-aVInv[aKPts]));
 
           // StdOut() << "Kp: " << aKPts << " PTS "<< aVIn[aKPts]<< " Dif " << aDif << "\n";
           // TestJacob(aMCS,aVInv[aKPts]);

       }
       if (aMaxD>1e-5)
       {
            StdOut() << "MOYD " << aMaxD  << " " << aMaxDisto << "\n";
            MMVII_INTERNAL_ASSERT_bench(false,"Distorsion inverse");
       }
   }

}

};
