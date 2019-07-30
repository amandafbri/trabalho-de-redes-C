<h1> Programa��o socket UDP em Windows </h1>

Trabalho desenvolvido em equipe para a disciplina de Redes de Computadores para Automa��o em 2018-2.

Proposta do trabalho:
Implementar um programa em linguagem C, para envio de um arquivo denominado teste0.doc de um computador X para outro computador Y via programa��o socket UDP.

Requisitos:
- Programar em Linguagem C;
- Todo pacote de dados enviado deve ter um cabe�alho com (pelo menos) valor checksum e n�mero de sequ�ncia associados;
- Receptor envia mensagens de reconhecimento relativos aos pacotes recebidos;
- Transmissor retransmite pacotes, quando necess�rio;
- Apresentar exemplos de erros e como s�o corrigidos. Ex. Erro na comunica��o de dados (perda do pacote. pacote chegando fora de ordem, erro de checksum, timeout etc).


Erros testados na apresenta��o:
- Perda do pacote;
- Pacote chegando fora de ordem;
- Erro de checksum;
- Timeout: muda variavel tempo;
- Retransmiss�o funciona!

Simula��es sugeridas: 
tempo = 3; => timeout
n_seq==2 ++=> esperava que chegasse o pacote 2, mas chega o 3 (perda de pacote)
n_seq==2 -1=> esperava que chegasse o pacote 2, mas chega o 1 de novo (evita gravar repetido)