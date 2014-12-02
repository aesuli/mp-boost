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
  Aargh args(argc,argv);

  if(args.GetArgCount()==0||!args.HasArg("e")) {
    cout << "Usage:" << endl
	 << "\t" << argv[0] << " -e evaluationfile" << endl
	 << endl
	 << "Optional parameters:" << endl
	 << "\t-c (show only contingency table)" << endl
	 << "\t-p (show only performance measures)" << endl
	 <<"\tnote: -c and -p are mutually exclusive, default is show both" << endl
	 << "\t-M (compute only MACRO-average measures)" << endl
	 << "\t-m (compute only MICRO-average measures)" << endl
	 <<"\tnote: -M and -m are mutually exclusive, default is compute both" << endl;
    return ERROR_COMMANDLINE;
  }
  
  string resultname = args.GetArg<string>("e","nofile.evaluation");
  cout << "Evaluation file: " << resultname << endl << endl;
  ifstream resultfile(resultname.c_str());
  Evaluation * evaluation = Evaluation::Read(resultfile);
  resultfile.close();
  
  if(!args.HasArg("p")) {
    if(!args.HasArg("M")) {
      cout << "Global table" << endl;
      cout << "TP\tTN\tFN\tFP" << endl;
      cout << evaluation->GetTP() << '\t'
	   << evaluation->GetTN() << '\t'
	   << evaluation->GetFN() << '\t'
	   << evaluation->GetFP() << endl;
      cout << endl;
    }
    if(!args.HasArg("m")) {
      cout << "Per-category tables" << endl;
      cout << "cat\tTP\tTN\tFN\tFP" << endl;
      for(int i = 0;i<evaluation->GetCategoryCount();++i) {
	cout << i << '\t'
	     << evaluation->GetTP(i) << '\t'
	     << evaluation->GetTN(i) << '\t'
	     << evaluation->GetFN(i) << '\t'
	     << evaluation->GetFP(i) << endl;
      }
      cout << endl;
    }
  }
  if(!args.HasArg("c")) {
    if(!args.HasArg("m")) {
      cout << "Per-category evaluation" << endl;
      cout << "cat\taccuracy\tprecision\trecall\tF1" << endl;
      for(int i = 0;i<evaluation->GetCategoryCount();++i) {
	cout << i << '\t'
	     << evaluation->GetAccuracy(i) << '\t'
	     << evaluation->GetPrecision(i) << '\t' 
	     << evaluation->GetRecall(i) << '\t'
	     << evaluation->GetF1(i) << endl;
      }
      cout << endl;
      cout << "MACRO-average evaluation" << endl;
      cout << "accuracy\tprecision\trecall\tF1" << endl;
      cout << evaluation->GetMacroAccuracy() << '\t'
	   << evaluation->GetMacroPrecision() << '\t' 
	   << evaluation->GetMacroRecall() << '\t'
	   << evaluation->GetMacroF1() << endl;
      cout << endl;
    }
    if(!args.HasArg("M")) {
      cout << "MICRO-average evaluation" << endl;
      cout << "accuracy\tprecision\trecall\tF1" << endl;
      cout << evaluation->GetMicroAccuracy() << '\t'
	   << evaluation->GetMicroPrecision() << '\t'
	   << evaluation->GetMicroRecall() << '\t'
	   << evaluation->GetMicroF1() << endl;
      cout << endl;
    }
  }
  return OK;
}
