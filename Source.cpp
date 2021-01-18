/*
	Hossam ElDin Khaled Mohamed Ali ElShaer
	Classical Ciphers
	16P3025
*/

#include "iostream"

#include <fstream>
#include <sstream>

#include <vector>

using namespace std;

string read_file(string file_path) {
	ifstream data(file_path);
	vector<string> words;
	string line, word;

	// Read file
	if (data.is_open()) {
		while (getline(data, line)) {
			istringstream stringStream(line);
			while (stringStream >> word) {
				words.push_back(word);
			}
		}
		data.close();
	}
	else {
		cout << "Failed to open file" << endl << "Please make sure Input Files exist in the same directory"
			<< endl << endl;
	}

	string s = "";
	for (int i = 0; i < words.size(); i++)
		s += words[i];

	return s;
}
void write_file(string file_path, string words) {
	ofstream new_file(file_path);
	new_file << words << endl;

	new_file.close();
}

bool is_lower_case(char c) {
	return c > 90;
}
char to_upper_case(char c) {
	if (is_lower_case(c))
		return c - 32;
	else
		return c;
}
char to_lower_case(char c) {
	if (is_lower_case(c))
		return c;
	else
		return c + 32;
}
char to_raw_digit(char c) {
	if (is_lower_case(c))
		return c - 97;
	else
		return c - 65;
}
char raw_to_upper(char c) {
	return c + 65;
}

string caesar_encrypt(string plaintext, int key) {
	string ciphertext = "";
	for (int j = 0; j < plaintext.length(); j++) {
		ciphertext += (((to_lower_case(plaintext[j]) - 97) + key) % 26) + 97;
	}
	return ciphertext;
}
string caesar_cipher(string plaintext, int key) {
	// read file
	string file_plaintext = read_file("Input Files/Caesar/caesar_plain.txt");

	// encryption
	string file_ciphertext = caesar_encrypt(file_plaintext, key);
	string ciphertext = caesar_encrypt(plaintext, key);

	// output
	write_file("Output Files/Caesar/caesar_cipher.txt", file_ciphertext);

	return ciphertext;
}

bool exists_inList(char key_list[25], char c) {
	for (int i = 0; i < 25; i++)
		if (key_list[i] == c)
			return true;
	return false;
}
int find_index(char* key_list, char c) {
	if (c == 'J') c = 'I';
	for (int i = 0; i < 25; i++) {
		if (key_list[i] == c)
			return i;
	}
	return -1;
}
void replace_char(char* key_list, char fc, char sc, char* c1, char* c2) {
	int fi = find_index(key_list, fc);
	int fir = fi / 5;
	int fic = fi % 5;

	int si = find_index(key_list, sc);
	int sir = si / 5;
	int sic = si % 5;

	if (fir == sir) {
		*c1 = key_list[fir * 5 + (fic + 1) % 5];
		*c2 = key_list[sir * 5 + (sic + 1) % 5];
	}
	else if (fic == sic) {
		*c1 = key_list[((fir + 1) % 5) * 5 + fic];
		*c2 = key_list[((sir + 1) % 5) * 5 + sic];
	}
	else {
		*c1 = key_list[fir * 5 + sic];
		*c2 = key_list[sir * 5 + fic];
	}
}
string playfair_encrypt(string plaintext, char* key_list) {
	string s = "";
	for (int j = 0; j < plaintext.length(); j += 2) {
		char fc, sc, c1, c2;
		if (j == plaintext.length() - 1) {
			fc = plaintext[j];
			sc = 'x';
		}
		else {
			fc = plaintext[j];
			sc = plaintext[j + 1];
			if (fc == sc) {
				sc = 'x';
				j--;
			}
		}
		replace_char(key_list, to_upper_case(fc), to_upper_case(sc), &c1, &c2);
		s += c1;
		s += c2;
	}
	return s;
}
string playfair_cipher(string plaintext, string key) {
	// read file
	string file_plaintext = read_file("Input Files/Playfair/playfair_plain.txt");

	// Key Matrix initialization
	char key_list[25];

	int j = 0;
	for (int i = 0; i < key.length(); i++) {
		if (!exists_inList(key_list, to_upper_case(key[i])))
			key_list[j++] = to_upper_case(key[i]);
	}
	for (int i = 65; i < 91; i++) {
		if (i != 74) {
			if (!exists_inList(key_list, char(i)))
				key_list[j++] = char(i);
		}
	}

	// encryption
	string file_ciphertext = playfair_encrypt(file_plaintext, key_list);
	string ciphertext = playfair_encrypt(plaintext, key_list);

	// output file
	write_file("Output Files/Playfair/playfair_cipher.txt", file_ciphertext);

	return ciphertext;
}

string adjust_length(string plaintext, int key_size) {
	int add = key_size - plaintext.length() % key_size;
	if (add == key_size)
		return plaintext;

	for (int i = 0; i < add; i++)
		plaintext += plaintext[i];

	return plaintext;
}
string hill_encrypt_2x2(string plaintext, int key[2][2]) {
	string ciphertext = "";

	// divide text and mutiply
	for (int i = 0; i < plaintext.length(); i += 2) {
		char fc = to_raw_digit(plaintext[i]);
		char sc = to_raw_digit(plaintext[i + 1]);

		ciphertext += raw_to_upper((key[0][0] * fc + key[0][1] * sc) % 26);
		ciphertext += raw_to_upper((key[1][0] * fc + key[1][1] * sc) % 26);
	}

	return ciphertext;
}
string hill_cipher_2x2(string plaintext, int* key) {
	// read file and adjust plaintext length
	string file_plaintext = read_file("Input Files/Hill/hill_plain_2x2.txt");
	file_plaintext = adjust_length(file_plaintext, 2);

	plaintext = adjust_length(plaintext, 2);

	// construct key matrix
	int key_matrix[2][2] = { {key[0], key[1]}, {key[2], key[3]} };

	// encrypt
	string file_ciphertext = hill_encrypt_2x2(file_plaintext, key_matrix);
	string ciphertext = hill_encrypt_2x2(plaintext, key_matrix);

	// output
	write_file("Output Files/Hill/hill_cipher_2x2.txt", file_ciphertext);

	return ciphertext;
}
string hill_encrypt_3x3(string plaintext, int key[3][3]) {
	string ciphertext = "";

	// divide text and mutiply
	for (int i = 0; i < plaintext.length(); i += 3) {
		char fc = to_raw_digit(plaintext[i]);
		char sc = to_raw_digit(plaintext[i + 1]);
		char tc = to_raw_digit(plaintext[i + 2]);

		ciphertext += raw_to_upper((key[0][0] * fc + key[0][1] * sc + key[0][2] * tc) % 26);
		ciphertext += raw_to_upper((key[1][0] * fc + key[1][1] * sc + key[1][2] * tc) % 26);
		ciphertext += raw_to_upper((key[2][0] * fc + key[2][1] * sc + key[2][2] * tc) % 26);
	}

	return ciphertext;
}
string hill_cipher_3x3(string plaintext, int* key) {
	// read file and adjust plaintext length
	string file_plaintext = read_file("Input Files/Hill/hill_plain_3x3.txt");
	file_plaintext = adjust_length(file_plaintext, 3);

	plaintext = adjust_length(plaintext, 3);

	// construct key matrix
	int key_matrix[3][3] = { {key[0], key[1], key[2]}, {key[3], key[4], key[5]}, {key[6], key[7], key[8]} };

	// encrypt
	string file_ciphertext = hill_encrypt_3x3(file_plaintext, key_matrix);
	string ciphertext = hill_encrypt_3x3(plaintext, key_matrix);

	// output
	write_file("Output Files/Hill/hill_cipher_3x3.txt", file_ciphertext);

	return ciphertext;
}

string generate_key(string plaintext, string key) {
	string generated_key = "";
	for (int i = 0; i < plaintext.length() / key.length() + 1; i++)
		generated_key += key;

	return generated_key;
}
string vigenere_encrypt(string plaintext, string key) {
	string ciphertext = "";
	for (int i = 0; i < plaintext.length(); i++) {
		ciphertext += raw_to_upper((to_upper_case(plaintext[i]) + to_upper_case(key[i])) % 26);
	}

	return ciphertext;
}
string vigenere_cipher(string plaintext, string key, bool auto_mode) {
	// read file
	string file_plaintext = read_file("Input Files/Vigenere/vigenere_plain.txt");

	// key generation
	string file_key = "", input_key = "";
	if (auto_mode) {
		file_key = key + file_plaintext;
		input_key = key + plaintext;
	}
	else {
		file_key = generate_key(file_plaintext, key);
		input_key = generate_key(plaintext, key);
	}

	// ecryption
	string file_ciphertext = vigenere_encrypt(file_plaintext, file_key);
	string ciphertext = vigenere_encrypt(plaintext, input_key);

	// output
	write_file("Output Files/Vigenere/vigenere_cipher.txt", file_ciphertext);
	return ciphertext;
}

string vernam_cipher(string plaintext, string key) {
	// read file
	string file_plaintext = read_file("Input Files/Vernam/vernam_plain.txt");

	// generate key
	string file_key = generate_key(file_plaintext, key);
	string input_key = generate_key(plaintext, key);

	// encrypt
	string file_ciphertext = vigenere_encrypt(file_plaintext, file_key);
	string ciphertext = vigenere_encrypt(plaintext, input_key);

	// output
	write_file("Output Files/Vernam/vernam_cipher.txt", file_ciphertext);
	return ciphertext;
}

int main() {
	
	cout << "Hossam ELShaer" << endl << "16P3025" << endl << "Classical Cipher Encryption" << endl << endl;
	
	cout << "Please enter plaintext (with no spaces): ";
	string plaintext;
	cin >> plaintext;
	cout << endl;

	
	cout << "Please enter Caesar Cipher key (must be a number): ";
	int caesar_key;
	cin >> caesar_key;
	string caesar_ciphertext = caesar_cipher(plaintext, caesar_key);
	cout << "Caesar Ciphertext: " << caesar_ciphertext << endl;
	cout << "Ciphertext of input file is saved under \"Output Files/Ceasar/ceasar_cipher.txt\"" << endl << endl;

	
	cout << "Please enter Playfair Cipher key: ";
	string playfair_key;
	cin >> playfair_key;
	string playfair_ciphertext = playfair_cipher(plaintext, playfair_key);
	cout << "Playfair Ciphertext: " << playfair_ciphertext << endl;
	cout << "Ciphertext of input file is saved under Output \"Files/Playfair/playfair_cipher.txt\"" << endl << endl;

	
	cout << "Please enter Hill Cipher key size (2 or 3): ";
	int key_size;
	cin >> key_size;
	if (key_size % 2 == 0) {
		cout << "Please enter Hill Cipher key 2x2: ";
		int hill_key_2x2[4];
		for (int i = 0; i < 4; i++) cin >> hill_key_2x2[i];
		string hill_2x2_ciphertext = hill_cipher_2x2(plaintext, hill_key_2x2);
		cout << "Hill 2x2 Ciphertext: " << hill_2x2_ciphertext << endl;
		cout << "Ciphertext of input file is saved under \"Output Files/Hill/hill_cipher_2x2.txt\"" << endl << endl;
	}
	else {
		cout << "Please enter Hill Cipher key 3x3: ";
		int hill_key_3x3[9];
		for (int i = 0; i < 9; i++) cin >> hill_key_3x3[i];
		string hill_3x3_ciphertext = hill_cipher_3x3(plaintext, hill_key_3x3);
		cout << "Hill 3x3 Ciphertext: " << hill_3x3_ciphertext << endl;
		cout << "Ciphertext of input file is saved under \"Output Files/Hill/hill_cipher_3x3.txt\"" << endl << endl;
	}

	cout << "Please enter Vigenere Cipher key: ";
	string vigenere_key;
	cin >> vigenere_key;
	cout << "Please enter Vigenere Cipher mode (0: repeating or 1:auto): ";
	int vigenere_mode;
	cin >> vigenere_mode;
	string vigenere_ciphertext = vigenere_cipher(plaintext, vigenere_key, vigenere_mode == 1);
	cout << "Vigenere Ciphertext: " << vigenere_ciphertext << endl;
	cout << "Ciphertext of input file is saved under \"Output Files/Vigenere/vigenere_cipher.txt\"" << endl << endl;
	
	cout << "Please enter Vernam Cipher key: ";
	string vernam_key;
	cin >> vernam_key;
	string vernam_ciphertext = vernam_cipher(plaintext, vernam_key);
	cout << "Vernam Ciphertext: " << vernam_ciphertext << endl;
	cout << "Ciphertext of input file is saved under \"Output Files/Vernam/vernam_cipher.txt\"" << endl << endl;
	
	return 0;
}