#include <bacs/external/judge.hpp>

#include <bunsan/protobuf/binary.hpp>

#include <boost/network/protocol/http/client.hpp>
#include <boost/network/uri.hpp>

namespace bacs{namespace external
{
    namespace http = boost::network::http;
    namespace uri = boost::network::uri;

    struct Judge::impl
    {
        explicit impl(const std::string &address): root(address) {}

        std::string root;
        http::client client;
    };

    Judge::Judge(const std::string &address): pimpl(new impl(address)) {}

    Judge::~Judge() { /* implicit ~impl() */ }

#define BUNSAN_RPC(RET, METHOD, ARG) \
    RET Judge::METHOD(ARG arg) \
    { \
        return bunsan::protobuf::binary::parse_make<RET>(Call( \
            #METHOD, \
            bunsan::protobuf::binary::to_string(arg) \
        )); \
    }

    BUNSAN_RPC(result::Handle, Submit, const submit::Submit &)
    BUNSAN_RPC(result::HandleList, SubmitAll, const submit::SubmitList &)

    BUNSAN_RPC(result::Handle, Rejudge, const submit::Handle &)
    BUNSAN_RPC(result::HandleList, RejudgeAll, const submit::HandleList &)

    BUNSAN_RPC(result::Submit, FetchResult, const result::Handle &)
    BUNSAN_RPC(result::SubmitList, FetchResultAll, const result::HandleList &)

    BUNSAN_RPC(result::Submit, FetchLatestResult, const submit::Handle &)
    BUNSAN_RPC(result::SubmitList, FetchLatestResultAll, const submit::HandleList &)

    BUNSAN_RPC(result::MultipleHandle, GetResultRevisions, const submit::Handle &)
    BUNSAN_RPC(result::MultipleHandleList, GetResultRevisionsAll, const submit::HandleList &)

    std::string Judge::Call(const std::string &method,
                            const std::string &message)
    {
        http::client::request request(pimpl->root + "/" + method);
        request << boost::network::body(message);
        http::client::response response = pimpl->client.get(request);
        return body(response);
    }
}}
