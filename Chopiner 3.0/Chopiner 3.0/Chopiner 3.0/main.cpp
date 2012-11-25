//
//  main.cpp
//  Chopiner 3.0
//
//  Created by Brett Wines on 11/25/12.
//  Copyright (c) 2012 Brett Wines. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <assert.h>
using namespace std;
string LH;
string RH;
int nSubBeats;
int timeSig;

struct noteT {
	double value;
	bool rest;
	double position;
};

string GetLine() {
	string result;
	getline(cin, result);
	return result;
}

int GetInteger() {
	while(true) {
		stringstream converter;
		converter << GetLine();
		
		int result;
		converter >> result;
		
		if(converter.fail())
			cout << "Please enter an integer. ";
		else {
			char remaining;
			converter >> remaining;
			
			if(converter.fail())
				return result;
			else
				cout << "Unexpected character: " << remaining << endl;
		}
		
		cout << "Retry: ";
	}
}

double Abs(double n) {
	return (n<0) ? -n : n;
}

bool CloseEnough(double sum) {
	return Abs(1 - sum) < .01;
}

double LCM(double a, double b) {
	double A = a;
	double B = b;
	while (!CloseEnough(1.0-(a-b)))
		(a < b) ? a += A : b += B;
	return ((int)a - a == 0) ? a : b;
}

int Round(double n) {
	int flooring = floor(n);
	int ceiling = ceil(n);
	return (n - flooring < ceiling - n) ? flooring : ceiling;
}

void PrintfBeat(string hand) {
	for (size_t i = 0; i < hand.length(); i++)
		hand[i] = '-';
	
	int every = (int)hand.length()/timeSig;
	for (size_t i = 0; i < timeSig; i++)
		hand[i*every] = '!';
	
	cout << "(The first line is the beat)\n" << hand << endl;
	return;
}

void CalculateNoteValue(const string source, noteT &currNote, const int i, int &n) {
	while (true) {
		if (source[i+n] == '_')
			currNote.value /= 2;
		else if (source[i+n] == '-')
			currNote.value *= 2;
		else if (source[i+n] == '.')
			currNote.value *= 1.5;
		else {
			n--;
			return;
		}
		n++;
	}
	
	return;
}

double FindModifier(const string source, int &n) {
	double modifier = 1;
	n++;
	while (source[n] == '_' || source[n] == '.' || source[n] == '-' ) {
		if (source[n] == '_')
			modifier *= .5;
		if (source[n] == '.')
			modifier *= 1.5;
		if (source[n] == '-')
			modifier++;
		n++;
	}
	n--;
	return modifier;
}

double WhatsTheKPlet(const string source, int &i);

double kPletBracketsSituation(const string source, int i) {
	vector<double> bracketedNotes, total;
	while (source[i] != ')') {
		double currentNoteValue = FindModifier(source, i);
		bracketedNotes.push_back(currentNoteValue);
		i++;
	}
	
	if (source[i+1] != '<') {
		int increment;
		for (size_t j = 0; j < bracketedNotes.size(); j++) { // Slightly inefficient but more readable and no copied code.
			increment = 1;
			noteT dummy;
			dummy.value = bracketedNotes[j];
			CalculateNoteValue(source, dummy, i, increment);
			total.push_back(dummy.value);
		}
		i += increment;
	} else if (source[i+1] == '<') {
		i += 2;
		double kPlet = WhatsTheKPlet(source, i);
		string number = "     ";
		double divideBy;
		int n = 0;
		while (source[i+n] != '>') {
			number[n] = source[i+n];
			n++;
		}
		i += n;
		
		divideBy = atof(number.c_str());
		double multiplier = divideBy/kPlet;
		
		for (size_t j = 0; j < bracketedNotes.size(); j++)
			total.push_back(bracketedNotes[j]*multiplier);
		
	} else {
		cout << "\nBad input: char \'" << source[i+1] << "\' after \')\'.\nSIGABRT time! Wheeeeeeeeeeeeeeeeeeee";
		assert(false);
	}
	
	double totalValue = 0;
	for (size_t x = 0; x < total.size(); x++)
		totalValue += total[x];
	
	return totalValue;
}

double WhatsTheKPlet(const string source, int &i) {
	int n = i;
	int count = 0;
	while (true) {
		if (source[n] == ')')
			count++;
		
		if (source[n] == '(') {
			count--;
			if (count == 0) {
				n--;
				break;
			}
		}
		n--;
	}
	
	n += 2;
	double kPlet = 0;
	while (n <= i - 2) {
		if (source[n] != '_' && source[n] != '-' && source[n] != '.' && source[n] != ')' && source[n] != '(' && source[n] != '>' && source[n] != '<') {
			double noteModifier = FindModifier(source, n);
			kPlet += 1.0*noteModifier;
			n++;
		} else if (source[n] == ')') {
			n++;
		} else {
			while (source[n] == '(')
				n++;
			
			if (source[n+1] == '>')
				break;
			
			kPlet += kPletBracketsSituation(source, n);
			
			while (source[n] != ')' && source[n] != '>')
				n++;
			n += 2;
			if (source[n+1] == '>')
				n += 2;
			if (source[n-1] == '<') {
				while (source[n] != '>')
					n++;
				n++;
			}
		}
	}
	
	while (source[i-1] != '<')
		i++;
    
	return kPlet;
}

void CalculateNplet(int &i, const string source, vector<noteT> &setOfNotes, vector<noteT> &notes) {
	i += 2;
	
	double kPlet = WhatsTheKPlet(source, i);
	
	string number = "      ";
	double divideBy;
	int n = 0;
	while (source[i+n] != '>') {
		number[n] = source[i+n];
		n++;
	}
	i += n;
	
	divideBy = atof(number.c_str());
	double multiplier = divideBy/kPlet;
	
	for (size_t j = 0; j < setOfNotes.size(); j++) {
		setOfNotes[j].value *= multiplier;
		notes.push_back(setOfNotes[j]);
	}
	
	return;
}

void CalculateCurrentNote(noteT &currNote, const string source, int i, int &increment) {
	CalculateNoteValue(source, currNote, i, increment);
	
	if (source[i] == '0')
		currNote.rest = true;
	else
		currNote.rest = false;
	return;
}

void WithinBrackets(const string source, int &i, vector<noteT> &notes) {
	vector<noteT> setOfNotes, within;
	i++; // To get past '(' char
	while (source[i] != ')') {
		if (source[i] == '(') {
			WithinBrackets(source, i, within);
			i++;
            
			for (size_t j = 0; j < within.size(); j++)
				setOfNotes.push_back(within[j]);
			
			within.clear();
		}
		
		if (source[i] == ')')
			break;
		
		noteT currNote;
		int increment = 1;
		currNote.value = 1.0 / timeSig;
		CalculateCurrentNote(currNote, source, i, increment);
		i += increment + 1;
        
		setOfNotes.push_back(currNote);
	}
	
	if (source[i+1] == '_' || source[i+1] == '-' || source[i] == '.') {
		int increment;
		for (size_t j = 0; j < setOfNotes.size(); j++) { // Slightly inefficient but more readable and no copied code.
			increment = 1;
			CalculateNoteValue(source, setOfNotes[j], i, increment);
			notes.push_back(setOfNotes[j]);
		}
		i += increment;
	} else if (source[i+1] == '<') {
		int dummy = i;
		CalculateNplet(dummy, source, setOfNotes, notes);
		i = dummy;
	} else {
		cout << "\nBad input: char \'" << source.substr(0, i+1) << "\' after \')\'.\nSIGABRT time! Wheeeeeeeeeeeeeeeeeeee";
		assert(false);
	}
}

bool BadBrackets(string &source) {
	int openSmooth = 0;
	int openAlligator = 0;
    
	for (size_t i = 0; i < source.length(); i++) {
		if (source[i] == ')')
			openSmooth--;
		if (source[i] == '(')
			openSmooth++;
		if (source[i] == '<')
			openAlligator++;
		if (source[i] == '>')
			openAlligator--;
	}
	return  openSmooth != 0 && openAlligator != 0;
}

void VectorizeNotes(vector<noteT> &notes, const string source) {
	for (size_t i = 0; i < source.length(); i++) {
		if (source[i] == ' ')
			return;
		
		if (source[i] != '(') {
			noteT currNote;
			int increment = 1;
			currNote.value = 1.0 / timeSig;
			CalculateCurrentNote(currNote, source, (int)i, increment);
			i += increment;
			notes.push_back(currNote);
		} else {
			int dummy = (int)i;
			WithinBrackets(source, dummy, notes);
			i = dummy;
		}
	}
	return;
}

bool VectorCount(const vector<double> &v, double n) {
	for (size_t i = 0; i < v.size(); i++)
		if (v[i] == n)
			return true;
	return false;
}

void RemoveSpaces(string &hand) {
	while (hand.find(" ") != -1) {
		for (size_t i = 0; i < hand.length(); i++) {
			if (hand[i] == ' ') {
				string left = hand.substr(0, i);
				string right = hand.substr(i+1, hand.size() - 1);
				hand = left + right;
			}
		}
	}
	return;
}

void ReadHand(vector<noteT> &notes, string &hand, const string whichHand) {
	cout << "Enter notes for the " << whichHand << " hand: ";
	while (true) {
		hand = GetLine();
		if (hand[hand.length()] != ' ')
			hand += " ";
		
		if (BadBrackets(hand))
			cout << "Unbalanced brackets! Try again: ";
		else
			break;
	}
	
	RemoveSpaces(hand);
	VectorizeNotes(notes, hand);
	return;
}

int FindNNotes(vector<noteT> notes) {
	vector<double> values;
	// Get rid of duplicate note values
	for (size_t i = 0; i < notes.size(); i++) {
		double value = 1.0/notes[i].value;
		if (!VectorCount(values, value)) {
			values.push_back(value);
		}
        
	}
	
	// Find LCM of all the note values
	for (size_t i = 0; i < values.size() - 1; i++) {
		values[i] = LCM(values[i], values[i+1]);
		values.erase(values.begin() + i +1);
		i--;
	}
	
	return values[0];
}

void DoHand(vector<noteT> &notes, string &hand) {
	hand = "";
	for (int i = 0; i < nSubBeats; i++)
		hand += "-";
	
	int numNotes = (int)notes.size();
	
	notes[0].position = 0;
	for (size_t i = 1; i < numNotes; i++) {
		notes[i].position = (notes[i-1].position + notes[i-1].value);
	}
	
	double sum = notes[numNotes-1].position + notes[numNotes-1].value;
	if (!CloseEnough(sum)) {
		cout << "\nOK, so, you didn't enter a full bar of notes. \nOr more than one. Try compiling and running again!\n";
		cout << "SIGABRT time!\n";
		assert(false);
		return;
	}
	
	for (size_t i = 0; i < numNotes; i++) {
		if (notes[i].rest == false) {
			int index = Round(notes[i].position*hand.length());
			hand[index] = 'o';
		} else {
			hand[notes[i].position*hand.length()] = '_';
			int howManyMore = notes[i].value*hand.length();
			for (int j = 1; j < howManyMore; j++) {
				hand[notes[i].position*hand.length()+j] = '_';
			}
		}
	}
	
	return;
}

void Welcome() {
	cout << "Hiya! Enter rhythms for both hands to see how they fit together.\n";
	cout << "First, some formatting and syntax info: \n";
	cout << "â€¢ Notes are assumed to be one beat long\n";
	cout << "â€¢ Select sets of notes with ( and ). For example: (oooo)\n";
	cout << "â€¢ Lengthen or shorten a note or set of notes with the following operators:\n	'-'(+1 beat)\n	'_'(/2)\n	'.'(*1.5)\n\n";
	cout << "â€¢ Make n-plets with alligator mouths; use them after a ')' char. An n-plet is n notes in the time of however many.\n";
	cout << "â€¢ For example: (ooooo)<2> is ooooo in the time of two beats. That is, five notes (ooooo) in the time of two notes of one beat each.\n";
	cout << "â€¢ You don't need to mark the notes within an n-plet as sixteenth notes or whatever -- just without any modifications will be fine.\n";
	cout << "â€¢ If notes within a n-plet are of different lengths, be sure to mark the first number within the alligator mouths as the number of beats within the n-plet.\n";
	cout << "â€¢ Finally, pretty much any character can signify a note, except for '_', '-', '.', '<', '>', '(', ')' and '0'. (0 signifies a rest).\n";
	cout << "Here's an example (In 3-4 time): \nRH is (ooo_)<2>o, LH is ooo. Generates the following: \no---o---o-o----\no----o----o----\n\n";
	return;
}

void Simpler() {
	cout << "Enter the number of notes played in the hand that has more notes to play: ";
	int more = GetInteger();
	cout << "Enter the number of notes played in the hand that has fewer to play: ";
	int fewer = GetInteger();
	
	string moreH = "";
	int totalLength = more * fewer;
	for (int i = 0; i < totalLength; i++)
		moreH+= "-";
	
	string fewerH = moreH;
	
	for (size_t i = 0; i < fewer; i++)
		fewerH[i*more] = 'o';
	
	for (size_t i = 0; i < more; i++)
		moreH[i*fewer] = 'o';
	
	cout << moreH << endl << fewerH << endl;
	return;
}

int main (int argc, char * const argv[]) {
	while (true) {
		cout << "Would you like to enter rhythms for both hands (enter '1') or uniform rhythms (enter '2')? ";
		string answer = GetLine();
		if (answer[0] == '2') {
			Simpler();
			return 0;
		} else if (answer[0] == '1')
			break;
	}
	
	vector<noteT> RHnotes, LHnotes;
	Welcome();
	
	while (true) {
		cout << "\nEnter the number on the top part of the time signature: ";
		timeSig = GetInteger();
		
		ReadHand(RHnotes, RH, "right");
		ReadHand(LHnotes, LH, "left");
		
		cout << "RH is " << RH << endl;
		cout << "LH is " << LH << endl;
		
		double nR = FindNNotes(RHnotes);
		cout << "The minimal number of beats to represent the right hand is " << nR << ".\n";
		double nL = FindNNotes(LHnotes);
		cout << "The minimal number of beats to represent the left hand is " << nL << ".\n";
		nSubBeats = LCM(nR, nL);
		cout << "The minimum total number of sub-beats is " << nSubBeats << ".\n";
		
		DoHand(RHnotes, RH);
		DoHand(LHnotes, LH);
		
		PrintfBeat(RH);
		cout << RH << endl;
		cout << LH << endl;
		
		cout << "Would you like to do another one? ";
		string response = GetLine();
		if (response[0] == 'n' || response[0] == 'N')
			break;
	}
	return 0;
}

