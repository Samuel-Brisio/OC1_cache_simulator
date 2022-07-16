#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <cmath>
#include <iomanip>

using namespace std::chrono;
using Clock = std::chrono::steady_clock;

struct Cache
{
    std::vector<u_int16_t> index;
    std::vector<bool> valido;
    std::vector<u_int32_t> endereco;
    std::vector<u_int> lru; // least recently used

    // define o tamanho da cache (numero de linhas)
    void define_size(uint tamanho)
    {
        index.resize(tamanho);
        valido.resize(tamanho);
        endereco.resize(tamanho);
        lru.resize(tamanho);
    }
};

int calcula_endereco(u_int32_t addr, int &offset_size);
void insecao_troca_blocos(Cache &cache, std::ifstream &file, int n_linhas, u_int32_t set_size, int &offset);
void print_estado_da_cache(Cache &cache);

int main(int argc, char **argv)
{

    // inputs
    //  Pegar esses valores via comando de terminal
    //  argv e argc

    int cache_size = std::stoi(argv[1]);
    int row_size = std::stoi(argv[2]);
    int set_size = std::stoi(argv[3]);
    std::string file_name = argv[4];

    // Calcula o numero de linhas
    const int N_ROWS = cache_size / row_size;

    // cria a cache
    Cache cache;
    cache.define_size(N_ROWS);

    // abre o arquivo
    std::ifstream file;
    file.open(file_name);

    // Calcula o tamanho do offset
    int offset = std::log2(row_size);

    // popula a coluna de indexes da tabela
    int index = 0;

    for (int i = 0; i < N_ROWS; i++)
    {
        cache.index[i] = i;
    }

    insecao_troca_blocos(cache, file, N_ROWS, set_size, offset);

    return 0;
}

void insecao_troca_blocos(Cache &cache, std::ifstream &file, int n_linhas, u_int32_t set_size, int &offset)
{
    u_int32_t addr;
    u_int32_t block_addr;

    // numero de conjuntos que a cache possui
    u_int32_t n_set = n_linhas / set_size;
    int num_bits_indentificador = std::ceil(std::log2f(n_set));

    u_int timer = 0;

    while (file >> std::hex >> addr)
    {
        timer++;

        block_addr = calcula_endereco(addr, offset);

        int index = block_addr % n_set;
        index = index * set_size;

        int tag = (block_addr >> num_bits_indentificador);

        // se o dado já está presente na cache
        bool hit = false;

        // se o dado foi inserido com sucesso na cache
        bool dado_inserido = false;

        // se o bloco já está na cache
        for (int i = 0; i < set_size; i++)
        {

            if (cache.valido[index + i] == 1 && cache.endereco[index + i] == tag)
            {
                cache.lru[index + i] = timer;
                hit = true;
                break;
            }
        }

        if (!hit)
        {

            // se o conjunto tem uma linha cujo o bit valido é 0
            for (int i = 0; i < set_size; i++)
            {

                if (cache.valido[index + i] == 0)
                {
                    cache.endereco[index + i] = tag;
                    cache.lru[index + i] = timer;
                    cache.valido[index + i] = true;
                    dado_inserido = true;
                    break;
                }
            }

            // se o bloco ainda não foi inserido na cache
            // É pq o conjunto já está populado
            // E teremos que substituir o bloco da cache que tem o menor LRU
            if (dado_inserido == false)
            {

                std::vector<std::pair<u_int, int>> LRU;

                for (int i = 0; i < set_size; i++)
                {
                    LRU.push_back(std::pair<u_int, int>(
                        cache.lru[index + i],
                        i));
                }

                // Encontra o bloco mais antigo
                std::pair<u_int, int> last_block = *(std::min_element(LRU.begin(), LRU.end()));

                cache.endereco[index + last_block.second] = tag;
                cache.lru[index + last_block.second] = timer;
            }
        }

        print_estado_da_cache(cache);
    }
}

int calcula_endereco(u_int32_t addr, int &offset_size)
{
    // verificar se esse shift é em relação a bits
    return addr >> offset_size;
}

void print_estado_da_cache(Cache &cache)
{

    std::cout << "================" << std::endl;
    std::cout << "IDX V ** ADDR **" << std::endl;

    for (int i = 0; i < cache.endereco.size(); i++)
    {

        std::cout << std::dec << std::setfill('0') << std::setw(3) << cache.index[i] << " "; // cache index o tamanho dele tem que ser 3
        std::cout << cache.valido[i];
        if (cache.valido[i])
        {
            std::cout << " 0x" << std::hex << std::setfill('0') << std::setw(8) << std::uppercase << cache.endereco[i] << std::endl;
        }
        else
            std::cout << std::endl;
    }
}