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
#include "StdAfx.h"
#include "cimgeo.h"
#include "cero_modelonepaire.h"


extern int RegTIRVIS_main(int , char **);


// we wish to improve coregistration between 2 orthos
class cCoreg2Ortho
{
    public:
    std::string mDir;
    cCoreg2Ortho(int argc,char ** argv);
    private:
    cImGeo * mO1;
    cImGeo * mO2;
    Im2D_REAL4 mO1clip,mO2clip;
    std::string mNameO1, mNameO2, mNameMapOut;
    Box2dr mBoxOverlapTerrain;

};



cCoreg2Ortho::cCoreg2Ortho(int argc,char ** argv)
{

    ElInitArgMain
            (
                argc,argv,
                LArgMain()  << EAMC(mNameO1,"Name Ortho master", eSAM_IsExistFile)
                            << EAMC(mNameO2,"Name Ortho slave",eSAM_IsExistFile),
                LArgMain()  << EAM(mNameMapOut,"Out",true, "Name of resulting map")
                );

    if (!MMVisualMode)
    {

        mDir="./";
        mNameMapOut=mNameO2 +"2"+ mNameO1;
        cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);

        if (ELISE_fp::exist_file(mNameO1) & ELISE_fp::exist_file(mNameO2))
        {
            // open orthos
            // Initialise les 2 orthos
            mO1 = new cImGeo(mDir+mNameO1);
            mO2 = new cImGeo(mDir+mNameO2);

            // je teste non pas la coreg mais le blending par gaussian distance weighting
            Im2D_REAL4 I1=mO1->toRAM();
            Im2D_REAL4 I2=mO2->toRAM();
            Box2dr boxMosaic=mO1->boxEnglob(mO2);
            Im2D_REAL4 mosaic=mO1->box2Im(boxMosaic);
            Pt2dr aCorner=Pt2dr(boxMosaic._p0.x,boxMosaic._p1.y); // xmin, ymax;
            Pt2di tr1=mO1->computeTrans(aCorner), tr2=mO2->computeTrans(aCorner);

            // right now i consider that nadir position is at the center of the image
            // express nadir position in pixel in geometry of mosaic
            Pt2di N1((Pt2di(I1.sz()/Pt2di(2,2))+tr1)),N2((Pt2di(I2.sz()/Pt2di(2,2))+tr2));
            double constLambda(0.5);

            // test la fonction Distance de chamfer sur une image binaire bidonne
            Im2D_Bits<1> mIBin(I1.sz().x,I1.sz().y,1);
            std::string aName("IBin1.tif");
            ELISE_COPY(
                        mIBin.all_pts(),
                        mIBin.in(),
                        Tiff_Im(aName.c_str(),
                                    mIBin.sz(),
                                    GenIm::int1,
                                    Tiff_Im::No_Compr,
                                    Tiff_Im::BlackIsZero).out()
                        );
            ELISE_COPY(
                        disc(Pt2dr(128,128),100),
                        0,
                        mIBin.out()
                        );

            ELISE_COPY(
                        disc(Pt2dr(500,500),100),
                        0,
                        mIBin.out()
                        );
            // la fonction chamfer fonctionne sur une image binaire et va calculer les distance à partir des pixels qui ont une valeur de 0.

            // la distance maximale est de 255 car la fonction chamfer


            Im2D_U_INT1  aD(I1.sz().x,I1.sz().y);
            ELISE_COPY(aD.all_pts(),mIBin.in(),aD.out());

            Chamfer::d32.im_dist(aD);

            std::string aName3("chamferTestd32.tif");
            ELISE_COPY(
                        aD.all_pts(),
                        aD.in(),
                        Tiff_Im(aName3.c_str(),
                                    aD.sz(),
                                    GenIm::real4,
                                    Tiff_Im::No_Compr,
                                    Tiff_Im::BlackIsZero).out()
                        );

            ELISE_COPY(
                        disc(Pt2dr(700,700),200),
                        0,
                        aD.out()
                        );


             Chamfer::d5711.im_dist(aD);

             std::string aName4("chamferTestd5711.tif");
             ELISE_COPY(
                         aD.all_pts(),
                         aD.in(),
                         Tiff_Im(aName4.c_str(),
                                     aD.sz(),
                                     GenIm::real4,
                                     Tiff_Im::No_Compr,
                                     Tiff_Im::BlackIsZero).out()
                         );

            /*
            std::cout << "mosaic of size " << mosaic.sz() << ".\n";

            for (unsigned int i(1) ; i <mosaic.sz().x;i++)
            {
                for (unsigned int j(1) ; j < mosaic.sz().y;j++)
                {
                    Pt2di pos(i,j);

                    if (i%100==0 & j%100==0)
                    {
                    std::cout << "process pixel at position " << pos << " of mosaic.\n";
                    std::cout << "Im1 position " << pos+tr1 << ".\n";
                    std::cout << "Im2 position " << pos+tr2 << ".\n";
                    }

                    // compute distance of current position from Nadir point Im1 and Nadir point Im2

                    double aDist1=euclid(Pt2di(pos-N1)), aDist2=euclid(Pt2di(pos-N2));
                    double r=aDist1/aDist2;


                    double Iij1(0.0),Iij2(0.0);
                    double w1(0),w2(0);

                    if (I1.Inside(Pt2di(pos+tr1)))
                        {
                        // I haven't loaded the mask thus I have to check here

                        double val=I1.GetR(Pt2di(pos+tr1));

                        if (val!=0)
                        {
                        Iij1=val;
                        //std::cout << "Im1, got" << Iij1 << ".\n";
                        //w1=pow(0.5,pow(r,2*constLambda));
                        w1=0.5;
                        }
                    }


                    if (I2.Inside(Pt2di(pos+tr2)))
                    {
                        double val=I2.GetR(Pt2di(pos+tr2));
                        if (val!=0)
                        {
                        Iij2=val;
                        //std::cout << "Im2, got " << Iij2 << ".\n";
                        w2=1-w1;
                        }
                    }

                    if (w2==0) w1=1;
                    //if (w1==0) w2=1;

                    double blend=w1*Iij1+w2*Iij2;
                    mosaic.SetR(pos,blend);

                }
            }

            std::string aName("mosaicTest.tif");
            ELISE_COPY(
                        mosaic.all_pts(),
                        mosaic.in(),
                        Tiff_Im(aName.c_str(),
                                    mosaic.sz(),
                                    GenIm::real4,
                                    Tiff_Im::No_Compr,
                                    Tiff_Im::BlackIsZero).out()
                        );



            // Determine la zone de recouvrement entre les 2 orthos

            mBoxOverlapTerrain=mO1->overlapBox(mO2);
            // clip les 2 ortho sur cette box terrain afin d'avoir des Im2D chargé en RAM
            mO1clip = mO1->clipImTer(mBoxOverlapTerrain);
            mO2clip = mO2->clipImTer(mBoxOverlapTerrain);

            std::string aOut1A("im1.tif"),aOut2A("im2.tif");
            ELISE_COPY(
                        mO2clip.all_pts(),
                        mO2clip.in(),
                        Tiff_Im(aOut2A.c_str(),
                                mO2clip.sz(),
                                GenIm::real4,
                                Tiff_Im::No_Compr,
                                Tiff_Im::BlackIsZero).out()
                        );

            ELISE_COPY(
                        mO1clip.all_pts(),
                        mO1clip.in(),
                        Tiff_Im(aOut1A.c_str(),
                                mO1clip.sz(),
                                GenIm::real4,
                                Tiff_Im::No_Compr,
                                Tiff_Im::BlackIsZero).out()
                        );


            Pt2dr aCorner=Pt2dr(mBoxOverlapTerrain._p0.x,mBoxOverlapTerrain._p1.y); // xmin, ymax;
            Pt2di transO1Tobox = mO1->computeTrans(aCorner);
            Pt2di transO2Tobox = mO2->computeTrans(aCorner);
            // Pt2di trans = mO1->computeTrans(mO2);

            Pt2di sz(25,25);
            unsigned int pasX=mO1clip.sz().x/10;
            unsigned int pasY=mO1clip.sz().y/10;
            std::cout << "step x " << pasX << ", step Y " <<pasY << "\n";

            for (unsigned int i(1) ; i < 10;i++)
            {
                for (unsigned int j(1) ; j < 10;j++)
                {

                    if((i*pasX>sz.x) & (j*pasY>sz.y) & ((i*pasX+sz.x)<mO1clip.sz().x) & ((j*pasY+sz.y)<mO1clip.sz().y) )
                    {

                        Pt2di pt=Pt2di(i*pasX,j*pasY);
                        //Im2D_REAL4 im1(2*sz.x,2*sz.y);
                        //Im2D_REAL4 im2(2*sz.x,2*sz.y);
                        Im2D_REAL4 im1(mO1clip.sz().x,mO1clip.sz().y);
                        Im2D_REAL4 im2(mO1clip.sz().x,mO1clip.sz().y);

                        //Im2D_REAL4 im1=mO1clip;
                        //Im2D_REAL4 im2=mO2clip;
                        std::cout << " rectange " << pt-sz << " , " << pt+sz<<", tuile "<< i << ", " << j <<"\n";
                     /*   ELISE_COPY(
                                    rectangle(pt-sz,pt+sz),
                                    Virgule(mO1clip.in(),mO2clip.in()),
                                    Virgule(im1.out(),im2.out())
                                    );

                        ELISE_COPY(
                                   rectangle(Pt2di(0,0),(Pt2di(2,2)*sz)),
                                   trans(im1.in(),pt-sz),
                                   im1.out()
                                    );



                        int nbPix(0);
                        /*
                        ELISE_COPY(
                                    select(im1.all_pts(),im1.in()!=0 && im2.in()!=0),
                                    1,
                                    sigma(nbPix)
                                    );
                        std::cout << "Tile " << i << "," <<j << ", number of pixel with data : "<< nbPix << "\n";

                     //   if (nbPix==im1.sz().x*im1.sz().y)
                      //  {


                        // save tile for visual check
                        std::string aPrefix="-"+std::to_string(i)+"_"+std::to_string(j)+".tif";
                        std::string aOut1("Tile_"+mNameO1 +aPrefix),aOut2("Tile_"+mNameO2 +aPrefix);
                        ELISE_COPY(
                                    im1.all_pts(),
                                    im1.in(),
                                    Tiff_Im(aOut1.c_str(),
                                            im1.sz(),
                                            GenIm::real4,
                                            Tiff_Im::No_Compr,
                                            Tiff_Im::BlackIsZero).out()
                                    );

                        ELISE_COPY(
                                    im2.all_pts(),
                                    im2.in(),
                                    Tiff_Im(aOut2.c_str(),
                                            im2.sz(),
                                            GenIm::real4,
                                            Tiff_Im::No_Compr,
                                            Tiff_Im::BlackIsZero).out()
                                    );
                    }
                    }

            }
*/

        } else { std::cout << "cannot find ortho 1 and 2, please check file names\n";}

    }

}



class  cFeatheringAndMosaicOrtho
{
    public:
    std::string mDir;
    cFeatheringAndMosaicOrtho(int argc,char ** argv);
    private:
    std::map<int, cImGeo*> mIms; // key=label
    std::map<int, Im2D_REAL4>   mIm2Ds;
    std::map<int, Im2D_INT2>  mChamferDist;
    std::map<int, Im2D_U_INT1>  mBlendingArea;
    std::map<int, Im2D_U_INT1>  mMosaicArea;
    std::string mNameMosaicOut, mFullDir;
    int mDist;
    std::list<std::string> mLFile;
    cInterfChantierNameManipulateur * mICNM;
    Box2dr mBoxOverlapTerrain;
    double mLambda;
};

cFeatheringAndMosaicOrtho::cFeatheringAndMosaicOrtho(int argc,char ** argv)
{
    mDist=100; // distance chamfer de 100 pour le feathering/estompage
    mLambda=0.2;
    ElInitArgMain
            (
                argc,argv,
                LArgMain()   << EAMC(mFullDir,"ortho pattern", eSAM_IsPatFile)
                ,
                LArgMain()  << EAM(mNameMosaicOut,"Out",true, "Name of resulting map")
                            << EAM(mDist,"Dist",true, "Distance for seamline feathering blending" )
                            << EAM(mLambda,"Lambda",true, "lambda value for gaussian distance weighting, def 0.2" )
                );

    if (!MMVisualMode)
    {

        mDir="./";
        mNameMosaicOut="mosaicFeatheringTest1.tif";
        mICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);
        mLFile = mICNM->StdGetListOfFile(mFullDir);

        cFileOriMnt MTD = StdGetFromPCP("MTDOrtho.xml",FileOriMnt);
        Box2dr boxMosaic(Pt2dr(MTD.OriginePlani().x,MTD.OriginePlani().y+MTD.ResolutionPlani().y*MTD.NombrePixels().y),Pt2dr(MTD.OriginePlani().x+MTD.ResolutionPlani().x*MTD.NombrePixels().x,MTD.OriginePlani().y));
        Im2D_REAL4 mosaic(MTD.NombrePixels().x,MTD.NombrePixels().y);
        Im2D_REAL4 NbIm(MTD.NombrePixels().x,MTD.NombrePixels().y);
        Im2D_INT4 SumDist(MTD.NombrePixels().x,MTD.NombrePixels().y);
        Im2D_INT4 SumDist2(MTD.NombrePixels().x,MTD.NombrePixels().y);
        Im2D_REAL4 PondInterne(MTD.NombrePixels().x,MTD.NombrePixels().y);
        Pt2dr aCorner=Pt2dr(boxMosaic._p0.x,boxMosaic._p1.y); // xmin, ymax;
        Pt2di sz(MTD.NombrePixels().x,MTD.NombrePixels().y);

        Im2D_U_INT1 label=Im2D_U_INT1::FromFileStd("Label.tif");
        /*
        std::string aName("testlabel.tif");
        ELISE_COPY(
                    label.all_pts(),
                    label.in(),
                    Tiff_Im(aName.c_str(),
                                label.sz(),
                                GenIm::real4,
                                Tiff_Im::No_Compr,
                                Tiff_Im::BlackIsZero).out()
                    );
        */

        unsigned int i(0); // i is the label of the image - and the key
        for (auto &im : mLFile)
        {
            std::cout << "Image " << im <<": load and compute feathering buffer.\n";
            // open orthos
            mIms[i]= new cImGeo(mDir+im);
            mIm2Ds[i]=  mIms[i]->toRAM();
            mChamferDist[i]=Im2D_INT2(mIms[i]->Im().sz().x,mIms[i]->Im().sz().y,1);
            Pt2di sz(mIms[i]->Im().sz());
            Pt2di tr= -mIms[i]->computeTrans(aCorner);

            // la fonction chamfer fonctionne sur une image binaire et va calculer les distance à partir des pixels qui ont une valeur de 0.
            // la distance maximale est de 255 car la fonction chamfer

            //detect seam line for this image
            //1) translation of label to be in ortho geometry
            Im2D_U_INT1 tmp(sz.x,sz.y,1);
            ELISE_COPY(select(tmp.all_pts(), trans(label.in(),tr)!=(int)i),
                       //trans(label.in(),tr),
                       0,
                       tmp.out()
                       );
            // set value to 0 for points that are not inside area of mosaicking for this image
            // compute chamfer Distance
            Chamfer::d32.im_dist(tmp);
            // inverse value of distance (enveloppe interne à la seamline)
            ELISE_COPY(mChamferDist[i].all_pts(),-tmp.in(),mChamferDist[i].out());

            // je réinitialise tmp
            ELISE_COPY(tmp.all_pts(),1,tmp.out());
            ELISE_COPY(
                        select(mChamferDist[i].all_pts(),mChamferDist[i].in()==-2),// distance ==-2 c'est les pixels Sur la ligne de raccord
                        0,
                        tmp.out());
            // ça m'ennuie qu'il retourne une distance à par rapport au bord de l'image, comment annuler ce comportement?
            Chamfer::d32.im_dist(tmp);

            ELISE_COPY(
                        select(mChamferDist[i].all_pts(),mChamferDist[i].in()==0),
                        tmp.in(),
                        mChamferDist[i].out());

            // apply the hidden part masq

            //std::string aNameIm =  mICNM->Assoc2To1("Key-Assoc-OpIm2PC@",im,true).first;
            std::string aNamePC = "PC"+ im.substr(3,im.size()-2);

            Im2D_U_INT1 masq=Im2D_U_INT1::FromFileStd(aNamePC);
            // apply the mask
            ELISE_COPY(
                        select(mChamferDist[i].all_pts(),masq.in()==255),
                        255,
                        mChamferDist[i].out());

            std::string aNameTmp="TestChamfer" + std::to_string(i) + ".tif";
            ELISE_COPY(
                        mChamferDist[i].all_pts(),
                        mChamferDist[i].in(),
                        Tiff_Im(aNameTmp.c_str(),
                                mChamferDist[i].sz(),
                                GenIm::real4,
                                Tiff_Im::No_Compr,
                                Tiff_Im::BlackIsZero).out()
                        );
            // warning, effet de bord du au calcul de distance chamfer!!!!!!!!!!!!!

            // comptage du nombre d'image a utiliser pour le blending (geométrie mosaic)

            ELISE_COPY(select(NbIm.all_pts(),trans(mChamferDist[i].in(mDist+1),-tr)<=mDist),
                       //mosaic.in()+trans(mIm2Ds[i].in(0),tr)*lut_w.in()[-trans(mChamferDist[i].in(0) ,tr)],
                       NbIm.in(0)+1,
                       NbIm.out()
                       );

            // Q? pourquoi je ne peux pas renseigner juste in() sans avoir une erreur genre  BITMAP :  out of domain while reading (RLE mode)


            // somme des distances de chamber dans les enveloppes externes  - pour gérer les cas de blending de 3 images
            ELISE_COPY(select(SumDist.all_pts(),trans(mChamferDist[i].in_proj(),-tr)<=mDist & trans(mChamferDist[i].in_proj(),-tr)>0),
                       SumDist.in(0)+trans(mChamferDist[i].in(0),-tr),
                       SumDist.out()
                       );
            // somme des distances de chamber dans les enveloppes inter  - également pour gérer les cas de blending de 3 images
            ELISE_COPY(select(SumDist2.all_pts(),trans(mChamferDist[i].in_proj(),-tr)>=-mDist & trans(mChamferDist[i].in_proj(),-tr)<0),
                       SumDist2.in(0)+trans(mChamferDist[i].in(0),-tr),
                       SumDist2.out()
                       );

            i++;
        }


        // je modifie la zone du blending de 3 images afin de 1) la réduire et 2) quel aie une forme plus adéquate
      // non je ne peux pas faire ça sinon la zone NbIm==3 que je supprime sera considérée comme NbIm=2 alors que c'est faux
       /* ELISE_COPY(select(NbIm.all_pts(),SumDist.in()>mDist),
                   2,
                   NbIm.out()
                   );

        Im2D_U_INT1 tmp(MTD.NombrePixels().x,MTD.NombrePixels().y,0);
        ELISE_COPY(select(tmp.all_pts(),NbIm.in()==3),
                   1,
                   tmp.out()
                   );

        Chamfer::d32.im_dist(tmp);

        std::string aNameTmp("TestTmp.tif");
        ELISE_COPY(
                    tmp.all_pts(),
                    tmp.in(),
                    Tiff_Im(aNameTmp.c_str(),
                            tmp.sz(),
                            GenIm::real4,
                            Tiff_Im::No_Compr,
                            Tiff_Im::BlackIsZero).out()
                    );
*/
        std::string aNameTmp("TestCDG.tif");

      aNameTmp="TestNbIm.tif";

        ELISE_COPY(
                    NbIm.all_pts(),
                    NbIm.in(),
                    Tiff_Im(aNameTmp.c_str(),
                            mosaic.sz(),
                            GenIm::real4,
                            Tiff_Im::No_Compr,
                            Tiff_Im::BlackIsZero).out()
                    );


        // détection du centre de gravité des zones de recouvements 3 images = point d'intersection des 3 images
        Im2D_U_INT1 Ibin(MTD.NombrePixels().x,MTD.NombrePixels().y,0);
        Im2D_U_INT1 chanferZTriple(sz.x,sz.y,0);
        ELISE_COPY(select(Ibin.all_pts(),NbIm.in()==3),
                   1,
                   Ibin.out()
                   );

        U_INT1 ** d = Ibin.data();
        int count(0);
        Neighbourhood V8=Neighbourhood::v8();

        std::cout << "start detection of area of 3 ortho blending\n";
        for (INT x=0; x < Ibin.sz().x; x++)
        {
            for (INT y=0; y < Ibin.sz().y; y++)
            {
                if (d[y][x] == 1)
                {
                    count++;
                    Liste_Pts_INT2 cc(2);
                    ELISE_COPY
                            (
                                 // flux: composantes connexes du point.
                                conc
                                (
                                    Pt2di(x,y),
                                    Ibin.neigh_test_and_set(V8, 1, 0,  20) ), // on change la valeur des points sélectionné comme ça à la prochaine itération on ne sélectionne plus cette zone de composante connexe
                                1, // valeur bidonne, c'est juste le flux que je sauve dans cc
                                cc
                                );

                        Pt2di cdg;
                                ELISE_COPY
                                (
                                    cc.all_pts(),
                                    Virgule(FX,FY),
                                    (cdg.sigma()) // sigma: somme des position X et Y
                                    );
                        cdg=cdg/cc.card(); // centre de grav: moyenne des positions
                    //std::cout << " points d'une ligne de raccord intersectant 3 orthos : num " << count << " position " << cdg <<"\n";
                    // fin if (d[y][x] == 1)
                     // ok j'ai le pt d'intersection, qu'est ce que j'en fait?
                     // 1distance autour de ce point. cela concerne uniquement la zone de recouvrement triple soit cc
                        Im2D_U_INT1 chanfer(sz.x,sz.y,1);
                        ELISE_COPY(cc.all_pts(),
                                   0,
                                   chanfer.out()
                                   );
                        Chamfer::d32.im_dist(chanfer);
                        // maintenant j'utilise cette distance en combinaison avec distance Env interne




                }
            }
        }

        aNameTmp="TestSumDist.tif";
        ELISE_COPY(
                    SumDist.all_pts(),
                    SumDist.in(),
                    Tiff_Im(aNameTmp.c_str(),
                            SumDist.sz(),
                            GenIm::u_int1,
                            Tiff_Im::No_Compr,
                            Tiff_Im::BlackIsZero).out()
                    );
        aNameTmp="TestSumDistInterne.tif";
        ELISE_COPY(
                    SumDist2.all_pts(),
                    SumDist2.in(),
                    Tiff_Im(aNameTmp.c_str(),
                            SumDist2.sz(),
                            GenIm::real4,
                            Tiff_Im::No_Compr,
                            Tiff_Im::BlackIsZero).out()
                    );

         // maintenant qu'on a toute les info on applique le blending
        std::cout << "Start Blending\n";

        // look up table de weight pour enveloppe interne
        Im1D_REAL4 lut_w(mDist+1);
        ELISE_COPY
        (
        lut_w.all_pts(),
       // FX/(double)mDist,
        pow(0.5,pow((FX/((double)mDist-FX+1)),2*mLambda)),
        lut_w.out()
        );

        // replace by 1 distance over the threshold of mDist
        ELISE_COPY(select(lut_w.all_pts(),lut_w.in()>1),1,lut_w.out());

        /*
        for (unsigned int i(0); i<mDist;i++)
        {
            std::cout << "Internal enveloppe weighting at Dist=" << i << " is equal to " << lut_w.At(i) << "\n";
        }
        */

        // pondération contribution de l'image à l'intérieur de son enveloppe; je peux effectuer le calcul du facteur de pondération pour toutes les images
        //  partie fixe pondérée seulement par le nombre d'image

        ELISE_COPY(select(PondInterne.all_pts(), NbIm.in()!=0),
                   1-(NbIm.in()-1)/NbIm.in(0),
                   PondInterne.out()
                   );

        // featherling dans l'enveloppe interne
          ELISE_COPY(select(PondInterne.all_pts(), NbIm.in()!=0 && SumDist2.in() <=0 && SumDist2.in()>=-mDist),
                  PondInterne.in()+ (1-(1/NbIm.in())) *  lut_w.in()[mDist+SumDist2.in()],
                  PondInterne.out()
                );

          aNameTmp="TestPondInterne.tif";
          ELISE_COPY(
                      PondInterne.all_pts(),
                      PondInterne.in(),
                      Tiff_Im(aNameTmp.c_str(),
                              PondInterne.sz(),
                              GenIm::real4,
                              Tiff_Im::No_Compr,
                              Tiff_Im::BlackIsZero).out()
                      );


        for (unsigned int i(0);  i < mIms.size();i++)
        {

            std::cout << "Image" << i << "\n";

            Pt2di tr= mIms[i]->computeTrans(aCorner);

            // enveloppe interne

/*
            // toute l'enveloppe interne, partie fixe pondérée seulement par le nombre d'image
            ELISE_COPY(select(mosaic.all_pts(),trans(mChamferDist[i].in(1),tr)<=0 & NbIm.in()!=0),
                       //  ok ça ca va : mosaic.in()+    trans(mIm2Ds[i].in(0),tr)*lut_w.in()[-trans(mChamferDist[i].in(0) ,tr)],
                       //trans(mIm2Ds[i].in(0),tr)*lut_w.in()[-trans(mChamferDist[i].in(0) ,tr)], fonctionne pas, croppe l'image
                      // mosaic.in()+   trans(mIm2Ds[i].in(0),tr)  * (1-(NbIm.in()-1)/NbIm.in()),

                       //mosaic.in()+ trans(mIm2Ds[i].in(0),tr) *(1-((NbIm.in()-1)*2.00)/(NbIm.in()*2.00)),
                       // PONDERATION ONLY
                       mosaic.in()+ (1-((NbIm.in()-1)*2.00)/(NbIm.in()*2.00)),
                       mosaic.out()
                       );


            // buffer dans l'enveloppe interne , je peux utiliser SumDist2 si je le garde au finish? non car sumdist interne a des valeurs pour les 2 coté de la ligne de raccord
              ELISE_COPY(select(mosaic.all_pts(),trans(mChamferDist[i].in(0),tr)<=0 & trans(mChamferDist[i].in(0),tr)>=-mDist & NbIm.in()!=0),
                       //mosaic.in()+   trans(mIm2Ds[i].in(0),tr)  * (1-(1/NbIm.in())) *  lut_w.in()[-trans(mChamferDist[i].in(0) ,tr)],
                      // mosaic.in()+  (1-(1/NbIm.in())) *  lut_w.in()[-trans(mChamferDist[i].in(0) ,tr)],
                       mosaic.in()+  (1-(2.00/(2.00*NbIm.in()))) *  lut_w.in()[-trans(mChamferDist[i].in(0) ,tr)],
                       mosaic.out()
                    );


            // enveloppe externe


            // ligne de raccord entre 2 images
            ELISE_COPY(select(mosaic.all_pts(),trans(mChamferDist[i].in(0),tr)>0 & trans(mChamferDist[i].in(0),tr)<=mDist & NbIm.in()==2),
                       //mosaic.in()+ trans(mIm2Ds[i].in(0),tr)* (1-lut_w.in()[trans(mChamferDist[i].in(0) ,tr)]) * (1.00-(2.00/(2*NbIm.in()))) * 2.0*trans(mChamferDist[i].in(0),tr)/(2*SumDist.in(0)),// le 2.O/2 il empeche un bug débile
                       //mosaic.in()+ (1-lut_w.in()[trans(mChamferDist[i].in(0) ,tr)]) * (1.00-(2.00/(2*NbIm.in()))) * 2.0*trans(mChamferDist[i].in(0),tr)/(2*SumDist.in(0)),
                       // ponderation only
                       mosaic.in()+ (1-lut_w.in()[trans(mChamferDist[i].in(0) ,tr)]) * (1.00-(2.00/(2*NbIm.in()))),
                       mosaic.out()
                    );


            // raccord entre 3 images
            ELISE_COPY(select(mosaic.all_pts(),trans(mChamferDist[i].in(0),tr)>0 & trans(mChamferDist[i].in(0),tr)<=mDist & NbIm.in()==3 & SumDist.in()>0),
                       // ponderation only
                      // mosaic.in()+  (1-((1-(2.00/(2.00*NbIm.in()))) * lut_w.in()[-SumDist2.in()])) * 2.0*trans(mChamferDist[i].in(0),tr)/(2.0*SumDist.in(0)),
                        mosaic.in()+  (1-(1-(2.00/(2.00*NbIm.in())) * lut_w.in()[-SumDist2.in()])) * 2.0*trans(mChamferDist[i].in(0),tr)/(2.0*SumDist.in(0)) ,
                       mosaic.out()
                    );
 */
            // enveloppe interne
            ELISE_COPY(select(mosaic.all_pts(),trans(mChamferDist[i].in(0),tr)<0),
                       mosaic.in()+ trans(mIm2Ds[i].in(0),tr)* PondInterne.in(),
                       //mosaic.in()+  PondInterne.in(),
                       mosaic.out()
                    );

            // ligne de raccord entre 2 images = partage du gateau en 2
            ELISE_COPY(select(mosaic.all_pts(),trans(mChamferDist[i].in(0),tr)>0 & trans(mChamferDist[i].in(0),tr)<=mDist & NbIm.in()==2),
                       mosaic.in()+ trans(mIm2Ds[i].in(0),tr)* (1-PondInterne.in()),
                       //mosaic.in()+(1-PondInterne.in()),
                       mosaic.out()
                );


            // ligne de raccord entre 3 images = partage du gateau en 3
            for (INT x = 0 ; x< mosaic.sz().x; x++)
            {
                for (INT y = 0 ; y< mosaic.sz().y; y++)
                {
                    Pt2di pos(x,y);
                    if (mChamferDist[i].Inside(Pt2di(pos+tr)) & NbIm.GetR(pos)==3)
                    {

                        // enveloppe interne
                        /*
                        if (mChamferDist[i].GetR(Pt2di(pos+tr))<0 & mChamferDist[i].GetR(Pt2di(pos+tr)) >=-mDist )
                        {
                        int lutPos = min( mDist-SumDist.GetR(pos)/2, mChamferDist[i].GetR(Pt2di(pos+tr)))   ;
                         std::cout << " min of  " << mDist-SumDist.GetR(pos)/2<< " and chamber dist  " << -mChamferDist[i].GetR(Pt2di(pos+tr)) <<" give " << lutPos << "\n";
                        //double val =     (1-PondInterne.GetR(pos)) * lut_w.At(lutPos) +  PondInterne.GetR(pos);
                        //PondInterne.SetR(pos,val);
                        }

                      */
                        if (mChamferDist[i].GetR(Pt2di(pos+tr))>0 & mChamferDist[i].GetR(Pt2di(pos+tr)) <=mDist & mChamferDist[i].GetR(Pt2di(pos+tr))> SumDist.GetR(pos)-mChamferDist[i].GetR(Pt2di(pos+tr)) )
                        {
                       double val =   mIm2Ds[i].GetR(Pt2di(pos+tr))    *  (1-PondInterne.GetR(pos)) +  mosaic.GetR(pos);
                            //double ratio =  mChamferDist[i].GetR(Pt2di(pos+tr))/SumDist.GetR(pos);
                        //double val =   mIm2Ds[i].GetR(Pt2di(pos+tr))    *  (1-PondInterne.GetR(pos)) *      ratio  +  mosaic.GetR(pos);
                       // std::cout << " Chamfer dist vaut " << mChamferDist[i].GetR(Pt2di(pos+tr)) << " et somme chanfer dist vaux " << SumDist.GetR(pos) << ", ratio de " << ratio << "\n";
                       mosaic.SetR(pos,val);
                        }
                        if (mChamferDist[i].GetR(Pt2di(pos+tr))>0 & mChamferDist[i].GetR(Pt2di(pos+tr)) <=mDist & mChamferDist[i].GetR(Pt2di(pos+tr))== SumDist.GetR(pos)-mChamferDist[i].GetR(Pt2di(pos+tr)) )
                        {
                        double val =  0.5* mIm2Ds[i].GetR(Pt2di(pos+tr))    *  (1-PondInterne.GetR(pos)) +  mosaic.GetR(pos);
                        mosaic.SetR(pos,val);
                        }

                    }
                }
            }


        }

        aNameTmp="TestPondInterne2.tif";
        ELISE_COPY(
                    PondInterne.all_pts(),
                    PondInterne.in(),
                    Tiff_Im(aNameTmp.c_str(),
                            PondInterne.sz(),
                            GenIm::real4,
                            Tiff_Im::No_Compr,
                            Tiff_Im::BlackIsZero).out()
                    );


        aNameTmp="TestMosaic.tif";

        ELISE_COPY(
                    mosaic.all_pts(),
                    mosaic.in(),
                    Tiff_Im(aNameTmp.c_str(),
                            mosaic.sz(),
                            GenIm::real4,
                            Tiff_Im::No_Compr,
                            Tiff_Im::BlackIsZero).out()
                    );

    }

}



// the VarioCam thermic camera record images at 16 bits, we want to convert them to 8 bits. A range of temperature is provided in order to  stretch the radiometric value on this range

class cVarioCamTo8Bits
{
    public:
    std::string mDir;
    cVarioCamTo8Bits(int argc,char ** argv);
    private:
    std::string mFullDir;
    std::string mPat;
    std::string mPrefix;
    bool mOverwrite;
    Pt2di mRangeT;
    bool mCelcius;
};


cVarioCamTo8Bits::cVarioCamTo8Bits(int argc,char ** argv) :
      mFullDir	("img.*.tif"),
      mPrefix ("8bits_"),
      mOverwrite (false),
      mCelcius(1)
{
    ElInitArgMain
    (
    argc,argv,
        LArgMain()  << EAMC(mFullDir,"image pattern", eSAM_IsPatFile)
                    << EAMC(mRangeT,"temperature range"),
        LArgMain()  << EAM(mOverwrite,"F",true, "Overwrite previous output images, def false")
                    << EAM(mCelcius,"Celcius",true, "Is the temperature range in celcius, default true, if false, Kelvin")
                    << EAM(mPrefix,"Prefix",true, "Prefix for output images")
    );


    if (!MMVisualMode)
    {

    SplitDirAndFile(mDir,mPat,mFullDir);
    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);
    const std::vector<std::string> aSetIm = *(aICNM->Get(mPat));

    Pt2di aRangeVario;
    // convert the range to
    if (mCelcius) {
        aRangeVario.x=100*(273.15+mRangeT.x) ;
        aRangeVario.y=100*(273.15+mRangeT.y) ;
    } else {aRangeVario=mRangeT;};
    std::cout << "Range of radiometric value of variocam images : " << aRangeVario << "\n";

    for (auto & im : aSetIm)
    {
        std::string NameOut(mDir+mPrefix+im);

        if (ELISE_fp::exist_file(NameOut) & !mOverwrite)
        {
            std::cout <<"Image " << NameOut <<" already exist, use F=1 to overwrite.\n";
        } else {

        int minRad(aRangeVario.x), rangeRad(aRangeVario.y-aRangeVario.x);

        // load input variocam images
        Tiff_Im mTifIn=Tiff_Im::StdConvGen(mDir+im,1,true);
        // create empty RAM image for imput image
        Im2D_REAL4 imIn(mTifIn.sz().x,mTifIn.sz().y);
        // create empty RAM image for output image
        Im2D_U_INT1 imOut(mTifIn.sz().x,mTifIn.sz().y);
        // fill it with tiff image value
        ELISE_COPY(
                    mTifIn.all_pts(),
                    mTifIn.in(),
                    imIn.out()
                   );
        // change radiometry and note min and max value
        int aMin(255), aMax(0),aSum(0),aNb(0);
        for (int v(0); v<imIn.sz().y;v++)
        {
            for (int u(0); u<imIn.sz().x;u++)
            {
                Pt2di pt(u,v);
                double aVal = imIn.GetR(pt);
                unsigned int val(0);

                if(aVal!=0){
                    if (aVal>=minRad && aVal <minRad+rangeRad)
                    {
                        val=255.0*(aVal-minRad)/rangeRad;
                    }
                    if (aVal >=minRad+rangeRad) val=255.0;
                }

                if (val>aMax) aMax=val;
                if (val!=0){
                    if (val<aMin) aMin=val;
                    aSum+=val;
                    aNb++;
                }
                imOut.SetR(pt,val);
                //std::cout << "aVal a la position " << pt << " vaut " << aVal << ", transfo en " << v <<"\n";
            }
        }

        Tiff_Im aTifOut
                (
                    NameOut.c_str(),
                    imOut.sz(),
                    GenIm::u_int1,
                    Tiff_Im::No_Compr,
                    Tiff_Im::BlackIsZero
                    );
        std::cout << "Writing " << NameOut << ", Min " << aMin <<" Max "<< aMax <<" Mean "<< aSum/aNb <<  "\n";

        ELISE_COPY(imOut.all_pts(),imOut.in(),aTifOut.out());

        }
    }
    }
}

// appliquer une translation à une orientation

class cOriTran_Appli
{
    public:
    cOriTran_Appli(int argc,char ** argv);

    private:
    std::string mDir;
    std::string mFullDir;
    std::string mPat;
    std::string mOriIn, mOriOut;
    Pt3dr mTr;
};


cOriTran_Appli::cOriTran_Appli(int argc,char ** argv)
{
    ElInitArgMain
    (
    argc,argv,
        LArgMain()  << EAMC(mFullDir,"image pattern", eSAM_IsPatFile)
                    << EAMC(mOriIn,"Orientation Directory", eSAM_IsExistDirOri )
                    << EAMC(mTr,"Translation vector" )
                    << EAMC(mOriOut,"Orientation Out" )
        ,LArgMain()
    );


    if (!MMVisualMode)
    {

    SplitDirAndFile(mDir,mPat,mFullDir);
    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);
     StdCorrecNameOrient(mOriIn,mDir);
    const std::vector<std::string> aSetIm = *(aICNM->Get(mPat));

    // the bad way to do this, because I cannot find how to change the optical center of a camera with micmac classes
    std::string aNameTmp("Tmp-OriTrans.txt");
    std::string aCom = MMDir()
                + std::string("bin/mm3d")

                + " OriExport Ori-"
                +  mOriIn + std::string("/Orientation-")
                +  mPat + std::string(".xml")
                + std::string(" ") + aNameTmp;
    std::cout << aCom << "\n";

    system_call(aCom.c_str());

    aCom= MMDir()
            + std::string("bin/mm3d OriConvert '#F=N_X_Y_Z_W_P_K' ")
            +  aNameTmp + std::string(" ")
            + std::string("OriTrans")
            + std::string("  OffsetXYZ=") + ToString(mTr);

    std::cout << aCom << "\n";

    system_call(aCom.c_str());

    // je fais une bascule la dessus

    // je vérifie également avec un oriexport que l'offset fonctionnne

    aCom="cp Ori-"+mOriIn +"/AutoCal* Ori-"+mOriOut+"/";
    system_call(aCom.c_str());

   /* aCom= MMDir()
            + std::string("bin/mm3d GCP '#F=N_X_Y_Z_S_S_S' ")
            +  aNameTmp + std::string(" ")
            + std::string(mOriOut)
            + std::string("  OffsetXYZ=") + ToString(mTr);

    std::cout << aCom << "\n";

    system_call(aCom.c_str());

*/


    }

}
//    Applique une homographie à l'ensemble des images thermiques pour les mettres dans la géométrie des images visibles prises simultanément

class cTIR2VIS_Appli;
class cTIR2VIS_Appli
{
    public:
    void ReechThermicIm(std::vector<std::string> aPatImgs, std::string aHomog);
    void CopyOriVis(std::vector<std::string> aPatImgs, std::string aOri);
    cTIR2VIS_Appli(int argc,char ** argv);
    string T2V_imName(string tirName);
    string T2Reech_imName(string tirName);
    void changeImSize(std::vector<std::string> aLIm); //list image
    void changeImRadiom(std::vector<std::string> aLIm); //list image

    std::string mDir;
    private:
    std::string mFullDir;
    std::string mPat;
    std::string mHomog;
    std::string mOri;
    std::string mPrefixReech;
    bool mOverwrite;
    Pt2di mImSzOut;// si je veux découper mes images output, ex: homography between 2 sensors of different shape and size (TIR 2 VIS) but I want to have the same dimension as output
    Pt2di mRadiomRange;// If I want to change radiometry value, mainly to convert 16 bits to 8 bits
};


cTIR2VIS_Appli::cTIR2VIS_Appli(int argc,char ** argv) :
      mFullDir	("img.*.tif"),
      mHomog	("homography.xml"),
      mOri		("RTL"),
      mPrefixReech("Reech"),
      mOverwrite (false)



{
    ElInitArgMain
    (
    argc,argv,
        LArgMain()  << EAMC(mFullDir,"image pattern", eSAM_IsPatFile)
                    << EAMC(mHomog,"homography XML file", eSAM_IsExistFile ),
        LArgMain()  << EAM(mOri,"Ori",true, "ori name of VIS images", eSAM_IsExistDirOri )
                    << EAM(mOverwrite,"F",true, "Overwrite previous resampled images, def false")
                    << EAM(mImSzOut,"ImSzOut",true, "Size of output images")
                    << EAM(mRadiomRange,"RadiomRange",true, "range of radiometry of input images, if given, output will be 8 bits images")
    );


    if (!MMVisualMode)
    {

    SplitDirAndFile(mDir,mPat,mFullDir);
    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);
    const std::vector<std::string> aSetIm = *(aICNM->Get(mPat));


    ReechThermicIm(aSetIm,mHomog);

     if (EAMIsInit(&mOri))
     {
         StdCorrecNameOrient(mOri,mDir);
         mOri="Ori-"+mOri+"/";
         std::cout << "Copy orientation file." << std::endl;
         CopyOriVis(aSetIm,mOri);
      }

    // changer la taille des images out
    if (EAMIsInit(&mImSzOut))
    {
        //open first reech image just to read the dimension in order to print a message
        Tiff_Im mTif=Tiff_Im::StdConvGen(T2Reech_imName(aSetIm.at(0)),1,true);
        std::cout << "Change size of output images from " << mTif.sz() << " to " << mImSzOut << "\n";
        changeImSize(aSetIm);
    }

    // change the image radiometry
    if (EAMIsInit(&mRadiomRange))
    {
        std::cout << "Change images dynamic from range " << mRadiomRange << " to [0, 255] \n";
        changeImRadiom(aSetIm);
    }

    }
}



void cTIR2VIS_Appli::ReechThermicIm(
                                      std::vector<std::string> _SetIm,
                                      std::string aHomog
                                      )
{

     std::list<std::string>  aLCom;

    for(unsigned int aK=0; aK<_SetIm.size(); aK++)
    {
                string  aNameOut = "Reech_" + NameWithoutDir(StdPrefix(_SetIm.at(aK))) + ".tif";// le nom default donnée par ReechImMap

                std::string aCom = MMDir()
                            + std::string("bin/mm3d")
                            + std::string(" ")
                            + "ReechImMap"
                            + std::string(" ")
                            + _SetIm.at(aK)
                            + std::string(" ")
                            + aHomog;

                            if (EAMIsInit(&mPrefixReech)) {  aCom += " PrefixOut=" + T2Reech_imName(_SetIm.at(aK)) ; }

                            //+ " Win=[3,3]";// taille de fenetre pour le rééchantillonnage, par défaut 5x5

                bool Exist= ELISE_fp::exist_file(T2Reech_imName(_SetIm.at(aK)));

                if(!Exist || mOverwrite) {

                    std::cout << "aCom = " << aCom << std::endl;
                    //system_call(aCom.c_str());
                    aLCom.push_back(aCom);
                } else {
                    std::cout << "Reech image " << T2Reech_imName(_SetIm.at(aK)) << " exist, use F=1 to overwrite \n";
                }
    }
    cEl_GPAO::DoComInParal(aLCom);
}

// dupliquer l'orientation des images visibles de la variocam pour les images thermiques accociées
void cTIR2VIS_Appli::CopyOriVis(
                                      std::vector<std::string> _SetIm,
                                      std::string aOri
                                      )
{

    for(auto & imTIR: _SetIm)
    {
        //cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(mDir);
        std::string aOriFileName(aOri+"Orientation-"+T2V_imName(imTIR)+".xml");
        if (ELISE_fp::exist_file(aOriFileName))
        {
        std::string aCom="cp " + aOriFileName + "   "+ aOri+"Orientation-" + T2Reech_imName(imTIR) +".xml";
        std::cout << "aCom = " << aCom << std::endl;
        system_call(aCom.c_str());
        } else
        {
        std::cout << "Can not copy orientation " << aOriFileName << " because file not found." << std::endl;
        }

    }
}



string cTIR2VIS_Appli::T2V_imName(string tirName)
{
   std::string visName=tirName;

   visName[0]='V';
   visName[2]='S';

   return visName;

}

string cTIR2VIS_Appli::T2Reech_imName(string tirName)
{
   return mPrefixReech+ "_" + tirName;
}



int T2V_main(int argc,char ** argv)
{
    cTIR2VIS_Appli aT2V(argc,argv);
    return EXIT_SUCCESS;
}


void cTIR2VIS_Appli::changeImSize(std::vector<std::string> aLIm)
{
    for(auto & imTIR: aLIm)
    {
    // load reech images
    Tiff_Im mTifIn=Tiff_Im::StdConvGen(T2Reech_imName(imTIR),1,true);
    // create RAM image
    Im2D_REAL4 im(mImSzOut.x,mImSzOut.y);
    // y sauver l'image
    ELISE_COPY(mTifIn.all_pts(),mTifIn.in(),im.out());
    // juste clipper
    Tiff_Im  aTifOut
             (
                 T2Reech_imName(imTIR).c_str(),
                 im.sz(),
                 GenIm::real4,
                 Tiff_Im::No_Compr,
                 Tiff_Im::BlackIsZero
             );
    // on écrase le fichier tif
   ELISE_COPY(im.all_pts(),im.in(),aTifOut.out());
    }
}

void cTIR2VIS_Appli::changeImRadiom(std::vector<std::string> aLIm)
{
    for(auto & imTIR: aLIm)
    {

    int minRad(mRadiomRange.x), rangeRad(mRadiomRange.y-mRadiomRange.x);

    // load reech images
    Tiff_Im mTifIn=Tiff_Im::StdConvGen(T2Reech_imName(imTIR),1,true);
    // create empty RAM image for imput image
    Im2D_REAL4 imIn(mTifIn.sz().x,mTifIn.sz().y);
    // create empty RAM image for output image
    Im2D_U_INT1 imOut(mTifIn.sz().x,mTifIn.sz().y);
    // fill it with tiff image value
    ELISE_COPY(
                mTifIn.all_pts(),
                mTifIn.in(),
                imIn.out()
               );

    // change radiometry
    for (int v(0); v<imIn.sz().y;v++)
    {
        for (int u(0); u<imIn.sz().x;u++)
        {
            Pt2di pt(u,v);
            double aVal = imIn.GetR(pt);
            unsigned int v(0);

            if(aVal!=0){
            if (aVal>minRad && aVal <minRad+rangeRad)
            {
                v=255.0*(aVal-minRad)/rangeRad;
            }
            }

            imOut.SetR(pt,v);
            //std::cout << "aVal a la position " << pt << " vaut " << aVal << ", transfo en " << v <<"\n";
        }
    }

    // remove file to be sure of result
    //ELISE_fp::RmFile(T2Reech_imName(imTIR));

    Tiff_Im aTifOut
             (
                 T2Reech_imName(imTIR).c_str(),
                 imOut.sz(),
                 GenIm::u_int1,
                 Tiff_Im::No_Compr,
                 Tiff_Im::BlackIsZero
             );
    // on écrase le fichier tif
   ELISE_COPY(imOut.all_pts(),imOut.in(),aTifOut.out());
    }
}









/*    comparaise des orthos thermiques pour déterminer un éventuel facteur de calibration spectrale entre 2 frame successif, expliquer pouquoi tant de variabilité spectrale est présente (mosaique moche) */
// à priori ce n'est pas ça du tout, déjà mauvaise registration TIR --> vis du coup les ortho TIR ne se superposent pas , du coup correction metrique ne peut pas fonctionner.
int CmpOrthosTir_main(int argc,char ** argv)
{
    std::string aDir, aPat="Ort_.*.tif", aPrefix="ratio";
    int aScale = 1;
    bool Test=true;
    std::list<std::string> mLFile;
    std::vector<cImGeo> mLIm;

    ElInitArgMain
    (
        argc,argv,
        LArgMain()  << EAMC(aDir,"Ortho's Directory", eSAM_IsExistFile),
        LArgMain()  << EAM(aPat,"Pat",false,"Ortho's image pattern, def='Ort_.*'",eSAM_IsPatFile)
                    << EAM(aScale,"Scale",false,"Scale factor for both Orthoimages ; Def=1")
                    << EAM(Test,"T",false, "Test filtre des bords")
                    << EAM(aPrefix,"Prefix", false,"Prefix pour les ratio, default = ratio")

    );

    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(aDir);
    // create the list of images starting from the regular expression (Pattern)
    mLFile = aICNM->StdGetListOfFile(aPat);


    for (auto & imName : mLFile){
        //read Ortho

        cImGeo aIm(imName);
        std::cout << "nom image " << aIm.Name() << "\n";
        mLIm.push_back(aIm);
    }
    std::cout << mLFile.size() << " Ortho chargées.\n";

    //  tester si l'overlap est suffisant
    int i(0);
    for (auto aCurrentImGeo: mLIm)
    {
    i++;
    for (unsigned int j=i ; j<mLFile.size(); j++)
    {

        if (mLIm.at(j).Name()!=aCurrentImGeo.Name() && mLIm.at(j).overlap(&aCurrentImGeo,70))
        {

        Pt2di aTr=aCurrentImGeo.computeTrans(&mLIm.at(j));
        //std::string aName="ratio"+std::to_string(j)+"on"+std::to_string(j+1)+".tif";
        std::string aName=aPrefix+aCurrentImGeo.Name()+"on"+mLIm.at(j).Name()+".tif";
        // copie sur disque de l'image // pas très pertinent, je devrais plutot faire tout les calcul en ram puis sauver l'image à la fin avec un constructeur de cImGeo qui utilise une image RAM et les info du georef
        cImGeo        aImGeo(& aCurrentImGeo, aName);

        aImGeo.applyTrans(aTr);

        Im2D_REAL4 aIm=aImGeo.toRAM();
        Im2D_REAL4 aIm2(aIm.sz().x, aIm.sz().y);
        Im2D_REAL4 aImEmpty(aIm.sz().x, aIm.sz().y);
        Im2D_REAL4 aIm3=mLIm.at(j).toRAM();

        // l'image 1 n'as pas la meme taille, on la copie dans une image de meme dimension que l'im 0
        ELISE_COPY
                (
                    aIm3.all_pts(),
                    aIm3.in(),// l'image 1 n'as pas la meme taille, on la copie dans une image de meme dimension que l'im 0n(),
                    aIm2.oclip()
                    );

        // division de im 0 par im 1
        ELISE_COPY
                (
                    select(aIm.all_pts(),aIm2.in()>0),
                    (aIm.in())/(aIm2.in()),
                    aImEmpty.oclip()
                    );

        if (Test){
        // etape de dilation, effet de bord non désiré
        int it(0);
        do{

        Neighbourhood V8 = Neighbourhood::v8();
        Liste_Pts_INT2 l2(2);

        ELISE_COPY
        (
        dilate
        (
        select(aImEmpty.all_pts(),aImEmpty.in()==0),
        sel_func(V8,aImEmpty.in_proj()>0)
        ),
        1000,// je me fous de la valeur c'est pour créer un flux de points surtout
        aImEmpty.out() | l2 // il faut écrire et dans la liste de point, et dans l'image, sinon il va repecher plusieur fois le meme point
        );
        // j'enleve l'effet de bord , valleurs nulles
        ELISE_COPY
                (
                    l2.all_pts(),
                    0,
                    aImEmpty.out()
                    );

        it++;

        } while (it<3);

        }
        // je sauve mon image RAM dans mon image tif file
        aImGeo.updateTiffIm(&aImEmpty);

        // je calcule la moyenne du ratio
        int nbVal(0);
        double somme(0);
        for(int aI=0; aI<aImEmpty.sz().x; aI++)
        {
            for(int aJ=0; aJ<aImEmpty.sz().y; aJ++)
            {
                Pt2di aCoor(aI,aJ);
                double aValue = aImEmpty.GetR(aCoor);
                if (aValue!=0) {
                    somme +=aValue;
                    nbVal++;
                    //std::cout <<"Valeur:"<<aValue<< "\n";
                }
            }
            //fprintf(aFP,"\n");
        }
        somme/=nbVal;

        std::cout << "Ratio de l'image " << aCurrentImGeo.Name() << " sur l'image " << mLIm.at(j).Name() << "  caclulé, moyenne de  "<< somme << " ------------\n";
        // end if
        }
        // end boucle 1
    }
    // end boucle 2
    }
    return EXIT_SUCCESS;
}


int ComputeStat_main(int argc,char ** argv)
{
    double NoData(0);
    std::string aPat("Ree*.tif");
    std::list<std::string> mLFile;
    ElInitArgMain
            (
                argc,argv,
                LArgMain()  << EAMC(aPat,"Image pattern",eSAM_IsPatFile),
                LArgMain()  << EAM(NoData,"ND", "no data value, default 0")
                );

    std::cout <<" Debut\n";
    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc("./");
    // create the list of images starting from the regular expression (Pattern)
    mLFile = aICNM->StdGetListOfFile(aPat);

    std::cout << mLFile.size() << " images\n";
    double aMax(0), aMin(0);

    for (auto & aName : mLFile){

    Tiff_Im mTif=Tiff_Im::StdConvGen(aName,1,true);
    Im2D_REAL4 aImRAM(mTif.sz().x, mTif.sz().y);
    ELISE_COPY
            (
                mTif.all_pts(),
                mTif.in(),
                aImRAM.out()
                );

    // je calcule la moyenne du ratio
    int nbVal(0);
    bool firstVal=1;
    double somme(0),min,max(0);
    for(int aI=0; aI<aImRAM.sz().x; aI++)
    {
        for(int aJ=0; aJ<aImRAM.sz().y; aJ++)
        {
            Pt2di aCoor(aI,aJ);
            double aValue = aImRAM.GetR(aCoor);
            if (aValue!=NoData) {
                if (firstVal)
                {
                    min=aValue;
                    firstVal=0;
                }
                if (aValue<min) min=aValue;
                if (aValue>max) max=aValue;
                somme +=aValue;
                nbVal++;
            }
        }
    }
    somme/=nbVal;
    std::cout <<"Statistique Image "<<aName<< "\n";
    std::cout << "Nb value !=" << NoData << " :" << nbVal << "\n";
    std::cout << "Mean :" << somme <<"\n";
    std::cout << "Max :" << max <<"\n";
    std::cout << "Min :" << min <<"\n";
    std::cout << "Dynamique (max-min) :" << max-min <<"\n";

    // stat sur toutes les images
    if (mLFile.front()==aName)
    {
        aMin=min;
        aMax=max;
    }

    if (max>aMax) aMax=max;
    if (min<aMin) aMin=min;

}
    std::cout << "Max de toutes les images :" << aMax <<"\n";
    std::cout << "Min de toutes les iamges :" << aMin <<"\n";
    std::cout << "Dynamique (max-min) :" << aMax-aMin <<"\n";

    return EXIT_SUCCESS;
}


// j'ai utilisé saisieAppui pour saisir des points homologues sur plusieurs couples d'images TIR VIS orienté
// je dois manipuler le résulat pour le tranformer en set de points homologues pour un unique couple d'images
// de plus, la saisie sur les im TIR est effectué sur des images rééchantillonnées, il faut appliquer une homographie inverse au points saisi
int TransfoMesureAppuisVario2TP_main(int argc,char ** argv)
{
    std::string a2DMesFileName, aOutputFile1, aOutputFile2,aImName("AK100419.tif"), aNameMap, aDirHomol("Homol-Man");

    ElInitArgMain
    (
    argc,argv,
    //mandatory arguments
    LArgMain()  << EAMC(a2DMesFileName, "Input mes2D file",  eSAM_IsExistFile)
                << EAMC(aNameMap, "Input homography to apply to TIR images measurements",  eSAM_IsExistFile),
    LArgMain()  << EAM(aImName,"ImName", true, "Name of Image for output files",  eSAM_IsOutputFile)
                << EAM(aOutputFile1,"Out1", true,  "Output TP file 1, def Homol-Man/PastisTIR_ImName/VIS_ImName.txt",  eSAM_IsOutputFile)
                << EAM(aOutputFile2,"Out2", true,  "Output TP file 2, def Homol-Man/PastisVIS_ImName/TIR_ImName.txt",  eSAM_IsOutputFile)
    );

    if (!EAMIsInit(&aOutputFile1)) {
        aOutputFile1=aDirHomol + "/PastisTIR_" + aImName + "/VIS_" + aImName + ".txt";
        if(!ELISE_fp::IsDirectory(aDirHomol)) ELISE_fp::MkDir(aDirHomol);
        if(!ELISE_fp::IsDirectory(aDirHomol + "/PastisTIR_" + aImName)) ELISE_fp::MkDir(aDirHomol + "/PastisTIR_" + aImName);
    }
    if (!EAMIsInit(&aOutputFile2)) {
        aOutputFile2=aDirHomol + "/PastisVIS_" + aImName + "/TIR_" + aImName + ".txt";
        if(!ELISE_fp::IsDirectory(aDirHomol)) ELISE_fp::MkDir(aDirHomol);
        if(!ELISE_fp::IsDirectory(aDirHomol + "/PastisVIS_" + aImName)) ELISE_fp::MkDir(aDirHomol + "/PastisVIS_" + aImName);
    }

    // lecture de la map 2D
    cElMap2D * aMap = cElMap2D::FromFile(aNameMap);

    // conversion de la map 2D en homographie; map 2D: plus de paramètres que l'homographie

    //1) grille de pt sur le capteur thermique auquel on applique la map2D
    ElPackHomologue  aPackHomMap2Homogr;
    for (int y=0 ; y<720; y +=10)
        {
         for (int x=0 ; x<1200; x +=10)
            {
             Pt2dr aPt(x,y);
             Pt2dr aPt2 = (*aMap)(aPt);
             ElCplePtsHomologues Homol(aPt,aPt2);
             aPackHomMap2Homogr.Cple_Add(Homol);
            }
        }
    // convert Map2D to homography
    cElHomographie H(aPackHomMap2Homogr,true);
    //H = cElHomographie::RobustInit(qual,aPackHomImTer,bool Ok(1),1, 1.0,4);

    // initialiser le pack de points homologues
    ElPackHomologue  aPackHom;

    cSetOfMesureAppuisFlottants aSetOfMesureAppuisFlottants=StdGetFromPCP(a2DMesFileName,SetOfMesureAppuisFlottants);

    int count=0;

    for( std::list< cMesureAppuiFlottant1Im >::const_iterator iTmes1Im=aSetOfMesureAppuisFlottants.MesureAppuiFlottant1Im().begin();
         iTmes1Im!=aSetOfMesureAppuisFlottants.MesureAppuiFlottant1Im().end();          iTmes1Im++    )
    {
        cMesureAppuiFlottant1Im anImTIR=*iTmes1Im;

        //std::cout<<anImTIR.NameIm().substr(0,5)<<" \n";
        // pour chacune des images thermique rééchantillonnée, recherche l'image visible associée
        if (anImTIR.NameIm().substr(0,5)=="Reech")
        {
            //std::cout<<anImTIR.NameIm()<<" \n";


            for (auto anImVIS : aSetOfMesureAppuisFlottants.MesureAppuiFlottant1Im()) {
            // ne fonctionne que pour la convention de préfixe Reech_TIR_ et VIS_
               if(anImTIR.NameIm().substr(10,anImTIR.NameIm().size()) == anImVIS.NameIm().substr(4,anImVIS.NameIm().size()))
               {
                   // j'ai un couple d'image.
                   //std::cout << "Couple d'images " << anImTIR.NameIm() << " et " <<anImVIS.NameIm() << "\n";

                   for (auto & appuiTIR : anImTIR.OneMesureAF1I())
                   {
                   //
                       for (auto & appuiVIS : anImVIS.OneMesureAF1I())
                       {
                       if (appuiTIR.NamePt()==appuiVIS.NamePt())
                       {
                           // j'ai 2 mesures pour ce point
                          // std::cout << "Pt " << appuiTIR.NamePt() << ", " <<appuiTIR.PtIm() << " --> " << appuiVIS.PtIm() << "\n";

                           // J'ajoute ce point au set de points homol
                           ElCplePtsHomologues Homol(appuiTIR.PtIm(),appuiVIS.PtIm());

                           aPackHom.Cple_Add(Homol);

                           count++;
                           break;
                       }
                       }
                   }
                   break;
               }
            }
       }

    // fin iter sur les mesures appuis flottant
    }
    std::cout << "Total : " << count << " tie points read \n" ;

    if (!EAMIsInit(&aOutputFile1) && !EAMIsInit(&aOutputFile2))
    {
    if(!ELISE_fp::IsDirectory(aDirHomol + "/PastisReech_TIR_" + aImName)) ELISE_fp::MkDir(aDirHomol + "/PastisReech_TIR_" + aImName);
    std::cout << "Homol pack saved in  : " << aDirHomol + "/PastisReech_TIR_" + aImName + "/VIS_" + aImName + ".txt" << " \n" ;
    aPackHom.StdPutInFile(aDirHomol + "/PastisReech_TIR_" + aImName + "/VIS_" + aImName + ".txt");
    aPackHom.SelfSwap();

    std::cout << "Homol pack saved in  : " << aDirHomol + "/PastisVIS_" + aImName + "/Reech_TIR_" + aImName + ".txt" << " \n" ;
    aPackHom.StdPutInFile(aDirHomol + "/PastisVIS_" + aImName + "/Reech_TIR_" + aImName + ".txt");

    }


    // appliquer l'homographie

    //aPackHom.ApplyHomographies(H.Inverse(),H.Id());
    aPackHom.ApplyHomographies(H,H.Id());
    // maintenant on sauve ce pack de points homologues
    std::cout << "Homol pack saved in  : " << aOutputFile1 << " \n" ;
    aPackHom.StdPutInFile(aOutputFile1);
    aPackHom.SelfSwap();
    std::cout << "Homol pack saved in  : " << aOutputFile2 << " \n" ;
    aPackHom.StdPutInFile(aOutputFile2);
}


/* j'ai saisi des points d'appuis sur un vol 2 altitudes thermiques, j'aimerai voir si cette radiance est corrélée à
-Distance entre sensor et object  -->NON
-angle --> PAS TESTE
moins probable mais je teste quand même:
-position sur capteur --> NON
-temps écoulé depuis début du vol PAS TESTE
 */
int statRadianceVarioCam_main(int argc,char ** argv)
{
    std::string a2DMesFileName, a3DMesFileName, aOutputFile, aOri;

    ElInitArgMain
            (
                argc,argv,
                //mandatory arguments
                LArgMain()  << EAMC(a2DMesFileName, "Input mes2D file",  eSAM_IsExistFile)
                << EAMC(a3DMesFileName, "Input mes3D file",  eSAM_IsExistFile)
                << EAMC(aOri, "Orientation",  eSAM_IsExistDirOri),
                LArgMain()
                << EAM(aOutputFile,"Out", true,  "Output .txt file with radiance observation for statistic",  eSAM_IsOutputFile)

                );
    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc("./");

    // open 2D measures
    cSetOfMesureAppuisFlottants aSetOfMesureAppuisFlottants=StdGetFromPCP(a2DMesFileName,SetOfMesureAppuisFlottants);
    // open 3D measures
    cDicoAppuisFlottant DAF=  StdGetFromPCP(a3DMesFileName,DicoAppuisFlottant);
    std::list<cOneAppuisDAF> & aLGCP =  DAF.OneAppuisDAF();

    // create a map of GCP and position
    std::map<std::string, Pt3dr> aGCPmap;

    for (auto & GCP : aLGCP)
    {
        aGCPmap[GCP.NamePt()]=Pt3dr(GCP.Pt().x,GCP.Pt().y,GCP.Pt().z);
    }


    std::cout << "Image GCP U V rayon Radiance GroundDist \n" ;


    for( std::list< cMesureAppuiFlottant1Im >::const_iterator iTmes1Im=aSetOfMesureAppuisFlottants.MesureAppuiFlottant1Im().begin();
         iTmes1Im!=aSetOfMesureAppuisFlottants.MesureAppuiFlottant1Im().end();          iTmes1Im++    )
    {
        cMesureAppuiFlottant1Im anImTIR=*iTmes1Im;

        // open the image
        if (ELISE_fp::exist_file(anImTIR.NameIm()))
        {

            Tiff_Im mTifIn=Tiff_Im::StdConvGen(anImTIR.NameIm(),1,true);
            // create empty RAM image
            Im2D_REAL4 im(mTifIn.sz().x,mTifIn.sz().y);
            // fill it with tiff image value
            ELISE_COPY(
                        mTifIn.all_pts(),
                        mTifIn.in(),
                        im.out()
                        );
            // open the CamStenope
            std::string aNameOri=aICNM->Assoc1To1("NKS-Assoc-Im2Orient@-"+aOri+"@",anImTIR.NameIm(), true);
            CamStenope * aCam(CamOrientGenFromFile(aNameOri,aICNM));
            //std::cout << "Optical center Cam" << aCam->PseudoOpticalCenter() << "\n";
            // loop on the
            for (auto & appuiTIR : anImTIR.OneMesureAF1I())
            {
                // je garde uniquement les GCP dont le nom commence par L
                if (appuiTIR.NamePt().substr(0,1)=="L")
                   {
                    Pt3dr pt = aGCPmap[appuiTIR.NamePt()];
                   // std::cout << " Image " << anImTIR.NameIm() << " GCP " << appuiTIR.NamePt() << " ground position " << pt << " Image position " << appuiTIR.PtIm() << " \n";

                    double aRadiance(0);
                    int aNb(0);

                    Pt2di UV(appuiTIR.PtIm());
                    Pt2di sz(1,1);
                    ELISE_COPY(
                                rectangle(UV-sz,UV+Pt2di(2,2)*sz),// not sure how it work
                                Virgule(im.in(),1),
                                Virgule(sigma(aRadiance),sigma(aNb))
                                );
                    aRadiance/=aNb;
                    std::cout << " Radiance on windows of " << aNb << " px " << aRadiance << " \n";
                    aRadiance=aRadiance-27315;
                    // now determine incid and distance from camera to GCP

                    double aDist(0);
                    Pt3dr vDist=aCam->PseudoOpticalCenter()-pt;
                    aDist=euclid(vDist);

                    double aDistUV=euclid(appuiTIR.PtIm()-aCam->PP());


                    std::cout << anImTIR.NameIm() << " " << appuiTIR.NamePt() << " " << appuiTIR.PtIm().x   <<  " " << appuiTIR.PtIm().y  << " " << aDistUV << " " << aRadiance << " " << aDist << " \n";

                    }
                }

        }
        // fin iter sur les mesures appuis flottant
    }
}


int MasqTIR_main(int argc,char ** argv)
{
    std::string aDir, aPat="Ort_.*.tif";
    std::list<std::string> mLFile;
    std::vector<cImGeo> mLIm;

    ElInitArgMain
    (
        argc,argv,
        LArgMain()  << EAMC(aDir,"Ortho's Directory", eSAM_IsExistFile),
        LArgMain()  << EAM(aPat,"Pat",false,"Ortho's image pattern, def='Ort_.*'",eSAM_IsPatFile)

    );

    cInterfChantierNameManipulateur * aICNM = cInterfChantierNameManipulateur::BasicAlloc(aDir);
    // create the list of images starting from the regular expression (Pattern)
    mLFile = aICNM->StdGetListOfFile(aPat);

    for (auto & aName : mLFile)
    {

    Tiff_Im im=Tiff_Im::StdConvGen("../OrthoTIR025/"+aName,1,true);

    std::string filenamePC= "PC"+aName.substr(3, aName.size()) ;

  /*
    //255: masqué. 0: ok
    Im2D_U_INT1 out(im.sz().x,im.sz().y);
    Im2D_REAL4 tmp(im.sz().x,im.sz().y);


    int minRad(27540), rangeRad(2546.0);


    ELISE_COPY
    (
    im.all_pts(),
    im.in(),
    tmp.out()
    );

    ELISE_COPY
    (
    select(tmp.all_pts(), tmp.in()>minRad && tmp.in()<minRad+rangeRad && tmp.in()!=0),
    255*(tmp.in()-minRad)/rangeRad,
    out.out()
    );

    ELISE_COPY
    (
    select(tmp.all_pts(), tmp.in()==0),
    0,
    out.out()
    );




    for (int v(0); v<tmp.sz().y;v++)
    {
        for (int u(0); u<tmp.sz().x;u++)
        {
            Pt2di pt(u,v);
            double aVal = tmp.GetR(pt);
            unsigned int v(0);

            if(aVal!=0){
            if (aVal>minRad && aVal <minRad+rangeRad)
            {
                v=255.0*(aVal-minRad)/rangeRad;
            }
            }

            out.SetR(pt,v);
            //std::cout << "aVal a la position " << pt << " vaut " << aVal << ", transfo en " << v <<"\n";
        }
    }






    std::cout << "je sauve l'image " << aName << "\n";
    ELISE_COPY
    (
        out.all_pts(),
        out.in(0),
        Tiff_Im(
            aName.c_str(),
            out.sz(),
            GenIm::u_int1,
            Tiff_Im::No_Compr,
            Tiff_Im::BlackIsZero
            ).out()
    );

*/
     Im2D_REAL4 masq(im.sz().x,im.sz().y);

    ELISE_COPY
    (
    im.all_pts(),
    im.in(0),
    masq.oclip()
    );


     std::cout << "détecte le bord pour image  " << filenamePC << "\n";
    int it(0);
    do{

    Neighbourhood V8 = Neighbourhood::v8();
    Liste_Pts_INT2 l2(2);

    ELISE_COPY
    (
    dilate
    (
    select(masq.all_pts(),masq.in()==0),
    sel_func(V8,masq.in_proj()>0)
    ),
    1000,// je me fous de la valeur c'est pour créer un flux de points surtout
    masq.oclip() | l2 // il faut écrire et dans la liste de point, et dans l'image, sinon il va repecher plusieur fois le meme point
    );
    // j'enleve l'effet de bord , valleurs nulles
    ELISE_COPY
            (
                l2.all_pts(),
                0,
                masq.oclip()
                );

    it++;

    } while (it<3);


/*
    // attention, écrase le ficher existant, pas propre ça
    std::cout << "je sauve l'image avec correction radiométrique " << aName << "\n";
    ELISE_COPY
    (
        masq.all_pts(),
        masq.in(0),
        Tiff_Im(
            aName.c_str(),
            masq.sz(),
            GenIm::int1,
            Tiff_Im::No_Compr,
            Tiff_Im::BlackIsZero
            ).out()
    );

*/
    ELISE_COPY
    (
    select(masq.all_pts(),masq.in()==0),
    255,
    masq.oclip()
    );

    ELISE_COPY
    (
    select(masq.all_pts(),masq.in()!=255),
    0,
    masq.oclip()
    );


    std::cout << "je sauve l'image les parties cachées " << filenamePC << "\n";
    ELISE_COPY
    (
        masq.all_pts(),
        masq.in(0),
        Tiff_Im(
            filenamePC.c_str(),
            masq.sz(),
            GenIm::u_int1,
            Tiff_Im::No_Compr,
            Tiff_Im::BlackIsZero
            ).out()
    );
    }
    return EXIT_SUCCESS;
}


int main_test2(int argc,char ** argv)
{
     //cORT_Appli anAppli(argc,argv);
     //CmpOrthosTir_main(argc,argv);
    //ComputeStat_main(argc,argv);
    //RegTIRVIS_main(argc,argv);
    //test_main(argc,argv);
    //MasqTIR_main(argc,argv);
    //cCoreg2Ortho(argc,argv);
    cFeatheringAndMosaicOrtho(argc,argv);
    //cOriTran_Appli(argc,argv);
    //TransfoMesureAppuisVario2TP_main(argc,argv);
    //statRadianceVarioCam_main(argc,argv);

   return EXIT_SUCCESS;
}


int main_testold(int argc,char ** argv)
{
 // manipulate the
    ofstream fout("/home/lisein/data/DIDRO/lp17/GPS_RGP/GNSS_pos/test.obs");
    ifstream fin("/home/lisein/data/DIDRO/lp17/GPS_RGP/GNSS_pos/tmp.txt");
    string line;

    std::string add("          40.000        40.000\n");
    add="\t40.000\t40.000\n";
      if (fin.is_open())
      {
        unsigned int i(0);
        while ( getline(fin,line) )
        {
          i++;

          if (i==17){
              i=0;
              fout << line << add;

          } else {

          if (i%2==1 && i>2)  {
               fout << line << add;
          } else {
              fout << line <<"\n";}
          }
        }

       } else { std:cout << "cannot open file in\n";}
        fin.close();
        fout.close();

   return EXIT_SUCCESS;
}

int VarioCamTo8Bits_main(int argc,char ** argv)
{

    cVarioCamTo8Bits(argc,argv);

   return EXIT_SUCCESS;
}




/*Footer-MicMac-eLiSe-25/06/2007

Ce logiciel est un programme informatique servant à la mise en
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

A cet égard  l'attention de l'ucApplitilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL-B, et que vous en avez accepté les
termes.
Footer-MicMac-eLiSe-25/06/2007*/
