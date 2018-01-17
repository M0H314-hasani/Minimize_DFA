#include<iostream>
#include<string>
#include<map>
#include <fstream>
using namespace std;
struct Alphabet{
	int size = 0;
	string* letter;
	void setendletter_num(int n) {
		letter = new string[n];
	}

};
class DFA
{
public:
	friend string getWord(string&);
	friend int convertStringToInt(string);
	DFA DFAminimization();
	void readFromFile(string);
	void writeToFile(string);
	map<string, map<string, string>> DFA_map;
	
private:
	Alphabet alphabet;
	int numOfStates = 0;
	int numOfEndStates = 0;
	void setendstates_num(int n) {
		endStates = new string[n];
	}
	string* endStates ;
	int numOfTransitions = 0;
	

	DFA getInfoFromDfaMap(map<string, map<string, string>>);
	bool checkEndStateTrigger(string ii) {
		for (int i = 0; i < numOfEndStates; i++)
			if (ii == endStates[i])
				return true;
		return false;
	}
	bool approvePair(string ,string);

};
bool DFA::approvePair(string  i, string  ii) {
	bool result = false;
	for (int j = 0; j < alphabet.size; j++) {
		string it = alphabet.letter[j];
		if (DFA_map[i][it] == DFA_map[ii][it] ||
			((DFA_map[i][it] == i || DFA_map[i][it] == ii) && (DFA_map[ii][it] == i || DFA_map[ii][it] == ii))
			)
		{
			result = true;
			continue;
		}
		else if ((checkEndStateTrigger(DFA_map[ii][it]) && checkEndStateTrigger(DFA_map[i][it]) == true) || (!checkEndStateTrigger(DFA_map[ii][it]) && checkEndStateTrigger(DFA_map[i][it]) == false)) {
			if (approvePair(DFA_map[i][it], DFA_map[ii][it])) {
				result = true;
				continue;
			}
			else {
				result = false;
				break;
			}
		}
		else
		{
			result = false;
			break;
		}
	}
	return result;
}
DFA DFA::DFAminimization() {
	map<string, map<string, string>> minimum_DFA_map;
	map<int,string> pairs_approve; int pairs_approve_i = 0; bool firstTime_check = true;

			for (auto & i : DFA_map) {
				for (auto & ii : DFA_map) {
					if (ii <= i)
						continue;
					if (checkEndStateTrigger(ii.first) && !checkEndStateTrigger(i.first))
						continue;
					else if (!checkEndStateTrigger(ii.first) && checkEndStateTrigger(i.first))
						continue;

					if (approvePair(i.first,ii.first)) {
						for (auto  t : pairs_approve) {
							if (pairs_approve[t.first].find(i.first) != string::npos && pairs_approve[t.first].find(ii.first) != string::npos) {
								break;
							}
							else if (pairs_approve[t.first].find(i.first) != string::npos) {
								pairs_approve[t.first] += "," + ii.first; break;
							}
							else if (pairs_approve[t.first].find(ii.first) != string::npos) {
								pairs_approve[t.first] += "," + i.first; break;
							}
							else
							{
								pairs_approve[pairs_approve_i] = i.first + "," + ii.first;
								pairs_approve_i++;
								break;
							}
						}

						if (firstTime_check) {
							pairs_approve[pairs_approve_i] = i.first + "," + ii.first;
							pairs_approve_i++;
							firstTime_check = false;
						}
					}
				}
			}
			for (auto & i : DFA_map) {
				bool exist_check = false;
				for (auto & j : pairs_approve) {
					if (pairs_approve[j.first].find(i.first) != string::npos) {
						exist_check = true;
						break;
					}
				}
				if (!exist_check) {
					pairs_approve[pairs_approve_i] = i.first;
					pairs_approve_i++;
				}
			}
			//////////////////////
			for (auto DFA_map_scroller : DFA_map) {
				for (auto pairs_approve_scroller : pairs_approve) {
					if (pairs_approve[pairs_approve_scroller.first].find(DFA_map_scroller.first) != string::npos) {
						for (int alphabet_scroller = 0; alphabet_scroller < alphabet.size; alphabet_scroller++) {
							for (auto pairs_approve_scroller2 : pairs_approve) {
								if (pairs_approve[pairs_approve_scroller2.first].find(DFA_map[DFA_map_scroller.first][alphabet.letter[alphabet_scroller]]) != string::npos) {
									minimum_DFA_map[pairs_approve_scroller.second][alphabet.letter[alphabet_scroller]] = pairs_approve[pairs_approve_scroller2.first];
								}
							}
						}
					}
				}
			}
			//////////////////////
			return getInfoFromDfaMap(minimum_DFA_map);
		}
DFA DFA::getInfoFromDfaMap(map<string, map<string, string>> minimum_DFA_map)  {
	DFA minimum_DFA;
	//
		map<string,map<string,string>>::iterator minimum_DFA_map_scroller = minimum_DFA_map.begin();
		minimum_DFA.alphabet.size = minimum_DFA_map[minimum_DFA_map_scroller->first].size();
		minimum_DFA.alphabet.setendletter_num(minimum_DFA.alphabet.size);
		int i = 0;
		for (auto minimum_DFA_map_scroller_j : minimum_DFA_map_scroller->second) {
			minimum_DFA.alphabet.letter[i] = minimum_DFA_map_scroller_j.first;
			i++;
		}
	//
	minimum_DFA.numOfStates = minimum_DFA_map.size();
	//
	for (auto minimum_DFA_map_scroller : minimum_DFA_map) {
		for (int i = 0; i < numOfEndStates; i++) {
			if (minimum_DFA_map_scroller.first.find(endStates[i]) != string::npos) {
				minimum_DFA.numOfEndStates++;
				break;
			}
		}

	}
	minimum_DFA.setendstates_num(minimum_DFA.numOfEndStates);
	int endStates_counter = 0;
	for (auto minimum_DFA_map_scroller : minimum_DFA_map) {
		for (int i = 0; i < numOfEndStates; i++) {
			if (minimum_DFA_map_scroller.first.find(endStates[i]) != string::npos) {
				minimum_DFA.endStates[endStates_counter] = minimum_DFA_map_scroller.first;
				endStates_counter++;
				break;
			}
		}

	}
	//
	minimum_DFA.numOfTransitions = minimum_DFA.alphabet.size * minimum_DFA.numOfStates;
	//
	minimum_DFA.DFA_map = minimum_DFA_map;
	//
	return minimum_DFA;
}
void DFA::readFromFile(string filename) {
	string line;
	ifstream File(filename);
	if (!File) {
		cout << "Cannot open input file.\n";
	}
	if (File) {

		getline(File, line);//line 1
		alphabet.size = convertStringToInt(getWord(line));
		alphabet.setendletter_num(alphabet.size);
		for (int i = 0; i < alphabet.size; i++) {
			alphabet.letter[i] = getWord(line);
		}

		getline(File, line);//line 2
		numOfStates = convertStringToInt(getWord(line));

		getline(File, line);//line 3
		numOfEndStates = convertStringToInt(getWord(line));
		setendstates_num(numOfEndStates);
		for (int i = 0; i < numOfEndStates; i++) {
			endStates[i] = getWord(line);
		}

		getline(File, line);//line 4
		numOfTransitions = convertStringToInt(getWord(line));
		while (File) {
			getline(File, line);
			if (line == "$") break;
			string i1 = getWord(line);
			string j1 = getWord(line);
			string ij = getWord(line);
			DFA_map[i1][j1] = ij;
		}
	}
	File.close();
}
void DFA::writeToFile(string filename) {
	ofstream File(filename);
	// line 1
	File<< alphabet.size;
	for (int i = 0; i < alphabet.size; i++) 
		File <<" "<<alphabet.letter[i];
	File << endl;
	// line 2
	File << numOfStates;
	File << endl;
	// line 3
	File << numOfEndStates;
	for (int i = 0; i < numOfEndStates; i++)
		File <<" "<< endStates[i];
	File << endl;
	// line 4
	File << numOfTransitions;
	File << endl;
	// line 5 -> $
	for (auto DFA_map_scroller : DFA_map) {
		for (int j = 0; j < alphabet.size; j++) {
			File << DFA_map_scroller.first << " " << alphabet.letter[j] << " " << DFA_map[DFA_map_scroller.first][alphabet.letter[j]]<<endl;
		}
	}
	File << "$";
	File.close();
}
string getWord(string& line) {
	string word;
	if(line != "") {
		word =  line.substr(0, (line.find(' ') != string::npos) ? line.find(' ') : line.size());
		line.replace(0, (line.find(' ') != string::npos) ? line.find(' ') + 1 : line.size(), "");
	}
	return  word;
}
int convertStringToInt(string str) {
	int num = 0;
	for (int i=0; i < str.size(); i++) {
		num += (int(str[i]) - 48) *(pow(10,(str.size() - i - 1)));
	}
	return num; 
}
	
void main(int argc ,char **argv) {
	DFA dfa;
	if (argc >= 2) {
		dfa.readFromFile(argv[1]);
		dfa.DFAminimization().writeToFile(argv[2]);
	}
	}