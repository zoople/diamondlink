#include <stdio.h>
#include <random>
#include <time.h>

using namespace std;

	std::mt19937_64 generator (int(time(0)));
	std::uniform_real_distribution<double> dis(1.0, 7.0);
	std::uniform_real_distribution<double> reroll(1.0, 11.0);
	std::uniform_real_distribution<double> gemSelect(1.0, 6.0);
	
	int weightedDice(int* dist)
	{
		int pick;

		std::uniform_real_distribution<double> temp(0.0, dist[1]);
		
		int dice = temp(generator);	
		int checkpoint=0;

		for (pick = 2; pick<=(dist[0]+1); pick++)
		{
				checkpoint += dist[pick];
				if (dice < checkpoint) break;
		}
		return pick-2;
	}

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
		int result = (int) dis(generator);
		return result;
	}
	

	/*
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

	double total=0;
	double idolHits = 0;
	int roll = 0;
	int prize = 0;
	int rerolls = 0;
	int checkReroll = 0;
	int numTrials = 10000000;
	int numGems = 0;
	
	int prizeTable[7] = 
	{
		0,  //buffer
		0,
		0,
		50,
		100,
		200,
		500
	};
	
	int diceTest[8] = {6,6,1,1,1,1,1,1};
	int reRoll[2][4] = {
		                {2,10,1,9},
		                {2,10,9,1}
						};
	int whichDice[4] = {2,2,1,1};
	int diceSelect = -1;
	
	for (int i=0; i<numTrials; i++) {
		numGems = 0;
		for (int j=0; j<15; j++)
		{
			diceSelect = weightedDice(whichDice);
			rerolls = 10;
			checkReroll = 0;
			
			roll =  weightedDice(diceTest)+1;
			//printf("Roll %i", roll);
		
			
			if (roll == 2) {
				checkReroll = weightedDice(reRoll[diceSelect]);
				//printf("...ReRoll check %i", checkReroll);
				//checkReroll = 0;
				
			}
			
				while (checkReroll == 0 && roll == 2 && rerolls >0)
				{
					rerolls--;
					roll =  weightedDice(diceTest)+1;
					//printf("...Roll %i", roll);	
					if (roll == 2)
					{					
						checkReroll =  weightedDice(reRoll[diceSelect]);
						//printf("...ReRoll check %i", checkReroll);
					}
				}
			
			total+= prizeTable[roll];
			if (roll == 1){
				numGems += gemSelect(generator);
				//printf("Gems: %i\n", numGems);
				
			}
		}
		idolHits+= numGems;
				
	}
	 printf("%f\n", total/numTrials);
	 printf("%f\n", idolHits/numTrials);
		
}