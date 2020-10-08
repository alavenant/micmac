/*Header-MicMac-eLiSe-25/06/2007

    MicMac : Multi Image Correspondances par Methodes Automatiques de Correlation
    eLiSe  : ELements of an Image Software Environnement

    www.micmac.ign.fr


    Copyright : Institut Geographique National
    Author : Marc Pierrot Deseilligny
    Contributors : Gregoire Maillet, Didier Boldo.

[1] M. Pierrot-Deseilligny, N. Paparoditis.
    "A multiresolution and optimization-based image matching approach:
    An application to surface reconstruction from SPOT5-HRS stereo imagery."
    In IAPRS vol XXXVI-1/W41 in ISPRS Workshop On Topographic Mapping From Space
    (With Special Emphasis on Small Satellites), Ankara, Turquie, 02-2006.

[2] M. Pierrot-Deseilligny, "MicMac, un lociel de mise en correspondance
    d'images, adapte au contexte geograhique" to appears in
    Bulletin d'information de l'Institut Geographique National, 2007.

Francais :

   MicMac est un logiciel de mise en correspondance d'image adapte
   au contexte de recherche en information geographique. Il s'appuie sur
   la bibliotheque de manipulation d'image eLiSe. Il est distibue sous la
   licences Cecill-B.  Voir en bas de fichier et  http://www.cecill.info.


English :

    MicMac is an open source software specialized in image matching
    for research in geographic information. MicMac is built on the
    eLiSe image library. MicMac is governed by the  "Cecill-B licence".
    See below and http://www.cecill.info.

Header-MicMac-eLiSe-25/06/2007*/


#include "SAT4GEO.h"


cCommonAppliSat3D::cCommonAppliSat3D() :
	mExe(true),
	mDir("./"),
	mSH(""),
	mExpTxt(false),
	mFilePairs("Pairs.xml"),
	mOutRPC("EpiRPC"),
	mDegreRPC(0),
	mZoom0(64),
	mZoomF(1),
	mNameEpiLOF("EpiListOfFile.xml"),
	mOutSMDM("Fusion/"),
	mArgCommon(new LArgMain)
{
	*mArgCommon
			<< EAM(mDir,"Dir",true,"Current directory, Def=./")
			<< EAM(mFilePairs,"Pairs",true,"File with overlapping pairs, Def=Pairs.xml")
			<< EAM(mDoIm,"DoEpi",true,"Epipolar rectification, Def=true")
			<< EAM(mDegreEpi,"DegreeEpi",true,"Epipolar rectification: polynomial degree, Def=9")
			<< EAM(mDir1,"Dir1",true,"Epipolar rectification: Direction of Epip one (when Ori=NONE)")
			<< EAM(mDir2,"Dir2",true,"Epipolar rectification: Direction of Epip two (when Ori=NONE)")
			<< EAM(mSH,"SH",true,"Epipolar rectification: Prefix Homologue (when Ori=NONE), Def=\"\"")
			<< EAM(mExpTxt,"ExpTxt",true,"Epipolar rectification: Homol in text format? (when Ori=NONE), Def=\"false\"")
			<< EAM(mNbZ,"NbZ",true,"Epipolar rectification: Number of Z, def=1 (NbLayer=1+2*NbZ)")
			<< EAM(mNbZRand,"NbZRand",true,"Epipolar rectification: Number of additional random Z in each bundle, Def=1")
			<< EAM(mIntZ,"IntZ",true,"Epipolar rectification: Z interval, for test or correct interval of RPC")
			<< EAM(mNbXY,"NbXY",true,"Epipolar rectification: Number of point / line or col, def=100")
			<< EAM(mNbCalcDir,"NbCalcDir",true,"Epipolar rectification: Calc directions : Nbts / NbEchDir")
			<< EAM(mExpCurve,"ExpCurve",true,"Epipolar rectification: 0-SzIm ,1-Number of Line,2- Larg (in [0 1]),3-Exag deform,4-ShowOut")
			<< EAM(mOhP,"OhP",true,"Epipolar rectification: Oh's method test parameter")
			<< EAM(mOutRPC,"OutRPC",true,"RPC recalculation: Output RPC orientation directory (after rectification)")
			<< EAM(mDegreRPC,"DegreRPC",true,"RPC recalculation: Degree of RPC polynomial correction, Def=0")
			<< EAM(mChSys,"ChSys",true,"RPC recalculation: File specifying a euclidean projection system of your zone")
			<< EAM(mZoom0,"Zoom0",true,"Image matching: Zoom Init (Def=64)")
			<< EAM(mZoomF,"ZoomF",true,"Image matching: Zoom Final (Def=1)")
			<< EAM(mCMS,"CMS",true,"Image matching: Multi Scale Correl (Def=ByEpip)")
			<< EAM(mDoPly,"DoPly",true,"Image matching: Generate Ply")
			<< EAM(mRegul,"ZReg",true,"Image matching: Regularisation factor (Def=0.05)")
			<< EAM(mDefCor,"DefCor",true,"Image matching: Def cor (Def=0.5)")
			<< EAM(mSzW0,"SzW0",true,"Image matching: Sz first Windows, def depend of NbS (1 MS, 2 no MS)")
			<< EAM(mCensusQ,"CensusQ",true,"Image matching: Use Census Quantitative")
			<< EAM(mOutSMDM,"OutSMDM",true,"Depth map fusion: Name of the output folder, Def=Fusion/")
			<< EAM(mExe,"Exe",true,"Execute all, Def=true")
			;

	mICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);

	StdCorrecNameOrient(mOutRPC,mDir,true);
}

LArgMain & cCommonAppliSat3D::ArgCom()
{
	return * mArgCommon;
}


std::string cCommonAppliSat3D::ComParamPairs()
{
	std::string aCom;
	aCom += aCom + " Out=" + mFilePairs;

	return aCom;
}

std::string cCommonAppliSat3D::ComParamEpip()
{
    std::string aCom;
    if (EAMIsInit(&mExpTxt))    aCom += aCom   + " ExpTxt=" + ToString(mExpTxt);
    if (EAMIsInit(&mSH))        aCom +=  " NameH=" + mSH;
    if (EAMIsInit(&mDegreEpi))  aCom +=  " Degre=" + ToString(mDegreEpi);
    if (EAMIsInit(&mDegreEpi))  aCom +=  " Degre=" + ToString(mDegreEpi);
    if (EAMIsInit(&mDir1))      aCom +=  " Dir1=" + ToString(mDir1);
    if (EAMIsInit(&mDir2))      aCom +=  " Dir2=" + ToString(mDir2);

    if (EAMIsInit(&mNbZ))       aCom +=  " NbZ=" + ToString(mNbZ);
    if (EAMIsInit(&mNbZRand))   aCom +=  " NbZRand=" + ToString(mNbZRand);
    if (EAMIsInit(&mIntZ))      aCom +=  " IntZ=" + ToString(mIntZ);
    if (EAMIsInit(&mNbXY))      aCom +=  " NbXY=" + ToString(mNbXY);
    if (EAMIsInit(&mNbCalcDir)) aCom +=  " NbCalcDir=" + ToString(mNbCalcDir);
    if (EAMIsInit(&mExpCurve))  aCom +=  " ExpCurve=" + ToString(mExpCurve);
    if (EAMIsInit(&mOhP))       aCom +=  " Oh=" + ToString(mOhP);

    return aCom;
}

std::string cCommonAppliSat3D::ComParamRPC()
{
	std::string aCom;
	if (EAMIsInit(&mDegreRPC))  aCom += aCom  + " Degre=" + ToString(mDegreRPC);
	if (EAMIsInit(&mChSys))     aCom += " ChSys=" + mChSys;
//	if (EAMIsInit(&mOutRPC))     aCom += " OutRPC=" + mOutRPC;

	return aCom;
}
 

std::string cCommonAppliSat3D::ComParamMatch()
{
	std::string aCom;
    if (EAMIsInit(&mExpTxt))  aCom += aCom  + " ExpTxt=" + ToString(mExpTxt);
    if (EAMIsInit(&mZoom0))   aCom +=  " Zoom0=" + ToString(mZoom0);
    if (EAMIsInit(&mZoomF))   aCom +=  " ZoomF=" + ToString(mZoomF);
    if (EAMIsInit(&mCMS))     aCom +=  " CMS=" + ToString(mCMS);
    if (EAMIsInit(&mDoPly))   aCom +=  " DoPly=" + ToString(mDoPly);
    if (EAMIsInit(&mRegul))   aCom +=  " Regul=" + ToString(mRegul);
    if (EAMIsInit(&mDefCor))  aCom +=  " DefCor=" + ToString(mDefCor);
    if (EAMIsInit(&mSzW0))    aCom +=  " SzW0=" + ToString(mSzW0);
    if (EAMIsInit(&mCensusQ)) aCom +=  " CensusQ=" + ToString(mCensusQ);

	return aCom;
}

std::string cCommonAppliSat3D::ComParamFuse()
{
	std::string aCom;
	if (EAMIsInit(&mOutRPC))     aCom += " OutRPC=" + mOutRPC;


	return aCom;
}


bool cSomSat::HasInter(const cSomSat & aS2) const
{
    if (mCam && (aS2.mCam))
    {
         const cElPolygone &  aPol1= mCam->EmpriseSol();
         const cElPolygone &  aPol2= aS2.mCam->EmpriseSol();
         const cElPolygone &  aInter = aPol1 * aPol2; 
	

         if (aInter.Surf() <= 0) 
		 	return false;

    }

    return true;
}



cGraphHomSat::cGraphHomSat(int argc,char ** argv) :
      mOut       ("Pairs.xml"),
      mAltiSol   (0)
{

	

    ElInitArgMain
    (
    	argc,argv,
        LArgMain()  << EAMC(mDir,"Directory", eSAM_IsDir)
                    << EAMC(mPat,"Images pattern", eSAM_IsPatFile)
                    << EAMC(mOri,"Orientation dir", eSAM_IsExistFile),
        LArgMain()  << EAM(mAltiSol,"AltiSol",true)
                    << EAM(mOut,"Out",true)

    );
    if (!MMVisualMode)
    {
		cTplValGesInit<std::string>  aTplFCND;
        mICNM = cInterfChantierNameManipulateur::StdAlloc(argc,argv,mDir,aTplFCND);
 
		StdCorrecNameOrient(mOri,mDir,true);
 
        mLFile =  mICNM->StdGetListOfFile(mPat,1);
 
        mNbSom =  (int)mLFile.size();
 
        std::cout << "Nb Images = " <<  mNbSom << "\n";

        int aCpt = 0;
		for
        (
             std::list<std::string>::const_iterator itS=mLFile.begin();
             itS!=mLFile.end();
             itS++
        )
        {

			
			CameraRPC * aCam = new CameraRPC(mICNM->StdNameCamGenOfNames(mOri,*itS));
			Pt3dr aC = aCam->OrigineProf();

			mVC.push_back(new cSomSat(*this,*itS,aCam,aC));
            std::cout << "Load  : remain " << (mNbSom-aCpt) << " to do\n";


			aCpt++;
		}



		cSauvegardeNamedRel aRel;
        for (int aK1=0 ; aK1<mNbSom ; aK1++)
        {
            for (int aK2=aK1+1 ; aK2<mNbSom ; aK2++)
            {
                 if (mVC[aK1]->HasInter(*(mVC[aK2])))
                 {
                    aRel.Cple().push_back(cCpleString(mVC[aK1]->mName,mVC[aK2]->mName));
                 }

            }
            std::cout << "Graphe : remain " << (mNbSom-aK1) << " to do\n";
        }
        MakeFileXML(aRel,mDir+mOut);


	}
}

int GraphHomSat_main(int argc,char** argv)
{
	cGraphHomSat aGHS(argc,argv);

	return EXIT_SUCCESS;
}


cAppliCreateEpi::cAppliCreateEpi(int argc, char** argv)
{
	ElInitArgMain
    (
         argc,argv,
         LArgMain()  << EAMC(mFilePairs,"List of overlapping image pairs",eSAM_IsExistFile)
                     << EAMC(mOri,"Orientation directory, (NONE if rectification from tie-points)",eSAM_IsDir),
         LArgMain()
                     << mCAS3D.ArgCom()
    );

	
	StdCorrecNameOrient(mOri,mCAS3D.mDir,true);

	cSauvegardeNamedRel aPairs = StdGetFromPCP(mCAS3D.mDir+mFilePairs,SauvegardeNamedRel);
	
	std::list<std::string> aLCom;

	for (auto itP : aPairs.Cple())
	{
		std::string aComTmp = MMBinFile(MM3DStr) + "CreateEpip " 
						      + itP.N1() + BLANK + itP.N2() + BLANK + mOri + BLANK 
							  + mCAS3D.ComParamEpip();

		aLCom.push_back(aComTmp);
	}


	if (mCAS3D.mExe)
		cEl_GPAO::DoComInParal(aLCom);
    else
	{
		for (auto iCmd : aLCom)
      		std::cout << "SUBCOM= " << iCmd << "\n";
	}



}

int CPP_AppliCreateEpi_main(int argc,char ** argv)
{
	cAppliCreateEpi anAppCreatEpi(argc,argv);

	return EXIT_SUCCESS;
}



cAppliRecalRPC::cAppliRecalRPC(int argc, char** argv)
{
    ElInitArgMain
    (
         argc,argv,
         LArgMain()  << EAMC(mCAS3D.mFilePairs,"List of overlapping image pairs",eSAM_IsExistFile)
		 			 << EAMC(mOri,"RPC original orientation (dir)",eSAM_IsDir), 
					 //mOri needed to recover the names of the Epi images

         LArgMain()
                     << mCAS3D.ArgCom()
    );

	StdCorrecNameOrient(mOri,mCAS3D.mDir,true);
    
	cSauvegardeNamedRel aPairs = StdGetFromPCP(mCAS3D.mDir+mCAS3D.mFilePairs,SauvegardeNamedRel);

    std::list<std::string> aLCom;

    for (auto itP : aPairs.Cple())
    {
        std::string aNAppuisI1 = mCAS3D.mICNM->NameAppuiEpip(mOri,itP.N1(),itP.N2());
        std::string aNAppuisI2 = mCAS3D.mICNM->NameAppuiEpip(mOri,itP.N2(),itP.N1());

        std::string aNI1 = mCAS3D.mICNM->NameImEpip(mOri,itP.N1(),itP.N2());
        std::string aNI2 = mCAS3D.mICNM->NameImEpip(mOri,itP.N2(),itP.N1());

		

        std::string aComI1 = MMBinFile(MM3DStr) + "Convert2GenBundle "
                              + aNI1 + BLANK + aNAppuisI1 + BLANK
							  + mCAS3D.mOutRPC.substr(0,mCAS3D.mOutRPC.size()-1) 
                              + mCAS3D.ComParamRPC();

        std::string aComI2 = MMBinFile(MM3DStr) + "Convert2GenBundle "
                              + aNI2 + BLANK + aNAppuisI2 + BLANK
							  + mCAS3D.mOutRPC.substr(0,mCAS3D.mOutRPC.size()-1)
                              + mCAS3D.ComParamRPC();


		std::string aKeyGB = "NKS-Assoc-Im2GBOrient@-" +  mCAS3D.mOutRPC.substr(0,mCAS3D.mOutRPC.size()-1);

		std::string aNameGBI1 = mCAS3D.mICNM->Assoc1To1(aKeyGB,aNI1,true);
		std::string aNameGBI2 = mCAS3D.mICNM->Assoc1To1(aKeyGB,aNI2,true);

		std::string aComConv1 = MMBinFile(MM3DStr) + "Satelib RecalRPC " 
							  + aNameGBI1 + " OriOut=" + mCAS3D.mOutRPC.substr(mCAS3D.mOutRPC.size()-1);
		std::string aComConv2 = MMBinFile(MM3DStr) + "Satelib RecalRPC " 
							  + aNameGBI2 + " OriOut=" + mCAS3D.mOutRPC.substr(mCAS3D.mOutRPC.size()-1);



        aLCom.push_back(aComI1);
        aLCom.push_back(aComI2);
		aLCom.push_back(aComConv1);
		aLCom.push_back(aComConv2);

		// remove tmp dir
		aLCom.push_back("rm -r Ori-" + mCAS3D.mOutRPC.substr(0,mCAS3D.mOutRPC.size()-1));
    }


    if (mCAS3D.mExe)
        cEl_GPAO::DoComInSerie(aLCom);
    else
    {
        for (auto iCmd : aLCom)
            std::cout << "SUBCOM= " << iCmd << "\n";
    }
}

int CPP_AppliRecalRPC_main(int argc,char ** argv)
{
    cAppliRecalRPC anAppRRPC(argc,argv);

    return EXIT_SUCCESS;
}



/*
 * If mOri is not initialized, then no convention and
 *        epipolar image names (EINs) are taken from mFilePairs 
 * If mOri is initialized, then MicMac convention and 
 *        EINs will be deduced from their original names in mFilesPairs + Ori
 * */
cAppliMM1P::cAppliMM1P(int argc, char** argv)
{
	ElInitArgMain
    (
         argc,argv,
         LArgMain()  << EAMC(mFilePairs,"List of overlapping image pairs",eSAM_IsExistFile),
         LArgMain()  << EAM(mOri,"Ori",true,"RPC original orientation")
					 //mOri maye be needed to recover the names of the Epi images
                     << mCAS3D.ArgCom()
    );

	if (EAMIsInit(&mOri))
		StdCorrecNameOrient(mOri,mCAS3D.mDir,true);
    
	cSauvegardeNamedRel aPairs = StdGetFromPCP(mCAS3D.mDir+mFilePairs,SauvegardeNamedRel);

    std::list<std::string> aLCom;

    for (auto itP : aPairs.Cple())
    {
		std::string aNI1;
		std::string aNI2;

		if (EAMIsInit(&mOri))
		{
			aNI1 = mCAS3D.mICNM->NameImEpip(mOri,itP.N1(),itP.N2());
			aNI2 = mCAS3D.mICNM->NameImEpip(mOri,itP.N2(),itP.N1());
		}
		else
		{
			aNI1 = itP.N1();
			aNI2 = itP.N2();
		}

		std::string aComTmp = MMBinFile(MM3DStr) + "MM1P "
                              + aNI1 + BLANK + aNI2 + " NONE "
                              + mCAS3D.ComParamMatch();

        aLCom.push_back(aComTmp);
    }

    if (mCAS3D.mExe)
        cEl_GPAO::DoComInSerie(aLCom);
    else
    {
        for (auto iCmd : aLCom)
            std::cout << "SUBCOM= " << iCmd << "\n";
    }


}

int CPP_AppliMM1P_main(int argc,char ** argv)
{
	cAppliMM1P anAppMM1P(argc,argv);
	
	return EXIT_SUCCESS;
}


cAppliFusion::cAppliFusion(int argc,char ** argv)
{
	ElInitArgMain
    (
         argc,argv,
         LArgMain()  << EAMC(mFilePairs,"List of overlapping image pairs",eSAM_IsExistFile),
         LArgMain()  << EAM(mOri,"Ori",true,"RPC original orientation")
                     //mOri may be needed to recover the names of the Epi images
                     << mCAS3D.ArgCom()
    );

	if (EAMIsInit(&mOri))
    	StdCorrecNameOrient(mOri,mCAS3D.mDir,true);

	ELISE_ASSERT(EAMIsInit(&mCAS3D.mOutRPC),"In TestLib SAT4GEO_Fuse the \"OutRPC\" must be initializes. We need to know the geolocation of your input depth maps to move them to a common reference frame!")
}


std::string cAppliFusion::PxZName(const std::string & aInPx)
{
	return StdPrefix(aInPx) + "_FIm1ZTerr.tif";
}

std::string cAppliFusion::NuageZName(const std::string & aInNuageProf)
{
	return StdPrefix(aInNuageProf) + "_FIm1ZTerr.xml";
}

/*
 * If mOri is not initialized, then no convention and
 *        epipolar image names (EINs) are taken from mFilePairs
 * If mOri is initialized, then MicMac convention and
 *        EINs will be deduced from their original names in mFilesPairs + Ori
 * */
void cAppliFusion::DoAll()
{
	cSauvegardeNamedRel aPairs = StdGetFromPCP(mCAS3D.mDir+mFilePairs,SauvegardeNamedRel);

	/* Key to retrieve MEC2Im directory name */
	std::string aKeyMEC2Im = "Key-Assoc-MEC-Dir";


	/* Create xml file list with the concerned epipolar images */	
	cListOfName 				aLON;
	std::map<std::string,int>	aMEp;//serves to save uniquely all epip images
	std::list<std::pair<std::string,std::string>>  aLP;

	int aCpt=0;
    for (auto itP : aPairs.Cple())
    {
        std::string aNI1;
        std::string aNI2;
	
	 	if (EAMIsInit(&mOri))
		{	
			aNI1 = mCAS3D.mICNM->NameImEpip(mOri,itP.N1(),itP.N2());
        	aNI2 = mCAS3D.mICNM->NameImEpip(mOri,itP.N2(),itP.N1());
		}
		else
		{
			aNI1 = itP.N1();
			aNI2 = itP.N2();
		}

		aLP.push_back(std::make_pair(aNI1,aNI2));
		//aLP.push_back(std::make_pair(aNI2,aNI1));


		if (!DicBoolFind(aMEp,aNI1))
		{
			aMEp[aNI1] = aCpt; 
    		aLON.Name().push_back(aNI1);
		}
		aCpt++;
		
		if (!DicBoolFind(aMEp,aNI2))
		{
			aMEp[aNI2] = aCpt;
    		aLON.Name().push_back(aNI2);
		}
		aCpt++;


	}
	if (mCAS3D.mExe)
    	MakeFileXML(aLON,mCAS3D.mNameEpiLOF);


	/* Define the global frame of the reconstruction */
	std::string aCom = MMBinFile(MM3DStr) + "Malt UrbanMNE " 
			             + "NKS-Set-OfFile@" + mCAS3D.mNameEpiLOF + BLANK 
						 + mCAS3D.mOutRPC + " DoMEC=0";

	
    if (mCAS3D.mExe)
		System(aCom);
    else
    {
        std::cout << "SUBCOM1= " << aCom << "\n";
    }
	


	/* Transform surfaces geometries 
	 * from eGeomPxBiDim to  eGeomMNTFaisceauIm1ZTerrain_Px1D */
	std::list<std::string> aLCTG;
	bool aAffineLast = false;
	int aNbEtape = 1 // Num premiere etape
			     + round_ni(log2(mCAS3D.mZoom0/ mCAS3D.mZoomF))
				 //+ 1   //  Dulication de pas a la premier
				 + (aAffineLast ? 1 : 0)  ;  // Raffinement de pas;


	std::string aNuageInName = "NuageImProf_LeChantier_Etape_" + ToString(aNbEtape) + ".xml";
	std::string aPxInName = "Px1_Num" + ToString(aNbEtape) 
			              + "_DeZoom" + ToString(mCAS3D.mZoomF) 
						  + "_LeChantier.tif";//not coherent with reality but it does not matter

	for (auto itP : aLP)
    {
		std::string aMECDir = mCAS3D.mICNM->Assoc1To2(aKeyMEC2Im,itP.first,itP.second,true);



		//collect cmd to do conversion in parallel
		std::string aCTG = MMBinFile(MM3DStr) + "TestLib TransGeom "
				         + mCAS3D.mDir + " " 
						 + itP.first + " "
						 + itP.second + " " 
						 + mCAS3D.mOutRPC + " "
						 + aMECDir+aNuageInName + " "
						 + aMECDir+NuageZName(aNuageInName) + " "
						 + aMECDir+PxZName(aPxInName);



		aLCTG.push_back(aCTG);

	}

    if (mCAS3D.mExe)
		cEl_GPAO::DoComInParal(aLCTG);
    else
    {
        for (auto iCmd : aLCTG)
            std::cout << "SUBCOM2= " << iCmd << "\n";
    }



	/* Transform individual surfaces to global frame */
	std::list<std::string> aLCom;

	std::string aNuageOutName = "NuageImProf_STD-MALT_Etape_" + ToString(aNbEtape+1) + ".xml";
	std::string aPref = "DSM_Pair";
	if (mCAS3D.mExe)
			ELISE_fp::MkDirSvp(mCAS3D.mOutSMDM);	
	aCpt=0;


	for (auto itP : aLP)
	{
		std::string aMECDir = mCAS3D.mICNM->Assoc1To2(aKeyMEC2Im,itP.first,itP.second,true);
		
		std::string aComFuse = MMBinFile(MM3DStr) + "NuageBascule " 
				             + aMECDir + NuageZName(aNuageInName) + " " 
							 + "MEC-Malt/" + aNuageOutName + " " 
							 + mCAS3D.mOutSMDM + aPref + ToString(aCpt) + ".xml"; 

		aCpt++;

		aLCom.push_back(aComFuse);
	}	

    if (mCAS3D.mExe)
		cEl_GPAO::DoComInSerie(aLCom);
    else
    {
        for (auto iCmd : aLCom)
            std::cout << "SUBCOM3= " << iCmd << "\n";
    }



	/* Fusion */
	std::string aComMerge = MMBinFile(MM3DStr) + "SMDM " 
			             + mCAS3D.mOutSMDM + aPref + ".*xml";


    if (mCAS3D.mExe)
		System(aComMerge);
    else
    {
        std::cout << "SUBCOM4= " << aComMerge << "\n";
    }

}

int CPP_AppliFusion_main(int argc,char ** argv)
{

	cAppliFusion aAppFus(argc,argv);
	aAppFus.DoAll();

	return EXIT_SUCCESS;
}


cAppliSat3DPipeline::cAppliSat3DPipeline(int argc,char** argv) :
	mDebug(false)

{
   ElInitArgMain
   (
        argc,argv,
        LArgMain()  << EAMC(mPat,"Pattern of images",eSAM_IsPatFile)
					<< EAMC(mOri,"Orientation directory",eSAM_IsDir),  
        LArgMain()

                    << EAM(mDebug, "Debug", true, "Debug mode, def false")
					<< mCAS3D.ArgCom()
   );

	StdCorrecNameOrient(mOri,mCAS3D.mDir,true);
}


void cAppliSat3DPipeline::StdCom(const std::string & aCom,const std::string & aPost)
{
	std::string  aFullCom = MMBinFile(MM3DStr) +  aCom + BLANK;
    aFullCom = aFullCom + aPost;


    if (mCAS3D.mExe)
       System(aFullCom);
    else
       std::cout << "COM= " << aFullCom << "\n";

    std::cout << " DONE " << aCom << " in time " << mChrono.uval() << "\n";
}

void cAppliSat3DPipeline::DoAll()
{


	/********************************/
	/* 1- Calculate the image pairs */
	/********************************/
	if (! EAMIsInit(&mCAS3D.mFilePairs)) 
	{
		StdCom("TestLib SAT4GEO_Pairs", 
			    mCAS3D.mDir + BLANK + QUOTE(mPat) + BLANK + mOri +  
				mCAS3D.ComParamPairs());
	}
	else
		std::cout << mCAS3D.mFilePairs << " used." << "\n";





	/****************************************************/
	/* 2- Rectify pairs of images to epipolar geometry  */
	/****************************************************/
	if (mCAS3D.mDoIm == true)
		StdCom("TestLib SAT4GEO_CreateEpip", 
				mCAS3D.mFilePairs + BLANK + mOri + BLANK + 
				mCAS3D.ComParamEpip());
	else
		std::cout << "No epipolar image creation." << "\n";



	/**************************************/
	/* 3- Recalculate the RPC orientation */
	/**************************************/
	StdCom("TestLib SAT4GEO_EpiRPC", mCAS3D.mFilePairs + BLANK + mOri
								    + mCAS3D.ComParamRPC());
	


	/******************************************************/
	/* 4- Perform dense image matching per pair of images */
	/******************************************************/
	StdCom("TestLib SAT4GEO_MM1P", 
			mCAS3D.mFilePairs + BLANK + "Ori=" + mOri + BLANK + mCAS3D.ComParamMatch());



	/**************************************************************************/
	/* 5- Transform the per-pair reconstructions to a commont reference frame 
	 *    and do the 3D fusion */
	/**************************************************************************/
	StdCom("TestLib SAT4GEO_Fuse", mCAS3D.mFilePairs + BLANK + "Ori=" + mOri 
								 + mCAS3D.ComParamFuse());  



}



/*******************************************/
/********* CPP_TransformGeom    ************/
/*******************************************/
int CPP_TransformGeom_main(int argc, char ** argv)
{

	std::string aDir;
	std::string aOri;
	std::string aNuageName;
	std::string aNuageNameOut;
	std::string aIm1;
	std::string aIm2;
	std::string aPx1NameOut;
	std::string aPx1MasqName;

	ElInitArgMain
   	(
        argc,argv,
        LArgMain()  << EAMC(aDir,"Current directory")
					<< EAMC(aIm1,"First (left) image")
					<< EAMC(aIm2,"Second (right) image")
                    << EAMC(aOri,"Orientation directory",eSAM_IsDir)
	   				<< EAMC(aNuageName,"XML NuageImProf file")
					<< EAMC(aNuageNameOut,"XML NuageImProf output file")
					<< EAMC(aPx1NameOut,"Depth map output name, e.g., Px1Z.tif"),
        LArgMain()  << EAM(aPx1MasqName,"Mask",true,"Depth map's mask (if exits)")

   	);

	cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(aDir);
    StdCorrecNameOrient(aOri,aDir,true);

	// Directory of the depth map
	std::string aDirMEC = DirOfFile(aNuageName);

	/* Read the depth map */
	cXML_ParamNuage3DMaille  aNuageIn = StdGetObjFromFile<cXML_ParamNuage3DMaille>
           								(
                							aNuageName,
                							StdGetFileXMLSpec("SuperposImage.xml"),
                							"XML_ParamNuage3DMaille",
                							"XML_ParamNuage3DMaille"
           								);
	
	cImage_Profondeur aImProfPx   = aNuageIn.Image_Profondeur().Val();

	std::string aImName = aDirMEC + aImProfPx.Image();
	Tiff_Im     aImProfPxTif(aImName.c_str());
	Pt2di       aSz = aImProfPxTif.sz();

	TIm2D<float,double> aImProfPxTIM(aSz);
	ELISE_COPY
    (
           aImProfPxTIM.all_pts(),
           aImProfPxTif.in(),
           aImProfPxTIM.out()
    );



	/* Read cameras */
	cBasicGeomCap3D * aCamI1 = aICNM->StdCamGenerikOfNames(aOri,aIm1);	
	cBasicGeomCap3D * aCamI2 = aICNM->StdCamGenerikOfNames(aOri,aIm2);	


	/* Read the mask */
	Im2D_Bits<1> aMasq(aSz.x,aSz.y,1);
	if (EAMIsInit(&aPx1MasqName))
	{
		ELISE_COPY
		(			aMasq.all_pts(),
					Tiff_Im(aPx1MasqName.c_str()).in(),
				   	aMasq.out()
		);
	}
	TIm2DBits<1> aTMasq(aMasq);


	/* Create the depth map to which we will write */
	TIm2D<float,double> aImProfZTIM(aSz);

	/* Triangulate and fill in aImProfZTIM */
	for (int aK1=0; aK1<aSz.x; aK1++)
	{
		for (int aK2=0; aK2<aSz.y; aK2++)
		{
			Pt2di aPt1(aK1, aK2);
			
			if (aTMasq.get(aPt1))
			{

				Pt2dr aPt2(aK1 + aImProfPxTIM.get(aPt1), aK2);
                
				ElSeg3D aSeg1 = aCamI1->Capteur2RayTer(Pt2dr(aPt1.x,aPt1.y)); 
				ElSeg3D aSeg2 = aCamI2->Capteur2RayTer(aPt2); 
                
				std::vector<ElSeg3D> aVSeg = {aSeg1,aSeg2};
				
				Pt3dr aRes =  ElSeg3D::L2InterFaisceaux(0,aVSeg,0);
                
				aImProfZTIM.oset(aPt1,aRes.z);
			}
		}
	}
	//cDecoupageInterv2D aDecoup = cDecoupageInterv2D::SimpleDec
	//int FusionCarteProf_main(int argc,char ** argv)


	/* Save new depth maps */
    Tiff_Im     aImProfZTif  ( aPx1NameOut.c_str(),
					           aSz,
							   aImProfPxTif.type_el(),
							   aImProfPxTif.mode_compr(),
							   aImProfPxTif.phot_interp());


    ELISE_COPY
    (
        aImProfZTif.all_pts(),
        aImProfZTIM.in(),
        aImProfZTif.out()
    );


	/* Update aNuageIn */
	aImProfPx.Image()      		= NameWithoutDir(aPx1NameOut);
	aImProfPx.GeomRestit() 		= eGeomMNTFaisceauIm1ZTerrain_Px1D;
	aNuageIn.Image_Profondeur() = aImProfPx;
	aNuageIn.NameOri() = aICNM->StdNameCamGenOfNames(aOri,aIm1);

	MakeFileXML(aNuageIn,aNuageNameOut);

	return EXIT_SUCCESS;
}


//TODO:
// move everything to new files inside dir SAT4GEO
//- MMBy1 from other correlator
//- add Bsur min in pairs creation
//-?triangulation per Box?
//

/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant �  la mise en
correspondances d'images pour la reconstruction du relief.

Ce logiciel est régi par la licence CeCILL-B soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL-B telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  �  l'utilisation,  �  la modification et/ou au
développement et �  la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe �
manipuler et qui le réserve donc �  des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités �  charger  et  tester  l'adéquation  du
logiciel �  leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
�  l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder �  cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
