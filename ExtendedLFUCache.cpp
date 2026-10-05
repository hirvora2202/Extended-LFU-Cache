#include <iostream>
#include <unordered_map>    
#include <unordered_set>
#include <stdexcept>
using namespace std;
int n;
// items in each frequency value
struct FrequencyNode
{
    int value;
    unordered_set<int> items;
};

// structure for each request coming into cache
struct LFUItem
{
    int data;
    int frequency;
    bool inCache;
};

// cache structure
struct LFUCache
{
    unordered_map<int, LFUItem *> bykey;
    unordered_map<int, FrequencyNode *> freqMap;
    FrequencyNode *minFreq;
    int size;
    int capacity;
};

// create structuer for a frequency with no items in it
FrequencyNode *NewFreqNode()
{
    FrequencyNode *o = new FrequencyNode();
    o->value = 0;
    o->items = unordered_set<int>{};
    return o;
}

// this function creates struc for data with its freq and incache=true
LFUItem *NewLFUItem(int data, int frequency)
{
    LFUItem *o = new LFUItem();
    o->data = data;
    o->frequency = frequency;
    o->inCache = true;
    return o;
}

// function to create new cache object wih given capacity and bykey for lfu items and freqmap for storing freq and their items
LFUCache *NewLFUCache(int capacity)
{
    LFUCache *o = new LFUCache();
    o->bykey = unordered_map<int, LFUItem *>{};
    o->freqMap = unordered_map<int, FrequencyNode *>{};
    o->minFreq = NULL;
    o->capacity = capacity;
    o->size = 0;
    return o;
}
// create lfu_cache object for cache structure with size n
LFUCache *lfu_cache;

// this function create new frequency node with value as frequency and add freq in lfucache's freqmap
FrequencyNode *GetNewNode(int value)
{
    FrequencyNode *nn = NewFreqNode();
    nn->value = value;
    lfu_cache->freqMap[value] = nn;
    return nn;
}

// delete a freq from freqmap
void DeleteNode(FrequencyNode *node)
{
    lfu_cache->freqMap.erase(node->value);
}

// to access a key from
int Access(int key)
{
    LFUItem *tmp = lfu_cache->bykey[key];
    if (tmp == NULL || !tmp->inCache)
    {
        throw runtime_error("No Such Key");
    }
    FrequencyNode *freq = lfu_cache->freqMap[tmp->frequency];
    FrequencyNode *next_freq = NULL;
    auto next_freq_pointer = lfu_cache->freqMap.find(tmp->frequency + 1);
    // create new freq in freqmap
    if (next_freq_pointer == lfu_cache->freqMap.end())
        next_freq = GetNewNode(freq->value + 1);
    else
        next_freq = next_freq_pointer->second;
    // add key to new freq in freqmap and erase from old freq in freqmap
    next_freq->items.insert(key);
    tmp->frequency = next_freq->value;
    freq->items.erase(key);
    if (freq->items.size() == 0)
    {
        if (freq == lfu_cache->minFreq)
            lfu_cache->minFreq = next_freq;
        DeleteNode(freq);
    }
    return tmp->data;
}

void Insert(int key, int value)
{
    // if key is in data and still we are trying to insert
    if (lfu_cache->bykey.find(key) != lfu_cache->bykey.end() && lfu_cache->bykey[key]->inCache)
    {
        throw runtime_error("Key Already Exists");
    }
    // if cache is full ,then delete key whose freq is minimum, see minfreq, a pointer to freqmap
    if (lfu_cache->size == lfu_cache->capacity)
    {
        int deletedKey = *(lfu_cache->minFreq->items.begin());
        lfu_cache->bykey[deletedKey]->inCache = false;
        lfu_cache->minFreq->items.erase(deletedKey);
        // if no items of a freq then delete that freq from freqmap
        if (lfu_cache->minFreq->items.size() == 0)
            DeleteNode(lfu_cache->minFreq);
        lfu_cache->size -= 1;
    }
    // if key is not in cache but was previously in cache
    if (lfu_cache->bykey.find(key) != lfu_cache->bykey.end())
    {
        LFUItem *tmp = lfu_cache->bykey[key];
        FrequencyNode *freq = NULL;
        // add this key to freqmap
        if (lfu_cache->freqMap.find(tmp->frequency + 1) != lfu_cache->freqMap.end())
        {
            freq = lfu_cache->freqMap[tmp->frequency + 1];
        }
        // create new freq node if freq not present in freqmap
        else
        {
            freq = GetNewNode(tmp->frequency + 1);
        }
        // update minfreq pointer
        if (freq->value < lfu_cache->minFreq->value)
            lfu_cache->minFreq = freq;
        // change minfreq pointer to another freq
        else if (lfu_cache->minFreq->items.size() == 0)
        {
            int i = 1;
            while (lfu_cache->freqMap.find(lfu_cache->minFreq->value + i) == lfu_cache->freqMap.end())
            {
                i++;
            }
            lfu_cache->minFreq = lfu_cache->freqMap[lfu_cache->minFreq->value + i];
        }
        // insert key into the freqmap
        freq->items.insert(key);
        tmp->inCache = true;
        tmp->frequency += 1;
    }
    else
    {
        FrequencyNode *freq = lfu_cache->minFreq;
        if (freq == NULL || freq->value != 1)
        {
            freq = GetNewNode(1);
            // minfreq points to freq node and also freqmap
            lfu_cache->minFreq = freq;
        }
        else if (lfu_cache->freqMap.find(lfu_cache->minFreq->value) == lfu_cache->freqMap.end())
        {
            lfu_cache->freqMap[lfu_cache->minFreq->value] = lfu_cache->minFreq;
        }
        // insert key into freqMAP
        freq->items.insert(key);
        // create lfu item for key
        lfu_cache->bykey[key] = NewLFUItem(value, freq->value);
    }
    lfu_cache->size += 1;
}

void printcache()
{
    if (lfu_cache->size == 0)
        return;
    cout << "the contents of cache are " << endl;
    for (auto x : lfu_cache->bykey)
    {
        if (x.second->inCache)
            cout << x.first << " " << x.second->data << endl;
    }
    cout << "contents of freq map are " << endl;
    for (auto x : lfu_cache->freqMap)
    {
        cout << x.first << " " << "[";
        for (auto item : x.second->items)
        {
            cout << item << " ";
        }
        cout << "]" << endl;
    }
}

int main()
{
    int i, key, data;
    cout << "enter size of cache " << endl;
    cin >> n;
    lfu_cache = NewLFUCache(n);
    while (1)
    {
        cout << "select 1.Insertion 2.Access 3.Print" << endl;
        cin >> i;
        if (i == 1)
        {
            cout << "enter key and data " << endl;
            cin >> key >> data;
            Insert(key, data);
            printcache();
        }
        if (i == 2)
        {
            cout << "enter key to access " << endl;
            cin >> key;
            Access(key);
            printcache();
        }
    }
}