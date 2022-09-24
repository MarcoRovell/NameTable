// NameTable.cpp

// This is a correct but inefficient implementation of
// the NameTable functionality.

#include "NameTable.h"
#include <string>
#include <vector>
#include <list>
#include <functional>
using namespace std;

// This class does the real work of the implementation.

const int maxBuckets = 20000;

class NameTableImpl
{
public:
    NameTableImpl();
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    struct node 
    {
        int line = -1;
        string name = "";
    };
    unsigned int mapFunc(const string& id) const;
    vector<list<node>> hashTable;
    vector<string> currentScope;
};

NameTableImpl::NameTableImpl() 
{
    hashTable.resize(maxBuckets);
}

unsigned int NameTableImpl::mapFunc(const string& hashMe) const
{
    hash<string> str_hash;
    unsigned int hashValue = str_hash(hashMe);
    unsigned int bucketNum = hashValue % maxBuckets;
    return bucketNum;
}

void NameTableImpl::enterScope()
{
    currentScope.push_back("");
}

bool NameTableImpl::exitScope()
{
    while (!currentScope.empty() && currentScope.back() != "")
    {
        size_t index = mapFunc(currentScope.back());
        hashTable[index].pop_front();
        currentScope.pop_back();
    }
    if (currentScope.empty()) { return false; } 
    currentScope.pop_back(); // pops back the empty string
    return true;
}

bool NameTableImpl::declare(const string& id, int lineNum)
{
    if (id.empty()) { return false; }
    for (size_t i = currentScope.size(); i > 0;)
    {
        i--;
        if (currentScope[i].empty()) { break; }
        if (currentScope[i] == id) { return false; }
    }
    currentScope.push_back(id);

    size_t index = mapFunc(id);
    node b;
    b.line = lineNum;
    b.name = id;
    hashTable[index].push_front(b);
    return true;
}

int NameTableImpl::find(const string& id) const
{
    if (id.empty()) { return -1; }
    size_t index = mapFunc(id);
    for (list<node>::const_iterator i = hashTable[index].begin(); i != hashTable[index].end(); i++) 
    {
        if ((*i).name == id) 
        {
            return (*i).line;
        }
    }
    return -1;
}

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}