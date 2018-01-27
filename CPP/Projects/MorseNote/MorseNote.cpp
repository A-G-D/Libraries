#include <hashtable.hpp>
#include <linkedlist.hpp>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

/*
*	Constants
*/
const string DIT 						= ".";
const string DAH 						= "-";
const string MORSE_SPACE 				= "/";
const string NOTE_FILE_EXTENSION 		= ".txt";
const string RECORDS_FILE_NAME 			= "MorseNotes_Records";
const string NOTES_FOLDER 				= "Notes\\";
const string EXPORT_FOLDER				= "Exported\\";
const string BACK_COMMAND 				= "<back>";
const string RULER 						= "----------------------------------------------------------------------";
const char ESC 							= 27;
const char SPACE 						= 32;
const int DISPLAY_LINE_LIMIT 			= 10;
const int INPUT_LINE_LIMIT 				= 18;
/*
*
*/
char MORSE_LETTER_SEPARATOR = -1;
HashTable<char, bool> morse_flag(Hasher::character, 12);
HashTable<char, string> morse(Hasher::character, 12);
HashTable<string, char> alpha(Hasher::string, 12);
/*
*	Header
*/
void Display(string section, string hint, string body) {
	system("cls");
	cout<<"||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"
		<<"||||||                    MORSE NOTE CREATOR                    ||||||\n"
		<<"||||||----------------------------------------------------------||||||\n"
		<<"||||||                    " +  section   + "                    ||||||\n"
		<<"||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n\n"
		<<hint + "\n" + RULER + "\n" + body;
}
/*
*
*/
string AlphaToMorse(char c) {
	if(morse.has(c))
		return morse.load(c);
	return "";
}
char MorseToAlpha(const string &code) {
	if(alpha.has(code))
		return alpha.load(code);
	return 0;
}
inline bool IsCharMorse(char c) {
	return morse_flag.has(c);
}
bool IsTextMorse(string path) {
	bool flag = true;
	char c;

	ifstream file(path.data());
	while(file.get(c))
		if(!IsCharMorse(c) && c != MORSE_LETTER_SEPARATOR) {
			flag = false;
			break;
		}
	file.close();

	return flag;
}
/*
*
*/
string GetInputStr() {
	char c[30];
	gets(c);
	return string(c);
}
inline string I2S(int i) {
	return static_cast<ostringstream*>(&(ostringstream()<<i))->str();
}
int S2I(string s) {
	int i;
	istringstream(s)>>i;
	return i;
}
/*
*	Checks if a file with a similar name already exists in the directory and
*	appends a proper numeric suffix to the input name if necessary
*/
string GetFileName(string folder, string name) {
	name = folder + name;

	if(ifstream((name + NOTE_FILE_EXTENSION).data())) {
		short count = 2;
		while(ifstream((name + " (" + I2S(count) + ")" + NOTE_FILE_EXTENSION).data()))
			++count;
		name += " (" + I2S(count) + ")";
	}
	return name + NOTE_FILE_EXTENSION;
}
/*
*	Returns the truncated path of the file
*/
string GetTruncatedFileName(string path) {
	int size = path.size();
	if(size < 14) {
		while(size++ < 14)
			path += " ";
		return path;
	}
	return "..." + path.erase(0, size - 11);
}
/*
*	Removes the last letter from file during backspace
*/
template <typename T>
void UpdateFile(string path, List<T> &list) {
	ofstream temp((NOTES_FOLDER + "temp").data());

	for(typename List<T>::Node node = list.first(); node != list.head(); ++node)
		temp<<node.data();

	temp.close();
	remove(path.data());
	rename((NOTES_FOLDER + "temp").data(), path.data());
}
/*
*	Deletes non-existent note files from the record
*/
void UpdateRecord(vector<string> &vec) {
	ofstream temp("temp");

	for(int i = 0; i < vec.size(); ++i)
		temp<<vec[i]<<'\n';

	temp.close();
	remove(RECORDS_FILE_NAME.data());
	rename("temp", RECORDS_FILE_NAME.data());
}
/*
*	Translates a morse code and stores each line into a list
*/
List<string> TranslateFromMorse(ifstream &file, int line_limit) {
	List<string> text;
	string line = "", letter = "";
	int i = 0;

	char alpha, c;
	while(file.get(c)) {
		if(!IsCharMorse(c) || c == MORSE_LETTER_SEPARATOR || c == '\n') {
			alpha = MorseToAlpha(letter);
			if(alpha == '\n' || (letter == "" && c == '\n')) {
				text.enqueue(line + '\n');
				line = "";
				if(line_limit > 0 && ++i >= line_limit)
					text.pop();
			}else if(alpha)
				line += alpha;
			letter = "";
		}else
			letter += c;
	}

	if(line != "") {
		text.enqueue(line);
		if(line_limit > 0 && ++i >= line_limit)
			text.pop();
	}

	return text;
}
/*
*	Displays note content
*	(Reads the morse text from the file, and translates it into alphabet if <morse> is false)
*/
void DisplayNote(bool morse, string path, int line_limit, string hint) {
	string temp  = "";

	if(morse) {

		ifstream file(path.c_str());
		List<string> text;
		char c;
		int i = 0;

		while(file.get(c)) {
			if(c == '\n') {
				text.enqueue(temp + '\n');
				temp = "";
				if(line_limit > 0 && ++i >= line_limit)
					text.pop();
			}else
				temp += c;
		}
		file.close();

		if(temp != "") {
			text.enqueue(temp);
			if(line_limit > 0 && ++i >= line_limit)
				text.pop();
			temp = "";
		}

		while(!text.empty())
			temp += text.pop();

	}else {
		ifstream file(path.c_str());
		List<string> text(TranslateFromMorse(file, line_limit));
		file.close();

		while(!text.empty())
			temp += text.pop();
	}

	Display("[ " + GetTruncatedFileName(path.erase(0, NOTES_FOLDER.size())) + " ]", hint, "\n" + temp);
}
/*
*	Edits note content
*/
void EditNote(bool morse, string path) {
	unsigned char input;

	while(true) {
		DisplayNote(morse, path, INPUT_LINE_LIMIT, "Press ESC to finish input");

		input = getch();
		/*
		*	If the key pressed is a special key, ignore
		*/
		if(input == 0 || input == 0xE0)
			getch();
		/*
		*	Else, proceed
		*/
		else if(input == ESC)
			break;
	
		else if(AlphaToMorse(input) != "") {
			ofstream file(path.data(), ios::out|ios::app);
			file<<AlphaToMorse(input);
			if(AlphaToMorse(input) != "\n")
				file<<MORSE_LETTER_SEPARATOR;
			file.close();

		}else if(input == '\b') {
			ifstream file(path.data());

			List<char> list;
			char c;
			while(file.get(c))
				list.enqueue(c);

			file.close();

			if(!list.empty()) {
				if(list.last().data() == '\n')
					list.eject();
				else {
					for(List<char>::Node node = list.last(); !IsCharMorse(node.data()) && node.data() != '\n' && node != list.head(); --node)
						list.eject();

					for(List<char>::Node node = list.last(); IsCharMorse(node.data()) && node.data() != '\n' && node != list.head(); --node)
						list.eject();
				}
				UpdateFile(path, list);
			}
		}
	}
}
/*
*	Renames a note
*/
void RenameNote(string &path) {
	Display("   [Rename Note]  ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter a new name:\n");

	string input = GetInputStr();
	if(input == BACK_COMMAND)
		return;

	string name = GetFileName(NOTES_FOLDER, input);

	if(rename(path.data(), name.data()))
		Display("   [Rename Note]  ", "Press any key to go back", "\nERROR: Unable to rename note\n");

	else {
		string line;
		vector<string> vec;

		ifstream records(RECORDS_FILE_NAME.data());
		while(getline(records, line))
			if(line != path)
				vec.push_back(line);
			else
				vec.push_back(name);
		records.close();

		path = name;

		UpdateRecord(vec);
		Display("   [Rename Note]  ", "Press any key to go back", "\nNote successfully renamed!\n");
	}

	getch();
}
/*
*	Deletes a note
*/
void DeleteNote(string path) {
	if(remove(path.data()))
		Display("   [Delete Note]  ", "Press any key to go back", "\nERROR: Unable to delete note\n");
	else
		Display("   [Delete Note]  ", "Press any key to go back", "\nNote successfully deleted!\n");

	getch();
}
/*
*	Displays notes
*/
vector<string> DisplayNotes() {
	string line;
	vector<string> vec;

	ifstream records(RECORDS_FILE_NAME.data());
	while(getline(records, line))
		if(ifstream(line.data()))
			vec.push_back(line);
	records.close();

	UpdateRecord(vec);

	string notes = "";
	if(0 < vec.size())
		for(int i = 0; i < vec.size(); ++i)
			notes += (I2S(i + 1) + " : " + string(vec[i]).erase(0, NOTES_FOLDER.size()) + "\n");
	Display("      [Notes]     ", "Enter '" + BACK_COMMAND + "' to go back", "\n" + notes);

	return vec;
}
/*
*	Displays note options
*/
void DisplayNoteOptions(bool morse, string path) {
	DisplayNote(morse, path, DISPLAY_LINE_LIMIT, "Press ESC to go back");

	if(morse)
		cout<<"\n\n" + RULER + "\n\n1 : Edit\n2 : View in Text\n3 : Rename\n4 : Delete\n";
	else
		cout<<"\n\n" + RULER + "\n\n1 : Edit\n2 : View in Morse\n3 : Rename\n4 : Delete\n";

displayNoteOptions:
	switch(getch()) {

		case '1':
			EditNote(morse, path);
			break;

		case '2':
			DisplayNoteOptions(!morse, path);
			return;

		case '3':
			RenameNote(path);
			break;

		case '4':
			DeleteNote(path);
			return;

		case ESC:
			return;

		default:
			goto displayNoteOptions;
	}

	DisplayNoteOptions(morse, path);
}
/*
*	Displays note list
*/
void DisplayNoteList() {
	vector<string> vec = DisplayNotes();
	if(!vec.empty()) {
		cout<<"\n\nEnter the number of the note you want to view\n";

displayNoteList:
		string option = GetInputStr();
		int n = S2I(option);

		if(n > 0 && n <= vec.size())
			DisplayNoteOptions(false, vec[n - 1]);
		else if(option == BACK_COMMAND)
			return;
		else
			goto displayNoteList;

	}else {
		Display("      [Notes]     ", "Press any key to go back", "\nNote list is empty...\n");
		getch();
		return;
	}

	DisplayNoteList();
}
/*
*	Creates a new blank note
*/
void CreateNote() {
	Display("   [Create Note]  ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the name of the file to be created:\n");

	string input = GetInputStr();
	if(input == BACK_COMMAND)
		return;

	string path = GetFileName(NOTES_FOLDER, input);

	if((CreateDirectory(NOTES_FOLDER.data(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) && ofstream(path.data())) {
		ofstream records(RECORDS_FILE_NAME.data(), ios::out|ios::app);
		records<<path + "\n";
		records.close();

		Display("   [Create Note]  ", "Press any key to go back", "\nNote successfully created!\n");

	}else
		Display("   [Create Note]  ", "Press any key to go back", "\nERROR : Unable to create file\n");

	getch();
}
/*
*	Creates a new morse note based on an existing text file
*/
void CreateMorseFromFile() {
	Display("   [Create Note]  ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the path of the file to be translated:\n");

	string path = GetInputStr();
	if(path == BACK_COMMAND)
		return;

	ifstream ifile(path.data());
	if(ifile) {

		Display("   [Create Note]  ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the name of the translation file:\n");

		string input = GetInputStr();
		if(input == BACK_COMMAND) {
			ifile.close();
			return;
		}

		string name = GetFileName(NOTES_FOLDER, input);

		if((CreateDirectory(NOTES_FOLDER.data(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS)) {

			ofstream ofile(name.data());
			if(ofile) {
				ofstream records(RECORDS_FILE_NAME.data(), ios::out|ios::app);
				records<<name + "\n";
				records.close();

				char c;
				string morse;
				while(ifile.get(c)) {
					morse = AlphaToMorse(c);
					if(morse != "") {
						ofile<<morse;
						if(morse != "\n")
							ofile<<' ';
					}
				}

				ofile.close();
				Display("   [Create Note]  ", "Press any key to go back", "\nNote successfully created!\n");

			}else
				Display("   [Create Note]  ", "Press any key to go back", "\nERROR : Unable to create file\n");

		}else
			Display("   [Create Note]  ", "Press any key to go back", "\nERROR : Unable to create file\n");

		ifile.close();
	}else
		Display("   [Create Note]  ", "Press any key to go back", "\nERROR : Unable to read file\n");

	getch();
}
/*
*	Displays the note creation options
*/
void CreateOptions() {
	Display("   [Create Note]  ", "Press ESC to go back", "\n1 : Create Blank Note\n2 : Create Note Based on a File\n");

createOptions:
	switch(getch()) {

		case '1':
			CreateNote();
			break;

		case '2':
			CreateMorseFromFile();
			break;

		case ESC:
			return;

		default:
			goto createOptions;
	}
}
/*
*	Alphabet -> Morse Code translator
*/
void TextToMorseTranslator() {
	Display("   [Translator]   ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the path of the text file you wish to translate to morse:\n");

	string path = GetInputStr();
	if(path == BACK_COMMAND)
		return;

	ifstream file(path.data());
	if(file) {

		char c;
		string morse, text = "";

		while(file.get(c)) {
			morse = AlphaToMorse(c);
			if(morse != "") {
				text += morse;
				if(morse != "\n")
					text += ' ';
			}
		}
		file.close();

		Display("[ " + GetTruncatedFileName(path) + " ]", "Press ESC to go back", text + "\n");

textToMorseTranslator:
		if(getch() == ESC)
			return;
		goto textToMorseTranslator;

	}else {
		Display("   [Translator]   ", "Press any key to try again", "\nERROR : Unable to read file\n");
		getch();
	}

	TextToMorseTranslator();
}
/*
*	Morse Code -> Alphabet translator
*/
void MorseToTextTranslator() {
	Display("   [Translator]   ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the path of the morse file you want to translate to text:\n");

	string path = GetInputStr();
	if(path == BACK_COMMAND)
		return;

	ifstream file(path.data());
	if(file) {

		List<string> list = TranslateFromMorse(file, 0);
		file.close();

		string text = "";
		while(!list.empty())
			text += list.pop();

		Display("[ " + GetTruncatedFileName(path) + " ]", "Press ESC to go back", text + "\n");

morseToTextTranslator:
		if(getch() == ESC)
			return;
		goto morseToTextTranslator;

	}else {
		Display("[ " + GetTruncatedFileName(path) + " ]", "Press any key to try again", "\nERROR : Unable to read file\n");
		getch();
	}

	MorseToTextTranslator();
}
/*
*	Translator options
*/
void TranslatorOptions() {
	Display("   [Translator]   ", "Press ESC to go back", "\n1 : Text to Morse Translator\n2 : Morse to Text Translator\n");

translatorOptions:
	switch(getch()) {

		case '1':
			TextToMorseTranslator();
			break;

		case '2':
			MorseToTextTranslator();
			break;

		case ESC:
			return;

		default:
			goto translatorOptions;
	}

	TranslatorOptions();
}
/*
*	Applies a user-specified level of encryption to a file
*/
void Encrypter() {
	Display("    [Encrypter]   ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the path of the file you want to encrypt:\n");

	string path = GetInputStr();
	if(path == BACK_COMMAND)
		return;

	ifstream file(path.data());
	if(file) {
		file.close();

		Display("    [Encrypter]   ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the name of the encrypted copy:\n");

		string name = GetInputStr();
		if(name == BACK_COMMAND)
			return;

		name = GetFileName(EXPORT_FOLDER, name);
		if((CreateDirectory(EXPORT_FOLDER.data(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) && ofstream(name.data())) {

			Display("    [Encrypter]   ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the level of encryption:\n");

			string input = GetInputStr();
			if(input == BACK_COMMAND)
				return;

			int level = S2I(input);
			if(level > 0) {

				ifstream ifile(path.data());
				ofstream ofile(name.data());

				ofile<<ifile.rdbuf();

				ifile.close();
				ofile.close();

				List<char> list;
				string morse;
				char c;

				while(level--) {

					file.open(name);
					while(file.get(c)) {
						morse = AlphaToMorse(c);
						for(int i = 0; i < morse.size(); ++i)
							list.enqueue(morse[i]);
						list.enqueue(MORSE_LETTER_SEPARATOR);
					}
					file.close();

					UpdateFile(name, list);
					list.clear();
				}

				Display("    [Encrypter]   ", "Press any key to go back", "\nFile successfully encrypted!\n");
				getch();
				return;

			}else
				Display("    [Encrypter]   ", "Press any key to try again", "ERROR : Entered level of encryption is invalid\n");
		}else {
			Display("    [Encrypter]   ", "Press any key to go back", "\nERROR : Unable to create file\n");
			return;
		}
		getch();

	}else {
		Display("    [Encrypter]   ", "Press any key to try again", "\nERROR : Unable to read file\n");
		getch();
	}

	Encrypter();
}
/*
*	Decrypts an encrypted file
*/
void Decrypter() {
	Display("    [Decrypter]   ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the path of the file you want to decrypt:\n");

	string path = GetInputStr();
	if(path == BACK_COMMAND)
		return;

	ifstream file(path.data());
	if(file) {
		file.close();

		Display("    [Decrypter]   ", "Enter '" + BACK_COMMAND + "' to go back", "\nEnter the name decrypted copy:\n");

		string name = GetInputStr();
		if(name == BACK_COMMAND)
			return;

		name = GetFileName(EXPORT_FOLDER, name);
		if((CreateDirectory(EXPORT_FOLDER.data(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) && ofstream(name.data())) {

			ifstream ifile(path.data());
			ofstream ofile(name.data());
	
			ofile<<ifile.rdbuf();
	
			ifile.close();
			ofile.close();
	
			List<string> list;

			while(IsTextMorse(name)) {
				file.open(name);
				list.clear();
				list = TranslateFromMorse(file, 0);
				file.close();

				UpdateFile(name, list);
			}

			Display("    [Decrypter]   ", "Press any key to go back", "\nFile successfully decrypted!\n");
			getch();
			return;

		}else {
			Display("    [Decrypter]   ", "Press any key to go back", "\nERROR : Unable to create file\n");
			return;
		}
		getch();

	}else {
		Display("    [Decrypter]   ", "Press any key to try again", "\nERROR : Unable to read file\n");
		getch();
	}

	Decrypter();
}
/*
*	File encryption menu
*/
void FileEncryption() {
	Display("[File Encryption] ", "Press ESC to go back", "\n1 : Encrypt File\n2 : Decrypt File\n");

fileEncryption:
	switch(getch()) {

		case '1':
			Encrypter();
			break;

		case '2':
			Decrypter();
			break;

		case ESC:
			return;

		default:
			goto fileEncryption;
	}

	FileEncryption();
}
/*
*	Main menu
*/
void MainMenu() {
	Display("    [Main Menu]   ", "Press ESC to exit program", "\n1 : Create Note\n2 : Display Notes\n3 : Translator\n4 : File Encryption\n");

mainMenu:
	switch(getch()) {

		case '1':
			CreateOptions();
			break;

		case '2':
			DisplayNoteList();
			break;

		case '3':
			TranslatorOptions();
			break;

		case '4':
			FileEncryption();
			break;

		case ESC:
			exit(0);

		default:
			goto mainMenu;
	}

	MainMenu();
}
/*
*	Initialize user defined translation
*/
bool InitCustomTranslation() {
	ifstream file("mn_translation");
	if(file) {

		List<char> list;

		char c;
		while(file.get(c))
			list.enqueue(c);
		file.close();

		short phase = 0;
		string str = "";
		List<char>::Node head = list.head();
		/*
		*	Look for the user-definition of the morse letter separator character
		*/
		for(List<char>::Node node = head.next(); node != head; ++node) {

			switch(phase) {

				case 0:
					if(node.data() == '#')
						phase = 1;
					break;

				case 1:
					if(node.data() == '\n')
						phase = 0;
					else {
						str += node.data();
						if(str == "MorseLetterSeparator")
							phase = 2;
					}
					break;

				case 2:
					if(node.data() == '=')
						phase = 3;
					else if(node.data() != ' ' && node.data() != '\t')
						phase = 0;
					break;

				case 3:
					if(node.data() == '\'')
						phase = 4;
					else if(node.data() != ' ' && node.data() != '\t')
						phase = 0;
					break;

				case 4:
					if(node.next().data() != '\'') {
						phase = 0;
						str = "";
					}else
						MORSE_LETTER_SEPARATOR = node.data();
					break;
			}
		}

		if(MORSE_LETTER_SEPARATOR == -1) {
			Display("     [Warning]    ", "Press any key to continue", "No valid morse letter separator is defined. Because of this, the default hardcoded translation will be used.\n");
			getch();
			return false;
		}
		/*
		*	Read all user-defined character translation
		*/
		phase = 0;
		str = "";
		List<char>::Node next;
		for(List<char>::Node node = head.next(); node != head; ++node) {

			switch(phase) {

				case 0:
					if(node.data() == '@')
						phase = 1;
					break;

				case 1:
					c = node.data();
					phase = 2;
					break;

				case 2:
					if(node.data() == '{')
						phase = 3;
					else if(node.data() == '@')
						phase = 1;
					else if(node.data() != ' ' && node.data() != '\t' && node.data() != '\n')
						phase = 0;
					break;

				case 3:
					if(node.data() == '}') {
						if(c == '\n')
							morse.insert('\r', str);
						morse.insert(c, str);
						::alpha.insert(str, c);
						str = "";
						phase = 0;

					}else if(node.data() == MORSE_LETTER_SEPARATOR) {
						morse.clear();
						::alpha.clear();

						Display("     [Warning]    ", "Press any key to continue", string("\nMorse code letter separator '") +
								MORSE_LETTER_SEPARATOR + "' is also used as a morse character, which may cause parsing failure.\
								Because of this, the default hardcoded translation is used instead.\n");
						getch();
						return false;

					}else {
						str += node.data();
						morse_flag.insert(node.data(), true);
					}
					break;
			}
		}

		if(morse.has(' ')) {
			string space = morse.load(' ');
			morse.insert('\t', space + MORSE_LETTER_SEPARATOR + space + MORSE_LETTER_SEPARATOR + space + MORSE_LETTER_SEPARATOR + space);
		}

		return !morse.empty();
	}

	Display("    [Warning]    ", "Press any key to continue", "Unable to find/read the file \"mn_translation\".\
			The program will use the default hardcoded translation.\n");
	getch();

	return false;
}
/*
*	Initializes Alphabet <-> Morse Code caster
*	Only runs if there's no user-specified translation
*/
void InitDefaultTranslation() {
	morse.insert('0', DAH + DAH + DAH + DAH + DAH);
	morse.insert('1', DIT + DAH + DAH + DAH + DAH);
	morse.insert('2', DIT + DIT + DAH + DAH + DAH);
	morse.insert('3', DIT + DIT + DIT + DAH + DAH);
	morse.insert('4', DIT + DIT + DIT + DIT + DAH);
	morse.insert('5', DIT + DIT + DIT + DIT + DIT);
	morse.insert('6', DAH + DIT + DIT + DIT + DIT);
	morse.insert('7', DAH + DAH + DIT + DIT + DIT);
	morse.insert('8', DAH + DAH + DAH + DIT + DIT);
	morse.insert('9', DAH + DAH + DAH + DAH + DIT);
	morse.insert('A', DIT + DAH);
	morse.insert('B', DAH + DIT + DIT + DIT);
	morse.insert('C', DAH + DIT + DAH + DIT);
	morse.insert('D', DAH + DIT + DIT);
	morse.insert('E', DIT);
	morse.insert('F', DIT + DIT + DAH + DIT);
	morse.insert('G', DAH + DAH + DIT);
	morse.insert('H', DIT + DIT + DIT + DIT);
	morse.insert('I', DIT + DIT);
	morse.insert('J', DIT + DAH + DAH + DAH);
	morse.insert('K', DAH + DIT + DAH);
	morse.insert('L', DIT + DAH + DIT + DIT);
	morse.insert('M', DAH + DAH);
	morse.insert('N', DAH + DIT);
	morse.insert('O', DAH + DAH + DAH);
	morse.insert('P', DIT + DAH + DAH + DIT);
	morse.insert('Q', DAH + DAH + DIT + DAH);
	morse.insert('R', DIT + DAH + DIT);
	morse.insert('S', DIT + DIT + DIT);
	morse.insert('T', DAH);
	morse.insert('U', DIT + DIT + DAH);
	morse.insert('V', DIT + DIT + DIT + DAH);
	morse.insert('W', DIT + DAH + DAH);
	morse.insert('X', DAH + DIT + DIT + DAH);
	morse.insert('Y', DAH + DIT + DAH + DAH);
	morse.insert('Z', DAH + DAH + DIT + DIT);
	morse.insert('a', DIT + DAH);
	morse.insert('b', DAH + DIT + DIT + DIT);
	morse.insert('c', DAH + DIT + DAH + DIT);
	morse.insert('d', DAH + DIT + DIT);
	morse.insert('e', DIT);
	morse.insert('f', DIT + DIT + DAH + DIT);
	morse.insert('g', DAH + DAH + DIT);
	morse.insert('h', DIT + DIT + DIT + DIT);
	morse.insert('i', DIT + DIT);
	morse.insert('j', DIT + DAH + DAH + DAH);
	morse.insert('k', DAH + DIT + DAH);
	morse.insert('l', DIT + DAH + DIT + DIT);
	morse.insert('m', DAH + DAH);
	morse.insert('n', DAH + DIT);
	morse.insert('o', DAH + DAH + DAH);
	morse.insert('p', DIT + DAH + DAH + DIT);
	morse.insert('q', DAH + DAH + DIT + DAH);
	morse.insert('r', DIT + DAH + DIT);
	morse.insert('s', DIT + DIT + DIT);
	morse.insert('t', DAH);
	morse.insert('u', DIT + DIT + DAH);
	morse.insert('v', DIT + DIT + DIT + DAH);
	morse.insert('w', DIT + DAH + DAH);
	morse.insert('x', DAH + DIT + DIT + DAH);
	morse.insert('y', DAH + DIT + DAH + DAH);
	morse.insert('z', DAH + DAH + DIT + DIT);
	morse.insert('.', DIT + DAH + DIT + DAH + DIT + DAH);
	morse.insert(',', DAH + DAH + DIT + DIT + DAH + DAH);
	morse.insert(':', DAH + DAH + DAH + DIT + DIT + DIT);
	morse.insert('?', DIT + DIT + DAH + DAH + DIT + DIT);
	morse.insert(44,  DIT + DAH + DAH + DAH + DAH + DIT);
	morse.insert('-', DAH + DIT + DIT + DIT + DIT + DAH);
	morse.insert('/', DAH + DIT + DIT + DAH + DIT);
	morse.insert('(', DAH + DIT + DAH + DAH + DIT + DAH);
	morse.insert('"', DIT + DAH + DIT + DIT + DAH + DIT);
	morse.insert('@', DIT + DAH + DAH + DIT + DAH + DIT);
	morse.insert('=', DAH + DIT + DIT + DIT + DAH);
	morse.insert('\r', "\n");
	morse.insert('\n', "\n");
	morse.insert('\t', MORSE_SPACE + " " + MORSE_SPACE + " " + MORSE_SPACE + " " + MORSE_SPACE);
	morse.insert(' ', MORSE_SPACE);

	alpha.insert(DAH + DAH + DAH + DAH + DAH, '0');
	alpha.insert(DIT + DAH + DAH + DAH + DAH, '1');
	alpha.insert(DIT + DIT + DAH + DAH + DAH, '2');
	alpha.insert(DIT + DIT + DIT + DAH + DAH, '3');
	alpha.insert(DIT + DIT + DIT + DIT + DAH, '4');
	alpha.insert(DIT + DIT + DIT + DIT + DIT, '5');
	alpha.insert(DAH + DIT + DIT + DIT + DIT, '6');
	alpha.insert(DAH + DAH + DIT + DIT + DIT, '7');
	alpha.insert(DAH + DAH + DAH + DIT + DIT, '8');
	alpha.insert(DAH + DAH + DAH + DAH + DIT, '9');
	alpha.insert(DIT + DAH, 'A');
	alpha.insert(DAH + DIT + DIT + DIT, 'B');
	alpha.insert(DAH + DIT + DAH + DIT, 'C');
	alpha.insert(DAH + DIT + DIT, 'D');
	alpha.insert(DIT, 'E');
	alpha.insert(DIT + DIT + DAH + DIT, 'F');
	alpha.insert(DAH + DAH + DIT, 'G');
	alpha.insert(DIT + DIT + DIT + DIT, 'H');
	alpha.insert(DIT + DIT, 'I');
	alpha.insert(DIT + DAH + DAH + DAH, 'J');
	alpha.insert(DAH + DIT + DAH, 'K');
	alpha.insert(DIT + DAH + DIT + DIT, 'L');
	alpha.insert(DAH + DAH, 'M');
	alpha.insert(DAH + DIT, 'N');
	alpha.insert(DAH + DAH + DAH, 'O');
	alpha.insert(DIT + DAH + DAH + DIT, 'P');
	alpha.insert(DAH + DAH + DIT + DAH, 'Q');
	alpha.insert(DIT + DAH + DIT, 'R');
	alpha.insert(DIT + DIT + DIT, 'S');
	alpha.insert(DAH, 'T');
	alpha.insert(DIT + DIT + DAH, 'U');
	alpha.insert(DIT + DIT + DIT + DAH, 'V');
	alpha.insert(DIT + DAH + DAH, 'W');
	alpha.insert(DAH + DIT + DIT + DAH, 'X');
	alpha.insert(DAH + DIT + DAH + DAH, 'Y');
	alpha.insert(DAH + DAH + DIT + DIT, 'Z');
	alpha.insert(DIT + DAH + DIT + DAH + DIT + DAH, '.');
	alpha.insert(DAH + DAH + DIT + DIT + DAH + DAH, ',');
	alpha.insert(DAH + DAH + DAH + DIT + DIT + DIT, ':');
	alpha.insert(DIT + DIT + DAH + DAH + DIT + DIT, '?');
	alpha.insert(DIT + DAH + DAH + DAH + DAH + DIT, 44 );
	alpha.insert(DAH + DIT + DIT + DIT + DIT + DAH, '-');
	alpha.insert(DAH + DIT + DIT + DAH + DIT, '/');
	alpha.insert(DAH + DIT + DAH + DAH + DIT + DAH, '(');
	alpha.insert(DIT + DAH + DIT + DIT + DAH + DIT, '"');
	alpha.insert(DIT + DAH + DAH + DIT + DAH + DIT, '@');
	alpha.insert(DAH + DIT + DIT + DIT + DAH, '=');
	alpha.insert("\n" 	 , '\n');
	alpha.insert(MORSE_SPACE, ' ');

	morse_flag.insert(DIT[0], true);
	morse_flag.insert(DAH[0], true);
	morse_flag.insert(MORSE_SPACE[0], true);

	MORSE_LETTER_SEPARATOR = ' ';
}
/*
*	Main function
*/
int main() {
	if(!InitCustomTranslation())
		InitDefaultTranslation();

	MainMenu();

	return 0;
}
