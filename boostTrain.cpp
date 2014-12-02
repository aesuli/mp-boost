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
#include "mpboostmodel.h"
#include "adaboostmhmodel.h"

using namespace std;
using namespace esuli::utils;

int main(int argc, char ** argv) {
	Aargh args(argc,argv);

	if(args.GetArgCount()==0||!args.HasArg("t")) {
	  cout << "Usage:" << endl
	       << "\t" << argv[0] << " -t trainfile" << endl
	       << endl
	       << "Optional parameters:" << endl
	       << "\t-i <number of iteration to perform> (default: 100)" << endl
	       << "\t-m <previous model file to load to continue training>" << endl
	       << "\t-d <assigned distribution to load at beginning of traininig>" << endl
	       << "\t-A (use AdaBoost.MH algorithm, default: MPBoost)" << endl
	       << "\t-dB (use a balanced distribution, default: uniform)" << endl
	       << "\t-om <name of output model file> (default: trainfile.model)" << endl
	       << "\t-od <name of output distribution file> (default: trainfile.distribution)" << endl
	       << "\t-v <verbosity> [0-4] (default:1)" << endl;
	  return ERROR_COMMANDLINE;
	}

	int requestedIterations = args.GetArg<int>("i",100);
	int verbosity = args.GetArg<int>("v",1);
	bool adaboost = args.HasArg("A");

	BoostingModel * model = NULL;
	Distribution * distribution;
	ExampleSet * exampleSet = NULL;
	
	if(verbosity>0)
		cout << "Loading data" << endl;

	string trainname = "nofile";

	trainname = args.GetArg<string>("t","nofile");
	if(verbosity>1)
		cout << "Loading training file: " << trainname << endl;
	ifstream trainfile(trainname.c_str());
	exampleSet = ExampleSet::Read(trainfile);
	trainfile.close();

	if(verbosity>1)
		cout << "Training file loaded (" << 
		exampleSet->size() << " examples, " << 
		exampleSet->GetFeatureCount() << " features, " << 
		exampleSet->GetCategoryCount() << " categories)" << endl;


	if(args.HasArg("m")) {
		string modelname = args.GetArg<string>("m","nofile.model");
		if(verbosity>1)
			cout << "Loading model file: " << modelname << endl;
		ifstream modelfile(modelname.c_str());
 		model = BoostingModel::Read(modelfile);
		modelfile.close();
		if(verbosity>1)
			cout << "Model file loaded (" << model->GetCompletedIterations() << " iteration completed)" << endl;
		requestedIterations += model->GetCompletedIterations();
	}
	else {
	  if(adaboost)
	    model = new AdaBoostMHModel(exampleSet->GetCategoryCount());
	  else
	    model = new MPBoostModel(exampleSet->GetCategoryCount());
	  
		if(verbosity>1)
			cout << "Model created" << endl;
	}
	
	if(args.HasArg("d")) {
		string distributionname = args.GetArg<string>("d","nofile.distribution");
		if(verbosity>0)
			cout << "Loading distribution file: " << distributionname << endl;
		ifstream distributionfile(distributionname.c_str());
		distribution = Distribution::Read(distributionfile);
		distributionfile.close();
		if(verbosity>0)
			cout << "Distribution file loaded" << endl;
	}
	else if(args.HasArg("dB")) {
		if(verbosity>0)
			cout << "Using balanced distribution" << endl;	
		distribution = new Distribution(exampleSet->size(),exampleSet->GetCategoryCount(),Distribution::BALANCED,exampleSet);
	}
	else {
		if(verbosity>0)
			cout << "Using uniform distribution" << endl;	
		distribution = new Distribution(exampleSet->size(),exampleSet->GetCategoryCount(),Distribution::UNIFORM);
	}

	if(verbosity>0)
		cout << "Data loaded" << endl;

	if(verbosity>0)
		cout << "Starting training" << endl;
	int retval;

	clock_t start = clock();
	retval = model->RunTrain(requestedIterations, exampleSet, &distribution,
				 cout, verbosity);
	clock_t end = clock();

	if(verbosity>0)
		cout << "Training completed in " << (end-start)/(double)CLOCKS_PER_SEC << " seconds." << endl;

	string outmodelname = args.GetArg<string>("om",trainname+".model");
	if(verbosity>0)
		cout << "Serializing model to file: " << outmodelname << endl;
	ofstream outmodel(outmodelname.c_str());
	model->Write(outmodel);
	outmodel.close();
	if(verbosity>0)
	  cout << "Serialization completed" << endl;
	
	string outdistributionname = args.GetArg<string>("od",trainname+".distribution");
	if(verbosity>0)
	  cout << "Serializing distribution to file: " << outdistributionname << endl;
	ofstream outdistribution(outdistributionname.c_str());
	distribution->Write(outdistribution);
	outdistribution.close();
	if(verbosity>0)
		cout << "Serialization completed" << endl;

	delete exampleSet;
	delete model;
	delete distribution;

	return retval;
}
