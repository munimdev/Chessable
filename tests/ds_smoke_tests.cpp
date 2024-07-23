#include <cstdlib>
#include <iostream>
#include <utility>

#include "AVL.h"
#include "Hash.h"
#include "List.h"

void expect(bool condition, const char *message)
{
    if (!condition)
    {
        std::cerr << "Test failure: " << message << std::endl;
        std::exit(1);
    }
}

void test_avl_insert_search_remove()
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);
    tree.insert(10);

    expect(tree.search(10), "AVL should find inserted key 10");
    expect(tree.search(5), "AVL should find inserted key 5");
    expect(tree.search(15), "AVL should find inserted key 15");
    expect(!tree.search(999), "AVL should not find missing key");

    tree.remove(5);
    expect(!tree.search(5), "AVL remove should delete key 5");
    expect(tree.search(10), "AVL should still contain key 10 after removing 5");
    expect(tree.search(15), "AVL should still contain key 15 after removing 5");

    tree.remove(12345);
    expect(tree.search(10), "AVL remove of missing key should not break tree");
}

void test_hash_insert_lookup_and_rehash()
{
    HashTable<int> table(3);

    table.doubleHashInsert("alpha", 1);
    table.doubleHashInsert("alpha", 2);
    table.doubleHashInsert("beta", 3);
    table.doubleHashInsert("gamma", 4);
    table.doubleHashInsert("delta", 5);

    const int alphaIndex = table.getIndex("alpha");
    const int betaIndex = table.getIndex("beta");
    const int gammaIndex = table.getIndex("gamma");
    const int deltaIndex = table.getIndex("delta");

    expect(alphaIndex != -1, "Hash should find alpha");
    expect(betaIndex != -1, "Hash should find beta");
    expect(gammaIndex != -1, "Hash should find gamma");
    expect(deltaIndex != -1, "Hash should find delta");
    expect(table.getIndex("missing") == -1, "Hash should return -1 for missing key");

    expect(table.hashTable[alphaIndex].list.size == 2, "Hash should group duplicate key values");
}

void test_list_iterator_and_rule_of_five()
{
    LinkedList<int> numbers;
    numbers.insert(1);
    numbers.insert(2);
    numbers.insert(3);

    auto it = numbers.begin();
    expect(*it == 1, "List begin should point at first value");

    auto old = it++;
    expect(*old == 1, "List post-increment should return previous iterator");
    expect(*it == 2, "List iterator should advance on post-increment");

    ++it;
    expect(*it == 3, "List iterator should advance on pre-increment");

    auto copy = numbers;
    expect(copy.size == numbers.size, "List copy constructor should preserve size");
    expect(copy.searchData(2), "List copy constructor should preserve values");

    LinkedList<int> moved = std::move(copy);
    expect(moved.searchData(1), "List move should preserve value 1");
    expect(moved.searchData(2), "List move should preserve value 2");
    expect(moved.searchData(3), "List move should preserve value 3");
    expect(copy.size == 0, "Moved-from list should be empty");

    auto eraseIt = moved.begin();
    ++eraseIt;
    eraseIt = moved.erase(eraseIt);
    expect(*eraseIt == 3, "List erase should return iterator to next element");
    expect(!moved.searchData(2), "List erase should remove selected value");
}

int main()
{
    test_avl_insert_search_remove();
    test_hash_insert_lookup_and_rehash();
    test_list_iterator_and_rule_of_five();

    std::cout << "All DS smoke tests passed." << std::endl;
    return 0;
}
