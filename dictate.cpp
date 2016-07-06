//////////////////////////////////////////////////////////
//                                                      //
//  Dictate - a utility for generating password lists   //
//            from information about a target person.   //
//  No Rights Reserved                                  //
//                                                      //
//  https://xkcd.com/1513                               //
//                                                      //
//////////////////////////////////////////////////////////

#define VER "1"

#include <algorithm>
#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>
#include <locale>
#include <vector>

using namespace std;

// <initialise functions>
int help(char* argv[]), go(vector<string> words, vector<string> dates, vector<string> names), write(vector<string> info), add(vector<string> info), permute(vector<string> allT);
vector<string> tovect(string str), extras(vector<string> allT), ifnotadd(string str, vector<string> perms);
string l33tize(string str);
bool l33table (string str), endnos = false;
// </initialise>
char* ofname = "dictionary.txt"; // name of output file
ofstream fout; // output stream
char vmode = 'n'; // verbosity mode. n = not set, v = verbose, q = quiet.
bool l33t = false;
int startlen = 5, endlen = 14;

int main(int argc, char* argv[]) {
  if (argc == 1) {
    cout << "usage: " << argv[0] << " [-h] [-V] [-q | -v] [-l] [-c a,b] [-o filename] [-d date1,date2] [-n name1,name2] [-w word1,word2]\n" << argv[0] << " -h for further information\n";
    return -1;
  }

  // <banner>
  if (vmode != 'q') {
    cout << "                 \\\\===========\/\/\n";
    cout << "__________________\\\\ DICTATE \/\/_________________\n";
    cout << "\\\\ generate personalised password dictionaries \/\/\n";
    cout << " \\\\===========================================\/\/\n\n";
  }
  // </banner>

  vector<string> sargv, words, dates, names;

  for (int i = 0; i < argc; i++) { // load argv into a vector<string>
    sargv.push_back(argv[i]);
  }

  // <options>
  for (int i = 1; i < argc; i++) {
    if (sargv.at(i) == "-h") {
      help(argv);
    } else if (sargv.at(i) == "-d") {
      dates = tovect(sargv.at(++i));
    } else if (sargv.at(i) == "-n") {
      names = tovect(sargv.at(++i));
    } else if (sargv.at(i) == "-w") {
      words = tovect(sargv.at(++i));
    } else if (sargv.at(i) == "-o") {
      ofname = strdup(sargv.at(++i).c_str());
    } else if (sargv.at(i) == "-v") {
      if (vmode != 'q') {
        vmode = 'v';
      }
    } else if (sargv.at(i) == "-q") {
      vmode = 'q';
    } else if (sargv.at(i) == "-l") {
      l33t = true;
    } else if (sargv.at(i) == "-e") {
      endnos = true;
    } else if (sargv.at(i) == "-V") {
      cout << "Dictate version " << VER << "\n";
      exit(0);
    } else if (sargv.at(i) == "-c") {
      istringstream issa(tovect(sargv.at(++i)).at(0));
      int a;
      issa >> startlen;
      istringstream issb(tovect(sargv.at(i)).at(1));
      int b;
      issb >> endlen;
    }
  }
  // </options>
  fout.open(ofname); // now we know what the output filename will be, no point hanging around

  // <extra info>
  if (vmode == 'v') {
    cout << "\nwords: ";
    for (int i = 0; i < words.size(); i++) {
      cout << words.at(i) << " ";
    }
    cout << "\ndates: ";
    for (int i = 0; i < dates.size(); i++) {
      cout << dates.at(i) << " ";
    }
    cout << "\nnames: ";
    for (int i = 0; i < names.size(); i++) {
      cout << names.at(i) << " ";
    }
    cout << "\n\n";
  }
  // </extra info>
  go(words,dates,names); // let's go!

  fout.close(); // tidying up
  return 0;
}

int help(char* argv[]) {
  cout << "Dictate is a tool for generating wordlists for dictionary attacks based on information\n";
  cout << "about a specific target individual.\n";
  cout << "syntax: " << argv[0] << " [-h] [-V] [-q | -v] [-l] [-c a,b] [-o filename] [-e] [-d date1,date2...daten]\n";
  cout << "[-n name1,name2...namen] [-w word1,word2...wordn]\n\n";
  cout << "options:\n";
  cout << "    -h : you're already here!\n";
  cout << "    -V : output the program version\n";
  cout << "    -q : quiet. suppress nonerror output. overrides -d\n";
  cout << "    -v : verbose. give more informative output\n";
  cout << "    -l : 13375P34|< mode. By default dictate adds versions of words with o and i swapped\n";
  cout << "         for 0 and 1. If you know your target suffers from a propensity to use more\n";
  cout << "         elaborate substitutions, this switch adds them\n";
  cout << "    -c : constrain the permutations to words between integer length a and b. by default it's\n";
  cout << "         between 5 and 14. anything much broader than that and it starts to get really slow.\n";
  cout << "    -e : add numbers on the end of generated passwords\n";
  cout << "    -o : specify an output filename (default dictionary.txt)\n";
  cout << "    -d : supply a comma seperated list of dates relevant to the target, without spaces,\n";
  cout << "         in the form ddmmyyyy or mmddyyyy (however the target would write it)\n";
  cout << "    -n : supply a comma seperated list of names relevant to the target, including theirs,\n";
  cout << "         without spaces, in the form firstname_lastname\n";
  cout << "    -w : supply a comma seperated list of words relevant to the target\n";
//  cout << "    -f : instead of -d, -n and -w, supply the path to a file containing the information.\n";
//  cout << "         they should be listed one word to a line, with headers \"[words]\",\"[names]\",\n";
//  cout << "         and \"[dates]\". not yet implemented.\n";
//  cout << "    -t : only do transforms, not permutations. this is primarily useful in combination with\n";
//  cout << "         -f to work with the contents of an existing wordlist. not yet implemented.\n\n";
  cout << "examples:\n";
  cout << "    for john smith, an accountant:\n";
  cout << "        " << argv[0] << " -o john.txt -e -n john_smith,jane_smith -w money,rich,win\n";
  cout << "        note that the repetition of 'smith' doesn't matter because duplicates are removed\n";
//  cout << "    modify the rockyou wordlist, suppressing nonerror output:\n";
//  cout << "        " << argv[0] << " -o newrockyou.txt -t -w rockyou.txt -q\n";
//  cout << "        you'd first have to insert '[words]' at the top of rockyou.txt\n\n";
  exit(0);
  return 0;
}

vector<string> tovect(string str) {
  vector<string> r;
  r.resize(1);
  stringstream ss(str);
  char c;
  int i = 0;
  while (ss >> c) {
    if (c != ',') {
      r.at(i) += c;
    } else {
      i++;
      r.resize(i+2); //keep r at i elements
    }
  }
  return r;
}

int go(vector<string> words, vector<string> dates, vector<string> names) {
  vector<string> wordsT, datesT, namesT, allT;
  // <apply transforms to words>
  if (vmode == 'n') {
    cout << "generating wordlist...\n";
  } else if (vmode == 'v') {
    cout << "applying transforms to words...\n";
  }
  for (int i = 0; i < words.size(); i++) {  // no transform
    wordsT.resize(i+2);
    wordsT.at(i) = words.at(i);
  }
  for (int i = 0; i < words.size(); i++) {  // capitalise first letter
    wordsT.resize(wordsT.size()+1);
    words.at(i)[0] = toupper(words.at(i)[0]);
    wordsT.at(wordsT.size()-1) = words.at(i);
  }
  for (int i = 0; i < words.size(); i++) {  // reverse and capitalised
    wordsT.resize(wordsT.size()+1);
    reverse(words.at(i).begin(),words.at(i).end());
    wordsT.at(wordsT.size()-1) = words.at(i);
  }
  for (int i = 0; i < words.size(); i++) {  // reverse and not capitalised
    wordsT.resize(wordsT.size()+1);
    words.at(i)[words.at(i).length()-1] = tolower(words.at(i)[words.at(i).length()-1]);
    wordsT.at(wordsT.size()-1) = words.at(i);
  }
  // </apply transforms to words>

  // <apply transforms to dates>
  if (dates.size() > 0) {
    if (vmode == 'v') {
      cout << "applying transforms to dates...\n";
    }
    for (int i = 0; i < dates.size(); i++) {  // no transform
      datesT.resize(i+2);
      datesT.at(i) = dates.at(i);
    }
    for (int i = 0; i < dates.size()-1; i++) { // just the year. why -1? I don't know, but otherwise it doesn't work.
      datesT.resize(datesT.size() + 1);
      datesT.at(datesT.size() - 1) = dates.at(i).substr(4);
    }
    for (int i = 0; i < dates.size(); i++) { // all but the year
      datesT.resize(datesT.size() + 1);
      datesT.at(datesT.size() - 1) = dates.at(i).substr(0,4);
    }
    for (int i = 0; i < dates.size(); i++) { // just the day/month (depending on how the date's written)
      datesT.resize(datesT.size() + 1);
      datesT.at(datesT.size() - 1) = dates.at(i).substr(0,2);
    }
    for (int i = 0; i < dates.size() - 1; i++) { // just the month/day (depending on how the date's written)
      datesT.resize(datesT.size() + 1);
      datesT.at(datesT.size() - 1) = dates.at(i).substr(2,2);
    }
    for (int i = 0; i < dates.size(); i++) { // year backwards
      datesT.resize(datesT.size() + 1);
      reverse(dates.at(i).begin(), dates.at(i).end());
      datesT.at(datesT.size() - 1) = dates.at(i).substr(0,4);
    }
  }
  // </apply transforms to dates>

  // <apply transforms to names>
  if (names.size() > 0) {
    if (vmode == 'v') {
      cout << "applying transforms to names...\n";
    }
    for (int i = 0; i < names.size(); i++) { // first name
      namesT.resize(i+2);
      namesT.at(i) = names.at(i).substr(0,names.at(i).find('_'));
    }
    for (int i = 0; i < names.size(); i++) { // last name
      namesT.resize(namesT.size() + 1);
      namesT.at(namesT.size() - 1) = names.at(i).substr(names.at(i).find('_')+1);
    }
    for (int i = 0; i < names.size(); i++) { // first name capitalised
      namesT.resize(namesT.size() + 1);
      names.at(i)[0] = toupper(names.at(i)[0]);
      names.at(i)[names.at(i).find('_')+1] = toupper(names.at(i)[names.at(i).find('_')+1]);
      namesT.at(namesT.size() - 1) = names.at(i).substr(0,names.at(i).find('_'));
    }
    for (int i = 0; i < names.size()-1; i++) { // last name capitalised
      namesT.resize(namesT.size() + 1);
      namesT.at(namesT.size() - 1) = names.at(i).substr(names.at(i).find('_')+1);
    }
    for (int i = 0; i < names.size(); i++) { // first name backwards capitalised
      namesT.resize(namesT.size() + 1);
      reverse(names.at(i).begin(),names.at(i).end());
      namesT.at(namesT.size() - 1) = names.at(i).substr(names.at(i).find('_')+1);
    }
    for (int i = 0; i < names.size(); i++) { // last name backwards capitalised
      namesT.resize(namesT.size() + 1);
      namesT.at(namesT.size() - 1) = names.at(i).substr(0,names.at(i).find('_'));
    }
    for (int i = 0; i < names.size(); i++) { // first initial capitalised
      namesT.resize(namesT.size() + 1); // no, I don't know why this doesn't go inside the if statement. probably the same reason that the initials come out in reverse order.
      if (names.at(i).size() > 0) {
        namesT.at(namesT.size() - 1) = names.at(i).substr(names.at(i).size()-1,1);
      }
    }
    for (int i = 0; i < names.size(); i++) { // last initial capitalised
      namesT.resize(namesT.size() + 1);
      if (names.at(i).size() > 0) {
        namesT.at(namesT.size() - 1) = names.at(i).substr(names.at(i).find('_')-1,1);
      }
    }
    for (int i = 0; i < names.size(); i++) { // first name backwards
      namesT.resize(namesT.size() + 1);
      names.at(i)[names.at(i).size()-1] = tolower(names.at(i)[names.at(i).size()-1]);
      names.at(i)[names.at(i).find('_')-1] = tolower(names.at(i)[names.at(i).find('_')-1]);
      namesT.at(namesT.size() - 1) = names.at(i).substr(names.at(i).find('_')+1);
    }
    for (int i = 0; i < names.size(); i++) { // last name backwards
      namesT.resize(namesT.size() + 1);
      namesT.at(namesT.size() - 1) = names.at(i).substr(0,names.at(i).find('_'));
    }
  }
  // </apply transforms to names>
  // <add to allT>
  for (int i = 0; i < wordsT.size(); i++) {
    allT.resize(allT.size() + 1);
    allT.at(allT.size() - 1) = wordsT.at(i);
  }
  for (int i = 0; i < datesT.size(); i++) {
    allT.resize(allT.size() + 1);
    allT.at(allT.size() - 1) = datesT.at(i);
  }
  for (int i = 0; i < namesT.size(); i++) {
    allT.resize(allT.size() + 1);
    allT.at(allT.size() - 1) = namesT.at(i);
  }
  //    <add varying levels of 13375P34k>
  if (vmode == 'v') {
    cout << "adding letter substitutions (l33tmode = ";
    if (l33t) {
      cout << "1337";
    } else {
      cout << "standard";
    }
    cout << ")...\n";
  }
  for (int i = 0; i < allT.size(); i++) {
    if (l33table(allT.at(i))) {
      allT.resize(allT.size() + 1);
      allT.at(allT.size() - 1) = l33tize(allT.at(i));
    }
  }
  //    </add varying levels of l3375P34k>
  //    <remove duplicates>
  if (vmode == 'v') {
    cout << "removing duplicates...\n";
  }
  for (int i = 0; i < allT.size(); i++) {
    for (int j = 0; j < allT.size(); j++) {
      if (allT.at(i) == allT.at(j) && i != j) {
        for (int k = j; k < allT.size() - 1; k++) {
          allT.at(k) = allT.at(k+1);
        }
        allT.resize(allT.size() - 1);
      }
    }
  }
  //    </remove duplicates>
  // </add to allT>
  write(allT); // add these to the wordlist file
  permute(allT); // generate combinations
  return 0;
}

int permute(vector<string> allT) {
  vector<string> perms;
  if (vmode != 'q') {
    cout << "generating permutations";
    if (vmode == 'v') {
      cout << " on " << allT.size() << " words";
    }
    cout << "...\n";
  }
  // <find all two and three part permutations of the transforms (this works because one of the values in allT is always "")>
  int ib = 0, jb = 0, kb = 0;
  for (int i = 0; i < allT.size(); i++) {
    for (int j = 0; j < allT.size(); j++) {
      for (int k = 0; k < allT.size(); k++) {
          perms = ifnotadd(allT.at(i) + allT.at(j) + allT.at(k), perms);
          perms = ifnotadd(allT.at(i) + allT.at(k) + allT.at(j), perms);
          perms = ifnotadd(allT.at(j) + allT.at(i) + allT.at(k), perms);
          perms = ifnotadd(allT.at(j) + allT.at(k) + allT.at(i), perms);
          perms = ifnotadd(allT.at(k) + allT.at(i) + allT.at(j), perms);
          perms = ifnotadd(allT.at(k) + allT.at(j) + allT.at(i), perms);
      }
    }
    if (vmode == 'v') {
      cout << "[" << ((i+1)*100)/allT.size() << "%]\n";
    }
    if (i == allT.size()/2) {
      if (vmode == 'n') {
        cout << "halfway there...\n"; // so they know something's happening if it's slow
      }
    }
  }
  // </permutations>
  // <add numbers on the end>
  if (endnos) {
    if (vmode == 'v') {
      cout << "adding number on the end...\n";
    }
    int unnummedperms = perms.size();
    for (int i = 0; i < 102; i++) {
      for (int j = 0; j < unnummedperms; j++) {
        perms.resize(perms.size() + 1);
        string isc = static_cast<ostringstream*>(&(ostringstream() << i))->str();
        if (i < 10) {
          perms.at(perms.size() - 1) = perms.at(j) + "0" + isc;
        } else {
          perms.at(perms.size() - 1) = perms.at(j) + isc;
        }
      }
    }
  }
  // </add numbers on the end>
  if (vmode == 'v') {
    cout << "generated " << perms.size() << " permutations.\n";
  }
  write(perms); // add the results to the wordlist file
  if (vmode != 'q') {
    cout << "wrote " << perms.size() << " words to " << ofname << ".\n";
  }
  return 0;
}

vector<string> ifnotadd(string str, vector<string> perms) {
  if (find(perms.begin(), perms.end(), str) == perms.end() && str.length() >= startlen && str.length() <= endlen) {
    perms.resize(perms.size() + 1);
    perms.at(perms.size() - 1) = str;
  }
  return perms;
}

bool l33table(string str) {
  if (l33t) {
    for (int i = 0; i < str.length(); i++) {  // TODO: support for custom character substitutionss
      if (str.at(i) == 'o') {
        return true;
      } else if (str.at(i) == 'i') {
        return true;
      } else if (str.at(i) == 'e') {
        return true;
      } else if (str.at(i) == 's') {
        return true;
      } else if (str.at(i) == 't') {
        return true;
      } else if (str.at(i) == 'a') {
        return true;
      } else if (str.at(i) == 'g') {
        return true;
      } else if (str.at(i) == 'z') {
        return true;
      }
    }
  } else {
    for (int i = 0; i < str.length(); i++) {
      if (str.at(i) == 'o') {
        return true;
      } else if (str.at(i) == 'i') {
        return true;
      }
    }
  }
  return false;
}

string l33tize(string str) {
  if (l33t) {
    for (int i = 0; i < str.length(); i++) {
      if (str.at(i) == 'o') {
        str.at(i) = '0';
      } else if (str.at(i) == 'i') {
        str.at(i) = '1';
      } else if (str.at(i) == 'e') {
        str.at(i) = '3';
      } else if (str.at(i) == 's') {
        str.at(i) = '5';
      } else if (str.at(i) == 't') {
        str.at(i) = '7';
      } else if (str.at(i) == 'a') {
        str.at(i) = '4';
      } else if (str.at(i) == 'g') {
        str.at(i) = '9';
      } else if (str.at(i) == 'z') {
        str.at(i) = '2';
      }
    }
  } else {
    for (int i = 0; i < str.length(); i++) {
      if (str.at(i) == 'o') {
        str.at(i) = '0';
      } else if (str.at(i) == 'i') {
        str.at(i) = '1';
      }
    }
  }
  return str;
}

int write(vector<string> info) {
  for (int i = 0; i < info.size(); i++) {
    if (info.at(i) != "") {
      fout << info.at(i) << "\n";
    }
  }
  return 0;
}
