#include <iostream>
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

class RandomTextGenerator
{
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
		distribution.param(std::uniform_int_distribution<int>::param_type(min, max-1));
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
		while ((userChars[i] = pc[i++]) != '\0');
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
					buffer[i] = alphabets[RandomInRange(0,numOfAlphabets)];
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
					buffer[i] = userChars[RandomInRange(0,userCharsLength)];
				else 
					buffer[i] = userChars[k%userCharsLength];
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
	void MainLoop();
private:
	RandomTextGenerator rtg;	char c[2];

};

void TypingTutor::MainLoop()
{
	char ch, lineBuffer[MAXCHARSPERLINE];
	int linebufferLength, index;
	time_t startTime, endTime;
	double timeElapsed, practiceDuration, charsPerSecond, wordsPerMinute, accuracy;
	int numOfCharsTyped, errorsOccured;
	int selectionBits;

	std::cout << "Welcome to random text typing tutor" << std::endl;
	while (true)
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

		if ((selectionBits & AlphabetsBit) == 0 && (selectionBits & SpecialCharactersBit) == 0 && (selectionBits & DigitsBit) == 0) {
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
			

		numOfCharsTyped = 0;
		errorsOccured = 0;
		startTime = time(NULL);
		do
		{
			rtg.GenText(lineBuffer, MAXCHARSPERLINE, selectionBits);
			std::cout << lineBuffer << std::endl;
			index = 0;
			linebufferLength = strlen(&lineBuffer[0]);
			do
			{
				ch = std::cin.get();
				if (ch == '\n') {
					break;
				}
				++numOfCharsTyped;
				if (ch != lineBuffer[index++]) {
					++errorsOccured;
				}
			} while (index < linebufferLength);
			endTime = time(NULL);
			timeElapsed = difftime(endTime, startTime);
		} while (timeElapsed <= practiceDuration);
		if (numOfCharsTyped == 0)
			accuracy = 0;
		else
			accuracy = ((numOfCharsTyped - errorsOccured) / (float)numOfCharsTyped) * 100.0;
		charsPerSecond = numOfCharsTyped / timeElapsed;
		wordsPerMinute = charsPerSecond * 60.0 / 5.0;

		std::cout << "Accuracy(%): " << accuracy << std::endl
			<< "Speed(CPS): " << charsPerSecond << std::endl
			<< "Speed(WPM): " << wordsPerMinute << std::endl;

		std::cout << "\nTest finished\n";
		std::cout << "Do you want to take another test? (y/n) :";
		c[0] = std::cin.get();
		c[1] = std::cin.get();
		if (c[0] == 'n' || c[0] == 'N' || c[1] == 'n' || c[1] == 'N')
			break;
		else
			continue;
	}

}

int main()
{
	TypingTutor tt;
	tt.MainLoop();
	return 0;
}
