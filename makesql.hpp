//
//  makesql.hpp
//  playingWithContainers-r2
//
//  Created by Clifford Campo on 10/24/18.
//  This header, makesql.hpp, introduces the makesql class.
//  Copyright © 2018 CliffordCampo. All rights reserved.
//

#ifndef makesql_hpp
#define makesql_hpp
#include <iostream>
#include <sstream>
#include <fstream> //For doing file i/o
#include <string>
#include <vector>
#include <regex>
#include <iterator>
#include "vector.hpp"
#include "StopWatch.hpp"
using namespace std;
class makesql {
private:
    const string myBigSelectStatement = "SELECT * FROM (SELECT * FROM tbl_sun_loc_site WHERE id BETWEEN ((SELECT a.id FROM (SELECT id FROM tbl_sun_loc_site WHERE     siteid=:SS AND lt::DATE=':DD' AND events IS NULL AND :FF = (SELECT max(:FF) FROM tbl_sun_loc_site where lt::DATE=':DD' AND siteid=:SS)    ORDER BY lt) a, (SELECT id FROM tbl_sun_loc_site WHERE     siteid=:SS AND lt::DATE=':DD' AND events IS NULL AND :FF = (SELECT min(:FF) FROM tbl_sun_loc_site where lt::DATE=':DD' AND siteid=:SS)    ORDER BY lt) b  where b.id-a.id=1) ) - :NN AND ((SELECT a.id FROM (SELECT id FROM tbl_sun_loc_site WHERE     siteid=:SS AND lt::DATE=':DD' AND events IS NULL AND :FF = (SELECT max(:FF) FROM tbl_sun_loc_site where lt::DATE=':DD' AND events IS NULL AND siteid=:SS)    ORDER BY lt) a, (SELECT id FROM tbl_sun_loc_site WHERE     siteid=:SS AND lt::DATE=':DD' AND     events IS NULL AND :FF = (SELECT min(:FF) FROM tbl_sun_loc_site where lt::DATE=':DD' AND events IS NULL AND siteid=:SS)    ORDER BY lt) b  where b.id-a.id=1) ) + :NN) c WHERE c.events IS NULL ORDER BY c.id;";
    const size_t growSpace = 126;
    const size_t outfilePosition = 2;
    const size_t infilePosition = 3;
    const size_t numberOfVectorSlotsPosition = 1;
    std::ifstream in_file;
    std::ofstream outFile;
    size_t numberOfVectorSlots;
    size_t numberOfTokenReplacementPairs=0;
    bool debug;
public:
    ostringstream stringStreamForOutput; 
    vector<std::string> vecReplStrings;
    std::vector<string> vecTokens;
    string inBuf;
    vector<std::regex> vecRegXpat;
    smatch m;
    size_t originalLength;
    size_t outercounter=0;
    size_t innercounter = 0;
    string::const_iterator pos;
    string cpyMBSS;                    //Declare cpyMBSS
    string outString;
    string token;
    string replacement;
    string regXresult;
    StopWatch swALL;
    StopWatch sw;
    StopWatch makesqlsw;
    regex rx;
    regex regX;
    size_t *elapsedNS=nullptr;
    int i=0;
    int j=0;
    size_t sumOfAll=0;

    ~makesql(void);
    makesql(int, char **);
    makesql(const char *, const char *, const char *, int=5);
    makesql(const char *, const char *, const char='\0', int=5);
    makesql(const char *, const char *, int=5);
    const char *getsql(void); //Used to return the executable SQL statement to the calling program
};

#endif /* makesql_hpp */
