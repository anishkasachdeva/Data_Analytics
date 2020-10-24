
#include<bits/stdc++.h>
using namespace std;

// set<int>singletons ;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// vector<vector<int> > singletons_using_frequencyHash(vector<vector<int> >&dataset)
// {
//     // set<int>singletons;
//     set<int>::iterator itr;

//     vector<vector<int> >elements;
//     // for(int i = 0; i < dataset.size(); i++)
//     // {
//     //     int size = dataset[i].size();
//     //     for(int j = 0; j < size; j++)
//     //     {
//     //         singletons.insert(dataset[i][j]);
//     //     }
//     // }
//     for(itr = singletons.begin(); itr != singletons.end(); itr++)
//     {
//         elements.push_back(vector<int>(1, *itr));
//     }
//     return elements;
// }


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


// long double getSupport(vector<int> candidate, vector<vector<int> >&dataset)
// {
//     int count = 0;
//     int size = dataset.size();

//     for(int k = 0; k < size; k++)
//     {
//         vector<int>transaction = dataset[k];
//         int i, j;

//         if(transaction.size() < candidate.size())
//             continue;

//         for(i = 0, j = 0; i < transaction.size(); i++)
//         {
//             if(j == candidate.size())
//                 break;

//             if(transaction[i] == candidate[j])
//                 j++;
//         }
//         if(j == candidate.size())
//         {
//             count++;
//         }
//     }
//     return (long double)count/dataset.size()*100.0;
// }


// vector<vector<int> > generate_frequent_itemsets(vector<vector<int> >&dataset, vector<vector<int> >&Candidates_k, int minsup)
// {
//     vector<vector<int> >frequents;
//     for(int i = 0; i < Candidates_k.size(); i++)
//     {
//         long double support = getSupport(Candidates_k[i], dataset);
//         if(round(support, 2) < minsup)
//             continue;

//         frequents.push_back(Candidates_k[i]);
//     }
//     return frequents;
// }

vector<long double> get_candidate_supports(vector<vector<int> >&Candidates_k, vector<vector<int> >&dataset)
{
    vector<int> support_count(Candidates_k.size(),0);
    vector<long double> support(Candidates_k.size(),0.0);
    for(int i=0;i<dataset.size();i++)
    {
        vector<int> indix(Candidates_k.size(),0);
        for(int j=0;j<dataset[i].size();j++)
        {
            for(int k=0;k<Candidates_k.size();k++)
            {
                if(indix[k]>=0&&dataset[i][j]==Candidates_k[k][indix[k]])
                {
                    indix[k]++;
                    if(indix[k]==Candidates_k[0].size())
                    {
                        indix[k]=-1;
                        support_count[k]+=1;
                    }
                }
            }
        }
    }
    for(int i=0;i<Candidates_k.size();i++)
    {
        support[i]=(long double)support_count[i]/dataset.size()*100.0;
    }
    return support;
}
void generate_frequent_itemsets(vector<vector<int> >&L_k,vector<vector<int> >&dataset, vector<vector<int> >&Candidates_k, int minsup)
{
    // vector<vector<int> >frequents;
    L_k.clear();
    vector<long double> Candidates_k_supports=get_candidate_supports(Candidates_k, dataset);
    for(int i = 0; i < Candidates_k.size(); i++)
    {
        // long double support = getSupport(Candidates_k[i], dataset);
        if(round(Candidates_k_supports[i], 2) < minsup)
            continue;

        L_k.push_back(Candidates_k[i]);
    }
}





void process_next_candidates(vector<vector<int> >&Candidates_k,vector<vector<int> > &L_k, vector<vector<int> >&dataset, int items_size)
{
    if(items_size == 1) //singletons
    {
        set<int>::iterator itr;

        vector<vector<int> >elements;
        set<int> singletons;
        for(int i = 0; i < dataset.size(); i++)
        {
            for(int j = 0; j < dataset[i].size(); j++)
            {
                singletons.insert(dataset[i][j]);
            }
        }
        for(itr = singletons.begin(); itr != singletons.end(); itr++)
        {
            Candidates_k.push_back(vector<int>(1, *itr));
        }
    }
    // else if(items_size == 2) //for itemset size = 2
    // {
            // return pairs_using_HashTechnique(dataset);
    // }
    else //for itemset size  > 2
    {
        Candidates_k.clear();

        //merge L
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
                    Candidates_k.push_back(v2);
                }
                else
                    break;
            }
        }
        //Pruneing
        set<vector<int> >L_k_set;
        set<vector<int> >::iterator itr;
        for(int i = 0; i < L_k.size(); i++)
        {
            L_k_set.insert(L_k[i]);
        }

        vector<vector<int> >pruned;
        bool is_present = false;
        for(int i = 0; i < Candidates_k.size(); i++)
        {
            is_present = false;
            for(int j = 0; j < Candidates_k[i].size(); j++)
            {
                vector<int>merged_item =Candidates_k[i];
                merged_item.erase(merged_item.begin() + j);
                if(L_k_set.find(merged_item) == L_k_set.end())
                {
                    is_present = true;
                    break;
                }
            }
            if(is_present == false)
                pruned.push_back(Candidates_k[i]);
        }
        Candidates_k= pruned;
    }
}


void run_apriori(vector<vector<int> >&Candidates_k, vector<vector<int> >&L_k, vector<vector<int> >&dataset,vector<vector<vector<int> > >&frequent_itemsets_mined, int items_size, int minsup)
{
    while(1)
    {
        process_next_candidates(Candidates_k,L_k, dataset,items_size);
        items_size++;
        if(Candidates_k.size() == 0)
            break;

        generate_frequent_itemsets(L_k,dataset, Candidates_k, minsup);
        // L_k = generate_frequent_itemsets(dataset, Candidates_k, minsup);
        frequent_itemsets_mined.push_back(L_k);

        cout << "Frequent Itemsets of size " << items_size-1 << " are : " << endl;
        for(int i = 0; i < L_k.size(); i++)
        {
            int size = L_k[i].size();
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
    set<int>singletons ;
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
                singletons.insert(item_ii);          //PRECOMPUTING SINGLETONS BEFOREHAND TO AVOID AN EXTRA PASS
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

    time(&start); 
    run_apriori(Candidates_k, L_k, dataset, frequent_itemsets_mined, items_size, minsup);
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