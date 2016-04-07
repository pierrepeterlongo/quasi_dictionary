#ifndef QUASIDICTIONNARY_H
#define QUASIDICTIONNARY_H


//#include "IteratorGzMPHF.hpp"
//#include "../BooPHF/BooPHF.h"
#include <iostream>
#include "native_bit_vector_array.h"
#include "probabilistic_set.h"

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<  u_int64_t, hasher_t  > boophf_t;



template <class T,class U>
class MyIterator : public std::iterator<std::input_iterator_tag, int>
{
  tuple<T,U>* _tuples ;
public:
  MyIterator(tuple<T,U>* tuples) :_tuples(tuples) {}
  MyIterator(const MyIterator& mit) : _tuples(mit._tuples) {}
  MyIterator& operator++() {++_tuples;return *this;}
  MyIterator operator++(int) {MyIterator tmp(*this); operator++(); return tmp;}
  bool operator==(const MyIterator& rhs) {return _tuples==rhs._tuples;}
  bool operator!=(const MyIterator& rhs) {return _tuples!=rhs._tuples;}
  T& operator*() {return get<0>(*_tuples);}
};




// iterator from disk file of T with buffered read
template <class T>
class bfile_iterator_first : public std::iterator<std::forward_iterator_tag, T>{
public:
	
	bfile_iterator_first()
	: _is(nullptr)
	, _pos(0) ,_inbuff (0), _cptread(0)
	{
		_buffsize = 10000;
		_buffer = (T *) malloc(_buffsize*sizeof(T));
	}
	
	bfile_iterator_first(const bfile_iterator_first& cr)
	{
		_buffsize = cr._buffsize;
		_pos = cr._pos;
		_is = cr._is;
		_buffer = (T *) malloc(_buffsize*sizeof(T));
		memcpy(_buffer,cr._buffer,_buffsize*sizeof(T) );
		_inbuff = cr._inbuff;
		_cptread = cr._cptread;
		_elem = cr._elem;
	}
	
	bfile_iterator_first(FILE* is): _is(is) , _pos(0) ,_inbuff (0), _cptread(0)
	{
		_buffsize = 10000;
		_buffer = (T *) malloc(_buffsize*sizeof(T));
		int reso = fseek(_is,0,SEEK_SET);
		advance();
	}
	
	~bfile_iterator_first()
	{
		if(_buffer!=NULL)
			free(_buffer);
	}
	
	
	T const& operator*()  {  return _elem;  }
	
	bfile_iterator_first& operator++()
	{
		advance();
		return *this;
	}
	
	friend bool operator==(bfile_iterator_first const& lhs, bfile_iterator_first const& rhs)
	{
		if (!lhs._is || !rhs._is)  {  if (!lhs._is && !rhs._is) {  return true; } else {  return false;  } }
		assert(lhs._is == rhs._is);
		return rhs._pos == lhs._pos;
	}
	
	friend bool operator!=(bfile_iterator_first const& lhs, bfile_iterator_first const& rhs)  {  return !(lhs == rhs);  }
private:
	void advance()
	{
		_pos++;
		
		if(_cptread >= _inbuff)
		{
			int res = fread(_buffer,sizeof(T),_buffsize,_is);
			_inbuff = res; _cptread = 0;
			
			if(res == 0)
			{
				_is = nullptr;
				_pos = 0;
				return;
			}
		}
		
		_elem = _buffer[_cptread];
		_cptread ++;
		_cptread ++;
	}
	T _elem;
	FILE * _is;
	unsigned long _pos;
	
	T * _buffer; // for buffered read
	int _inbuff, _cptread;
	int _buffsize;
};

template <class T>
class file_binary_first{
public:
	
	file_binary_first(const char* filename)
	{
		_is = fopen(filename, "rb");
		if (!_is) {
			throw std::invalid_argument("Error opening " + std::string(filename));
		}
	}
	
	~file_binary_first()
	{
		fclose(_is);
	}
	
	bfile_iterator_first<T> begin() const
	{
		return bfile_iterator_first<T>(_is);
	}
	
	bfile_iterator_first<T> end() const {return bfile_iterator_first<T>(); }
	
	size_t        size () const  {  return 0;  }//todo ?
	
private:
	FILE * _is;
};





// iterator from disk file of T with buffered read
template <class T>
class bfile_iterator : public std::iterator<std::forward_iterator_tag, T>{
    public:

    bfile_iterator()
    : _is(nullptr)
    , _pos(0) ,_inbuff (0), _cptread(0)
    {
        _buffsize = 10000;
        _buffer = (T *) malloc(_buffsize*sizeof(T));
    }

    bfile_iterator(const bfile_iterator& cr)
    {
        _buffsize = cr._buffsize;
        _pos = cr._pos;
        _is = cr._is;
        _buffer = (T *) malloc(_buffsize*sizeof(T));
         memcpy(_buffer,cr._buffer,_buffsize*sizeof(T) );
        _inbuff = cr._inbuff;
        _cptread = cr._cptread;
        _elem = cr._elem;
    }

    bfile_iterator(FILE* is): _is(is) , _pos(0) ,_inbuff (0), _cptread(0)
    {
        _buffsize = 10000;
        _buffer = (T *) malloc(_buffsize*sizeof(T));
        int reso = fseek(_is,0,SEEK_SET);
        advance();
    }

    ~bfile_iterator()
    {
        if(_buffer!=NULL)
            free(_buffer);
    }


    T const& operator*()  {  return _elem;  }

    bfile_iterator& operator++()
    {
        advance();
        return *this;
    }

    friend bool operator==(bfile_iterator const& lhs, bfile_iterator const& rhs)
    {
        if (!lhs._is || !rhs._is)  {  if (!lhs._is && !rhs._is) {  return true; } else {  return false;  } }
        assert(lhs._is == rhs._is);
        return rhs._pos == lhs._pos;
    }

    friend bool operator!=(bfile_iterator const& lhs, bfile_iterator const& rhs)  {  return !(lhs == rhs);  }
    private:
    void advance()
    {
        _pos++;

        if(_cptread >= _inbuff)
        {
            int res = fread(_buffer,sizeof(T),_buffsize,_is);
            _inbuff = res; _cptread = 0;

            if(res == 0)
            {
                _is = nullptr;
                _pos = 0;
                return;
            }
        }

        _elem = _buffer[_cptread];
        _cptread ++;
    }
    T _elem;
    FILE * _is;
    unsigned long _pos;

    T * _buffer; // for buffered read
    int _inbuff, _cptread;
    int _buffsize;
};




template <class T>
class file_binary{
    public:

    file_binary(const char* filename)
    {
        _is = fopen(filename, "rb");
        if (!_is) {
            throw std::invalid_argument("Error opening " + std::string(filename));
        }
    }

    ~file_binary()
    {
        fclose(_is);
    }

    bfile_iterator<T> begin() const
    {
        return bfile_iterator<T>(_is);
    }

    bfile_iterator<T> end() const {return bfile_iterator<T>(); }

    size_t        size () const  {  return 0;  }//todo ?

    private:
    FILE * _is;
};



template <typename RangeKeyOnly, typename RangeKeyValue>
class quasiDictionnary
{
public:

    // Creates a probabilisticSet for the set of elements.
    // Creates a MPHF for the elements
    quasiDictionnary(u_int64_t nelement, RangeKeyOnly& itKey, RangeKeyValue& it, const int fingerprint_size, const int value_size, double gammaFactor=1, int nthreads=1)
	: _nelement(nelement), _gammaFactor(gammaFactor), _fingerprint_size(fingerprint_size), _nthreads(nthreads)
    {



		_itKeyOnly = itKey;
		_itKeyValue = it;
		_valueSize = value_size;


		cout << "NB elems: " << _nelement << " elems" << endl;
		cout << "Fingerprint size: " << _fingerprint_size << " bits" << endl;
		cout << "Value size: " << _valueSize << " bits" << endl;

    	createMPHF();
    	createValues();

		//printf("iterate over key-values \n");
		//for (auto var : input_keys_values)
		//{
		//	printf("%lli  %lli\n",std::get<0>(var),std::get<1>(var));
		//}
		//_iterator = input_keys_values;
		
		//printf("iterate over key only \n");
		//IteratorKeyWrapper key_iterator(input_keys_values);
		//iterator_first<u_int64_t> key_iterator(input_keys_values);
		//input_keys_values.begin();

		//ƒ<u_int64_t> key_iterator ("keyfile");

		/*
		for (auto var : itKey)
		{
			cout << var << endl;
			break;
			//cout << std::get<0>(var) << " " << std::get<1>(var) << endl;
		}
		
		for (auto var : it)
		{
			cout << std::get<0>(var) << " " << std::get<1>(var) << endl;
			break;
		}*/

		
        // Creates a MPFH containing _nelement taken from input_range





    }

    void createMPHF(){
        cout << "MPHF creating " << endl;
        _bphf = new boomphf::mphf<u_int64_t,hasher_t>(_nelement,_itKeyOnly,_nthreads,_gammaFactor);

        cout << "MPHF created" << endl;
    }

    void createValues(){

    	cout << "creating values" << endl;

    	_prob_set = probabilisticSet(_nelement, _fingerprint_size);

    	_values = bitArraySet(_nelement, _valueSize);

        for(auto& key_value: _itKeyValue){
        	const u_int64_t& index = _bphf->lookup(get<0>(key_value));
        	insertValue(index, get<1>(key_value));
        }
    	/*
    	_lca2values = bitArraySet(_nelement, _valueSize*2);

        for(auto& key_value: _itKeyValue){


        	u_int64_t lca = get<1>(key_value);
            u_int32_t lca1 = getLca1(lca);
            u_int32_t lca2 = getLca2(lca);

            cout << lca1 << " " << lca2 << endl;
            _prob_set.add(indice, get<0>(key_value));
            //_values_set.set_i(indice, );
        }*/
    }



private:
    probabilisticSet _prob_set;
    bitArraySet _values;
    u_int64_t _nelement;
    double _gammaFactor;
    boophf_t * _bphf;
    int _nthreads;

    int _fingerprint_size;
    int _valueSize;

    RangeKeyOnly _itKeyOnly;
    RangeKeyValue _itKeyValue;
};

#endif // QUASIDICTIONNARY_H
