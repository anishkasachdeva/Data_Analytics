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
        for(int j = 0; j < dataset[i].size(); j++)
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

long double getSupport(vector<int> item, vector<vector<int> >&dataset) {
        int ret = 0;
        for(auto&row:dataset){
            int i, j;
            if(row.size() < item.size()) continue;
            for(i=0, j=0; i < row.size();i++) {
                if(j==item.size()) break;
                if(row[i] == item[j]) j++;
            }
            if(j==item.size()){
                ret++;
            }
        }
        // cout << (long double)ret/transactions.size()*100.0 << endl;
        return (long double)ret/dataset.size()*100.0;
    }



vector<vector<int> > generate_frequent_itemsets(vector<vector<int> >&dataset, vector<vector<int> >&Candidates_k, int minsup)
{
    // cout << "Came to generate frequent itemsets" << endl;
    vector<vector<int> >ret;
    for(int i = 0; i < Candidates_k.size(); i++)
    {
        long double support = getSupport(Candidates_k[i], dataset);
        if(round(support, 2) < minsup)
            continue;

        ret.push_back(Candidates_k[i]);
    }
    return ret;
}


vector<vector<int> > prune(vector<vector<int> >&L_k, vector<vector<int> >&merged_L_k)
{
    set<vector<int> >L_k_set;
    set<vector<int> >::iterator itr;
    for(int i = 0; i < L_k.size(); i++)
    {
        L_k_set.insert(L_k[i]);
    }

    // cout << L_k_set.size() << endl;
    // for(itr = L_k_set.begin(); itr != L_k_set.end(); itr++)
    // {
    //     for(int j = 0; j < itr.size(); j++)
    //     {
    //         cout << L
    //     }
    // }
    // cout << "original " << endl;
    // for(int i = 0; i < L_k.size(); i++)
    // {
    //     for(int j = 0; j < L_k[i].size();j++)
    //     {
    //         cout << L_k[i][j] << " ";
    //     }
    //     cout << endl;
    // }


    // cout << "merged " << endl;
    // for(int i = 0; i < merged_L_k.size(); i++)
    // {
    //     for(int j = 0; j < merged_L_k[i].size();j++)
    //     {
    //         cout << merged_L_k[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    vector<vector<int> >pruned;
    bool is_present = false;
    for(int i = 0; i < merged_L_k.size(); i++)
    {
        is_present = false;
        for(int j = 0; j < merged_L_k[i].size(); j++)
        {
            vector<int>merged_item = merged_L_k[i];
            merged_item.erase(merged_item.begin() + j);
            if(L_k_set.find(merged_item) == L_k_set.end())
            {
                // cout << "fghjkgfhj" << endl;
                is_present = true;
                break;
            }
        }
        if(is_present == false)
            pruned.push_back(merged_L_k[i]);
    }
    // for(int i = 0; i < pruned.size(); i++)
    // {
    //     for(int j = 0; j < pruned[i].size();j++)
    //     {
    //         cout << pruned[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    return pruned;
}


vector<vector<int> > merge(vector<vector<int>> L_k)
{
    // cout << "came to merge" << endl;
    // for(int i = 0; i < L_k.size(); i++)
    // {
    //     for(int j = 0; j < L_k[i].size();j++)
    //     {
    //         cout << L_k[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    vector<vector<int>>merged_L_k;
    for(int i = 0; i < L_k.size(); i++)
    {
        vector<int>v1 = L_k[i];  
        // cout << "v1 ";
        // for(int x = 0; x < v1.size(); x++)
        // {
        //     cout << v1[x] << " ";
        // }
        // cout << endl;
        int last_item_v1 = v1[v1.size()-1];
        v1.pop_back();
        for(int j = i + 1; j < L_k.size(); j++)
        {
            // cout << j << endl;
            vector<int>v2 = L_k[j];
            // cout << "v2 ";
            // for(int y = 0; y < v1.size(); y++)
            // {
            //     cout << v2[y] << " ";
            // }
            // cout << endl;
            int last_item_v2 = v2[v2.size()-1];
            v2.pop_back();
            if(v1 == v2)
            {
                // cout << "cameeeeeeeeeeeeeeeeeeee" << endl;
                int small = min(last_item_v1, last_item_v2);
                int large = max(last_item_v1, last_item_v2);
                v2.push_back(small);
                v2.push_back(large);
                // for(int t = 0; t < v2.size(); t++)
                // {
                //     cout << v2[t] << " ";
                // }
                // cout << endl;
                merged_L_k.push_back(v2);
            }
            else
            {
                break;
            }
        }
    }
    // for(int i = 0; i < merged_L_k.size(); i++)
    // {
    //     for(int j = 0; j < merged_L_k[i].size();j++)
    //     {
    //         cout << merged_L_k[i][j] << " ";
    //     }
    //     cout << endl;
    // }
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
        // for(int i = 0; i < merged_L_k.size(); i++)
        // {
        //     for(int j = 0; j < merged_L_k[i].size(); j++)
        //     {
        //         cout << merged_L_k[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        return prune(L_k, merged_L_k);
        // return prune();
    }
}


void run_apriori(vector<vector<int> >&Candidates_k, vector<vector<int> >&L_k, vector<vector<int> >&dataset,vector<vector<vector<int> > >&frequent_itemsets_mined, int items_size, int minsup)
{
    while(1)
    {
        Candidates_k = process_next_candidates(L_k, dataset,items_size);
        items_size++;
        if(Candidates_k.size() == 0)
            break;


        // for(int i = 0; i < L_k.size(); i++)
        // {
        //     for(int j = 0; j < L_k[i].size(); i++)
        //     {
        //         cout << L_k[i][j] << " ";
        //     }
        //     cout << endl;
        // }

        L_k = generate_frequent_itemsets(dataset, Candidates_k, minsup);

        frequent_itemsets_mined.push_back(L_k);
        // cout << L_k.size() << endl;
        // for(int i = 0; i < L_k.size(); i++)
        // {
        //     for(int j = 0; j < L_k[i].size(); j++)
        //     {
        //         cout << L_k[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << "Item_size : " << items_size-1 << endl;
        // for(int i = 0; i < Candidates_k.size(); i++)
        // {
        //     for(int j = 0; j < Candidates_k[i].size(); i++)
        //     {
        //         cout << Candidates_k[i][j] << " ";
        //     }
        //     cout << endl;
        // }
        // cout << "---------------------" << endl;
        // for(int i = 0; i < L_k.size(); i++)
        // {
        //     for(int j = 0; j < L_k[i].size(); i++)
        //     {
        //         cout << L_k[i][j] << " ";
        //     }
        //     cout << endl;
        // }
    }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
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

    //Write error message for not opening a file!----------------------- TODO !!
    ifstream file_data(input_file);
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
    vector<vector<int> >Candidates_k;
    vector<vector<int> >L_k;
    vector<vector<vector<int> > >frequent_itemsets_mined;
    int items_size = 1; //initially for singletons


    run_apriori(Candidates_k, L_k, dataset, frequent_itemsets_mined, items_size, minsup);

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
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //1-ITEMSET - SINGLETONS

    // unordered_map<int,int>singleton_frequency_hashmap;
    // long long int singleton_frequency_hashmap[max_single_item + 5] = {0};
    // set<int>frequent_itemsets_singletons;
    // set<int>:: iterator itr;
    // singletons_using_frequencyHash(minsup, dataset,singleton_frequency_hashmap, frequent_itemsets_singletons);

    // PRINTING THE FREQUENCIES OF SINGLETONS
    // for(int items = 0; items <= max_single_item; items++)
    // {
    //     cout << singleton_frequency_hashmap[items] << " ";
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
    // pairs_using_matrixHash(minsup, dataset);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // > 2-ITEMSET
    // remaining_using_hashTree(minsup, dataset);
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------