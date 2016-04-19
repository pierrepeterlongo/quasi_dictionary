#ifndef QUASIDICTIONNARY_H
#define QUASIDICTIONNARY_H


#include "IteratorGzMPHF.hpp"
//#include "../BooPHF/BooPHF.h"
#include <iostream>
#include "native_bit_vector_array.h"
#include "probabilistic_set.h"

typedef boomphf::SingleHashFunctor<u_int64_t>  hasher_t;
typedef boomphf::mphf<  u_int64_t, hasher_t  > boophf_t;

/**

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


*/

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
	quasiDictionnary(){
	}

    /**
     * @brief quasiDictionnary : probabilistic dictionnary: may have false positives
     * @param nelement: number of elements to store
     * @param itKey: iterator over the keys to be stored
     * @param it: iterator over the keys and their values to store
     * @param fingerprint_size: size of the fingerprint associated to each key to verify its existance in the original set. Can be set to zero if this is not needed
     * @param value_size: size of each value associated to each key. This must be below 62 bits.
     * @param gammaFactor: for MPHF
     * @param nthreads: for MPHF construction
     */
    quasiDictionnary(u_int64_t nelement, RangeKeyOnly& itKey, RangeKeyValue& it, const int fingerprint_size, const int value_size, double gammaFactor=1, int nthreads=1)
    {



		_itKeyOnly = itKey;
		_itKeyValue = it;
		_valueSize = value_size;
		_nelement = nelement;
		_gammaFactor = gammaFactor;
		_fingerprint_size = fingerprint_size;
		_nthreads = nthreads;



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


	bool contains(u_int64_t key){
		u_int64_t index = _bphf->lookup(key);
		if(index == ULLONG_MAX) return false;
		if(_fingerprint_size>0)
			return _prob_set.exists(index, key);
		return true;
	}

    /**
     * @brief get_value: returns a value from a key in a quasi dictionnary
     * @param key: the key of the seek value
     * @param exists: set to true is detected as indexed in the quasiDictionnary, else false
     * @return 0 if nothing found (and exists set to false) or the value associated to the key else
     */
	u_int64_t get_value(u_int64_t key, bool &exists){
		//cout << "\t get: " << key << endl;
		const u_int64_t& index = _bphf->lookup(key);
		if(index == ULLONG_MAX){
			exists = false;
			return 0;
		}
		//cout << "\t index: " << index << endl;
		if(_fingerprint_size>0){
			if(_prob_set.exists(index, key)){
				exists = true;
				//cout << "\t FOUND" << endl;
				return _values.get_i(index);
			}
		}
		else{
			exists = true;
			return _values.get_i(index);
		}
		//cout << "\t NOT FOUND" << endl;
		exists = false;
		return 0;
	}

    //u_int64_t get_value(u_int64_t key, bool &exists){
	//    const u_int64_t& index = _bphf->lookup(key);
	//    //TODO: what if not found by MPHF -1 ?
	//    if(_fingerprint_size>0&&!_prob_set.exists(index, key)){
	//            exists=false;
	//            return 0;
	//        }
	//    exists=true;
	//    return _values.get_i(index);
	//}

	void save(std::ostream& os) const
	{
		os.write(reinterpret_cast<char const*>(&_valueSize), sizeof(_valueSize));
		os.write(reinterpret_cast<char const*>(&_nelement), sizeof(_nelement));
		os.write(reinterpret_cast<char const*>(&_gammaFactor), sizeof(_gammaFactor));
		os.write(reinterpret_cast<char const*>(&_fingerprint_size), sizeof(_fingerprint_size));
		os.write(reinterpret_cast<char const*>(&_nthreads), sizeof(_nthreads));
		_prob_set.save(os);
		_values.save(os);
		_bphf->save(os);

	}

	void load(std::istream& is)
	{
		is.read(reinterpret_cast<char*>(&_valueSize), sizeof(_valueSize));
		//cout << _valueSize << endl;
		is.read(reinterpret_cast<char*>(&_nelement), sizeof(_nelement));
		//cout << _nelement << endl;
		is.read(reinterpret_cast<char*>(&_gammaFactor), sizeof(_gammaFactor));
		//cout << _gammaFactor << endl;
		is.read(reinterpret_cast<char*>(&_fingerprint_size), sizeof(_fingerprint_size));
		//cout << _fingerprint_size << endl;
		is.read(reinterpret_cast<char*>(&_nthreads), sizeof(_nthreads));
		_prob_set.load(is);
		_values.load(is);

        _bphf = new boomphf::mphf<u_int64_t,hasher_t>();
		_bphf->load(is);
	}

private:
    /**
     * @brief createMPHF constructs the MPHF from the set of keys
     */
    void createMPHF(){
        cout << "MPHF creating " << endl;
        _bphf = new boomphf::mphf<u_int64_t,hasher_t>(_nelement,_itKeyOnly,_nthreads,_gammaFactor);

        cout << "MPHF created" << endl;
    }
    /**
     * @brief createValues once the MPHF is constructed: construct the probabilisticSet storing the fingerprints and stores the values in a newly constructed bitArraySet
     */
    void createValues(){

        cout << "creating values" << endl;
        if(_fingerprint_size>0)
            _prob_set = probabilisticSet(_nelement, _fingerprint_size);

        _values = bitArraySet(_nelement, _valueSize);

        for(auto& key_value: _itKeyValue){
        	const u_int64_t& index = _bphf->lookup(std::get<0>(key_value));
        	if (_fingerprint_size>0){
        		_prob_set.add(index, std::get<0>(key_value));
        	}
        	_values.set_i(index, std::get<1>(key_value));
        }

    }






private:

    /**
     * @brief insertValue Add a new key/value in the quasi dictionnary.
     * @param index: index (from MPHF where to insert the value)
     * @param key: key of the value (used for the fingerprint creation)
     * @param value: value to store.

    void insertValue(u_int64_t index, u_int64_t key, u_int64_t value){
        if (_fingerprint_size>0){
            _prob_set.add(index,key);
        }
        _values.set_i(index,value);
    }*/

    /**
     * @brief _prob_set probabilistic set used to inform about the existance of a query element.
     */
    probabilisticSet _prob_set;
    /**
     * @brief _values stores for each indexed element the value associated to a key
     */
    bitArraySet _values;

    /**
     * @brief _nelement number of elements to be stored (fix)
     */
    u_int64_t _nelement;


    double _gammaFactor;

    /**
     * @brief _bphf MPHF used to assign an index to a key
     */
    boophf_t * _bphf;
    int _nthreads;

    /**
     * @brief _fingerprint_size size of the fingerprint. In [0,61]
     */
    int _fingerprint_size;

    /**
     * @brief _valueSize Size of the stored values. In [0,61] (but zero is really, really stupid.
     */
    int _valueSize;

    /**
     * @brief _itKeyOnly iterator on the key set
     */
    RangeKeyOnly _itKeyOnly;

    /**
     * @brief _itKeyValue iterator on tuples key,value
     */
    RangeKeyValue _itKeyValue;
};

#endif // QUASIDICTIONNARY_H
