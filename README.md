<h1> Programação socket UDP em Windows </h1>

Trabalho desenvolvido em equipe para a disciplina de Redes de Computadores para Automação em 2018-2.

Proposta do trabalho:
Implementar um programa em linguagem C, para envio de um arquivo denominado teste0.doc de um computador X para outro computador Y via programação socket UDP.

Requisitos:
- Programar em Linguagem C;
- Todo pacote de dados enviado deve ter um cabeçalho com (pelo menos) valor checksum e número de sequência associados;
- Receptor envia mensagens de reconhecimento relativos aos pacotes recebidos;
- Transmissor retransmite pacotes, quando necessário;
- Apresentar exemplos de erros e como são corrigidos. Ex. Erro na comunicação de dados (perda do pacote. pacote chegando fora de ordem, erro de checksum, timeout etc).


Erros testados na apresentação:
- Perda do pacote;
- Pacote chegando fora de ordem;
- Erro de checksum;
- Timeout: muda variavel tempo;
- Retransmissão funciona!

Simulações sugeridas: 
- tempo = 3; => timeout
- n_seq==2 ++=> esperava que chegasse o pacote 2, mas chega o 3 (perda de pacote)
- n_seq==2 -1=> esperava que chegasse o pacote 2, mas chega o 1 de novo (evita gravar repetido)
