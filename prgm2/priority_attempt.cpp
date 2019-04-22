#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct knap_item{
  int profit = 0;
  int weight = 0;
};


struct item{
  int current_weight = 0;
  int current_profit = 0;
  int bound = 0;
  int item_num = 0;
  vector<knap_item> included;

  bool operator<(const item &other) const{
    return bound < other.bound;
  }
};


bool compare_items(knap_item x, knap_item y){
  double x_ratio = x.profit/x.weight;
  double y_ratio = y.profit/y.weight;
  return x_ratio > y_ratio;
}


double fractional_best(vector<knap_item> knapsack, int capacity, int current_profit, int starting_index){
  if(capacity == 0) return current_profit;
  double profit = current_profit;
  for(unsigned int i = starting_index; i < knapsack.size(); i++){
    if(capacity >= knapsack[i].weight){
      profit += knapsack[i].profit;
      capacity -= knapsack[i].weight;
    }
    else if (capacity == 0){
      break;
    }
    else{
      double ratio = 1.0*capacity/knapsack[i].weight;
      profit += knapsack[i].profit*ratio;
      capacity = 0;
    }
  }
  return profit;
}


int main(int argc, char** argv){
  if(argc == 3){
    cout << "Correct number of args" << endl;
  }
  std::fstream file;
  int num_items, capacity, temp_weight, temp_profit;
  char temp_char;

  file.open(argv[1]);
  file >> num_items;
  file >> temp_char;
  file >> capacity;
  cout << "Num: " << num_items << " Capacity: " << capacity << endl;

  vector<knap_item> knapsack;
  knap_item* temp_item;
  int* profits = new int[num_items];
  for(int i = 0; i < num_items; i++){
    file >> temp_weight;
    file >> temp_char;
    file >> temp_profit;
    cout << "Item weight: " << temp_weight << " Item profit: " << temp_profit << endl;
    temp_item = new knap_item();
    temp_item -> weight = temp_weight;
    temp_item -> profit = temp_profit;
    knapsack.push_back(*temp_item);
  }

  file.close();
  for(int i = 0; i < num_items; i++){
    profits[i] = 0;
    for(int j = i; j >= 0; j--){
      cout << "J: " << j << " I: " << i << " Num Items: " << num_items << endl;
      profits[j] += knapsack[j].profit;
    }
  }

  int num_leaves = 0, total_nodes = 0;
  vector<knap_item>  optimal;

  std::sort(knapsack.begin(), knapsack.end(), compare_items);
  int local_max = 0;
  
  // We must include 1. A vector of items under consideration.
  // 2. an int of the next item we are considering
  // 3. The upper bound for each node (The value we are comparing)
  //
  priority_queue<item> pq;
  item temp, temp2;
  temp.bound = fractional_best(knapsack, capacity, 0, 0);
  pq.push(temp);

  // Go in with a single node temp
  while(!pq.empty()){
    total_nodes++;
    temp = pq.top();
    pq.pop();
    cout << "Item: " << temp.item_num << " Profit: " << temp.current_profit << " Weight: " << temp.current_weight << " Bound: " << temp.bound << endl;
    // If we have more than max profit, make this optimal
    if(temp.current_profit > local_max){
      local_max = temp.current_profit;
      optimal = temp.included;
    }

    // If we have no more items, we hit the end
    if(temp.item_num > num_items){
      num_leaves++;
      continue;
    }
    // If our upper bound is above max, investigate
    if(temp.bound > local_max){
      // If maximum obtainable profit is less than max, stop.
      if (temp.current_profit + profits[temp.item_num] < local_max){
      // We have a sub optimal solution
        num_leaves++;
      } else {
        // Do not include the item here
        temp2.item_num = temp.item_num +1;
        temp2.current_profit = temp.current_profit;
        temp2.current_weight = temp.current_weight;
        temp2.included = temp.included;
        temp2.bound = fractional_best(knapsack, capacity -temp2.current_weight, temp2.current_profit, temp2.item_num);
        cout << "Temp2 Bound: " << temp2.bound << endl;
        // If we can go beyond max, push
        if(temp2.bound > local_max)
          pq.push(temp2);
        else
          num_leaves++;

        if(temp.current_weight + knapsack[temp.item_num].weight < capacity){
          // Consider adding the item
          
          // Add yes path to original
          temp.included.push_back(knapsack[temp.item_num]);

          temp.current_profit += knapsack[temp.item_num].profit;
          temp.current_weight += knapsack[temp.item_num].weight;
          temp.item_num++;
          temp.bound = fractional_best(knapsack, capacity -temp.current_weight, temp.current_profit, temp.item_num);
          if(temp.bound > local_max){
            pq.push(temp);
          }
          else
            num_leaves++;
        } // If we can't fit this item, let the path die.
      }
    } else {
      // We have a sub optimal solution
        num_leaves++;
    }      
  }
  std::ofstream ofile;
  cout << argv[2] << endl;
  ofile.open(argv[2]);
  ofile << num_items << "," << local_max << "," << optimal.size() << endl;
  ofile << total_nodes << "," << num_leaves << endl;
  for(int i = 0; i < optimal.size(); i++)
    ofile << optimal[i].weight << "," << optimal[i].profit << endl;
  ofile.close();
  return 1;
}
