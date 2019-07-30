/*
	Cliente UDP
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

#define SERVER "127.0.0.1"	//Endereco IP do servidor UDP
#define PORT 50000	//A porta que "ouve" os dados

#define ACK 1
#define NACK 0

char checksum(char dados[], int tam)
{
   unsigned char check;
   int i;

   for(check = i = 0; i < tam; i++)
       check = check ^ dados[i]; // operador XOR byte a byte

   return check;
}

//Medicoes para fazer calculo de timeout
float tempo;
time_t t_ini,t_fim;

unsigned int n_seq;

int main(void)
{
    //Inicializa Socket
	struct sockaddr_in si_other;
	int s, slen=sizeof(si_other);
	FILE * arq;
	WSADATA wsa;

    //Abre arquivo. Caso nao exista, ja ira dar erro.
	arq = fopen("teste.txt","r");
    if(arq == NULL){
        puts("Erro");
        exit(-1);
    }

	printf("\nInicializando Socket...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Falhou. Erro de codigo %d\n",WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Inicializada.\n");

	//Cria Socket
	if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() falhou com codigo de erro %d\n" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//Prepara a estrutura "sockaddr_in"
	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

    //Inicia comunicacao
	while(!feof(arq))
	{
		printf("Pacote: \n");
		//Pega dados do pacote para serem gravados posteriormente
		fgets(pacote1.linha,sizeof(pacote1.linha),arq);

        unsigned char calculado;
        calculado = checksum((char *)&pacote1, sizeof(pacote1.linha));
        calculado = ~ calculado;    // Faz o complemento (inverte os bits) do valor calculado
        pacote1.check = calculado;
        printf("Checksum: %d\n",calculado);

        pacote1.n_sequencia = n_seq;
        printf("Numero Sequencia: %d\n",n_seq);
        n_seq++;

        //Simula erro de checksum
       //if(n_seq == 2){
         //   calculado = checksum((char *)&pacote1, sizeof(pacote1.linha));
          //  pacote1.check = calculado;
        //}
        //else{
         //  calculado = checksum((char *)&pacote1, sizeof(pacote1.linha));
          // calculado = ~ calculado;
           //pacote1.check = calculado;
        //}

        //Simula erros de perda de pacote e pacote fora de ordem
        //if(n_seq == 2){
          //  getchar();
            //n_seq++; // esperava que chegasse o pacote 2, mas chega o 3 (perda de pacote)
           //n_seq = n_seq - 1; //esperava que chegasse o pacote 2, mas chega o 1 de novo (fora de ordem = evita gravar repetido)
        //}

        //Envia pacote
		if (sendto(s,(const char*) &pacote1, sizeof(pacote1) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() falhou com codigo de erro %d\n" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		else{
            t_ini= time(NULL);
		}

		//Recebe pacote do servidor
		if (recvfrom(s,(char*) &pacote2, sizeof(pacote2), 0, (struct sockaddr *) &si_other, &slen) == SOCKET_ERROR){
			printf("recvfrom() falhou com codigo de erro %d\n" , WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		else{
            t_fim = time(NULL);
		}

		//Simula timeout quando tempo = 3, por exemplo
        tempo = difftime(t_fim,t_ini);
        printf("Tempo de transmissao: %f\n", tempo);

		//Se recebeu um ACK do servidor, envia o proximo pacote
		if(pacote2.reconhece == ACK && tempo < 1){
            printf("Enviando proximo...\n");
		}
		//Se recebeu um NACK do servidor ou deu timeout, retransmite
		else if(pacote2.reconhece == NACK || tempo >= 1){
            calculado = checksum((char *)&pacote1, sizeof(pacote1.linha));
            calculado = ~ calculado;    // Faz o complemento (inverte os bits) do valor calculado
            pacote1.check = calculado;

            printf("Retransmitindo...\n");
               if (sendto(s,(const char*) &pacote1, sizeof(pacote1) , 0 , (struct sockaddr *) &si_other, slen) == SOCKET_ERROR){
                    printf("sendto() falhou com codigo de erro %d\n" , WSAGetLastError());
                    exit(EXIT_FAILURE);
                }
		}

        printf("%s \n",pacote2.linha);
	}

	puts("FIM");

	closesocket(s);
	WSACleanup();

	return 0;
}
