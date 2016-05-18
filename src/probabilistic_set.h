//
//  probabilistic_set.h
//  bank_request
//
//  Created by Pierre Peterlongo on 01/03/16.
//
//

#ifndef probabilistic_set_h
#define probabilistic_set_h
using namespace std;

#include <iostream>
#include "native_bit_vector_array.h"

using namespace std;



class probabilisticSet {
public:
	probabilisticSet(){}
    probabilisticSet(const uint64_t nb_elements, const int fingerprint_size): _nb_elements(nb_elements), _fingerprint_size(fingerprint_size){
        //cout << fingerprint_size << endl;
    	_bas = bitArraySet(nb_elements,fingerprint_size);
        _fingerprint_range = (uint64_t)1<<_fingerprint_size;
    }
    
    void add(const uint64_t i, const uint64_t key){
        uint64_t fingerprint = korenXor(key)%_fingerprint_range;
        _bas.set_i(i,fingerprint);
    }
    
    bool exists(const uint64_t i, const uint64_t key) const{
        const uint64_t fingerprint = korenXor(key)%_fingerprint_range;
        const uint64_t stored_fingerprint = _bas.get_i(i);
        if (fingerprint == stored_fingerprint) return true;
        return false;
    }
    
	void save(std::ostream& os) const
	{
		os.write(reinterpret_cast<char const*>(&_fingerprint_size), sizeof(_fingerprint_size));
		os.write(reinterpret_cast<char const*>(&_fingerprint_range), sizeof(_fingerprint_range));
		os.write(reinterpret_cast<char const*>(&_nb_elements), sizeof(_nb_elements));
		_bas.save(os);
	}

	void load(std::istream& is)
	{
		is.read(reinterpret_cast<char*>(&_fingerprint_size), sizeof(_fingerprint_size));
		is.read(reinterpret_cast<char*>(&_fingerprint_range), sizeof(_fingerprint_range));
		is.read(reinterpret_cast<char*>(&_nb_elements), sizeof(_nb_elements));

		//cout << _fingerprint_size << endl;
		//cout << _fingerprint_range << endl;
		//cout << _nb_elements << endl;
		_bas.load(is);
	}

private:
    
    inline uint64_t korenXor(uint64_t x)const{
        x ^= (x << 21);
        x ^= (x >> 35);
        x ^= (x << 4);
        return x;
    }
    
    bitArraySet _bas;
    uint64_t _nb_elements;
    int _fingerprint_size;
    uint64_t _fingerprint_range;
};



#endif
