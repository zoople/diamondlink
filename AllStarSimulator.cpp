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
	 
	const int numTrials = 100000000;

	
	
	int diceRollRNGResult = -1;				// used to store the result from the RNG for the dice roll
	int rerollCheckRNGResult = -1;          // used to store the result from the RNG for checking rerolls
	int diceSelectRNGResult = -1; 			// used to store the result from the RNG for selecting the dice to use in a position
	int numGemsSelectRNGResult = -1;

	int numGems = 0; 				// the number of gems collected in the trial
	
	int numIdols = 0;			// the number of idols collected in the trial
	double totalPrize=0;

	int numRerolls = 0;
	
	
	int prizeTable[6] = 
	{
		0,
		0,
		1,
		2,
		5,
		10
	};
	
	const int IDOLRESULT = 0;
	const int RESPINRESULT = 1;
	
	int stats[15] = {0};
	
	int diceWeights[3][8] = {
						{6,6,3,2,1,0,0,0},
						{6,6,2,1,1,1,1,0},
						{6,6,2,1,1,1,1,0}
						};
	
	int reRollWeights[3][4] = {
		                {2,10,4,6},
		                {2,10,9,1},
						{2,10,5,5}
						
						};
						
	//Weights that detremine how many Gems are awarded	
	int gemAwardWeights[16][32] =	
	{	
{30,1000000,1000000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{30,1000000,200000,200000,200000,200000,200000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{30,1000000,0,110000,110000,110000,110000,110000,110000,110000,110000,120000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{30,1000000,0,0,125000,130000,125000,125000,125000,125000,125000,20000,20000,20000,20000,20000,20000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{30,1000000,0,0,0,149996,149996,149996,149996,149996,150000,10000,10000,10000,10000,10000,10000,10000,10000,10000,10000,20,0,0,0,0,0,0,0,0,0,0},
{30,1000000,0,0,0,0,200000,200000,200000,200000,149898,5000,5000,5000,5000,5000,5000,5000,5000,5000,5000,20,20,20,20,20,2,0,0,0,0,0},
{30,1000000,0,0,0,0,0,212000,212000,212000,213800,15000,15000,15000,15000,15000,15000,15000,15000,15000,15000,20,20,20,20,20,20,20,20,20,19,1},
{30,1000000,0,0,0,0,0,0,213900,213000,213000,35000,35000,35000,35000,35000,35000,35000,35000,35000,35000,2000,2000,2000,2000,2000,20,20,20,20,18,2},
{30,1000000,0,0,0,0,0,0,0,154495,154495,64000,64000,64000,64000,64000,64000,64000,64000,64000,64000,10000,10000,10000,10000,10000,200,200,200,200,200,10},
{30,1000000,0,0,0,0,0,0,0,0,4990,84000,84000,84000,84000,84000,84000,84000,84000,84000,84000,30000,30000,30000,30000,30000,1000,1000,1000,1000,1000,10},
{30,1000000,0,0,0,0,0,0,0,0,0,79000,79000,79000,79000,79000,79000,79000,79000,79000,78900,40000,40000,40000,40000,40000,2000,2000,2000,2000,2000,100},
{30,1000000,0,0,0,0,0,0,0,0,0,0,72500,72500,72900,72000,72000,72000,72000,72000,72000,60000,60000,60000,60000,60000,10000,10000,10000,10000,10000,100},
{30,1000000,0,0,0,0,0,0,0,0,0,0,0,57000,56900,56000,56000,56000,56000,56000,56000,80000,80000,80000,80000,80000,30000,30000,30000,30000,30000,100},
{30,1000000,0,0,0,0,0,0,0,0,0,0,0,0,33000,33000,33000,33000,33000,33000,32000,70000,70000,70000,70000,70000,80000,80000,80000,80000,80000,20000},
{30,1000000,0,0,0,0,0,0,0,0,0,0,0,0,0,40000,45000,45000,45000,45000,40000,16000,16000,16000,16000,16000,100000,100000,100000,100000,100000,160000},
{30,1000000,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1000000},
	};	
		
	//Weights to detremine the dice	
	int diceSelectionWeights[15][5] =	
	{	
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},
{3,3,1,1,1},



	};	

	
	int jackpotCollectionTiers[4] = {10,20,25,30};
	int jackpotAmounts[4] = {20,100,1000,18000};
	
	for (int i=0; i<numTrials; i++) {
		//Print the status
		if (i%(numTrials/100) ==0) printf("%f\n", (double)i/numTrials);

		//Reset the collection each trial
		numGems = 0;
		numIdols = 0;
		
		//For each position
		for (int reelPos=0; reelPos<15; reelPos++)
		{
			
			rerollCheckRNGResult = -1;
			
			//Select which dice to use
			diceSelectRNGResult = weightedDice(diceSelectionWeights[reelPos]);
		//	diceSelectRNGResult = 0;

			//Roll the dice
			diceRollRNGResult =  weightedDice(diceWeights[diceSelectRNGResult]);
			//printf("Roll %i", roll);
			
			//Check for a reroll
			if (diceRollRNGResult == RESPINRESULT) {
				//Each dice has a different reroll weight, check agianst that
				rerollCheckRNGResult = weightedDice(reRollWeights[diceSelectRNGResult]);
				//printf("...ReRoll check %i", checkReroll);
				//rerollCheckRNGResult = 0;
			}
			
				while (rerollCheckRNGResult == 0 && diceRollRNGResult == RESPINRESULT )
				{
					numRerolls--;
					diceRollRNGResult =  weightedDice(diceWeights[diceSelectRNGResult]);
					//printf("...Roll");	
					if (diceRollRNGResult == RESPINRESULT)
					{					
						rerollCheckRNGResult =  weightedDice(reRollWeights[diceSelectRNGResult]);
						//printf("...ReRoll check %i", checkReroll);
					}
					//rerollCheckRNGResult = 0;
				}
			
			totalPrize+= prizeTable[diceRollRNGResult];
			if (diceRollRNGResult == IDOLRESULT){
				numIdols++;
				//printf("Gems: %i\n", numGems);
			}
		}

		//stats[numIdols]++;
		

		numGems = weightedDice(gemAwardWeights[numIdols]) + 1;
		//if (numGems >= jackpotCollectionTiers[3]) printf("[%i]>(%i)\n", numGems,jackpotCollectionTiers[3]);

		
		if (numGems >= jackpotCollectionTiers[3]) totalPrize+=jackpotAmounts[3];
		else if (numGems >= jackpotCollectionTiers[2]) totalPrize+=jackpotAmounts[2];
		else if (numGems >= jackpotCollectionTiers[1]) totalPrize+=jackpotAmounts[1];
		else if (numGems >= jackpotCollectionTiers[0]) totalPrize+=jackpotAmounts[0];
	
		if (numGems >= jackpotCollectionTiers[3]) stats[0]++;
		
				
	}
	 printf("Prizes: %f\n", totalPrize/numTrials);
	 
	 for (int i=0; i<=15; i++) printf("%i : %f\n", i, (double)stats[i]/(double)numTrials);

	 	 time_t end = time(0); 
		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		
}