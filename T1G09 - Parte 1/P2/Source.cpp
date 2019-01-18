#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

void toUpperPalavra(string &palavra)      //Põe uma palavra com tudo letras maisculas
{
	for (size_t i = 0; i < palavra.size(); i++)
	{
		palavra[i] = toupper(palavra[i]);
	}
}

void retiraEspacos(string &palavra)
{
	string resp;
	int i, tam = palavra.length();
	for (i = 0; i < tam; i++)
	{
		if ((palavra[i] >= 'A') && (palavra[i] <= 'Z'))
			resp = resp + palavra[i];
	}
	palavra = resp;
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

string mix(string word)
{

	int size = word.size();
	for (int i = 0; i <= size / 2; i++)
	{
		int j;
		j = rand() % size;
		char k = word[i], l = word[j];
		word[i] = l;
		word[j] = k;
	}
	return word;

}

bool wildcardMatch(const char *str, const char *strWild)
{
	while (*strWild)
	{
		if (*strWild == '?')
		{
			if (!*str)
				return false;

			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			if (wildcardMatch(str, strWild + 1))
				return true;

			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			return false;
		}
		else
		{
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	return !*str && !*strWild;
}

void game1(const vector<string> &v)
{
	string word;
	cout << "Game rules: In this game you have to enter a word and we'll see if the word belongs to the word list. Let's begin." << endl;
	cout << "Enter a word: ";
	cin >> word;
	toUpperPalavra(word);
	if (binarySearch(v, word) != -1)
		cout << "Congratulations! The word was found." << endl;
	else
		cout << "Sorry, that word doesn't belong to the word list." << endl;
}

void game2(string originalword)
{
	cout << "Game rules: I will give you a set of letters that correspond to a scrambled word. Can you guess the original word? " << endl;
	string mixedword = mix(originalword);
	while (mixedword == originalword)
		mix(originalword);
	cout << mixedword << endl;

	bool acertou = false;

	for (int i = 1; i <= 3; i++)
	{
		string resposta;
		cout << "Guess the word: ";
		cin >> resposta;
		toUpperPalavra(resposta);
		if (i == 3)
			cout << "You lost. The right word was " << originalword << endl;
		else
		{
			if (originalword == resposta)
				acertou = true;
			else
				acertou = false;

			if (!acertou)
				cout << "You missed. Try again, you have " << 3 - i << " chances left." << endl;
			else
			{
				cout << "Congratulations. You guessed the word.";
				break;
			}
		}
	}
}

void game3(const vector<string> &v)
{
	cout << "Game Rules: Give a set of letters(no spaces between letters) and i will show you all the words that can be built with it." << endl;
	string letters;
	bool built = true;
	vector<string> formadas;

	cout << "Letters: ";
	cin >> letters;
	retiraEspacos(letters);
	toUpperPalavra(letters);
	for (size_t cont = 0; cont < v.size(); cont++)
	{
		if (v[cont].length() == letters.length())
		{
			string tmp = v[cont];

			for (int i = 0; i < letters.length(); i++)
			{
				if (tmp.find(letters[i]) != string::npos)
					tmp.erase(tmp.begin() + tmp.find(letters[i]));
				else
					break;
			}

			if (tmp.length() == 0)
				formadas.push_back(v[cont]);
		}
	}

	if (formadas.size() != 0)
	{
		cout << "Words from the word list built with the letters " << letters << ":" << endl;
		for (size_t cont = 0; cont < formadas.size(); cont++)
		{
			cout << formadas[cont] << endl;
		}
	}
	else
		cout << "There are no words from the word list that can be built with the letters " << letters << "." << endl;
}

int minimo(char v[], int n_elem)
{
	int min = v[0];
	for (size_t i = 1; i < n_elem; i++)
	{
		if (v[i] < min)
			min = v[i];
	}
	return min;
}

int frequency(const vector<string> &v, char letter)
{
	int contador = 0;
	for (int i = 0; i< v.size(); i++)
	{
		string s1 = v[i];
		for (int j = 0; j < s1.size(); j++)
		{
			if (s1[j] == letter)
				contador++;

		}
	}
	return contador;
}

vector<char> letrasProp(char v[], int n_elem)
{
	vector<char> resp;
	for (size_t i = 0; i < n_elem; i++)
	{
		for (int j = 0; j < v[i]; j++)
			resp.push_back(char(i + 'A'));
	}
	return resp;
}

vector<string> aux4(vector<string> palavras, string letters)
{
	bool built = true;
	vector<string> formadas;

	for (size_t cont = 0; cont < palavras.size(); cont++)
	{
		string tmp = palavras[cont];

		for (int i = 0; i < letters.length(); i++)
		{
			if (tmp.find(letters[i]) != string::npos)
				tmp.erase(tmp.begin() + tmp.find(letters[i]));
			else
				break;
		}

		if (tmp.length() == 0)
			formadas.push_back(palavras[cont]);
	}
	return formadas;
}
void game4(const vector<string> &v)
{
	cout << " Game rules: Choose the size for a set of letters. I will give you one and you have to try and form a word using it. Be careful, you can't use same letter more than it's frequency in the dictionary." << endl;
	int wordsize;
	cout << "Insert number of letters: ";
	cin >> wordsize;
	string userword;
	string randomset;
	srand(time(NULL));
	vector<string> possiblewords;
	vector<int> freq;
	char j = 65;

	int minindex = 27, minfreq = 95903;
	for (int i = 0; i < 26; i++)
	{
		if ((frequency(v, j + i)) < minfreq)
		{
			minindex = i;
			minfreq = (frequency(v, j + i));
		}
		freq.push_back(frequency(v, j + i));
	}

	double minfreqdec = minfreq / 95903.0000;
	vector<char> basevector;
	int sum = 0;
	for (int n = 0; n < freq.size(); n++)
	{
		freq.at(n) = (((freq.at(n) / 95903.000) * 2) / minfreqdec);
		sum = sum + freq.at(n);
		for (int x = 0; x < freq.at(n); x++)
		{
			basevector.push_back(65 + n);
		}
	}
	srand(time(NULL));
	while (possiblewords.empty())
	{
		for (int i = 0; i < wordsize; i++)
		{
			int s = rand() % basevector.size();
			char letter = basevector.at(s);
			randomset.push_back(letter);
		}
	possiblewords = aux4(v, randomset);
	}

	cout << "Form valid word with this set of letters: " << randomset << endl;
	cout << "Insert word: ";
	cin >> userword;
	toUpperPalavra(userword);
	
	if (binarySearch(v, userword) != -1)
		cout << "Congratulations! The word was found." << endl;
	else
		cout << "Sorry, that word doesn't belong to the word list." << endl;
}

void game5(const vector<string> vpalavras)
{
	cout << " Game rules: Insert a part of a word using wildcards in the missing spaces. Use '?' if you only want one letter in that space or '*' if you want words with different sizes." << endl;
	string partialword;
	cout << "Partial word: ";
	cin >> partialword;
	toUpperPalavra(partialword);
	const char *strwild = &partialword[0];
	for (size_t i = 0; i < vpalavras.size(); i++)
	{
		string strin = vpalavras[i];
		const char *str = &strin[0];
		if (wildcardMatch(str, strwild))
			cout << str << endl;
	}
}

int main()
{
	ifstream ficheiro;
	string file;
	cout << "Word file ? ";
	getline(cin, file);
	ficheiro.open(file);

	if (ficheiro.fail())
	{
		cerr << "Input file opening failed.\n";
		exit(1);
	}

	string linha;
	vector<string> palavras;
	while (getline(ficheiro, linha))
		palavras.push_back(linha);

	char answer;
	bool sair = false;

	while (!sair)
	{
		cout << "Choose an option from the following:" << endl;
		cout << "1. Does it exist? " << endl;
		cout << "2. Guess the word. " << endl;
		cout << "3. Can a word be built? " << endl;
		cout << "4. Can you build it? " << endl;
		cout << "5. Show me the words. " << endl;
		cout << "E. Exit" << endl;
		cout << "Option?";
		cin >> answer;
		cout << endl;
		answer = toupper(answer);
		while (!cin || ((answer != '1') && (answer != '2') && (answer != '3') && (answer != '4') && (answer != '5') && (answer != 'E')))
		{
			cin.clear();
			cin.ignore(1000000000, '\n');
			cout << "Option?";
			cin >> answer;
			answer = toupper(answer);
		}

		switch (answer)
		{
		case '1':
		{
			game1(palavras);
			cout << endl;
			break;
		}
		case '2':
		{
			cout << "Game rules: I will give you a set of letters that correspond to a scrambled word. Can you guess the original word? " << endl;
			int size = palavras.size();
			srand(time(NULL));
			int randomIndex = rand() % size;
			game2(palavras[randomIndex]);
			cout << endl;
			break;
		}
		case '3':
		{
			game3(palavras);
			cout << endl;
			break;
		}
		case '4':
		{
			game4(palavras);
			cout << endl;
			break;
		}
		case '5':
		{
			game5(palavras);
			cout << endl;
			break;
		}
		case 'E':
			sair = true;
			cout << endl << "Program developed by:" << endl << "Leonor Sousa" << endl << "Silvia Rocha" << endl;
			cout << "Alunas do Mestrado Integrado em Engenharia Informatica na Faculdade de Engenharia da Universidade do Porto" << endl;
			break;
		}
	}
	ficheiro.close();
	return 0;
}




