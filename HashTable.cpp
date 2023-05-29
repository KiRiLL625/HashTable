//
// Created by user on 28.05.2023.
//

#include "HashTable.h"

HashTable::HashTable(size_t size) noexcept{
    _capacity = size;
    _filled = 0;
    table.resize(_capacity);
}

HashTable::~HashTable(){
    table.clear();
}

void HashTable::insert(const KeyType &key, const ValueType &value){
    size_t index = hash_function(key);
    for (auto & it : table[index]){
        if (it.first == key){
            it.second = value;
            return;
        }
    }
    table[index].emplace_back(key, value);
    ++_filled;
    if (getLoadFactor() > 0.75){
        _capacity *= 2;
        std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(_capacity);
        for (auto &i : table){
            for (auto &j : i){
                size_t index = hash_function(j.first);
                new_table[index].push_back(j);
            }
        }
        table = new_table;
    }
}

bool HashTable::find(const KeyType &key, ValueType &value) const{
    size_t index = hash_function(key);
    for (const auto & it : table[index]){
        if (it.first == key){
            value = it.second;
            return true;
        }
    }
    return false;
}

void HashTable::remove(const KeyType &key){
    size_t index = hash_function(key);
    for (auto it = table[index].begin(); it != table[index].end(); ++it){
        if (it->first == key){
            table[index].erase(it);
            --_filled;
            return;
        }
    }
}

ValueType& HashTable::operator[](const KeyType &key){
    size_t index = hash_function(key);
    for (auto & it : table[index]){
        if (it.first == key){
            return it.second;
        }
    }
    table[index].emplace_back(key, ValueType());
    ++_filled;
    if (getLoadFactor() > 0.75){
        _capacity *= 2;
        std::vector<std::list<std::pair<KeyType, ValueType>>> new_table(_capacity);
        for (auto &i : table){
            for (auto &j : i){
                size_t index = hash_function(j.first);
                new_table[index].push_back(j);
            }
        }
        table = new_table;
    }
    return table[index].back().second;
}

double HashTable::getLoadFactor(){
    return static_cast<double>(_filled) / _capacity;
}

size_t HashTable::hash_function(const KeyType &key) const{
    size_t hash = 0;
    for (auto &i : key){
        hash = (hash * 31 + i) % _capacity;
    }
    return hash;
}