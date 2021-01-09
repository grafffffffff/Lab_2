#include "BTree.hpp"#include <iostream>const int DEGREE = 3;template <class TKey, class TElement>class IDictionary{public:    BTree<TKey, TElement>* dictionary;    int count;    IDictionary()    {        BTree<TKey, TElement>* dictionary = new BTree<TKey, TElement>(DEGREE);        count = 0;    }    IDictionary(TKey key, TElement element, bool (*comp)(TKey first, TKey second))    {        dictionary = new BTree<TKey, TElement>(key, element, DEGREE, comp);        count = 1;    }    IDictionary(bool (*comp)(TKey first, TKey second))    {        dictionary = new BTree<TKey, TElement>(DEGREE, comp);        count = 0;    }    int getCount()    {        return this->count;    }    TElement get(TKey key)    {        BNode<TKey, TElement>* node = dictionary->search(key);        return node->getValue(key);    }    bool containsKey(TKey key)    {        if (dictionary->search(key) != nullptr)            return true;        else            return false;    }    void add(TKey key, TElement element)    {        dictionary->insert(key, element);        count++;    }    void update(TKey key, TElement element)    {        (dictionary->search(key))->update(key, element);    }    std::string toStringSeq()    {        return this->dictionary->getNode()->toStringSeq();    }    std::string toStringPair()    {        return this->dictionary->getNode()->toStringPair();    }    void remove(TKey key)    {        dictionary->deletion(key);    }};