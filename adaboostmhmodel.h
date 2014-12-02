/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#ifndef _ADABOOSTMHMODEL_H_
#define _ADABOOSTMHMODEL_H_

#include <vector>
#include <set>
#include <values.h>
#include <math.h>

#include "defs.h"
#include "exampleset.h"
#include "predictionset.h"
#include "boostingmodel.h"
#include "adaboostmhhypothesis.h"
#include "distribution.h"
#include "invertedlist.h"
#include "evaluation.h"

class AdaBoostMHModel : public BoostingModel {
 public:
  
 AdaBoostMHModel(int categoryCount):completedIterations(0),
    categoryCount(categoryCount) {
      hypothesisSet = new std::vector<AdaBoostMHHypothesis *>();
    }
  
  ~AdaBoostMHModel() {
    delete hypothesisSet;
  }
  
  virtual inline int GetCompletedIterations() { return completedIterations; }
  
  virtual inline int GetCategoryCount() { return categoryCount; }

  virtual inline int GetHypothesisCount() { return hypothesisSet->size(); }
  
  inline std::vector<AdaBoostMHHypothesis *>::const_iterator GetHypothesisIterator() {
    return hypothesisSet->begin();
  }
  
  inline std::vector<AdaBoostMHHypothesis *>::const_iterator GetHypothesisIteratorEnd() {
    return hypothesisSet->end();
  }
  
  inline void CutHypothesis(int num) {
    std::vector<AdaBoostMHHypothesis *>::const_iterator it = hypothesisSet->begin()+num;
    std::vector<AdaBoostMHHypothesis *>::const_iterator tend = hypothesisSet->end();
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
  static AdaBoostMHModel * Read(std::istream & in);

  static AdaBoostMHModel * Read(std::istream & in,int count);
  
  virtual void Write(std::ostream & out);
  
 private:
  int completedIterations;
  int categoryCount;
  std::vector<AdaBoostMHHypothesis *> * hypothesisSet;
};

#endif // _ADABOOSTMHMODEL_H_
