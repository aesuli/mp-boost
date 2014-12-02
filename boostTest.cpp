/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include <time.h>
#include <iostream>
#include "aarg.h"
#include "boostingmodel.h"

using namespace std;
using namespace esuli::utils;

int main(int argc, char ** argv) {
	Aargh args(argc,argv);

	if(args.GetArgCount()==0||!args.HasArg("t")||!args.HasArg("m")) {
		cout << "Usage:" << endl
			<< "\t" << argv[0] << " -t testfile -m modelfile" << endl
			<< endl
			<< "Optional parameters:" << endl
			<< "\t-h <number of hypothesis to use from model> (default: all)" << endl
			<< "\t-oe <name of output evaluation file> (default: testfile.evaluation)" << endl
			<< "\t-op <name of output prediction file> (default: testfile.prediction)" << endl
			<< "\t-v <verbosity> [0-2] (default:1)" << endl
			<< "\t-nE (do NOT save evaluations, default: save)"<< endl
			<< "\t-nP (do NOT save predictions, default: save)"<< endl;

		return ERROR_COMMANDLINE;
	}

	int verbosity = args.GetArg<int>("v",1);

	BoostingModel * model = NULL;
	ExampleSet * exampleSet = NULL;
	Evaluation * evaluation = NULL;
	PredictionSet * predictionSet = NULL;

	if(verbosity>0)
		cout << "Loading data" << endl;

	string modelname = args.GetArg<string>("m","nofile.model");
	if(verbosity>1)
		cout << "Loading model file: " << modelname << endl;
	ifstream modelfile(modelname.c_str());
	if(args.HasArg("h")) {
	  int hyp = args.GetArg<int>("h",100);
	  if(verbosity>0)
	    cout << "Using the first " << hyp << " hypothesis" << endl;
	  model = BoostingModel::Read(modelfile,hyp);
	}
	else {
	  model = BoostingModel::Read(modelfile);
	  if(verbosity>0)
	    cout << "Using all the hypothesis in the model (" << model->GetHypothesisCount() << ")" << endl;
	}
	modelfile.close();
	if(verbosity>1)
	  cout << "Model file loaded (" << model->GetCompletedIterations() << " iterations, " <<
	    model->GetCategoryCount() << " categories)" << endl;
	
	if(args.HasArg("nP")) 
	  predictionSet = NULL;
	else
	  predictionSet = new PredictionSet(model->GetCategoryCount());
	
	if(args.HasArg("nE")) 
	  evaluation = NULL;
	else
	  evaluation = new Evaluation(model->GetCategoryCount());
	
	if(verbosity>0)
	  cout << "Data loaded" << endl;
	
	string testname = "nofile";
	
	testname = args.GetArg<string>("t","nofile");
	if(verbosity>1)
	  cout << "Opening test file: " << testname << endl;
	
	ifstream testfile(testname.c_str());

	if(verbosity>0)
	  cout << "Starting test" << endl;

	clock_t start = clock();
	int retval = model->RunTest(testfile,evaluation,predictionSet,cout,verbosity);
	clock_t end = clock();

	testfile.close();

	if(verbosity>0)
		cout << "Test completed in " << (end-start)/(double)CLOCKS_PER_SEC << " seconds." << endl;


	if(!args.HasArg("nE")) {
		string outevaluationname = args.GetArg<string>("oe",testname+".evaluation");
		if(verbosity>0)
			cout << "Serializing evaluation to file: " << outevaluationname << endl;
		ofstream outevaluation(outevaluationname.c_str());
		evaluation->Write(outevaluation);
		outevaluation.close();
		if(verbosity>0)
			cout << "Serialization completed" << endl;
		delete evaluation;
	}

	if(!args.HasArg("nP")) {
		string outpredictionname = args.GetArg<string>("op",testname+".prediction");
		if(verbosity>0)
			cout << "Serializing prediction to file: " << outpredictionname << endl;
		ofstream outprediction(outpredictionname.c_str());
		predictionSet->Write(outprediction);
		outprediction.close();
		if(verbosity>0)
			cout << "Serialization completed" << endl;
		delete predictionSet;
	}

	delete exampleSet;
	delete model;

	return retval;
}
