#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

string retiraEspacos(string palavra)
{
	string resp;
	int i, tam = palavra.length();
	for (i = 0; i < tam; i++)
	{
		if ((palavra[i] >= 'A') && (palavra[i] <= 'Z'))
			resp = resp + palavra[i];
	}
	return resp;
}

bool eExpressao(string expressao)
{
	if (expressao.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ ") != string::npos)
		return true;
	else
	{
		string a = expressao.substr(expressao.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ")); //a partir da primeira letra
		int posicao_espaco = a.find_first_of(" ");
		if (posicao_espaco != string::npos)   //se existir algum espaco depois da primeira letra
		{	                                       //posicao do primeiro espaco depois da primeira letra
			string b = a.substr(posicao_espaco);                                            //a partir do 1. espaco depois da primeira letra (tudo o que vem depois da primeira palavra)
			if (b.find_first_of("ABCDEFGHIJKLMNOPQRSTWXYZ") != string::npos)                // tem mais letras - é expressão
				return true;
			else                                                                            //não tem mais letras - é palavra
				return false;
		}
		else
			return false; //nao existia nenhum espaco depois da primeira letra - é uma só palavra
	}
}

void adicionaPalavras(vector<string> &lista, ifstream &dicionario)
{
	string linha;
	int contador = 0, f;
	string abecedario = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	while (getline(dicionario, linha))
	{
		//imprime pontinhos por cada 100 palavras
		if ((contador % 100 == 0) && (contador != 0)) 
			cout << '.';
		//linha com uma palavra valida - verifica se: não tem nada mais que espacos e letras maisculas, verifica se tem letras (nao e uma linha vazia) e se é uma palavra ou expressao
		if ((linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ ") == string::npos) && (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos) && (!eExpressao(linha)))
		{	
			//imprime letra se mudou de letra
			if (abecedario[0] == retiraEspacos(linha)[0])
			{
				cout << endl << abecedario[0] << endl;
				abecedario = abecedario.substr(1);
			}
			lista.push_back(retiraEspacos(linha));
			contador++;
		}
		//linha com mais do que uma palavra - verifica se nao tem nada mais que espaços, letras maisculas e pontos e virgulas e  verifica se tem letras (não e linha vazia)
		else if ((linha.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ ;'-") == string::npos) && (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos))
		{
			f = linha.find(';');
			//imprime letra se mudou de letra
			if (abecedario[0] == retiraEspacos(linha)[0])
			{
				cout << endl << abecedario[0] << endl;
				abecedario = abecedario.substr(1);
			}
			// para cada expressao separada por ponto e virgula (exceto a ultima)
			while (f != string::npos)
			{
				if (linha.find_first_of(';') == linha.length()-1)
					break;
				if (!eExpressao(linha.substr(0, f)))
				{
					lista.push_back(retiraEspacos(linha.substr(0, f)));
					contador++;
				}
				linha = linha.substr(f + 1);
				f = linha.find(';');
			}
			//para a ultima expressao
			if ((!eExpressao(linha.substr(0, f))) && (linha.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos))
				lista.push_back(retiraEspacos(linha.substr(0, f)));
		}
	}
}

int binarySearch(const vector<string> &v, string value)
{
	int bottom = 0, top = v.size() - 1, middle;
	bool found = false;
	while ((bottom <= top) && (!found))
	{
		middle = (bottom + top) / 2;
		if (v[middle] == value)
			found = true;
		else if (v[middle] < value)
			bottom = middle + 1;
		else
			top = middle - 1;
	}
	if (!found)
		return -1;
	else
		return middle;
}

vector<string> removeDuplicados(vector<string> &lista)
{
	vector<string> resp;
	for (size_t i = 0; i < lista.size(); i++)
	{
		if (binarySearch(resp, lista[i]) == -1)
			resp.push_back(lista[i]);
	}
	return resp;
}

void guardaPalavras(vector<string> &lista, ofstream &lista_palavras)
{
	for (size_t i = 0; i < lista.size(); i++)
		lista_palavras << lista[i] << endl;
}

void showVector(const vector<string> &v)
{
	for (size_t i = 0; i < v.size(); i++)
		cout << v[i] << endl;
}

void ordenaPalavras(vector<string> &data)
{
	int j;
	string tmp;
	for (size_t i = 1; i < data.size(); i++)
	{
		j = i;
		tmp = data[i];
		while (j > 0 && tmp < data[j - 1])
		{
			data[j] = data[j - 1];
			j--;
		}
		data[j] = tmp;
	}
}

int main()
{
	ifstream dicionario;
	ofstream lista_palavras;
	string ficheiro_final, ficheiro_dicionario;
	vector<string> lista;

	cout << "EXTRACTION OF WORD LIST FROM DICTIONARY" << endl;
	cout << "=======================================" << endl << endl;
	cout << "Dictionary file ? ";
	getline(cin, ficheiro_dicionario);
	dicionario.open(ficheiro_dicionario);
	if (dicionario.fail())
	{
		cerr << "Input file opening failed." << endl;
		exit(1);
	}
	cout << "Word list file ? ";
	getline(cin, ficheiro_final);
	lista_palavras.open(ficheiro_final);
	if (lista_palavras.fail())
	{
		cerr << "Output file opening failed." << endl;
		exit(1);
	}

	cout << endl << "Extracting simple words from file " << ficheiro_dicionario << endl;
	cout << "beginning with letter ...";

	//adiciona as palavras ao vetor
	adicionaPalavras(lista, dicionario);
	cout << endl << "Number of simple words = " << lista.size() << endl;
	cout << "Sorting words ..." << endl;
	// ordena as palavras do vetor
	ordenaPalavras(lista);
	cout << endl << "Removing duplicate words ..." << endl;
	//remove palavras duplicadas
	lista = removeDuplicados(lista);
	cout << "Number of non-duplicate simple words = " << lista.size() << endl;
	cout << "Saving words into file " << ficheiro_final << " ..." << endl;
	//guarda as palavras no ficheiro
	guardaPalavras(lista, lista_palavras);
	cout << "End of processing." << endl;

	dicionario.close();
	lista_palavras.close();
	return 0;
}
