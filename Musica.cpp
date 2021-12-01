#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>
#include <stdio.h>

struct reg {
	char musica[20];
	char autor[20];
	char ano[20];
	char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void geraarqtxt(FILE *);
void excluir(FILE *);

int main() {
	setlocale(LC_ALL,"");
	int op;
	FILE *arq;

	if ((arq = fopen("agenda1.dat", "rb+")) == NULL) {
		if ((arq = fopen("agenda1.dat", "wb+")) == NULL) {
			printf("Falha ao abrir o arquivo!\n");
			system("pause");
		}
	}

	//existentes no arquivo agenda1.dat
	do {
		system("CLS");
		printf("\n======= LISTA DE M�SICAS ======= \n");
		printf("1.Cadastrar nova m�sica\n");
		printf("2.Consultar m�sica\n");
		printf("3.Gerar arquivo txt\n");
		printf("4.Excluir registro\n");
		printf("5.Sair\n");
		printf("===========M�sicas:%d=\n", tamanho(arq));
		printf("Opcao:");
		scanf("%d", &op);
		switch (op) {
			case 1://cadastrar nova m�sica
				cadastrar(arq);
				break;
			case 2://consultar por c�digo
				consultar(arq);
				break;
				//	case 3:consultanome(arq);
				break;
			case 3:
				geraarqtxt(arq);
				//gerar arquivo texto com todos os contatos
				//ordenados 22alfabeticamente
				break;
			case 4: //exclui um registro do arq
				excluir(arq);
				break;

			case 5:
				fclose(arq);
		}
	} while (op != 5);
}

void cadastrar(FILE *arq) {
	reg contato;
	char confirma;
	contato.status = ' ';
	fflush(stdin);

	printf("Cadastrando nova m�sica: \n");
	printf("\nNumero do m�sicas a serem cadastradas: %d\n", tamanho(arq) +1);
	printf("Nome da musica............: ");
	gets(contato.musica);
	printf("Autor.....................: ");
	gets(contato.autor);
	printf("Ano.......................: ");
	gets(contato.ano);
	printf("\nConfirma <s/n>:");
	fflush(stdin);
	scanf("%c", &confirma);

	if (toupper(confirma) == 'S') {
		printf("\cadastrando...\n\n");
		fseek(arq, 0, SEEK_END);
		fwrite(&contato, sizeof(reg), 1, arq);

	}
	system("pause");
}

void consultar(FILE *arq) {
	reg consultar; // // "forma de bolo" para moldar o conte�do
	int nr;
	printf("Insira o n�mero de cadastro da m�sica:\n");
	scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0)) {
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET); // procura no arq na posi��o c-1*sizeof  e deixa a mem�ria apontada para aquela regi�o
		fread(&consultar, sizeof(reg), 1, arq);// est� lendo o registro e salvando dentro da vari�vel contato
		if (consultar.status == ' ') {
			printf("\nMusica:....: %s", consultar.musica);
			printf("\nAutor:.....: %s", consultar.autor);
			printf("\nAno:.......: %s\n", consultar.ano);
		}

		else
			printf("\nM�sica excluida! \n");

	} else {
		printf("\nN�mero de registro da m�sica invalido!\n");
	}
	system("pause");
}

void geraarqtxt(FILE *arq) {
	char nomearq[20];
	printf("Nome do arquivo texto:");
	scanf("%s", nomearq);
	strcat(nomearq, ".txt");
	FILE *arqtxt = fopen(nomearq, "w");
	if (!arqtxt) {
		printf("Nao foi possivel gerar este arquivo!\n");
		system("pause");
		//return;
	}
	fprintf(arqtxt, "M�sica                    Autor                   Ano                   Status\n");
	fprintf(arqtxt, "======================================================================\n");
	int nr;
	reg contato;
	for (nr = 0; nr<tamanho(arq); nr++) {
		fseek(arq, nr * sizeof(reg), SEEK_SET);
		fread(&contato, sizeof(reg), 1, arq);
		fprintf(arqtxt, "%-25s%-25s%-25s- %c\n", contato.musica, contato.autor, contato.ano, contato.status);
	}
	fprintf(arqtxt, "================================================================\n");
	fclose(arqtxt);
}

void excluir(FILE *arq) {
	reg contato;
	char confirma;
	int nr;

	printf("\nInforme o codigo do registro para excluir\n");
	scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0)) {
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&contato, sizeof(reg), 1, arq);
		if (contato.status == ' ') {
			printf("\nNome da m�sica.....:%s", contato.musica);
			printf("\nNome do autor......:%s", contato.autor);
			printf("\nAno da m�sica......:%s", contato.ano);
			printf("\nNConfirma a exclus�o: <s/n>\n");
			getchar();
			scanf("%c", &confirma);

			if (toupper(confirma) == 'S') {
				printf("\nexcluindo...\n\n");
				fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
				contato.status = '*';
				fwrite(&contato, sizeof(reg), 1, arq);
			}
		} else
			printf("Registro inexistente! \n");
	} else {
		printf("\nNumero de registro invalido!\n");
	}
	system("pause");
}

int tamanho(FILE *arq) {
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / sizeof(reg);
}

