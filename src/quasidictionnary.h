#ifndef QUASIDICTIONNARY_H
#define QUASIDICTIONNARY_H

#include <iostream>
#include <native_bit_vector_array.h>
#include <probabilistic_set.h>
#include <BooPHF.h>
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

class quasiDictionnary
{
public:

    // Creates a probabilisticSet for the set of elements.
    // Creates a MPHF for the elements
    template <typename Range>
    quasiDictionnary(u_int64_t nelement, Range const& input_keys_values, const int fingerprint_size, const int value_size, double gammaFactor=1, int nthreads=1): _nelement(nelement), _gammaFactor(gammaFactor), _fingerprint_size(fingerprint_size), _nthreads(nthreads), _prob_set(_nelement,_fingerprint_size)
    {
        cout<<"mphf creating "<<nelement<<endl;
        MyIterator<u_int64_t,u_int64_t> fti_input_keys_values (input_keys_values.begin());

        // Creates a MPFH containing _nelement taken from input_range
        _bphf = new boomphf::mphf<u_int64_t,hasher_t>(_nelement,fti_input_keys_values,_nthreads,_gammaFactor); // TODO MAKE A VIEW ON THE FIRST ELEMENT TO ITERATE.
        cout<<"mphf created"<<endl;

        _values_set = bitArraySet(nelement, value_size);

        // inserts fingerprints of elements and values of elements
        for(auto& key_value: input_keys_values){
            const u_int64_t& indice = _bphf->lookup(get<0>(key_value));
            _prob_set.add(indice, get<0>(key_value));
            _values_set.set_i(indice, get<1>(key_value));
        }

    }

private:
    probabilisticSet _prob_set;
    bitArraySet _values_set;
    u_int64_t _nelement;
    double _gammaFactor;
    boophf_t * _bphf;
    int _nthreads;

    int _fingerprint_size;
};

#endif // QUASIDICTIONNARY_H
