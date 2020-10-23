#include<bits/stdc++.h>
using namespace std;


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
vector<vector<int> > singletons_using_frequencyHash(vector<vector<int> >&dataset)
{
    set<int>singletons;
    set<int>::iterator itr;

    vector<vector<int> >elements;
    for(int i = 0; i < dataset.size(); i++)
    {
        int size = dataset[i].size();
        for(int j = 0; j < size; j++)
        {
            singletons.insert(dataset[i][j]);
        }
    }
    for(itr = singletons.begin(); itr != singletons.end(); itr++)
    {
        elements.push_back(vector<int>(1, *itr));
    }
    return elements;
}


vector<vector<int> > pairs_using_HashTechnique(vector<vector<int> >&dataset)
{
    vector<vector<int> >elements;
    return elements;
}

long double round(long double value, int pos)
{
    long double temp;
    temp = value * pow( 10, pos );
    temp = floor( temp + 0.5 );
    temp *= pow( 10, -pos );
    return temp;
}

long double getSupport(vector<int> candidate, vector<vector<int> >&dataset, vector<bool>&transaction_reduction_flag, vector<int>&transaction_temp_flag) {
        int count = 0;
        int size = dataset.size();

        for(int t_id = 0; t_id < size; t_id++)
        {
            vector<int>transaction = dataset[t_id];
            int i, j;
            if(transaction_reduction_flag[t_id] == false)
            {
                if(transaction.size() < candidate.size())
                    continue;

                for(i = 0, j = 0; i < transaction.size(); i++)
                {
                    if(j == candidate.size())
                        break;

                    if(transaction[i] == candidate[j])
                        j++;
                }
                if(j == candidate.size())
                {
                    transaction_temp_flag[t_id] = 1;
                    count++;
                }
            }
        }
        return (long double)count/dataset.size()*100.0;
    }


vector<vector<int> > generate_frequent_itemsets(vector<vector<int> >&dataset, vector<vector<int> >&Candidates_k, vector<bool>&transaction_reduction_flag, int minsup)
{
    vector<int>transaction_temp_flag(dataset.size(),0);
    vector<vector<int> >frequents;
    for(int i = 0; i < Candidates_k.size(); i++)
    {
        long double support = getSupport(Candidates_k[i], dataset, transaction_reduction_flag, transaction_temp_flag);
        if(round(support, 2) < minsup)
            continue;

        frequents.push_back(Candidates_k[i]);
    }
    for(int i = 0; i < transaction_temp_flag.size(); i++)
    {
        if(transaction_temp_flag[i] == 0)
            transaction_reduction_flag[i] = true;
    }
    // for(int i = 0; i < transaction_reduction_flag.size(); i++)
    // {
    //     cout << transaction_reduction_flag[i] << " ";
    // }
    // cout << endl;
    return frequents;
}


vector<vector<int> > prune(vector<vector<int> >&L_k, vector<vector<int> >&merged_L_k)
{
    set<vector<int> >L_k_set;
    set<vector<int> >::iterator itr;
    for(int i = 0; i < L_k.size(); i++)
    {
        L_k_set.insert(L_k[i]);
    }

    vector<vector<int> >pruned;
    bool is_present = false;
    for(int i = 0; i < merged_L_k.size(); i++)
    {
        is_present = false;
        int size = merged_L_k[i].size();
        for(int j = 0; j < size; j++)
        {
            vector<int>merged_item = merged_L_k[i];
            merged_item.erase(merged_item.begin() + j);
            if(L_k_set.find(merged_item) == L_k_set.end())
            {
                is_present = true;
                break;
            }
        }
        if(is_present == false)
            pruned.push_back(merged_L_k[i]);
    }
    return pruned;
}


vector<vector<int> > merge(vector<vector<int>> L_k)
{
    vector<vector<int>>merged_L_k;
    for(int i = 0; i < L_k.size(); i++)
    {
        vector<int>v1 = L_k[i];  
        int last_item_v1 = v1[v1.size()-1];
        v1.pop_back();
        for(int j = i + 1; j < L_k.size(); j++)
        {
            vector<int>v2 = L_k[j];
            int last_item_v2 = v2[v2.size()-1];
            v2.pop_back();
            if(v1 == v2)
            {
                int small = min(last_item_v1, last_item_v2);
                int large = max(last_item_v1, last_item_v2);
                v2.push_back(small);
                v2.push_back(large);
                merged_L_k.push_back(v2);
            }
            else
                break;
        }
    }
    return merged_L_k;
}


vector<vector<int> > process_next_candidates(vector<vector<int>> L_k, vector<vector<int> >&dataset, int items_size)
{
    if(items_size == 1) //singletons
    {
        return singletons_using_frequencyHash(dataset);
    }
    // else if(items_size == 2) //for itemset size = 2
    // {
            // return pairs_using_HashTechnique(dataset);
    // }
    else //for itemset size  > 2
    {
        vector<vector<int>>merged_L_k = merge(L_k);
        return prune(L_k, merged_L_k);
    }
}


void run_apriori(vector<vector<int> >&Candidates_k, vector<vector<int> >&L_k, vector<vector<int> >&dataset,vector<vector<vector<int> > >&frequent_itemsets_mined,vector<bool>&transaction_reduction_flag, int items_size, int minsup)
{
    while(1)
    {
        Candidates_k = process_next_candidates(L_k, dataset,items_size);
        items_size++;
        if(Candidates_k.size() == 0)
            break;

        L_k = generate_frequent_itemsets(dataset, Candidates_k, transaction_reduction_flag, minsup);

        frequent_itemsets_mined.push_back(L_k);

        cout << "Frequent Itemsets of size " << items_size-1 << " are : " << endl;
        for(int i = 0; i < L_k.size(); i++)
        {
            int size = L_k[i].size();
            // cout << "{";
            for(int j = 0; j < size; j++)
            {
                cout << L_k[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl << "------------------------------------------------------------" << endl;        
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
    time_t start, end; 
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //BASIC INITIALZATIONS
    double minsup;
    cout << "Enter the Minimum Support Value : ";
    cin >> minsup;
    cout << "------------------------------------------------------------" << endl;

    string input_file;
    cout << "Enter the Transactional Dataset Filename : ";
    cin >> input_file;
    cout << "------------------------------------------------------------" << endl;

    int max_single_item = INT_MIN;
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //CONVERSION OF ORIGINAL TEXT DATA INTO THE TRANSACTIONAL DATASET
    vector<vector<int> >dataset;
    vector<int>transaction;

    //Error message for not opening a file
    ifstream file_data(input_file);
    if(!file_data)
    {
        cout << endl << "The given file does not exist!" << endl;
        return -1;
    }
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
                i += 2;
            }
            else
            {
                item += single_transaction[i];
            }
            i++;
        }
        sort(transaction.begin(), transaction.end());
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
    vector<bool>transaction_reduction_flag(dataset.size(),false);
    vector<vector<int> >Candidates_k;
    vector<vector<int> >L_k;
    vector<vector<vector<int> > >frequent_itemsets_mined;
    int items_size = 1; //initially for singletons

    time(&start); 
    run_apriori(Candidates_k, L_k, dataset, frequent_itemsets_mined,transaction_reduction_flag, items_size, minsup);
    time(&end); 

    // for(int i = 0; i < frequent_itemsets_mined.size(); i++)
    // {
    //     cout << "Frequent Itemsets of size " << i << " are : " << endl;
    //     for(int j = 0; j < frequent_itemsets_mined[i].size(); j++)
    //     {
    //         for(int k = 0; k < frequent_itemsets_mined[j].size(); k++)
    //         {
    //             cout << frequent_itemsets_mined[i][j][k] << " ";
    //         }
    //         cout << endl;
    //     }
    //     cout << endl;
    // }
    double time_taken = double(end - start); 
    cout << endl << endl << "Time taken by the Apriori Algorithm with Hash Based Technique is : " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl; 
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------