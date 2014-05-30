// OmahaHiLo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "OmahaRanking.h"

int main(int argc, char* argv[])
{
    FILE* pFileIn;
	FILE* pFileOut;
	long  lSizeOfFileIn, lSavePosi, lCurrPosi;
	char* pInData;
	char  cOneLine[57];
	int i;
	char cHandA[12];
	char cHandB[12];
	char cBoard[15];
	char* pChar;
	CString Str;

	COmahaRanking OmahaRanking;

	pFileIn = fopen(argv[1], "r");
	//pFileIn  = fopen("E:\\May\\OmahaHiLo\\input.txt", "r");

	pFileOut = fopen(argv[2], "w");
	//pFileOut = fopen("E:\\May\\OmahaHiLo\\output.txt", "w");

	lSavePosi = ftell(pFileIn);
	fseek(pFileIn, 0L, SEEK_END);
    lSizeOfFileIn = ftell(pFileIn);
	fseek(pFileIn, lSavePosi, SEEK_SET);
	lCurrPosi = 0;

	pInData = new char[lSizeOfFileIn];

	while((lCurrPosi+57)<lSizeOfFileIn)
	{
    	fgets(cOneLine, 57, pFileIn);
		lSavePosi = ftell(pFileIn);
		lCurrPosi += 57;
		lCurrPosi += 1;
		fseek(pFileIn, lCurrPosi, SEEK_SET);
		lSavePosi = ftell(pFileIn);


		for(i=0; i<11; i++)
		{
			cHandA[i] = cOneLine[i+6];
			cHandB[i] = cOneLine[i+24];
		}
		cHandA[11] = '\0';
		cHandB[11] = '\0';

		for(i=0; i<14; i++)
			cBoard[i] = cOneLine[i+42];
		cBoard[14] = '\0';

		OmahaRanking.AnalyzeCard(cHandA, cHandB, cBoard);

		printf("HandA:");
		printf(cHandA);
		printf("  ");
		printf("HandB:");
		printf(cHandB);
		printf("  ");
		printf("Board:");
		printf(cBoard);
		printf("\r\n");

        fputs(cOneLine, pFileOut);
		fputs("\r\n", pFileOut);

		printf("=> ");
		Str = "=> ";

		if((OmahaRanking.iMaxRankWeightA>OmahaRanking.iMaxRankWeightB)
		  ||((OmahaRanking.iMaxRankWeightA==OmahaRanking.iMaxRankWeightB)&&(OmahaRanking.lMaxKickerWeightA>OmahaRanking.lMaxKickerWeightB)))
		{
			printf("HandA wins Hi ");
			Str += "HandA wins Hi ";
			pChar = &OmahaRanking.cMaxRankFlagA;
		}
		else
		{
			if((OmahaRanking.iMaxRankWeightA==OmahaRanking.iMaxRankWeightB)
			  &&(OmahaRanking.lMaxKickerWeightA==OmahaRanking.lMaxKickerWeightB))
			{
				printf("Split Pot Hi ");
				Str += "Split Pot Hi ";
				pChar = &OmahaRanking.cMaxRankFlagA;
			}
			else
			{
				printf("HandB wins Hi ");
				Str += "HandB wins Hi ";
				pChar = &OmahaRanking.cMaxRankFlagB;
			}
		}
		switch(*pChar)
		{
		case 0x00: // High card
			printf("(High Card);    ");
			Str += "(High Card);    ";
			break;
		case 0x01: // One pair
			printf("(One Pair);    ");
			Str += "(One Pair);    ";
			break; 
		case 0x02: // Two pairs
			printf("(Two Pairs);    ");
			Str += "(Two Pairs);    ";
			break;
		case 0x04: // 3-of-a-kind
			printf("(3-of-a-kind);    ");
			Str += "(3-of-a-kind);    ";
			break;
		case 0x08: // 4-of-a-kind
			printf("(4-of-a-kind);    ");
			Str += "(4-of-a-kind);    ";
			break;
		case 0x10: // Straight
			printf("(Straight);    ");
			Str += "(Straight);    ";
			break;
		case 0x20: // Flush
			printf("(Flush);    ");
			Str += "(Flush);    ";
			break;
		case 0x05: // Full house
			printf("(Full House);    ");
			Str += "(Full House);    ";
			break;
		case 0x30: // Straight flush
			printf("(Strsight Flush);    ");
			Str += "(Strsight Flush);    ";
			break;
		default:
			break;
		}

		if(((OmahaRanking.bMinLowHandFlagA==TRUE)&&(OmahaRanking.bMinLowHandFlagB==FALSE))
		  ||((OmahaRanking.bMinLowHandFlagA==TRUE)&&(OmahaRanking.bMinLowHandFlagB==TRUE)&&(OmahaRanking.lMinLowHandWeightA<OmahaRanking.lMinLowHandWeightB)))
		{
			printf("HandA wins Lo (");
			Str += "HandA wins Lo (";
			for(i=0; i<5; i++)
			{
				printf("%c", OmahaRanking.MinLowHandA.cArray[i][0]);
				Str += OmahaRanking.MinLowHandA.cArray[i][0];
			}
			printf(")\r\n");
			Str += ")\r\n";
		}
		else
		{
			if((OmahaRanking.bMinLowHandFlagA==FALSE)&&(OmahaRanking.bMinLowHandFlagB==FALSE))
			{
				printf("No hand qualified for Low\r\n");
				Str += "No hand qualified for Low\r\n";
			}
			else
			{
				if((OmahaRanking.bMinLowHandFlagA==TRUE)&&(OmahaRanking.bMinLowHandFlagB==TRUE)&&(OmahaRanking.lMinLowHandWeightA==OmahaRanking.lMinLowHandWeightB))
				{
					printf("Split Pot Lo (");
					Str += "Split Pot Lo (";
					for(i=0; i<5; i++)
					{
						printf("%c", OmahaRanking.MinLowHandA.cArray[i][0]);
						Str += OmahaRanking.MinLowHandA.cArray[i][0];
					}
					printf(")\r\n");
					Str += ")\r\n";
				}
				else
				{
					printf("HandB wins Lo (");
					Str += "HandB wins Lo (";
					for(i=0; i<5; i++)
					{
						printf("%c", OmahaRanking.MinLowHandB.cArray[i][0]);
						Str += OmahaRanking.MinLowHandB.cArray[i][0];
					}
					printf(")\r\n");
					Str += ")\r\n";
				}
			}
		}

		printf("\r\n");
		Str += "\r\n";

		fputs(Str, pFileOut);
	}

	fclose(pFileOut);
    fclose(pFileIn);
    delete []pInData;

	return 0;
}

