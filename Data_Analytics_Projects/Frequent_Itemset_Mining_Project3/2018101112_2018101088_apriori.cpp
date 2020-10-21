#include<bits/stdc++.h>
using namespace std;

void singletons_using_frequencyHash(int minsup, vector<vector<int> >&dataset, long long int singleton_frequency_hashmap[], set<int>&frequent_itemsets_singletons)
{
    for(int i = 0; i < dataset.size(); i++)
    {
        for(int j = 0; j < dataset[i].size(); j++)
        {
            singleton_frequency_hashmap[dataset[i][j]]++;
            if(singleton_frequency_hashmap[dataset[i][j]] >= minsup)
            {
                frequent_itemsets_singletons.insert(dataset[i][j]);
            }
        }
    }
}

void pairs_using_matrixHash(int minsup, vector<vector<int>>&dataset)
{

}

void remaining_using_hashTree(int minsup, vector<vector<int>>&dataset)
{

}

int main(void)
{
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //BASIC INITIALZATIONS
    int minsup = 6000;
    int max_single_item = INT_MIN;
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //CONVERSION OF ORIGINAL TEXT DATA INTO THE TRANSACTIONAL DATASET
    vector<vector<int> >dataset;
    vector<int>transaction;


    ifstream file_data("bible.txt");
    string single_transaction;
    while(getline(file_data, single_transaction))
    {
        string item = "";
        for(int i = 0; i < single_transaction.size(); )
        {
            if(single_transaction[i] == ' ')
            {
                stringstream item_i(item);
                int item_ii = 0;
                item_i >> item_ii;
                max_single_item = max(max_single_item, item_ii);
                transaction.push_back(item_ii);
                item = "";
            }
            else if(single_transaction[i] == '-')
            {
                i+=2;
            }
            else
            {
                item += single_transaction[i];
            }
            i++;
        }
        dataset.push_back(transaction);
        transaction.clear();
    }
    file_data.close();

    //PRINTING THE DATASET A AS TRANSACTIONS (VECTOR OF VECTORS)
    //  [[*,*,*,*,*],[*,*,*,*,*,*,*,*],[*,*,*]........]

    // for(int i = 0; i < dataset.size(); i++)
    // {
    //     for(int j = 0; j < dataset[i].size(); j++)
    //     {
    //         cout << dataset[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //1-ITEMSET - SINGLETONS

    // unordered_map<int,int>singleton_frequency_hashmap;
    long long int singleton_frequency_hashmap[max_single_item + 5] = {0};
    set<int>frequent_itemsets_singletons;
    set<int>:: iterator itr;
    singletons_using_frequencyHash(minsup, dataset,singleton_frequency_hashmap, frequent_itemsets_singletons);

    // PRINTING THE FREQUENCIES OF SINGLETONS
    // for(int i = 0; i <= max_single_item; i++)
    // {
    //     cout << singleton_frequency_hashmap[i] << " ";
    // }
    // cout << endl;


    //PRINTING THE FREQUENT SINGLETONS
    // for(itr = frequent_itemsets_singletons.begin(); itr != frequent_itemsets_singletons.end(); itr++)
    // {
    //     cout << *itr << " ";
    // }
    // cout << endl;
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //2-ITEMSET - PAIRS
    pairs_using_matrixHash(minsup, dataset);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // > 2-ITEMSET
    remaining_using_hashTree(minsup, dataset);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    return 0;
}