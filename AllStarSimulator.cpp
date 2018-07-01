#include <stdio.h>
#include <random>
#include <time.h>
#include "randomc.h"
#include "mersenne.cpp"

using namespace std;

	//Set up the RNG
	int seed = (int)time(0); 
	CRandomMersenne RanGen(seed); 
	

	/*  WEIGHTED DICE 
		weightedDice(int* dist)

		Get a random number from a weighted distribution

	 
 		dist: the weighted distribution - note: dist[0] must be the number of items
		                                        dist[1] must be the total of the weightings

 		eg: weightedDice({3,100,20,60,20}) says, there are three items, that add up to 100. They are 20, 60 and 20

		return: int - indicated the index between [ 0, dist[1] ]
	*/
	int weightedDice(int* dist)
	{
		int pick;

		int dice = 	RanGen.IRandom(0, dist[1]-1);
		
		int checkpoint=0;

		for (pick = 2; pick<=(dist[0]+1); pick++)
		{
				checkpoint += dist[pick];
				if (dice < checkpoint) break;
		}
		return pick-2;
	}

	//Machine object
	class EGM
	{
		public:
			double moneyin=0;
			double moneyout=0;
			double numSpins = 0;
			double balance=0;
	};
	
	int gameOutcome(int bet)
	{
		//int result = (int) dis(generator);
		return -1;
	}
	

	/* PLAY GAME
		Plays a game, ie it gets a game outcome and modifies the machine details
		Assumes the player has the sufficient balance to play the game at the required bet

		Returns -1 for all errors
	*/
	int playGame(EGM &machine, int bet)
	{
		machine.balance-=bet;
		machine.moneyin+=bet;
		machine.numSpins++;
		
		int result = gameOutcome(bet);
		
		machine.moneyout+= result;
		machine.balance+=result;
		printf("SPIN %f - BALANCE: %f OUTCOME %i\n",  machine.numSpins, machine.balance, result);
	}
	
	int playJackpotGame(EGM &machine, EGM &jackpot, int bet)
	{
		playGame(machine, bet);
		jackpot.balance += 1;
	}
	
	/* plays a machine untill it has no credits left and retuns the number of spins it made */
	double burnDown(EGM &machine, int bet)
	{
		while(machine.balance >= bet)
		{
			playGame(machine, bet);
		}
		
		return machine.numSpins;
	}
	
int burndownrunner()
{
	
	EGM machine[5];
	int betAmount = 3;
	double burn = -1;
	
	double stats[1000]={0};
	
	int numTrials = 10001;
	int medianPos = numTrials/2.0 + 0.5;
	printf("%i\n", medianPos);
	int runningTotal =0;
	int median = -1;
	
	for (int i=0; i<numTrials; i++)
	{
		machine[0].balance = 10;
		machine[0].numSpins = 0;
		burn = burnDown(machine[0], betAmount);
		//printf("%i \n", (int)burn);
		stats[(int)burn]++;
	
		//printf("NEW RUN\n");
	}
	
	printf("SIMS DONE\n");
	for (int i=0; i<1000; i++)
	{
		if (stats[i]>0) printf("%i: %f\n", i, stats[i]); 
			runningTotal+=(int)stats[i];
		if (runningTotal >= medianPos && median == -1) median = i;
		
	}
	printf("MEDIAN: %i\n", median); 
}

int main()
{
	 clock_t tStart = clock();
	 
	const int numTrials = 1000000;

	
	
	
	int diceRollRNGResult = 0;				// used to store the result from the RNG for the dice roll
	int rerollCheckRNGResult = -1;          // used to store the result from the RNG for checking rerolls
	int diceSelectRNGResult = -1; 			// used to store the result from the RNG for selecting the dice to use in a position
	
	double numGems = 0; 				// the number of gems collected in the trial
	
	double numIdols = 0;			// the number of ideols collected in the trial
	double totalPrize=0;

	int numRerolls = 0;
	
	
	int prizeTable[6] = 
	{
		0,
		0,
		5,
		10,
		20,
		50
	};
	
	const int IDOLRESULT = 0;
	const int RESPINRESULT = 1;
	
	
	int diceWeights[3][8] = {
						{6,6,2,3,1,0,0,0},
						{6,6,1,1,1,1,1,1},
						{6,6,1,1,1,1,1,1}
						};
	
	int reRollWeights[3][4] = {
		                {2,10,1,9},
		                {2,10,9,1},
						{2,10,5,5}
						
						};
						
	int gemWeights[3][7] = {
							{5,7,1,2,2,1,1},
							{5,5,1,1,1,1,1},
							{5,7,1,1,2,2,1},
							
						};		
	int gemValue[5] = {1,2,3,4,5};
	
	int whichDice[5] = {3,3,1,1,1};
	
	int jackpotCollectionTiers[4] = {10,20,25,35};
	int jackpotAmounts[4] = {20,150,1000,18000};
	
	for (int i=0; i<numTrials; i++) {
		if (i%(numTrials/100) ==0) printf("%f\n", (double)i/numTrials);
		numGems = 0;
		numIdols = 0;
		
		for (int j=0; j<15; j++)
		{
			diceSelectRNGResult = weightedDice(whichDice);
			numRerolls = 100;
			rerollCheckRNGResult = 0;
			
			diceRollRNGResult =  weightedDice(diceWeights[diceSelectRNGResult]);
			//printf("Roll %i", roll);
		
			
			if (diceRollRNGResult == RESPINRESULT) {
				rerollCheckRNGResult = weightedDice(reRollWeights[diceSelectRNGResult]);
				//printf("...ReRoll check %i", checkReroll);
				//checkReroll = 0;
				
			}
			
				while (rerollCheckRNGResult == 0 && diceRollRNGResult == RESPINRESULT && numRerolls >0)
				{
					numRerolls--;
					diceRollRNGResult =  weightedDice(diceWeights[diceSelectRNGResult]);
					//printf("...Roll");	
					if (diceRollRNGResult == RESPINRESULT)
					{					
						rerollCheckRNGResult =  weightedDice(reRollWeights[diceSelectRNGResult]);
						//printf("...ReRoll check %i", checkReroll);
					}
				}
			
			totalPrize+= prizeTable[diceRollRNGResult];
			if (diceRollRNGResult == IDOLRESULT){
				numGems += gemValue[weightedDice(gemWeights[diceSelectRNGResult])] ;
				//printf("Gems: %i\n", numGems);
				
			}
		}
		
		
		if (numGems >= jackpotCollectionTiers[3]) totalPrize+=jackpotAmounts[3];
		else if (numGems >= jackpotCollectionTiers[2]) totalPrize+=jackpotAmounts[2];
		else if (numGems >= jackpotCollectionTiers[1]) totalPrize+=jackpotAmounts[1];
		else if (numGems >= jackpotCollectionTiers[0]) totalPrize+=jackpotAmounts[0];
		
		
				
	}
	 printf("Prizes: %f\n", totalPrize/numTrials);
	 printf("Gems %f\n", numGems/numTrials);
	 
	 	 time_t end = time(0); 
		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		
}