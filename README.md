# dictate

Dictate is a tool for generating wordlists for dictionary attacks based on information about a specific target individual - somewhat like CUPP.  
- takes **words**, **names** and **dates** associated with the target  
- applies various transforms to them - such as taking the year of a date backwards or the initials of a name  
- goes through the resulting list adding versions with letters replaced with numbers  
- generates all possible two- and three-part permutations on that list  
- optionally, adds numbers on the end  

---

##### syntax:
```
dictate [-h] [-q | -v] [-l] [-t] [-T n] [-o filename] [-e] [-d date1,date2...daten] [-n name1,name2...namen] [-w word1,word2...wordn] [-f filename]
```
---
###### example usage:  
for john smith, an accountant:  
`dictate -o john.txt -e -n john_smith,jane_smith -w money,rich,win`  
note that the repetition of 'smith' doesn't matter because duplicates are removed  

modify the rockyou wordlist, using four threads and suppressing nonerror output:

`./dictate -T 4 -o newrockyou.txt -t -f rockyou.txt -q`

---
###### installation:
Dictate uses only the c++ standard libraries with the C++11 standard.  
Download the zip file or `git clone https://github.com/silver-ag/dictate.git dictate`. You can either run `make` and `sudo make install` in the source directory, or compile it yourself with your compiler of choice - it's only one file.
