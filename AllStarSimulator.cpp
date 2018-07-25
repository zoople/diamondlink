#include <stdio.h>
#include <random>
#include <time.h>
#include "randomc.h"
#include "mersenne.cpp"
 #include <thread>
 #include "prizes.h"

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
	
	int numTrials = 1000;
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



void diamondLinkFeature(double* stats)
{

	//dice results
	int diceRollRNGResult = -1;				// used to store the result from the RNG for the dice roll
	int rerollCheckRNGResult = -1;          // used to store the result from the RNG for checking rerolls
	int diceSelectRNGResult = -1; 			// used to store the result from the RNG for selecting the dice to use in a position
	int numGemsSelectRNGResult = -1;

	
	double totalPrize=0;
	double jackpotPrize = 0;
	
	

		//Reset the collection each trial
		int numGems = 0;
		int numIdols = 0;
		int numRerolls = 0;
		
		//For each position
		for (int reelPos=0; reelPos<15; reelPos++)
		{
			numRerolls = 0;
			
			rerollCheckRNGResult = -1;
			
			//Select which dice to use
			diceSelectRNGResult = weightedDice(diceSelectionWeights[reelPos]);
			//diceSelectRNGResult = 1;

			//Roll the dice
			diceRollRNGResult =  weightedDice(diceWeights[diceSelectRNGResult]);
			//printf("Roll %i ", diceRollRNGResult);
			
			
				while (diceRollRNGResult == RESPINRESULT )
				{
					numRerolls++;
					diceRollRNGResult =  weightedDice(diceWeights[diceSelectRNGResult]);
					//printf(", %i ", diceRollRNGResult);
				}
			//printf("\n");

				if (numRerolls>19) numRerolls = 19;
			
			//printf("The prize is %i\n", prizeTable[diceRollRNGResult]);
			//printf("The prize actually is %i\n", prizeTable[diceRollRNGResult]*(numRerolls+1));


			if (numRerolls >0) totalPrize+= prizeTable[diceRollRNGResult]*(numRerolls+1);
			else totalPrize+= prizeTable[diceRollRNGResult];

			
	
			
			if (diceRollRNGResult == IDOLRESULT){
				numIdols++;
				//printf("Gems: %i\n", numGems);
			}
		}

		//stats[numIdols]++;
		

		numGems = weightedDice(gemAwardWeights[numIdols]) + 1;
		//if (numGems >= jackpotCollectionTiers[3]) printf("[%i]>(%i)\n", numGems,jackpotCollectionTiers[3]);
		//numGems = 0;
		
		if (numGems >= jackpotCollectionTiers[3]) 	   {jackpotPrize+=jackpotAmounts[3]; }//stats[3]++;}
		else if (numGems >= jackpotCollectionTiers[2]) {jackpotPrize+=jackpotAmounts[2]; }//stats[2]++;}
		else if (numGems >= jackpotCollectionTiers[1]) {jackpotPrize+=jackpotAmounts[1];}//stats[1]++;}
		else if (numGems >= jackpotCollectionTiers[0]) {jackpotPrize+=jackpotAmounts[0];}//stats[0]++;}
	
		stats[0] += totalPrize;
		stats[1] +=jackpotPrize;
		stats[2]++;
	
}


int main()
{
	clock_t tStart = clock();
	 
	const int numTrials = 10000000;

	
	
	double stats[15] = {0};
	

	

	int num_threads = 2;

	std::thread th[num_threads];
		 
	
	

	for (int i=0; i<numTrials; i++) {

	 if (i % (numTrials/100) == 0) printf("[%i]\n",i);

		
			// for (int t=0; t<num_threads;t++)
		// {
         //Launch a thread
			//th[t] = thread( diamondLinkFeature, stats);
			diamondLinkFeature(stats);			
		// }
		 
        //Join the threads with the main thread
        //for (int t = 0; t < num_threads; ++t) {
             //th[t].join();
        // }
		 

	}
	 printf("Prizes: %f\n", stats[0]/numTrials);
	 printf("Jackpots: %f\n", stats[1]/numTrials);
	 printf("I got called %f times\n", stats[2]);
	 
	 
	// for (int i=0; i<=15; i++) printf("%i : %f\n", i, (double)stats[i]/(double)numTrials);

	 	 time_t end = time(0); 
		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		
}