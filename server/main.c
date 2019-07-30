/*
    Servidor UDP
    Trabalho para a disciplina de Redes de Computadores para Automacao (DAS5314)

    Amanda Furtado Brinhosa
    Leonardo Fernandes
    Pedro de Melo
*/

#include<stdio.h>
#include<winsock2.h>
#include<time.h>
#include<tipo_pacote.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define PORT 50000 //A porta que "ouve" os dados

#define ACK 1
#define NACK 0

// Cria um arquivo "testeX.doc" onde "X" é um número pertencente ao conjunto dos números inteiros.
// E retorna o nome do arquivo criado

FILE *arquivo1;
char *nome_arquivo1;
int checkTeste;
int contagem = 0;

// Função que retorna 1 se o arquivo já existe e 0 caso contrário;

int arquivoExiste(const char * filename){

    FILE *file;
    if (file = fopen(filename, "r")){
        fclose(file);
        return 1;
    }
    return 0;
}

char *criar_arquivo(){

  int i = 0;
  static char nome[20];

  sprintf(nome, "teste%d.doc", i);

  while(arquivoExiste(nome)){
    i = i + 1;
    sprintf(nome, "teste%d.doc", i);
  }

   return &nome;
}

FILE *abrir_arquivo(const char * nomeArquivo){
    FILE *arquivo;
    arquivo = fopen(nomeArquivo,"w");
    return arquivo;
}

//Escreve uma linha no arquivo criado

void escrever_arquivo(char linha[],FILE *arquivo){
    fputs(linha, arquivo);
}

//Fecha o arquivo onde estao sendo inseridos os dados enviados pelo cliente

void fechar_arquivo(FILE *arquivo){
    fclose (arquivo);
}

char checksum(char dados[], int tam)
{
   unsigned char check;
   int i;

   for(check = i = 0; i < tam; i++)
       check = check ^ dados[i]; // operador XOR byte a byte

   return check;
}

int main()
{
    //Inicializa Socket
    SOCKET s;
    struct sockaddr_in server, si_other;
    int slen , recv_len;
    WSADATA wsa;

    slen = sizeof(si_other) ;

    printf("\nInicializando Socket...");


    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Falhou. Erro de codigo %d \n",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Inicializada.\n");

    //Cria Socket
    if((s = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Nao foi possivel criar Socket : %d \n" , WSAGetLastError());
    }
    printf("Socket criada.\n");

    //Prepara estrutura sockaddr_in
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );

    //Bind (ligacao)
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Ligacao falhou com codigo de erro %d\n" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Ligacao feita.\n");

    //Deixa arquivo preparado
    nome_arquivo1 = criar_arquivo();
    arquivo1 = abrir_arquivo(nome_arquivo1);

    //Servidor fica "ouvindo" para saber se ha dados a receber
    while(1)
    {
        printf("Aguardando dados...\n");
        fflush(stdout);

        //Recebe pacote do cliente
        if ((recv_len = recvfrom(s, &pacote1, 1000, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() falhou com codigo de erro %d\n" , WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        //Faz verificacao do checksum
        unsigned char calculado;
        calculado = checksum(pacote1.linha,sizeof(pacote1.linha));
        calculado = ~ calculado;

        //Se checksum nao tem erro, verifica se numero de sequencia era o esperado.
            //Caso sim, grava dados no arquivo e responde com ACK.
            //Caso não, apenas responde com ACK, para evitar duplicacao de informacoes.
        if(calculado == pacote1.check){
                if(contagem == pacote1.n_sequencia){
                    escrever_arquivo(pacote1.linha,arquivo1);
                    printf("Valor da contagem: %d \n",contagem);
                    contagem++;
                    pacote2.reconhece = ACK;
                    printf("ACK \n");
                    pacote2.check = pacote1.check;
                    pacote2.n_sequencia = pacote1.n_sequencia;
                    if (sendto(s, (const char*)&pacote2, sizeof(pacote2), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR){
                        printf("sendto() falhou com codigo de erro %d\n" , WSAGetLastError());
                        exit(EXIT_FAILURE);
                    }
                }
                //Esperava 11, mas chega 10, por exemplo
                else if(contagem > pacote1.n_sequencia){
                    printf("Valor da contagem: %d \n",contagem);
                    contagem++;
                    pacote2.reconhece = ACK;
                    printf("ACK \n");
                    pacote2.check = pacote1.check;
                    pacote2.n_sequencia = pacote1.n_sequencia + 1;
                    if (sendto(s, (const char*)&pacote2, sizeof(pacote2), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR){
                        printf("sendto() falhou com codigo de erro %d\n" , WSAGetLastError());
                        exit(EXIT_FAILURE);
                    }
                }
                //Esperava 11, mas chega 12, por exemplo
                else{
                    printf("Valor da contagem: %d \n",contagem);
                    contagem++;
                    pacote2.reconhece = NACK;
                    printf("NACK \n");
                    pacote2.check = pacote1.check;
                    pacote2.n_sequencia = pacote1.n_sequencia;
                    if (sendto(s, (const char*)&pacote2, sizeof(pacote2), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR){
                        printf("sendto() falhou com codigo de erro %d\n" , WSAGetLastError());
                        exit(EXIT_FAILURE);
                    }
                }
         }
        //Se checksum tem erro, responde com NACK para que cliente retransmita.
        else{
            //contagem++;
            pacote2.reconhece = NACK;
            printf("NACK \n");
            pacote2.check = pacote1.check;
            pacote2.n_sequencia = pacote1.n_sequencia;
            if (sendto(s, (const char*)&pacote2, sizeof(pacote2), 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR){
                printf("sendto() falhou com codigo de erro %d\n" , WSAGetLastError());
                exit(EXIT_FAILURE);
            }
        }

        //como são 6 linhas, após as 6 linhas forem enviadas deve-se fechar o arquivo.
        if(contagem == 6){
            fechar_arquivo(arquivo1);
        }

        //Imprime detalhes da comunicacao
        printf("Pacote recebido de %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Dados: %s\n" , pacote1.linha);

    }

    closesocket(s);
    WSACleanup();

    return 0;
}
