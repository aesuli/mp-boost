/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _AARG_H_
#define _AARG_H_

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstring>
#include <sstream>

namespace esuli {
	namespace utils {
		// Advanced ARGument Header
		class Aargh {
		public:
			Aargh() {
			}

			Aargh(const std::string & filename) {
				LoadConf(filename);
			}

			Aargh(int argc,char **argv) {
				ParseArgs(argc,argv);
			}

			Aargh(int argc,char **argv,const std::string & filename) {
				ParseArgs(argc,argv);
				LoadConf(filename);
			}

			bool ParseArgs(int argc,char **argv) {
				bool allright = true;
				std::string base = "";
				for(int i = 1; i<argc; ++i) {
					if(argv[i][0] == '-') {
						base = std::string((char *)(argv[i]+sizeof(char)));
						argmap[base] = "_Aargh";
					}
					else {
						if(base != "" ) {
							argmap[base] = std::string(argv[i]);
							base = "";
						}
						else
							allright = false;
					}
				}
				return allright;
			}

			bool LoadConf(const std::string & filename) {
				bool allright = true;
				std::ifstream in(filename.c_str());

				if(!in.is_open())
					return false;

				char line[1024];
				while(!in.eof()) {
					in.getline(line,1024);
					char *ln = line;
					if(*ln!='#'&&*ln!='\n') {
						char *key = ln;
						char *value = ln;

						while((*value!=' ')&&(*value!='\t')&&(*value!='\n'))
							++value;

						if(*value=='\n') {
							*value = '\0';
							argmap[key] = "_Aargh";
							continue;
						}

						while((*value==' ')||(*value=='\t')) {
							*value = '\0';
							++value;
						}

						char *end = value;
						while((*end!='\n')&&(*end))
							++end;
						*end = '\0';

						if(strcmp(key,"")&&strcmp(value,""))
							argmap[key] = value;
					}
				}
				in.close();
				return allright;
			}

			inline int GetArgCount() {
				return argmap.size();
			}

			inline bool HasArg(const std::string & arg) {
				return argmap.find(arg)!=argmap.end();
			}

			template<typename T>
			T GetArg(const std::string & arg,const T & defval) {
				std::map<std::string,std::string>::const_iterator mip = argmap.find(arg);
				if(mip==argmap.end()) {
					std::ostringstream ostrstr;
					ostrstr << defval << std::ends;
					std::string temp(ostrstr.str());
					argmap[arg] = temp;
					return defval;
				}
				else {
					std::istringstream istrstr(mip->second.c_str());
					T val;
					istrstr >> val;
					return val;
				}
			}

			void Clear() {
				argmap.clear();
			}

			void Write(std::ostream & out) {
				std::map<std::string,std::string>::const_iterator mip = argmap.begin();
				std::map<std::string,std::string>::const_iterator mep = argmap.end();
				while(mip!=mep) {
					out << mip->first << '\t' << mip->second << std::endl;
					++mip;
				}
			}

		private:
			std::map<std::string,std::string> argmap;
		};
	}
}

#endif // _AARG_H_
