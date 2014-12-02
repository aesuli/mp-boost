/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include <string>
#include <sstream>

#include "mpboostmodel.h"

void MPBoostModel::CompressHypothesis() {
  std::set<int> featsSet;
  std::vector<MPBoostHypothesis *>::const_iterator it = hypothesisSet->begin();
  std::vector<MPBoostHypothesis *>::const_iterator tend = hypothesisSet->end();
  while(it!=tend) {
    for(int i = 0;i<categoryCount;++i)
      featsSet.insert((*it)->GetFeature(i));
    ++it;
  }
  std::vector<int> feats(featsSet.begin(),featsSet.end());
  std::sort(feats.begin(),feats.end());
  std::vector<MPBoostHypothesis*> vect;
  std::vector<int>::const_iterator intit = feats.begin();
  std::vector<int>::const_iterator intend = feats.end();
  while(intit!=intend) {
    int feat = *intit;
    MPBoostHypothesis * hyp = new MPBoostHypothesis(categoryCount);
    vect.push_back(hyp);
    for(int i = 0;i<categoryCount;++i) {
      hyp->SetFeature(i,feat);
      hyp->SetC0(i,0.0);
      hyp->SetC1(i,0.0);
    }
    it = hypothesisSet->begin();
    while(it!=tend) {
      MPBoostHypothesis * hyp2 = *it;
      for(int i = 0;i<categoryCount;++i) {
	if(hyp2->GetFeature(i)==feat) {
	  hyp->AddToC0(i,hyp2->GetC0(i));
	  hyp->AddToC1(i,hyp2->GetC1(i));
	}
      }
      ++it;
    }
    ++intit;
  }
  CutHypothesis(0);
  it = vect.begin();
  tend = vect.end();
  while(it!=tend) {
    hypothesisSet->push_back(*it);
    ++it;
  }
}

std::set<int> * MPBoostModel::GetPivotFeatures() {
  std::set<int> * pivots = new std::set<int>();

  std::vector<MPBoostHypothesis *>::const_iterator hypit = hypothesisSet->begin();
  std::vector<MPBoostHypothesis *>::const_iterator hypend = hypothesisSet->end();
  while(hypit!=hypend) {
    for(int i = 0; i<categoryCount; ++i)
      pivots->insert((*hypit)->GetFeature(i));
    ++hypit;
  }
  
  return pivots;
}

int MPBoostModel::RunTrain(int requestedIterations,
			    ExampleSet * exampleSet,
			    Distribution ** distributionRef,
			    std::ostream & out,int verbosity) {
  int exampleCount = exampleSet->size();
  exampleSet->ResetIds();

  Distribution * distribution = *distributionRef;
  if(distribution==NULL) {
    distribution = new Distribution(exampleCount,categoryCount,Distribution::UNIFORM);
    *distributionRef = distribution;
  }
  else if(distribution->GetCategoryCount()!=categoryCount ||
	  distribution->GetExampleCount()!=exampleCount)
    return ERROR_WRONG_DISTRIBUTION;

  distribution->NormalizeByColumn();

  if(verbosity>1)
    out << "Building inverted list" << std::endl;
  InvertedList<ExampleSet,Example> invertedList(exampleSet);
  if(verbosity>1)
    out << "Inverted list completed" << std::endl;

  double epsilon = 1.0/(double)(categoryCount*exampleCount);
  if(verbosity>2)
    out << "Epsilon value is: " << epsilon << std::endl;

  double * Wp = new double[categoryCount];
  double * Wp0 = new double[categoryCount];
  double * Wp1 = new double[categoryCount];

  double * Wm = new double[categoryCount];
  double * Wm0 = new double[categoryCount];
  double * Wm1 = new double[categoryCount];

  int featureCount = invertedList.GetFeatureCount();

  double* scores = new double[categoryCount];

  while(completedIterations<requestedIterations) {

    if(verbosity>0)
      out << "Iteration " << completedIterations+1 << std::endl;

    MPBoostHypothesis * currHyp = new MPBoostHypothesis(categoryCount);
    for(int i =0;i<categoryCount;++i)
      currHyp->SetScore(i,MAXDOUBLE);

    for(int i = 0;i< categoryCount;++i)
      Wp[i] = 0.0;

    std::vector<Example *>:: const_iterator eit = exampleSet->begin();
    std::vector<Example *>:: const_iterator eend = exampleSet->end();
    while(eit!=eend) {
      Example * example = *eit;
      std::set<int>::const_iterator cit = example->GetCategoryIterator();
      std::set<int>::const_iterator cend = example->GetCategoryIteratorEnd();
      while(cit!=cend) {
	Wp[*cit] += distribution->GetValue(example->GetId(),*cit);
	++cit;
      }
      ++eit;
    }

    for(int i = 0;i < categoryCount;++i) {
      double categorySum = 0.0;
      for(int j = 0;j< exampleCount;++j)
	categorySum += distribution->GetValue(j,i);
      Wm[i] = categorySum - Wp[i];
    }

    for(int feature = 0;feature<featureCount;++feature) {
      for(int i = 0;i<categoryCount;++i) {
	Wp1[i] = 0.0;
	Wm1[i] = 0.0;
      }

      int length = invertedList.GetLength(feature);
      for(int pos = 0;pos<length;++pos) {
	Example * example = invertedList.Get(feature,pos);
	int id = example->GetId();

	int nextCategory = 0;
	std::set<int>::const_iterator cit = example->GetCategoryIterator();
	std::set<int>::const_iterator cend = example->GetCategoryIteratorEnd();
	while(cit!=cend) {
	  int currentCategory = *cit;

	  for(int i = nextCategory;i< currentCategory;++i)
	    Wm1[i] += distribution->GetValue(id,i);

	  Wp1[currentCategory] += distribution->GetValue(id,currentCategory);

	  nextCategory = currentCategory + 1;
	  ++cit;
	}

	for(int i = nextCategory;i<categoryCount;++i)
	  Wm1[i] += distribution->GetValue(id,i);

      }

      for(int i = 0;i<categoryCount;++i) {
	Wp0[i] = Wp[i] - Wp1[i];
	if(Wp0[i]<0)
	  Wp0[i]=0;
	Wm0[i] = Wm[i] - Wm1[i];
	if(Wm0[i]<0)
	  Wm0[i]=0;
      }

      for(int i = 0;i<categoryCount;++i)
	scores[i] = 2.0*(sqrt(Wp0[i]*Wm0[i]) + sqrt(Wp1[i]*Wm1[i]));

      for(int i = 0;i<categoryCount;++i) {      
	if(scores[i]<currHyp->GetScore(i)) {
	  currHyp->SetScore(i,scores[i]);
	  currHyp->SetFeature(i,feature);
	  currHyp->SetC0(i,0.5*log((Wp0[i]+epsilon)/(Wm0[i]+epsilon)));
	  currHyp->SetC1(i,0.5*log((Wp1[i]+epsilon)/(Wm1[i]+epsilon)));
	}
      }
    }

    if(verbosity>2) {
      for(int i = 0;i<categoryCount;++i)
	out << '(' << currHyp->GetFeature(i) << ',' << currHyp->GetScore(i) << ") ";
      out << std::endl;
    }
    
    eit = exampleSet->begin();
    while(eit!=eend) {
      Example * example = *eit;
      for(int k = 0;k<categoryCount;++k) {
	double exponent;
	if(example->HasFeature(currHyp->GetFeature(k))) {
	  if(example->HasCategory(k))
	    exponent = -currHyp->GetC1(k);
	  else
	    exponent = currHyp->GetC1(k);
	}
	else {
	  if(example->HasCategory(k))
	    exponent = -currHyp->GetC0(k);
	  else
	    exponent = currHyp->GetC0(k);
	}
	distribution->MulToValue(example->GetId(),k,
				 pow(2.7182818285,exponent));
      }
      ++eit;
    }
    
    if(verbosity>1) 
      out << "Distribution sum before normalization = " << distribution->GetSum() << std::endl;

    distribution->NormalizeByColumn();
    
    hypothesisSet->push_back(currHyp);
    
    ++completedIterations;
  }

  delete Wp;
  delete Wp0;
  delete Wp1;

  delete Wm;
  delete Wm0;
  delete Wm1;

  delete scores;

  return OK;
}

int MPBoostModel::RunTest(std::istream & testfile,
			   Evaluation * evaluation,
			   PredictionSet * predictionSet,
			   std::ostream & out,int verbosity) {
  CompressHypothesis();
  double * sums = new double[categoryCount];

  std::set<int> * pivots = GetPivotFeatures();

  std::string line;
  while(std::getline(testfile,line)) {
    std::istringstream buffer(line);
    Example * example = Example::Read(buffer,pivots);

    for(int i=0; i<categoryCount; ++i)
      sums[i] = 0.0;
    
    MPBoostHypothesis * hyp;
    double * ref;
    std::vector<MPBoostHypothesis *>::const_iterator hypit = hypothesisSet->begin();
    std::vector<MPBoostHypothesis *>::const_iterator hypend = hypothesisSet->end();
    while(hypit!=hypend) {
      hyp = *hypit;
      if (example->HasFeature(hyp->GetFeature(0)))
	ref = hyp->GetC1();
      else
	ref = hyp->GetC0();
      for(int i = 0; i<categoryCount; ++i,++ref) {
	if(*ref!=0.0)
	  sums[i] += *ref;
      }
      ++hypit;
    }
    
    std::vector<std::pair<int,double> > categories;
    for(int i=0; i<categoryCount; ++i) {
      if(predictionSet!=NULL) {
	std::pair<int,double> ev(i,sums[i]);
	categories.push_back(ev);
      }
      if(evaluation!=NULL) {
	if (sums[i] > 0.0){
	  if (example->HasCategory(i))
	    evaluation->AddTP(i);
	  else
	    evaluation->AddFP(i);
	}
	else {
	  if (example->HasCategory(i))
	    evaluation->AddFN(i);
	  else 
	    evaluation->AddTN(i);
	}
      }
    }
    if(predictionSet!=NULL)
      predictionSet->push_back(new Prediction(example->GetId(),categories));

    if(verbosity>2)
      out << ".";
  }
  
  if(verbosity>2)
    out << std::endl;
  
  delete sums;
  delete pivots;

  return OK;
}

MPBoostModel * MPBoostModel::Read(std::istream & in) {
  MPBoostModel * abm = new MPBoostModel(0);
  in >> abm->completedIterations;
  delete abm->hypothesisSet;
  abm->hypothesisSet = new std::vector<MPBoostHypothesis *>();
  in >> abm->categoryCount;
  int hypothesisCount;
  in >> hypothesisCount;
  for(int i = 0;i<hypothesisCount;++i)
    abm->hypothesisSet->push_back(MPBoostHypothesis::Read(in));
  return abm;
}

MPBoostModel * MPBoostModel::Read(std::istream & in,int count) {
  MPBoostModel * abm = new MPBoostModel(0);
  in >> abm->completedIterations;
  delete abm->hypothesisSet;
  abm->hypothesisSet = new std::vector<MPBoostHypothesis *>();
  in >> abm->categoryCount;
  int hypothesisCount;
  in >> hypothesisCount;
  if(hypothesisCount>count)
    hypothesisCount = count;
  for(int i = 0;i<hypothesisCount;++i)
    abm->hypothesisSet->push_back(MPBoostHypothesis::Read(in));
  return abm;
}

void MPBoostModel::Write(std::ostream & out) {
  out << MPBOOST << std::endl;
  out << completedIterations << std::endl;
  out << categoryCount << std::endl;
  out << (int)hypothesisSet->size() << std::endl;
  std::vector<MPBoostHypothesis *>::const_iterator it = hypothesisSet->begin();
  std::vector<MPBoostHypothesis *>::const_iterator tend = hypothesisSet->end();
  while(it!=tend) {
    (*it)->Write(out);
    ++it;
  }
}
