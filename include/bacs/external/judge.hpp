#pragma once

#include <bacs/external/external.pb.h>

#include <boost/noncopyable.hpp>

#include <memory>

namespace bacs {
namespace external {

class Judge : private boost::noncopyable {
 public:
  explicit Judge(const std::string &address);
  ~Judge();

 public:
  result::Id Send(const Submit &submit);
  result::IdList SendAll(const SubmitList &submits);

  result::Id Rejudge(const Submit::Id &id);
  result::IdList RejudgeAll(const Submit::IdList &ids);

  result::Submit FetchResult(const result::Id &id);
  result::SubmitList FetchResultAll(const result::IdList &ids);

  result::Submit FetchLatestResult(const Submit::Id &id);
  result::SubmitList FetchLatestResultAll(const Submit::IdList &ids);

  result::MultipleId GetResultRevisions(const Submit::Id &id);
  result::MultipleIdList GetResultRevisionsAll(const Submit::IdList &ids);

 private:
  std::string Call(const std::string &method, const std::string &message);

 private:
  class impl;
  std::unique_ptr<impl> pimpl;
};

}  // namespace external
}  // namespace bacs
