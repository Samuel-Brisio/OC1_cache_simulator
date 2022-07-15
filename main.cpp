#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

#define N_FIELDS 4


using namespace std::chrono;
using Clock = std::chrono::steady_clock;

struct Cache
{
    std::vector <u_int16_t> index;
    std::vector <bool> valido;
    std::vector <u_int32_t> endereco;
    std::vector <steady_clock::time_point> lru; //least recently used


    // define o tamanho da cache (numero de linhas)
    void define_size (uint tamanho) {
        index.resize(tamanho);
        valido.resize(tamanho);
        endereco.resize(tamanho);
        lru.resize(tamanho);
    } 
};

int calcula_endereco(u_int32_t addr, u_int16_t offset_size);
void faz_computacao(Cache &cache,std::ifstream &file,int n_linhas, u_int32_t set_size, int offset) ;

int main() {


    //inputs

    int cache_size;
    int row_size;
    int set_size;
    std::string file_name = "test.txt"; 


    // Calcula o numero de linhas
    const int N_ROWS = cache_size / row_size;

    // cria a cache
    //std::vector < int > cache (N_ROWS, (std::vector <int> (N_FIELDS);
    Cache cache;
    cache.define_size(N_ROWS);

    // abre o arquivo
    std::ifstream file;
    file.open(file_name);

    // Calcula o tamanho do offset
    int offset = 0;

    faz_computacao(cache, file, row_size, set_size, offset);

    return 0;
}


void faz_computacao(Cache &cache,std::ifstream &file,int n_linhas, u_int32_t set_size, int offset) {
    u_int32_t addr;
    u_int32_t block_addr;

    // numero de conjuntos que a cache possui
    u_int32_t n_set = n_linhas / set_size;

    while(file >> std::hex >> addr) {
        block_addr = calcula_endereco(addr, offset);

        int index = block_addr % n_set;
        
        // se o dado foi inserido com sucesso na cache
        bool dado_inserido = false;
        for (int i = 0; i < set_size; i++) {

            if(cache.valido[index + i] == 0) {
                cache.endereco[index + i] = addr;
                cache.lru[index + i] = Clock::now();
                dado_inserido = true;
                break;
            }
        }

        // se o bloco ainda não foi inserido na cache
        // É pq o conjunto já está populado
        // E teremos que substituir o bloco da cache que tem o menor LRU
        if(dado_inserido == false) {
            for (int i = 0; i < set_size; i++) {
                
            }
        }

    }
}

int calcula_endereco(u_int32_t addr, u_int16_t offset_size) {
    return addr << offset_size;
}
