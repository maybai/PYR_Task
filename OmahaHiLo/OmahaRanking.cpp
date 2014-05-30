// HandsComparison.cpp | Author: May Bai | Date: May 29th, 2014

#include "stdafx.h"
#include "OmahaRanking.h"

/////////////////////////////////////////////////////////////////////////////
BOOL COmahaRanking::RankHand(char* pHand, int uiLen)
{
	int i, j, k, iCount, iCountOfA, iPosiOfA;
	char cTempCard, cOldCard, cOldFlush;
	char cFlushFlag;
	int iStraightCount;

    if(uiLen!=15)
		return FALSE;

	// Copy the original hand to local Current Hand and change the characters of kind to numbers
	for(i=0; i<15; i++)
	{
		switch(*(pHand+i))
		{
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			CurrHand.cCard[i] = *(pHand+i) - 48;
			break;
		case 'T':
			CurrHand.cCard[i] = 10;
			break;
		case 'J':
			CurrHand.cCard[i] = 11;
			break;
		case 'Q':
			CurrHand.cCard[i] = 12;
			break;
		case 'K':
			CurrHand.cCard[i] = 13;
			break;
		case 'A':
			CurrHand.cCard[i] = 14;
			break;
		default:
			CurrHand.cCard[i] = *(pHand+i);
			break;
		}
	}

	// Bubble sort the array of Current Hand
    for(i=0; i<4; i++)
	{
		for(j=i+1; j<5; j++)
		{
			if(CurrHand.cArray[i][0]<CurrHand.cArray[j][0])
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[i][k];
					CurrHand.cArray[i][k] = CurrHand.cArray[j][k];
					CurrHand.cArray[j][k] = cTempCard;
					k++;
				}
			}
		}
	}

	// Record all combination of Rank in Current Hand
	iCount = 0;
	iRankWeight = 0;
	cRankFlag = 0;
	iStraightCount = 0;
	cOldCard = CurrHand.cArray[0][0];
	for(i=1; i<5; i++)
	{
		// Looking for Straight
		if((CurrHand.cArray[i][0]+1)==cOldCard)
		    iStraightCount++;

        // Looking for Pairs
		if(cOldCard==CurrHand.cArray[i][0])
		{
			iCount++;
			if(i==4)
			{
				switch(iCount)
				{
				case 0:
					break;
				case 1: // pair
					if(cRankFlag&0x01) // two pairs
					{
						cRankFlag &= 0xfe;
						cRankFlag |= 0x02;
						iRankWeight = iRankWeight + 4 - 3;
						iPosiPair1 = i - iCount;
					}
					else //One pair
					{
						cRankFlag |= 0x01;
						iRankWeight = iRankWeight + 3;
						iPosiPair0 = i - iCount;
					}
					break;
				case 2: // Three of a kind
					cRankFlag |= 0x04;
					iRankWeight = iRankWeight + 5;
					iPosi3OfKind = i - iCount;
					break;
				case 3: // Four of a kind
					cRankFlag |= 0x08;
					iRankWeight = iRankWeight + 9;
					iPosi4OfKind = i - iCount;
					break;
				default:
					break;
				}
			}
		}
        else
		{
			switch(iCount)
			{
			case 0:
				break;
			case 1: // pair
				if(cRankFlag&0x01) // two pairs
				{
					cRankFlag &= 0xfe;
					cRankFlag |= 0x02;
					iRankWeight = iRankWeight + 4 - 3;
				    iPosiPair1 = i - iCount - 1;
				}
				else //One pair
				{
					cRankFlag |= 0x01;
					iRankWeight = iRankWeight + 3;
				    iPosiPair0 = i - iCount - 1;
				}
				break;
			case 2: // Three of a kind
				cRankFlag |= 0x04;
				iRankWeight = iRankWeight + 5;
				iPosi3OfKind = i - iCount -1;
				break;
			case 3: // Four of a kind
				cRankFlag |= 0x08;
				iRankWeight = iRankWeight + 9;
				iPosi4OfKind = i - iCount - 1;
				break;
			default:
				break;
			}
			iCount = 0;
		}
		cOldCard = CurrHand.cArray[i][0];
	}

	// Is this hand a Stright?
	if(iStraightCount==4)
	{
        cRankFlag |= 0x10;
		iRankWeight += 6;
	}
	else
	{
		// Is this hand the Stright "5, 4, 3, 2, A"?
		if( (CurrHand.cArray[0][0]==14)
	      &&(CurrHand.cArray[1][0]==5)
		  &&(CurrHand.cArray[2][0]==4)
		  &&(CurrHand.cArray[3][0]==3)
		  &&(CurrHand.cArray[4][0]==2))
		{
			// Move A to the end of array
			for(i=0; i<4; i++)
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[i][k];
					CurrHand.cArray[i][k] = CurrHand.cArray[i+1][k];
					CurrHand.cArray[i+1][k] = cTempCard;
					k++;
				}
			}

			cRankFlag |= 0x10;
			iRankWeight += 6;
		}
	}

	// Is this hand a Flush?
	cFlushFlag = TRUE;
	cOldFlush = CurrHand.cArray[0][1];
	for(i=1; i<5; i++)
	{
		if(CurrHand.cArray[i][1]!=cOldFlush)
		{
			cFlushFlag = FALSE;
			break;
		}
		cOldFlush = CurrHand.cArray[i][1];
	}
	if(cFlushFlag)
	{
        cRankFlag |= 0x20;
		iRankWeight += 7;
	}

    // Move the combination of Rank to beginning position
	switch(cRankFlag&0x0f)
	{
	case 0x08: // Four of a kind
		while(iPosi4OfKind)
		{
			for(i=0; i<4; i++)
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[iPosi4OfKind+i-1][k];
					CurrHand.cArray[iPosi4OfKind+i-1][k] = CurrHand.cArray[iPosi4OfKind+i][k];
					CurrHand.cArray[iPosi4OfKind+i][k] = cTempCard;
					k++;
				}
			}
			iPosi4OfKind--;
		}
		break;
	case 0x05: // Full house
	case 0x04: // Three of a kind
		while(iPosi3OfKind)
		{
			for(i=0; i<3; i++)
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[iPosi3OfKind+i-1][k];
					CurrHand.cArray[iPosi3OfKind+i-1][k] = CurrHand.cArray[iPosi3OfKind+i][k];
					CurrHand.cArray[iPosi3OfKind+i][k] = cTempCard;
					k++;
				}
			}
			iPosi3OfKind--;
		}
		break;
	case 0x02: // Two pairs
		while(iPosiPair0) // Move the first pair forward
		{
			for(i=0; i<2; i++)
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[iPosiPair0+i-1][k];
					CurrHand.cArray[iPosiPair0+i-1][k] = CurrHand.cArray[iPosiPair0+i][k];
					CurrHand.cArray[iPosiPair0+i][k] = cTempCard;
					k++;
				}
			}
			iPosiPair0--;
		}
		while(iPosiPair1-2) // Move the second pair forward
		{
			for(i=0; i<2; i++)
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[iPosiPair1+i-1][k];
					CurrHand.cArray[iPosiPair1+i-1][k] = CurrHand.cArray[iPosiPair1+i][k];
					CurrHand.cArray[iPosiPair1+i][k] = cTempCard;
					k++;
				}
			}
			iPosiPair1--;
		}
		break;
	case 0x01: // One pair
		while(iPosiPair0) // Move the first pair forward
		{
			for(i=0; i<2; i++)
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[iPosiPair0+i-1][k];
					CurrHand.cArray[iPosiPair0+i-1][k] = CurrHand.cArray[iPosiPair0+i][k];
					CurrHand.cArray[iPosiPair0+i][k] = cTempCard;
					k++;
				}
			}
			iPosiPair0--;
		}
		break;
	default:
		break;
	}

    // Count the weight of kicker
	lKickerWeight = 0;
	for(i=0; i<5; i++)
	{
		lKickerWeight += CurrHand.cArray[i][0]*(MyPow(15,(4-i)));
	}

	// Copy currunt hand to High Hand
    for(i=0; i<15; i++)
	{
		switch(CurrHand.cCard[i])
		{
		case 1:
			HighHand.cCard[i] = 'A';
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			HighHand.cCard[i] = CurrHand.cCard[i] + 48;
			break;
		case 10:
			HighHand.cCard[i] = 'T';
			break;
		case 11:
			HighHand.cCard[i] = 'J';
			break;
		case 12:
			HighHand.cCard[i] = 'Q';
			break;
		case 13:
			HighHand.cCard[i] = 'K';
			break;
		case 14:
			HighHand.cCard[i] = 'A';
			break;
		case '\0':
		case '-':
			HighHand.cCard[i] = '-';
			break;
		default:
			HighHand.cCard[i] = CurrHand.cCard[i];
			break;
		}
		HighHand.cCard[14] = '\0';
	}

    // Analyze low hand
    bLowHandFlag = FALSE;
    lLowHandWeight = 0;

	if((cRankFlag&0x0f)==0)	// Whether is there no any pairs in this hand?
	{

		// Is there letter 'A' in this hand?
		iCountOfA = 0;
		iPosiOfA = 0;
		for(i=0; i<5; i++)
		{
			if((CurrHand.cArray[i][0]==1)||(CurrHand.cArray[i][0]==14))
			{
				iCountOfA++;
				iPosiOfA = i;
			}
		}

		if(iCountOfA==1)
		{
			CurrHand.cArray[iPosiOfA][0] = 1;

			// Move 'A' backward to the end of array
			for(i=iPosiOfA; i<4; i++)
			{
				k = 0;
				while(k<3)
				{
					cTempCard = CurrHand.cArray[i+1][k];
					CurrHand.cArray[i+1][k] = CurrHand.cArray[i][k];
					CurrHand.cArray[i][k] = cTempCard;
					k++;
				}
			}
		}

		if(CurrHand.cArray[0][0]<=8) // Whether all cards are lower than or equal to 8
		{
			// Calculate the weight of Low Hand
			for(i=0; i<5; i++)
			{
				lLowHandWeight += CurrHand.cArray[i][0]*(MyPow(15, (4-i)));
			}
			bLowHandFlag = TRUE;
		}
	}

	// Copy Currunt Hand to Low Hand
    if(bLowHandFlag)
	{
		for(i=0; i<15; i++)
		{
			switch(CurrHand.cCard[i])
			{
			case 1:
				LowHand.cCard[i] = 'A';
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				LowHand.cCard[i] = CurrHand.cCard[i] + 48;
				break;
			case 10:
				LowHand.cCard[i] = 'T';
				break;
			case 11:
				LowHand.cCard[i] = 'J';
				break;
			case 12:
				LowHand.cCard[i] = 'Q';
				break;
			case 13:
				LowHand.cCard[i] = 'K';
				break;
			case 14:
				LowHand.cCard[i] = 'A';
				break;
			case '\0':
			case '-':
				LowHand.cCard[i] = '-';
				break;
			default:
				LowHand.cCard[i] = CurrHand.cCard[i];
				break;
			}
			LowHand.cCard[14] = '\0';
		}
	}

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
BOOL COmahaRanking::AnalyzeCard(char* pHandA, char* pHandB, char* pHandPublic)
{
	int i, j, x, y, z, k;
	int iCount;

	// Copy original hands to internal hands
	for(i=0; i<12; i++)
	{
		CurrHandA[i] = *(pHandA+i);
		CurrHandB[i] = *(pHandB+i);
	}

	for(i=0; i<15; i++)
		CurrHandPublic[i] = *(pHandPublic+i);

    // Combine original hands to form a new hand and analyze it.
	iCount = 0;
	for(i=0;i<3; i++)
	{
		for(j=i+1; j<4; j++)
		{
			for(x=0; x<3; x++)
			{
				for(y=x+1; y<4; y++)
				{
					for(z=y+1; z<5; z++)
					{
						// Combine cards to form two new hands
						k = 0;
						while(k<3)
						{
                            CombinedHandA[k] = CurrHandA[i*3+k];
							CombinedHandB[k] = CurrHandB[i*3+k];
							k++;
						}
						k = 0;
						while(k<3)
						{
                            CombinedHandA[k+3] = CurrHandA[j*3+k];
							CombinedHandB[k+3] = CurrHandB[j*3+k];
							k++;
						}
						k = 0;
						while(k<3)
						{
                            CombinedHandA[k+6] = CurrHandPublic[x*3+k];
							CombinedHandB[k+6] = CurrHandPublic[x*3+k];
							k++;
						}
						k = 0;
						while(k<3)
						{
                            CombinedHandA[k+9] = CurrHandPublic[y*3+k];
							CombinedHandB[k+9] = CurrHandPublic[y*3+k];
							k++;
						}
						k = 0;
						while(k<3)
						{
                            CombinedHandA[k+12] = CurrHandPublic[z*3+k];
							CombinedHandB[k+12] = CurrHandPublic[z*3+k];
							k++;
						}
                        
						// Analyze HandA
                        RankHand(CombinedHandA, 15);
						if(iCount==0)
						{
							k = 0;
							while(k<15)
							{
							    MaxHighhandA.cCard[k] = HighHand.cCard[k];
								k++;
							}
							cMaxRankFlagA = cRankFlag;
							iMaxRankWeightA = iRankWeight;
                            lMaxKickerWeightA = lKickerWeight;

							k = 0;
							while(k<15)
							{
							    MinLowHandA.cCard[k] = LowHand.cCard[k];
								k++;
							}
							lMinLowHandWeightA = lLowHandWeight;
							bMinLowHandFlagA = bLowHandFlag;	
						}
						else
						{
							if((iMaxRankWeightA<iRankWeight)
							  ||((iMaxRankWeightA==iRankWeight)&&(lMaxKickerWeightA<lKickerWeight)))
							{
								k = 0;
								while(k<15)
								{
									MaxHighhandA.cCard[k] = HighHand.cCard[k];
									k++;
								}
								cMaxRankFlagA = cRankFlag;
								iMaxRankWeightA = iRankWeight;
								lMaxKickerWeightA = lKickerWeight;
							}

							if(((bLowHandFlag==TRUE)&&(bMinLowHandFlagA==FALSE))
							  ||((bLowHandFlag==TRUE)&&(bMinLowHandFlagA==TRUE)&&(lMinLowHandWeightA>lLowHandWeight)))
							{
								k = 0;
								while(k<15)
								{
									MinLowHandA.cCard[k] = LowHand.cCard[k];
									k++;
								}
								lMinLowHandWeightA = lLowHandWeight;
								bMinLowHandFlagA = bLowHandFlag;
							}
						}

						// Analyze HandB
                        RankHand(CombinedHandB, 15);
						if(iCount==0)
						{
							k = 0;
							while(k<15)
							{
							    MaxHighhandB.cCard[k] = HighHand.cCard[k];
								k++;
							}
							cMaxRankFlagB = cRankFlag;
							iMaxRankWeightB = iRankWeight;
                            lMaxKickerWeightB = lKickerWeight;

							k = 0;
							while(k<15)
							{
							    MinLowHandB.cCard[k] = LowHand.cCard[k];
								k++;
							}
							lMinLowHandWeightB = lLowHandWeight;
							bMinLowHandFlagB = bLowHandFlag;	
						}
						else
						{
							if((iMaxRankWeightB<iRankWeight)
							  ||((iMaxRankWeightB==iRankWeight)&&(lMaxKickerWeightB<lKickerWeight)))
							{
								k = 0;
								while(k<15)
								{
									MaxHighhandB.cCard[k] = HighHand.cCard[k];
									k++;
								}
								cMaxRankFlagB = cRankFlag;
								iMaxRankWeightB = iRankWeight;
								lMaxKickerWeightB = lKickerWeight;
							}

							if(((bLowHandFlag==TRUE)&&(bMinLowHandFlagB==FALSE))
							  ||((bLowHandFlag==TRUE)&&(bMinLowHandFlagB==TRUE)&&(lMinLowHandWeightB>lLowHandWeight)))
							{
								k = 0;
								while(k<15)
								{
									MinLowHandB.cCard[k] = LowHand.cCard[k];
									k++;
								}
								lMinLowHandWeightB = lLowHandWeight;
								bMinLowHandFlagB = bLowHandFlag;
							}
						}

                        iCount++;
					}
				}
			}
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
long COmahaRanking::MyPow(int x, int y)
{
	long lResult = 1;
    int i;

	for(i=0; i<y; i++)
		lResult *= x;

	return lResult;
}
