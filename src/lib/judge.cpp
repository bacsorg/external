#include <bacs/external/judge.hpp>

#include <bunsan/curl/easy.hpp>
#include <bunsan/curl/options/callback.hpp>
#include <bunsan/curl/options/http.hpp>
#include <bunsan/curl/options/network.hpp>
#include <bunsan/protobuf/binary.hpp>

namespace bacs {
namespace external {

namespace curl = bunsan::curl;

struct Judge::impl {
  explicit impl(const std::string &address) : root(address) {}

  std::string root;
  curl::easy client;
};

Judge::Judge(const std::string &address) : pimpl(new impl(address)) {}

Judge::~Judge() {
  // ~impl()
}

#define BUNSAN_RPC(RET, METHOD, ARG)                              \
  RET Judge::METHOD(ARG arg) {                                    \
    return bunsan::protobuf::binary::parse_make<RET>(             \
        Call(#METHOD, bunsan::protobuf::binary::to_string(arg))); \
  }

BUNSAN_RPC(result::Handle, Send, const Submit &)
BUNSAN_RPC(result::HandleList, SendAll, const SubmitList &)

BUNSAN_RPC(result::Handle, Rejudge, const Submit::Id &)
BUNSAN_RPC(result::HandleList, RejudgeAll, const Submit::IdList &)

BUNSAN_RPC(result::Submit, FetchResult, const result::Handle &)
BUNSAN_RPC(result::SubmitList, FetchResultAll, const result::HandleList &)

BUNSAN_RPC(result::Submit, FetchLatestResult, const Submit::Id &)
BUNSAN_RPC(result::SubmitList, FetchLatestResultAll, const Submit::IdList &)

BUNSAN_RPC(result::MultipleHandle, GetResultRevisions, const Submit::Id &)
BUNSAN_RPC(result::MultipleHandleList, GetResultRevisionsAll,
           const Submit::IdList &)

std::string Judge::Call(const std::string &method, const std::string &message) {
  pimpl->client.set(curl::options::url(pimpl->root + "/" + method));
  pimpl->client.set(curl::options::postfields(message));
  std::string response;
  pimpl->client.set(curl::options::readfunction([&](char *ptr, size_t size) {
    response.append(ptr, size);
    return size;
  }));
  pimpl->client.perform();
  return response;
}

}  // namespace external
}  // namespace bacs
