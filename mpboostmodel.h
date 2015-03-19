/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _MPBOOSTMODEL_H_
#define _MPBOOSTMODEL_H_

#include <vector>
#include <set>
#include <limits>
#include <math.h>

#include "defs.h"
#include "exampleset.h"
#include "predictionset.h"
#include "boostingmodel.h"
#include "mpboosthypothesis.h"
#include "distribution.h"
#include "invertedlist.h"
#include "evaluation.h"

class MPBoostModel : public BoostingModel {
 public:
  
 MPBoostModel(int categoryCount):completedIterations(0),
    categoryCount(categoryCount) {
      hypothesisSet = new std::vector<MPBoostHypothesis *>();
    }
  
  ~MPBoostModel() {
    delete hypothesisSet;
  }
  
  virtual inline int GetCompletedIterations() { return completedIterations; }
  
  virtual inline int GetCategoryCount() { return categoryCount; }
  
  virtual inline int GetHypothesisCount() { return hypothesisSet->size(); }
  
  inline std::vector<MPBoostHypothesis *>::const_iterator GetHypothesisIterator() {
    return hypothesisSet->begin();
  }
  
  inline std::vector<MPBoostHypothesis *>::const_iterator GetHypothesisIteratorEnd() {
    return hypothesisSet->end();
  }
  
  void CutHypothesis(int num) {
    std::vector<MPBoostHypothesis *>::const_iterator it = hypothesisSet->begin()+num;
    std::vector<MPBoostHypothesis *>::const_iterator tend = hypothesisSet->end();
    while(it!=tend) {
      delete *it;
      ++it;
    }
    hypothesisSet->erase(hypothesisSet->begin()+num,hypothesisSet->end());
  }
  
  void CompressHypothesis();
  
  virtual std::set<int> * GetPivotFeatures();
  
  virtual int RunTrain(int requestedIterations,
	       ExampleSet * exampleSet,
	       Distribution ** distributionRef,
	       std::ostream & out,int verbosity);

  virtual int RunTest(std::istream & testfile,
	      Evaluation * evaluation,
	      PredictionSet * predictionSet,
	      std::ostream & out,int verbosity);

  /** Data format:
      <number of completed iteration>
      <number of categories>
      <number of hypothesis>
      <Hypothesis 0>
      ..
      <Hypothesis n>
  */
  static MPBoostModel * Read(std::istream & in);

  static MPBoostModel * Read(std::istream & in,int count);
  
  virtual void Write(std::ostream & out);
  
 private:
  int completedIterations;
  int categoryCount;
  std::vector<MPBoostHypothesis *> * hypothesisSet;
};

#endif // _MPBOOSTMODEL_H_
