#include "include/MMVII_all.h"

namespace MMVII
{


/* ============================================= */
/*      cDataMapping<Type>                       */
/* ============================================= */

     //  =========== Constructors =============

template <class Type,const int DimIn,const int DimOut> 
    cDataMapping<Type,DimIn,DimOut>::cDataMapping(const tPtIn & aEpsJac) :
       mEpsJac          (aEpsJac),
       mJacByFiniteDif  (mEpsJac.x()>0)
#if (! MAP_STATIC_BUF)
   ,  mBufIn1Val       ({tPtIn()})
#endif
{
    // BufIn1Val().clear();
    // BufIn1Val().push_back(tPtIn());
    // We need to at least be abble to put one point for finite diff
}

template <class Type,const int DimIn,const int DimOut> 
    cDataMapping<Type,DimIn,DimOut>::cDataMapping(const Type & aVal) :
    cDataMapping<Type,DimIn,DimOut>(tPtIn::PCste(aVal))
{
}


template <class Type,const int DimIn,const int DimOut> 
    cDataMapping<Type,DimIn,DimOut>::cDataMapping() :
    cDataMapping<Type,DimIn,DimOut>(Type(0.0))
{
}




     //  =========== Compute values =============

    ///   Buffered mode by default calls unbeferred mode
template <class Type,const int DimIn,const int DimOut> 
    const typename cDataMapping<Type,DimIn,DimOut>::tVecOut & 
                   cDataMapping<Type,DimIn,DimOut>::Values(tVecOut & aBufOut,const tVecIn & aVIn) const
{
/**/MACRO_CHECK_RECURS_BEGIN;
    for (const auto  & aP : aVIn)
        aBufOut.push_back(Value(aP));
/**/MACRO_CHECK_RECURS_END;
    return aBufOut;
}

template <class Type,const int DimIn,const int DimOut> 
    const typename cDataMapping<Type,DimIn,DimOut>::tVecOut & 
                   cDataMapping<Type,DimIn,DimOut>::Values(const tVecIn & aVIn) const
{
   return Values(BufOutCleared(),aVIn);
}




    ///   Unbeferred mode  by default calls buferred mode
template <class Type,const int DimIn,const int DimOut> 
    typename cDataMapping<Type,DimIn,DimOut>::tPtOut  
             cDataMapping<Type,DimIn,DimOut>::Value(const tPtIn & aPt) const
{
   BufIn1Val()[0] = aPt;
   const tVecOut & aRes = Values(BufIn1Val());
   return aRes[0];
}

     //  =========== Compute jacobian and vals =============

template <class Type,const int DimIn,const int DimOut> 
    typename cDataMapping<Type,DimIn,DimOut>::tVecJac & 
             cDataMapping<Type,DimIn,DimOut>::BufJac(tU_INT4 aSz) 
#if (!MAP_STATIC_BUF)
                                                                     const
#endif
{
#if (MAP_STATIC_BUF)
   cMemManager::SetActiveMemoryCount(false);  // static vector of shared matrix will never be unallocated
   static tVecJac  mJacReserve;
   static tVecJac  mJacResult;
#endif
   while (mJacReserve.size()<aSz)
         mJacReserve.push_back(tJac(DimIn,DimOut));
   // If too small
   for (tU_INT4 aK=mJacResult.size() ; aK<aSz ; aK++)
       mJacResult.push_back(mJacReserve[aK]);
   // If too big
   while (mJacResult.size()>aSz)
        mJacResult.pop_back();
#if (MAP_STATIC_BUF)
   cMemManager::SetActiveMemoryCount(true);
#endif
   return mJacResult;
}



    ///   Buffered mode by default calls finit difference OR  unbeferred mode 
template <class Type,const int DimIn,const int DimOut> 
    typename cDataMapping<Type,DimIn,DimOut>::tCsteResVecJac
                   cDataMapping<Type,DimIn,DimOut>::Jacobian(tResVecJac aRes,const tVecIn & aVIn) const
{
/**/MACRO_CHECK_RECURS_BEGIN;

    tU_INT4 aNbIn = aVIn.size();
    tVecOut & aJBufOut = *(aRes.first);
    tVecJac & aBufJac =  *(aRes.second);
/*
    tResVecJac aRes(&aJBufOut,&aBufJac);
*/
    // tResVecJac aRes(nullptr,nullptr);
    if (mJacByFiniteDif)
    {
       // tU_INT4 aNbPByJac = 1+2*DimIn;
       tU_INT4 aNbInBuf = std::max(tU_INT4(1),tU_INT4(aNbIn/(1+2*DimIn)));
       for (tU_INT4 aKpt0=0 ; aKpt0<aNbIn ; aKpt0+=aNbInBuf)
       {
          tU_INT4 aKpt1 = std::min(aKpt0+aNbInBuf,aNbIn);
          tVecIn& aBufIn = JBufInCleared();
          for (tU_INT4 aKpt=aKpt0 ; aKpt<aKpt1 ; aKpt++)
          {
              tPtIn aPK = aVIn[aKpt];
              aBufIn.push_back(aPK);
              for (int aD=0 ; aD<DimIn ; aD++)
              {
                  aPK[aD] -= mEpsJac[aD];
                  aBufIn.push_back(aPK);
                  aPK[aD] += 2.0*mEpsJac[aD];
                  aBufIn.push_back(aPK);
                  aPK[aD] -= mEpsJac[aD];
              }
          }
          const tVecOut & aResOut =   Values(aBufIn);
          int aInd = 0;
          for (tU_INT4 aKpt=aKpt0 ; aKpt<aKpt1 ; aKpt++)
          {
             aJBufOut.push_back(aResOut[aInd++]);
             for (int aD=0 ; aD<DimIn ; aD++)
             {
                tPtOut aPm = aResOut[aInd++];
                tPtOut aGrad = (aResOut[aInd++]-aPm) / (2.0*mEpsJac[aD]);

                SetCol(aBufJac[aKpt],aD,aGrad);
             }
          }
       }
    }
    else
    {
        for (tU_INT4 aKpt=0 ; aKpt<aNbIn ; aKpt++)
        {
           auto [aPt,aJac] = Jacobian(aVIn[aKpt]);
           aJBufOut.push_back(aPt);
           aBufJac[aKpt] = aJac;
        }
    }
/**/MACRO_CHECK_RECURS_END;
    return tCsteResVecJac(aRes.first,aRes.second);
}

template <class Type,const int DimIn,const int DimOut> 
    typename cDataMapping<Type,DimIn,DimOut>::tCsteResVecJac
                   cDataMapping<Type,DimIn,DimOut>::Jacobian(const tVecIn & aVIn) const
{
/**/MACRO_CHECK_RECURS_BEGIN;
    tU_INT4 aNbIn = aVIn.size();
    tVecOut & aJBufOut = JBufOutCleared();
    tVecJac & aBufJac = BufJac(aNbIn);
    tResVecJac aRes(&aJBufOut,&aBufJac);
/**/MACRO_CHECK_RECURS_END;
    return Jacobian(aRes,aVIn);
}

template <class Type,const int DimIn,const int DimOut> 
    typename cDataMapping<Type,DimIn,DimOut>::tResJac
                   cDataMapping<Type,DimIn,DimOut>::Jacobian(const tPtIn & aPtIn) const
{
   BufIn1Val()[0] = aPtIn;
   tCsteResVecJac  aResVec = Jacobian(BufIn1Val());
   return tResJac(aResVec.first->at(0),aResVec.second->at(0));
}

/* ============================================= */
/*                cMapping<Type>                 */
/* ============================================= */

template <class Type,const int DimIn,const int DimOut>  
       cMapping<Type,DimIn,DimOut>::cMapping(tDataMap * aDM) :
            mPtr    (aDM),
            mRawPtr (aDM)
{
}


/* ============================================= */
/*        cMappingIdentity<Type>                 */
/* ============================================= */


template <class Type,const int Dim>
    const typename cMappingIdentity<Type,Dim>::tVecPt & 
                   cMappingIdentity<Type,Dim>::Values(tVecPt & aVRes,const tVecPt & aVIn) const
{
   // tVecPt & aBufOut = this->BufOut();
   // aBufOut = aVIn;
   aVRes = aVIn;
   return aVRes;
}


template <class Type,const int Dim>
    typename cMappingIdentity<Type,Dim>::tPt 
                   cMappingIdentity<Type,Dim>::Value(const tPt & aPt) const
{
   return aPt;
}

     /* ============================================= */
     /* ============================================= */
     /* ============================================= */

#define INSTANCE_TWO_DIM_MAPPING(DIM1,DIM2)\
template class cDataMapping<double,DIM1,DIM2>;\
template class cMapping<double,DIM1,DIM2>;

#define INSTANCE_ONE_DIM_MAPPING(DIM)\
template class cMappingIdentity<double,DIM>;\
INSTANCE_TWO_DIM_MAPPING(DIM,2);\
INSTANCE_TWO_DIM_MAPPING(DIM,3);


INSTANCE_ONE_DIM_MAPPING(2)
INSTANCE_ONE_DIM_MAPPING(3)


/* ============================================= */
/* ============================================= */
/* ====                                      === */       
/* ====            CHECK/BENCH               === */ 
/* ====                                      === */
/* ============================================= */
/* ============================================= */


class cTestMapp 
{
   public :
       static cPt3dLR Image(const cPt2dLR & aP) 
       {
          float x=  0.3*aP.x() + 0.9*aP.y() + 0.1*sin(aP.y());
          float y=  aP.x() + 0.1*sin(2*aP.x());
          float z=  1/(1+Square(aP.x())+Square(aP.y()));
          return cPt3dLR (x,y,z);
       }
       static cDenseMatrix<tREAL16> Grad(const cPt2dLR & aP) 
       {
          double aDiv = Square(1+Square(aP.x())+Square(aP.y()));
          cPt3dLR aGx(0.3,1.0+0.2*cos(2*aP.x()),(-2*aP.x())/aDiv);
          cPt3dLR aGy(0.9+0.1*cos(aP.y()),0,(-2*aP.y())/aDiv);

          cDenseMatrix<tREAL16> aRes(2,3);
          SetCol(aRes,0,aGx);
          SetCol(aRes,1,aGy);
          return aRes;
       }
};

class cTestMap1 : public cDataMapping<tREAL16,2,3>
{
    public :
        cPt3dLR Value(const cPt2dLR & aP) const override {return cTestMapp::Image(aP);}
        cTestMap1() :
            cDataMapping<tREAL16,2,3>(cPt2dLR(1e-3,1e-3))
        {
        }
};

class cTestMap2 : public cDataMapping<tREAL16,2,3>
{
    public :
        const std::vector<cPt3dLR> & Values
                   (
                       std::vector<cPt3dLR> & aBufOut,
                       const std::vector<cPt2dLR> & aVIn
                   ) const override 
        {
            // std::vector<cPt3dLR> & aBufOut = *aRes;
            for (const auto & aP : aVIn)
                aBufOut.push_back(cTestMapp::Image(aP));
            return aBufOut;
        }
        cTestMap2() :
            cDataMapping<tREAL16,2,3>(cPt2dLR(1e-3,1e-3))
        {
        }
};


template <class TypeMap> void OneBenchMapping(cParamExeBench & aParam)
{

    for (int aKTest=0 ; aKTest<1000; aKTest++)
    {
        tU_INT4 aSzV = RandUnif_N(50);
        std::vector<cPt2dLR>  aVIn;
        std::vector<cPt3dLR>  aVOut;
        std::vector<cDenseMatrix<tREAL16>>  aVDif;
        for (tU_INT4 aKP=0 ; aKP<aSzV ; aKP++)
        {
             cPt2dLR aP= cPt2dLR::PRandC();
             aVIn.push_back(aP);
             aVOut.push_back(cTestMapp::Image(aP));
             aVDif.push_back(cTestMapp::Grad(aP));
        }
        TypeMap aMap;
        cDataMapping<tREAL16,2,3> * aPM = &aMap;
        // compute vector of input
        const auto & aVO2 = aPM->Values(aVIn);
        MMVII_INTERNAL_ASSERT_bench(aVOut.size()==aSzV,"Sz in BenchMapping");
        MMVII_INTERNAL_ASSERT_bench(aVO2.size() ==aSzV,"Sz in BenchMapping");

        // check vector  of input with grad + unbefferd
        for (tU_INT4 aKP=0 ; aKP<aSzV ; aKP++) 
        {
            MMVII_INTERNAL_ASSERT_bench(Norm2(aVOut[aKP] - aVO2[aKP])<1e-5,"Buf/UnBuf in mapping");
            MMVII_INTERNAL_ASSERT_bench(Norm2(aVOut[aKP] - aPM->Value(aVIn[aKP]) )<1e-5,"Buf/UnBuf in mapping");
        }

        // check jacobian
        auto [aVO3,aVG3] = aPM->Jacobian(aVIn);
        for (tU_INT4 aKP=0 ; aKP<aSzV ; aKP++)
        {
            MMVII_INTERNAL_ASSERT_bench(Norm2(aVOut[aKP]-(*aVO3)[aKP])<1e-5,"Val in Grad in mapping");
            MMVII_INTERNAL_ASSERT_bench(aVDif[aKP].L2Dist(aVG3->at(aKP))<1e-3,"Jacobian in mapping");
        }
    }
}

#if (0)

class cTestMapInv : public cDataIterInvertMapping<tREAL8,3>
{
    public :
/*  Initialisation a bit tricky, because class is its own rough invers and we must
   must avoid infinite recursion,  TO CHANGE LATER with a two step init ...
*/
       cTestMapInv(double aFx,double aFy,double aFz,double aFreqCos,double aMulCos,bool ForRoughInv=false) :
          cDataIterInvertMapping<tREAL8,3> 
          (
              cPt3dr::PCste(1e-3/std::max(1e-5,mFreqCos)),
              cMapping<tREAL8,3,3>(ForRoughInv?nullptr:new cTestMapInv(1.0/aFy,1.0/aFx,1.0/aFz,1.0,0.0,true)),
              1e-4,
              20
          ),
          mFx      (aFx),
          mFy      (aFy),
          mFz      (aFz),
          mFreqCos (aFreqCos),
          mMulCos  (aMulCos)
       {
       }

       cPt3dr Value(const cPt3dr & aP) const override 
       {
           return   cPt3dr
                    (
                        mFx * aP.y() + cos((aP.x()+aP.y())*mFreqCos)*mMulCos,
                        mFy * aP.x() + sin((aP.y()-aP.z())*mFreqCos)*mMulCos,
                        mFz * aP.z() + sin((1.0+aP.x()-aP.y()+aP.z())*mFreqCos)*mMulCos
                    );
       }
       double mFx;
       double mFy;
       double mFz;
       double mFreqCos;
       double mMulCos;
};


void BenchInvertMapping(cParamExeBench & aParam)
{
    // Check in pure linear case, the inverse is exact
    {
       cTestMapInv  aM1(0.3,4.1,2.2,1000.0,0.0);
       const cDataMapping<tREAL8,3,3> & aM2 = *(aM1.RoughInv());
       for (int aK=0; aK<1000 ; aK++)
       {
           cPt3dr aP1 = cPt3dr::PRandC()*100.0;
           cPt3dr aP12 = aM1.Value(aM2.Value(aP1));
           cPt3dr aP21 = aM2.Value(aM1.Value(aP1));
           MMVII_INTERNAL_ASSERT_bench(Norm2(aP1-aP12)<1e-5,"cTestMapInv rough inverse");
           MMVII_INTERNAL_ASSERT_bench(Norm2(aP1-aP21)<1e-5,"cTestMapInv rough inverse");
       }
    }

    for (int aKMap=0 ; aKMap<100 ; aKMap++)
    {
        double aFX = RandUnif_C_NotNull(1e-1) * 3.0;
        double aFY = RandUnif_C_NotNull(1e-1) * 3.0;
        double aFZ = RandUnif_C_NotNull(1e-1) * 3.0;
        double aFreq = RandUnif_C_NotNull((1e-1) * 3.0);
        double aFMin =std::min(std::abs(aFX),std::min(std::abs(aFY),std::abs(aFZ)));
        double aMulCos =  (aFMin / aFreq) * 0.2 * RandUnif_0_1();

       cTestMapInv  aM1(aFX,aFY,aFZ,aFreq,aMulCos);
/*
       cMapping     aMInv(new cTestMapInv(aM1.RoughInverse()));
       double aEpsInv = 1e-4;
       aM1.SetRoughInv(aMInv,aEpsInv,20);
*/
       cDataInvertibleMapping<tREAL8,3> * aPM1 = & aM1;


       tREAL8  aEpsInv = aM1.DTolInv();
       for (int aKP=0 ; aKP<100 ; aKP++)
       {
           cPt3dr aPt = cPt3dr::PRandC()*100.0;
           cPt3dr aPtD = aM1.Value(aPt);
           cPt3dr aPtDI = aM1.Inverse(aPtD);

           cPt3dr aPtI = aM1.Inverse(aPt);
           cPt3dr aPtID = aM1.Value(aPtI);

           MMVII_INTERNAL_ASSERT_bench(Norm2(aPt -aPtDI)<10*aEpsInv,"elem inverse");
           MMVII_INTERNAL_ASSERT_bench(Norm2(aPt -aPtID)<10*aEpsInv,"elem inverse");
       }

       for (int aKV=0 ; aKV<10 ; aKV++)
       {
           int aNb = RandUnif_N(100);
           std::vector<cPt3dr> aVIn;
           for (int aKP=0 ; aKP<aNb ; aKP++)
           {
              aVIn.push_back(cPt3dr::PRandC()*100.0);
           }
           std::vector<cPt3dr> aVOut =  aPM1->Values(aVIn);
           std::vector<cPt3dr> aVInv =  aPM1->Inverses(aVOut);
           for (int aKP=0 ; aKP<aNb ; aKP++)
           {
              double aD = Norm2(aVIn[aKP] - aVInv[aKP]);
              MMVII_INTERNAL_ASSERT_bench(aD<10*aEpsInv,"elem inverse");
// std::cout << "NNN: " << Norm2(aVIn[aKP] - aVInv[aKP]) << "\n";
           }
       }
    }
}
#endif

class cBugRecMap : public cDataMapping<tREAL8,2,2>
{
    public :
};

void BenchInvertMapping(cParamExeBench & aParam);
void BenchSymDerMap(cParamExeBench & aParam);

void BenchMapping(cParamExeBench & aParam)
{
    if (! aParam.NewBench("GenMapping")) return;

    
    if (aParam.GenerateBug("MapRecursion"))
    {
       cBugRecMap aMap;
       aMap.Value(cPt2dr(2,2));
    }

    BenchInvertMapping(aParam);
    BenchSymDerMap(aParam);

    OneBenchMapping<cTestMap1>(aParam);
    OneBenchMapping<cTestMap2>(aParam);
    aParam.EndBench();
}

/* ===================================================== */
/* =====              INSTANTIATION                ===== */
/* ===================================================== */


};
