#include <iostream>
#include <string>
#include <sstream>
#include "Comps.hpp"

template <class TKey, class TElement>
class BTree;

template <class TKey, class TElement>
class BNode
{
private:
    TKey* keys;
    TElement* values;
    BNode<TKey, TElement>** children;
    int count;
    int order;
    bool leaf;
    bool (*comp)(TKey, TKey);
    bool compare(TKey firstValue, TKey secondValue)
    {
        return (*comp)(firstValue, secondValue);
    }
public:
    friend class BTree<TKey, TElement>;

    BNode<TKey, TElement>(int temp, bool boolLeaf)
    {
        this->count = temp;
        this->leaf = boolLeaf;
        this->keys = new TKey[2 * temp - 1];
        this->values = new TElement[2 * temp - 1];
        this->children = new BNode*[2 * temp];
        this->order = 0;
    }

    BNode<TKey, TElement>(int temp, bool (*comp1)(TKey first, TKey second))
    {
        this->count = temp;
        this->leaf = true;
        this->keys = new TKey[2 * temp - 1];
        this->values = new TElement[2 * temp - 1];
        this->children = new BNode*[2 * temp];
        this->order = 0;
        this->comp = comp1;
    }

    BNode<TKey, TElement>(int temp, bool boolLeaf, bool (*comp1)(TKey first, TKey second))
    {
        this->count = temp;
        this->leaf = boolLeaf;
        this->keys = new TKey[2 * temp - 1];
        this->values = new TElement[2 * temp - 1];
        this->children = new BNode*[2 * temp];
        this->order = 0;
        this->comp = comp1;
    }

    BNode<TKey, TElement>(TKey key, TElement value, int temp, bool (*comp1)(TKey first, TKey second))
    {
        this->count = temp;
        this->leaf = true;
        this->keys = new TKey[2 * temp - 1];
        this->keys[0] = key;
        this->values = new TElement[2 * temp - 1];
        this->values[0] = value;
        this->children = new BNode*[2 * temp];
        this->order = 1;
        this->comp = comp1;
    }

    void insertNonFull(TKey key, TElement value)
    {
        int i = order - 1;

        if (leaf == true)
        {
            while ((i >= 0) && compare(keys[i], key))
            {
                keys[i + 1] = keys[i];
                values[i + 1] = values[i];
                i--;
            }
            keys[i + 1] = key;
            values[i + 1] = value;
            order += 1;
        }
        else
        {
            while (i >= 0 && compare(keys[i], key))
                i--;

            if (children[i + 1]->order == 2 * count - 1)
            {
                splitChild(i + 1, children[i + 1]);
                if (compare(key, keys[i + 1]))
                {
                    i++;
                }
            }
            children[i + 1]->insertNonFull(key, value);
        }
    }

    void splitChild(int i, BNode<TKey, TElement>* temp)
    {
        BNode* current = new BNode(temp->count, temp->leaf, temp->comp);
        current->order = count - 1;
        for (int j = 0; j < count - 1; j++)
        {
            current->keys[j] = current->keys[j + count];
            current->values[j] = current->values[j + count];
        }
        if (temp->leaf == false)
        {
            for (int j = 0; j < count; j++)
            {
                current->children[j] = temp->children[j + count];
            }
        }
        temp->order = count - 1;
        for (int j = order; j >= i + 1; j--)
        {
            children[j + 1] = children[j];
        }
        children[i + 1] = current;
        for (int j = order - 1; j >= i; j--)
        {
            keys[j + 1] = keys[j];
            values[j + 1] = values[j];
        }
        keys[i] = temp->keys[count - 1];
        values[i] = temp->values[count - 1];
        order += 1;
    }

    BNode<TKey, TElement>* search(TKey key)
    {
        int i = 0;
        while (i < order && key > keys[i])
        {
            i++;
        }
        if (key == keys[i])
        {
            return this;
        }
        if (leaf == true)
        {
            return nullptr;
        }
        return children[i]->search(key);
    }

    TElement getValue(TKey key)
    {
        int i = 0;
        while (this->keys[i] != key)
        {
            i++;
        }
        return this->values[i];
    }

    void update(TKey key, TElement value)
    {
        int i = 0;
        while (key != keys[i])
        {
            i++;
        }
        values[i] = value;
    }

    void traverse(TElement (*fun)(TElement element))
    {
        int i;
        for (i = 0; i < order; i++)
        {
            if (!leaf)
            {
                children[i]->traverse(fun);
            }
            values[i] = fun(values[i]);
        }
        if (!leaf)
        {
            children[i]->traverse(fun);
        }
    }

    std::string toStringPair() {
        std::ostringstream  ans;
        int i;
        for (i = 0; i < order; i++) {
            if (!leaf)
                ans << children[i]->toStringPair();
            ans << "[" << keys[i].first + 1 << "] [" << keys[i].second + 1 << "] -> " << values[i] << "\n";
        }

        if (!leaf)
            ans << children[i]->toStringPair();
        return ans.str();
    }
    std::string toStringSeq()
    {
        std::ostringstream  ans;
        int i;
        for (i = 0; i < order; i++) {
            if (!leaf)
                ans << children[i]->toStringSeq();
            ans << "(" << keys[i] << " -> ";
            for (int j = 0; j < values[i]->GetLength(); j++) {
                ans << values[i]->Get(j) << " ";
            }
            ans << ")\n";
        }
        if (!leaf)
            ans << children[i]->toStringSeq();
        return ans.str();
    }

    std::string toStringStr()
    {
        std::ostringstream ans;
        int i;
        for (i = 0; i < order; i++) {
            if (!leaf)
                ans << children[i]->toStringStr();
            ans << "(" << keys[i] << " -> "<< values[i] << ") \n";
        }
        if (!leaf)
        {
            ans << children[i]->toStringStr();
        }
        return ans.str();
    }

    int findKey(int key)
    {
        int index = 0;
        while ((index < order) && (keys[index] < key))
        {
            index++;
        }
        return index;
    }

    void deletion(TKey k)
    {
        int index = this->findKey(k);
        if (index < order && keys[index] == k)
        {
            if (leaf)
                removeFromLeaf(index);
            else
                removeFromNonLeaf(index);
        }
        else
        {
            if (leaf)
                return;
            bool flag = ((index == order) ? true : false);
            if (children[index]->n < count)
                fill(index);
            if ((flag && index) > order)
                children[index - 1]->deletion(k);
            else
                children[index]->deletion(k);
        }
        return;
    }

    void removeFromLeaf(int index)
    {
        for (int i = index + 1; i < order; ++i)
        {
            keys[i - 1] = keys[i];
            values[i - 1] = values[i];
        }
        order--;
        return;
    }

    void removeFromNonLeaf(int index)
    {
        int k = keys[index];

        if (children[index]->order >= count)
        {
            int pred = getPredecessor(index);
            keys[index] = pred;
            children[index]->deletion(pred);
        }
        else if (children[index + 1]->order >= count)
        {
            TKey succ = getSuccessor(index);
            keys[index] = succ;
            children[index + 1]->deletion(succ);
        }

        else
        {
            merge(index);
            children[index]->deletion(k);
        }
        return;
    }

    TKey getPredecessor(int index)
    {
        BNode<TKey, TElement>* cur = children[index];
        while (!cur->leaf)
            cur = cur->children[cur->order];
        return cur->keys[cur->order - 1];
    }

    TKey getSuccessor(int index)
    {
        BNode<TKey, TElement>* cur = children[index + 1];
        while (!cur->leaf)
            cur = cur->children[0];
        return cur->keys[0];
    }

    void fill(int index)
    {
        if (index != 0 && children[index - 1]->order >= count)
            borrowFromPrev(index);
        else if (index != order && children[index + 1]->order >= count)
            borrowFromNext(index);
        else
        {
            if (index != order)
                merge(index);
            else
                merge(index - 1);
        }
        return;
    }

    void borrowFromPrev(int index)
    {
        BNode<TKey, TElement>* child = children[index];
        BNode<TKey, TElement>* sibling = children[index - 1];
        for (int i = child->order - 1; i >= 0; --i)
            child->keys[i + 1] = child->keys[i];
        if (!child->leaf)
        {
            for (int i = child->order; i >= 0; --i)
                child->children[i + 1] = child->children[i];
        }
        child->keys[0] = keys[index - 1];

        if (!child->leaf)
            child->children[0] = sibling->children[sibling->order];

        keys[index - 1] = sibling->keys[sibling->order - 1];
        child->order += 1;
        sibling->order -= 1;
        return;
    }

    void borrowFromNext(int index)
    {
        BNode<TKey, TElement>* child = children[index];
        BNode<TKey, TElement>* sibling = children[index + 1];

        child->keys[(child->order)] = keys[index];

        if (!(child->leaf))
            child->children[(child->order) + 1] = sibling->children[0];

        keys[index] = sibling->keys[0];

        for (int i = 1; i < sibling->order; ++i)
            sibling->keys[i - 1] = sibling->keys[i];

        if (!sibling->leaf)
        {
            for (int i = 1; i <= sibling->order; ++i)
                sibling->children[i - 1] = sibling->children[i];
        }

        child->order += 1;
        sibling->order -= 1;

        return;
    }

    void merge(int index)
    {
        BNode<TKey, TElement>* child = children[index];
        BNode<TKey, TElement>* sibling = children[index + 1];

        child->keys[count - 1] = keys[index];

        for (int i = 0; i < sibling->order; ++i)
            child->keys[i + count] = sibling->keys[i];

        if (!child->leaf) {
            for (int i = 0; i <= sibling->order; ++i)
                child->children[i + count] = sibling->children[i];
        }

        for (int i = index + 1; i < order; ++i)
            keys[i - 1] = keys[i];

        for (int i = index + 2; i <= order; ++i)
            children[i - 1] = children[i];

        child->order += sibling->order + 1;
        order--;

        delete (sibling);
        return;
    }
};

template <class TKey, class TElement>
class BTree
{
private:
    BNode<TKey, TElement>* root;
    int t;
public:
    BTree<TKey, TElement>(int temp)
    {
        root = nullptr;
        t = temp;
    }

    BTree<TKey, TElement>(int temp, bool (*comp1)(TKey first, TKey second))
    {
        root = new BNode<TKey, TElement>(temp, comp1);
        t = temp;
    }

    BTree<TKey, TElement>(TKey key, TElement element, int temp, bool (*comp1)(TKey first, TKey seond))
    {
        root = new BNode<TKey, TElement>(key, element, temp, comp1);
        t = temp;
    }

    BNode<TKey, TElement>* getNode()
    {
        return this->root;
    }

    void traverse(TElement(*fun)(TElement element))
    {
        if (this != nullptr)
            root->traverse(fun);
    }

    BNode<TKey, TElement>* search(TKey key)
    {
        if (this != nullptr)
            return root->search(key);
        else
            return nullptr;
    }

    void insert(TKey key, TElement value)
    {
        if (root == nullptr)
        {
            root = new BNode<TKey, TElement>(t, true);
            root->values[0] = value;
            root->keys[0] = key;
            root->order = 1;
        }
        else
        {
            if (root->order == 2 * t - 1) {     // if node is full
                BNode<TKey, TElement>* s = new BNode<TKey, TElement>(t, false, root->comp);
                s->order = 0;
                s->children[0] = root;
                s->splitChild(0, root);
                int i = 0;

                if (this->root->comp(key, s->keys[0]))
                    i++;
                s->children[i]->insertNonFull(key, value);

                root = s;
            }
            else
                root->insertNonFull(key, value);
        }
    }

    void update(TKey key, TElement element)
    {
        BNode<TKey, TElement>* node = this->search(key);
        node->update(key, element);
    }

    void deletion(TKey key)
    {
        if (!root)
        {
            return;
        }
        root->deletion(key);
        if (root->order == 0) {
            BNode<TKey, TElement>* cur = root;
            if (root->leaf)
                root = nullptr;
            else
                root = root->children[0];

            delete cur;
        }
        return;
    }
};