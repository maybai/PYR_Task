#include "afxwin.h"

union PokerHand
{
	char cCard[15];
    char cArray[5][3];
};

class COmahaRanking: public CObject
{
private:
	int   iPosiPair0, iPosiPair1;
	int   iPosi3OfKind;
	int   iPosi4OfKind;

	BOOL  bLowHandFlag; // =TRUE Low Hand is available; = FALSE Low Hand is not available.
	char  cRankFlag;
	int   iRankWeight;
	long  lKickerWeight, lLowHandWeight;
	union PokerHand CurrHand;
	union PokerHand HighHand;
	union PokerHand LowHand;

	char  CurrHandA[12];
	char  CurrHandB[12];
	char  CurrHandPublic[15];
	char  CombinedHandA[15];
	char  CombinedHandB[15];

public:
	union PokerHand MaxHighhandA;
	char  cMaxRankFlagA;
	int   iMaxRankWeightA;
	long  lMaxKickerWeightA;
	union PokerHand MinLowHandA;
	long  lMinLowHandWeightA;
	BOOL  bMinLowHandFlagA;

	union PokerHand MaxHighhandB;
	char  cMaxRankFlagB;
	int   iMaxRankWeightB;
	long  lMaxKickerWeightB;
	union PokerHand MinLowHandB;
	long  lMinLowHandWeightB;
	BOOL  bMinLowHandFlagB;

private:
	BOOL RankHand(char* pHand, int uiLen);
	long MyPow(int x, int y);

public:
	BOOL AnalyzeCard(char* pHandA, char* pHandB, char* pHandPublic);
};
