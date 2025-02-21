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
    int quantidade[10];
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
int buscarCliente(Pessoa *clientes, int numClientes, FILE *arquivo)
{
    int opcao, idBuscar, i;
    char nomeBuscar[200];
    int encontrados = 0;

    if (numClientes == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
        return 1;
    }

    printf("Escolha a forma de busca:\n");
    printf("1 - Buscar por ID\n");
    printf("2 - Buscar por nome\n");
    scanf("%d", &opcao);
    getchar();

    fseek(arquivo, 0, SEEK_SET);
    Pessoa *clientesTemp = (Pessoa *)malloc(numClientes * sizeof(Pessoa));
    if (!clientesTemp)
    {
        perror("Erro ao alocar memoria");
        return 1;
    }

    fread(clientesTemp, sizeof(Pessoa), numClientes, arquivo);

    if (opcao == 1)
    {
        printf("Digite o ID do cliente que deseja buscar: ");
        scanf("%d", &idBuscar);
        getchar();

        for (i = 0; i < numClientes; i++)
        {
            if (clientesTemp[i].id == idBuscar)
            {
                printf("\n====Cliente Encontrado====\n");
                printf("ID: %d\n", clientesTemp[i].id);
                printf("Nome: %s\n", clientesTemp[i].nome);
                printf("Idade: %d\n", clientesTemp[i].idade);
                printf("-------------------------\n");
                encontrados++;
                break;
            }
        }

        if (encontrados == 0)
        {
            printf("Nenhum cliente com esse ID foi encontrado!\n");
        }
    }
    else if (opcao == 2)
    {
        printf("Digite o nome do cliente que deseja buscar: ");
        fgets(nomeBuscar, 200, stdin);
        nomeBuscar[strcspn(nomeBuscar, "\n")] = '\0';

        for (i = 0; i < numClientes; i++)
        {
            if (strcmp(clientesTemp[i].nome, nomeBuscar) == 0)
            {
                printf("\n====Cliente Encontrado====\n");
                printf("ID: %d\n", clientesTemp[i].id);
                printf("Nome: %s\n", clientesTemp[i].nome);
                printf("Idade: %d\n", clientesTemp[i].idade);
                printf("-------------------------\n");
                encontrados++;
            }
        }

        if (encontrados == 0)
        {
            printf("Nenhum cliente com esse nome foi encontrado!\n");
        }
    }
    else
    {
        printf("Opcao invalida!\n");
    }

    free(clientesTemp);
    return encontrados > 0 ? 0 : 1;
}
int editarCliente(Pessoa **clientes, int numClientes, FILE *arquivo)
{
    int idBuscar, i;
    int encontrado = 0;

    if (numClientes == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
        return 1;
    }

    printf("Digite o ID do cliente que deseja editar: ");
    scanf("%d", &idBuscar);
    getchar();

    fseek(arquivo, 0, SEEK_SET);
    Pessoa *clientesTemp = (Pessoa *)malloc(numClientes * sizeof(Pessoa));
    if (!clientesTemp)
    {
        perror("Erro ao alocar memoria");
        return 1;
    }

    fread(clientesTemp, sizeof(Pessoa), numClientes, arquivo);

    for (i = 0; i < numClientes; i++)
    {
        if (clientesTemp[i].id == idBuscar)
        {
            printf("\n====Cliente Encontrado====\n");
            printf("ID: %d\n", clientesTemp[i].id);
            printf("Nome: %s\n", clientesTemp[i].nome);
            printf("Idade: %d\n", clientesTemp[i].idade);
            printf("-------------------------\n");

            printf("Digite o novo nome do cliente: ");
            fgets(clientesTemp[i].nome, 200, stdin);
            clientesTemp[i].nome[strcspn(clientesTemp[i].nome, "\n")] = '\0';

            printf("Digite a nova idade do cliente: ");
            scanf("%d", &clientesTemp[i].idade);
            getchar();

            encontrado = 1;
            break;
        }
    }

    if (encontrado)
    {
        // Gravar os clientes atualizados no arquivo
        fclose(arquivo);
        arquivo = fopen("cliente.dat", "wb+");
        if (!arquivo)
        {
            perror("Erro ao abrir arquivo para escrita");
            free(clientesTemp);
            return 1;
        }

        fwrite(clientesTemp, sizeof(Pessoa), numClientes, arquivo);
        printf("Cliente editado com sucesso!\n");
    }
    else
    {
        printf("Nenhum cliente com esse ID foi encontrado!\n");
    }

    free(clientesTemp);
    return encontrado ? 0 : 1;
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

int buscarProduto(FILE *arquivo)
{
    int opcao, idBuscar;
    char nomeBuscar[200];
    int encontrados = 0;
    Produto produto;

    printf("Escolha a forma de busca:\n");
    printf("1 - Buscar por ID\n");
    printf("2 - Buscar por nome\n");
    scanf("%d", &opcao);
    getchar();

    fseek(arquivo, 0, SEEK_SET);

    if (opcao == 1)
    {
        printf("Digite o ID do produto que deseja buscar: ");
        scanf("%d", &idBuscar);
        getchar();

        while (fread(&produto, sizeof(Produto), 1, arquivo) == 1)
        {
            if (produto.id == idBuscar)
            {
                printf("\n====Produto Encontrado====\n");
                printf("ID: %d\n", produto.id);
                printf("Nome: %s\n", produto.nome);
                printf("Preco: %.2f\n", produto.preco);
                printf("-------------------------\n");
                encontrados++;
                break;
            }
        }

        if (encontrados == 0)
        {
            printf("Nenhum produto com esse ID foi encontrado!\n");
        }
    }
    else if (opcao == 2)
    {
        printf("Digite o nome do produto que deseja buscar: ");
        fgets(nomeBuscar, 200, stdin);
        nomeBuscar[strcspn(nomeBuscar, "\n")] = '\0';

        while (fread(&produto, sizeof(Produto), 1, arquivo) == 1)
        {
            if (strcmp(produto.nome, nomeBuscar) == 0)
            {
                printf("\n====Produto Encontrado====\n");
                printf("ID: %d\n", produto.id);
                printf("Nome: %s\n", produto.nome);
                printf("Preco: %.2f\n", produto.preco);
                printf("-------------------------\n");
                encontrados++;
            }
        }

        if (encontrados == 0)
        {
            printf("Nenhum produto com esse nome foi encontrado!\n");
        }
    }
    else
    {
        printf("Opcao invalida!\n");
    }

    return encontrados > 0 ? 0 : 1;
}

int editarProduto(FILE *arquivo)
{
    int idBuscar;
    int encontrado = 0;
    Produto produto;
    long int posicao;

    printf("Digite o ID do produto que deseja editar: ");
    scanf("%d", &idBuscar);
    getchar();

    fseek(arquivo, 0, SEEK_SET);

    while (fread(&produto, sizeof(Produto), 1, arquivo) == 1)
    {
        if (produto.id == idBuscar)
        {
            printf("\n====Produto Encontrado====\n");
            printf("ID: %d\n", produto.id);
            printf("Nome: %s\n", produto.nome);
            printf("Preco: %.2f\n", produto.preco);
            printf("-------------------------\n");

            printf("Digite o novo nome do produto: ");
            fgets(produto.nome, 200, stdin);
            produto.nome[strcspn(produto.nome, "\n")] = '\0';

            printf("Digite o novo preco do produto: ");
            scanf("%f", &produto.preco);
            getchar();

            // Voltar para a posição correta no arquivo para sobrescrever o produto
            posicao = ftell(arquivo) - sizeof(Produto);
            fseek(arquivo, posicao, SEEK_SET);
            fwrite(&produto, sizeof(Produto), 1, arquivo);

            encontrado = 1;
            break;
        }
    }

    if (encontrado)
    {
        printf("Produto editado com sucesso!\n");
    }
    else
    {
        printf("Nenhum produto com esse ID foi encontrado!\n");
    }

    return encontrado ? 0 : 1;
}
int adicionarCompra(Pessoa *clientes, int numClientes, Produto *produtos, int numProdutos, FILE *arquivoClientes, FILE *arquivoProdutos, FILE *arquivoCompras)
{
    int idCliente, idProduto, quantidade, i;
    int numProdutosCompra = 0;
    Compra novaCompra;

    // Verificar se há clientes e produtos cadastrados
    if (numClientes == 0)
    {
        printf("Nenhum cliente cadastrado. Cadastre um cliente primeiro.\n");
        return 1;
    }
    if (numProdutos == 0)
    {
        printf("Nenhum produto cadastrado. Cadastre produtos primeiro.\n");
        return 1;
    }

    // Solicitar o ID do cliente
    printf("Digite o ID do cliente que esta realizando a compra: ");
    scanf("%d", &idCliente);
    getchar();

    // Verificar se o cliente existe
    int clienteEncontrado = 0;
    fseek(arquivoClientes, 0, SEEK_SET);
    while (fread(&novaCompra.cliente, sizeof(Pessoa), 1, arquivoClientes) == 1)
    {
        if (novaCompra.cliente.id == idCliente)
        {
            clienteEncontrado = 1;
            break;
        }
    }

    if (!clienteEncontrado)
    {
        printf("Cliente com ID %d nao encontrado.\n", idCliente);
        return 1;
    }

    // Inicializar a compra
    novaCompra.idCompra = (rand() % 1000) + 1; // Gerar um ID único para a compra
    for (i = 0; i < 10; i++)
    {
        novaCompra.produtos[i].id = -1; // Inicializar os IDs dos produtos como -1 (inválido)
        novaCompra.quantidade[i] = 0;   // Inicializar as quantidades como 0
    }

    // Adicionar produtos à compra
    printf("Adicionar produtos à compra (maximo 10 produtos):\n");
    while (numProdutosCompra < 10)
    {
        printf("Digite o ID do produto %d (ou 0 para finalizar): ", numProdutosCompra + 1);
        scanf("%d", &idProduto);
        getchar();

        if (idProduto == 0)
        {
            break; // Finalizar a compra
        }

        // Verificar se o produto existe
        int produtoEncontrado = 0;
        fseek(arquivoProdutos, 0, SEEK_SET);
        while (fread(&novaCompra.produtos[numProdutosCompra], sizeof(Produto), 1, arquivoProdutos) == 1)
        {
            if (novaCompra.produtos[numProdutosCompra].id == idProduto)
            {
                produtoEncontrado = 1;
                break;
            }
        }

        if (!produtoEncontrado)
        {
            printf("Produto com ID %d nao encontrado.\n", idProduto);
            continue;
        }

        // Solicitar a quantidade do produto
        printf("Digite a quantidade do produto %d: ", idProduto);
        scanf("%d", &quantidade);
        getchar();

        if (quantidade <= 0)
        {
            printf("Quantidade invalida. Digite um valor maior que 0.\n");
            continue;
        }

        // Adicionar o produto e a quantidade à compra
        novaCompra.quantidade[numProdutosCompra] = quantidade;
        numProdutosCompra++;
    }

    if (numProdutosCompra == 0)
    {
        printf("Nenhum produto adicionado à compra. Compra cancelada.\n");
        return 1;
    }

    // Salvar a compra no arquivo
    fseek(arquivoCompras, 0, SEEK_END);
    fwrite(&novaCompra, sizeof(Compra), 1, arquivoCompras);
    fclose(arquivoCompras);

    printf("Compra registrada com sucesso! ID da compra: %d\n", novaCompra.idCompra);
    return 0;
}
int removerCompra(FILE *arquivoCompras)
{
    int idBuscar, encontrada = 0;
    Compra compra;
    long int posicao;

    // Verificar se o arquivo está vazio
    fseek(arquivoCompras, 0, SEEK_END);
    if (ftell(arquivoCompras) == 0)
    {
        printf("Nenhuma compra cadastrada.\n");
        return 1;
    }
    fseek(arquivoCompras, 0, SEEK_SET);

    // Solicitar o ID da compra a ser removida
    printf("Digite o ID da compra que deseja remover: ");
    scanf("%d", &idBuscar);
    getchar();

    // Criar um arquivo temporário para armazenar as compras que não serão removidas
    FILE *arquivoTemp = fopen("temp.dat", "wb");
    if (!arquivoTemp)
    {
        perror("Erro ao criar arquivo temporario");
        return 1;
    }

    // Ler todas as compras do arquivo original
    fseek(arquivoCompras, 0, SEEK_SET);
    while (fread(&compra, sizeof(Compra), 1, arquivoCompras) == 1)
    {
        if (compra.idCompra == idBuscar)
        {
            encontrada = 1; // Compra encontrada
            printf("Compra ID %d removida com sucesso!\n", idBuscar);
        }
        else
        {
            // Escrever a compra no arquivo temporário (exceto a compra removida)
            fwrite(&compra, sizeof(Compra), 1, arquivoTemp);
        }
    }

    fclose(arquivoCompras);
    fclose(arquivoTemp);

    if (!encontrada)
    {
        printf("Nenhuma compra com ID %d foi encontrada.\n", idBuscar);
        remove("temp.dat"); // Remove o arquivo temporário, pois não houve alterações
        return 1;
    }

    // Substituir o arquivo original pelo temporário
    remove("compra.dat");
    rename("temp.dat", "compra.dat");

    // Reabrir o arquivo original para uso posterior
    arquivoCompras = fopen("compra.dat", "rb+");
    if (!arquivoCompras)
    {
        perror("Erro ao reabrir o arquivo de compras");
        return 1;
    }

    return 0;
}
void listarCompras(FILE *arquivoCompras)
{
    Compra compra;
    int i;
    float totalCompra;

    fseek(arquivoCompras, 0, SEEK_SET); // Posiciona o ponteiro no início do arquivo

    printf("====== Lista de Compras ======\n");

    // Ler todas as compras do arquivo
    while (fread(&compra, sizeof(Compra), 1, arquivoCompras) == 1)
    {
        totalCompra = 0; // Inicializa o total da compra
        printf("\n==== Compra ID: %d ====\n", compra.idCompra);
        printf("Cliente: %s (ID: %d)\n", compra.cliente.nome, compra.cliente.id);
        printf("Produtos comprados:\n");

        // Listar os produtos da compra
        for (i = 0; i < 10; i++)
        {
            if (compra.produtos[i].id != -1) // Verifica se o produto foi adicionado
            {
                // Exibe o produto, quantidade e preço
                printf("  - Produto: %s (ID: %d)\n", compra.produtos[i].nome, compra.produtos[i].id);
                printf("    Quantidade: %d\n", compra.quantidade[i]);
                printf("    Preco: %.2f\n", compra.produtos[i].preco);

                // Calcula o subtotal do produto e adiciona ao total da compra
                totalCompra += compra.produtos[i].preco * compra.quantidade[i];
            }
        }

        // Exibe o total da compra
        printf("TOTAL: %.2f\n", totalCompra);
        printf("-------------------------\n");
    }

    // Verificar se não há compras cadastradas
    if (ftell(arquivoCompras) == 0)
    {
        printf("\nNenhuma compra cadastrada.\n");
    }
}
void buscarCompra(FILE *arquivoCompras)
{
    int idBuscar, encontrada = 0;
    Compra compra;
    float totalCompra = 0;

    // Verificar se o arquivo está vazio
    fseek(arquivoCompras, 0, SEEK_END);
    if (ftell(arquivoCompras) == 0)
    {
        printf("Nenhuma compra cadastrada.\n");
        return;
    }
    fseek(arquivoCompras, 0, SEEK_SET);

    // Solicitar o ID da compra a ser buscada
    printf("Digite o ID da compra que deseja buscar: ");
    scanf("%d", &idBuscar);
    getchar();

    // Ler todas as compras do arquivo
    while (fread(&compra, sizeof(Compra), 1, arquivoCompras) == 1)
    {
        if (compra.idCompra == idBuscar)
        {
            encontrada = 1;  // Compra encontrada
            totalCompra = 0; // Inicializa o total da compra

            // Remove caracteres especiais do nome do cliente
            removerCaracteresEspeciais(compra.cliente.nome);

            // Exibe as informações da compra
            printf("\n==== Compra ID: %d ====\n", compra.idCompra);
            printf("Cliente: %s (ID: %d)\n", compra.cliente.nome, compra.cliente.id);
            printf("Produtos comprados:\n");

            // Listar os produtos da compra
            for (int i = 0; i < 10; i++)
            {
                if (compra.produtos[i].id != -1) // Verifica se o produto foi adicionado
                {
                    // Remove caracteres especiais do nome do produto
                    removerCaracteresEspeciais(compra.produtos[i].nome);

                    // Exibe o produto, quantidade e preço
                    printf("  - Produto: %s (ID: %d)\n", compra.produtos[i].nome, compra.produtos[i].id);
                    printf("    Quantidade: %d\n", compra.quantidade[i]);
                    printf("    Preco: %.2f\n", compra.produtos[i].preco);

                    // Calcula o subtotal do produto e adiciona ao total da compra
                    totalCompra += compra.produtos[i].preco * compra.quantidade[i];
                }
            }

            // Exibe o total da compra
            printf("TOTAL: %.2f\n", totalCompra);
            printf("-------------------------\n");
            break; // Encerra a busca após encontrar a compra
        }
    }

    if (!encontrada)
    {
        printf("Nenhuma compra com ID %d foi encontrada.\n", idBuscar);
    }
}
int menu()
{
    int opcao;

    printf("\n 1)Adicionar Cliente");        // funcionando
    printf("\n 2)Remover Cliente");          // funcionando
    printf("\n 3)Listar todos os Clientes"); // funcionando
    printf("\n 4)Buscar Cliente");           // funcionando
    printf("\n 5)Editar Cliente");           // funcionando
    printf("\n 6)Adicionar Produto");        // funcionando
    printf("\n 7)Remover Produto");          // funcionando
    printf("\n 8)Listar todos os Produtos"); // funcionando
    printf("\n 9)Buscar Produto");           // funcionando
    printf("\n 10)Editar Produto");          // funcionando
    printf("\n 11)Adicionar Compra");        // funcionando
    printf("\n 12)Remover Compra");          // funcionando
    printf("\n 13)Listar todas as Compras"); // funcionando
    printf("\n 14)Buscar Compra");           // funcionando
    printf("\n 15)Editar Compra");           
    printf("\n 16)Gerar relatorio");         
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
    FILE *fClientes;
    FILE *fProdutos;
    FILE *fCompras;

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

        numClientes = tamanhoArquivo / sizeof(Pessoa);
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
        fseek(f, 0, SEEK_END);         // Mover o ponteiro para o final do arquivo
        int tamanhoArquivo = ftell(f); // Obter o tamanho do arquivo em bytes
        rewind(f);                     // Mover o ponteiro de volta para o início do arquivo
        numProdutos = tamanhoArquivo / sizeof(Produto);
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

        switch (opcao)
        {
        case 1:
            printf("Quantos clientes serao adicionados: ");
            scanf("%d", &novosClientes);
            getchar();
            cliente = realloc(cliente, (numClientes + novosClientes) * sizeof(Pessoa));
            if (cliente == NULL)
            {
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
            f = fopen("cliente.dat", "rb");
            buscarCliente(cliente, numClientes, f);
            fclose(f);
            break;

        case 5:
            f = fopen("cliente.dat", "rb+");
            editarCliente(&cliente, numClientes, f);
            fclose(f);
            break;

        case 6:
            printf("Quantos produtos serao adicionados: ");
            scanf("%d", &novosProdutos);
            getchar();
            produto = realloc(produto, (numProdutos + novosProdutos) * sizeof(Produto));
            if (produto == NULL)
            {
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
            f = fopen("produto.dat", "rb");
            buscarProduto(f);
            fclose(f);
            break;

        case 10:
            f = fopen("produto.dat", "rb+");
            editarProduto(f);
            fclose(f);
            break;

        case 11:
            fClientes = fopen("cliente.dat", "rb");
            fProdutos = fopen("produto.dat", "rb");
            fCompras = fopen("compra.dat", "ab+"); // Abre o arquivo de compras em modo de adição ao final
            if (fClientes == NULL || fProdutos == NULL || fCompras == NULL)
            {
                printf("Erro ao abrir arquivos.\n");
                return 1;
            }
            adicionarCompra(cliente, numClientes, produto, numProdutos, fClientes, fProdutos, fCompras);
            fclose(fClientes);
            fclose(fProdutos);
            fclose(fCompras);
            break;
            break;

        case 12:
            fCompras = fopen("compra.dat", "rb+");
            if (fCompras == NULL)
            {
                printf("Erro ao abrir o arquivo de compras.\n");
                return 1;
            }
            removerCompra(fCompras);
            fclose(fCompras);
            break;

        case 13:
            fCompras = fopen("compra.dat", "rb");
            if (fCompras == NULL)
            {
                printf("Erro ao abrir o arquivo de compras.\n");
                return 1;
            }
            listarCompras(fCompras);
            fclose(fCompras);
            break;

        case 14:
            fCompras = fopen("compra.dat", "rb");
            if (fCompras == NULL)
            {
                printf("Erro ao abrir o arquivo de compras.\n");
                return 1;
            }
            buscarCompra(fCompras);
            fclose(fCompras);
            break;

        case 15:
            // implementar
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
