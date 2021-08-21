#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>

template<typename TBASE,typename TRET=unsigned int>
class SimpPRNG {
private:
    TBASE x;

public:
    SimpPRNG(const TBASE seed = 2569018830187087903ull) {
        x = seed;
        for( int i=0 ; i<30 ; i++ ) {
            (*this)();
        }
    }
    TRET operator()() {
        return ( x = (x * 79908886691ull + 8143076871913ull) ) >> ( (sizeof(TBASE) - sizeof(TRET) ) * CHAR_BIT );
    }
};


int main( int argc , char *argv[] ) {
    if( argc > 1 && strstr(argv[1],"help") ) {
        printf("SimpPRNG seed size\n");
        return 0;
    }
    unsigned long long seed = 1ull;
    if( argc > 1 ) {
        char *end;
        seed = strtoull(argv[1],&end,10);
    }
    unsigned long long size = ULLONG_MAX;
    if( argc > 2 ) {
        char *end;
        size = strtoull(argv[2],&end,10);
    }
    typedef unsigned int type_out;
    SimpPRNG<__uint128_t, type_out> r(seed);
    constexpr int BUFF_SIZE = (1<<12);
    char buff[BUFF_SIZE];
    for( unsigned long long i=0 ; i<size ; ) {
        int j=0;
        while( j<BUFF_SIZE && i<size ) {
            type_out v = r();
            for( int k=0 ; k<sizeof(type_out)*CHAR_BIT && j<BUFF_SIZE && i<size ; k++, j++, i++ ) {
                buff[j] = '0' + (v & 1);
                v >>= 1;
            }
        }
        if( fwrite( buff , 1 , j , stdout ) != j ) {
            break;
        }
    }
    return 0;
}
