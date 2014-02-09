//This is a code from project daedalus
#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdio.h>
#include <vector>
#include <boost/algorithm/string.hpp>
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
#include <boost/lexical_cast.hpp>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

using namespace std;

Configuration::Configuration(string inputFile){
    string line;
	string buffer = "" ;
    ifstream myfile (inputFile.c_str());
    if (myfile.is_open()){
        while ( myfile.good() ){
            getline (myfile,line);
            if(line.c_str()[0] =='#'){} //ignoring comments
            if(line.size()>0){
				buffer += line ;
                vector<string> tokens;
				boost::split(tokens, buffer, boost::is_any_of("="));
				istringstream iss(buffer);
				copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(tokens));
				if(tokens.size()>0){
					setParameter(tokens[0], tokens[1]);
				}
				buffer="";
			}
        }
        myfile.close();
    }else{ //  if (myfile.is_open())
        
        char cCurrentPath[FILENAME_MAX];
        GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
    	cout << "[Configuration] Configuration file " <<  cCurrentPath << "\\"  << inputFile << " not found." << endl ;
    }
}

void Configuration::getParameter(string parameter, string* result){
	*result = db[parameter];
}

void Configuration::getParameter(string parameter, int* result){
	try{
	    *result = boost::lexical_cast<int>(db[parameter]);
	}catch (const std::exception&){
		*result = 0;
	}
}

void Configuration::getParameter(string parameter, double* result){
	try{
	    *result = boost::lexical_cast<double>(db[parameter]);
	}catch (const std::exception&){
		cout << "Variable " << parameter << " not found";
		*result = 0;
	}
}

void Configuration::getParameter(string parameter, int i, double* result){
	try{
		stringstream ss;
		ss << parameter << "[" << i << "]";
		*result = boost::lexical_cast<double>(db[ss.str()]);
	}
	catch (const std::exception&){
		cout << "Variable " << parameter << "@" << i <<" not found" << endl ;
		*result = 0;
	}
}

void Configuration::setParameter(string key, string value){
    db[key] = value ;
}

bool Configuration::checkParameter(string key, string value){
	if (strcmp(db[key].c_str(), value.c_str()) == 0){
		return true;
	}
	else{
		return false;
	}
}

string Configuration::get(string key){
	try{
		return db[key];
	}
	catch (const std::exception&){
		stringstream ss; 
		ss << "The key " << key << " not found in configuration file" << endl;
		return ss.str();
	}
	
}