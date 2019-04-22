#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

struct item{
  int weight;
  int profit;
};

using namespace std;

bool compare_items(item x, item y){
  double x_ratio = x.profit/x.weight;
  double y_ratio = y.profit/y.weight;
  return x_ratio > y_ratio;
}


double fractional_best(vector<item> knapsack, int capacity, int current_profit, int starting_index){
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

int recursive_function(vector<item>& knapsack, vector<item> accepted_items, vector<item>& optimal, unsigned int item_num, int capacity, int profit,  int &local_max, int* total_profits, int& num_leaves, int& total_nodes){
  //std::cout << "Profit: " << profit << " Capacity: " << capacity << endl;
  total_nodes++;
  if(item_num >= knapsack.size()){
    cout << "No more items" << endl;
    return local_max;
  }
  int upper = fractional_best(knapsack, capacity, profit, item_num);
  std::cout << "Upper: " << upper << endl;
  if(upper + profit < local_max){
      //This means that this path is sub optimal
      cout << "Terminating sub optimal branch" << endl;
      num_leaves++;
      return local_max;

  } else if(profit + total_profits[item_num] < local_max ){
        //This means that the max possible is sub optimal
      cout << "Terminating sub optimal branch" << endl;
      num_leaves++;
      return local_max;
  } else if(knapsack[item_num].weight > capacity){
    cout << "Skipping item" << endl;
    if(item_num == knapsack.size()-1) {
      num_leaves++;
      return local_max;
    }
    return recursive_function(knapsack, accepted_items, optimal, item_num+1, capacity, profit, local_max, total_profits, num_leaves, total_nodes);
  }
  else {
        //This means it is possible to get an optimal solution
        vector<item> other = accepted_items;
        accepted_items.push_back(knapsack[item_num]);
        if(profit +knapsack[item_num].profit > local_max){
          local_max = profit +knapsack[item_num].profit;
          cout << "Updated max: " << local_max << endl;
          optimal = accepted_items;
          
        }
        cout << "Accepted" << endl;
        for(int i = 0; i < accepted_items.size(); i++){
            cout << accepted_items[i].weight << "," << optimal[i].profit << endl;
          }

          cout << endl;
        int left, right;
        if(item_num == knapsack.size()-1) {
          num_leaves++;
          return local_max;
        }
        left = recursive_function(knapsack, other, optimal, item_num+1, capacity, profit, local_max, total_profits, num_leaves, total_nodes);
        right = recursive_function(knapsack, accepted_items, optimal, item_num+1, capacity-knapsack[item_num].weight, profit+knapsack[item_num].profit, local_max, total_profits, num_leaves, total_nodes);
        cout << "Left: " << left << " Right: " << right << endl;
        return std::max(left, right);
  }
}


int main(int argc, char** argv){
  if(argc == 3){
    cout << "Correct number of args" << endl;
  }
  else if (argc < 3){
    cout << "too few args, using test shit" << endl;
  }
  std::fstream file;
  int num_items, capacity, temp_weight, temp_profit;
  char temp;

  file.open(argv[1]);
  file >> num_items;
  file >> temp;
  file >> capacity;
  cout << "Num: " << num_items << " Capacity: " << capacity << endl;

  vector<item> knapsack;
  item* temp_item;
  int* profits = new int[num_items];
  for(int i = 0; i < num_items; i++){
    file >> temp_weight;
    file >> temp;
    file >> temp_profit;
    cout << "Item weight: " << temp_weight << " Item profit: " << temp_profit << endl;
    temp_item = new item();
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

  int num_leaves = 0, total_nodes = 0, optimal_nodes = 0;
  cout << "done reading" << endl;
  vector<item> temp_vec, optimal;

  std::sort(knapsack.begin(), knapsack.end(), compare_items);
  int local_max = 0;
  int ansr = recursive_function(knapsack,temp_vec, optimal, 0, capacity, 0, local_max, profits, num_leaves, total_nodes);



  std::fstream ofile;
  cout << argv[2] << endl;
  ofile.open(argv[2]);
  ofile << num_items << "," << local_max << "," << optimal.size() << endl;
  ofile << total_nodes << "," << num_leaves << endl;
  for(int i = 0; i < optimal.size(); i++)
    ofile << optimal[i].weight << "," << optimal[i].profit << endl;
  cout << "Ansr: " << ansr << std::endl;
  ofile.close();

  // For Best firs on knapsack we need a fuction to calculate the fractional knapsack 
  // We need a recursive call as well
}
