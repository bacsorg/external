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
  result::Handle Send(const Submit &submit);
  result::HandleList SendAll(const SubmitList &submits);

  result::Handle Rejudge(const Submit::Id &id);
  result::HandleList RejudgeAll(const Submit::IdList &ids);

  result::Submit FetchResult(const result::Handle &handle);
  result::SubmitList FetchResultAll(const result::HandleList &handles);

  result::Submit FetchLatestResult(const Submit::Id &id);
  result::SubmitList FetchLatestResultAll(const Submit::IdList &ids);

  result::MultipleHandle GetResultRevisions(const Submit::Id &id);
  result::MultipleHandleList GetResultRevisionsAll(const Submit::IdList &ids);

 private:
  std::string Call(const std::string &method, const std::string &message);

 private:
  class impl;
  std::unique_ptr<impl> pimpl;
};

}  // namespace external
}  // namespace bacs
