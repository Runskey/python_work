/* 
 *INTEL CONFIDENTIAL
 *Copyright 2009-2014 Intel Corporation All Rights Reserved.
 *
 *The source code contained or described herein and all documents related to the
 *source code ("Material") are owned by Intel Corporation or its suppliers or
 *licensors. Title to the Material remains with Intel Corporation or its
 *suppliers and licensors. The Material may contain trade secrets and proprietary
 *and confidential information of Intel Corporation and its suppliers and
 *licensors, and is protected by worldwide copyright and trade secret laws and
 *treaty provisions. No part of the Material may be used, copied, reproduced,
 *modified, published, uploaded, posted, transmitted, distributed, or disclosed
 *in any way without Intels prior express written permission.
 *No license under any patent, copyright, trade secret or other intellectual
 *property right is granted to or conferred upon you by disclosure or delivery
 *of the Materials, either expressly, by implication, inducement, estoppel or
 *otherwise. Any license under such intellectual property rights must be
 *express and approved by Intel in writing.
 *
 *Include any supplier copyright notices as supplier requires Intel to use.
 *Include supplier trademarks or logos as supplier requires Intel to use,
 *preceded by an asterisk.
 *An asterisked footnote can be added as follows: 
 *  *Third Party trademarks are the property of their respective owners.

 * Unless otherwise agreed by Intel in writing, you may not remove or alter this
 * notice or any other notice embedded in Materials by Intel or Intels suppliers
 * or licensors in any way.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#include <iostream>               // for std::cout

// PCL/PPL API definition
#include "test_suite_common.h"
#include "msg_process.h"
#include "kheap.h"
#include "mem_rw.h"	
#include "base\base_header.h"
//#include "test_entry.hxx"

extern "C"
{
#include "osal.h"
#include "..\common\dlphydef.h"
}

using namespace boost::filesystem;          // for ease of tutorial presentation;
using namespace std;
using namespace IPG;

// IPG parameter list as input
#include "base/paramlist.h"

extern "C"
{
#define         _FALSE            0
#define         _TRUE             1
#define         P_DTXBit          2
#define         P_FillBit         2
#define         _DTX              1
#define         _NO_DTX           0

#define         GARBAGE_POINTER   0x87654321
#define         MMAX(a,b)         ((a)>(b) ? (a) : (b))
#define         ABS(a)            (((a)<0) ? -(a) : (a))
#define         MMIN(a,b)         (((a)<(b)) ? (a) : (b))
}

dlR99PxlMsg_t               R99PxlMsg;
int                         MapOutput[38400 * 4 * 2];
int                         BpTestVec[38400 * 4];
//int  BpOutput[38400 * 4];
//int  FecOutput[40000];
//int  FecTestVec[10000];
int                         ErrCountTotal = 0;
U32                         copySize;
int                         MapDdrAddr;
std::vector<IPG::ParamList> par_array;

#ifdef FEC_BP_TEST
//void udelay(int us)
//{
//static int n=0;
//for( int i=0; i<1000*us; i++){
//n++;
//}
//}
#endif

// dump output from bit-wide vector to U32 array
bool dumpMemAndCompare(std::string& testCaseParamFileName,  int index)
{
  FILE *pfBp;

  //#ifdef FEC_BP_TEST
  //FILE *pfFec;
  //#endif

  int i, errCount;
  std::string  bpOutput, fecOutput;
  stringstream fileName, fileName1, fileName2;

  //fileName << "BPTestVector_tfci_" << index;
  //bpOutput = testCaseParamFileName + "\\" + fileName.str() + ".txt";

  //if ((pfBp = fopen(bpOutput.c_str(), "r")) == NULL)
  //{
    //printf("##### ERROR ##### can't open %s\n", bpOutput.c_str());
    //return false;
  //}
  //for (i=0; i<38400*4; i++)
  //{
    //fscanf(pfBp, "%08x\n", &(BpTestVec[i]));
  //}
  //fclose(pfBp);

#ifdef FEC_BP_TEST
  //fileName1<< "FECTestVector_tfci_"<< index;
  //fecOutput = testCaseParamFileName + "\\" +fileName1.str() + ".txt";
  //if((pfFec = fopen(fecOutput.c_str(),"r"))==NULL)
  //{
  //printf("##### ERROR ##### can't open %s\n",fecOutput.c_str());
  //}
  //for(i=0; i<10000[>copySize<];i++)
  //{
  //fscanf(pfFec, "%08x\n",&(FecTestVec[i])); 
  //}
#endif

  errCount = 0;

#ifdef FEC_BP_TEST
  //udelay(5000);
  //memdump(FEC_BASE_ADDR, 10000, FecOutput);
  //memdump(BP_BASE_ADDR, 38400 * 4, BpOutput);
  //#if 0
  //for(U32 i=0; i<copySize ; i++) 
  //{
  //if(FecTestVec[i] != FecOutput[i])
  //{
  //printf("[ FEC ]ERROR: TFCI: [%d]:::::Mismatch at %dth word %x and %x \n",index, i, FecTestVec[i], FecOutput[i]);
  //errCount++;
  //break;
  //}
  //}
  //#endif
  //for(U32 i=0; i<38400 * 4; i++) 
  //{
  //if(BpTestVec[i] != BpOutput[i])
  //{
  //printf("[ BP ]ERROR: TFCI: [%d]:::::Mismatch at %dth word %x and %x \n", index,i, BpTestVec[i], BpOutput[i]);
  //errCount++;
  //break;
  //}
  //}
  //#if 0    
  //if(par.fetch<int>("diversityMode") == 0)//active
  //{
  //fileName2<< "BPTestVector_tfci_ant1_"<< index;
  //bpOutput = testCaseParamFileName + "\\" + fileName2.str() + ".txt";
  //if((pfBp = fopen(bpOutput.c_str(),"r"))==NULL)
  //{
  //printf("##### ERROR #####\n");
  //}
  //for(i=0; i<38400 *4;i++)
  //{
  //fscanf(pfBp, "%08x\n",&(BpTestVec[i])); 
  //}

  //memdump(BP_ANT_1_BASE_ADDR, 38400 * 4, BpOutput);
  //for(U32 i=0; i<38400 * 4; i++) 
  //{
  //if(BpTestVec[i] != BpOutput[i])
  //{
  //printf("[ BP ANT1]ERROR: TFCI: [%d]:::::Mismatch at %dth word %x and %x \n", index,i, BpTestVec[i], BpOutput[i]);
  //errCount++;
  //break;
  //}
  //}
  //}
  //#endif
  //ErrCountTotal += errCount;
  ////printf("BP ERROR: TFCI: [%d]:::::Mismatch at %d\n", index,errCount);
  //if (!errCount) 
  //{
  //printf("\n\n---- TFCI: [%03d]:::::-all BP and FEC output passed!!!!!-----------\n\n ",index);
  //return true;
  //}
#else
  printf("Now dump data from MAP memory ......\n", MapOutput[i]);
  memdump(MapDdrAddr, 38400*4*2, MapOutput);

  {
    // Store dumped data for debug
    std::string dumpOut = testCaseParamFileName + "\\dumpData.txt";
    FILE *pDump = fopen(dumpOut.c_str(), "w");
    for (U32 i=0; i<38400*4*2; i++)
    {
      fprintf(pDump, "%08x\n", MapOutput[i]);
    }
    fclose(pDump);
  }

  U32 j = 0;
  for (U32 i=0; i<38400*4; i++)
  {
    if (BpTestVec[i] != MapOutput[j])
    {
      // read again
      printf("re-reading MAP address 0x%08x\n", MapDdrAddr+j);
      memdump(MapDdrAddr, 38400*4*2, MapOutput);
      if (BpTestVec[i] != MapOutput[j])
      {
        printf("[ MAP ]ERROR: TFCI: [%d]:::::Mismatch at %dth word %x and %x \n", index, i, BpTestVec[i], MapOutput[j]);
        errCount++;
        break;
      }
    }
    j += 2;
  }

  ErrCountTotal += errCount;
  if (!errCount) 
  {
    printf("\n\n---- TFCI: [%03d]::::: MAP output passed!!!!!-----------\n\n", index);
    return true;
  }
  else
  {
    return false;
  }
#endif

}

#ifdef FEC_BP_TEST

void dumpMemAndSave(std::string& testCaseParamFileName, int tfci)
{
  FILE *pfBp, *pfFec;
  int i;
  std::string  bpOutput, fecOutput;
  stringstream fileName,fileName1,fileName2;

  fileName<< "BPTestVector_tfci_"<< tfci;
  bpOutput = testCaseParamFileName + "\\" + fileName.str() + ".txt";

  fileName1<< "FECTestVector_tfci_"<< tfci;
  fecOutput = testCaseParamFileName + "\\" +  fileName1.str() + ".txt";
  if((pfBp = fopen(bpOutput.c_str(),"w"))==NULL)
  {
    printf("##### ERROR #####\n");
  }
  if((pfFec = fopen(fecOutput.c_str(),"w"))==NULL)
  {
    printf("##### ERROR #####\n");
  }
  //memdump(BP_CNT_ADDR, 1, &BpCnt);
  //while(BpCnt)
  udelay(2000000);
  memdump(FEC_BASE_ADDR, 10000, FecOutput);
  memdump(BP_BASE_ADDR, 38400 * 4, BpOutput);

  //memdump(MapDdrAddr, 38400 * 4, BpOutput);

  for(i=0; i<10000;i++)
  {
    fprintf(pfFec, "%08x\n",FecOutput[i]); 
  }
  fclose(pfFec);
  for(i=0; i<38400 * 4;i++)
    fprintf(pfBp, "%08x\n",BpOutput[i]); 
  fclose(pfBp);
  if(par.fetch<int>("diversityMode") == 0)//active
  {
    fileName2<< "BPTestVector_evm_tfci_ant1_"<< tfci;
    bpOutput = testCaseParamFileName + "\\" + fileName2.str() + ".txt";
    if((pfBp = fopen(bpOutput.c_str(),"w"))==NULL)
    {
      printf("##### ERROR #####\n");
    }

    memdump(BP_ANT_1_BASE_ADDR, 38400 * 4, BpOutput);
    for(i=0; i<38400 * 4;i++)
      fprintf(pfBp, "%08x\n",BpOutput[i]); 
    fclose(pfBp);
  }
}

#endif //FEC_BP_TEST


//void clearMemory(U32 addr)
//{
//int freeRes;

//if (DUMMY_ADDR == addr)
//{
//return;
//}

////	cout << "freeing address " << std::hex << addr << endl;
//freeRes = kheap_free(kMEMID_DDR, (void*)&addr);
//if (freeRes == FAILURE) 
//{
//teePrintf("error in freeing memory\n");
//}
//}

U32 ComputeR99RateMatchingFlexibleDTX(DlCctrch_t *r99User, U8 tfciConfig[MAX_DL_TFCI_NUM][MAX_DL_TRCH], U8 maxTfci, U8 thisTfci)
{
  S16 i, j;
  S32 Xi, Fi[MAX_DL_TRCH];
  S32 dNi_tti, dNi,Size;
  S32 iDenom, iZ, itemp;
  S32 idNi_max[MAX_DL_TRCH], idNi[MAX_DL_TRCH];

  S16 crcID, multiplier, tailbits, Ndataj, tableIndex;
  unsigned long long numerator[MAX_DL_TRCH];
  DlFecRMmode_e PuncRepMode;
  U8 tfc,trch;
  U32 sizeCctrch, maxCctrch, sizeTrch;
  U32 n[MAX_DL_TFCI_NUM][MAX_DL_TRCH];
  S64 accumulator;
  S32 deltaNtti[MAX_DL_TFCI_NUM][MAX_DL_TRCH];
  S32 D;

  memset(&deltaNtti[0][0], 0, sizeof(U32) * MAX_DL_TFC * MAX_DL_TRCH);

  // do rate matching parameters

  // calc max (RM(i) * N(i))
  maxCctrch = 0;
  for (tfc = 0; tfc < maxTfci; tfc++)
  {
    sizeCctrch = 0;
    for (trch = 0; trch < r99User->dlFecDriverCctrch.numTrCh; trch++)
    {
      switch (r99User->trCh[trch].dlFecDriverTrch.codeType)
      {
        case CONV_ONE_HALF:
          tailbits = 16;
          multiplier = 2;
          break;

        case CONV_ONE_THIRD:
          tailbits = 24;
          multiplier = 3;
          break;

        case TURBO_ONE_THIRD:
          multiplier = 3;
          tailbits = 12;
          break;
      }

      switch (r99User->trChType)
      {
        case (BCH_3G):
          Ndataj = 18 * 15;
          break;

        case (FACH_3G):
        case (PCH_3G):
          Ndataj = DL_SCCPCH_TABLE[r99User->slotFormat][2];      //Ndata1
          Ndataj = Ndataj * 15;
          break;

        case (DCH_3G):
        default:
          // the RM parameters for DL normal & compressed mode are the smae
          tableIndex = 3 * r99User->slotFormat;
          Ndataj = DL_DPCH_TABLE[tableIndex][0];    //Ndata0
          Ndataj += DL_DPCH_TABLE[tableIndex][1];    //Ndata1
          Ndataj = Ndataj * 15;
          break;
      }

      Fi[trch] = TTI_tbl[r99User->trCh[trch].dlFecDriverTrch.tti];
      crcID = CRC_tbl[r99User->trCh[trch].dlFecDriverTrch.crcSize];
      sizeTrch = tfciConfig[tfc][trch] * (r99User->trCh[trch].blkSizeArray[r99User->trCh[trch].TFC[tfc]]+ crcID);
      if (sizeTrch != 0)
      {
        // now compute size of Encoder output
        sizeTrch = multiplier * sizeTrch + tailbits;
      }
      // extra multiply by 8, gets removed later
      n[tfc][trch] = (sizeTrch * 8) / Fi[trch];

      sizeCctrch = sizeCctrch + (r99User->trCh[trch].rmAttribute * n[tfc][trch]);

    }
    if (sizeCctrch > maxCctrch)
    {
      maxCctrch = sizeCctrch;
    }
  }


  // calc deltaN,TTI
  for (tfc = 0; tfc < maxTfci; tfc++)
  {
    for (trch = 0; trch < r99User->dlFecDriverCctrch.numTrCh; trch++)
    {
      deltaNtti[tfc][trch] = 0;
      if (n[tfc][trch] !=0)
      {
        accumulator = (S64)(Ndataj) * (S64)r99User->trCh[trch].rmAttribute * (S64)n[tfc][trch] * Fi[trch];
        accumulator = (accumulator - 1) / (S64)(Fi[trch] * maxCctrch) + 1;
        accumulator = Fi[trch] * accumulator * 8;
        deltaNtti[tfc][trch] = accumulator - (n[tfc][trch] * Fi[trch]);
      }
    }
  }

  // now handle second phase
  for (tfc = 0; tfc < maxTfci; tfc++)
  {
    D = 0;
    for (trch = 0; trch < r99User->dlFecDriverCctrch.numTrCh; trch++)
    {
      D = D + (deltaNtti[tfc][trch] / Fi[trch]) + n[tfc][trch];
    }
    D >>= 3;
    if (D > Ndataj)
    {
      iDenom = 0;
      for (trch = 0; trch < r99User->dlFecDriverCctrch.numTrCh; trch++)
      {
        iDenom += r99User->trCh[trch].rmAttribute * n[tfc][trch];
      }

      for (i = 0; i < r99User->dlFecDriverCctrch.numTrCh; i++)
      {
        numerator[i] = 0;
        for (j = 0; j <= i; j++)
        {
          numerator[i] +=  r99User->trCh[j].rmAttribute * n[tfc][j];
        }
      }

      iZ = 0;
      for (i = 0 ; i < r99User->dlFecDriverCctrch.numTrCh; i++)
      {
        itemp = (S32) ( numerator[i] * Ndataj  / iDenom )<<3;
        idNi_max[i] =  Fi[i] * (itemp - iZ - n[tfc][i]);
        idNi[i] = idNi_max[i] / Fi[i];

        iZ =  itemp;
      }

      // update all the tfc cases for modified TrCh
      for (j = 0; j < maxTfci; j++)
      {
        for (trch = 0; trch < r99User->dlFecDriverCctrch.numTrCh; trch++)
        {
          if (tfciConfig[tfc][trch] == tfciConfig[j][trch])
          {
            if (r99User->trCh[trch].blkSizeArray[r99User->trCh[trch].TFC[tfc]] == r99User->trCh[trch].blkSizeArray[r99User->trCh[trch].TFC[j]])
            {
              // update all same-sized TFC with adjusted sizes
              deltaNtti[j][trch] = idNi[trch] * Fi[trch];
            }
          }
        }
      }
    }
  }

  //
  for (trch = 0; trch < r99User->dlFecDriverCctrch.numTrCh; trch++)
  {
    r99User->trCh[trch].rateMatchParms.DTX_Out_length = 0;
    idNi[trch] = deltaNtti[thisTfci][trch];
    if (tfciConfig[thisTfci][trch] != 0)
    {
      Size = Fi[trch] * n[thisTfci][trch] + idNi[trch];
      Size>>=3;
      r99User->trCh[trch].rateMatchParms.DTX_Out_length = Size;
    }
  }

  for (i = 0 ; i < r99User->dlFecDriverCctrch.numTrCh; i++)
  {
    dNi_tti = (idNi[i])>>3;
    if (dNi_tti == 0)
    {
      //neither puncturing or repetition

      r99User->trCh[i].rateMatchParms.eini[0] = 1;
      r99User->trCh[i].rateMatchParms.eplus[0] = 0;
      r99User->trCh[i].rateMatchParms.eminus[0] = 0;
      PuncRepMode = NOT_REP_OR_PUNCT;
    }
    else if ((r99User->trCh[i].dlFecDriverTrch.codeType != TURBO_ONE_THIRD) || 
        (r99User->trCh[i].dlFecDriverTrch.codeType == TURBO_ONE_THIRD && (idNi_max[i] > 0)))
    {  
      //a = 2;
      r99User->trCh[i].rateMatchParms.eini[0] = 1;
      r99User->trCh[i].rateMatchParms.eplus[0] =  (Fi[i] * n[thisTfci][i])>>2;
      r99User->trCh[i].rateMatchParms.eminus[0] = (abs(dNi_tti))<<1;

      if (idNi_max[i] > 0)
      {
        PuncRepMode = REPETITION;
      }
      else
      {
        PuncRepMode = PUNCTURING;
      }
    }
    else
    {
      // turbo with puncturing
      PuncRepMode = PUNCTURING;
      // systematic
      Xi = ((Fi[i]*n[thisTfci][i])>>3) / 3;
      r99User->trCh[i].rateMatchParms.eini[0] = 1;
      r99User->trCh[i].rateMatchParms.eplus[0] = 0;
      r99User->trCh[i].rateMatchParms.eminus[0] = 0;
      //parity 1, a = 2;
      dNi = dNi_tti>>1; //floor
      r99User->trCh[i].rateMatchParms.eini[1] = Xi;
      r99User->trCh[i].rateMatchParms.eplus[1] = Xi<<1;
      r99User->trCh[i].rateMatchParms.eminus[1] = (abs(dNi))<<1;
      //parity 2, a = 1
      dNi = ((dNi_tti -1)>>1)  + 1; //ceiling
      r99User->trCh[i].rateMatchParms.eini[2]   = Xi;
      r99User->trCh[i].rateMatchParms.eplus[2]  = Xi;
      r99User->trCh[i].rateMatchParms.eminus[2] = abs(dNi);
    }

    if (r99User->trCh[i].dlFecDriverTrch.codeType != TURBO_ONE_THIRD)
    {
      if (PuncRepMode == NOT_REP_OR_PUNCT)
      {
        r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_NOT_REP_OR_PUNCT;
      }
      else if (PuncRepMode == PUNCTURING)
      {
        r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_CONV_PUNCTURING;
      }
      else
      {
        r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_REPETITION;
      }
    }
    else //turbo
    {
      if (PuncRepMode == PUNCTURING)
      {
        r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_TURBO_PUNCTURING;
      }
      else
      {
        r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_REPETITION;
      }
    }
    r99User->trCh[i].rateMatchParms.PuncRepMode = PuncRepMode;
    r99User->trCh[i].dlFecDriverTrch.numTrBksM = tfciConfig[thisTfci][i];
    crcID = CRC_tbl[r99User->trCh[i].dlFecDriverTrch.crcSize];
    //sizeTrch = tfciConfig[thisTfci][i] * (r99User->trCh[i].dlFecDriverTrch.trBksizeA + crcID);
    sizeTrch = tfciConfig[thisTfci][i] * (r99User->trCh[i].blkSizeArray[r99User->trCh[i].TFC[thisTfci]]+ crcID);       
    if (sizeTrch == 0)
    {
      r99User->trCh[i].rateMatchParms.C = 0;
      r99User->trCh[i].rateMatchParms.K = 0;
      r99User->trCh[i].rateMatchParms.Y = 0;
    }
    else
    {
      if (r99User->trCh[i].dlFecDriverTrch.codeType == TURBO_ONE_THIRD)
      {
        // ceiling(X/5114)
        r99User->trCh[i].rateMatchParms.C = (S16)((sizeTrch-1)/5114 + 1);
      }
      else
      {
        //ceiling(X/504)
        r99User->trCh[i].rateMatchParms.C = (S16)((sizeTrch-1)/504 + 1);
      }
      // K = ceiling(X/C)
      r99User->trCh[i].rateMatchParms.K = (sizeTrch - 1) / r99User->trCh[i].rateMatchParms.C + 1;
      // Y = C * K - X
      r99User->trCh[i].rateMatchParms.Y = r99User->trCh[i].rateMatchParms.C * r99User->trCh[i].rateMatchParms.K - sizeTrch;
    }
  } // end of for TrCh

  r99User->dlFecDriverCctrch.secondInterleaverSize = Ndataj;
  return (DLFEC_OK);
}

U32 ComputeR99RateMatchParms(DlCctrch_t *r99User)
{
  S16 TrChIdx, i, j;
  S32 N_TTI, N_TTI_max, Xi, Fi[MAX_DL_TRCH];
  S32 dNi_tti, dNi, K, C, Y, Size;
  S32 BitsPerRadioFrame;
  S32 Numbits, iDenom, iZ, itemp;
  S32 Nil_TTI[MAX_DL_TRCH], iNi[MAX_DL_TRCH], Nil_TTI_max[MAX_DL_TRCH], idNi_max[MAX_DL_TRCH], idNi[MAX_DL_TRCH];
  S16 crcID, multiplier, tailbits, Ndataj, tableIndex;
  unsigned long long numerator[MAX_DL_TRCH];
  DlFecRMmode_e PuncRepMode;
  BOOLEAN	TTI_boundary;

  // do rate matching parameters
  // if on TTI boundary
  TTI_boundary = _FALSE;
  for (TrChIdx = 0; TrChIdx < r99User->dlFecDriverCctrch.numTrCh; TrChIdx++)
  {
    if (r99User->radioFrameIdx % TTI_tbl[r99User->trCh[TrChIdx].dlFecDriverTrch.tti] == 0)
      TTI_boundary = _TRUE;
  }

  if (TTI_boundary)
  {
    for (i = 0 ; i < r99User->dlFecDriverCctrch.numTrCh; i++)
    {
      if (r99User->trCh[i].dlFecDriverTrch.codeType == CONV_ONE_HALF)
      {
        // one half conv
        tailbits = 16;
        multiplier = 2;
      }
      else if (r99User->trCh[i].dlFecDriverTrch.codeType == CONV_ONE_THIRD)
      {
        // one third conv
        tailbits = 24;
        multiplier = 3;
      }
      else
      {
        //_ONE_THIRD TURBO
        multiplier = 3;
        tailbits = 12;
      }

      switch (r99User->trChType)
      {
        case (BCH_3G):
          Ndataj = 18 * 15;
          break;

        case (FACH_3G):
        case (PCH_3G):
          Ndataj = DL_SCCPCH_TABLE[r99User->slotFormat][2];      //Ndata1
          Ndataj = Ndataj * 15;
          break;

        case (DCH_3G):
        default:
          tableIndex = 3 * r99User->slotFormat;

          Ndataj	 = DL_DPCH_TABLE[tableIndex][0];    //Ndata0
          Ndataj	+= DL_DPCH_TABLE[tableIndex][1];    //Ndata1
          Ndataj	 = Ndataj * 15;
          break;
      }

      r99User->dlFecDriverCctrch.secondInterleaverSize = Ndataj;
      Fi[i] = TTI_tbl[r99User->trCh[i].dlFecDriverTrch.tti];
      crcID = CRC_tbl[r99User->trCh[i].dlFecDriverTrch.crcSize] ;
      N_TTI = (r99User->trCh[i].dlFecDriverTrch.trBksizeA + crcID) * r99User->trCh[i].dlFecDriverTrch.numTrBksM;
      Numbits = N_TTI;

      if ( (Numbits < 40) && (Numbits != 0) && (r99User->trCh[i].dlFecDriverTrch.codeType == TURBO_ONE_THIRD) )
      {
        Numbits = 40;
      }
      if ( (Numbits < 8) && (r99User->trCh[i].dlFecDriverTrch.codeType != TURBO_ONE_THIRD) )
      {
        Numbits = 8;
      }

      if (Numbits == 0)
      {
        C = 0;
      }
      else
      {
        if (r99User->trCh[i].dlFecDriverTrch.codeType == TURBO_ONE_THIRD)
        {
          C = (S16) ( (Numbits-1)/5114 + 1 );               // ceiling
        }
        else
        {
          C = (S16) ( (Numbits-1)/504 + 1 );                // ceiling
        }
      }
      if (C != 0)
      {
        K = (Numbits - 1) / C + 1;                              // ceiling
        Y = C * K - N_TTI;
      }
      else
      {
        // TC_DLL requires K [8..504] so gotta BYPASS_ENC
        K = 1;
        Y = 0;
      }
      Numbits = (K * multiplier + tailbits) * C;
      Nil_TTI[i] = Numbits;

      r99User->trCh[i].rateMatchParms.K = K;
      r99User->trCh[i].rateMatchParms.C = C;
      r99User->trCh[i].rateMatchParms.Y = Y;

      N_TTI_max = 0;
      if (r99User->trCh[i].maxTrBkWithinTFS != 0)
      {
        N_TTI_max = (r99User->trCh[i].maxTrBkWithinTFS + crcID) * r99User->trCh[i].dlFecDriverTrch.numTrBksM;
        if ( (N_TTI_max < 40) && (r99User->trCh[i].dlFecDriverTrch.codeType == TURBO_ONE_THIRD) )
        {
          N_TTI_max = 40;
        }
        else if ((N_TTI_max < 8) && (r99User->trCh[i].dlFecDriverTrch.codeType != TURBO_ONE_THIRD) )
        {
          // TC_DLL requires K [8..504]
          N_TTI_max = 8;
        }

        if (r99User->trCh[i].dlFecDriverTrch.codeType == TURBO_ONE_THIRD)
        {
          C = (S16) ( (N_TTI_max-1)/5114 + 1 );             // ceiling
        }
        else
        {
          C = (S16) ( (N_TTI_max-1)/504 + 1 );              // ceiling
        }
        K = (N_TTI_max - 1) / C + 1;                            // ceiling
        //Y = C * K - N_TTI_max;
        N_TTI_max = ( K * multiplier + tailbits ) * C;
      }


      iNi[i] = (N_TTI_max<<3) / Fi[i];
      Nil_TTI_max[i] = N_TTI_max;   //?????

    }  //end of for TrCh loop

    iDenom = 0;
    for (i = 0 ; i < r99User->dlFecDriverCctrch.numTrCh; i++)
    {
      iDenom += r99User->trCh[i].rmAttribute * iNi[i];
    }

    for (i = 0; i < r99User->dlFecDriverCctrch.numTrCh; i++)
    {
      numerator[i] = 0;
      for (j = 0; j <= i; j++)
      {
        numerator[i] +=  r99User->trCh[j].rmAttribute * iNi[j];
      }
    }

    iZ = 0;
    for (i = 0 ; i < r99User->dlFecDriverCctrch.numTrCh; i++)
    {
      itemp = (S32) ( numerator[i] * Ndataj  / iDenom )<<3;
      idNi_max[i] =  Fi[i] * (itemp - iZ - iNi[i]);
      idNi[i] = idNi_max[i] / Fi[i];
      iZ =  itemp;
    }

    for (i = 0 ; i < r99User->dlFecDriverCctrch.numTrCh; i++)
    {
      dNi_tti =   (Fi[i] *idNi[i])>>3;
      if (dNi_tti == 0)
      {
        //neither puncturing or repetition

        r99User->trCh[i].rateMatchParms.eini[0] = 0;
        r99User->trCh[i].rateMatchParms.eplus[0] = 0;
        r99User->trCh[i].rateMatchParms.eminus[0] = 0;
        PuncRepMode = NOT_REP_OR_PUNCT;
        Xi = Nil_TTI[i];
      }
      else if ((r99User->trCh[i].dlFecDriverTrch.codeType != TURBO_ONE_THIRD) || 
          (r99User->trCh[i].dlFecDriverTrch.codeType == TURBO_ONE_THIRD && (idNi_max[i] > 0)))
      {  
        //a = 2;
        r99User->trCh[i].rateMatchParms.eini[0] = 1;
        r99User->trCh[i].rateMatchParms.eplus[0] = Nil_TTI_max[i]<<1;
        r99User->trCh[i].rateMatchParms.eminus[0] = (abs(idNi_max[i]>>3))<<1;

        if (idNi_max[i] > 0) 
          PuncRepMode = REPETITION;
        else
          PuncRepMode = PUNCTURING;
      }
      else
      {
        // turbo with puncturing
        PuncRepMode = PUNCTURING;
        Xi = Nil_TTI_max[i] / 3 ; 
        //a = 2;
        dNi = dNi_tti>>1;
        r99User->trCh[i].rateMatchParms.eini[1] = Xi;
        r99User->trCh[i].rateMatchParms.eplus[1] = Xi<<1;
        r99User->trCh[i].rateMatchParms.eminus[1] = (abs(dNi))<<1;

        //a = 1;
        dNi =  ((dNi_tti-1)>>1) + 1;
        r99User->trCh[i].rateMatchParms.eini[2]   = Xi;
        r99User->trCh[i].rateMatchParms.eplus[2]  = Xi;
        r99User->trCh[i].rateMatchParms.eminus[2] = abs(dNi);
      }

      if (r99User->trCh[i].dlFecDriverTrch.codeType != TURBO_ONE_THIRD)
      {
        if (PuncRepMode == NOT_REP_OR_PUNCT)
        {
          r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_NOT_REP_OR_PUNCT;
        }
        else if (PuncRepMode == PUNCTURING)
        {
          r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_CONV_PUNCTURING;
        }
        else
        {
          r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_REPETITION;
        }
      }
      else //turbo
      {
        if (PuncRepMode == PUNCTURING)
        {
          r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_TURBO_PUNCTURING;
        }
        else
        {
          r99User->trCh[i].dlFecDriverTrch.rateMatchType = R99_REPETITION;
        }
      }

      r99User->trCh[i].rateMatchParms.PuncRepMode = PuncRepMode;
      Size = Fi[i] * (iNi[i] + idNi[i] );
      Size>>=3;
      r99User->trCh[i].rateMatchParms.DTX_Out_length = Size;

    }	// end of for TrCh

  }   // endif TTI_boundary

  Size = 0;
  for (i = 0; i < r99User->dlFecDriverCctrch.numTrCh ; i++)
  {
    BitsPerRadioFrame = r99User->trCh[i].rateMatchParms.DTX_Out_length / TTI_tbl[r99User->trCh[i].dlFecDriverTrch.tti];
    Size += BitsPerRadioFrame ;
  }  // end of for TrCh loop

  i =  r99User->dlFecDriverCctrch.numTrCh; //+   DCH_User->RFIndex ;
  r99User->dlFecDriverCctrch.secondInterleaverSize = Size;

  return (DLFEC_OK);
}

void InitDlPcpichSetup(PxlMsg_t *pMsg, U16 resouceId, U8 cellId)
{
  PxlDlConfigCpich_t *pConfigCpich;
  //header
  pMsg->header.msgType    = PXL_DLCONFIG_CPICH;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigCpich_t);
  //body
  pConfigCpich = (PxlDlConfigCpich_t *)&(pMsg->msgUn);
  pConfigCpich->cellId              = cellId;
  pConfigCpich->chanCodeIndex       = 0;
  pConfigCpich->chanPower           = -150;
  pConfigCpich->cpichType           = PXL_P_CPICH;
  pConfigCpich->diversityMode       = PXL_CPICH_SISO;
  pConfigCpich->resourceId          = resouceId;
  pConfigCpich->scramblingCodeIndex = 0;
  //printf(">--- PCL to PPL Ul capacity request message %d---<\n", PXL_ULCONFIG_CAPABILITYREQ);
  printf("PCL --->  PPL PCpich setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void InitDlPSchSetup(PxlMsg_t *pMsg, U16 resourceId, U8 cellId)
{
  PxlDlConfigSch_t *pConfigSch;
  //header
  pMsg->header.msgType    = PXL_DLCONFIG_SCH;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigSch_t);
  //body
  pConfigSch = (PxlDlConfigSch_t *)&(pMsg->msgUn);
  pConfigSch->cellId        = 0;
  pConfigSch->schType       = PXL_P_SCH;
  pConfigSch->chanPower     = -150;
  pConfigSch->diversityMode = TSTD_INACTIVE;
  pConfigSch->resourceId    = resourceId;
  //printf(">--- PCL to PPL Ul capacity request message %d---<\n", PXL_ULCONFIG_CAPABILITYREQ);
  printf("PCL --->  PPL Psch setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void InitDlSSchSetup(PxlMsg_t *pMsg, U16 resouceId, U8 cellId)
{
  PxlDlConfigSch_t *pConfigSch;
  //header
  pMsg->header.msgType    = PXL_DLCONFIG_SCH;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigSch_t);
  //body
  pConfigSch = (PxlDlConfigSch_t *)&(pMsg->msgUn);
  pConfigSch->cellId        = cellId;
  pConfigSch->schType       = PXL_S_SCH;
  pConfigSch->chanPower     = -150;
  pConfigSch->diversityMode = TSTD_INACTIVE;
  pConfigSch->resourceId    = resouceId;
  //printf(">--- PCL to PPL Ul capacity request message %d---<\n", PXL_ULCONFIG_CAPABILITYREQ);
  printf("PCL --->  PPL Ssch setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void InitDlPccpchSetup(PxlMsg_t *pMsg, U16 resouceId, U8 cellId)
{
  PxlDlConfigPccpch_t *pConfigPccpch;
  //header
  pMsg->header.msgType    = PXL_DLCONFIG_PCCPCH;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigPccpch_t);
  //body
  pConfigPccpch = (PxlDlConfigPccpch_t *)&(pMsg->msgUn);
  pConfigPccpch->cellId         = cellId;
  pConfigPccpch->chanPower      = -150;
  pConfigPccpch->diversityMode  = STTD_INACTIVE;
  pConfigPccpch->resourceId     = resouceId;
  //printf(">--- PCL to PPL Ul capacity request message %d---<\n", PXL_ULCONFIG_CAPABILITYREQ);
  printf("PCL --->  PPL Pccpch setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void ConfigSccpch(PxlMsg_t *pMsg, IPG::ParamList *par)
{
  PxlDlConfigSccpch_t *pConfig;
  pMsg->header.msgType    = PXL_DLCONFIG_SCCPCH;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigSccpch_t);

  pConfig = (PxlDlConfigSccpch_t *)&(pMsg->msgUn);
  pConfig->resourceId           = par->fetch<int>("UserId");
  pConfig->cellId               = 0;
  pConfig->scramblingCodeIndex  = par->fetch<int>("scramblingCodeIndex");
  pConfig->chanCodeIndex        = par->fetch<int>("chanCodeIndex");
  pConfig->slotFormat           = par->fetch<int>("Ch_SlotFormat");
  pConfig->chipOffset           = par->fetch<int>("chipOffset");
  pConfig->tfciPresence         = TFCI_PRESENT;
  pConfig->diversityMode        = TxDivInd_e(par->fetch<int>("diversityMode"));
  pConfig->powerOffset1         = 0;
  pConfig->powerOffset3         = 0;
  //printf("PCL --->  PPL Sccpch setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void InitDlHsscchSetup(PxlMsg_t *pMsg, IPG::ParamList *par)
{
  PxlDlConfigHsscch_t *pConfigHscch;

  pMsg->header.msgType    = PXL_DLCONFIG_HSSCCH;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigHsscch_t);

  pConfigHscch = (PxlDlConfigHsscch_t*)&(pMsg->msgUn);
  pConfigHscch->cellId              = par->fetch<int>("cellId");
  pConfigHscch->chanCodeIndex       = par->fetch<int>("chanCodeIndex");
  pConfigHscch->resourceId          = par->fetch<int>("resourceId");
  pConfigHscch->scramblingCodeIndex = par->fetch<int>("scramblingCodeIndex");

  //printf("PCL --->  PPL hsscch setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void ConfigDynamicHsssch(PxlMsg_t *pMsg, IPG::ParamList *par)
{
  PxlDlHsDynamicHsscch_t *pDynamicHsscch;
  pMsg->header.msgType = PXL_DLHSDYNAMIC_HSSCCH;
  pMsg->header.msgBodyLen  = sizeof(PxlDlHsDynamicHsscch_t);

  pDynamicHsscch = (PxlDlHsDynamicHsscch_t *)&(pMsg->msgUn);
  pDynamicHsscch->resourceId    = par->fetch<int>("resourceId");
  pDynamicHsscch->hsscchType    = par->fetch<int>("hsscchType");
  pDynamicHsscch->diversityMode = par->fetch<int>("diversityMode");
  pDynamicHsscch->frameNumber   = 0;
  pDynamicHsscch->hRnti         = par->fetch<int>("hRnti");
  pDynamicHsscch->ndi           = par->fetch<int>("ndi");
  pDynamicHsscch->harqProc      = par->fetch<int>("harqProc");
  pDynamicHsscch->priRcv        = par->fetch<int>("priRcv");
  pDynamicHsscch->secRcv        = par->fetch<int>("secRcv");
  pDynamicHsscch->subframeNumber= 0;
  pDynamicHsscch->userId        = par->fetch<int>("UserId");
  if((pDynamicHsscch->hsscchType    == PXL_HSSCCH_TYPE1)
      ||(pDynamicHsscch->hsscchType == PXL_HSSCCH_TYPE3_1TB)
      || (pDynamicHsscch->hsscchType== PXL_HSSCCH_TYPE3_2TB))
  {
    pDynamicHsscch->priTbsi.tbsi= par->fetch<int>("priTbsi");
    pDynamicHsscch->secTbsi.tbsi= par->fetch<int>("secTbsi");
  }
  else if(pDynamicHsscch->hsscchType== PXL_HSSCCH_TYPE2)
  {
    pDynamicHsscch->priTbsi.specialInfoType = par->fetch<int>("priTbsi");
    pDynamicHsscch->secTbsi.specialInfo     = par->fetch<int>("secTbsi");
  }
  else
  {
    pDynamicHsscch->priTbsi.xord= par->fetch<int>("priTbsi");
    pDynamicHsscch->secTbsi.xodt= par->fetch<int>("secTbsi");
  }
  pDynamicHsscch->xms   = par->fetch<int>("xms");
  pDynamicHsscch->pwi   = par->fetch<int>("pwi");

  int hspdschActive     = par->fetch<int>("hspdschActive");
  if (hspdschActive > 0)
  {
    U8 O, P, tmp, tmp1;
    O     = par->fetch<int>("startCodeIndex");
    P     = par->fetch<int>("numCodes");
    tmp   = MMIN(P-1, 15-P);
    tmp1  = ABS(O-1-(P/8)*15);
    pDynamicHsscch->xccs  = (tmp << 4) | (tmp1 & 0x0f);
  }
  else
  {
    pDynamicHsscch->xccs  = par->fetch<int>("xccs");
  }
}

void InitDlHspdschSetup(PxlMsg_t *pMsg, IPG::ParamList *par)
{
  PxlDlHsDynamicHspdsch_t *pConfig;
  int firstDescHdr;
  Vec<ParamList> tbList;
  IPG::VecINT bitOffsetFlag, blkSize, numBlks;

  //header
  pMsg->header.msgType    = PXL_DLHSDYNAMIC_HSPDSCH;
  pMsg->header.msgBodyLen = sizeof(PxlDlHsDynamicHspdsch_t);
  //body
  pConfig = (PxlDlHsDynamicHspdsch_t *)&(pMsg->msgUn);
  memset(pConfig, 0, sizeof(PxlDlHsDynamicHspdsch_t));

  //read parameters from input file
  pConfig->cellId               = par->fetch<int>("cellId");
  pConfig->scramblingCodeIndex  = par->fetch<int>("scramblingCodeIndex");
  pConfig->mimoMode             = par->fetch<int>("mimoMode");
  pConfig->pwi                  = par->fetch<int>("pwi");
  pConfig->startCodeIndex       = par->fetch<int>("startCodeIndex");
  pConfig->numCodes             = par->fetch<int>("numCodes");
  pConfig->nIR                  = par->fetch<int>("nIR");
  pConfig->crcAttachMethod      = par->fetch<int>("crcAttachMethod");
  pConfig->hRnti                = par->fetch<int>("hRnti");
  pConfig->codePower            = par->fetch<int>("codePower");
  pConfig->diversityMode        = par->fetch<int>("diversityMode");
  pConfig->numTBs               = par->fetch<int>("numTBs");

  tbList.setLength(pConfig->numTBs);
  for(int i=0 ; i<pConfig->numTBs; i++)
  {
    stringstream TBName;
    TBName << "TB_" << i;
    tbList[i] = par->fetch<ParamList>(TBName.str());

    pConfig->tbDesc[i].tbs        = tbList[i].fetch<int>("tbs");
    pConfig->tbDesc[i].modType    = tbList[i].fetch<int>("modType");
    pConfig->tbDesc[i].rvCoding   = tbList[i].fetch<int>("rvCoding");
    pConfig->tbDesc[i].numDataDesc= tbList[i].fetch<int>("numDataDesc");
    bitOffsetFlag = tbList[i].fetch<Array<int>>("bitOffsetFlag");
    blkSize       = tbList[i].fetch<Array<int>>("blkSize");
    numBlks       = tbList[i].fetch<Array<int>>("numBlks");
    for(int j=0; j<pConfig->tbDesc[i].numDataDesc; j++)
    {
      pConfig->tbDesc[i].dataDesc[j].bitOffsetFlag= (PxlBool8_t)bitOffsetFlag[j];
      pConfig->tbDesc[i].dataDesc[j].blkSize      = blkSize[j];
      pConfig->tbDesc[i].dataDesc[j].numBlks      = numBlks[j];
      pConfig->tbDesc[i].dataDesc[j].pData        = (U8 *)GARBAGE_POINTER;  // garbage pointer gets overwritten by pclEmulator
    }
  }

  // But if the firstDescHdr flag is set, then the first descriptor is the header and pclEmulator needs to know about it
  firstDescHdr = par->fetch<int>("firstDescIsHdr");
  if (firstDescHdr == 1)
  {
    pConfig->tbDesc[0].dataDesc[0].pData = NULL;
  }
  //printf("PCL --->  PPL hspdsch setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void InitDlChannelDeletion(PxlMsg_t *pMsg, PxlResourceType_e resourceType, U16 resourceId, U8 cellId)
{
  PxlDlConfigChanDelete_t  *pDlConfig;

  //header
  pMsg->header.msgType    = PXL_DLCONFIG_CHANDELETE;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigChanDelete_t);
  //body
  pDlConfig = (PxlDlConfigChanDelete_t *)&(pMsg->msgUn);
  pDlConfig->resourceType       = PXL_USER;
  pDlConfig->resourceType       = resourceType;
  pDlConfig->resourceId.userId  = resourceId;
  //printf("PCL ---> PPL user %d deletion request \n", userId);
}

void InitDlUserSetup(PxlMsg_t *pMsg, IPG::ParamList *par, U8 cellId, U16 chipOffset, U8 scramblingCodeIndex)
{
  PxlDlConfigUser_t *pConfigUser;
  U16 phyChType = par->fetch<int>("PhyCh_Type");

  //header
  pMsg->header.msgType    = PXL_DLCONFIG_USER;
  pMsg->header.msgBodyLen = sizeof(PxlDlConfigUser_t);
  //bodycd 
  pConfigUser       = (PxlDlConfigUser_t *)&(pMsg->msgUn);
  memset(pConfigUser, 0, sizeof(PxlDlConfigUser_t));

  //dpchSlotFormat    = par->fetch<int>("Ch_SlotFormat");
  //int numBits       = DL_DPCH_TABLE[dpchSlotFormat*3][5];
  //int spreadFactor  = 2560 / numBits * 2;
  //U8 chanCodeIndex  = spreadFactor >> 1;

  pConfigUser->cellId         = par->fetch<int>("CellId");
  pConfigUser->userId         = par->fetch<int>("UserId");

  if (phyChType == PXL_HSSCCH)
  {
    // Some parameters are fixed for HSSCCH
    pConfigUser->dpchSlotFormat     = 4;
    pConfigUser->dlTpcStepSize      = STEP_SIZE_0_5_DB;
    pConfigUser->limitedPowerInc    = LIM_POW_INC_USED;
    pConfigUser->ilpcStatus         = INNER_LP_PC_STAT_INACTIVE;
    pConfigUser->powerOffset1       = 0;
    pConfigUser->powerOffset2       = 0;
    pConfigUser->powerOffset3       = 0;
    // SRB on channel code to prevent overlap with HS-DSCH
    pConfigUser->chanCodeIndex      = 15;
    pConfigUser->diversityMode      = TXDIV_INACTIVE;
    pConfigUser->frameOffset        = 1;
    pConfigUser->initialPower       = -200;
    pConfigUser->maxPower           = -50;
    pConfigUser->minPower           = -400;
    pConfigUser->hsScchInfo.hsscchRequired  = TRUE;
    pConfigUser->hsScchInfo.chanPowerRef    = PXL_POFF_REF_MAXREFAMP;
    pConfigUser->hsScchInfo.chanPower       = -150;
  }
  else
  {
    pConfigUser->dpchSlotFormat     = par->fetch<int>("Ch_SlotFormat");
    pConfigUser->dlTpcStepSize      = (DlTpcSetpSize_e)(par->fetch<int>("dlTpcStepSize"));
    pConfigUser->limitedPowerInc    = (LimitedPowerInc_e)(par->fetch<int>("limitedPowerInc"));
    pConfigUser->ilpcStatus         = (InnerLpDlPcStatus_e)(par->fetch<int>("ilpcStatus"));
    pConfigUser->powerOffset1       = par->fetch<int>("powerOffset1");
    pConfigUser->powerOffset2       = par->fetch<int>("powerOffset2");
    pConfigUser->powerOffset3       = par->fetch<int>("powerOffset3");
    pConfigUser->chanCodeIndex      = par->fetch<int>("chanCodeIndex");
    pConfigUser->diversityMode      = (TxDivInd_e)(par->fetch<int>("diversityMode"));
    pConfigUser->frameOffset        = par->fetch<int>("frameOffset");
    pConfigUser->initialPower       = par->fetch<int>("initialPower");
    pConfigUser->maxPower           = par->fetch<int>("maxPower");
    pConfigUser->minPower           = par->fetch<int>("minPower");
  }

  pConfigUser->fdpchPresence        = FALSE;
  pConfigUser->fdpchSlotFormat      = 0;
  pConfigUser->chipOffset           = chipOffset;//par->fetch<int>("chipOffset");
  pConfigUser->scramblingCodeIndex  = scramblingCodeIndex;//par->fetch<int>("scramblingCodeIndex");
  pConfigUser->dlPowerBalInfo.dlPowerBalActive  = false;
  pConfigUser->dlPowerBalInfo.maxAdjStep        = 1;
  pConfigUser->dlPowerBalInfo.dlRefPower        = -100;
  pConfigUser->dlPowerBalInfo.adjPeriod         = 1;
  pConfigUser->dlPowerBalInfo.adjRatio          = 50;
  //printf("PCL --->  PPL User setup request id %x,msgBodyLen  %d\n",pMsg->header.msgType,pMsg->header.msgBodyLen);
}

void TempPclMsgConfigR99Frame(DlCctrch_t *Rel99, U16 userId, U16 activeTrch, PxlDlDynamicPhych_e dlPhychType, PxlDlDynamicR99Frame_t *pDynamicR99Frame)
{
  S16 trCh;

  pDynamicR99Frame->chanId.userId         = userId;
  pDynamicR99Frame->dlPhychType           = dlPhychType;
  pDynamicR99Frame->chanPower             = -150;
  pDynamicR99Frame->cmInfo.cmActive       = FALSE;
  pDynamicR99Frame->dtxOverrideInfo       = PXL_DL_DTX_NONE;
  pDynamicR99Frame->frameNumber           = 0;
  pDynamicR99Frame->secondInterleaverSize = Rel99->dlFecDriverCctrch.secondInterleaverSize;
  pDynamicR99Frame->numTrch               = activeTrch;
  for (trCh=0; trCh<MAX_DL_TRCH; trCh++)
  {
    pDynamicR99Frame->trchId[trCh]        = (U8)trCh;
  }
}

void TempPclMsgConfigR99Trch(DlCctrch_t *Rel99, U16 userId, U16 trChId, PxlDlDynamicR99Tti_t *pDynamicR99Tti, PxlDlDynamicTrch_e dlTrchType)
{
  S16 k;
  U16 bitSize[MAX_DL_TRCH] = {0};
  U8 *pBuffer[MAX_DL_TRCH] = {0};

  pDynamicR99Tti->chanId.userId = userId;
  pDynamicR99Tti->codeBlkCount  = Rel99->trCh[trChId].rateMatchParms.C;
  pDynamicR99Tti->codeBlkLen    = Rel99->trCh[trChId].rateMatchParms.K;
  if (Rel99->trCh[trChId].dlFecDriverTrch.codeType == TURBO_ONE_THIRD)
  {
    pDynamicR99Tti->codingType = CODING_TURBO_13;
  }
  else if (Rel99->trCh[trChId].dlFecDriverTrch.codeType == CONV_ONE_THIRD)
  {
    pDynamicR99Tti->codingType = CODING_CONV_13;
  }
  else if (Rel99->trCh[trChId].dlFecDriverTrch.codeType == CONV_ONE_HALF)
  {
    pDynamicR99Tti->codingType = CODING_CONV_12;
  }

  pDynamicR99Tti->crcType     = (CrcType_e)Rel99->trCh[trChId].dlFecDriverTrch.crcSize;
  pDynamicR99Tti->dlTrchType  = dlTrchType;
  for (k=0; k<3; k++)
  {
    pDynamicR99Tti->eValues[k].Eini   = Rel99->trCh[trChId].rateMatchParms.eini[k];
    pDynamicR99Tti->eValues[k].Eminus = Rel99->trCh[trChId].rateMatchParms.eminus[k];
    pDynamicR99Tti->eValues[k].Eplus  = Rel99->trCh[trChId].rateMatchParms.eplus[k];
  }
  pDynamicR99Tti->fillerBitCount      = Rel99->trCh[trChId].rateMatchParms.Y;
  pDynamicR99Tti->firstInterleaverSize= Rel99->trCh[trChId].rateMatchParms.DTX_Out_length;

  switch (Rel99->trCh[trChId].dlFecDriverTrch.rateMatchType)
  {
    case R99_REPETITION:
      pDynamicR99Tti->rateMatchType = PXL_DL_RM_REPEAT;
      break;
    case R99_TURBO_PUNCTURING:
      pDynamicR99Tti->rateMatchType = PXL_DL_RM_TURBO_PUNC;
      break;
    case R99_CONV_PUNCTURING:
      pDynamicR99Tti->rateMatchType = PXL_DL_RM_CONV_PUNC;
      break;
    case R99_NOT_REP_OR_PUNCT:
      pDynamicR99Tti->rateMatchType = PXL_DL_RM_NONE;
      break;
  }
  pDynamicR99Tti->trBlkCount = Rel99->trCh[trChId].dlFecDriverTrch.numTrBksM;
  pDynamicR99Tti->trBlkLen = Rel99->trCh[trChId].dlFecDriverTrch.trBksizeA;
  pDynamicR99Tti->trchId = trChId;
  pDynamicR99Tti->tti = (Tti_e)(Rel99->trCh[trChId].dlFecDriverTrch.tti - 1);
  pDynamicR99Tti->frameNumber = 0;

  // input buffer
  pDynamicR99Tti->pTrchData = 0;
}

void TestMacConfigureFACHPICH(DlCctrch_t *Rel99, IPG::ParamList *par, U8 userId, int TFCI)
{
  int i, numTrCh, j;
  U32 result, tfcNum;
  int MaxTFCINum;
  U32 tableIndex;
  int crcLength, rmAttribute, encType, ttiValue, maxTrBlkInTfs,  fpMode;
  IPG::VecINT blkSize,numTrBlks,TFC;
  ParamList      TrchPar;
  Vec<ParamList> trchList;
  int DTXposition;
  U8 tfciConfig[MAX_DL_TFCI_NUM][MAX_DL_TRCH] = {0};

  Rel99->slotFormat               = par->fetch<int>("Ch_SlotFormat");
  Rel99->dlFecDriverCctrch.numTrCh= par->fetch<int>("numTrCh");
  Rel99->trChType                 = par->fetch<int>("TrCh_Type");
  MaxTFCINum                      = par->fetch<int>("MaxTFCINum");
  DTXposition                     = par->fetch<int>("DTXposition");
  Rel99->flexibleDTX              = DTXposition;

  // read DCH parameters
  tableIndex  = Rel99->slotFormat*3; //normal frame
  numTrCh     = Rel99->dlFecDriverCctrch.numTrCh;
  trchList.setLength(numTrCh);
  for(i=0; i<numTrCh; i++)
  {
    stringstream trchName;
    trchName << "trch_" << i;
    trchList[i] = par->fetch<ParamList>(trchName.str());
    blkSize     = trchList[i].fetch<Array<int>>("trBlkSizeA");
    numTrBlks   = trchList[i].fetch<Array<int>>("NumTrBlks");
    TFC         = trchList[i].fetch<Array<int>>("TFC");
    tfcNum      = trchList[i].fetch<int>("NumTfc");
    for(j=0; j< MaxTFCINum; j++)
    {
      tfciConfig[j][i] = numTrBlks[TFC[j]];
    }
    crcLength     = trchList[i].fetch<int>("CRC_length");
    rmAttribute   = trchList[i].fetch<int>("RM_attribute");
    encType       = trchList[i].fetch<int>("Encoder");
    ttiValue      = trchList[i].fetch<int>("TTI_value");
    maxTrBlkInTfs = trchList[i].fetch<int>("MaxTrBkwithinTFS");
    fpMode        = trchList[i].fetch<int>("Frame_Protocol");

    Rel99->trCh[i].modScheme = QPSK;
    Rel99->trCh[i].dlFecDriverTrch.trBksizeA = (U16)blkSize[TFC[TFCI]];
    if(DTXposition == 0)//fixed
    {
      if(numTrBlks[TFC[TFCI]] == 0)
      {
        Rel99->trCh[i].dlFecDriverTrch.numTrBksM = 1;
        Rel99->trCh[i].tbNumChange = 1;
      }
      else
      {
        Rel99->trCh[i].dlFecDriverTrch.numTrBksM = (U8)numTrBlks[TFC[TFCI]];
        Rel99->trCh[i].tbNumChange = 0;
      }
    }
    else
    {
      Rel99->trCh[i].dlFecDriverTrch.numTrBksM = (U8)numTrBlks[TFC[TFCI]];	
    }

    for(j=0; j<tfcNum; j++)
    {
      Rel99->trCh[i].blkSizeArray[j]    = blkSize[j];
      Rel99->trCh[i].numTrBlksArray[j]  = numTrBlks[j];
    }

    for(j=0; j<MaxTFCINum; j++)
    {
      Rel99->tfciConfig[j][i] = numTrBlks[TFC[j]];
      Rel99->trCh[i].TFC[j]   = TFC[j];
    }

    Rel99->trCh[i].dlFecDriverTrch.crcSize          = (DlFecCrc_e)crcLength;
    Rel99->trCh[i].rmAttribute                      = (U16)rmAttribute;
    Rel99->trCh[i].maxTrBkWithinTFS                 = (U16)maxTrBlkInTfs;
    Rel99->trCh[i].dlFecDriverTrch.tti              = (DlFecTti_e)ttiValue;
    Rel99->trCh[i].dlFecDriverTrch.codeType         = (DlFecCodeType_e)encType;
    Rel99->trCh[i].dlFecDriverTrch.useFrameProtocol = (BOOLEAN)fpMode;
  }

  // Calculate the rate matching parameters for each TrCh of this CCTrCh
  if (DTXposition == 1)
  {
    result = ComputeR99RateMatchingFlexibleDTX(Rel99, tfciConfig, MaxTFCINum, TFCI);
  }
  else
  {
    result = ComputeR99RateMatchParms(Rel99);
  }

  for (i=0; i<Rel99->dlFecDriverCctrch.numTrCh; i++)
  {
    if (Rel99->trCh[i].tbNumChange == 1)
    {
      Rel99->trCh[i].dlFecDriverTrch.numTrBksM = 0;
      Rel99->trCh[i].rateMatchParms.C = 0;
    }
    TempPclMsgConfigR99Trch(Rel99, userId, i, &R99PxlMsg.R99TrCh[i], PXL_DLDYNAMIC_FACH);
  }
  TempPclMsgConfigR99Frame(Rel99, userId, Rel99->dlFecDriverCctrch.numTrCh, PXL_DLDYNAMIC_SCCPCH, &R99PxlMsg.R99Frame);

  R99PxlMsg.R99Frame.tfciValue  = TFCI;
  R99PxlMsg.numTrch             = Rel99->dlFecDriverCctrch.numTrCh;
  R99PxlMsg.slotFormat          = Rel99->slotFormat;
}

void TestMacConfigureDCH(DlCctrch_t *Rel99, IPG::ParamList *par, int TFCI)
{
  U32 result,tfcNum;
  U32  tableIndex, slotSize;
  int crcLength, rmAttribute, encType, ttiValue, maxTrBlkInTfs,  fpMode;
  IPG::VecINT blkSize,numTrBlks,TFC;
  //ParamList      TrchPar;
  Vec<ParamList> trchList;
  U8 tfciConfig[MAX_DL_TFCI_NUM][MAX_DL_TRCH] = {0};

  Rel99->slotFormat               = par->fetch<int>("Ch_SlotFormat");
  Rel99->dlFecDriverCctrch.numTrCh= par->fetch<int>("numTrCh");
  Rel99->trChType                 = par->fetch<int>("TrCh_Type");
  int MaxTFCINum                  = par->fetch<int>("MaxTFCINum");
  // TFCI = par->fetch<int>("TFCI");	
  int DTXposition                 = par->fetch<int>("DTXposition");	
  Rel99->flexibleDTX              = DTXposition;

  // read DCH parameters
  tableIndex  = Rel99->slotFormat * 3; //normal frame
  slotSize    =  DL_DPCH_TABLE[tableIndex][0] + DL_DPCH_TABLE[tableIndex][1];
  copySize    = ((slotSize - 1) / 4) + 1;
  copySize    = copySize * 15;
  copySize    = ((copySize + 3) / 4) * 4;
  //printf("-----copySize %d\n", copySize);
  int numTrCh = Rel99->dlFecDriverCctrch.numTrCh;
  trchList.setLength(numTrCh);
  for (int i=0; i<numTrCh; i++)
  {
    stringstream trchName;

    trchName << "trch_" << i;
    trchList[i] = par->fetch<ParamList>(trchName.str());
    blkSize     = trchList[i].fetch<Array<int>>("trBlkSizeA");
    numTrBlks   = trchList[i].fetch<Array<int>>("NumTrBlks");
    TFC         = trchList[i].fetch<Array<int>>("TFC");
    tfcNum      = trchList[i].fetch<int>("NumTfc");

    for (int j=0; j<MaxTFCINum; j++)
    {
      tfciConfig[j][i] = numTrBlks[TFC[j]];
    }

    crcLength     = trchList[i].fetch<int>("CRC_length");
    rmAttribute   = trchList[i].fetch<int>("RM_attribute");
    encType       = trchList[i].fetch<int>("Encoder");
    ttiValue      = trchList[i].fetch<int>("TTI_value");
    maxTrBlkInTfs = trchList[i].fetch<int>("MaxTrBkwithinTFS");
    fpMode        = trchList[i].fetch<int>("Frame_Protocol");

    Rel99->trCh[i].modScheme                  = QPSK;
    Rel99->trCh[i].dlFecDriverTrch.trBksizeA  = (U16)blkSize[TFC[TFCI]];
    if (DTXposition == 0) //fixed
    {
      if (numTrBlks[TFC[TFCI]] == 0)
      {
        Rel99->trCh[i].dlFecDriverTrch.numTrBksM  = 1;
        Rel99->trCh[i].tbNumChange                = 1;
      }
      else
      {
        Rel99->trCh[i].dlFecDriverTrch.numTrBksM  = (U8)numTrBlks[TFC[TFCI]];
        Rel99->trCh[i].tbNumChange                = 0;
      }
    }
    else
    {
      Rel99->trCh[i].dlFecDriverTrch.numTrBksM    = (U8)numTrBlks[TFC[TFCI]];	
    }

    for (int j=0; j<tfcNum; j++)
    {
      Rel99->trCh[i].blkSizeArray[j]    = blkSize[j];
      Rel99->trCh[i].numTrBlksArray[j]  = numTrBlks[j];
    }
    for (int j=0; j<MaxTFCINum; j++)
    {
      Rel99->tfciConfig[j][i] = numTrBlks[TFC[j]];
      Rel99->trCh[i].TFC[j]   = TFC[j];
    }

    Rel99->trCh[i].dlFecDriverTrch.crcSize  = (DlFecCrc_e)crcLength;
    Rel99->trCh[i].rmAttribute              = (U16)rmAttribute;
    Rel99->trCh[i].maxTrBkWithinTFS         = (U16)maxTrBlkInTfs;
    Rel99->trCh[i].dlFecDriverTrch.tti      = (DlFecTti_e)ttiValue;
    Rel99->trCh[i].dlFecDriverTrch.codeType = (DlFecCodeType_e)encType;
    Rel99->trCh[i].dlFecDriverTrch.useFrameProtocol = (BOOLEAN)fpMode;
  }

  // Calculate the rate matching parameters for each TrCh of this CCTrCh
  if (DTXposition == 1)
  {
    result = ComputeR99RateMatchingFlexibleDTX(Rel99, tfciConfig, MaxTFCINum, TFCI);
  }
  else	
  {
    result = ComputeR99RateMatchParms(Rel99);
  }

  U16 userId = par->fetch<int>("UserId");
  for (int i=0; i<Rel99->dlFecDriverCctrch.numTrCh; i++)
  {
    if (Rel99->trCh[i].tbNumChange == 1)
    {
      Rel99->trCh[i].dlFecDriverTrch.numTrBksM = 0;
      Rel99->trCh[i].rateMatchParms.C = 0;
    }
    TempPclMsgConfigR99Trch(Rel99, userId, i, &R99PxlMsg.R99TrCh[i], PXL_DLDYNAMIC_DCH);
  }

  TempPclMsgConfigR99Frame(Rel99, userId, Rel99->dlFecDriverCctrch.numTrCh, PXL_DLDYNAMIC_DPCH, &R99PxlMsg.R99Frame);
  R99PxlMsg.R99Frame.tfciValue  = TFCI;
  R99PxlMsg.numTrch             = Rel99->dlFecDriverCctrch.numTrCh;
  R99PxlMsg.slotFormat          = Rel99->slotFormat;
}

RESULTCODE testcase(std::string& testCaseParamFileName)
{
  RESULTCODE res=SUCCESS;
  //U32 portArray[6] = {0,1,2,6,7,8};
  DlCctrch_t Rel99 = {0};
  U32 apiSize;
  U8 *pTmp;
  U32 portId;
  U32 *pAdd;
  PxlMsg_t apiStruct;

  // user test code starts here
  // -------------------------------------------
  std::string testName(testCaseParamFileName);
  teePrintf("running test case %s\n", testName.c_str());

  // return the filenames of all files that have the extension .par
  namespace fs = ::boost::filesystem;
  if (!fs::exists(testName))        return FAILURE;
  if (!fs::is_directory(testName))  return FAILURE;
  fs::directory_iterator it(testName);
  for (fs::directory_iterator it=fs::directory_iterator(testName); 
      it!=fs::directory_iterator(); it++)
  {
    if (!fs::is_regular_file(*it) || it->path().extension() != ".par") 
    {
      continue;
    }
    cout << "Read Par File: " << it->path().filename() << endl;
    IPG::ParamList par;
    par.init(it->path().string());
    par_array.push_back(par);
  }
  if (par_array.empty())            return FAILURE;

  MapDdrAddr = (U32)0x30184300; //memr(MAP_CRAM_ADDR);
  printf("tx0LogPa %08x \n", MapDdrAddr);

  // Process channel configuration one by one
  for (std::vector<IPG::ParamList>::iterator it=par_array.begin();
      it!=par_array.end(); it++)
  {
    U16 phyChType = it->fetch<int>("PhyCh_Type");
    //PXL_PICH,
    //PXL_MICH,
    //PXL_AICH,
    //PXL_EAGCH,
    //PXL_ERGCH_EHICH,
    //PXL_USER,
    //PXL_NONE
    if (phyChType == PXL_CPICH)
    {
      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      InitDlPcpichSetup(&apiStruct,0,0);
      portId  = apiStruct.header.msgType/0x100;
      pAdd    = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL);
    }
    else if (phyChType == PXL_SCH)
    {
      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      InitDlPSchSetup(&apiStruct,0,0);
      portId = apiStruct.header.msgType/0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL);

      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      InitDlSSchSetup(&apiStruct,1,0);
      portId = apiStruct.header.msgType/0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL);
    }
    else if (phyChType == PXL_PCCPCH)
    {
      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      InitDlPccpchSetup(&apiStruct, 0, 0);
      portId  = apiStruct.header.msgType / 0x100;
      pAdd    = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL);
    }
    else if (phyChType == PXL_SCCPCH)
    {
      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      int maxTfciNum = it->fetch<int>("MaxTFCINum");

      ConfigSccpch(&apiStruct, &(*it));
      portId = apiStruct.header.msgType / 0x100;
      pAdd    = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);    // this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR,portId);

      //dynamic message
      TestMacConfigureFACHPICH(&Rel99, &(*it), 0, maxTfciNum-1);
      for(int i=0; i < R99PxlMsg.numTrch; i++)
      {
        apiStruct.header.msgType = PXL_DLDYNAMIC_R99TTI;
        apiStruct.header.msgBodyLen = sizeof(PxlDlDynamicR99Tti_t);
        portId = apiStruct.header.msgType / 0x100;
        memcpy(&apiStruct.msgUn, &R99PxlMsg.R99TrCh[i], sizeof(PxlDlDynamicR99Tti_t));
        apiSize= sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
        apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
        sendApi(MSG_TYPE_PPL_API, MSG_DEST_PCLEMULATOR,portId);
      }

      apiStruct.header.msgType = PXL_DLDYNAMIC_R99FRAME;
      apiStruct.header.msgBodyLen = sizeof(PxlDlDynamicR99Frame_t);
      pTmp = (U8 *)&apiStruct;
      pTmp += 3;
      *pTmp = 1;
      portId = apiStruct.header.msgType / 0x100;
      memcpy(&apiStruct.msgUn, &R99PxlMsg.R99Frame, sizeof(PxlDlDynamicR99Frame_t));
      apiSize= sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);    // this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PCLEMULATOR,portId);

    }
    else if (phyChType == PXL_DPCH)
    {
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      int maxTfciNum = it->fetch<int>("MaxTFCINum");
      U16 userId = it->fetch<int>("UserId");

#ifdef FEC_BP_TEST
      memw(TESTING_SWITCH, 1);
#endif

      //for(k=1; k< maxTfciNum; k++)
      InitDlUserSetup(&apiStruct, &(*it), 0, 0, 0);
      portId = apiStruct.header.msgType / 0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);

      //dynamic message
      TestMacConfigureDCH(&Rel99, &(*it), maxTfciNum-1);

      for(int i=0; i < R99PxlMsg.numTrch; i++)
      {
        apiStruct.header.msgType    = PXL_DLDYNAMIC_R99TTI;
        apiStruct.header.msgBodyLen = sizeof(PxlDlDynamicR99Tti_t);
        portId = apiStruct.header.msgType / 0x100;
        memcpy(&apiStruct.msgUn, &R99PxlMsg.R99TrCh[i], sizeof(PxlDlDynamicR99Tti_t));
        apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
        apiAddSegment0(&apiStruct, apiSize);    // this is send actual API length
        sendApi(MSG_TYPE_PPL_API, MSG_DEST_PCLEMULATOR, portId);
      }

      apiStruct.header.msgType    = PXL_DLDYNAMIC_R99FRAME;
      apiStruct.header.msgBodyLen = sizeof(PxlDlDynamicR99Frame_t);
      pTmp = (U8*)&apiStruct;
      pTmp += 3;
      *pTmp = 1;
      portId = apiStruct.header.msgType / 0x100;
      memcpy(&apiStruct.msgUn, &R99PxlMsg.R99Frame, sizeof(PxlDlDynamicR99Frame_t));
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);    // this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PCLEMULATOR,portId);
    }
    else if (phyChType == PXL_HSSCCH)
    {
      //capacity request
      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      InitDlHsscchSetup(&apiStruct, &(*it));
      portId  = apiStruct.header.msgType/0x100;
      pAdd    = (U32*)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);  // this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);

      // DL DCH
      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      InitDlUserSetup(&apiStruct, &(*it), 0, 0, 0);
      portId = apiStruct.header.msgType/0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);  // this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);

      // Dynamic HSSCCH
      memset(&apiStruct, 0, sizeof(PxlMsg_t));
      ConfigDynamicHsssch(&apiStruct, &(*it));
      pTmp = (U8 *)&apiStruct;
      pTmp += 3;
      *pTmp = 1;
      portId = apiStruct.header.msgType/0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);  // this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PCLEMULATOR, portId);

      int hspdschActive = it->fetch<int>("hspdschActive");
      if (hspdschActive > 0)
      {
        //hspdsch setup
        memset(&apiStruct, 0, sizeof(PxlMsg_t));
        InitDlHspdschSetup(&apiStruct, &(*it));
        pTmp = (U8 *)&apiStruct;
        pTmp += 3;
        *pTmp = 1;
        portId = apiStruct.header.msgType/0x100;
        pAdd = (U32 *)&apiStruct;
        apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
        apiAddSegment0(&apiStruct, apiSize);  // this is send actual API length
        sendApi(MSG_TYPE_PPL_API, MSG_DEST_PCLEMULATOR, portId);
      }

    }

#ifdef FEC_BP_TEST
    udelay(10000);
    memw(FEC_CNT_ADDR, 2);
    memw(BP_ADDR_PTR, BP_BASE_ADDR);
    udelay(10000);
#endif

  }

  //int tfci = par.fetch<int>("TFCI");
  //   dumpMemAndSave(testCaseParamFileName,k);
  dumpMemAndCompare(testCaseParamFileName, 1);
  Sleep(1000);

  for (std::vector<IPG::ParamList>::iterator it=par_array.begin();
      it!=par_array.end(); it++)
  {
    PxlMsg_t apiStruct;
    U16 phyChType = it->fetch<int>("PhyCh_Type");

    if (phyChType == PXL_CPICH)
    {
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      InitDlChannelDeletion(&apiStruct, PXL_CPICH, 0, 0);
      portId = apiStruct.header.msgType / 0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);
    }
    else if (phyChType == PXL_SCH)
    {
      // delete S-SCH (resourceId==1)
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      InitDlChannelDeletion(&apiStruct, PXL_SCH, 1, 0);
      portId = apiStruct.header.msgType / 0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);

      // delete P-SCH (resourceId==0)
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      InitDlChannelDeletion(&apiStruct, PXL_SCH, 0, 0);
      portId = apiStruct.header.msgType / 0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);
    }
    else if (phyChType == PXL_PCCPCH)
    {
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      InitDlChannelDeletion(&apiStruct, PXL_PCCPCH, 0, 0);
      portId = apiStruct.header.msgType / 0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);
    }
    else if (phyChType == PXL_SCCPCH)
    {
      // create mesage PXL_DLCONFIG_CHANDELETE
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      U16 resourceId = it->fetch<int>("UserId");
      InitDlChannelDeletion(&apiStruct, PXL_SCCPCH, resourceId, 0);
      portId = apiStruct.header.msgType / 0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);
    }
    else if (phyChType == PXL_DPCH)
    {
      // create mesage PXL_DLCONFIG_CHANDELETE
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      U16 userId = it->fetch<int>("UserId");
      InitDlChannelDeletion(&apiStruct, PXL_USER, userId, 0);
      portId = apiStruct.header.msgType / 0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);
    }
    else if (phyChType == PXL_HSSCCH)
    {
      U16 userId      = it->fetch<int>("UserId");
      U16 resourceId  = it->fetch<int>("resourceId");

      // create mesage PXL_DLCONFIG_CHANDELETE
      memset(&apiStruct,0,sizeof(PxlMsg_t));
      InitDlChannelDeletion(&apiStruct, PXL_HSSCCH, resourceId, 0);
      portId = apiStruct.header.msgType/0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);

      memset(&apiStruct,0,sizeof(PxlMsg_t));
      InitDlChannelDeletion(&apiStruct, PXL_USER, userId, 0);
      portId = apiStruct.header.msgType/0x100;
      pAdd = (U32 *)&apiStruct;
      apiSize = sizeof(PxlMsgHdr_t) + apiStruct.header.msgBodyLen;
      apiAddSegment0(&apiStruct, apiSize);	// this is send actual API length
      sendApi(MSG_TYPE_PPL_API, MSG_DEST_PPL_PCLEMULATOR, portId);
    }

#ifdef FEC_BP_TEST
    //memw(FEC_CNT_ADDR, 0);
    //memw(BP_CNT_ADDR, 0);
    //memw(BP_ADDR_PTR, BP_BASE_ADDR);
    //udelay(10000);
#else
    Sleep(10);
#endif //FEC_BP_TEST

  }

  // user test code ends here
  // -------------------------------------------
  if (ErrCountTotal == 0)
  {
    teePrintf("[test %s return state = PASSED]\n", testName.c_str());
    return SUCCESS;
  } else
  {
    teePrintf("[test %s return state = FAILED]\n", testName.c_str());
    return FAILURE;
  }
}
