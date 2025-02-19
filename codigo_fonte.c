#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Pessoa
{
    int id;
    char nome[200];
    int idade;
} Pessoa;

typedef struct Produto
{
    int id;
    char nome[200];
    float preco;
} Produto;

// Struct que estabelece um relacionamento entre a pessoa e o produto
typedef struct Compra
{
    int idCompra;
    Pessoa cliente;
    Produto produtos[10]; // Será uma lista de até 10 produtos
    int quantidade;
} Compra;

//-------------------------------------- 1 --------------------------------------
int adicionarCliente(Pessoa *pessoa, int numClientes, int numClientesExistentes, FILE *arquivo)
{
    int i;
    if (pessoa == NULL)
    { // Verificando se a alocação realmente deu certo
        return 1;
    }

    for (i = 0; i < numClientes; i++)
    {
        printf("\n====Pessoa %d====\n", numClientesExistentes + i + 1);
        printf("Nome do Cliente: ");
        fgets(pessoa[i].nome, 200, stdin);
        // Remover o '\n' que pode ser capturado pelo fgets()
        pessoa[i].nome[strcspn(pessoa[i].nome, "\n")] = '\0';

        printf("Qual a idade do cliente: ");
        scanf("%d", &pessoa[i].idade);
        getchar();

        pessoa[i].id = (rand() % 100) + 1;
        printf("ID do cliente[%d] eh: %d\n", i + 1, pessoa[i].id); // Por ser como chave primária para buscas não pode haver risco de duplicação.
        printf("------------------\n");
    }
    fseek(arquivo, 0, SEEK_END);
    fwrite(pessoa, sizeof(Pessoa), numClientes, arquivo);
    fclose(arquivo);

    return 0;
}

//-------------------------------------- 2 --------------------------------------
int adicionarProduto(Produto *produto, int numProdutos, int numProdutosExistentes, FILE *arquivo)
{
    int i;
    if (produto == NULL)
    { // Re-verifica a alocacao
        return 1;
    }

    for (i = 0; i < numProdutos; i++)
    {
        printf("\n====Produto %d====\n", numProdutosExistentes + i + 1);
        printf("Nome do produto: ");
        fgets(produto[i].nome, 200, stdin);
        produto[i].nome[strcspn(produto[i].nome, "\n")] = '\0'; // Remove '\n'

        printf("Qual eh seu valor: ");
        scanf("%f", &produto[i].preco);
        getchar();

        produto[i].id = (rand() % 100) + 1; // Gerando ID aleatório, semelhante ao adicionarCliente
        printf("ID do produto[%d] eh: %d\n", i + 1, produto[i].id);
    }

    fseek(arquivo, 0, SEEK_END);
    fwrite(produto, sizeof(Produto), numProdutos, arquivo);
    fclose(arquivo);

    return 0;
}

//-------------------------------------- 3 --------------------------------------
int removerCliente(Pessoa **clientes, int *numClientes, FILE *arquivo)
{
    int idBuscar, i, j;
    int removidos = 0;

    if (*numClientes == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
        return 0;
    }

    printf("Digite o ID do cliente que deseja remover: ");
    scanf("%d", &idBuscar);
    getchar();

    // Ler todos os clientes do arquivo
    fseek(arquivo, 0, SEEK_SET);
    Pessoa *clientesTemp = (Pessoa *)malloc((*numClientes) * sizeof(Pessoa));
    if (!clientesTemp)
    {
        perror("Erro ao alocar memoria");
        return 1;
    }

    fread(clientesTemp, sizeof(Pessoa), *numClientes, arquivo);

    // Encontrar e remover o cliente com o ID especificado
    for (i = 0, j = 0; i < *numClientes; i++)
    {
        if (clientesTemp[i].id != idBuscar)
        {
            clientesTemp[j++] = clientesTemp[i];
        }
        else
        {
            removidos++;
        }
    }

    if (removidos > 0)
    {
        *numClientes -= removidos;
        Pessoa *clientesRealloc = realloc(*clientes, (*numClientes) * sizeof(Pessoa));
        if (*numClientes > 0 && clientesRealloc == NULL)
        {
            perror("Erro ao realocar memoria");
            free(clientesTemp);
            return 1;
        }
        *clientes = clientesRealloc;

        if (*numClientes > 0)
        {
            memcpy(*clientes, clientesTemp, (*numClientes) * sizeof(Pessoa));
        }

        // Gravar os clientes atualizados no arquivo
        fclose(arquivo);
        arquivo = fopen("cliente.dat", "wb+");
        if (!arquivo)
        {
            perror("Erro ao abrir arquivo para escrita");
            free(clientesTemp);
            return 1;
        }

        fwrite(*clientes, sizeof(Pessoa), *numClientes, arquivo);
        printf("%d cliente(s) removido(s)!\n", removidos);
    }
    else
    {
        printf("Nenhum cliente com esse ID foi encontrado!\n");
    }

    free(clientesTemp);
    return removidos > 0 ? 0 : 1;
}

//-------------------------------------- 4 --------------------------------------
int removerProduto(Produto **produto, int *numProduto, FILE *arquivo)
{
    int idBuscar, i, j;
    int removidos = 0;

    if (*numProduto == 0)
    {
        printf("Nenhum produto cadastrado.\n");
        return 0;
    }

    printf("Digite o ID do produto que deseja remover: ");
    scanf("%d", &idBuscar);
    getchar();

    // Ler todos os produtos do arquivo
    fseek(arquivo, 0, SEEK_SET);
    Produto *produtoTemp = (Produto *)malloc((*numProduto) * sizeof(Produto));
    if (!produtoTemp)
    {
        perror("Erro ao alocar memoria");
        return 1;
    }

    fread(produtoTemp, sizeof(Produto), *numProduto, arquivo);

    // Encontrar e remover o produto com o ID especificado
    for (i = 0, j = 0; i < *numProduto; i++)
    {
        if (produtoTemp[i].id != idBuscar)
        {
            produtoTemp[j++] = produtoTemp[i];
        }
        else
        {
            removidos++;
        }
    }

    if (removidos > 0)
    {
        *numProduto -= removidos;
        Produto *produtoRealloc = realloc(*produto, (*numProduto) * sizeof(Produto));
        if (*numProduto > 0 && produtoRealloc == NULL)
        {
            perror("Erro ao realocar memoria");
            free(produtoTemp);
            return 1;
        }
        *produto = produtoRealloc;

        if (*numProduto > 0)
        {
            memcpy(*produto, produtoTemp, (*numProduto) * sizeof(Produto));
        }

        // Gravar os produtos atualizados no arquivo
        fclose(arquivo);
        arquivo = fopen("produto.dat", "wb+");
        if (!arquivo)
        {
            perror("Erro ao abrir arquivo para escrita");
            free(produtoTemp);
            return 1;
        }

        fwrite(*produto, sizeof(Produto), *numProduto, arquivo);
        printf("%d produto(s) removido(s)!\n", removidos);
    }
    else
    {
        printf("Nenhum produto com esse ID foi encontrado!\n");
    }

    free(produtoTemp);
    return removidos > 0 ? 0 : 1;
}

//-------------------------------------- 5 --------------------------------------
//   ===   || //
//  || ||  ||//
//           || ||  ||\\
//   ===   || \\
//funcionando
void listarClientes(FILE *arquivo)
{
    Pessoa cliente;
    fseek(arquivo, 0, SEEK_SET);
    printf("======Pessoas Cadastradas======\n");
    while (fread(&cliente, sizeof(Pessoa), 1, arquivo) == 1)
    {
        printf("Nome: %s\n", cliente.nome);
        printf("Idade: %d\n", cliente.idade);
        printf("ID: %d\n", cliente.id);
    }
    if (ftell(arquivo) == 0)
    {
        printf("\nNenhum cliente cadastrado.\n");
    }
}

//-------------------------------------- 6 --------------------------------------
void listarProdutos(FILE *arquivo)
{
    Produto produto;
    fseek(arquivo, 0, SEEK_SET);
    printf("======Produtos Cadastrados======\n");
    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1)
    {
        printf("Nome: %s\n", produto.nome);
        printf("Preco: %.2f\n", produto.preco);
        printf("ID: %d\n", produto.id);
        printf("-----------------------------\n");
    }
    if (ftell(arquivo) == 0)
    {
        printf("\nNenhum produto cadastrado.\n");
    }
}

int menu()
{
    int opcao;

    printf("\n 1)Adicionar Cliente"); // funcionando
    printf("\n 2)Remover Cliente");   // funcionando
    printf("\n 3)Listar todos os Clientes"); // funcionando
    printf("\n 4)Buscar Cliente");
    printf("\n 5)Editar Cliente");
    printf("\n 6)Adicionar Produto"); // funcionando
    printf("\n 7)Remover Produto");
    printf("\n 8)Listar todos os Produtos"); // funcionando
    printf("\n 9)Buscar Produto");
    printf("\n 10)Editar Produto");
    printf("\n 11)Adicionar Compra");
    printf("\n 12)Remover Compra");  
    printf("\n 13)Listar todas as Compras");
    printf("\n 14)Buscar Compra");
    printf("\n 15)Gerar relatorio");
    printf("\n 0)Sair");

    printf("\n O que deseja fazer? \n");
    scanf("%d", &opcao);
    getchar();

    return opcao;
}

int main()
{
    srand(time(NULL)); // Inicializa a seed do rand() com o tempo atual, para nunca haver repetições.

    FILE *f;

    Pessoa *cliente = NULL;
    Produto *produto = NULL;
    int numClientes = 0;
    int numProdutos = 0;
    int opcao, novosClientes, novosProdutos;

    // verifica se existe o arquivo de cliente e cria se não existir
    f = fopen("cliente.dat", "rb+"); // Criação do arquivo em Binário
    if (f == NULL)
    {
        f = fopen("cliente.dat", "wb+");
        if (f == NULL)
        { // Aqui podemos continuar tendo falha na criação por causa de PERMISSÃO, ou falta de espaço no disco Local.
            exit(0);
        }
        // ALT + Z = ele faz o ajuste do conteúdo ao tamanho da tela.
        // CTRL + ; = Deixa a linha inteira como comentário, e também o comentário
        fclose(f);
    }
    else
    {
        fseek(f, 0, SEEK_END);         // Mover o ponteiro para o final do arquivo
        int tamanhoArquivo = ftell(f); // Obter o tamanho do arquivo em bytes
        rewind(f);                     // Mover o ponteiro de volta para o início do arquivo

        numClientes = tamanhoArquivo / sizeof(Pessoa); // Calcular o número de clientes no arquivo

        printf("\n%d NUMERO DE CLIENTES\n\n", numClientes);
        printf("\n%d TAMANHO DO ARQUIVO\n\n", tamanhoArquivo);
    }
    fclose(f);

    // verifica se existe o arquivo de produtos e cria se não existir
    f = fopen("produto.dat", "rb+"); // Criação do arquivo em Binário
    if (f == NULL)
    {
        f = fopen("produto.dat", "wb+");
        if (f == NULL)
        { // Aqui podemos continuar tendo falha na criação por causa de PERMISSÃO, ou falta de espaço no disco Local.
            exit(0);
        }
        fclose(f);
    }
    else
    {
        numProdutos = (int)(ftell(f) / sizeof(Produto));
    }
    fclose(f);

    // verifica se existe o arquivo de compra e cria se não existir
    f = fopen("compra.dat", "rb+"); // Criação do arquivo em Binário
    if (f == NULL)
    {
        f = fopen("compra.dat", "wb+");
        if (f == NULL)
        { // Aqui podemos continuar tendo falha na criação por causa de PERMISSÃO, ou falta de espaço no disco Local.
            exit(0);
        }
        fclose(f);
    }
    fclose(f);

    printf("\n========Bem Vindo========");

    do
    {
        opcao = menu();

        switch (opcao) {
            case 1:
                printf("Quantos clientes serao adicionados: ");
                scanf("%d", &novosClientes);
                getchar();
                cliente = realloc(cliente, (numClientes + novosClientes) * sizeof(Pessoa));
                if (cliente == NULL) {
                    printf("Erro na alocacao de memoria. \n");
                    return 1;
                }
                f = fopen("cliente.dat", "rb+");
                adicionarCliente(&cliente[numClientes], novosClientes, numClientes, f);
                numClientes += novosClientes;
                fclose(f);
                break;
        
            case 2:
                f = fopen("cliente.dat", "rb+");
                removerCliente(&cliente, &numClientes, f);
                fclose(f);
                break;
        
            case 3:
                f = fopen("cliente.dat", "rb+");
                listarClientes(f);
                fclose(f);
                break;
        
            case 4:
                //implementar
                break;
        
            case 5:
                //implementar
                break;
        
            case 6:
                printf("Quantos produtos serao adicionados: ");
                scanf("%d", &novosProdutos);
                getchar();
                produto = realloc(produto, (numProdutos + novosProdutos) * sizeof(Produto));
                if (produto == NULL) {
                    printf("Erro na alocacao de memoria. \n");
                    return 1;
                }
                f = fopen("produto.dat", "rb+");
                adicionarProduto(&produto[numProdutos], novosProdutos, numProdutos, f);
                numProdutos += novosProdutos;
                fclose(f);
                break;
        
            case 7:
                f = fopen("produto.dat", "rb+");
                removerProduto(&produto, &numProdutos, f);
                fclose(f);
                break;
        
            case 8:
                f = fopen("produto.dat", "rb+");
                listarProdutos(f);
                fclose(f);
                break;
        
            case 9:
                //implementar
                break;
        
            case 10:
                //implementar
                break;
        
            case 11:
                //implementar
                break;
        
            case 12:
                //implementar
                break;
        
            case 13:
                //implementar
                break;
        
            case 14:
                //implementar
                break;
        
            case 15:
                //implementar
                break;
        
            default:
                printf("Opcao invalida!\n");
                break;
        }

    } while (opcao != 0);
    printf("Encerrando o programa!");

    free(cliente);
    free(produto);

    return 0;
}
