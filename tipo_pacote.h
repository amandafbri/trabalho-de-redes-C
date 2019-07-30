/*
 Estrutura para pacotes
*/

struct tipo_pacote{
     char linha[50]; //reserva espaco para linha, apesar de nao ser a melhor solucao
     unsigned int n_sequencia;  // reserva 1 byte para o número de sequencia
     unsigned char check;       // reserva 1 byte para o checksum
     char reconhece;
} pacote1,pacote2;
