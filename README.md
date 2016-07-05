# dictate

Dictate is a tool for generating wordlists for dictionary attacks based on information about a specific target individual.

syntax:
```
dictate [-h] [-q | -v] [-l] [-o filename] [-e] [ [-d date1,date2...daten] [-n name1,name2...namen] [-w word1,word2...wordn] | [-f filename] ]
```

options:
    -h : you're already here!
    -o : specify an output filename (default dictionary.txt)
    -q : quiet. suppress nonerror output. overrides -d
    -v : verbose. give more informative output
    -l : 13375P34|< mode. By default dictate adds versions of words with o and i swapped
         for 0 and 1. If you know your target suffers from a propensity to use more
         elaborate substitutions, this switch adds them
    -e : add numbers on the end of generated passwords
    -d : supply a comma seperated list of dates relevant to the target, without spaces,
         in the form ddmmyyyy or mmddyyyy (however the target would write it)
    -n : supply a comma seperated list of names relevant to the target, including theirs,
         without spaces, in the form firstname_lastname
    -w : supply a comma seperated list of words relevant to the target
examples:
    for john smith, an accountant:
        `dictate -o john.txt -e -n john_smith,jane_smith -w money,rich,win`
        note that the repetition of 'smith' doesn't matter because duplicates are removed
