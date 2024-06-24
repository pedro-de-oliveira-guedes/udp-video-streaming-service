# Contextualização
O presente trabalho apresenta o desenvolvimento de duas aplicações de console em C, uma de cliente e outra de servidor, destinadas a simular o funcionamento básico de aplicativos de streaming. Para a comunicação, foi escolhido o protocolo UDP, para priorizar a rapidez na transmissão de dados, essencial em ambientes de streaming, apesar da ausência de garantias de entrega.
O servidor é projetado para gerenciar múltiplas requisições simultâneas utilizando multithreading, o que permite várias sessões de streaming serem atendidas ao mesmo tempo, melhorando a eficiência e a capacidade de resposta do sistema.

# Organização dos arquivos
Inicialmente, o projeto consistirá de duas pastas e três arquivos, que são:
- ***include/***: Pasta com os arquivos de cabeçalho que contém as definições de TAD e funções utilizadas para o funcionamento do programa.
- ***src/***: Pasta com os arquivos de código fonte que implementam os cabeçalhos e permitem o funcionamento da conexão.
- ***.gitignore***: Arquivos a serem ignorados pelo Git ao fazer o versionamento.
- ***Makefile***: Arquivo com as regras necessárias para a compilação do código fonte.
- ***README.md***: Documentação simplificada em markdown do projeto.
  
Quando o usuário faz a compilação dos programas do cliente e do servidor, de acordo com as instruções que serão dadas na próxima subseção, surgem duas novas pastas. São elas:
- ***bin/***: Pasta com os binários gerados pela compilação do código fonte. Serão gerados dois arquivos: client e server, representando o programa do cliente e do servidor respectivamente.
- ***obj/***: Pasta com os arquivos de objeto gerados como subproduto da compilação dos programas de cliente e servidor.

## Instruções de compilação
Para que o programa seja compilado, basta que o usuário acesse a pasta raiz do projeto e digite “make all”, ou apenas “make”, que a compilação será feita, gerando os binários na pasta “bin/”.

Para que os objetos e binários sejam removidos, basta que o usuário, também na pasta raiz do projeto, digite o comando “make clean”, que as pastas “bin/” e “obj/” serão apagadas.

# Explicação do código
Ao longo das próximas subseções, cada uma das funções utilizadas nos arquivos principais para a comunicação UDP serão explicadas. Note que já existem comentários e documentação no próprio código, que podem auxiliar o leitor mais profundamente no entendimento do propósito e funcionamento de cada um.

## Configurações de rede (network_config.c)
As funções implementadas nesse arquivo são:
- ***logError(...)***: Essa função é chamada quando ocorre um erro no programa. Ela exibe uma mensagem customizada junto com a causa do erro e finaliza o programa com código 1 (erro).
- ***clientSocketInit(...)***: Essa função é chamada ao iniciar o cliente, recebendo uma estrutura do tipo sockaddr_storage, que funciona como uma “superclasse” para endereços do tipo IPv4 e IPv6. De acordo com a versão de IP recebida pela linha de comando, é feita uma tentativa de conversão do endereço em string para uma sequência de bytes IPv4 ou IPv6 na chamada da função inet_pton(...). Caso a conversão seja bem sucedida, a estrutura recebida é especializada através de um type casting para sockaddr_in ou sockaddr_in6 e é retornado 0 indicando sucesso. Caso uma versão de IP desconhecida seja informada, ou o endereço informado na linha de comando seja inválido, a função retorna -1 indicando falha.
- ***serverSocketInit(...)***: Essa função tem o mesmo objetivo da anterior, executando inclusive passos muito parecidos. Porém, ela não recebe um endereço IP por linha de comando, dessa forma o servidor pode ser acessado por qualquer endereço local disponível na máquina de hosting. Como não há endereço IP para tentar a conversão, a função se baseia apenas na versão do IP informada na linha de comando para execução do programa do servidor.
- ***convertAddressToString(...)***: Essa função recebe uma estrutura do tipo sockaddr, que é uma versão base do endereço do soquete funcionando tanto para IPv4 quanto para IPv6, e converte essa estrutura para uma identificação do endereço IP em string. Para fazer essa conversão, é chamada a função inet_ntop(...), que é o oposto da inet_pton(...) usada nas funções descritas anteriormente para converter a string do IP para a estrutura. Essa função é apenas para informar os endereços e portas nas conexões entre cliente e servidor, não desempenhando papel crucial para a conexão em si.

## Cliente (client.c)
As funções implementadas nesse arquivo são:
- ***createClient(...)***: Aloca memória para a estrutura do cliente e inicializa invocando a função clientSocketInit() explicada na seção anterior.
- ***printClientUsage()***: Imprime as instruções de utilização do programa do cliente.
- ***parseArgumentsAndCreateClient(...)***: Recebe os parâmetros informados na linha de comando para execução do programa do cliente e cria o TAD do cliente para uso posterior no programa.
- ***connectToServer(...)***: Essa função é chamada quando o usuário seleciona um ID de filme válido no menu inicial. Ela cria um soquete com a função socket(...), que retorna um número inteiro como descritor do arquivo de soquete criado com base na versão do IP informado e no tipo de conexão, que no caso desse projeto é SOCK_DGRAM (UDP). Como não há confirmação de conexão, o soquete do servidor apenas é armazenado na estrutura do cliente e o código segue.
- ***sendIntegerToServer(...)***: Utiliza a função sendto(...) da biblioteca de soquetes para enviar um número inteiro para o servidor através da conexão UDP.
- ***receiveIntegerFromServer(...)***: Utiliza a função recvfrom(...) da biblioteca de soquetes para receber um número inteiro do servidor através da conexão UDP.
- ***receiveStringFromServer(...)***: Utiliza a função recvfrom(...) da biblioteca de soquetes para receber uma string do servidor através da conexão UDP.
- ***closeClient(...)***: Função invocada quando o usuário opta por sair do aplicativo, fechando o soquete de conexão com o servidor e liberando a memória alocada para o cliente.

## Servidor (server.c)
As funções implementadas nesse arquivo são:
- ***createServer(...)***: Aloca espaço para o servidor e chama a função serverSocketInit(...), que foi descrita anteriormente, para inicializar as informações necessárias para o soquete do servidor. Logo após, o soquete do servidor é de fato criado pela função socket(...) e é invocada a função setsockopt(...) para liberar a porta ocupada pelo servidor imediatamente após ele ser encerrado.
- ***printServerUsage()***: Imprime as instruções de utilização do programa do servidor.
- ***parseArgumentsAndCreateServer(...)***: Recebe os parâmetros informados na linha de comando para execução do programa do servidor e cria o TAD do servidor para uso posterior no programa.
- ***setupServer(...)***: Faz a ligação (binding) do soquete do servidor ao endereço e porta que foram alocados a ele através da função bind(...). Como não há confirmação de conexão no protocolo UDP, não é feita mais nenhuma configuração nessa função.
- ***receiveIntegerFromClient(...)***: Utiliza a função recvfrom(...) da biblioteca de soquetes para receber um número inteiro do cliente através da conexão UDP.
- ***sendIntegerToClient(...)***: Utiliza a função sendto(...) da biblioteca de soquetes para enviar um número inteiro para o cliente através da conexão UDP.
- ***sendStringToClient(...)***: Utiliza a função sendto(...) da biblioteca de soquetes para enviar uma string para o cliente através da conexão UDP.
- ***closeServer(...)***: Função invocada ao finalizar o funcionamento do servidor, fechando o soquete do servidor e desalocando a memória dele.

## Cliente e servidor de streaming (streaming_client.c e streaming_server.c)
Na tentativa de separar a lógica de negócio das funções que manipulam de fato a conexão, foram criados esses dois arquivos. Como o código presente em ambos não é muito técnico e apenas garante que cliente e servidor funcionem de acordo com as especificações, eles não serão comentados nesta seção. Vale apenas a menção do uso de um mutex no arquivo streaming_server.c, que serve para proteger a modificação da variável global currentlyConnectedClients no incremento e decremento da quantidade de clientes conectados.

# Testes
Para testar os programas, basta executar o comando “./bin/server” em um terminal na raiz do projeto, e “./bin/client” em duas outras janelas de terminal para testar a funcionalidade de multithreading. Ao fazer isso, serão impressas as instruções de uso para cada programa. Ajustando os comandos para obedecer às instruções, tanto o programa do cliente quanto o do servidor devem ser inicializados na mesma porta de comunicação, como mostra a imagem a seguir:

![Inicialização dos programas](https://github.com/pedro-de-oliveira-guedes/udp-video-streaming-service/assets/72149404/92656fd0-c365-41ce-a5ec-3dec1b549329)

No terminal de cima, é possível ver que o servidor foi inicializado com sucesso e ainda não há nenhum cliente conectado a ele, já que a conexão só é estabelecida no momento de solicitação de um dos filmes do catálogo. Nos dois terminais de baixo, foi impresso o menu de utilização do cliente, sendo que o programa está aguardando por uma solicitação do usuário.

O passo seguinte consiste em ambos os clientes solicitarem uma conexão com o servidor para assistir filmes, que podem ser os mesmos ou diferentes. Para o propósito dessa demonstração, será solicitado o filme com ID 1 no terminal do cliente inferior da esquerda, será aguardado que o terminal do servidor (superior) notifique que há um cliente conectado e, logo após, no terminal do cliente inferior da direita, será solicitado o filme com ID 3. Dessa forma, será possível verificar tanto o funcionamento correto do servidor ao prover as informações solicitadas, quanto a capacidade de lidar com requisições em paralelo.

![Solicitação de filmes dos dois clientes ao servidor.](https://github.com/pedro-de-oliveira-guedes/udp-video-streaming-service/assets/72149404/e7b090fa-629a-4587-b95d-29e67f542cf1)


Como é possível verificar no terminal do servidor (superior), ele começa com 0 conexões de clientes, aumenta para uma no momento em que o terminal de cliente inferior a esquerda solicita o filme com ID 1 e aumenta para duas conexões no momento em que o terminal de cliente inferior a direita solicita o filme com ID 3. A quantidade de clientes permanece igual a 2, até que o filme do cliente inferior da esquerda finaliza a reprodução, finalizando a conexão e reduzindo o número de clientes conectados de volta para 1. Pouco tempo depois, o mesmo ocorre para o terminal de cliente inferior da direita.
