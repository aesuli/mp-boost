/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _BOOSTINGMODEL_H_
#define _BOOSTINGMODEL_H_

#include <iostream>

#include "defs.h"
#include "exampleset.h"
#include "predictionset.h"
#include "distribution.h"
#include "evaluation.h"

class BoostingModel {
 public:
  
  virtual int GetCompletedIterations() = 0;
  
  virtual int GetCategoryCount() = 0;

  virtual int GetHypothesisCount() = 0;

  virtual std::set<int> * GetPivotFeatures() = 0;

  virtual int RunTrain(int requestedIterations,
	       ExampleSet * exampleSet,
	       Distribution ** distributionRef,
	       std::ostream & out,int verbosity) = 0;

  virtual int RunTest(std::istream & testfile,
		      Evaluation * evaluation,
		      PredictionSet * predictionSet,
		      std::ostream & out,int verbosity) = 0;

  /** Data format:
      <model type>
      <model data>
  */
  static BoostingModel * Read(std::istream & in);
  
  static BoostingModel * Read(std::istream & in,int count);
  
  virtual void Write(std::ostream & out) = 0;

};

#endif // _BOOSTINGMODEL_H_
