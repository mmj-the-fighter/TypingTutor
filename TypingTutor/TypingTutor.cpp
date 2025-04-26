/*

Command to compile:
g++ -std=c++11 TypingTutor.cpp -o TypingTutor

Command to run:
./TypingTutor

*/
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include<limits>

#define MAXCHARSPERLINE 80

enum {
	AlphabetsBit = 1,
	SpecialCharactersBit = 2,
	DigitsBit = 4
};

class TextFileLoader
{
private:
	char *text;
	int textLength;
	bool textEnded;
	std::string lastFilename;
public:
	TextFileLoader()
	{
		text = nullptr;
		textLength = 0;
		textEnded = false;
		lastFilename = "";
	}

	~TextFileLoader() {
		if (text != nullptr){
			delete[] text;
		}
	}

	bool LoadTextFromFile(std::string filename)
	{
		if (filename.compare("") == 0) {
			return false;
		}

		if (filename != lastFilename) {
			std::ifstream in(filename);
			if (!in.is_open()) {
				return false;
			}
			lastFilename = filename;
			std::string str((std::istreambuf_iterator<char>(in)),
				std::istreambuf_iterator<char>());
			//delete previous text
			if (text != nullptr){
				delete[] text;
				text = nullptr;
			}
			//allocate userchars and copy from temp string
			textLength = str.size();
			text = new char[textLength + 1];
			const char* pc = str.c_str();
			int i = 0;
			while ((text[i] = pc[i]) != '\0'){
				++i;
			}

		}
		textEnded = false;
		return true;
	}

	bool isTextEnded(){
		return textEnded;
	}

	void Reset() {
		textEnded = false;
	}

	void CopyText(char* buffer, int maxBufferLength, int *SSM)
	{
		int i = *SSM;
		int j = 0;
		while (true)
		{
			if (j > maxBufferLength - 1) {
				buffer[j] = '\0';
				*SSM = i;
				break;
			}

			//wrap around text
			if (i > textLength - 1) {
				buffer[j] = '\0';
				*SSM = 0;
				textEnded = true;
				break;
			}

			if (text[i] == '\n') {
				buffer[j] = '\0';
				*SSM = i + 1;
				break;
			}
			buffer[j] = text[i];
			++i; ++j;
		}
	}

};

class RandomTextGenerator
{
private:
	const char* atozSentence = "the quick brown fox jumps over a lazy dog";
	const char* alphabets = "abcdefghijklmnopqrstuvwxyz";
	const char* specialChars = " `~!@#$%^&*()-_=+[{]}\\|;:\"',<.>/?";
	const char* digits = "0123456789";
	char* userChars;
	int userCharsLength;
	bool randomSpace;
	bool randomCaps;
	bool randomText;

	int numOfAlphabets;
	int numOfSpecialChars;
	int numOfDigits;
	std::random_device seeder;
	std::mt19937 generator;
	std::uniform_int_distribution<int> distribution;
public:
	RandomTextGenerator() {
		numOfAlphabets = strlen(alphabets);
		numOfSpecialChars = strlen(specialChars);
		numOfDigits = strlen(digits);
		userChars = nullptr;
		randomCaps = true;
		randomSpace = true;
		randomText = true;
		userCharsLength = 0;
		generator.seed(seeder());
	}

	~RandomTextGenerator() {
		if (userChars != nullptr){
			delete[] userChars;
		}
	}

	inline int RandomInRange(int min, int max) {
		distribution.param(std::uniform_int_distribution<int>::param_type(min, max - 1));
		return distribution(generator);
	}

	void ReadUserChars()
	{
		//read characters into a temp string
		std::string str;
		std::getline(std::cin, str);
		//delete previous userchars
		if (userChars != nullptr){
			delete[] userChars;
			userChars = nullptr;
		}
		//allocate userchars and copy from temp string
		userCharsLength = str.size();
		userChars = new char[userCharsLength + 1];
		const char* pc = str.c_str();
		int i = 0;
		while ((userChars[i] = pc[i]) != '\0') {
			++i;
		}
	}

	void SetMode(bool _randomCaps, bool _randomSpace, bool _randomText)
	{
		randomCaps = _randomCaps;
		randomSpace = _randomSpace;
		randomText = _randomText;
	}

	void GenText(char* buffer, int bufferLength, int selectionBits) {
		bool AreAlphabetsRequired = (selectionBits & AlphabetsBit) ? true : false;
		bool AreSpecialCharsRequired = (selectionBits & SpecialCharactersBit) ? true : false;
		bool AreDigitsRequired = (selectionBits & DigitsBit) ? true : false;
		bufferLength = 5 + RandomInRange(0, bufferLength - 5);
		int choice;
		int lastSpaceIndex = -1;
		int i = 0, k = 0;
		while (i < bufferLength - 1)
		{
			if (randomSpace && i - lastSpaceIndex > 3) {
				if (RandomInRange(0, 4) == 0){
					buffer[i] = ' ';
					lastSpaceIndex = i;
					++i;
					continue;
				}
			}

			if (AreAlphabetsRequired && AreSpecialCharsRequired && AreDigitsRequired)
			{
				choice = RandomInRange(0, 3);
				if (choice == 0)
					buffer[i] = alphabets[RandomInRange(0, numOfAlphabets)];
				else if (choice == 1)
					buffer[i] = specialChars[RandomInRange(0, numOfSpecialChars)];
				else
					buffer[i] = digits[RandomInRange(0, numOfDigits)];
			}
			else if (!AreAlphabetsRequired && AreSpecialCharsRequired && AreDigitsRequired) {
				if (RandomInRange(0, 2) == 0)
					buffer[i] = specialChars[RandomInRange(0, numOfSpecialChars)];
				else
					buffer[i] = digits[RandomInRange(0, numOfDigits)];
			}
			else if (AreAlphabetsRequired && !AreSpecialCharsRequired && AreDigitsRequired) {
				if (RandomInRange(0, 2) == 0)
					buffer[i] = alphabets[RandomInRange(0, numOfAlphabets)];
				else
					buffer[i] = digits[RandomInRange(0, numOfDigits)];
			}
			else if (AreAlphabetsRequired && AreSpecialCharsRequired && !AreDigitsRequired) {
				if (RandomInRange(0, 2) == 0)
					buffer[i] = alphabets[RandomInRange(0, numOfAlphabets)];
				else
					buffer[i] = specialChars[RandomInRange(0, numOfSpecialChars)];
			}
			else if (!AreAlphabetsRequired && !AreSpecialCharsRequired && AreDigitsRequired) {
				buffer[i] = digits[RandomInRange(0, numOfDigits)];
			}
			else if (AreAlphabetsRequired && !AreSpecialCharsRequired && !AreDigitsRequired) {
				buffer[i] = alphabets[RandomInRange(0, numOfAlphabets)];
			}
			else if (!AreAlphabetsRequired && AreSpecialCharsRequired && !AreDigitsRequired) {
				buffer[i] = specialChars[RandomInRange(0, numOfSpecialChars)];
			}
			else
			{
				//if (RandomInRange(0, 2) == 0)
				//	buffer[i] = alphabets[RandomInRange(0, numOfAlphabets)];
				//else
				//	buffer[i] = digits[RandomInRange(0, numOfDigits)];
				if (randomText)
					buffer[i] = userChars[RandomInRange(0, userCharsLength)];
				else
					buffer[i] = userChars[k % userCharsLength];
			}
			if (randomCaps && i > 0 && buffer[i - 1] == ' ' && isalpha(buffer[i]) && RandomInRange(0, 6) == 0)
				buffer[i] = toupper(buffer[i]);
			++i; ++k;
		}
		buffer[bufferLength - 1] = '\0';
	}
};

class TypingTutor
{
public:
	void ReadConfigurationFromUser();
	void MainLoop();
private:
	RandomTextGenerator rtg;	char c[2];
	TextFileLoader tfl;
	bool fPracticeFileText;
	bool fPracticeFileTextWithTimeLimit;
	double practiceDuration;
	int selectionBits;
};

void TypingTutor::ReadConfigurationFromUser()
{
	char ch;
	std::string filename;
	filename = "";
	std::cout << "Enter filename: " << std::endl;
	std::getline(std::cin, filename);
	if (tfl.LoadTextFromFile(filename)) {
		fPracticeFileText = true;
		std::cout << "Do you want time limit? (y/n): ";
		(std::cin.get(ch)).get();
		if (ch == 'y' || ch == 'Y')
		{
			fPracticeFileTextWithTimeLimit = true;
			std::cout << "Enter intended practice duration in seconds: ";
			(std::cin >> practiceDuration).get();
		}
		else
		{
			fPracticeFileTextWithTimeLimit = false;
		}
	}
	else
		fPracticeFileText = false;

	if (!fPracticeFileText)
	{
		std::cout << "Enter intended practice duration in seconds: ";
		(std::cin >> practiceDuration).get();
		selectionBits = 0;
		std::cout << "Do you want to practice alphabets? (y/n): ";
		(std::cin.get(ch)).get();
		if (ch == 'y' || ch == 'Y')
			selectionBits |= AlphabetsBit;

		std::cout << "Do you want to practice special characters? (y/n): ";
		(std::cin.get(ch)).get();
		if (ch == 'y' || ch == 'Y')
			selectionBits |= SpecialCharactersBit;

		std::cout << "Do you want to practice digits? (y/n): ";
		(std::cin.get(ch)).get();
		if (ch == 'y' || ch == 'Y')
			selectionBits |= DigitsBit;

		if ((selectionBits & AlphabetsBit) == 0 &&
			(selectionBits & SpecialCharactersBit) == 0 &&
			(selectionBits & DigitsBit) == 0) {
			std::cout << "Enter the characters you want to practice:" << std::endl;
			rtg.ReadUserChars();
			std::cout << "Do you want to randomize this practice? (y/n): ";
			(std::cin.get(ch)).get();
			if (ch == 'y' || ch == 'Y')
				rtg.SetMode(false, false, true);
			else
				rtg.SetMode(false, false, false);
		}
		else{
			rtg.SetMode(true, true, true);
		}
	}

}

void TypingTutor::MainLoop()
{
	std::string str;
	char ch, lineBuffer[MAXCHARSPERLINE + 1];
	unsigned int linebufferLength;
	time_t startTime, endTime;
	double timeElapsed, charsPerSecond, wordsPerMinute, accuracy;
	int numOfCharsTyped, errorsOccured;
	bool continueLoading = false;
	bool continueWithCurConfig = false;

	std::cout << "Welcome to typing tutor" << std::endl;
	while (true)
	{
		if (!continueWithCurConfig) {
			ReadConfigurationFromUser();
		}
		numOfCharsTyped = 0;
		errorsOccured = 0;
		startTime = time(NULL);
		int SSM = 0;
		do
		{
			if (fPracticeFileText)
				tfl.CopyText(lineBuffer, MAXCHARSPERLINE, &SSM);
			else
				rtg.GenText(lineBuffer, MAXCHARSPERLINE, selectionBits);
			//display the line generated
			std::cout << lineBuffer << std::endl;
			//read the keys untill a newline is pressed
			std::getline(std::cin, str);
			//check the correctness
			linebufferLength = strlen(&lineBuffer[0]);
			for (unsigned int i = 0; i < linebufferLength; i++)
			{
				if (i < str.size()) {
					if (lineBuffer[i] != str[i])
						++errorsOccured;
					++numOfCharsTyped;
				}
				//else{
				//	++errorsOccured;
				//	++numOfCharsTyped;
				//}
			}
			int excess = str.size() - linebufferLength;
			if (excess > 0) {
				errorsOccured += excess;
				numOfCharsTyped += excess;
			}

			//note time
			endTime = time(NULL);
			timeElapsed = difftime(endTime, startTime);

			//decide for loading next section
			if (fPracticeFileText) {
				if (fPracticeFileTextWithTimeLimit)
					continueLoading = (timeElapsed <= practiceDuration) ? true : false;
				else
					continueLoading = !tfl.isTextEnded();
			}
			else{
				continueLoading = (timeElapsed <= practiceDuration) ? true : false;
			}
		} while (continueLoading);
		if (numOfCharsTyped == 0)
			accuracy = 0;
		else
			accuracy = ((numOfCharsTyped - errorsOccured) / (float)numOfCharsTyped) * 100.0;
		charsPerSecond = numOfCharsTyped / timeElapsed;
		wordsPerMinute = charsPerSecond * 60.0 / 5.0;

		std::cout
			<< "Time Elapsed: " << timeElapsed << std::endl
			<< "Errors: " << errorsOccured << std::endl
			<< "Accuracy(%): " << accuracy << std::endl
			<< "Speed(CPS): " << charsPerSecond << std::endl
			<< "Speed(WPM): " << wordsPerMinute << std::endl;

		std::cout << "\nTest finished\n";
		std::cout << "Do you want to take another test? (y/n) :";
		std::cin.getline(c, sizeof(c));
		if (c[0] == 'n' || c[0] == 'N' || c[1] == 'n' || c[1] == 'N') {
			break;
		}
		else {
			std::cout << "Do you want to continue with the current configuration? (y/n) :";
			std::cin.getline(c, sizeof(c));
			if (c[0] == 'n' || c[0] == 'N' || c[1] == 'n' || c[1] == 'N') {
				continueWithCurConfig = false;
			}
			else {
				continueWithCurConfig = true;
			}
			if (fPracticeFileText || fPracticeFileTextWithTimeLimit) {
				tfl.Reset();

			}
			continue;
		}
	}

}

int main()
{
	TypingTutor tt;
	tt.MainLoop();
	return 0;
}
