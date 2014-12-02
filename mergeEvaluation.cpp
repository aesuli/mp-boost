/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include <iostream>
#include "aarg.h"
#include "evaluation.h"

using namespace std;
using namespace esuli::utils;

int main(int argc, char ** argv) {
	if(argc<3) {
		cout << "Usage:" << endl
			<< "\t" << argv[0] << " output_evaluationfile [input_evaluation_file]+" << endl;
		return ERROR_COMMANDLINE;
	}

	string firstInput(argv[2]);
	ifstream firstfile(firstInput.c_str());
	cout << "Merging: " << firstfile << endl;
	Evaluation * evaluation = Evaluation::Read(firstfile);
	firstfile.close();

	for(int i = 3;i<argc;++i) {
		string otherInput(argv[i]);
		ifstream otherfile(otherInput.c_str());
		cout << "Merging: " << otherfile << endl;
		Evaluation * otherEvaluation = Evaluation::Read(otherfile);
		otherfile.close();
		evaluation->Merge(otherEvaluation);
		delete otherEvaluation;
	}

	string output(argv[1]);
	cout << "Output file: " << output << endl;
	ofstream outevaluation(output.c_str());
	evaluation->Write(outevaluation);

	delete evaluation;

	return OK;
}
