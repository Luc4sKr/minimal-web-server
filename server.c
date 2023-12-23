#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <netinet/in.h>

void main() {
    // um socket é uma maneira de os programas se comunicarem através de uma rede, 
    //permitindo a troca de dados entre eles de forma confiável e eficiente.
    
    // domain, type, protocol
    int s = socket(AF_INET, SOCK_STREAM, 0);

    // AF_INET: é uma constante que indica o domínio de endereço para o socket.
    // SOCK_STREAM: provê fluxos de bytes sequenciais, confiáveis, bidirecionais
    // e baseados em conexão. 

    // estrutura que descreve um endereço de soquete da Internet.
    struct sockaddr_in addr = {
        AF_INET,    // família de endereços (no caso, IPv4)
        0x901f,     // número de porta 8080 (o número da porta é o hex invertido)
        0           // endereço IP (neste caso, 0 indica um endereço "qualquer" ou "local")
    };

    // associa um endereço a um socket específico.
    bind(s, &addr, sizeof(addr));

    // coloca o socket em um estado passivo, pronto para aceitar conexões de entrada
    listen(s, 10);
    // 10: máximo de conexões

    // aceita uma conexão pendente em um socket que foi colocado em estado
    // de escuta (listen) e está aguardando conexões de entrada.
    // obs: fd = file descriptor
    int client_fd = accept(s, 0, 0);
    // 0, 0: são parâmetros que indicam a função accept para aceitar a
    // conexão pendente sem preocupação em obter informações sobre o
    // endereço do cliente ou configurações especiais do socket. normalmente,
    // quando não se deseja especificar informações detalhadas sobre o cliente
    // ou definir opções específicas para o socket, são passados como argumentos nulos (0 ou NULL).

    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);

    // recv: usado para receber mensagens de um socket
    // client_fd: é o descritor do socket que representa a conexão específica com um cliente. 
    // buffer: é um array de caracteres onde os dados recebidos serão armazenados. 
    
    // primeiros caracteres do buffer
    // GET /file.html .....
    
    // inicializa com o endereço de memória do quinto caractere a partir da string contida em buffer
    char* f = buffer + 5; 

    // encontra a primeira ocorrência do caractere de espaço (' ') na string
    // apontada por 'f' e o substitui por um caractere nulo (0).
    *strchr(f, ' ') = 0;

    // Dessa forma, após a execução desse código, a string referenciada por f conterá
    // apenas o nome do arquivo, cortando qualquer texto após o primeiro espaço 
    // encontrado na posição que começou a ser apontada por f.
    
    // abre o arquivo 
    int opened_fd = open(f, O_RDONLY);
    
    // envia o conteúdo do arquivo associado ao descritor opened_fd para o socket client_fd. 
    sendfile(client_fd, opened_fd, 0, 256);

    // fecha os descritores
    close(opened_fd);
    close(client_fd);
    close(s);
}