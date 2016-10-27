// File Automatically generated by eLiSe
#include "general/all.h"
#include "private/all.h"
#include "cEqAppui_NoDist__GL__PTInc_M2CPolyn6.h"


cEqAppui_NoDist__GL__PTInc_M2CPolyn6::cEqAppui_NoDist__GL__PTInc_M2CPolyn6():
    cElCompiledFonc(2)
{
   AddIntRef (cIncIntervale("Intr",0,53));
   AddIntRef (cIncIntervale("Orient",53,59));
   AddIntRef (cIncIntervale("Tmp_PTer",59,62));
   Close(false);
}



void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::ComputeVal()
{
   double tmp0_ = mCompCoord[53];
   double tmp1_ = mCompCoord[54];
   double tmp2_ = cos(tmp1_);
   double tmp3_ = cos(tmp0_);
   double tmp4_ = tmp3_*tmp2_;
   double tmp5_ = sin(tmp0_);
   double tmp6_ = tmp5_*tmp2_;
   double tmp7_ = sin(tmp1_);
   double tmp8_ = mCompCoord[55];
   double tmp9_ = sin(tmp8_);
   double tmp10_ = -(tmp9_);
   double tmp11_ = -(tmp7_);
   double tmp12_ = cos(tmp8_);
   double tmp13_ = mCompCoord[59];
   double tmp14_ = mCompCoord[56];
   double tmp15_ = tmp13_-tmp14_;
   double tmp16_ = -(tmp5_);
   double tmp17_ = tmp16_*tmp10_;
   double tmp18_ = tmp3_*tmp11_;
   double tmp19_ = tmp18_*tmp12_;
   double tmp20_ = tmp17_+tmp19_;
   double tmp21_ = tmp3_*tmp10_;
   double tmp22_ = tmp5_*tmp11_;
   double tmp23_ = tmp22_*tmp12_;
   double tmp24_ = tmp21_+tmp23_;
   double tmp25_ = tmp2_*tmp12_;
   double tmp26_ = mCompCoord[60];
   double tmp27_ = mCompCoord[57];
   double tmp28_ = tmp26_-tmp27_;
   double tmp29_ = mCompCoord[61];
   double tmp30_ = mCompCoord[58];
   double tmp31_ = tmp29_-tmp30_;
   double tmp32_ = tmp16_*tmp12_;
   double tmp33_ = tmp18_*tmp9_;
   double tmp34_ = tmp32_+tmp33_;
   double tmp35_ = tmp3_*tmp12_;
   double tmp36_ = tmp22_*tmp9_;
   double tmp37_ = tmp35_+tmp36_;
   double tmp38_ = tmp2_*tmp9_;
   double tmp39_ = (tmp20_)*mLocGL_0_0;
   double tmp40_ = (tmp24_)*mLocGL_1_0;
   double tmp41_ = tmp39_+tmp40_;
   double tmp42_ = tmp25_*mLocGL_2_0;
   double tmp43_ = tmp41_+tmp42_;
   double tmp44_ = (tmp43_)*(tmp15_);
   double tmp45_ = (tmp20_)*mLocGL_0_1;
   double tmp46_ = (tmp24_)*mLocGL_1_1;
   double tmp47_ = tmp45_+tmp46_;
   double tmp48_ = tmp25_*mLocGL_2_1;
   double tmp49_ = tmp47_+tmp48_;
   double tmp50_ = (tmp49_)*(tmp28_);
   double tmp51_ = tmp44_+tmp50_;
   double tmp52_ = (tmp20_)*mLocGL_0_2;
   double tmp53_ = (tmp24_)*mLocGL_1_2;
   double tmp54_ = tmp52_+tmp53_;
   double tmp55_ = tmp25_*mLocGL_2_2;
   double tmp56_ = tmp54_+tmp55_;
   double tmp57_ = (tmp56_)*(tmp31_);
   double tmp58_ = tmp51_+tmp57_;
   double tmp59_ = tmp4_*mLocGL_0_0;
   double tmp60_ = tmp6_*mLocGL_1_0;
   double tmp61_ = tmp59_+tmp60_;
   double tmp62_ = tmp7_*mLocGL_2_0;
   double tmp63_ = tmp61_+tmp62_;
   double tmp64_ = (tmp63_)*(tmp15_);
   double tmp65_ = tmp4_*mLocGL_0_1;
   double tmp66_ = tmp6_*mLocGL_1_1;
   double tmp67_ = tmp65_+tmp66_;
   double tmp68_ = tmp7_*mLocGL_2_1;
   double tmp69_ = tmp67_+tmp68_;
   double tmp70_ = (tmp69_)*(tmp28_);
   double tmp71_ = tmp64_+tmp70_;
   double tmp72_ = tmp4_*mLocGL_0_2;
   double tmp73_ = tmp6_*mLocGL_1_2;
   double tmp74_ = tmp72_+tmp73_;
   double tmp75_ = tmp7_*mLocGL_2_2;
   double tmp76_ = tmp74_+tmp75_;
   double tmp77_ = (tmp76_)*(tmp31_);
   double tmp78_ = tmp71_+tmp77_;
   double tmp79_ = (tmp78_)/(tmp58_);
   double tmp80_ = (tmp34_)*mLocGL_0_0;
   double tmp81_ = (tmp37_)*mLocGL_1_0;
   double tmp82_ = tmp80_+tmp81_;
   double tmp83_ = tmp38_*mLocGL_2_0;
   double tmp84_ = tmp82_+tmp83_;
   double tmp85_ = (tmp84_)*(tmp15_);
   double tmp86_ = (tmp34_)*mLocGL_0_1;
   double tmp87_ = (tmp37_)*mLocGL_1_1;
   double tmp88_ = tmp86_+tmp87_;
   double tmp89_ = tmp38_*mLocGL_2_1;
   double tmp90_ = tmp88_+tmp89_;
   double tmp91_ = (tmp90_)*(tmp28_);
   double tmp92_ = tmp85_+tmp91_;
   double tmp93_ = (tmp34_)*mLocGL_0_2;
   double tmp94_ = (tmp37_)*mLocGL_1_2;
   double tmp95_ = tmp93_+tmp94_;
   double tmp96_ = tmp38_*mLocGL_2_2;
   double tmp97_ = tmp95_+tmp96_;
   double tmp98_ = (tmp97_)*(tmp31_);
   double tmp99_ = tmp92_+tmp98_;
   double tmp100_ = (tmp99_)/(tmp58_);

  mVal[0] = (mLocNDP0_x+mLocNDdx_x*(tmp79_)+mLocNDdy_x*(tmp100_))-mLocXIm;

  mVal[1] = (mLocNDP0_y+mLocNDdx_y*(tmp79_)+mLocNDdy_y*(tmp100_))-mLocYIm;

}


void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::ComputeValDeriv()
{
   double tmp0_ = mCompCoord[53];
   double tmp1_ = mCompCoord[54];
   double tmp2_ = cos(tmp1_);
   double tmp3_ = cos(tmp0_);
   double tmp4_ = tmp3_*tmp2_;
   double tmp5_ = sin(tmp0_);
   double tmp6_ = tmp5_*tmp2_;
   double tmp7_ = sin(tmp1_);
   double tmp8_ = mCompCoord[55];
   double tmp9_ = sin(tmp8_);
   double tmp10_ = -(tmp9_);
   double tmp11_ = -(tmp7_);
   double tmp12_ = cos(tmp8_);
   double tmp13_ = mCompCoord[59];
   double tmp14_ = mCompCoord[56];
   double tmp15_ = tmp13_-tmp14_;
   double tmp16_ = -(tmp5_);
   double tmp17_ = tmp16_*tmp10_;
   double tmp18_ = tmp3_*tmp11_;
   double tmp19_ = tmp18_*tmp12_;
   double tmp20_ = tmp17_+tmp19_;
   double tmp21_ = tmp3_*tmp10_;
   double tmp22_ = tmp5_*tmp11_;
   double tmp23_ = tmp22_*tmp12_;
   double tmp24_ = tmp21_+tmp23_;
   double tmp25_ = tmp2_*tmp12_;
   double tmp26_ = mCompCoord[60];
   double tmp27_ = mCompCoord[57];
   double tmp28_ = tmp26_-tmp27_;
   double tmp29_ = mCompCoord[61];
   double tmp30_ = mCompCoord[58];
   double tmp31_ = tmp29_-tmp30_;
   double tmp32_ = tmp16_*tmp12_;
   double tmp33_ = tmp18_*tmp9_;
   double tmp34_ = tmp32_+tmp33_;
   double tmp35_ = tmp3_*tmp12_;
   double tmp36_ = tmp22_*tmp9_;
   double tmp37_ = tmp35_+tmp36_;
   double tmp38_ = tmp2_*tmp9_;
   double tmp39_ = (tmp20_)*mLocGL_0_0;
   double tmp40_ = (tmp24_)*mLocGL_1_0;
   double tmp41_ = tmp39_+tmp40_;
   double tmp42_ = tmp25_*mLocGL_2_0;
   double tmp43_ = tmp41_+tmp42_;
   double tmp44_ = (tmp43_)*(tmp15_);
   double tmp45_ = (tmp20_)*mLocGL_0_1;
   double tmp46_ = (tmp24_)*mLocGL_1_1;
   double tmp47_ = tmp45_+tmp46_;
   double tmp48_ = tmp25_*mLocGL_2_1;
   double tmp49_ = tmp47_+tmp48_;
   double tmp50_ = (tmp49_)*(tmp28_);
   double tmp51_ = tmp44_+tmp50_;
   double tmp52_ = (tmp20_)*mLocGL_0_2;
   double tmp53_ = (tmp24_)*mLocGL_1_2;
   double tmp54_ = tmp52_+tmp53_;
   double tmp55_ = tmp25_*mLocGL_2_2;
   double tmp56_ = tmp54_+tmp55_;
   double tmp57_ = (tmp56_)*(tmp31_);
   double tmp58_ = tmp51_+tmp57_;
   double tmp59_ = -(1);
   double tmp60_ = tmp59_*tmp5_;
   double tmp61_ = tmp60_*tmp2_;
   double tmp62_ = tmp4_*mLocGL_0_0;
   double tmp63_ = tmp6_*mLocGL_1_0;
   double tmp64_ = tmp62_+tmp63_;
   double tmp65_ = tmp7_*mLocGL_2_0;
   double tmp66_ = tmp64_+tmp65_;
   double tmp67_ = (tmp66_)*(tmp15_);
   double tmp68_ = tmp4_*mLocGL_0_1;
   double tmp69_ = tmp6_*mLocGL_1_1;
   double tmp70_ = tmp68_+tmp69_;
   double tmp71_ = tmp7_*mLocGL_2_1;
   double tmp72_ = tmp70_+tmp71_;
   double tmp73_ = (tmp72_)*(tmp28_);
   double tmp74_ = tmp67_+tmp73_;
   double tmp75_ = tmp4_*mLocGL_0_2;
   double tmp76_ = tmp6_*mLocGL_1_2;
   double tmp77_ = tmp75_+tmp76_;
   double tmp78_ = tmp7_*mLocGL_2_2;
   double tmp79_ = tmp77_+tmp78_;
   double tmp80_ = (tmp79_)*(tmp31_);
   double tmp81_ = tmp74_+tmp80_;
   double tmp82_ = -(tmp3_);
   double tmp83_ = tmp82_*tmp10_;
   double tmp84_ = tmp60_*tmp11_;
   double tmp85_ = tmp84_*tmp12_;
   double tmp86_ = tmp83_+tmp85_;
   double tmp87_ = tmp60_*tmp10_;
   double tmp88_ = tmp87_+tmp19_;
   double tmp89_ = tmp82_*tmp12_;
   double tmp90_ = tmp84_*tmp9_;
   double tmp91_ = tmp89_+tmp90_;
   double tmp92_ = tmp60_*tmp12_;
   double tmp93_ = tmp92_+tmp33_;
   double tmp94_ = (tmp34_)*mLocGL_0_0;
   double tmp95_ = (tmp37_)*mLocGL_1_0;
   double tmp96_ = tmp94_+tmp95_;
   double tmp97_ = tmp38_*mLocGL_2_0;
   double tmp98_ = tmp96_+tmp97_;
   double tmp99_ = (tmp98_)*(tmp15_);
   double tmp100_ = (tmp34_)*mLocGL_0_1;
   double tmp101_ = (tmp37_)*mLocGL_1_1;
   double tmp102_ = tmp100_+tmp101_;
   double tmp103_ = tmp38_*mLocGL_2_1;
   double tmp104_ = tmp102_+tmp103_;
   double tmp105_ = (tmp104_)*(tmp28_);
   double tmp106_ = tmp99_+tmp105_;
   double tmp107_ = (tmp34_)*mLocGL_0_2;
   double tmp108_ = (tmp37_)*mLocGL_1_2;
   double tmp109_ = tmp107_+tmp108_;
   double tmp110_ = tmp38_*mLocGL_2_2;
   double tmp111_ = tmp109_+tmp110_;
   double tmp112_ = (tmp111_)*(tmp31_);
   double tmp113_ = tmp106_+tmp112_;
   double tmp114_ = (tmp86_)*mLocGL_0_0;
   double tmp115_ = (tmp88_)*mLocGL_1_0;
   double tmp116_ = tmp114_+tmp115_;
   double tmp117_ = (tmp116_)*(tmp15_);
   double tmp118_ = (tmp86_)*mLocGL_0_1;
   double tmp119_ = (tmp88_)*mLocGL_1_1;
   double tmp120_ = tmp118_+tmp119_;
   double tmp121_ = (tmp120_)*(tmp28_);
   double tmp122_ = tmp117_+tmp121_;
   double tmp123_ = (tmp86_)*mLocGL_0_2;
   double tmp124_ = (tmp88_)*mLocGL_1_2;
   double tmp125_ = tmp123_+tmp124_;
   double tmp126_ = (tmp125_)*(tmp31_);
   double tmp127_ = tmp122_+tmp126_;
   double tmp128_ = ElSquare(tmp58_);
   double tmp129_ = tmp59_*tmp7_;
   double tmp130_ = tmp129_*tmp3_;
   double tmp131_ = tmp129_*tmp5_;
   double tmp132_ = -(tmp2_);
   double tmp133_ = tmp132_*tmp3_;
   double tmp134_ = tmp133_*tmp12_;
   double tmp135_ = tmp132_*tmp5_;
   double tmp136_ = tmp135_*tmp12_;
   double tmp137_ = tmp129_*tmp12_;
   double tmp138_ = tmp133_*tmp9_;
   double tmp139_ = tmp135_*tmp9_;
   double tmp140_ = tmp129_*tmp9_;
   double tmp141_ = tmp134_*mLocGL_0_0;
   double tmp142_ = tmp136_*mLocGL_1_0;
   double tmp143_ = tmp141_+tmp142_;
   double tmp144_ = tmp137_*mLocGL_2_0;
   double tmp145_ = tmp143_+tmp144_;
   double tmp146_ = (tmp145_)*(tmp15_);
   double tmp147_ = tmp134_*mLocGL_0_1;
   double tmp148_ = tmp136_*mLocGL_1_1;
   double tmp149_ = tmp147_+tmp148_;
   double tmp150_ = tmp137_*mLocGL_2_1;
   double tmp151_ = tmp149_+tmp150_;
   double tmp152_ = (tmp151_)*(tmp28_);
   double tmp153_ = tmp146_+tmp152_;
   double tmp154_ = tmp134_*mLocGL_0_2;
   double tmp155_ = tmp136_*mLocGL_1_2;
   double tmp156_ = tmp154_+tmp155_;
   double tmp157_ = tmp137_*mLocGL_2_2;
   double tmp158_ = tmp156_+tmp157_;
   double tmp159_ = (tmp158_)*(tmp31_);
   double tmp160_ = tmp153_+tmp159_;
   double tmp161_ = -(tmp12_);
   double tmp162_ = tmp59_*tmp9_;
   double tmp163_ = tmp161_*tmp16_;
   double tmp164_ = tmp162_*tmp18_;
   double tmp165_ = tmp163_+tmp164_;
   double tmp166_ = tmp161_*tmp3_;
   double tmp167_ = tmp162_*tmp22_;
   double tmp168_ = tmp166_+tmp167_;
   double tmp169_ = tmp162_*tmp2_;
   double tmp170_ = tmp162_*tmp16_;
   double tmp171_ = tmp12_*tmp18_;
   double tmp172_ = tmp170_+tmp171_;
   double tmp173_ = tmp162_*tmp3_;
   double tmp174_ = tmp12_*tmp22_;
   double tmp175_ = tmp173_+tmp174_;
   double tmp176_ = tmp12_*tmp2_;
   double tmp177_ = (tmp165_)*mLocGL_0_0;
   double tmp178_ = (tmp168_)*mLocGL_1_0;
   double tmp179_ = tmp177_+tmp178_;
   double tmp180_ = tmp169_*mLocGL_2_0;
   double tmp181_ = tmp179_+tmp180_;
   double tmp182_ = (tmp181_)*(tmp15_);
   double tmp183_ = (tmp165_)*mLocGL_0_1;
   double tmp184_ = (tmp168_)*mLocGL_1_1;
   double tmp185_ = tmp183_+tmp184_;
   double tmp186_ = tmp169_*mLocGL_2_1;
   double tmp187_ = tmp185_+tmp186_;
   double tmp188_ = (tmp187_)*(tmp28_);
   double tmp189_ = tmp182_+tmp188_;
   double tmp190_ = (tmp165_)*mLocGL_0_2;
   double tmp191_ = (tmp168_)*mLocGL_1_2;
   double tmp192_ = tmp190_+tmp191_;
   double tmp193_ = tmp169_*mLocGL_2_2;
   double tmp194_ = tmp192_+tmp193_;
   double tmp195_ = (tmp194_)*(tmp31_);
   double tmp196_ = tmp189_+tmp195_;
   double tmp197_ = tmp59_*(tmp43_);
   double tmp198_ = tmp59_*(tmp49_);
   double tmp199_ = tmp59_*(tmp56_);
   double tmp200_ = (tmp81_)/(tmp58_);
   double tmp201_ = (tmp113_)/(tmp58_);
   double tmp202_ = tmp61_*mLocGL_0_0;
   double tmp203_ = tmp4_*mLocGL_1_0;
   double tmp204_ = tmp202_+tmp203_;
   double tmp205_ = (tmp204_)*(tmp15_);
   double tmp206_ = tmp61_*mLocGL_0_1;
   double tmp207_ = tmp4_*mLocGL_1_1;
   double tmp208_ = tmp206_+tmp207_;
   double tmp209_ = (tmp208_)*(tmp28_);
   double tmp210_ = tmp205_+tmp209_;
   double tmp211_ = tmp61_*mLocGL_0_2;
   double tmp212_ = tmp4_*mLocGL_1_2;
   double tmp213_ = tmp211_+tmp212_;
   double tmp214_ = (tmp213_)*(tmp31_);
   double tmp215_ = tmp210_+tmp214_;
   double tmp216_ = (tmp215_)*(tmp58_);
   double tmp217_ = (tmp81_)*(tmp127_);
   double tmp218_ = tmp216_-tmp217_;
   double tmp219_ = (tmp218_)/tmp128_;
   double tmp220_ = (tmp91_)*mLocGL_0_0;
   double tmp221_ = (tmp93_)*mLocGL_1_0;
   double tmp222_ = tmp220_+tmp221_;
   double tmp223_ = (tmp222_)*(tmp15_);
   double tmp224_ = (tmp91_)*mLocGL_0_1;
   double tmp225_ = (tmp93_)*mLocGL_1_1;
   double tmp226_ = tmp224_+tmp225_;
   double tmp227_ = (tmp226_)*(tmp28_);
   double tmp228_ = tmp223_+tmp227_;
   double tmp229_ = (tmp91_)*mLocGL_0_2;
   double tmp230_ = (tmp93_)*mLocGL_1_2;
   double tmp231_ = tmp229_+tmp230_;
   double tmp232_ = (tmp231_)*(tmp31_);
   double tmp233_ = tmp228_+tmp232_;
   double tmp234_ = (tmp233_)*(tmp58_);
   double tmp235_ = (tmp113_)*(tmp127_);
   double tmp236_ = tmp234_-tmp235_;
   double tmp237_ = (tmp236_)/tmp128_;
   double tmp238_ = tmp130_*mLocGL_0_0;
   double tmp239_ = tmp131_*mLocGL_1_0;
   double tmp240_ = tmp238_+tmp239_;
   double tmp241_ = tmp2_*mLocGL_2_0;
   double tmp242_ = tmp240_+tmp241_;
   double tmp243_ = (tmp242_)*(tmp15_);
   double tmp244_ = tmp130_*mLocGL_0_1;
   double tmp245_ = tmp131_*mLocGL_1_1;
   double tmp246_ = tmp244_+tmp245_;
   double tmp247_ = tmp2_*mLocGL_2_1;
   double tmp248_ = tmp246_+tmp247_;
   double tmp249_ = (tmp248_)*(tmp28_);
   double tmp250_ = tmp243_+tmp249_;
   double tmp251_ = tmp130_*mLocGL_0_2;
   double tmp252_ = tmp131_*mLocGL_1_2;
   double tmp253_ = tmp251_+tmp252_;
   double tmp254_ = tmp2_*mLocGL_2_2;
   double tmp255_ = tmp253_+tmp254_;
   double tmp256_ = (tmp255_)*(tmp31_);
   double tmp257_ = tmp250_+tmp256_;
   double tmp258_ = (tmp257_)*(tmp58_);
   double tmp259_ = (tmp81_)*(tmp160_);
   double tmp260_ = tmp258_-tmp259_;
   double tmp261_ = (tmp260_)/tmp128_;
   double tmp262_ = tmp138_*mLocGL_0_0;
   double tmp263_ = tmp139_*mLocGL_1_0;
   double tmp264_ = tmp262_+tmp263_;
   double tmp265_ = tmp140_*mLocGL_2_0;
   double tmp266_ = tmp264_+tmp265_;
   double tmp267_ = (tmp266_)*(tmp15_);
   double tmp268_ = tmp138_*mLocGL_0_1;
   double tmp269_ = tmp139_*mLocGL_1_1;
   double tmp270_ = tmp268_+tmp269_;
   double tmp271_ = tmp140_*mLocGL_2_1;
   double tmp272_ = tmp270_+tmp271_;
   double tmp273_ = (tmp272_)*(tmp28_);
   double tmp274_ = tmp267_+tmp273_;
   double tmp275_ = tmp138_*mLocGL_0_2;
   double tmp276_ = tmp139_*mLocGL_1_2;
   double tmp277_ = tmp275_+tmp276_;
   double tmp278_ = tmp140_*mLocGL_2_2;
   double tmp279_ = tmp277_+tmp278_;
   double tmp280_ = (tmp279_)*(tmp31_);
   double tmp281_ = tmp274_+tmp280_;
   double tmp282_ = (tmp281_)*(tmp58_);
   double tmp283_ = (tmp113_)*(tmp160_);
   double tmp284_ = tmp282_-tmp283_;
   double tmp285_ = (tmp284_)/tmp128_;
   double tmp286_ = (tmp81_)*(tmp196_);
   double tmp287_ = -(tmp286_);
   double tmp288_ = tmp287_/tmp128_;
   double tmp289_ = (tmp172_)*mLocGL_0_0;
   double tmp290_ = (tmp175_)*mLocGL_1_0;
   double tmp291_ = tmp289_+tmp290_;
   double tmp292_ = tmp176_*mLocGL_2_0;
   double tmp293_ = tmp291_+tmp292_;
   double tmp294_ = (tmp293_)*(tmp15_);
   double tmp295_ = (tmp172_)*mLocGL_0_1;
   double tmp296_ = (tmp175_)*mLocGL_1_1;
   double tmp297_ = tmp295_+tmp296_;
   double tmp298_ = tmp176_*mLocGL_2_1;
   double tmp299_ = tmp297_+tmp298_;
   double tmp300_ = (tmp299_)*(tmp28_);
   double tmp301_ = tmp294_+tmp300_;
   double tmp302_ = (tmp172_)*mLocGL_0_2;
   double tmp303_ = (tmp175_)*mLocGL_1_2;
   double tmp304_ = tmp302_+tmp303_;
   double tmp305_ = tmp176_*mLocGL_2_2;
   double tmp306_ = tmp304_+tmp305_;
   double tmp307_ = (tmp306_)*(tmp31_);
   double tmp308_ = tmp301_+tmp307_;
   double tmp309_ = (tmp308_)*(tmp58_);
   double tmp310_ = (tmp113_)*(tmp196_);
   double tmp311_ = tmp309_-tmp310_;
   double tmp312_ = (tmp311_)/tmp128_;
   double tmp313_ = tmp59_*(tmp66_);
   double tmp314_ = tmp313_*(tmp58_);
   double tmp315_ = (tmp81_)*tmp197_;
   double tmp316_ = tmp314_-tmp315_;
   double tmp317_ = (tmp316_)/tmp128_;
   double tmp318_ = tmp59_*(tmp98_);
   double tmp319_ = tmp318_*(tmp58_);
   double tmp320_ = (tmp113_)*tmp197_;
   double tmp321_ = tmp319_-tmp320_;
   double tmp322_ = (tmp321_)/tmp128_;
   double tmp323_ = tmp59_*(tmp72_);
   double tmp324_ = tmp323_*(tmp58_);
   double tmp325_ = (tmp81_)*tmp198_;
   double tmp326_ = tmp324_-tmp325_;
   double tmp327_ = (tmp326_)/tmp128_;
   double tmp328_ = tmp59_*(tmp104_);
   double tmp329_ = tmp328_*(tmp58_);
   double tmp330_ = (tmp113_)*tmp198_;
   double tmp331_ = tmp329_-tmp330_;
   double tmp332_ = (tmp331_)/tmp128_;
   double tmp333_ = tmp59_*(tmp79_);
   double tmp334_ = tmp333_*(tmp58_);
   double tmp335_ = (tmp81_)*tmp199_;
   double tmp336_ = tmp334_-tmp335_;
   double tmp337_ = (tmp336_)/tmp128_;
   double tmp338_ = tmp59_*(tmp111_);
   double tmp339_ = tmp338_*(tmp58_);
   double tmp340_ = (tmp113_)*tmp199_;
   double tmp341_ = tmp339_-tmp340_;
   double tmp342_ = (tmp341_)/tmp128_;
   double tmp343_ = (tmp66_)*(tmp58_);
   double tmp344_ = (tmp81_)*(tmp43_);
   double tmp345_ = tmp343_-tmp344_;
   double tmp346_ = (tmp345_)/tmp128_;
   double tmp347_ = (tmp98_)*(tmp58_);
   double tmp348_ = (tmp113_)*(tmp43_);
   double tmp349_ = tmp347_-tmp348_;
   double tmp350_ = (tmp349_)/tmp128_;
   double tmp351_ = (tmp72_)*(tmp58_);
   double tmp352_ = (tmp81_)*(tmp49_);
   double tmp353_ = tmp351_-tmp352_;
   double tmp354_ = (tmp353_)/tmp128_;
   double tmp355_ = (tmp104_)*(tmp58_);
   double tmp356_ = (tmp113_)*(tmp49_);
   double tmp357_ = tmp355_-tmp356_;
   double tmp358_ = (tmp357_)/tmp128_;
   double tmp359_ = (tmp79_)*(tmp58_);
   double tmp360_ = (tmp81_)*(tmp56_);
   double tmp361_ = tmp359_-tmp360_;
   double tmp362_ = (tmp361_)/tmp128_;
   double tmp363_ = (tmp111_)*(tmp58_);
   double tmp364_ = (tmp113_)*(tmp56_);
   double tmp365_ = tmp363_-tmp364_;
   double tmp366_ = (tmp365_)/tmp128_;

  mVal[0] = (mLocNDP0_x+mLocNDdx_x*(tmp200_)+mLocNDdy_x*(tmp201_))-mLocXIm;

  mCompDer[0][0] = 0;
  mCompDer[0][1] = 0;
  mCompDer[0][2] = 0;
  mCompDer[0][3] = 0;
  mCompDer[0][4] = 0;
  mCompDer[0][5] = 0;
  mCompDer[0][6] = 0;
  mCompDer[0][7] = 0;
  mCompDer[0][8] = 0;
  mCompDer[0][9] = 0;
  mCompDer[0][10] = 0;
  mCompDer[0][11] = 0;
  mCompDer[0][12] = 0;
  mCompDer[0][13] = 0;
  mCompDer[0][14] = 0;
  mCompDer[0][15] = 0;
  mCompDer[0][16] = 0;
  mCompDer[0][17] = 0;
  mCompDer[0][18] = 0;
  mCompDer[0][19] = 0;
  mCompDer[0][20] = 0;
  mCompDer[0][21] = 0;
  mCompDer[0][22] = 0;
  mCompDer[0][23] = 0;
  mCompDer[0][24] = 0;
  mCompDer[0][25] = 0;
  mCompDer[0][26] = 0;
  mCompDer[0][27] = 0;
  mCompDer[0][28] = 0;
  mCompDer[0][29] = 0;
  mCompDer[0][30] = 0;
  mCompDer[0][31] = 0;
  mCompDer[0][32] = 0;
  mCompDer[0][33] = 0;
  mCompDer[0][34] = 0;
  mCompDer[0][35] = 0;
  mCompDer[0][36] = 0;
  mCompDer[0][37] = 0;
  mCompDer[0][38] = 0;
  mCompDer[0][39] = 0;
  mCompDer[0][40] = 0;
  mCompDer[0][41] = 0;
  mCompDer[0][42] = 0;
  mCompDer[0][43] = 0;
  mCompDer[0][44] = 0;
  mCompDer[0][45] = 0;
  mCompDer[0][46] = 0;
  mCompDer[0][47] = 0;
  mCompDer[0][48] = 0;
  mCompDer[0][49] = 0;
  mCompDer[0][50] = 0;
  mCompDer[0][51] = 0;
  mCompDer[0][52] = 0;
  mCompDer[0][53] = (tmp219_)*mLocNDdx_x+(tmp237_)*mLocNDdy_x;
  mCompDer[0][54] = (tmp261_)*mLocNDdx_x+(tmp285_)*mLocNDdy_x;
  mCompDer[0][55] = (tmp288_)*mLocNDdx_x+(tmp312_)*mLocNDdy_x;
  mCompDer[0][56] = (tmp317_)*mLocNDdx_x+(tmp322_)*mLocNDdy_x;
  mCompDer[0][57] = (tmp327_)*mLocNDdx_x+(tmp332_)*mLocNDdy_x;
  mCompDer[0][58] = (tmp337_)*mLocNDdx_x+(tmp342_)*mLocNDdy_x;
  mCompDer[0][59] = (tmp346_)*mLocNDdx_x+(tmp350_)*mLocNDdy_x;
  mCompDer[0][60] = (tmp354_)*mLocNDdx_x+(tmp358_)*mLocNDdy_x;
  mCompDer[0][61] = (tmp362_)*mLocNDdx_x+(tmp366_)*mLocNDdy_x;
  mVal[1] = (mLocNDP0_y+mLocNDdx_y*(tmp200_)+mLocNDdy_y*(tmp201_))-mLocYIm;

  mCompDer[1][0] = 0;
  mCompDer[1][1] = 0;
  mCompDer[1][2] = 0;
  mCompDer[1][3] = 0;
  mCompDer[1][4] = 0;
  mCompDer[1][5] = 0;
  mCompDer[1][6] = 0;
  mCompDer[1][7] = 0;
  mCompDer[1][8] = 0;
  mCompDer[1][9] = 0;
  mCompDer[1][10] = 0;
  mCompDer[1][11] = 0;
  mCompDer[1][12] = 0;
  mCompDer[1][13] = 0;
  mCompDer[1][14] = 0;
  mCompDer[1][15] = 0;
  mCompDer[1][16] = 0;
  mCompDer[1][17] = 0;
  mCompDer[1][18] = 0;
  mCompDer[1][19] = 0;
  mCompDer[1][20] = 0;
  mCompDer[1][21] = 0;
  mCompDer[1][22] = 0;
  mCompDer[1][23] = 0;
  mCompDer[1][24] = 0;
  mCompDer[1][25] = 0;
  mCompDer[1][26] = 0;
  mCompDer[1][27] = 0;
  mCompDer[1][28] = 0;
  mCompDer[1][29] = 0;
  mCompDer[1][30] = 0;
  mCompDer[1][31] = 0;
  mCompDer[1][32] = 0;
  mCompDer[1][33] = 0;
  mCompDer[1][34] = 0;
  mCompDer[1][35] = 0;
  mCompDer[1][36] = 0;
  mCompDer[1][37] = 0;
  mCompDer[1][38] = 0;
  mCompDer[1][39] = 0;
  mCompDer[1][40] = 0;
  mCompDer[1][41] = 0;
  mCompDer[1][42] = 0;
  mCompDer[1][43] = 0;
  mCompDer[1][44] = 0;
  mCompDer[1][45] = 0;
  mCompDer[1][46] = 0;
  mCompDer[1][47] = 0;
  mCompDer[1][48] = 0;
  mCompDer[1][49] = 0;
  mCompDer[1][50] = 0;
  mCompDer[1][51] = 0;
  mCompDer[1][52] = 0;
  mCompDer[1][53] = (tmp219_)*mLocNDdx_y+(tmp237_)*mLocNDdy_y;
  mCompDer[1][54] = (tmp261_)*mLocNDdx_y+(tmp285_)*mLocNDdy_y;
  mCompDer[1][55] = (tmp288_)*mLocNDdx_y+(tmp312_)*mLocNDdy_y;
  mCompDer[1][56] = (tmp317_)*mLocNDdx_y+(tmp322_)*mLocNDdy_y;
  mCompDer[1][57] = (tmp327_)*mLocNDdx_y+(tmp332_)*mLocNDdy_y;
  mCompDer[1][58] = (tmp337_)*mLocNDdx_y+(tmp342_)*mLocNDdy_y;
  mCompDer[1][59] = (tmp346_)*mLocNDdx_y+(tmp350_)*mLocNDdy_y;
  mCompDer[1][60] = (tmp354_)*mLocNDdx_y+(tmp358_)*mLocNDdy_y;
  mCompDer[1][61] = (tmp362_)*mLocNDdx_y+(tmp366_)*mLocNDdy_y;
}


void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::ComputeValDerivHessian()
{
  ELISE_ASSERT(false,"Foncteur cEqAppui_NoDist__GL__PTInc_M2CPolyn6 Has no Der Sec");
}

void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_0_0(double aVal){ mLocGL_0_0 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_0_1(double aVal){ mLocGL_0_1 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_0_2(double aVal){ mLocGL_0_2 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_1_0(double aVal){ mLocGL_1_0 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_1_1(double aVal){ mLocGL_1_1 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_1_2(double aVal){ mLocGL_1_2 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_2_0(double aVal){ mLocGL_2_0 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_2_1(double aVal){ mLocGL_2_1 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetGL_2_2(double aVal){ mLocGL_2_2 = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetNDP0_x(double aVal){ mLocNDP0_x = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetNDP0_y(double aVal){ mLocNDP0_y = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetNDdx_x(double aVal){ mLocNDdx_x = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetNDdx_y(double aVal){ mLocNDdx_y = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetNDdy_x(double aVal){ mLocNDdy_x = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetNDdy_y(double aVal){ mLocNDdy_y = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetXIm(double aVal){ mLocXIm = aVal;}
void cEqAppui_NoDist__GL__PTInc_M2CPolyn6::SetYIm(double aVal){ mLocYIm = aVal;}



double * cEqAppui_NoDist__GL__PTInc_M2CPolyn6::AdrVarLocFromString(const std::string & aName)
{
   if (aName == "GL_0_0") return & mLocGL_0_0;
   if (aName == "GL_0_1") return & mLocGL_0_1;
   if (aName == "GL_0_2") return & mLocGL_0_2;
   if (aName == "GL_1_0") return & mLocGL_1_0;
   if (aName == "GL_1_1") return & mLocGL_1_1;
   if (aName == "GL_1_2") return & mLocGL_1_2;
   if (aName == "GL_2_0") return & mLocGL_2_0;
   if (aName == "GL_2_1") return & mLocGL_2_1;
   if (aName == "GL_2_2") return & mLocGL_2_2;
   if (aName == "NDP0_x") return & mLocNDP0_x;
   if (aName == "NDP0_y") return & mLocNDP0_y;
   if (aName == "NDdx_x") return & mLocNDdx_x;
   if (aName == "NDdx_y") return & mLocNDdx_y;
   if (aName == "NDdy_x") return & mLocNDdy_x;
   if (aName == "NDdy_y") return & mLocNDdy_y;
   if (aName == "XIm") return & mLocXIm;
   if (aName == "YIm") return & mLocYIm;
   return 0;
}


cElCompiledFonc::cAutoAddEntry cEqAppui_NoDist__GL__PTInc_M2CPolyn6::mTheAuto("cEqAppui_NoDist__GL__PTInc_M2CPolyn6",cEqAppui_NoDist__GL__PTInc_M2CPolyn6::Alloc);


cElCompiledFonc *  cEqAppui_NoDist__GL__PTInc_M2CPolyn6::Alloc()
{  return new cEqAppui_NoDist__GL__PTInc_M2CPolyn6();
}


