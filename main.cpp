/* Written by Andrew Doyle */

#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

int main(){
  std::srand(std::time(0)); //Seed our random number generator with the time of day
  int maxWeight = 30; //Can carry this ammount of weight
  int numItems = 6; //This many items to consider
  
  std::vector<int> weights(numItems, 0); //Weight of each item
  std::vector<int> values(numItems, 0); //Value of each item
  
  //This table holds values for each useful item and weight combination.
  //Combinations that build up our solution to a higher value than previous steps are included.
  std::vector<std::vector<int> > worksheet(numItems, std::vector<int>(maxWeight+1, 0));

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
    for(int i=weights[0]; i<maxWeight-1; i++)
      worksheet[0][i+1]=worksheet[0][i];
  }
  else if(weights[0] == maxWeight)
    worksheet[0][weights[0]] = values[0];
  
  //Loop over the worksheet considering all items.
  for(int i=1; i<numItems; i++)
    for(int j=0; j<maxWeight+1; j++){ //Consider each item at every valid weight as a subproblem.
      if(weights[i]>j) //Drop in value from the row above if item is too heavy for this iteration.
	worksheet[i][j] = worksheet[i-1][j];
      else if(worksheet[i-1][j-weights[i]] +values[i] > worksheet[i-1][j]) 
	//If the value of the worksheet would go up by adding this item, include it.
	worksheet[i][j] = worksheet[i-1][j-weights[i]] +values[i];
      else //Drop in the value from the row above if item doesn't help.
	worksheet[i][j] = worksheet[i-1][j];
      if(j>0 && worksheet[i][j]< worksheet[i][j-1]) //Copy value forward if prior entry was greater.
	worksheet[i][j]= worksheet[i][j-1];
    }
  
  //Print maximum value
  std::cout << '\n' << "Maximum value: $" << worksheet[numItems-1][maxWeight] << '\n' << '\n';
  
  //Print worksheet to study the algorithm's output.
  /* Our rows represent the number of items considered. 1-numItems rows.
     Columns represent the total weight. 0-maxWeight columns. (possible to have items weigh 0).
     Entries on the worksheet represent the max value at that weight and number of items. */
  for(int i=0; i<numItems; i++){
    for(int j=0; j<maxWeight+1; j++)
      std::cout << worksheet[i][j] << " ";
    std::cout << '\n';
  }
  
  return 0;
}