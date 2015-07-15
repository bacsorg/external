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
  result::Handle Submit(const submit::Submit &submit);
  result::HandleList SubmitAll(const submit::SubmitList &submits);

  result::Handle Rejudge(const submit::Handle &handle);
  result::HandleList RejudgeAll(const submit::HandleList &handles);

  result::Submit FetchResult(const result::Handle &handle);
  result::SubmitList FetchResultAll(const result::HandleList &handles);

  result::Submit FetchLatestResult(const submit::Handle &handle);
  result::SubmitList FetchLatestResultAll(const submit::HandleList &handles);

  result::MultipleHandle GetResultRevisions(const submit::Handle &handle);
  result::MultipleHandleList GetResultRevisionsAll(
      const submit::HandleList &handles);

 private:
  std::string Call(const std::string &method, const std::string &message);

 private:
  class impl;
  std::unique_ptr<impl> pimpl;
};

}  // namespace external
}  // namespace bacs
