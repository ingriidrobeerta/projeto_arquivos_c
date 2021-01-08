#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <Windows.h>


#define USUARIOS "usuarios.bin"
#define USUARIOS_TEMP "usuariosTMP.tmp"
#define ENTER  13
#define BACKSPACE 8
#define CATEGORIAS "categorias.bin"
#define CATEGORIAS_TEMP "categorias.tmp"
#define PRODUTOS "produtos.bin"
#define PRODUTOS_TEMP "produtos.tmp"
#define AVALIACOES "avaliacoes.bin"
#define AVALIACOES_TEMP "avaliacoes.tmp"


// CONFIGURAR PROMPT DE COMANDOS
void configTela() {
	system("color 1E");
	SetConsoleTitle("E-COMMERCE");
	HWND hwnd = FindWindow(NULL, "E-COMMERCE");
	SendMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	setlocale(LC_ALL, "portuguese");
	system("TITLE E-COMMERCE");
	system("chcp 1252 > nul");
}


// STRUCTS
struct Usuario {
	int codigo;
	char nome[51];
	char login[15];
	char senha[15];
	int idade;
	char sexo;
	char telefone[13];
};
struct Categoria {
	int codigo;
	char nome[51];
};
struct Produto {
	int codigo;
	char titulo[100];
	char descricao[256];
	int categoria;
	float preco;
};
struct Avaliacao {
	int codigoUsuario;
	int codigoProduto;
	int nota;
};


// AUXILIAR
void telefone(char tel[]) {
	int i = 0, test = 4;
	printf("Telefone: ");
	while (1) {
		tel[i] = getch();
		if (tel[i] == ENTER)
			break;
		if (tel[i] != BACKSPACE) {
			if (i == 0)
				printf("(");
			if (i == 3)
				printf(") ");
			if (i == 8)
				printf("-");
			printf("%c", tel[i]);
			if (i == 11) {
				tel[12] = '\0';
				break;
			}
			i++;
			if (i <= 2)
				test = 1;
			else if (i <= 6)
				test = 3;
			else
				test = 4;
		}
		else if (tel[i] == BACKSPACE && i >= 1) {
			tel[i] = '\0';
			printf("\b \b");
			i--;
		}
		else if (tel[i] == BACKSPACE && test > 0) {
			tel[i] = '\0';
			printf("\b \b");
			test--;
			if (test == 0)
				i = 0;
		}
	}
}
void senha(char vet[]) {
	int i = 0;
	printf("Senha: ");
	fflush(stdin);
	while (1) {
		vet[i] = getch();
		if (vet[i] == ENTER) {
			vet[i] = '\0';
			break;
		}
		if (vet[i] != BACKSPACE) {
			printf("*");
			i++;
		}
		if (i == 14) {
			vet[i] = '\0';
			break;
		}
		else if (vet[i] == BACKSPACE && i >= 1) {
			vet[i] = '\0';
			i--;
			printf("\b \b");
		}
	}
}


// AVALIAÇÃO
struct Usuario login(char *nomeArq) {
	FILE *arq;
	char login[15], pass[15];
	struct Usuario usuario;
	int flag = 0;
	if (!(arq = fopen(nomeArq, "a+b"))) {
		printf("Erro ao tentar abrir o arquivo para leitura!\n");
		return;
	}
	getchar();
	do {
		fseek(arq, 0, SEEK_SET);
		system("cls");
		printf("\n\n\t\t\t      Hora: ");
		system("time/t");
		printf("\t********************************************************\n");
		printf("\t*                                                      *\n");
		printf("\t*                    L  O  G  I  N                     *\n");
		printf("\t*                                                      *\n");
		printf("\t********************************************************\n\n");
		do {
			printf("Login: ");
			fgets(login, 15, stdin);
			if (login[0] == '\n')
				printf("Login obrigatório!\n");
		} while (login[0] == '\n');
		login[strlen(login) - 1] = '\0';
		do {
			senha(pass);
			if (pass[0] == '\0')
				printf("Senha obrigatória!\n");
			if (!validarSenha(pass))
				printf("\nSenha possui caracter inválido!\nUse somente letras e números!\n");
		} while (!validarSenha(pass) || pass[0] == '\0');
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		while (!feof(arq)) {
			if (!strcmp(login, usuario.login)) {
				if (!strcmp(pass, usuario.senha)) {
					system("cls");
					printf("\n\n\t\t\t      Hora: ");
					system("time/t");
					printf("\t********************************************************\n");
					printf("\t*                                                      *\n");
					printf("\t*                    L  O  G  I  N                     *\n");
					printf("\t*                                                      *\n");
					printf("\t********************************************************\n\n");
					printf("\nOlá, %s!\nLogin efetuado com sucesso!!\n \n", usuario.nome);
					system("pause");
					if (fclose(arq)) {
						printf("\nErro ao tentar fechar o arquivo!\n");
					}
					return usuario;
				}
				else
					flag = 1;
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
		}
		if (flag == 1)
			printf("\nSenha incorreta\n");
		else
			printf("\nUsuário inexistente\n");
		system("PAUSE");
	} while (1);
	if (fclose(arq)) {
		printf("\nErro ao tentar fechar o arquivo!\n");
	}
	system("PAUSE");
	return;
}
int buscarAvaliacao(int cod, int codProd) {
	FILE *arq;
	struct Usuario usuario;
	struct Avaliacao avaliacao;

	if (!(arq = fopen(AVALIACOES, "rb"))) {
		printf("Erro ao abrir arquivo para gravação");
	}

	fread(&avaliacao, sizeof(struct Avaliacao), 1, arq);
	while (!feof(arq)) {
		if (codProd == avaliacao.codigoProduto) {
			if (cod == avaliacao.codigoUsuario) {
				return 0;
			}
		}
		fread(&avaliacao, sizeof(struct Avaliacao), 1, arq);
	}
	return 1;
}
int validarCodigoConsProd(int cod) {
	struct Produto produto;
	FILE *arq;
	if (!(arq = fopen(PRODUTOS, "rb"))) {
		printf("Erro ao tentar abrir o arquivo para leitura!\n");
		return;
	}
	fread(&produto, sizeof(struct Produto), 1, arq);
	while (!feof(arq)) {
		if (cod == produto.codigo)
			return 0;
		fread(&produto, sizeof(struct Produto), 1, arq);
	}
	return 1;
}
struct Produto consultarProdutosAvaliacao(char *nomeArq) {
	struct Produto produto;
	FILE *arq;
	int op, codigo;
	char titulo[100], descricao[257];
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir arquivo para leitura!\n");
		return;
	}
	system("cls");
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*     C O N S U L T A     D E     P R O D U T O S      *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\nPelo: \n");
	printf("(1) -- Codigo\n");
	printf("(2) -- Parte do titulo\n");
	printf("(3) -- Voltar\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op > 3);
	system("cls");
	switch (op) {
	case 1:
		do {
			printf("Código: ");
			scanf("%i", &codigo);
			if (validarCodigoConsProd(codigo) == 1) {
				system("cls");
				printf("Produto não consta em nosso banco de dados, digite um novo ");
			}
		} while (codigo < 1 || validarCodigoConsProd(codigo) == 1);
		system("cls");
		fread(&produto, sizeof(struct Produto), 1, arq);
		while (!feof(arq)) {
			if (codigo == produto.codigo) {
				printf("=========================================================\n");
				printf("Código %i\n", produto.codigo);
				printf("Titulo: %s\n", produto.titulo);
				printf("Descrição: %s\n", produto.descricao);
				printf("Categoria: %i\n", produto.categoria);
				printf("Preço: %.2f\n", produto.preco);
				break;
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
		}
		printf("=========================================================\n");
		break;
	case 2:
		printf("Informe parte do titulo do produto: ");
		getchar();
		fgets(titulo, 51, stdin);
		titulo[strlen(titulo) - 1] = '\0';
		system("cls");
		fread(&produto, sizeof(struct Produto), 1, arq);
		while (!feof(arq)) {
			if (strstr(produto.titulo, titulo)) {
				printf("=========================================================\n");
				printf("Código %i\n", produto.codigo);
				printf("Titulo: %s\n", produto.titulo);
				printf("Descrição: %s\n", produto.descricao);
				printf("Categoria: %i\n", produto.categoria);
				printf("Preço: %.2f\n", produto.preco);
				printf("\n");
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
		}
		break;
	}
	return produto;
	if (fclose(arq)) {
		printf("\nErro ao tentar fechar arquivo!\n");
	}
}
void avaliacao(char *nomeArq) {
	FILE *arq;
	struct Avaliacao avaliacao;
	struct Usuario usuario;
	struct Produto produto;
	int conf;
	usuario = login(USUARIOS);
	do {
		if (!(arq = fopen(nomeArq, "a+b"))) {
			printf("Erro ao abrir arquivo para gravação");
		}
		do {
			produto = consultarProdutosAvaliacao(PRODUTOS);
			avaliacao.codigoProduto = produto.codigo;
			avaliacao.codigoUsuario = usuario.codigo;
			if (buscarAvaliacao(usuario.codigo, produto.codigo) == 0) {
				printf("Você já avaliou este produto!\n");
				printf("Selecione outro produto para avaliar.\n\n");
				system("PAUSE");
			}
		} while (buscarAvaliacao(usuario.codigo, produto.codigo) == 0);
		do {
			printf("\nDigite sua nota para este produto (1 a 5): ");
			scanf("%i", &avaliacao.nota);
			if (avaliacao.nota == 1) {
				printf("\n* (1)\n");
				printf("1 -- Confirmar avaliação\n");
				printf("2 -- Alterar avaliação\n");
				scanf("%i", &conf);
				system("cls");
			}
			else if (avaliacao.nota == 2) {
				printf("\n* * (2)\n");
				printf("1 -- Confirmar avaliação\n");
				printf("2 -- Alterar avaliação\n");
				scanf("%i", &conf);
				system("cls");
			}
			else if (avaliacao.nota == 3) {
				printf("\n* * * (3)\n");
				printf("1 -- Confirmar avaliação\n");
				printf("2 -- Alterar avaliação\n");
				scanf("%i", &conf);
				system("cls");
			}
			else if (avaliacao.nota == 4) {
				printf("\n* * * * (4)\n");
				printf("1 -- Confirmar avaliação\n");
				printf("2 -- Alterar avaliação\n");
				scanf("%i", &conf);
				system("cls");
			}
			else if (avaliacao.nota == 5) {
				printf("\n* * * * * (5)\n");
				printf("1 -- Confirmar avaliação\n");
				printf("2 -- Alterar avaliação\n");
				printf("Opção: ");
				scanf("%i", &conf);
				system("cls");
			}
		} while (avaliacao.nota > 5 || avaliacao.nota < 1 || conf == 2);
		system("cls");
		printf("\n\n\t\t\t      Hora: ");
		system("time/t");
		printf("\t********************************************************\n");
		printf("\t*                                                      *\n");
		printf("\t*           DESEJA FAZER UMA NOVA AVALIAÇÃO?           *\n");
		printf("\t*                                                      *\n");
		printf("\t********************************************************\n\n");
		printf("(1) -- Sim\n");
		printf("(2) -- Não\n");
		do {
			printf("Opção: ");
			scanf("%i", &conf);
			if (conf < 1 || conf > 2) {
				printf("Opção inválida. Digite uma nova ");
			}
		} while (conf < 1 || conf > 2);
		if (fwrite(&avaliacao, sizeof(struct Avaliacao), 1, arq) != 1) {
			printf("Erro ao salvar avaliação!");
		}
		if (fclose(arq)) {
			printf("Erro ao fechar arquivo!");
		}
	} while (conf == 1);
	if (conf == 2)
		return;
}


// VALIDAÇÕES
int validarTel(char tel[]) {
	int i;
	for (i = 0; i < strlen(tel); i++) { // Verifica se foi digitado letras no campo de telefone
		if (tel[i]<'0' || tel[i]>'9')
			return 0;
	}
	return 1;
}
int validarCodigoUsuario(int cod, char *nomeArq) {
	struct Usuario usuario;
	FILE *arq;
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir arquivo para leitura!\n");
		return;
	}
	fread(&usuario, sizeof(usuario), 1, arq);
	while (!feof(arq)) {
		if (cod == usuario.codigo) // Valida se o código de usuário já existe (para não ocorrer duplicação)
			return 0;
		fread(&usuario, sizeof(usuario), 1, arq);
	}
	return 1;
}
int validarCodigoCategoria(int cod, char *nomeArq) {
	struct Categoria categoria;
	FILE *arq;
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir arquivo para leitura!\n");
		return;
	}
	fread(&categoria, sizeof(categoria), 1, arq);
	while (!feof(arq)) {
		if (cod == categoria.codigo) // Valida se a categoria existe, para não ser digitado um código inexistente
			return 0;
		fread(&categoria, sizeof(categoria), 1, arq);
	}
	return 1;
}
int validarCodigoProduto(int cod, char *nomeArq) {
	struct Produto produto;
	FILE *arq;
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir arquivo para leitura!\n");
		return;
	}
	fread(&produto, sizeof(produto), 1, arq);
	while (!feof(arq)) {
		if (cod == produto.codigo) // Valida se o código do produto existe, para não ser digitado um código inexistente
			return 0;
		fread(&produto, sizeof(produto), 1, arq);
	}
	return 1;
}
int validarLogin(char *login, char *nomeArq) {
	struct Usuario usuario;
	FILE *arq;
	if (!(arq = fopen(nomeArq, "a+b"))) {
		printf("Erro ao tentar abrir arquivo para gravação!\n");
		return;
	}
	fread(&usuario, sizeof(usuario), 1, arq);
	while (!feof(arq)) {
		if (strcmp(login, usuario.login) == 0) // Valida se o login existe
			return 0;
		fread(&usuario, sizeof(usuario), 1, arq);
	}
	return 1;
}
int validarSenha(char vet[]) {
	int i;
	for (i = 0; i < strlen(vet); i++) {
		if (!(isalpha(vet[i]) || isdigit(vet[i]))) { // Valida se no campo de senha, está sendo digitado apenas letras e números)
			return 0;
		}
	}
	return 1;
}
int validarNome(char nome[]) {
	int i;
	for (i = 0; i < strlen(nome) - 1; i++) {
		if (!(isalpha(nome[i]) || nome[i] == ' ')) { // Valida se está sendo digitado apenas letras e espaços
			return 0;
		}
	}
	return 1;
}
int validarCategoria(int cat) {
	struct Categoria categoria;
	FILE *arq;
	if (!(arq = fopen(CATEGORIAS, "rb"))) {
		printf("Erro ao tentar abrir o arquivo para leitura!\n");
		return;
	}
	fread(&categoria, sizeof(categoria), 1, arq);
	while (!feof(arq)) {
		if (cat == categoria.codigo) // Valida se a categoria existe, para não ser digitada uma inexistente
			return 0;
		fread(&categoria, sizeof(categoria), 1, arq);
	}
	return 1;
}


// LISTAR
void listarUsuario(char *nomeArq) {
	FILE *arq, *arqAux, *arqAux2;
	struct Usuario usuario;
	struct Avaliacao avaliacao;
	struct Produto produto;
	int i;
	if (!(arq = fopen(USUARIOS, "a+b"))) {
		printf("Erro ao abrir arquivo para leitura\n");
		return;
	}
	if (!(arqAux = fopen(AVALIACOES, "a+b"))) {
		printf("Erro ao abrir arquivo para leitura\n");
		return;
	}
	if (!(arqAux2 = fopen(PRODUTOS, "a+b"))) {
		printf("Erro ao abrir arquivo para leitura\n");
		return;
	}
	fseek(arq, 0, SEEK_SET);
	fseek(arqAux, 0, SEEK_SET);
	fseek(arqAux2, 0, SEEK_SET);
	fread(&usuario, sizeof(struct Usuario), 1, arq);
	printf("=========================================================\n");
	while (!feof(arq)) {
		printf("USUÁRIO %i\n", usuario.codigo);
		printf("Nome: %s", usuario.nome);
		printf("\nLogin: %s", usuario.login);
		printf("\nSenha: %s", usuario.senha);
		printf("\nIdade: %i", usuario.idade);
		printf("\nSexo: %c\n", usuario.sexo);
		fseek(arqAux, 0, SEEK_SET);
		fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
		printf("\nAvaliações: \n");
		printf("*********************************************************\n");
		while (!feof(arqAux)) {
			if (usuario.codigo == avaliacao.codigoUsuario) {
				fseek(arqAux2, 0, SEEK_SET);
				fread(&produto, sizeof(struct Produto), 1, arqAux2);
				while (!feof(arqAux2)) {
					if (produto.codigo == avaliacao.codigoProduto) {
						printf("Produto: %s\n", produto.titulo);
						printf("   Nota: %i", avaliacao.nota);
						printf("\n*********************************************************\n");
					}
					fread(&produto, sizeof(struct Produto), 1, arqAux2);
				}
			}
			fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
		}
		printf("\nTelefone: ");
		for (i = 0; i < strlen(usuario.telefone); i++) {
			if (i == 0)
				printf("(");
			if (i == 3)
				printf(") ");
			if (i == 8)
				printf("-");
			printf("%c", usuario.telefone[i]);
		}
		printf("\n=========================================================\n");
		fread(&usuario, sizeof(struct Usuario), 1, arq);
	}
	if (fclose(arq)) {
		printf("\nErro ao fechar arquivo!\n");
	}
	if (fclose(arqAux)) {
		printf("\nErro ao fechar arquivo!\n");
	}
	if (fclose(arqAux2)) {
		printf("\nErro ao fechar arquivo!\n");
	}
	printf("\n");
}
void listarCategoria(char *nomeArq) {
	FILE *arq;
	struct Categoria categoria;
	if (!(arq = fopen(nomeArq, "a+b"))) {
		printf("Erro ao abrir arquivo para leitura!\n");
		return;
	}
	fseek(arq, 0, SEEK_SET);
	fread(&categoria, sizeof(struct Categoria), 1, arq);
	printf("CATEGORIAS:\n");
	while (!feof(arq)) {
		printf("=========================================================\n");
		printf("Categoria %i\n", categoria.codigo);
		printf("%s", categoria.nome);
		printf("\n");
		fread(&categoria, sizeof(struct Categoria), 1, arq);
	}
	printf("=========================================================\n\n");
	if (fclose(arq)) {
		printf("\nErro ao fechar arquivo!\n");
	}
}
void listarProduto(char *nomeArq) { // FALTANDO FAZER AS OPÇÕES DE ORDENAMENTO.
	FILE *arq;
	FILE *arqAux;
	struct Produto produto;
	struct Categoria categoria;
	int i;
	if (!(arq = fopen(nomeArq, "rb")))
		printf("Erro ao abrir arquivo de produtos para leitura!");
	if (!(arqAux = fopen(CATEGORIAS, "rb")))
		printf("Erro ao abrir arquivo de categorias para leitura!");
	fread(&categoria, sizeof(struct Categoria), 1, arqAux);
	fread(&produto, sizeof(struct Produto), 1, arq);
	printf("=========================================================\n");
	while (!feof(arq)) {
		printf("PRODUTO %i\n", produto.codigo);
		printf("Título: %s", produto.titulo);
		printf("\nDescrição: %s", produto.descricao);
		fseek(arqAux, 0, SEEK_SET);
		do {
			fread(&categoria, sizeof(struct Categoria), 1, arqAux);
			if (produto.categoria == categoria.codigo) {
				printf("\nCategoria: %s", categoria.nome);
			}
		} while (!(feof(arqAux)) && (produto.categoria != categoria.codigo));
		printf("\nPreço: %.2f", produto.preco);
		printf("\n=========================================================\n");
		fread(&produto, sizeof(struct Produto), 1, arq);
	}
	if (fclose(arq)) {
		printf("\nErro ao fechar arquivo de produtos!\n");
	}
	if (fclose(arqAux)) {
		printf("\nErro ao fechar arquivo categorias!\n");
	}
	printf("\n");
}


// CADASTROS
void cadastroUsuario(char *nomeArq) {
	int i;
	struct Usuario usuario;
	FILE *user;
	if (!(user = fopen(nomeArq, "a+b"))) {
		printf("Erro ao tentar abrir arquivo para gravação!\n");
		return;
	}
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*        C A D A S T R O   D E   U S U Á R I O S       *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	do {
		printf("Código: ");
		scanf("%i", &usuario.codigo);
		getchar();
		if (validarCodigoUsuario(usuario.codigo, USUARIOS) == 0)
			printf("Codigo já existente\nInforme novamente o ");
	} while (usuario.codigo < 0 || validarCodigoUsuario(usuario.codigo, USUARIOS) == 0);
	do {
		fflush(stdin);
		printf("Nome: ");
		fgets(usuario.nome, 51, stdin);
		if (!validarNome(usuario.nome))
			printf("Caracter inválido!\n");
		if (usuario.nome[0] == '\n')
			printf("Nome obrigatório!\n");
		usuario.nome[strlen(usuario.nome) - 1] = '\0';
	} while (!validarNome(usuario.nome) || usuario.nome[0] == '\n');
	do {
		printf("Login: ");
		fgets(usuario.login, 15, stdin);
		usuario.login[strlen(usuario.login) - 1] = '\0';
		if (validarLogin(usuario.login, USUARIOS) == 0)
			printf("Login já existente\n");
		if (usuario.login[0] == '\n')
			printf("Login obrigatório!\n");
	} while (usuario.login[0] == '\n' || validarLogin(usuario.login, USUARIOS) == 0);
	do {
		senha(usuario.senha);
		if (usuario.senha[0] == '\0')
			printf("Senha obrigatória!\n");
		if (!validarSenha(usuario.senha))
			printf("\nSenha possui caracter inválido!\nUse somente letras e números!\n");
	} while (!validarSenha(usuario.senha) || usuario.senha[0] == '\0');
	do {
		printf("\nIdade: ");
		scanf("%i", &usuario.idade);
		if (usuario.idade > 120)
			printf("\nIdade inválida!");
	} while (usuario.idade > 120);
	do {
		printf("Sexo (M = Masculino | F = Feminino): ");
		scanf(" %c", &usuario.sexo);
		usuario.sexo = toupper(usuario.sexo);
		if (usuario.sexo == '\n')
			printf("Sexo obrigatório!\n");
		if (usuario.sexo != 'M' && usuario.sexo != 'F')
			printf("Caracter inválido, use M para masculino e F para feminino");
	} while ((usuario.sexo != 'M' && usuario.sexo != 'F') || usuario.sexo == '\n');
	do {
		telefone(usuario.telefone);
		if (usuario.telefone == '\n')
			printf("Telefone obrigatório!\n");
		if (!validarTel(usuario.telefone))
			printf("\nTelefone inválido!");
	} while (!validarTel(usuario.telefone) || usuario.telefone == '\n');

	if (fwrite(&usuario, sizeof(struct Usuario), 1, user) != 1) {
		printf("\nErro ao salvar usuário!\n");
	}
	else {
		printf("\nUsuário cadastrado com sucesso!\n");
	}
	if (fclose(user)) {
		printf("\nErro ao fechar arquivo!\n");
	}
}
void cadastroCategoria(char *nomeArq) {
	FILE *arq;
	struct Categoria categoria;
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*      C A D A S T R O   D E   C A T E G O R I A S     *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	if (!(arq = fopen(nomeArq, "a+b"))) {
		printf("Erro ao abrir arquivo para gravação.");
	}
	do {
		printf("Código: ");
		scanf("%i", &categoria.codigo);
		getchar();
		if (validarCodigoCategoria(categoria.codigo, CATEGORIAS) == 0)
			printf("Codigo já existente\nInforme novamente o ");
	} while (categoria.codigo < 0 || validarCodigoCategoria(categoria.codigo, CATEGORIAS) == 0);
	do {
		printf("Categoria: ");
		fgets(categoria.nome, 51, stdin);
		if (!validarNome(categoria.nome))
			printf("Caracter inválido!\n");
		if (categoria.nome[0] == '\n')
			printf("Categoria obrigatória!\n");
		categoria.nome[strlen(categoria.nome) - 1] = '\0';
	} while (!validarNome(categoria.nome) || categoria.nome[0] == '\n');
	if (fwrite(&categoria, sizeof(struct Categoria), 1, arq) != 1) {
		printf("\nErro ao salvar a categoria!\n");
	}
	else {
		printf("\nCategoria cadastrada com sucesso!\n\n");
	}
	if (fclose(arq)) {
		printf("\nErro ao fechar arquivo!\n");
	}
}
void cadastroProduto(char *nomeArq) {
	int i;
	struct Produto produto;
	FILE *user;
	if (!(user = fopen(nomeArq, "a+b"))) {
		printf("Erro ao tentar abrir o arquivo para gravação!\n");
		return;
	}
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*        C A D A S T R O   D E   P R O D U T O S       *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	do {
		printf("Código do produto: ");
		scanf("%i", &produto.codigo);
		if (validarCodigoProduto(produto.codigo, PRODUTOS) == 0)
			printf("Codigo já existente\nInforme novamente o ");
	} while (produto.codigo < 0 || validarCodigoProduto(produto.codigo, PRODUTOS) == 0);
	do {
		getchar();
		printf("Título: ");
		fgets(produto.titulo, 100, stdin);
		if (produto.titulo[0] == '\n')
			printf("Título obrigatório!\n");
		produto.titulo[strlen(produto.titulo) - 1] = '\0';
	} while (produto.titulo[0] == '\n');
	do {
		printf("Descrição: ");
		fgets(produto.descricao, 256, stdin);
		if (produto.descricao[0] == '\n')
			printf("Nome obrigatório!\n");
		produto.descricao[strlen(produto.descricao) - 1] = '\0';
	} while (produto.descricao[0] == '\n');
	do {
		printf("\nDigite 0 para listar categorias.\n");
		printf("Código da categoria: ");
		scanf("%i", &produto.categoria);
		if (produto.categoria == 0) {
			system("cls");
			listarCategoria(CATEGORIAS);
			printf("Código da categoria: ");
			scanf("%i", &produto.categoria);
		}
		if (validarCategoria(produto.categoria, PRODUTOS) == 1) {
			printf("Esta categoria não consta em nosso banco de dados!\nDigite novamente o ");
		}
	} while (validarCategoria(produto.categoria, PRODUTOS) == 1 || produto.categoria < 1);
	do {
		printf("Preço: ");
		scanf("%f", &produto.preco);
		getchar();
		if (produto.preco < 0)
			printf("\nPreço inválido! Digite um valor acima de 0.\n");
	} while (produto.preco < 0);

	if (fwrite(&produto, sizeof(struct Produto), 1, user) != 1) {
		printf("\nErro ao salvar produto!\n");
	}
	else {
		printf("Produto cadastrado com sucesso!\n\n");
	}
	if (fclose(user)) {
		printf("\nErro ao fechar arquivo!\n");
	}
}


// CONSULTAS
void consultarUsuario(char *nomeArq) {
	struct Usuario usuario;
	FILE *arq;
	int op, i;
	char tel[13], login[15], nome[51];
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir o arquivo para gravação!\n");
		return;
	}
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*       C O N S U L T A   D E   U S U Á R I O S        *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\nConsultar pelo: \n");
	printf("(1) -- Telefone\n");
	printf("(2) -- Login\n");
	printf("(3) -- Parte do nome\n\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op> 3);
	system("cls");
	switch (op) {
	case 1:
		telefone(tel);
		system("cls");
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		while (!feof(arq)) {
			if (!strcmp(tel, usuario.telefone)) {
				printf("=========================================================\n");
				printf("USUÁRIO %i\n", usuario.codigo);
				printf("Nome: %s", usuario.nome);
				printf("\nLogin: %s", usuario.login);
				printf("\nSenha: %s", usuario.senha);
				printf("\nIdade: %i", usuario.idade);
				printf("\nSexo: %c\n", usuario.sexo);
				printf("Telefone: ");
				for (i = 0; i < strlen(usuario.telefone); i++) {
					if (i == 0)
						printf("(");
					if (i == 3)
						printf(") ");
					if (i == 8)
						printf("-");
					printf("%c", usuario.telefone[i]);
				}
				printf("\n=========================================================\n");
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
		}
		break;
	case 2:
		getchar();
		printf("Login: ");
		fgets(login, 15, stdin);
		login[strlen(login) - 1] = '\0';
		system("cls");
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		while (!feof(arq)) {
			if (!strcmp(login, usuario.login)) {
				printf("=========================================================\n");
				printf("USUARIO %i\n", usuario.codigo);
				printf("Nome: %s", usuario.nome);
				printf("\nLogin: %s", usuario.login);
				printf("\nSenha: %s", usuario.senha);
				printf("\nIdade: %i", usuario.idade);
				printf("\nSexo: %c\n", usuario.sexo);
				printf("Telefone: ");
				for (i = 0; i < strlen(usuario.telefone); i++) {
					if (i == 0)
						printf("(");
					if (i == 3)
						printf(") ");
					if (i == 8)
						printf("-");
					printf("%c", usuario.telefone[i]);
				}
				printf("\n=========================================================\n");
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
		}
		break;
	case 3:
		getchar();
		printf("Parte do nome: ");
		fgets(nome, 15, stdin);
		nome[strlen(nome) - 1] = '\0';
		system("cls");
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		while (!feof(arq)) {
			if (strstr(usuario.nome, nome)) {
				printf("=========================================================\n");
				printf("USUARIO %i\n", usuario.codigo);
				printf("Nome: %s", usuario.nome);
				printf("\nLogin: %s", usuario.login);
				printf("\nSenha: %s", usuario.senha);
				printf("\nIdade: %i", usuario.idade);
				printf("\nSexo: %c\n", usuario.sexo);
				printf("Telefone: ");
				for (i = 0; i < strlen(usuario.telefone); i++) {
					if (i == 0)
						printf("(");
					if (i == 3)
						printf(") ");
					if (i == 8)
						printf("-");
					printf("%c", usuario.telefone[i]);
				}
				printf("\n=========================================================\n");
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
		}
		break;
	}
	if (fclose(arq)) {
		printf("\nErro ao fechar arquivo!\n");
	}
}
void consultarCategoria(char *nomeArq) {
	struct Categoria categoria;
	FILE *arq;
	int op, codigo;
	char nome[51];
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir arquivo para leitura!\n");
		return;
	}
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*     C O N S U L T A   D E   C A T E G O R I A S      *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\nPelo: \n");
	printf("(1) -- Código\n");
	printf("(2) -- Parte do nome\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op> 2);
	system("cls");
	switch (op) {
	case 1:
		do {
			printf("Código: ");
			scanf("%i", &codigo);
		} while (codigo < 1);
		system("cls");
		fread(&categoria, sizeof(struct Categoria), 1, arq);
		printf("=========================================================\n");
		while (!feof(arq)) {
			if (codigo == categoria.codigo) {
				printf("Código %i\n", categoria.codigo);
				printf("Categoria: %s", categoria.nome);
				printf("\n");
				printf("=========================================================\n\n");
			}
			fread(&categoria, sizeof(struct Categoria), 1, arq);
		}
		break;
	case 2:
		printf("Informe parte do nome da categoria: ");
		getchar();
		fgets(nome, 51, stdin);
		nome[strlen(nome) - 1] = '\0';
		system("cls");
		fread(&categoria, sizeof(struct Categoria), 1, arq);
		printf("=========================================================\n");
		while (!feof(arq)) {
			if (strstr(categoria.nome, nome)) {
				printf("Codigo %i\n", categoria.codigo);
				printf("Categoria: %s", categoria.nome);
				printf("\n");
				printf("=========================================================\n");
			}
			fread(&categoria, sizeof(struct Categoria), 1, arq);
		}
		break;
	}
	if (fclose(arq)) {
		printf("\nErro ao tentar fechar arquivo!\n");
	}
}
void consultarProdutos(char *nomeArq) {
	struct Produto produto;
	struct Avaliacao avaliacao;
	FILE *arq, *arqAux;
	int op, codigo, cont = 0;
	float media;
	char titulo[100], descricao[257];
	if (!(arq = fopen(nomeArq, "a+b"))) {
		printf("Erro ao tentar abrir arquivo para leitura!\n");
		return;
	}
	if (!(arqAux = fopen(AVALIACOES, "a+b"))) {
		printf("Erro ao tentar abrir arquivo para leitura!\n");
		return;
	}
	fseek(arqAux, 0, SEEK_SET);
	fseek(arq, 0, SEEK_SET);
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*     C O N S U L T A     D E     P R O D U T O S      *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\nPelo: \n");
	printf("(1) -- Codigo\n");
	printf("(2) -- Parte do titulo\n");
	printf("(3) -- Parte da descrição\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op> 3);
	system("cls");
	switch (op) {
	case 1:
		do {
			printf("Código: ");
			scanf("%i", &codigo);
		} while (codigo < 1);
		system("cls");
		fread(&produto, sizeof(struct Produto), 1, arq);
		printf("=========================================================\n");
		while (!feof(arq)) {
			if (codigo == produto.codigo) {
				fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
				media = 0;
				cont = 0;
				while (!feof(arqAux)) {
					if (produto.codigo == avaliacao.codigoProduto) {
						cont++;
						media = media + avaliacao.nota;
					}
					fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
				}
				media = media / cont;
				printf("Código %i\n", produto.codigo);
				printf("Titulo: %s\n", produto.titulo);
				printf("Descrição: %s\n", produto.descricao);
				printf("Categoria: %i\n", produto.categoria);
				printf("Preço: %.2f\n", produto.preco);
				printf("Media de avaliacoes: %.1f", media);
				printf("\n");
				printf("=========================================================\n\n");
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
		}
		break;
	case 2:
		printf("Informe parte do titulo do produto: ");
		getchar();
		fgets(titulo, 51, stdin);
		titulo[strlen(titulo) - 1] = '\0';
		system("cls");
		fread(&produto, sizeof(struct Produto), 1, arq);
		printf("=========================================================\n");
		while (!feof(arq)) {
			if (strstr(produto.titulo, titulo)) {
				fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
				media = 0;
				while (!feof(arqAux)) {
					if (produto.codigo == avaliacao.codigoProduto) {
						cont++;
						media = media + avaliacao.nota;
					}
					fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
				}
				media = media / cont;
				printf("Código %i\n", produto.codigo);
				printf("Titulo: %s\n", produto.titulo);
				printf("Descrição: %s\n", produto.descricao);
				printf("Categoria: %i\n", produto.categoria);
				printf("Preço: %.2f\n", produto.preco);
				printf("Media de avaliacoes: %.1f", media);
				printf("\n");
				printf("=========================================================\n\n");
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
		}
		break;
	case 3:
		printf("Informe parte da descrição do produto: ");
		getchar();
		fgets(descricao, 51, stdin);
		descricao[strlen(descricao) - 1] = '\0';
		system("cls");
		fread(&produto, sizeof(struct Produto), 1, arq);
		printf("=========================================================\n");
		while (!feof(arq)) {
			if (strstr(produto.descricao, descricao)) {
				fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
				media = 0;
				while (!feof(arqAux)) {
					if (produto.codigo == avaliacao.codigoProduto) {
						cont++;
						media = media + avaliacao.nota;
					}
					fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAux);
				}
				media = media / cont;
				printf("Código %i\n", produto.codigo);
				printf("Titulo: %s\n", produto.titulo);
				printf("Descrição: %s\n", produto.descricao);
				printf("Categoria: %i\n", produto.categoria);
				printf("Preço: %.2f\n", produto.preco);
				printf("Media de avaliações: %.1f", media);
				printf("\n");
				printf("=========================================================\n\n");
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
		}
		break;
	}
	if (fclose(arq)) {
		printf("\nErro ao tentar fechar arquivo!\n");
	}
}


// ALTERAR
void menuAlterarUsuario(int *menu) {
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*               O  QUE  DESEJA  ALTERAR?               *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	printf("(1) -- Alterar o Nome\n");
	printf("(2) -- Alterar o Login\n");
	printf("(3) -- Alterar o Senha\n");
	printf("(4) -- Alterar o Idade\n");
	printf("(5) -- Alterar o Sexo\n");
	printf("(6) -- Alterar o Telefone\n\n");
	do {
		printf("Opção: ");
		scanf("%i", menu);
		if (*menu < 1 || *menu> 6)
			printf("Informe uma opção válida\n");
	} while (*menu < 1 || *menu> 6);
	system("cls");
}
void menuAlterarCategoria(int *menu) {
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*               O  QUE  DESEJA  ALTERAR?               *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	printf("(1) -- Alterar o Nome\n");
	printf("(2) -- Voltar para menu anterior\n");
	do {
		printf("Opção: ");
		scanf("%i", menu);
		if (*menu < 1 || *menu> 6)
			printf("Informe uma opção válida\n");
	} while (*menu < 1 || *menu> 6);
}
void menuAlterarProduto(int *menu) {
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*               O  QUE  DESEJA  ALTERAR?               *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	printf("(1) -- Alterar o Titulo\n");
	printf("(2) -- Alterar o Descrição\n");
	printf("(3) -- Alterar o Categoria\n");
	printf("(4) -- Alterar o Preço\n");
	printf("(5) -- Voltar para menu anterior\n");
	do {
		printf("Opção: ");
		scanf("%i", menu);
		if (*menu < 1 || *menu> 6)
			printf("Informe uma opção válida\n");
	} while (*menu < 1 || *menu> 6);
}
void alterarUsuario(char *nomeArq) {
	FILE *arq;
	struct Usuario usuario;
	int op;
	int codigo;
	int menu;
	char login[15], nome[51];
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*        COMO VOCÊ DESEJA SELECIONAR O USUÁRIO?        *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	printf("Por: \n");
	printf("(1) -- Código\n");
	printf("(2) -- Login\n");
	printf("(3) -- Parte do nome\n\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op> 3);
	if (!(arq = fopen(nomeArq, "r+b"))) {
		printf("Erro ao tentar abrir o arquivo para gravação!\n");
		return;
	}
	system("cls");
	switch (op) {
	case 1:
		menuAlterarUsuario(&menu);
		switch (menu) {
		case 1:
			do {
				printf("Informe o código do usuário que deseja alterar: ");
				scanf("%i", &codigo);
				getchar();
			} while (codigo < 0);
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (usuario.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						system("cls");
						printf("Novo nome: ");
						fgets(usuario.nome, 51, stdin);
						if (!validarNome(usuario.nome))
							printf("Caracter inválido!\n");
						if (usuario.nome[0] == '\n')
							printf("Nome obrigatório!\n");
						usuario.nome[strlen(usuario.nome) - 1] = '\0';
					} while (!validarNome(usuario.nome) || usuario.nome[0] == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao tentar alterar o usuário!\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 2:
			system("cls");
			printf("Informe o código do usuário que deseja alterar: ");
			scanf("%i", &codigo);
			getchar();
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (usuario.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("Login: ");
						fgets(usuario.login, 15, stdin);
						usuario.login[strlen(usuario.login) - 1] = '\0';
						if (validarLogin(usuario.login, USUARIOS) == 0)
							printf("Login já existente\n");
						if (usuario.login[0] == '\n')
							printf("Login obrigatório!\n");
					} while (usuario.login[0] == '\n' || validarLogin(usuario.login, USUARIOS) == 0);
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer a alteração!\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);

				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 3:
			system("cls");
			printf("Informe o código do usuário que deseja alterar: ");
			scanf("%i", &codigo);
			getchar();
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (usuario.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						senha(usuario.senha);
						if (usuario.senha[0] == '\0')
							printf("Senha obrigatória!\n");
						if (!validarSenha(usuario.senha))
							printf("\nSenha possui caracter inválido!\nUse somente letras e números!\n");
					} while (!validarSenha(usuario.senha) || usuario.senha[0] == '\0');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer a alteração!\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 4:
			system("cls");
			printf("Informe o código do usuário que deseja alterar: ");
			scanf("%i", &codigo);
			getchar();
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (usuario.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("\nInforme sua idade: ");
						scanf("%i", &usuario.idade);
						if (usuario.idade > 120)
							printf("\nIdade inválida!");
					} while (usuario.idade > 120);
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 5:
			system("cls");
			printf("Informe o código do usuário que deseja alterar: ");
			scanf("%i", &codigo);
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (usuario.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("Sexo ( M = Masculino | F = Feminino): ");
						scanf(" %c", &usuario.sexo);
						usuario.sexo = toupper(usuario.sexo);
						if (usuario.sexo == '\n')
							printf("Sexo obrigatório!\n");
						if (usuario.sexo != 'M' && usuario.sexo != 'F')
							printf("Caracter inválido, use M para masculino e F para feminino");
					} while ((usuario.sexo != 'M' && usuario.sexo != 'F') || usuario.sexo == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 6:
			system("cls");
			printf("Informe o código do usuário que deseja alterar: ");
			scanf("%i", &codigo);
			getchar();
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (usuario.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						telefone(usuario.telefone);
						if (usuario.telefone == '\n')
							printf("Telefone obrigatório!\n");
						if (!validarTel(usuario.telefone))
							printf("\nTelefone inválido!");
					} while (!validarTel(usuario.telefone) || usuario.telefone == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		}
		break;
	case 2:
		menuAlterarUsuario(&menu);
		switch (menu) {
		case 1:
			do {
				system("cls");
				printf("Informe o login do usuário que você deseja alterar: ");
				fgets(login, 15, stdin);
				fgets(login, 15, stdin);
				login[strlen(login) - 1] = '\0';
				if (login[0] == '\n')
					printf("Login obrigatório!\n");
			} while (login[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (!strcmp(login, usuario.login)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						fflush(stdin);
						printf("Informe o novo Nome: ");
						fgets(usuario.nome, 51, stdin);
						if (!validarNome(usuario.nome))
							printf("Caracter inválido!\n");
						if (usuario.nome[0] == '\n')
							printf("Nome obrigatório!\n");
						usuario.nome[strlen(usuario.nome) - 1] = '\0';
					} while (!validarNome(usuario.nome) || usuario.nome[0] == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 2:
			do {
				system("cls");
				getchar();
				printf("Informe o login do usuário que você deseja alterar: ");
				fgets(login, 15, stdin);
				login[strlen(login) - 1] = '\0';
				if (login[0] == '\n')
					printf("Login obrigatório!\n");
			} while (login[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (!strcmp(login, usuario.login)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("Informe o novo login: ");
						fgets(usuario.login, 15, stdin);
						usuario.login[strlen(usuario.login) - 1] = '\0';
						if (validarLogin(usuario.login, USUARIOS) == 0)
							printf("Login já existente\n");
						if (usuario.login[0] == '\n')
							printf("Login obrigatório!\n");
					} while (usuario.login[0] == '\n' || validarLogin(usuario.login, USUARIOS) == 0);
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 3:
			do {
				system("cls");
				getchar();
				printf("Informe o login do usuário que você deseja alterar: ");
				fgets(login, 15, stdin);
				login[strlen(login) - 1] = '\0';
				if (login[0] == '\n')
					printf("Login obrigatório!\n");
			} while (login[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (!strcmp(login, usuario.login)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						senha(usuario.senha);
						if (usuario.senha[0] == '\0')
							printf("Senha obrigatória!\n");
						if (!validarSenha(usuario.senha))
							printf("\nSenha possui caracter inválido!\nUse somente letras e números!\n");
					} while (!validarSenha(usuario.senha) || usuario.senha[0] == '\0');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 4:
			do {
				system("cls");
				getchar();
				printf("Informe o login do usuário que você deseja alterar: ");
				fgets(login, 15, stdin);
				login[strlen(login) - 1] = '\0';
				if (login[0] == '\n')
					printf("Login obrigatório!\n");
			} while (login[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (!strcmp(login, usuario.login)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("\nInforme sua idade: ");
						scanf("%i", &usuario.idade);
						if (usuario.idade > 120)
							printf("\nIdade inválida!");
					} while (usuario.idade > 120);
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 5:
			do {
				system("cls");
				getchar();
				printf("Informe o login do usuário que você deseja alterar: ");
				fgets(login, 15, stdin);
				login[strlen(login) - 1] = '\0';
				if (login[0] == '\n')
					printf("Login obrigatório!\n");
			} while (login[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (!strcmp(login, usuario.login)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("Sexo (M = Masculino | F = Feminino): ");
						scanf(" %c", &usuario.sexo);
						usuario.sexo = toupper(usuario.sexo);
						if (usuario.sexo == '\n')
							printf("Sexo obrigatório!\n");
						if (usuario.sexo != 'M' && usuario.sexo != 'F')
							printf("Caracter inválido, use M para masculino e F para feminino");
					} while ((usuario.sexo != 'M' && usuario.sexo != 'F') || usuario.sexo == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 6:
			do {
				getchar();
				system("cls");
				printf("Informe o login do usuário que você deseja alterar: ");
				fgets(login, 15, stdin);
				login[strlen(login) - 1] = '\0';
				if (login[0] == '\n')
					printf("Login obrigatório!\n");
			} while (login[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (!strcmp(login, usuario.login)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						telefone(usuario.telefone);
						if (usuario.telefone == '\n')
							printf("Telefone obrigatório!\n");
						if (!validarTel(usuario.telefone))
							printf("\nTelefone inválido!");
					} while (!validarTel(usuario.telefone) || usuario.telefone == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		}
		break;
	case 3:
		menuAlterarUsuario(&menu);
		switch (menu) {
		case 1:
			do {
				system("cls");
				getchar();
				printf("Parte do nome: ");
				fgets(nome, 51, stdin);
				nome[strlen(nome) - 1] = '\0';
				if (nome[0] == '\n')
					printf("Parte do nome obrigatório!\n");
			} while (nome[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (strstr(usuario.nome, nome)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						fflush(stdin);
						printf("Informe o novo Nome: ");
						fgets(usuario.nome, 51, stdin);
						if (!validarNome(usuario.nome))
							printf("Caracter inválido!\n");
						if (usuario.nome[0] == '\n')
							printf("Nome obrigatório!\n");
						usuario.nome[strlen(usuario.nome) - 1] = '\0';
					} while (!validarNome(usuario.nome) || usuario.nome[0] == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 2:
			do {
				system("cls");
				getchar();
				printf("Parte do nome: ");
				fgets(nome, 51, stdin);
				nome[strlen(nome) - 1] = '\0';
				if (nome[0] == '\n')
					printf("Parte do nome obrigatório!\n");
			} while (nome[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (strstr(usuario.nome, nome)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						fflush(stdin);
						printf("Informe o novo Login: ");
						fgets(usuario.login, 15, stdin);
						usuario.login[strlen(usuario.login) - 1] = '\0';
						if (validarLogin(usuario.login, USUARIOS) == 0)
							printf("Login já existente\n");
						if (usuario.login[0] == '\n')
							printf("Login obrigatório!\n");
					} while (usuario.login[0] == '\n' || validarLogin(usuario.login, USUARIOS) == 0);
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 3:
			do {
				system("cls");
				getchar();
				printf("Parte do nome: ");
				fgets(nome, 51, stdin);
				nome[strlen(nome) - 1] = '\0';
				if (nome[0] == '\n')
					printf("Parte do nome obrigatório!\n");
			} while (nome[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (strstr(usuario.nome, nome)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						senha(usuario.senha);
						if (usuario.senha[0] == '\0')
							printf("Senha obrigatória!\n");
						if (!validarSenha(usuario.senha))
							printf("\nSenha possui caracter inválido!\nUse somente letras e números!\n");
					} while (!validarSenha(usuario.senha) || usuario.senha[0] == '\0');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 4:
			do {
				system("cls");
				getchar();
				printf("Parte do nome: ");
				fgets(nome, 51, stdin);
				nome[strlen(nome) - 1] = '\0';
				if (nome[0] == '\n')
					printf("Parte do nome obrigatório!\n");
			} while (nome[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (strstr(usuario.nome, nome)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("\nInforme sua idade: ");
						scanf("%i", &usuario.idade);
						if (usuario.idade > 120)
							printf("\nIdade inválida!");
					} while (usuario.idade > 120);
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 5:
			do {
				system("cls");
				getchar();
				printf("Parte do nome: ");
				fgets(nome, 51, stdin);
				nome[strlen(nome) - 1] = '\0';
				if (nome[0] == '\n')
					printf("Parte do nome obrigatório!\n");
			} while (nome[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (strstr(usuario.nome, nome)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						printf("Sexo (M = Masculino | F = Feminino): ");
						scanf(" %c", &usuario.sexo);
						usuario.sexo = toupper(usuario.sexo);
						if (usuario.sexo == '\n')
							printf("Sexo obrigatório!\n");
						if (usuario.sexo != 'M' && usuario.sexo != 'F')
							printf("Caracter inválido, use M para masculino e F para feminino");
					} while ((usuario.sexo != 'M' && usuario.sexo != 'F') || usuario.sexo == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n\n");
					else
						printf("\nUsuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		case 6:
			do {
				system("cls");
				getchar();
				printf("Parte do nome: ");
				fgets(nome, 51, stdin);
				nome[strlen(nome) - 1] = '\0';
				if (nome[0] == '\n')
					printf("Parte do nome obrigatório!\n");
			} while (nome[0] == '\n');
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			while (!feof(arq)) {
				if (strstr(usuario.nome, nome)) {
					fseek(arq, -1 * sizeof(struct Usuario), SEEK_CUR);
					do {
						telefone(usuario.telefone);
						if (usuario.telefone == '\n')
							printf("Telefone obrigatório!\n");
						if (!validarTel(usuario.telefone))
							printf("\nTelefone inválido!");
					} while (!validarTel(usuario.telefone) || usuario.telefone == '\n');
					if (fwrite(&usuario, sizeof(struct Usuario), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Usuário alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&usuario, sizeof(struct Usuario), 1, arq);
			}
			break;
		}
		break;
	}
	if (fclose(arq)) {
		printf("Erro ao fechar arquivo!\n");
	}
}
void alterarCategoria(char *nomeArq) {
	FILE *arq;
	struct Categoria categoria;
	int op, i, status;
	int codigo;
	int menu;
	int pont, pont1;
	char login[15], nome[51];
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*       COMO VOCÊ DESEJA SELECIONAR A CATEGORIA?       *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	printf("Por: \n");
	printf("(1) -- Código\n");
	printf("(2) -- Parte do nome\n\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op> 2);
	if (!(arq = fopen(nomeArq, "r+b"))) {
		printf("Erro ao tentar abrir arquivo para gravação!\n");
		return;
	}
	system("cls");
	menuAlterarCategoria(&menu);
	system("cls");
	switch (menu) {
	case 1:
		switch (op) {
		case 1:
			do {
				printf("Informe o código do usuário que deseja alterar: ");
				scanf("%i", &codigo);
				getchar();
			} while (codigo < 0);
			fread(&categoria, sizeof(struct Categoria), 1, arq);
			while (!feof(arq)) {
				if (categoria.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Categoria), SEEK_CUR);
					pont1 = ftell(arq);
					do {
						printf("Novo nome: ");
						fgets(categoria.nome, 51, stdin);
						if (!validarNome(categoria.nome))
							printf("Caracter inválido!\n");
						if (categoria.nome[0] == '\n')
							printf("Nome obrigatório!\n");
						categoria.nome[strlen(categoria.nome) - 1] = '\0';
					} while (!validarNome(categoria.nome) || categoria.nome[0] == '\n');
					if (fwrite(&categoria, sizeof(struct Categoria), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n");
					else
						printf("Usuário alterado com sucesso!");
					fseek(arq, 0, SEEK_END);
				}
				fread(&categoria, sizeof(struct Categoria), 1, arq);
			}
			break;
		case 2:
			do {
				getchar();
				printf("Informe parte do nome da categoria que deseja alterar: ");
				fgets(nome, 51, stdin);
				nome[strlen(nome) - 1] = '\0';
				if (nome[0] == '\n')
					printf("Parte do nome obrigatório!\n");
			} while (nome[0] == '\n');
			fread(&categoria, sizeof(struct Categoria), 1, arq);
			while (!feof(arq)) {
				if (strstr(categoria.nome, nome)) {
					fseek(arq, -1 * sizeof(struct Categoria), SEEK_CUR);
					do {
						printf("Informe o novo nome: ");
						fgets(categoria.nome, 51, stdin);
						if (!validarNome(categoria.nome))
							printf("Caracter inválido!\n");
						if (categoria.nome[0] == '\n')
							printf("Nome obrigatório!\n");
						categoria.nome[strlen(categoria.nome) - 1] = '\0';
					} while (!validarNome(categoria.nome) || categoria.nome[0] == '\n');
					if (fwrite(&categoria, sizeof(struct Categoria), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n");
					else
						printf("Usuário alterado com sucesso!");
					fseek(arq, 0, SEEK_END);
				}
				fread(&categoria, sizeof(struct Usuario), 1, arq);
			}
		}
		if ((fclose(arq))) {
			printf("Erro ao fechar arquivo para gravação.");
			system("PAUSE");
		}
		break;
	case 2:
		break;
	}
}
void alterarProduto(char *nomeArq) {
	FILE *arq;
	struct Produto produto;
	int op;
	int codigo, categoria;
	int menu;
	char  titulo[100], descricao[256];
	float preco;
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*        COMO VOCÊ DESEJA SELECIONAR O PRODUTO?        *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\n");
	printf("Por: \n");
	printf("(1) -- Código\n");
	printf("(2) -- Parte do título\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op> 2);
	if (!(arq = fopen(nomeArq, "r+b"))) {
		printf("Erro ao tentar abrir o arquivo para gravação!\n");
		return;
	}
	system("cls");
	switch (op) {
	case 1:
		menuAlterarProduto(&menu);
		switch (menu) {
		case 1:
			do {
				printf("Informe o código do produto que deseja alterar: ");
				scanf("%i", &codigo);
				getchar();
			} while (codigo < 0);
			fread(&produto, sizeof(struct Produto), 1, arq);
			while (!feof(arq)) {
				if (produto.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Produto), SEEK_CUR);
					do {
						system("cls");
						printf("Novo título: ");
						fgets(produto.titulo, 100, stdin);
						if (produto.titulo[0] == '\n')
							printf("Título obrigatório!\n");
						produto.titulo[strlen(produto.titulo) - 1] = '\0';
					} while (produto.titulo[0] == '\n');
					if (fwrite(&produto, sizeof(struct Produto), 1, arq) != 1)
						printf("Erro ao tentar alterar o usuário!\n");
					else
						printf("Produto alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&produto, sizeof(struct Produto), 1, arq);
			}
			system("pause");
			break;
		case 2:
			system("cls");
			printf("Informe o código do produto que deseja alterar: ");
			scanf("%i", &codigo);
			getchar();
			fread(&produto, sizeof(struct Produto), 1, arq);
			while (!feof(arq)) {
				if (produto.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Produto), SEEK_CUR);
					do {
						system("cls");
						printf("Nova descrição: ");
						fgets(produto.descricao, 256, stdin);
						if (produto.descricao[0] == '\n')
							printf("Descrição obrigatória!\n");
						produto.descricao[strlen(produto.descricao) - 1] = '\0';
					} while (produto.descricao[0] == '\n');
					if (fwrite(&produto, sizeof(struct Produto), 1, arq) != 1)
						printf("Erro ao fazer a alteração!\n");
					else
						printf("Produto alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);

				}
				fread(&produto, sizeof(struct Produto), 1, arq);
			}
			system("pause");
			break;
		case 3:
			system("cls");
			printf("Informe o código do produto que deseja alterar: ");
			scanf("%i", &codigo);
			getchar();
			fread(&produto, sizeof(struct Produto), 1, arq);
			while (!feof(arq)) {
				if (produto.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Produto), SEEK_CUR);
					do {
						printf("\nDigite 0 para listar categorias.\n");
						printf("Novo código da categoria: ");
						scanf("%i", &produto.categoria);
						if (produto.categoria == 0) {
							system("cls");
							listarCategoria(CATEGORIAS);
							printf("Novo código da categoria: ");
							scanf("%i", &produto.categoria);
						}
						if (validarCategoria(produto.categoria, PRODUTOS) == 1) {
							printf("Esta categoria não consta em nosso banco de dados!\nDigite novamente o ");
						}
					} while (validarCategoria(produto.categoria, PRODUTOS) == 1 || produto.categoria < 1);
					if (fwrite(&produto, sizeof(struct Produto), 1, arq) != 1)
						printf("Erro ao fazer a alteração!\n");
					else
						printf("Produto alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&produto, sizeof(struct Produto), 1, arq);
			}
			system("pause");
			break;
		case 4:
			system("cls");
			printf("Informe o código do produto que deseja alterar: ");
			scanf("%i", &codigo);
			getchar();
			fread(&produto, sizeof(struct Produto), 1, arq);
			while (!feof(arq)) {
				if (produto.codigo == codigo) {
					fseek(arq, -1 * sizeof(struct Produto), SEEK_CUR);
					do {
						printf("Preço: ");
						scanf("%f", &produto.preco);
						if (produto.preco < 0)
							printf("\nPreço inválido! Digite um valor acima de 0.\n");
					} while (produto.preco < 0);
					if (fwrite(&produto, sizeof(struct Produto), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Produto alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&produto, sizeof(struct Produto), 1, arq);
			}
			system("pause");
			break;
		}
		break;
	case 2:
		menuAlterarProduto(&menu);
		switch (menu) {
		case 1:
			do {
				system("cls");
				printf("Informe a parte do título do produto que você deseja alterar: ");
				fgets(titulo, 100, stdin);
				fgets(titulo, 100, stdin);
				titulo[strlen(titulo) - 1] = '\0';
				if (titulo[0] == '\n')
					printf("Título obrigatório!\n");
			} while (titulo[0] == '\n');
			fread(&produto, sizeof(struct Produto), 1, arq);
			while (!feof(arq)) {
				if (strstr(produto.titulo, titulo)) {
					fseek(arq, -1 * sizeof(struct Produto), SEEK_CUR);
					do {
						system("cls");
						printf("Novo título: ");
						fgets(produto.titulo, 100, stdin);
						if (produto.titulo[0] == '\n')
							printf("Título obrigatório!\n");
						produto.titulo[strlen(produto.titulo) - 1] = '\0';
					} while (produto.titulo[0] == '\n');
					if (fwrite(&produto, sizeof(struct Produto), 1, arq) != 1)
						printf("Erro ao tentar alterar o usuário!\n");
					else
						printf("Produto alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&produto, sizeof(struct Produto), 1, arq);
			}
			system("pause");
			break;
		case 2:
			do {
				system("cls");
				printf("Informe a parte do título do produto que você deseja alterar: ");
				fgets(titulo, 100, stdin);
				fgets(titulo, 100, stdin);
				titulo[strlen(titulo) - 1] = '\0';
				if (titulo[0] == '\n')
					printf("Título obrigatório!\n");
			} while (titulo[0] == '\n');
			fread(&produto, sizeof(struct Produto), 1, arq);
			while (!feof(arq)) {
				if (strstr(produto.titulo, titulo)) {
					fseek(arq, -1 * sizeof(struct Produto), SEEK_CUR);
					do {
						system("cls");
						printf("Nova descrição: ");
						fgets(produto.descricao, 256, stdin);
						if (produto.descricao[0] == '\n')
							printf("Descrição obrigatória!\n");
						produto.descricao[strlen(produto.descricao) - 1] = '\0';
					} while (produto.descricao[0] == '\n');
					if (fwrite(&produto, sizeof(struct Produto), 1, arq) != 1)
						printf("Erro ao fazer a alteração!\n");
					else
						printf("Produto alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);

				}
				fread(&produto, sizeof(struct Produto), 1, arq);
			}
			system("pause");
			break;
		case 3:
			do {
				system("cls");
				printf("Informe a parte do título do produto que você deseja alterar: ");
				fgets(titulo, 100, stdin);
				fgets(titulo, 100, stdin);
				titulo[strlen(titulo) - 1] = '\0';
				if (titulo[0] == '\n')
					printf("Título obrigatório!\n");
			} while (titulo[0] == '\n');
			fread(&produto, sizeof(struct Produto), 1, arq);
			while (!feof(arq)) {
				if (strstr(produto.titulo, titulo)) {
					fseek(arq, -1 * sizeof(struct Produto), SEEK_CUR);
					do {
						printf("Preço: ");
						scanf("%f", &produto.preco);
						if (produto.preco < 0)
							printf("\nPreço inválido! Digite um valor acima de 0.\n");
					} while (produto.preco < 0);
					if (fwrite(&produto, sizeof(struct Produto), 1, arq) != 1)
						printf("Erro ao fazer alteração!\n\n");
					else
						printf("Produto alterado com sucesso!\n\n");
					fseek(arq, 0, SEEK_END);
				}
				fread(&produto, sizeof(struct Produto), 1, arq);
			}
			system("pause");
			break;
		}
		break;
	case 5:
		system("cls");
		break;
	}
	if (fclose(arq)) {
		printf("Erro ao fechar arquivo!\n");
	}
}


// REMOVER
void removerUsuario(char *nomeArq, char *nomeTemp) {
	int op;
	int flag = 0;
	FILE *arq;
	FILE *temp;
	FILE *arqAv;
	FILE *arqAvTemp;
	char login[15], nome[51];
	int codigo, pont = -1;
	struct Usuario usuario;
	struct Avaliacao avaliacao;
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir o arquivo para gravação!\n");
		return;
	}
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t* COMO VOCÊ DESEJA SELECIONAR O USUÁRIO PARA EXCLUSÃO? *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\nPelo: \n");
	printf("(1) -- Código\n");
	printf("(2) -- Login\n");
	printf("(3) -- Parte do nome\n\n");
	do {
		printf("Opção: ");
		scanf("%i", &op);
	} while (op < 1 || op> 3);
	switch (op) {
	case 1:
		system("cls");
		printf("Código do usuário que você deseja excluir: ");
		scanf("%i", &codigo);
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		pont = ftell(arq);
		while (!feof(arq)) {
			if (codigo == usuario.codigo) {
				flag = 1;
				break;
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			pont = ftell(arq);
		}
		break;
	case 2:
		system("cls");
		printf("Login do usuário que você deseja excluir: ");
		getchar();
		fgets(login, 15, stdin);
		login[strlen(login) - 1] = '\0';
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		pont = ftell(arq);
		while (!feof(arq)) {
			if (!strcmp(usuario.login, login)) {
				flag = 1;
				break;
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			pont = ftell(arq);
		}
		break;
	case 3:
		do {
			getchar();
			printf("Parte do nome do usuário que você deseja excluir: ");
			fgets(nome, 51, stdin);
			getchar();
			if (!validarNome(nome))
				printf("Caracter inválido!\n");
			if (nome[0] == '\n')
				printf("Nome obrigatório!\n");
			nome[strlen(nome) - 1] = '\0';
		} while (!validarNome(nome) || nome[0] == '\n');
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		pont = ftell(arq);
		while (!feof(arq)) {
			if (strstr(usuario.nome, nome)) {
				pont = ftell(arq);
				flag = 1;
				break;
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
			pont = ftell(arq);
		}
		break;
	}
	if (!(arqAv = fopen(AVALIACOES, "rb"))) {
		printf("Erro ao abrir arquivo de avaliações para gravação");
	}
	if (!(arqAvTemp = fopen(AVALIACOES_TEMP, "wb"))) {
		printf("Erro ao abrir arquivo de avaliações para gravação");
	}
	fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAv);
	while (!feof(arqAv)) {
		if (avaliacao.codigoUsuario != usuario.codigo) {
			if (fwrite(&avaliacao, sizeof(struct Avaliacao), 1, arqAvTemp) != 1) {
				printf("Erro ao gravar informação no arquivo de avaliações");
			}
		}
		fread(&avaliacao, sizeof(struct Avaliacao), 1, arqAv);
	}
	if (fclose(arqAv)) {
		printf("\nErro ao tentar fechar arquivo!\n");
	}
	if (fclose(arqAvTemp)) {
		printf("\nErro ao tentar fechar arquivo!\n");
	}
	if (remove(AVALIACOES)) {
		printf("Erro ao remover arquivo temporário!");
	}
	if (rename(AVALIACOES_TEMP, AVALIACOES)) {
		printf("Erro ao renomear arquivo!");
	}
	if (flag == 1) {
		if (!(temp = fopen(nomeTemp, "wb"))) {
			printf("Erro ao tentar abrir o arquivo para gravação!\n");
			return;
		}
		fseek(arq, 0, SEEK_SET);
		fread(&usuario, sizeof(struct Usuario), 1, arq);
		while (!feof(arq)) {
			if (pont != ftell(arq)) {
				if (fwrite(&usuario, sizeof(struct Usuario), 1, temp) != 1) {
					printf("\nErro ao tentar salvar a pessoa no arquivo!\n");
				}
			}
			fread(&usuario, sizeof(struct Usuario), 1, arq);
		}
		if (fclose(arq)) {
			printf("\nErro ao tentar fechar arquivo!\n");
		}
		if (fclose(temp)) {
			printf("\nErro ao tentar fechar arquivo!\n");
		}
		if (remove(nomeArq)) {
			printf("Erro ao remover arquivo temporário!");
		}
		if (rename(nomeTemp, nomeArq)) {
			printf("Erro ao renomear arquivo!");
		}
	}
}
void removerCategoria(char *nomeArq, char *nomeTemp) {
	int op;
	int flag = 0;
	FILE *arq;
	FILE *temp;
	FILE *prod;
	FILE *prodTemp;
	char nome[51];
	int codigo, pont = -1;
	struct Categoria categoria;
	struct Produto produto;
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir o arquivo para leitura!\n");
		return;
	}
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t*COMO VOCÊ DESEJA SELECIONAR A CATEGORIA PARA EXCLUSÃO?*\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\nPelo: \n");
	printf("(1) -- Código\n");
	printf("(2) -- Parte do nome\n\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op > 2);
	switch (op) {
	case 1:
		system("cls");
		printf("Código da categoria que você deseja excluir: ");
		scanf("%i", &codigo);
		fread(&categoria, sizeof(struct Categoria), 1, arq);
		pont = ftell(arq);
		while (!feof(arq)) {
			if (codigo == categoria.codigo) {
				flag = 1;
				break;
			}
			fread(&categoria, sizeof(struct Categoria), 1, arq);
			pont = ftell(arq);
		}
		break;
	case 2:
		do {
			getchar();
			printf("Parte do nome da categoria que você deseja excluir: ");
			fgets(nome, 51, stdin);
			if (!validarNome(nome))
				printf("Caracter inválido!\n");
			if (nome[0] == '\n')
				printf("Nome obrigatório!\n");
			nome[strlen(nome) - 1] = '\0';
		} while (!validarNome(nome) || nome[0] == '\n');
		fread(&categoria, sizeof(struct Categoria), 1, arq);
		pont = ftell(arq);
		while (!feof(arq)) {
			if (strstr(categoria.nome, nome)) {
				pont = ftell(arq);
				flag = 1;
				break;
			}
			fread(&categoria, sizeof(struct Categoria), 1, arq);
			pont = ftell(arq);
		}
		break;
	}
	if (!(prod = fopen(PRODUTOS, "rb"))) {
		printf("Erro ao abrir arquivo de avaliações para gravação");
	}
	if (!(prodTemp = fopen(PRODUTOS_TEMP, "wb"))) {
		printf("Erro ao abrir arquivo de avaliações para gravação");
	}
	fread(&produto, sizeof(struct Produto), 1, prod);
	while (!feof(prod)) {
		if (produto.categoria != categoria.codigo) {
			if (fwrite(&produto, sizeof(struct Produto), 1, prodTemp) != 1) {
				printf("Erro ao gravar informação no arquivo de avaliações");
			}
		}
		fread(&produto, sizeof(struct Produto), 1, prod);
	}
	if (fclose(prod)) {
		printf("\nErro ao tentar fechar arquivo!\n");
	}
	if (fclose(prodTemp)) {
		printf("\nErro ao tentar fechar arquivo!\n");
	}
	if (remove(PRODUTOS)) {
		printf("Erro ao remover arquivo temporário!");
	}
	if (rename(PRODUTOS_TEMP, PRODUTOS)) {
		printf("Erro ao renomear arquivo!");
	}
	if (flag == 1) {
		if (!(temp = fopen(nomeTemp, "wb"))) {
			printf("Erro ao tentar abrir o arquivo para gravação!\n");
			return;
		}
		fseek(arq, 0, SEEK_SET);
		fread(&categoria, sizeof(struct Categoria), 1, arq);
		while (!feof(arq)) {
			if (pont != ftell(arq)) {
				if (fwrite(&categoria, sizeof(struct Categoria), 1, temp) != 1) {
					printf("\nErro ao alterar a categoria!\n");
				}
			}
			fread(&categoria, sizeof(struct Categoria), 1, arq);
		}
		if (fclose(arq)) {
			printf("\nErro ao tentar fechar arquivo!\n");
		}
		if (fclose(temp)) {
			printf("\nErro ao tentar fechar arquivo!\n");
		}
		if (remove(nomeArq)) {
			printf("Erro ao remover arquivo temporário!");
		}
		if (rename(nomeTemp, nomeArq)) {
			printf("Erro ao renomear arquivo!");
		}
		else
			printf("Categoria removida com sucesso!\n\n");
	}
}
void removerProduto(char *nomeArq, char *nomeTemp) {
	int op;
	int flag = 0;
	FILE *arq;
	FILE *temp;
	char titulo[100];
	int codigo, pont = -1;
	struct Produto produto;
	if (!(arq = fopen(nomeArq, "rb"))) {
		printf("Erro ao tentar abrir o arquivo para leitura!\n");
		return;
	}
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\t********************************************************\n");
	printf("\t*                                                      *\n");
	printf("\t* COMO VOCÊ DESEJA SELECIONAR O PRODUTO PARA EXCLUSÃO? *\n");
	printf("\t*                                                      *\n");
	printf("\t********************************************************\n\nPelo: \n");
	printf("(1) -- Código\n");
	printf("(2) -- Parte do título\n\n");
	do {
		printf("Informe a opção: ");
		scanf("%i", &op);
	} while (op < 1 || op > 2);
	switch (op) {
	case 1:
		system("cls");
		printf("Código do produto que você deseja excluir: ");
		scanf("%i", &codigo);
		fread(&produto, sizeof(struct Produto), 1, arq);
		pont = ftell(arq);
		while (!feof(arq)) {
			if (codigo == produto.codigo) {
				flag = 1;
				break;
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
			pont = ftell(arq);
		}
		break;
	case 2:
		do {
			getchar();
			printf("Título: ");
			fgets(titulo, 100, stdin);
			if (titulo[0] == '\n')
				printf("Título obrigatório!\n");
			titulo[strlen(titulo) - 1] = '\0';
		} while (titulo[0] == '\n');
		fread(&produto, sizeof(struct Produto), 1, arq);
		pont = ftell(arq);
		while (!feof(arq)) {
			if (strstr(produto.titulo, titulo)) {
				pont = ftell(arq);
				flag = 1;
				break;
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
			pont = ftell(arq);
		}
		break;
	}
	if (flag == 1) {
		if (!(temp = fopen(nomeTemp, "wb"))) {
			printf("Erro ao tentar abrir o arquivo para gravação!\n");
			return;
		}
		fseek(arq, 0, SEEK_SET);
		fread(&produto, sizeof(struct Produto), 1, arq);
		while (!feof(arq)) {
			if (pont != ftell(arq)) {
				if (fwrite(&produto, sizeof(struct Produto), 1, temp) != 1) {
					printf("\nErro ao alterar o produto!\n");
				}
			}
			fread(&produto, sizeof(struct Produto), 1, arq);
		}
		if (fclose(arq)) {
			printf("\nErro ao tentar fechar arquivo!\n");
		}
		if (fclose(temp)) {
			printf("\nErro ao tentar fechar arquivo!\n");
		}
		if (remove(nomeArq)) {
			printf("Erro ao remover arquivo temporário!");
		}
		if (rename(nomeTemp, nomeArq)) {
			printf("Erro ao renomear arquivo!");
		}
		else
			printf("Categoria removida com sucesso!\n\n");
	}
}


// MENUS
void menuUsuario() {
	int op;
	do {
		do {
			system("cls");
			printf("\n\n\t\t\t      Hora: ");
			system("time/t");
			printf("\t********************************************************\n");
			printf("\t*                                                      *\n");
			printf("\t*                    U S U Á R I O S                   *\n");
			printf("\t*                                                      *\n");
			printf("\t********************************************************\n\n");
			printf("(1) -- Novo Usuario\n");
			printf("(2) -- Listar\n");
			printf("(3) -- Consultar\n");
			printf("(4) -- Alterar\n");
			printf("(5) -- Remover\n");
			printf("(6) -- Voltar para o menu inicial\n\n");
			printf("Opção: ");
			scanf("%i", &op);
		} while (op < 1 || op > 6);
		system("cls");
		switch (op) {
		case 1:
			cadastroUsuario(USUARIOS);
			system("PAUSE");
			break;
		case 2:
			listarUsuario(USUARIOS);
			system("PAUSE");
			break;
		case 3:
			consultarUsuario(USUARIOS);
			system("PAUSE");
			break;
		case 4:
			alterarUsuario(USUARIOS);
			system("PAUSE");
			break;
		case 5:
			removerUsuario(USUARIOS, USUARIOS_TEMP);
			system("PAUSE");
			break;
		case 6:
			break;
		}
	} while (op != 6);
}
void menuCategoria() {
	int op;

	do {
		do {
			system("cls");
			printf("\n\n\t\t\t      Hora: ");
			system("time/t");
			printf("\t********************************************************\n");
			printf("\t*                                                      *\n");
			printf("\t*                  C A T E G O R I A S                 *\n");
			printf("\t*                                                      *\n");
			printf("\t********************************************************\n\n");
			printf("(1) -- Cadastrar\n");
			printf("(2) -- Listar\n");
			printf("(3) -- Consultar\n");
			printf("(4) -- Alterar\n");
			printf("(5) -- Remover\n");
			printf("(6) -- Voltar para o menu inicial\n\n");
			printf("Opção: ");
			scanf("%i", &op);
		} while (op < 1 || op > 6);
		system("cls");
		switch (op) {
		case 1:
			cadastroCategoria(CATEGORIAS);
			system("PAUSE");
			break;
		case 2:
			listarCategoria(CATEGORIAS);
			system("PAUSE");
			break;
		case 3:
			consultarCategoria(CATEGORIAS);
			system("PAUSE");
			break;
		case 4:
			alterarCategoria(CATEGORIAS);
			break;
		case 5:
			removerCategoria(CATEGORIAS, CATEGORIAS_TEMP);
			system("PAUSE");
			break;
		case 6:
			break;
		}
	} while (op != 6);
}
int menuProduto(int menu) {
	int op;
	if (menu != 5) {
		do {
			do {
				system("cls");
				printf("\n\n\t\t\t      Hora: ");
				system("time/t");
				printf("\t********************************************************\n");
				printf("\t*                                                      *\n");
				printf("\t*                   P R O D U T O S                    *\n");
				printf("\t*                                                      *\n");
				printf("\t********************************************************\n\n");
				printf("(1) -- Cadastrar\n");
				printf("(2) -- Listar\n");
				printf("(3) -- Consultar\n");
				printf("(4) -- Alterar\n");
				printf("(5) -- Remover\n");
				printf("(6) -- Voltar para o menu inicial\n\n");
				printf("Opção: ");
				scanf("%i", &op);
			} while (op < 1 || op > 6);
			system("cls");
			switch (op) {
			case 1:
				cadastroProduto(PRODUTOS);
				system("PAUSE");
				break;
			case 2:
				listarProduto(PRODUTOS);
				system("PAUSE");
				break;
			case 3:
				consultarProdutos(PRODUTOS);
				system("PAUSE");
				break;
			case 4:
				alterarProduto(PRODUTOS);
				system("PAUSE");
				break;
			case 5:
				removerProduto(PRODUTOS, PRODUTOS_TEMP);
				system("PAUSE");
				break;
			case 6:
				break;
			}
		} while (op != 6);
		return op;
	}
	else
		return menu;
}



int main() {
	int op;
	configTela();
	printf("\n\n\t\t\t      Hora: ");
	system("time/t");
	printf("\n  ************************************************************************\n");
	printf("  * ____                                 _               _             _ *\n");
	printf("  *| __ )    ___   _ __ ___     __   __ (_)  _ __     __| |   ___     | |*\n");
	printf("  *|  _ \\   / _ \\ | '_ ` _ \\    \\ \\ / / | | | '_ \\   / _` |  / _ \\    | |*\n");
	printf("  *| |_) |  | __/ | | | | | |    \\ V /  | | | | | | | (_| | | (_) |   |_|*\n");
	printf("  *|____/   \\___| |_| |_| |_|     \\_/   |_| |_| |_|  \\__,_|  \\___/    (_)*\n");
	printf("  ************************************************************************\n\n\n");
	printf("\n\n--Desenvolvedore                        Matricula\n\n\n");
	printf("João Gabriel Costa Sodré da Mota    ||  201520863-2\n");
	printf("Ramon Ranieri Alves Alcântara       ||  201520474-0\n");
	printf("Tiago Marques Emerenciano           ||  201520867-9\n\n");
	system("pause");
	system("cls");
	do {
		do {
			do {
				system("cls");
				printf("\n\n\t\t\t      Hora: ");
				system("time/t");
				printf("\t********************************************************\n");
				printf("\t*                                                      *\n");
				printf("\t*                  SISTEMA E-COMMERCE                  *\n");
				printf("\t*                                                      *\n");
				printf("\t********************************************************\n\n");
				printf("\n(1) -- Usuários");
				printf("\n(2) -- Categorias");
				printf("\n(3) -- Produtos");
				printf("\n(4) -- Avaliar Produtos");
				printf("\n(5) -- Sair");
				printf("\n\nOpção: ");
				scanf("%i", &op);
			} while (op < 1 || op > 5);
			switch (op) {
			case 1:
				menuUsuario();
				break;
			case 2:
				menuCategoria();
				break;
			case 3:
				menuProduto(op);
				break;
			case 4:
				avaliacao(AVALIACOES);
				break;
			case 5:
				break;
			}
		} while (op != 5);
	} while (menuProduto(op) == 6);
	system("pause");
	return 0;
}