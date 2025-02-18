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
int removerCliente(Pessoa **cliente, int *numClientes)
{
    int i, j;
    int removidos = 0;
    char nomeBuscar[200];

    if (*numClientes == 0)
    {
        printf("Nenhum cliente cadastrado. \n");
        return 0;
    }

    printf("Qual o nome do cliente para remocao: ");
    fgets(nomeBuscar, 200, stdin);

    // Remover o '\n' que pode ser capturado pelo fgets()
    nomeBuscar[strcspn(nomeBuscar, "\n")] = 0;

    /*"strcmp " compara duas strings, caso sejam iguais retorna 0,
    caso a primeira seja menor que a segundo retorna um numero negativo, e caso a primeira for maior que a segundo
    retorna um numero positivo*/
    for (i = 0; i < *numClientes; i++)
    {
        if (strcmp((*cliente)[i].nome, nomeBuscar) == 0)
        {
            //"Removerá o cliente encontrado, substituindo os dados da posição a frente nele"
            for (j = i; j < *numClientes - 1; j++)
            {
                (*cliente)[j] = (*cliente)[j + 1];
            }

            (*numClientes)--; // atualiza o n° de clientes
            i--;              // para verificar a mesma posição, ja que os dados foram substituídos pelo da frente
            removidos++;      // contador de remoção
        }
    }

    if (removidos > 0)
    { // caso há remocao realocamos o vetor de struct

        if (*numClientes > 0)
        { // E também caso o numero de clientes for maior que 0, se não o vetor de struct poderá estar vazio e com vazamento de memória
            *cliente = realloc(*cliente, (*numClientes) * sizeof(Pessoa));
        }
        else
        {
            free(*cliente);
            *cliente = NULL;
        }
        printf("%d clientes removidos! \n", removidos);
    }
    else
    {
        printf("\n============================================ \n");
        printf("Nenhum cliente com esse nome foi encontrado! \n");
        printf("============================================ \n");
        return 1;
    }
    return 0;
}

//-------------------------------------- 4 --------------------------------------
int removerProduto(Produto **produto, int *numProduto)
{
    int i, j;
    int removidos = 0;
    char produtoBuscar[200];

    if (*numProduto == 0)
    {
        printf("Nenhum produto cadastrado. \n");
        return 0;
    }

    printf("Qual o nome do produto para remocao: ");
    fgets(produtoBuscar, 200, stdin);

    // Remover o '\n' que pode ser capturado pelo fgets()
    produtoBuscar[strcspn(produtoBuscar, "\n")] = 0;

    /*"strcmp " compara duas strings, caso sejam iguais retorna 0,
    caso a primeira seja menor que a segundo retorna um numero negativo, e caso a primeira for maior que a segundo
    retorna um numero positivo*/
    for (i = 0; i < *numProduto; i++)
    {
        if (strcmp((*produto)[i].nome, produtoBuscar) == 0)
        {
            //"Removerá o produto encontrado, substituindo os dados da posição a frente nele"
            for (j = i; j < *numProduto - 1; j++)
            {
                (*produto)[j] = (*produto)[j + 1];
            }

            (*numProduto)--; // atualiza o n° de clientes
            i--;             // para verificar a mesma posição, ja que os dados foram substituídos pelo da frente
            removidos++;     // contador de remoção
        }
    }

    if (removidos > 0)
    { // caso há remocao realocamos o vetor de struct

        if (*numProduto > 0)
        { // E também caso o numero de clientes for maior que 0, se não o vetor de struct poderá estar vazio e com vazamento de memória
            *produto = realloc(*produto, (*numProduto) * sizeof(Produto));
        }
        else
        {
            free(*produto);
            *produto = NULL;
        }
        printf("\n%d Produtos removidos! \n", removidos);
    }
    else
    {
        printf("\n============================================ \n");
        printf("Nenhum produto com esse nome foi encontrado! \n");
        printf("============================================ \n");
        return 1;
    }
    return 0;
}

//-------------------------------------- 5 --------------------------------------
//   ===   || //
//  || ||  ||//
//       || ||  ||\\
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

    printf("\n 1)Adicionar Cliente");//funcionando
    printf("\n 2)Adicionar Produto");//funcionando
    printf("\n 3)Remover Cliente");
    printf("\n 4)Remover Produto");
    printf("\n 5)Listar todos os Clientes");//funcionando
    printf("\n 6)Listar todos os Produtos");//funcionando
    printf("\n 7)Buscar Cliente");
    printf("\n 8)Buscar Produto");
    printf("\n 9)Buscar Compra");
    printf("\n 10)Atualizar todos os dados");
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
        // ALT + Z = ele faz o ajuste do conteúdo ao tamanho da tela.
        // CTRL + ; = Deixa a linha inteira como comentário, e também o comentário
        fclose(f);
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
        // ALT + Z = ele faz o ajuste do conteúdo ao tamanho da tela.
        // CTRL + ; = Deixa a linha inteira como comentário, e também o comentário
        fclose(f);
    }
    fclose(f);

    printf("\n========Bem Vindo========");

    do
    {
        opcao = menu();

        switch (opcao)
        {

        case 1:
            printf("Quantos cliente serao adicionados: ");
            scanf("%d", &novosClientes);
            getchar();
            cliente = realloc(cliente, (numClientes + novosClientes) * sizeof(Pessoa));
            if (cliente == NULL)
            {
                printf("Erro na alocacao de memoria. \n");
                return 1;
            }
            f = fopen("cliente.dat", "wb+");
            adicionarCliente(&cliente[numClientes], novosClientes, numClientes, f);
            numClientes += novosClientes; // Mesma coisa que: numClientes = novosClientes + numClientes;
            // Feito para atualizar o numero max de clientes depois de add.
            fclose(f);
            break;

        case 2:
            printf("Quantos produtos serao adicionados: ");
            scanf("%d", &novosProdutos);
            getchar();

            produto = realloc(produto, (numProdutos + novosProdutos) * sizeof(Produto));
            if (produto == NULL)
            {
                printf("Erro na alocacao de memoria. \n");
                return 1;
            }
            f = fopen("produto.dat", "wb+");
            adicionarProduto(&produto[numProdutos], novosProdutos, numProdutos,f);
            numProdutos += novosProdutos;
            fclose(f);
            break;

        case 3:
            removerCliente(&cliente, &numClientes);
            break;

        case 4:
            removerProduto(&produto, &numProdutos);
            break;

        case 5:
            f = fopen("cliente.dat", "rb+");
            listarClientes(f);
            fclose(f);
            break;

        case 6:
            f = fopen("produto.dat", "rb+");
            listarProdutos(f);
            fclose(f);
            break;

        default:
            break;
        }

    } while (opcao != 0);
    printf("Encerrando o programa!");

    free(cliente);
    free(produto);

    return 0;
}
