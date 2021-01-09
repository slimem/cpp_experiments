#include <bits/stdc++.h>

using namespace std;

template<typename K, typename V>
class range_map {

public:
    std::map<K,V> _map;
    
    // constructor associates whole range of K with val by inserting (K_min, val) into the map
    range_map( V const& val) {
        _map.insert(_map.end(),std::make_pair(std::numeric_limits<K>::lowest(),val));
    }
    

    // I first implemented and tested the following method which is not optimal,
    // then I used it to compare it to my optimal implementation
    void insert_gold( K const& keyBegin, K const& keyEnd, V const& val ) {
        
        //V valEnd = (*this)[keyEnd];

        if(keyBegin<keyEnd){
            V valEnd = (*this)[keyEnd];
            auto itEnd = _map.insert({keyEnd, valEnd});
            if(!itEnd.second) itEnd.first->second = valEnd;
            auto itBegin = _map.insert({keyBegin, val});
            if(!itBegin.second) itBegin.first->second = val;
            if(itBegin.first != _map.begin()){
                V valBegin = (--itBegin.first)->second;
                ++itBegin.first;
                if(valBegin == val) _map.erase(itBegin.first, itEnd.first);
                else _map.erase(++itBegin.first, itEnd.first);
            }
            else{
                _map.erase(++itBegin.first, itEnd.first);
            }
            if(valEnd == val) _map.erase(itEnd.first);
    }

    // This is the most optimal implementation that I can come up with (2 * logN)
    void insert( K const& keyBegin, K const& keyEnd, V const& val ) {
        if (keyBegin < keyEnd) { //O(1)
            auto itEnd = --_map.upper_bound(keyEnd); //O(logN)
            if (!(itEnd->second == val)) { //O(1)
                itEnd = _map.emplace_hint(itEnd, keyEnd, itEnd->second); //O(1) (amortized)
            }
            auto itBegin = --_map.upper_bound(keyBegin); //O(logN)
            auto it1 = itBegin; //O(1)
            if (itBegin != itEnd) { //O(1)
                _map.erase(++itBegin, itEnd); //O(it1-it2-1)
                if ((itEnd->second == val)) _map.erase(itEnd); //O(1) 
            }
            if (!(it1->second == val)) { //O(1)
                if (it1 != _map.begin()) { //O(1)
                    --it1; //O(1)
                    auto it2 = ++it1; //O(1)
                    if ( (!(it2->first < keyBegin)) && ((--it1)->second == val)) { //O(1)
                        _map.erase(++it1); //O(1)
                    } else {
                        _map.emplace_hint(++it1, keyBegin, val)->second = val;  //O(1)
                    }
                } else {
                    _map.emplace_hint(it1, keyBegin, val)->second = val; //O(1)
                }
            }
        }
    }

    // find the value associated with key K
    // we prepend it with -- because upper_bound() returns the iterator+1.
    // It returns end() if the key wasnt found
    V const& operator[]( K const& key ) const {
        return (--_map.upper_bound(key))->second;
    }
};

int main() {

    // The following is some testing code that inserts random ranges inside
    // two range_maps (the first using the gold inserter and the second the fast one)

    bool fine = true;
    long tt; 
    do {        
    srand (time(NULL));
    cout << time(NULL) << endl;
    tt = time(NULL);
    char construct = rand() % 26 + 97;
    //char construct2 = rand() % 26 + 97;
    range_map<unsigned int,char> m_perfect(construct);
    range_map<unsigned int,char> m(construct);

    for(int i=0;i<100000;i++){
        unsigned int a = rand() % 400;
        unsigned int b = rand() % 400;
        char d = rand() % 26 + 97;
        //cout << a << " " << b << " " << d << endl;
        m.insert(a,b,d);
        m_perfect.insert_gold(a,b,d);
        //for(auto u: m._map){
        //    cout<<u.first<<" "<<u.second<<"\n";
        //}
        //Test equality
        if (!(m._map == m_perfect._map)) {
            cout << "ERROR: SEQUENCE NOT EQUAL AT (" << i << ")\n";
            exit(EXIT_FAILURE);
        }
        
        
        char x = '0';
        for(auto u: m._map){
            char y = u.second;
            if (x==y) {
                for(auto u: m._map){
                 cout<<u.first<<" "<<u.second<<"\n";
                }
                fine = false;
                cout << "ERROR\n\n";
                goto LABEL;
                //exit(EXIT_FAILURE);
            }
            x=y;
        }
        if (!fine) goto LABEL;
    }

    } while (true);
    LABEL : 
    cout << "TT = " << tt << " IS FAILING\n";

    return 0;
}

