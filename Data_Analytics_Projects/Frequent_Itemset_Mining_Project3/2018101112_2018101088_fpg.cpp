#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;

set<int>singletons;


struct FP_TreeNode {    

    int count;
    unordered_map<int, FP_TreeNode*>children;

}FP_TreeNode;


struct FP_TreeNode* create_TreeNode()
{
    struct FP_TreeNode* newNode = new struct FP_TreeNode();
    newNode->count = 0;
    return newNode;
}


void insert(struct FP_TreeNode* root, vector<int>&transaction)
{
    struct FP_TreeNode *crawler = root;
    for(ll item_id = 0; item_id < transaction.size(); item_id++)
    {
        if(crawler->children.find(transaction[item_id]) == crawler->children.end())
            crawler->children[transaction[item_id]] = create_TreeNode();

        crawler = crawler->children[transaction[item_id]];
        crawler->count++;
    }
}


int main(void)
{
    time_t start, end; 
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

    vector<int>sigleton_frequency;
    //Error message for not opening a file
    ifstream file_data(input_file);
    if(!file_data)
    {
        cout << endl << "The given file does not exist!" << endl;
        return -1;
    }
    string single_transaction;
    // set<int>singletons;
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
        dataset.push_back(transaction);
        transaction.clear();
    }
    file_data.close();

    struct FP_TreeNode* root = NULL;

    vector<int>t;

    time(&start); 
    insert(root, t);
    time(&end); 

    double time_taken = double(end - start); 
    cout << endl << endl << "Time taken by the FP Growth Algorithm is : " << fixed 
         << time_taken << setprecision(5); 
    cout << " sec " << endl; 

    return 0;
}