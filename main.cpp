#include <iostream>
#include <vector>
#include <fstream>

#define N_FIELDS 4

struct Cache
{
    std::vector <u_int16_t> index;
    std::vector <bool> valido;
    std::vector <u_int32_t> endereco;
    std::vector <int> lru; //least recently used


    // define o tamanho da cache (numero de linhas)
    void define_size (uint tamanho) {
        index.resize(tamanho);
        valido.resize(tamanho);
        endereco.resize(tamanho);
        lru.resize(tamanho);
    } 
};

int calcula_endereco(u_int32_t arr, u_int16_t offset_size);
void faz_computacao(Cache &cache,std::ifstream &file);

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


    faz_computacao(cache, file);

    return 0;
}


void faz_computacao(Cache &cache,std::ifstream &file,int n_linhas, u_int32_t set_size) {
    u_int32_t addr;
    u_int16_t offset = 0;
    u_int32_t block_addr;
    u_int32_t n_set = n_linhas / set_size;

    while(file >> std::hex >> addr) {
        block_addr = calcula_endereco(addr, offset);

        int index = block_addr % n_set;
        
        // se o dado foi inserido com sucesso na cache
        bool dado_inserido = false;
        for (int i = 0; i < set_size; i++) {

            if(cache.valido[index + i] == 0) {
                cache.endereco[index + i] = addr;
                dado_inserido = true;
                break;
            }
        }

        // se o bloco ainda não foi inserido na cache
        // É pq o conjunto já está populado
        if(dado_inserido == false) {
            for (int i = 0; i < set_size; i++) {
                
            }
        }

    }
}

int calcula_endereco(u_int32_t arr, u_int16_t offset_size) {
    return 0;
}
