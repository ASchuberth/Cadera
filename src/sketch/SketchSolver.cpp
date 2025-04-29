#include "SketchSolver.hpp"
#include "pch.hpp"

void CADERA_APP_NAMESPACE::sketch::SketchSolver::setActiveSketch(Sketch *S) {

  pActiveSketch = S;
}

bool CADERA_APP_NAMESPACE::sketch::SketchSolver::checkNewRelation(
    const Relation &R) {

  std::list<int> newRelationIds = R.mFeatureIds;

  // If relation doesn't involve any features, don't add relation
  if (newRelationIds.size() == 0)
    return false;

  for (const auto &id : newRelationIds) {

    // If point doesn't have any other relation, skip
    if (pActiveSketch->Points[id].relationIds.size() == 0) {
      continue;
    }

    // Does the point already have relations to the other points in the new
    // relation
    std::list<int> relatedPointIds = getRelatedPointIds(id);

    bool hasRelations = false;

    for (const auto &relPtId : relatedPointIds) {

      // Returns true if related point id is in the new relation ids;
      hasRelations = std::find(newRelationIds.begin(), newRelationIds.end(),
                               relPtId) != newRelationIds.end();

      if (hasRelations)
        return false;
    }
  }

  return true;
}

bool CADERA_APP_NAMESPACE::sketch::SketchSolver::checkCommenRelationPts(
    const Relation &R) {

  std::list<int> newRelationIds = R.mFeatureIds;

  size_t totalsize = 0;

  std::list<int> allUniqueRelatedPtIds;

  for (const auto &id : newRelationIds) {

    std::list<int> relatedPointIds = getRelatedPointIds(id);

    totalsize += relatedPointIds.size();

    bool hasRelations = false;

    std::list<int> UniqueIds =
        getDifferenceVector(allUniqueRelatedPtIds, relatedPointIds);

    for (const auto &relPtid : UniqueIds) {

      allUniqueRelatedPtIds.push_back(relPtid);
    }
  }

  if (allUniqueRelatedPtIds.size() == totalsize) {
    return true;
  } else {
    return false;
  }
}

// Returns a vector of the ids of points that have relations to the inputed
// point id
std::list<int>
CADERA_APP_NAMESPACE::sketch::SketchSolver::getRelatedPointIds(const int &PId) {

  if (pActiveSketch == nullptr)
    throw std::runtime_error("SketchSolver: Active Sketch is nullptr!!!");

  Point P = pActiveSketch->Points[PId];

  std::list<int> relatedPointIds;

  for (const auto &relId : P.relationIds) {

    std::list<int> v = {PId};
    std::list<int> featureIds =
        getDifferenceVector(v, pActiveSketch->mRelations[relId].mFeatureIds);
    ;

    featureIds = getDifferenceVector(relatedPointIds, featureIds);

    for (const auto &featId : featureIds) {

      relatedPointIds.push_back(featId);
    }
  }

  return relatedPointIds;
}

bool CADERA_APP_NAMESPACE::sketch::SketchSolver::addNewSketchRelation(
    const std::vector<int> &ids, RelationType Type) {

  bool isSuccess = false;

  Relation NewRelation;

  for (const auto &id : ids) {
    NewRelation.mFeatureIds.push_back(id);
  }

  NewRelation.mType = Type;

  if (checkNewRelation(NewRelation) && checkCommenRelationPts(NewRelation))
    pActiveSketch->addRelation(ids, Type);

  return isSuccess;
}
