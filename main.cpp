/* Written by Andrew Doyle */

#include <vector>
#include <iostream>
#include <random>
#include <ctime>

int main(){
  std::srand(std::time(0)); //Seed our random number generator with the time of day
  int maxWeight = 30; //Can carry this ammount of weight
  int numItems = 6; //This many items to consider
  
  std::vector<int> weights(numItems, 0); //Weight of each item
  std::vector<int> values(numItems, 0); //Value of each item
  std::vector<int> knapsack; //Will hold item indexes needed for final solution
  
  //This table holds values for each useful item and weight combination.
  //Combinations that build up our solution to a higher value than previous steps are included.
  std::vector<std::vector<int> > worksheet(numItems, std::vector<int>(maxWeight+1, 0));
  //This table holds booleans to show if we took an item or not.
  std::vector<std::vector<bool> > taken(numItems, std::vector<bool>(maxWeight+1, 0));

  //Generate some random weights and values
  for(int i=0; i<numItems; i++){
    values[i] = rand()%10;
    weights[i] = rand()%50;
  }
  
  //Print our random weights and values to see what we're working with.
  for(int i=0; i<numItems; i++)
    std::cout << "Item " << i << ": weighs " << weights[i] 
      << " pounds, worth $" << values[i] << '\n';  
  
  //Put our first item on our worksheet to get it started.
  //Avoids indexing problems in the main loop.
  if(weights[0] < maxWeight){
    worksheet[0][weights[0]] = values[0];
    taken[0][weights[0]]=true;
    for(int i=weights[0]; i<maxWeight+1; i++){
      worksheet[0][i+1]=worksheet[0][i];
      taken[0][i]=true;
    }
  }
  else if(weights[0] == maxWeight){
    worksheet[0][weights[0]] = values[0];
    taken[0][weights[0]]=true;
  }
  //Loop over the worksheet considering all items.
  for(int i=1; i<numItems; i++)
    for(int j=0; j<maxWeight+1; j++){ //Consider each item at every valid weight as a subproblem.
      if(weights[i]>j) //Drop in value from the row above if item is too heavy for this iteration.
	worksheet[i][j] = worksheet[i-1][j];
      else if(worksheet[i-1][j-weights[i]] +values[i] > worksheet[i-1][j]){
	//If the value of the worksheet would go up by adding this item, include it.
	worksheet[i][j] = worksheet[i-1][j-weights[i]] +values[i];
	//Note that we have taken this item at this step.
	taken[i][j]=true;
      }
      else //Drop in the value from the row above if item doesn't help.
	worksheet[i][j] = worksheet[i-1][j];
      if(j>0 && worksheet[i][j]< worksheet[i][j-1]) //Copy value forward if prior entry was greater.
	worksheet[i][j]= worksheet[i][j-1];
    }
  int W = maxWeight;
  int solutionWeight=0;
  //Find our items included in the solution
  for(int i=numItems-1; i>-1; i--)
    if(taken[i][W]==true){
      knapsack.push_back(i); //Put the items in the knapsack if they were in the optimal solution.
      W -= weights[i];
      solutionWeight+=weights[i];
    }
      
  
  //Print maximum value and the items required for that solution
  std::cout << '\n' << "Maximum weight of our knapsack: " << maxWeight;
  std::cout << '\n' << "Maximum value we can carry: $" << worksheet[numItems-1][maxWeight];
  std::cout << '\n' << "Total weight used to carry maximum value: " << solutionWeight << '/' << maxWeight;
  std::cout << '\n' << "Which item(s) to take for maximum value: ";
   for(int i=0; i<knapsack.size(); i++){
     std::cout << knapsack[i];
   if(i==knapsack.size()-1) std::cout << ". ";
   else std::cout << ", ";
   }
  std::cout << '\n' << '\n';
  
  //Print worksheet to study the algorithm's output.
  /* Our rows represent the number of items considered. 1-numItems rows.
     Columns represent the total weight. 0-maxWeight columns. (possible to have items weigh 0).
     Entries on the worksheet represent the max value at that weight and number of items. */
  std::cout << "Ending worksheet:" << '\n';
  for(int i=0; i<numItems; i++){
    for(int j=0; j<maxWeight+1; j++)
      std::cout << worksheet[i][j] << " ";
    std::cout << '\n';
  }
  std::cout << '\n';

  //Print taken decisions to study the algorithm's output.
  /* 1's in rows represent the fact that we took that item for the subproblem at that weight */
  std::cout << "Ending taken decisions:" << '\n';
  for(int i=0; i<numItems; i++){
    for(int j=0; j<maxWeight+1; j++)
      std::cout << taken[i][j] << " ";
    std::cout << '\n';
  }
  
  return 0;
}
