//
//  makesql.cpp
//
//  project makesql was cloned from project playingWithContainers-r2, which  was cloned from playingWithContainers-r1 on 2018-10-24T07:18.
//  Then, makesql.cpp was renamed from the cloned playingWithContainers-r2/main.cpp.
//  We need to create a new playingWithContainers-r2/main.cpp
//
//  Created by Clifford Campo on 2018-10-24.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//
#include "makesql.hpp"

// Why do we need this? vector<string> globalVector;

makesql::~makesql(void) {
    if (debug) clog << "Processing has just enetered the desctructor for this instance of class makesql. " \
        << "\nWill be deleting dynamically acquired storage, elapsedNS, at location: " << elapsedNS << endl;
    if(elapsedNS) {
        delete [] elapsedNS; //Delete the dynamically acquired storage.
    } else {
        clog << "elapsedNS was not dynamically allocated, nothing to delete." << endl;
    }
    if (debug) clog << "Have just deleted dynamically acquired storage, elapsedNS." \
        << "\n One less potential memory leak to worry about!" << endl;
}
makesql::makesql(const char *inputFileName, const char *outputFileName, const char debug_, int slots ) {
    
    numberOfVectorSlots = slots;
    
    if (debug_) {
        debug = true;
    } else {
        debug = false;
    }
    
    originalLength = inBuf.length();
    if (debug) cout << "0. inBuf looks like: " << inBuf << " and has a length of " << originalLength << " characters." <<  endl;
    cpyMBSS.erase();
    vecReplStrings.reserve(numberOfVectorSlots);
    vecReplStrings.clear(); //Remove junk from vecReplStrings.
    vecTokens.reserve(numberOfVectorSlots);
    vecTokens.clear(); //Make sure there are no junk characters in vecTokens.
    in_file.open(string(inputFileName), std::ifstream::in);
    //    std::ifstream infile(inputFileName, std::ifstream::in);
    if (in_file.is_open()) { //Above statement should have opened the file.
        std::getline(in_file, inBuf); //From the file whose name is provided by the sixth command line string, get the \
        prototype SQL we will work on and place it in location inBuf;
    } else {
        cerr << "Failed to open file " << string( inputFileName ) << endl;
        exit (-1); //Otherwise, quit.
    }
    originalLength = inBuf.length();
    if (debug) cout << "1. inBuf looks like: " << inBuf << " and now has a length of " << originalLength << " characters." <<  endl;
    cpyMBSS.reserve( growSpace + inBuf.capacity() ); //Reserve sufficient space for cpyMBSS
    cpyMBSS = {'\0'};
    cpyMBSS = string(inBuf); //Populate cpyMBSS with what we getlined into inBuf
    
    outString.reserve( growSpace + inBuf.capacity());
    outString = {'\0'};
    /*  Definition of what some regular expression should look like
     abc…    Letters
     123…    Digits
     \d    Any Digit
     \D    Any Non-digit character
     .    Any Character
     \.    Period
     [abc]    Only a, b, or c
     [^abc]    Not a, b, nor c
     [a-z]    Characters a to z
     [0-9]    Numbers 0 to 9
     \w    Any Alphanumeric character
     \W    Any Non-alphanumeric character
     {m}    m Repetitions
     {m,n}    m to n Repetitions
     *    Zero or more repetitions
     +    One or more repetitions
     ?    Optional character
     \s    Any Whitespace
     \S    Any Non-whitespace character
     ^…$    Starts and ends
     (…)    Capture Group
     (a(bc))    Capture Sub-group
     (.*)    Capture all
     (abc|def)    Matches abc or def
     Now use regular expressions, as defined above, to extract the Token/Replacemnet value found in each succeeding line of file input.
     Capture group 2 ----------+
     Capture group 1 ---+      |
     |      |
     m[1] --> V      V <-- m[2] */
    regX.assign("<(.*)><(.*)>"); //Note that we have two capture groups, m[1] and m[2]. Any character between the angle brackets is\
    fair game for a capture group.
    /*Capture group 3 ----------+
     Capture group 2 -----+    |
     Capture group 1 +    |    |
     |    |    |
     m[1] --> V    V    V <-- m[3] */
    rx.assign("<(.*)>(.*)<(.*)>"); //Initialize this regular expression; Another way to make this assignment. This works, too.
    StopWatch sWatch;
    while( getline(in_file, inBuf)  ) {
        sWatch.Restart();
        if (debug) cout << "inBuf looks like: " << inBuf << endl;
        regex_search(inBuf, m, rx ); // `m` returns that for which we were seaching.
        if (debug) {
            cout << "m[1].str()              :" << m[1].str() << endl;    //String of characters of first capture group
            cout << "m[1].length()           :" << m[1].length() << endl; //Length of first capture group.
            cout << "m[2].str()              :" << m[2].str() << endl;    //String of characters of the second capture group.
            cout << "m[2].length()           :" << m[2].length() << endl; //Length of the second capture group.
            cout << "m[3].str()              :" << m[3].str() << endl;    //String of characters of the third capture group.
            cout << "m[3].length()           :" << m[3].length() << endl; //Length of the third capture group.
        }
        token = m[1].str();
        replacement = m[3].str();
        vecTokens.push_back(token); //Insert the token we found into the vecTokens vector, named vecTokens
        
        vecReplStrings.push_back(replacement); //Insert the replacement string into the replacement string vector, named \
        vecReplStrings
        numberOfTokenReplacementPairs++; //Keep track of the number of different token/replacement pairs we need to process;
        if (debug) cout << "It took " << sWatch.ElapsedNs() << " ns to execute this code segment.\n" << endl;
    }
    in_file.close();
    regXresult.erase();
    elapsedNS = new size_t[1 + numberOfTokenReplacementPairs]; //Dynamically acquire storage for our array of stop-watch entries.
    swALL.Restart();
    for (i=0; i < vecReplStrings.size(); i++) {
        sw.Restart();
        outercounter++;
        innercounter++;
        if (debug) cout << i+1 << ". token " << vecTokens[i] << " is being replaced with " << vecReplStrings[i] << endl;
        regex_replace(back_inserter(regXresult), cpyMBSS.cbegin(), cpyMBSS.cend(), regex(vecTokens[i]), vecReplStrings[i] ); //Each time\
        regex_replace executes, it replaces all vecTokens of a particular token type with the replacement string. cpyMBSS was \
        initialized way back at the first getline.
        if (debug) {
            cout << outercounter << ".1" <<  "\t. regXresult NOW looks like: " << regXresult << endl;
            cout << outercounter << ".2" <<  "\t. cpyMBSS NOW looks like: " << cpyMBSS << endl;
        }
        cpyMBSS.erase();
        cpyMBSS = regXresult;
        regXresult.erase();
        elapsedNS[i] =  sw.ElapsedNs();
    } //End of for loop
    elapsedNS[vecReplStrings.size()] = swALL.ElapsedNs();
    
    for (auto _pos = vecTokens.cbegin(); _pos != vecTokens.cend(); ++_pos )  {
        const auto elem = *_pos;
        if (debug) cout << "Loop " << 1+j << " took " << elapsedNS[j] << " ns to do all the replacements of the token `" \
            << elem << "` with `" << vecReplStrings[j] << "` replacement string."  << endl;
        sumOfAll += elapsedNS[j] ;
        j++;
    }
    stringStreamForOutput << "\n--Time to do useful work in this makesql dylib: " << makesqlsw.ElapsedUs() \
    << " micro-seconds. Note that debug flag is set to: " << debug;
    if (debug) {
        cout << "It took " << elapsedNS[vecReplStrings.size()] << "|<=vs=>|" << sumOfAll << " ns to do all the insertions. (∆ = " \
        <<  elapsedNS[vecReplStrings.size()] - sumOfAll << " ns)" << endl;
        cout << "\nregXresult now looks like: " << regXresult;
        cout << "\ncpyMBSS now looks like: " << cpyMBSS;
        cout << "\nand is " << regXresult.length() << " bytes long.";
        cout << "\nThe original prototype SQL length was: " << originalLength;
        cout << "\nThe New SQL length is: " << cpyMBSS.length();
        cout << "\nWe grew in length by " << cpyMBSS.length()-originalLength << " characters (bytes).";
        cout << "\nThe name of the input file looks like: " << inputFileName;
        cout << "\nThe name of the output file looks like: " << outputFileName;
        cout << "\nThe number of vector slots is: " << slots << endl;
    }
    if (outputFileName) { //Were we supplied with the name of an output file?
        outFile.open( outputFileName, std::ofstream::out);
        
        outFile << cpyMBSS << endl;
        outFile << "\n--regXresult now looks like: " << regXresult;
        outFile << "\n--cpyMBSS now looks like: " << cpyMBSS;
        outFile << ", and is " << cpyMBSS.length() << " bytes long.";
        outFile << "\n--original prototype length: " << originalLength << " characters.";
        outFile << "\n--new length of executable SQL is " << cpyMBSS.length() << " characters.";
        outFile << "\n--The prototype SQL grew in length by " << cpyMBSS.length() - originalLength << " characters (bytes) to become the executable SQL.";
        outFile << stringStreamForOutput.str() << endl;
        outFile.close();
    }
}
const char *makesql::getsql() {
    return(cpyMBSS.c_str()); //Return to the caller, as a c_string, the executable SQL that we created from the prototype sql.
}
