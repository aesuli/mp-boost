/************************************************************************/
/*                                                                      */
/*   MPBoost++                                                          */
/*                                                                      */
/*   Copyright: Andrea Esuli                                            */
/*   Date: 16 august 2010                                               */
/*   License and Info: http://www.esuli.it/                             */           
/*                                                                      */
/************************************************************************/

#include "boostingmodel.h"
#include "mpboostmodel.h"
#include "adaboostmhmodel.h"
#include "defs.h"

BoostingModel * BoostingModel::Read(std::istream & in) {
  int modelType;
  in >> modelType;
  if(modelType==MPBOOST)
    return MPBoostModel::Read(in);
  if(modelType==ADABOOSTMH)
    return AdaBoostMHModel::Read(in);
  return NULL;
}

BoostingModel * BoostingModel::Read(std::istream & in,int count) {
  int modelType;
  in >> modelType;
  if(modelType==MPBOOST)
    return MPBoostModel::Read(in,count);
  if(modelType==ADABOOSTMH)
    return AdaBoostMHModel::Read(in,count);
  return NULL;
}
