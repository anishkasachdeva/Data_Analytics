#include<bits/stdc++.h>
#include <chrono> 
using namespace std::chrono; 
  
using namespace std;
typedef long long int ll;



vector<vector<int> >frequents_global;

struct FP_TreeNode {    

    int count;
    int node_value;
    FP_TreeNode* parent;
    map<int, FP_TreeNode*>children;
    bool marked;

};


FP_TreeNode* create_TreeNode(FP_TreeNode* parent_pointer)
{
    FP_TreeNode* newNode = new FP_TreeNode;
    newNode->count = 0;
    newNode->node_value = -1;
    newNode->parent = parent_pointer;
    newNode->marked=false;
    return newNode;
}


string findBinary(int n)
{
    string final = "";
    while(n > 0)
    {
        int t = n%2;
        if(t == 0)
            final += "0";
        else
            final += "1";
        
        n = n>>2;
    }
    return final;
}

void print_Tree(FP_TreeNode *cur)
{
    for(auto i: cur->children)
    {
        cout << "parent value - " << cur->node_value<<" node value - "<<i.second->node_value<<" frequency - "<<i.second->count<<endl;
        print_Tree(i.second);
    }
}

void starter2(vector<pair<set<int>, int> >dataset, int minsup, vector<int>alpha)
{   

    map<int,int>order_map; //to store the frequency of items (singletons)

    set<int>::iterator transaction_itr;

    for(int i = 0; i < dataset.size(); i++)
    {
        set<int>transaction = dataset[i].first;
        for(transaction_itr = transaction.begin(); transaction_itr != transaction.end(); transaction_itr++)
        {
            if(order_map.find(*transaction_itr) == order_map.end())
                order_map[*transaction_itr] = 0;

            order_map[*transaction_itr] +=  dataset[i].second;
        }
    }

    map<int,int>::iterator order_map_itr; //

    vector<pair<int,int> >order; 

    for(order_map_itr = order_map.begin(); order_map_itr != order_map.end(); order_map_itr++)
    {
        if(order_map_itr->second >= minsup)
            order.push_back(make_pair(order_map_itr->second, order_map_itr->first));
    }


    if(order.size() == 0)
        return;

   
    sort(order.rbegin(), order.rend()); //sort on the basis of the frequency of the singletons (descending order)



    map<int,int>elementPosition_in_order; //where a particular item lies in the descending order

    for(int i = 0; i < order.size(); i++)
    {
       elementPosition_in_order[order[i].second] = i;
    }

    vector<set<FP_TreeNode*> >header_table(order.size());
    
    


    FP_TreeNode* root = nullptr;
    if(root == nullptr)
		root = create_TreeNode(nullptr); 

    FP_TreeNode* crawler =  root;

    set<int>::iterator itr;
    for(int t_id = 0; t_id < dataset.size(); t_id++)
    {
        
        set<int>transaction = dataset[t_id].first;
        int cnt = dataset[t_id].second;

        crawler = root;
        for(int i = 0; i < order.size(); i++)
        {
            if(transaction.find(order[i].second) != transaction.end())
            {

                if(crawler->children.find(order[i].second) == crawler->children.end())
                {
                    FP_TreeNode *temp = create_TreeNode(crawler);
                    temp->node_value = order[i].second;
                    crawler->children[order[i].second] = temp;
                    header_table[elementPosition_in_order[(order[i].second)]].insert(temp);
                }

                crawler->children[order[i].second]->count += cnt;
                crawler = crawler->children[order[i].second];
            }
        }
    }


    if(root->children.size() == 0)
        return;

    bool is_single_branch = true;


    crawler =  root;
    while(crawler != nullptr)
    {
        if(crawler->children.size() > 1) //multiple branch
        {
            is_single_branch = false;
            break;
        }
        else if(crawler->children.size() == 0) //single branch finished
        {
            break;
        }
        else
        {
            map<int, FP_TreeNode*>::iterator itr; 

            itr = crawler->children.begin();
            crawler = itr->second;             
        }
    }
    if(is_single_branch) //is singe branch
    {
        vector<int>single_branch_path;


        for(int i = 0; i < order.size(); i++)
        {
            single_branch_path.push_back(order[i].second);
        }

        int num = pow(2,single_branch_path.size());

        vector<int>itemset;

        for(int i = 1; i < num; i++)
        {
            string subsets = findBinary(i);

            for(int i = 0; i < alpha.size(); i++)
            {
                itemset.push_back(alpha[i]);
            }
            for(int i = 0; i < subsets.size(); i++)
            {
                if(subsets[i] == '1')
                    itemset.push_back(single_branch_path[i]);
            }
            frequents_global.push_back(itemset);
            itemset.clear();
        }
        return;
    }
    else
    {
        vector<vector<pair<set<int> , int> > >  dataset_arr(header_table.size());
        for(int i = header_table.size()-1; i >= 0; i--)
        {
            set<FP_TreeNode*>::iterator it=header_table[i].begin();
            while(it!=header_table[i].end())
            {
                if((*it)->marked==true)
                {
                    it++;
                    continue;
                }
                set<int>transaction;
                vector<int>path;
                vector<int>fr;
                vector< FP_TreeNode* > address;
                FP_TreeNode* crawler = *it;


                while(crawler->parent->node_value != -1)
                {
                    path.push_back(crawler->parent->node_value);
                    fr.push_back(crawler->parent->count);
                    address.push_back(crawler->parent);
                    crawler = crawler->parent;
                }
                if(path.size()>0) 
                {
                    transaction.insert(path.back());
                    address.back()->marked=true;
                }
                for(int k=path.size()-2;k>=0;k--)
                {
                    if(!address[k]->marked)
                    {
                    dataset_arr[elementPosition_in_order[path[k]]].push_back(make_pair(transaction,fr[k]));
                    address[k]->marked=true;
                    }
                    transaction.insert(path[k]);
                }
                if(transaction.size() > 0 && (*it)->marked==false)
                    dataset_arr[i].push_back(make_pair(transaction, (*it)->count));
                (*it)->marked=true;
                // for(int k=0;k<address.size();k++)
                // {
                //     if(address[k]->marked)
                //         header_table[elementPosition_in_order[path[k]]].erase(address[k]);
                // }
                it++;
            }
            vector<int>beta;

            for(int j = 0; j < alpha.size(); j++)
            {
                beta.push_back(alpha[j]);
            }
            beta.push_back(order[i].second);
            frequents_global.push_back(beta);
            starter2(dataset_arr[i],minsup,beta); //RECURSION
        }
    }
}

void starter1(vector<pair<set<int>, int> >dataset, int minsup, vector<int>alpha)
{   

    map<int,int>order_map; //to store the frequency of items (singletons)

    set<int>::iterator transaction_itr;

    for(int i = 0; i < dataset.size(); i++)
    {
        set<int>transaction = dataset[i].first;
        for(transaction_itr = transaction.begin(); transaction_itr != transaction.end(); transaction_itr++)
        {
            if(order_map.find(*transaction_itr) == order_map.end())
                order_map[*transaction_itr] = 0;

            order_map[*transaction_itr] +=  dataset[i].second;
        }
    }

    map<int,int>::iterator order_map_itr; //


    vector<pair<int,int> >order; 

    for(order_map_itr = order_map.begin(); order_map_itr != order_map.end(); order_map_itr++)
    {
        if(order_map_itr->second >= minsup)
            order.push_back(make_pair(order_map_itr->second, order_map_itr->first));
    }


    if(order.size() == 0)
        return;

   
    sort(order.rbegin(), order.rend()); //sort on the basis of the frequency of the singletons (descending order)



    map<int,int>elementPosition_in_order; //where a particular item lies in the descending order

    for(int i = 0; i < order.size(); i++)
    {
       elementPosition_in_order[order[i].second] = i;
    }

    vector<set<FP_TreeNode*> >header_table(order.size());
    
    


    FP_TreeNode* root = nullptr;
    if(root == nullptr)
		root = create_TreeNode(nullptr); 

    FP_TreeNode* crawler =  root;

    set<int>::iterator itr;
    for(int t_id = 0; t_id < dataset.size(); t_id++)
    {
        
        set<int>transaction = dataset[t_id].first;
        int cnt = dataset[t_id].second;

        crawler = root;
        for(int i = 0; i < order.size(); i++)
        {
            if(transaction.find(order[i].second) != transaction.end())
            {

                    if(crawler->children.find(order[i].second) == crawler->children.end())
                    {
                        FP_TreeNode *temp = create_TreeNode(crawler);
                        temp->node_value = order[i].second;
                        crawler->children[order[i].second] = temp;
                        header_table[elementPosition_in_order[(order[i].second)]].insert(temp);
                    }

                    crawler->children[order[i].second]->count += cnt;
                    crawler = crawler->children[order[i].second];
                // }
            }
        }
    }


    if(root->children.size() == 0)
        return;

    bool is_single_branch = true;


    crawler =  root;
    while(crawler != nullptr)
    {
        if(crawler->children.size() > 1) //multiple branch
        {
            is_single_branch = false;
            break;
        }
        else if(crawler->children.size() == 0) //single branch finished
        {
            break;
        }
        else
        {
            map<int, FP_TreeNode*>::iterator itr; 

            itr = crawler->children.begin();
            crawler = itr->second;             
        }
    }
    if(is_single_branch) //is singe branch
    {
        vector<int>single_branch_path;


        for(int i = 0; i < order.size(); i++)
        {
            single_branch_path.push_back(order[i].second);
        }

        int num = pow(2,single_branch_path.size());

        vector<int>itemset;

        for(int i = 1; i < num; i++)
        {
            string subsets = findBinary(i);

            for(int i = 0; i < alpha.size(); i++)
            {
                itemset.push_back(alpha[i]);
            }
            for(int i = 0; i < subsets.size(); i++)
            {
                if(subsets[i] == '1')
                    itemset.push_back(single_branch_path[i]);
            }
            frequents_global.push_back(itemset);
            itemset.clear();
        }
        return;
    }
    else
    {
        vector<vector<pair<set<int> , int> > >  dataset_arr(header_table.size());
        for(int i = header_table.size()-1; i >= 0; i--)
        {
            set<FP_TreeNode*>::iterator it=header_table[i].begin();
            while(it!=header_table[i].end())
            {
                set<int>transaction;
                vector<int>path;
                FP_TreeNode* crawler = *it;


                while(crawler->parent->node_value != -1)
                {
                    path.push_back(crawler->parent->node_value);
                    crawler = crawler->parent;
                }
                for(int k=path.size()-1;k>=0;k--)
                {
                    transaction.insert(path[k]);
                }
                if(transaction.size() > 0 && (*it)->marked==false)
                    dataset_arr[elementPosition_in_order[(*it)->node_value]].push_back(make_pair(transaction, (*it)->count));
                // (*it)->marked=true;
                // for(int k=0;k<address.size();k++)
                // {
                //     if(address[k]->marked)
                //         header_table[elementPosition_in_order[path[k]]].erase(address[k]);
                // }
                it++;
            }
            vector<int>beta;

            for(int j = 0; j < alpha.size(); j++)
            {
                beta.push_back(alpha[j]);
            }
            beta.push_back(order[i].second);
            frequents_global.push_back(beta);
            starter1(dataset_arr[i],minsup,beta); ////////////////////////////RECURSION
        }
    }
}
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
    // vector<set<int> >dataset;
    vector<pair<set<int>,int> >dataset;
    set<int>transaction;

    vector<int>sigleton_frequency;
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
        dataset.push_back(make_pair(transaction,1));
        transaction.clear();
    }
    file_data.close();

    vector<int>alpha;
    auto start = high_resolution_clock::now();
    starter1(dataset, minsup, alpha);
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 

    vector<pair<int, vector<int> > >frequent_itemsets_mined;
    int Size,cs;
    vector<int> counts;

    cout <<"~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"<<endl;


    cout << endl << "OUTPUTS GENERATED BY NORMAL FP-GROWTH ALGORITHM" << endl << endl;
    for(int i = 0; i < frequents_global.size(); i++)
    {
        frequent_itemsets_mined.push_back(make_pair(frequents_global[i].size(), frequents_global[i]));
    }

    sort(frequent_itemsets_mined.begin(), frequent_itemsets_mined.end());
    for(int i = 0; i < frequent_itemsets_mined.size();)
    {
        Size=frequent_itemsets_mined[i].first;
        cs=0;
        while(i<frequent_itemsets_mined.size() && frequent_itemsets_mined[i].first==Size)
        {
            cs++;
            i++;
        }
        counts.push_back(cs);
    }
    reverse(counts.begin(),counts.end());
    for(int i = 0; i < frequent_itemsets_mined.size();)
    {
        Size=frequent_itemsets_mined[i].first;
        cout<<"Frequent Itemsets for k = "<<Size<<endl;
        cout<<"Number of Itemsets formed are : "<<counts.back()<<endl<<endl<<"[";
        counts.pop_back();
        while(i<frequent_itemsets_mined.size() && frequent_itemsets_mined[i].first==Size)
        {
            cout<<"[";
            for(int j = 0; j < frequent_itemsets_mined[i].second.size(); j++)
            {
                cout << frequent_itemsets_mined[i].second[j] ;
                if(j!=frequent_itemsets_mined[i].second.size()-1)
                    cout<<",";
            }
            cout << "]";
            i++;
            if(i<frequent_itemsets_mined.size() && frequent_itemsets_mined[i].first==Size)
            cout<<",";
        }
        cout<<"]"<<endl<<endl;
    }
    cout << "Total number of Itemsets Generated are : " << frequent_itemsets_mined.size() << endl << endl << endl;

    cout << "TIME TAKEN : "<< double(duration.count())/1e6 << " sec" << endl;

    cout << endl <<"~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"<<endl;

    alpha.clear();
    counts.clear();
    frequents_global.clear();
    frequent_itemsets_mined.clear();
    start = high_resolution_clock::now();
    starter2(dataset, minsup, alpha);
    stop = high_resolution_clock::now(); 
    duration = duration_cast<microseconds>(stop - start); 
    
    cout << endl << "OUTPUTS GENERATED BY FP-GROWTH ALGORITHM OPTIMIZED WITH MERGING STRATEGY" << endl << endl;    
    
    // cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl<<endl<<endl;

    for(int i = 0; i < frequents_global.size(); i++)
    {
        frequent_itemsets_mined.push_back(make_pair(frequents_global[i].size(), frequents_global[i]));
    }

    sort(frequent_itemsets_mined.begin(), frequent_itemsets_mined.end());
    for(int i = 0; i < frequent_itemsets_mined.size();)
    {
        Size=frequent_itemsets_mined[i].first;
        cs=0;
        while(i<frequent_itemsets_mined.size() && frequent_itemsets_mined[i].first==Size)
        {
            cs++;
            i++;
        }
        counts.push_back(cs);
    }
    reverse(counts.begin(),counts.end());
    for(int i = 0; i < frequent_itemsets_mined.size();)
    {
        Size=frequent_itemsets_mined[i].first;
        cout<<"Frequent Itemsets for k = "<<Size<<endl;
        cout<<"Number of Itemsets formed are : "<<counts.back()<<endl<<endl<<"[";
        counts.pop_back();
        while(i<frequent_itemsets_mined.size() && frequent_itemsets_mined[i].first==Size)
        {
            cout<<"[";
            for(int j = 0; j < frequent_itemsets_mined[i].second.size(); j++)
            {
                cout << frequent_itemsets_mined[i].second[j] ;
                if(j!=frequent_itemsets_mined[i].second.size()-1)
                    cout<<",";
            }
            cout << "]";
            i++;
            if(i<frequent_itemsets_mined.size() && frequent_itemsets_mined[i].first==Size)
            cout<<",";
        }
        cout<<"]"<<endl<<endl;
    }
    cout << "Total number of Itemsets Generated are : " << frequent_itemsets_mined.size() << endl << endl << endl;


    cout << "TIME TAKEN : "<< double(duration.count())/1e6 << " sec" << endl;
    cout << endl <<"~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~"<<endl;

    return 0;
}