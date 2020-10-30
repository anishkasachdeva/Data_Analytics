#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

vector<int> get_candidate_supports_transactionReduction(vector<vector<int> >&Candidates_k, vector<set<int> >&dataset, vector<bool>&transaction_reduction_flag)
{
    vector<int> support_count(Candidates_k.size(),0);

    for(int t_id = 0; t_id < dataset.size(); t_id++)
    {
        bool is_transaction_useful = false;
        if(transaction_reduction_flag[t_id] == true)
        {
            if(dataset[t_id].size() < Candidates_k[0].size())
                continue;

            for(int c_id = 0; c_id < Candidates_k.size(); c_id++)
            {
                int count = 0;
                for(int c_element = 0; c_element < Candidates_k[0].size(); c_element++)
                {
                    if(dataset[t_id].find(Candidates_k[c_id][c_element]) != dataset[t_id].end())
                    {
                        count++;
                    }
                    else
                    {
                        break;
                    }
                }
                if(count == Candidates_k[0].size())
                {
                    is_transaction_useful = true;
                    support_count[c_id] += 1;
                }    
            }
            if(is_transaction_useful == false)
            {
                transaction_reduction_flag[t_id] = false;
            }
        }
    }
    return support_count;
}


vector<int> get_candidate_supports_normal(vector<vector<int> >&Candidates_k, vector<set<int> >&dataset, vector<bool>&transaction_reduction_flag)
{
    vector<int> support_count(Candidates_k.size(),0);

    for(int t_id = 0; t_id < dataset.size(); t_id++)
    {
        if(dataset[t_id].size() < Candidates_k[0].size())
            continue;

        for(int c_id = 0; c_id < Candidates_k.size(); c_id++)
        {
            int count = 0;
            for(int c_element = 0; c_element < Candidates_k[0].size(); c_element++)
            {
                if(dataset[t_id].find(Candidates_k[c_id][c_element]) != dataset[t_id].end())
                {
                    count++;
                }
                else
                {
                    break;
                }
            }
            if(count == Candidates_k[0].size())
            {
                support_count[c_id] += 1;
            }    
        }
    }
    return support_count;
}

void generate_frequent_itemsets(vector<vector<int> >&L_k,vector<set<int> >&dataset, vector<vector<int> >&Candidates_k, vector<bool>&transaction_reduction_flag, int minsup, int type)
{
    L_k.clear();
    vector<int> Candidates_k_supports;
    if(type == 1 || type == 3)
        Candidates_k_supports = get_candidate_supports_transactionReduction(Candidates_k, dataset,transaction_reduction_flag);
    else
        Candidates_k_supports = get_candidate_supports_normal(Candidates_k, dataset,transaction_reduction_flag);
    
    for(int i = 0; i < Candidates_k.size(); i++)
    {
        if(Candidates_k_supports[i] < minsup)
            continue;

        L_k.push_back(Candidates_k[i]);
    }
}


void process_next_candidates(vector<vector<int> >&Candidates_k,vector<vector<int> > &L_k, vector<set<int> >&dataset, int items_size, int type)
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
        //Pruning
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
            Candidates_k = pruned;
}


void run_apriori(vector<vector<int> >&Candidates_k, vector<vector<int> >&L_k, vector<bool>&transaction_reduction_flag, vector<set<int> >&dataset,vector<vector<vector<int> > >&frequent_itemsets_mined, int items_size, int minsup, int type)
{
    while(1)
    {
        process_next_candidates(Candidates_k,L_k, dataset,items_size, type);
        items_size++;
        if(Candidates_k.size() == 0)
            break;

        generate_frequent_itemsets(L_k,dataset, Candidates_k, transaction_reduction_flag, minsup, type);

        if(L_k.size() > 0)
            frequent_itemsets_mined.push_back(L_k);

    }
}


void display_frequent_itemsets(vector <vector <vector <int> > >& frequent_itemsets_mined)
{
	int z=0,j,k, total_itemsets = 0;
    if(frequent_itemsets_mined.size() == 0)
        cout << "No Frequent Itemsets Found" << endl;
    else
    {
        for(auto i : frequent_itemsets_mined)
        {
            total_itemsets += i.size();
            cout << "\nFrequent Itemsets for k = " << ++z << "\n" << "Number of Itemsets formed are : " << i.size() << endl << "\n[";
            for(j=0;j<i.size()-1;j++)
            {
                cout << "[";
                for(k=0;k<i[j].size()-1;k++)
                {
                    cout << i[j][k] << ",";
                }
                cout << i[j][k] << "],";
            }
            cout << "[";
            for(k=0;k<i[j].size()-1;k++)
            {
                cout << i[j][k] << ",";
            }
            cout << i[j][k] << "]";
            cout << "]\n";
        }
        cout << endl << "Total Number of Itemsets Generated are : " << total_itemsets << endl << endl;
    }
}
// //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    //BASIC INITIALZATIONS
    int minsup;
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
    vector<set<int> >dataset;
    set<int>transaction;
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
                transaction.insert(item_ii);
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
        dataset.push_back(transaction);
        transaction.clear();
    }
    file_data.close();
    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    vector<vector<int> >Candidates_k;
    vector<vector<int> >L_k;
    vector<vector<vector<int> > >frequent_itemsets_mined;
    vector<bool>transaction_reduction_flag(dataset.size(), true);
    int items_size; //initially for singletons
    int type;
    


    set<int> c1;
    map<int,int>count_c1;

    type = 0; //NORMAL
    // type = 1; //TRANSACTION REDUCTION
    // type = 2; //HASH BASED TECHNIQUE
    // type = 3; //COMPLETELY OPTIMIZED
    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl << endl;

    // cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl << endl;
    if(type == 0)
    {
        cout << "OUTPUTS GENERATED BY NORMAL APRIORI ALGORITHM" << endl << endl;
        items_size = 2;
        Candidates_k.clear();
        L_k.clear();
        frequent_itemsets_mined.clear();
        c1.clear();
        count_c1.clear();
        vector<bool>transaction_reduction_flag(dataset.size(), true);
        
        auto start = high_resolution_clock::now();
        for(int i = 0; i < dataset.size(); i++)
        {
            set<int>transaction = dataset[i];
            set<int>::iterator itr_t;
            for(itr_t = transaction.begin(); itr_t != transaction.end(); itr_t++)
            {
                c1.insert(*itr_t);
                count_c1[*itr_t]++;
            }
        }

        set<int>::iterator itr_c1;
        for(itr_c1 = c1.begin(); itr_c1 != c1.end(); itr_c1++)
        {
            if(count_c1[*itr_c1]>=minsup)
                L_k.push_back(vector<int>(1,*itr_c1));
        }
        frequent_itemsets_mined.push_back(L_k);


        run_apriori(Candidates_k, L_k, transaction_reduction_flag, dataset, frequent_itemsets_mined, items_size, minsup, type);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        display_frequent_itemsets(frequent_itemsets_mined);
        cout << endl;
        cout << endl << "TIME TAKEN : " << fixed 
            << duration.count()/1000000.0 << setprecision(5); 
        cout << " sec " << endl << endl;
    }
    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl << endl;

    type = 1;

    if(type == 1)
    {
        cout << "OUTPUTS GENERATED BY APRIORI ALGORITHM OPTIMIZED WITH TRANSACTION REDUCTION TECHNIQUE" << endl << endl;
        items_size = 2;
        Candidates_k.clear();
        L_k.clear();
        frequent_itemsets_mined.clear();
        c1.clear();
        count_c1.clear();
        vector<bool>transaction_reduction_flag(dataset.size(), true);
        auto start = high_resolution_clock::now();
        for(int i = 0; i < dataset.size(); i++)
        {
            set<int>transaction = dataset[i];
            set<int>::iterator itr_t;
            for(itr_t = transaction.begin(); itr_t != transaction.end(); itr_t++)
            {
                c1.insert(*itr_t);
                count_c1[*itr_t]++;
            }
        }

        set<int>::iterator itr_c1;
        for(itr_c1 = c1.begin(); itr_c1 != c1.end(); itr_c1++)
        {
            if(count_c1[*itr_c1]>=minsup)
                L_k.push_back(vector<int>(1,*itr_c1));
        }
        frequent_itemsets_mined.push_back(L_k);


        run_apriori(Candidates_k, L_k, transaction_reduction_flag, dataset, frequent_itemsets_mined, items_size, minsup, type);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        display_frequent_itemsets(frequent_itemsets_mined);
        cout << endl << "TIME TAKEN : " << fixed 
            << duration.count()/1000000.0 << setprecision(5); 
        cout << " sec " << endl << endl;

    }

    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl << endl;


    type = 2;

    if(type == 2)
    {
        cout << "OUTPUTS GENERATED BY APRIORI ALGORITHM OPTIMIZED WITH HASH BASED TECHNIQUE" << endl << endl;
        items_size = 3;
        Candidates_k.clear();
        L_k.clear();
        frequent_itemsets_mined.clear();
        c1.clear();
        count_c1.clear();
        vector<bool>transaction_reduction_flag(dataset.size(), true);

        auto start = high_resolution_clock::now(); 
        // set<int> c1;
        c1.clear();
        count_c1.clear();
        // map<int,int>count_c1;
        map<pair<int,int>,int>count_c2;

        set<int>::iterator itr1,itr2;
        for(int i=0;i<dataset.size();i++)
        {
            itr1=dataset[i].begin();
            while(itr1!=dataset[i].end())
            {
                c1.insert(*itr1);
                if(count_c1.find(*itr1)!=count_c1.end())
                {
                    count_c1[*itr1]++;
                }
                else
                {
                    count_c1[*itr1]=1;
                }
                itr2=next(itr1,1);
                while(itr2!=dataset[i].end())
                {
                    if(count_c2.find(make_pair(*itr1,*itr2))!=count_c2.end())
                    {
                        count_c2[make_pair(*itr1,*itr2)]++;
                    }
                    else
                    {
                        count_c2[make_pair(*itr1,*itr2)]=1;
                    }
                    itr2++;
                }
                itr1++;
            }
        }
        itr1=c1.begin();
        while(itr1!=c1.end())
        {
            if(count_c1[*itr1]>=minsup)
                L_k.push_back(vector<int>(1,*itr1));
            itr1++;
        }
        frequent_itemsets_mined.push_back(L_k);


        L_k.clear();
        map<pair<int,int>,int>::iterator itr_map=count_c2.begin();
        while(itr_map!=count_c2.end())
        {
            if(itr_map->second>=minsup)
            {
                L_k.push_back(vector<int>(2,0));
                L_k[L_k.size()-1][0]=itr_map->first.first;
                L_k[L_k.size()-1][1]=itr_map->first.second;
            }
            itr_map++;
        }
        frequent_itemsets_mined.push_back(L_k);


        run_apriori(Candidates_k, L_k, transaction_reduction_flag, dataset, frequent_itemsets_mined, items_size, minsup, type);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        display_frequent_itemsets(frequent_itemsets_mined);
        cout << endl << "TIME TAKEN : " << fixed 
            << duration.count()/1000000.0 << setprecision(5); 
        cout << " sec " << endl << endl; 

    }

    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl << endl;

    type = 3;

    if(type == 3)
    {
        cout << "OUTPUTS GENERATED BY APRIORI ALGORITHM OPTIMIZED WITH BOTH TRANSACTION REDUCTION AND HASH BASED TECHNIQUE" << endl << endl;
        items_size = 3;
        Candidates_k.clear();
        L_k.clear();
        frequent_itemsets_mined.clear();
        c1.clear();
        count_c1.clear();
        vector<bool>transaction_reduction_flag(dataset.size(), true);

         auto start = high_resolution_clock::now(); 
        c1.clear();
        count_c1.clear();
        map<pair<int,int>,int>count_c2;

        set<int>::iterator itr1,itr2;
        for(int i=0;i<dataset.size();i++)
        {
            itr1=dataset[i].begin();
            while(itr1!=dataset[i].end())
            {
                c1.insert(*itr1);
                if(count_c1.find(*itr1)!=count_c1.end())
                {
                    count_c1[*itr1]++;
                }
                else
                {
                    count_c1[*itr1]=1;
                }
                itr2=next(itr1,1);
                while(itr2!=dataset[i].end())
                {
                    if(count_c2.find(make_pair(*itr1,*itr2))!=count_c2.end())
                    {
                        count_c2[make_pair(*itr1,*itr2)]++;
                    }
                    else
                    {
                        count_c2[make_pair(*itr1,*itr2)]=1;
                    }
                    itr2++;
                }
                itr1++;
            }
        }
        itr1=c1.begin();
        while(itr1!=c1.end())
        {
            if(count_c1[*itr1]>=minsup)
                L_k.push_back(vector<int>(1,*itr1));
            itr1++;
        }
        frequent_itemsets_mined.push_back(L_k);


        L_k.clear();
        map<pair<int,int>,int>::iterator itr_map=count_c2.begin();
        while(itr_map!=count_c2.end())
        {
            if(itr_map->second>=minsup)
            {
                L_k.push_back(vector<int>(2,0));
                L_k[L_k.size()-1][0]=itr_map->first.first;
                L_k[L_k.size()-1][1]=itr_map->first.second;
            }
            itr_map++;
        }
        frequent_itemsets_mined.push_back(L_k);


        run_apriori(Candidates_k, L_k, transaction_reduction_flag, dataset, frequent_itemsets_mined, items_size, minsup, type);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        display_frequent_itemsets(frequent_itemsets_mined);
        cout << endl << "TIME TAKEN : " << fixed 
            << duration.count()/1000000.0 << setprecision(5); 
        cout << " sec " << endl << endl; 

    }
    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl << endl;
    return 0;
}