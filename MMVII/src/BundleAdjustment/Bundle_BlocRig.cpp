#include "BundleAdjustment.h"
#include "MMVII_BlocRig.h"


namespace MMVII
{

cBA_BlocRig::cBA_BlocRig
(
     const cPhotogrammetricProject &aPhProj,
     const std::vector<double> & aSigma
)  :
    mPhProj  (aPhProj),
    mBlocs   (aPhProj.ReadBlocCams()),  // use the standar interface to create the bloc
    mSigma   (aSigma),
    mAllPair (false),
    mEqBlUK  (EqBlocRig(true,1,true))  // get the class computing rigidity equation
{
    // push the weigth for the 3 equation on centers
    for (int aK=0 ; aK<3 ; aK++)
        mWeight.push_back(Square(1/mSigma.at(0)));
    
    // push the weigth for the 9 equation on rotation
    for (int aK=0 ; aK<9 ; aK++)
        mWeight.push_back(Square(1/mSigma.at(1)));
}

cBA_BlocRig::~cBA_BlocRig()
{
    DeleteAllAndClear(mBlocs);
}

void cBA_BlocRig::Save()
{
     for (const auto & aBloc : mBlocs)
	 mPhProj.SaveBlocCamera(*aBloc);
}

void cBA_BlocRig::AddCam (cSensorCamPC * aCam)
{
     size_t aNbAdd = 0;
     //  Parse all the bloc to try to add it
     for (const auto & aBloc : mBlocs)
     {
         // AddSensor return a boolean value indicating if it was added
         aNbAdd += aBloc->AddSensor(aCam);
     }
     // it may happen that a sensor does not belong to any bloc,
     // but a sensor may "never" belongs to several bloc
     if (aNbAdd>1)
     {
         MMVII_UnclasseUsEr("Multiple bloc for "+ aCam->NameImage());
     }

}


void cBA_BlocRig::AddToSys(cSetInterUK_MultipeObj<tREAL8> & aSet)
{
    //  "cSetInterUK_MultipeObj" is a structure that contains a set of
    //  unknowns, here we "declarate" all the unknwon, the object
    //  declared must derive from "cObjWithUnkowns". The "declaration" is
    //  made by calling "AddOneObj" in aSet
    //
    //  For each bloc, the unkowns are the "cPoseWithUK" contained in "mMapPoseUKInBloc"
    //

     //  .....
     // map all bloc
     //   map all pair of MapStrPoseUK
     //        add cPoseWithUK
}

/**  In a bundle adjusment its current that some variable are "hard" frozen, i.e they are
 *   considered as constant.  We could write specific equation, but generally it's more
 *   economic (from software devlopment) to just indicate this frozen part to the system.
 *
 *   Here the frozen unknown are the poses of the master camera of each bloc because the
 *   calibration is purely relative
 *
 */
void cBA_BlocRig::SetFrozenVar(cResolSysNonLinear<tREAL8> & aSys)  
{
     // ... parse all bloc
     {
	   
         //  ... extract the master

	 // The system handle the unknwon as integers,  the "object" (here aPose)
	 // "knows" the association between its member and local integers, that's why
	 // we pass object and members to do the job
	 //
	 //  ...  freeze the center
	 //  ... freez omega
     }
}

    // =========================================================
    //       The RigidityEquation itself
    // =========================================================

cPt3dr cBA_BlocRig::OnePairAddRigidityEquation(size_t aKS,size_t aKBl1,size_t aKBl2,cBlocOfCamera& aBloc,cResolSysNonLinear<tREAL8> & aSys)
{
    OrderMinMax(aKBl1,aKBl2); // not sure necessary, but prefer to fix arbitrary order
			    
    //  extract the sensor corresponding to the time and num of bloc
    cSensorCamPC* aCam1 = aBloc.CamKSyncKInBl(aKS,aKBl1);
    cSensorCamPC* aCam2 = aBloc.CamKSyncKInBl(aKS,aKBl2);


    // it may happen that some image are absent, non oriented ...
    if ((aCam1==nullptr) || (aCam2==nullptr)) return cPt3dr(0,0,0);

    // extract the unknown pose for each cam
    cPoseWithUK &  aPBl1 =  aBloc.PoseUKOfNumBloc(aKBl1);
    cPoseWithUK &  aPBl2 =  aBloc.PoseUKOfNumBloc(aKBl2);

    FakeUseIt(aPBl1); FakeUseIt(aPBl2);

    // We must create the observation/context of the equation; here we will push the coeef of matrix
    // for linearization 
    std::vector<double> aVObs;

    //   for the 4 pose use PushObs to  add it context in aVObs
    // ...

    // We must create a vector that contains all the global num of unknowns
    //
    // ...
    std::vector<int>  aVInd;

    // now we are ready to add the equation
    aSys.R_CalcAndAddObs
    (
          mEqBlUK,  // the equation itself
	  aVInd,
	  aVObs,
	  cResidualWeighterExplicit<tREAL8>(false,mWeight)
    );


    //  Now compute the residual  in Tr and Rot,
    //  ie agregate   (Rx,Ry,Rz, m00 , m01 ...)
    //
    //          ....
    //
    //  mEqBlUK->ValComp(0,aKU)

    return cPt3dr(0,0,1);
}


void cBA_BlocRig::OneBlAddRigidityEquation(cBlocOfCamera& aBloc,cResolSysNonLinear<tREAL8> & aSys)
{
     cPt3dr aRes(0,0,0);

     //  Parse all the bloc of image acquired at same sync time
     for (size_t  aKSync=0 ; aKSync<aBloc.NbSync() ; aKSync++)
     {
         // case "AllPair", to symetrise the problem we process all pair w/o distinguish master
         if (mAllPair)
	 {
            for (size_t aKBl1=0 ; aKBl1<aBloc.NbInBloc() ; aKBl1++)
            {
                for (size_t aKBl2=aKBl1+1 ; aKBl2<aBloc.NbInBloc() ; aKBl2++)
                {
                     aRes += OnePairAddRigidityEquation(aKSync,aKBl1,aKBl2,aBloc,aSys);
                }
            }
	 }
	 // other case, we only add the equation implyng the "master" camera
         else
         {
            size_t aKM = aBloc.IndexMaster();
            for (size_t aKBl=0 ; aKBl<aBloc.NbInBloc() ; aKBl++)
                if (aKBl!=aKM)
                   aRes += OnePairAddRigidityEquation(aKSync,aKM,aKBl,aBloc,aSys);
         }
     }

     aRes = aRes/aRes.z();

     StdOut() << "  Residual for Bloc : "  <<  aBloc.Name() << ", Tr=" << aRes.x() << ", Rot=" << aRes.y() << std::endl;
}

void cBA_BlocRig::AddRigidityEquation(cResolSysNonLinear<tREAL8> & aSys)
{

     for (const auto & aBloc : mBlocs)
         OneBlAddRigidityEquation(*aBloc,aSys);
}

};
