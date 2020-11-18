#include "router.h"

#include <deque>

using namespace cpphttp::server;
using namespace cpphttp::response;

class router::impl
{
public:
    std::string process(cpphttp::request::request &req)
    {
        if (!req.isReady())
            throw std::invalid_argument("Invalid requests are not allowed");

        response::response res;

        callErrorFunctions("error", req, res);

        if (res.hasEnded())
            return res.toString();

        return "500 Internal Server Error";
    }

    void error(error_function f)
    {
        m_errorFunctions.push_back(f);
    }

private:
    std::deque<error_function> m_errorFunctions;

    void callErrorFunctions(const std::string &error, request::request &req, response::response &res)
    {
        for (error_function f : m_errorFunctions)
        {
            if (res.hasEnded())
                break;
            f(error, req, res);
        }
    }
};

router::router() : m_impl(std::make_unique<impl>()) {}

router::~router() {}

std::string router::process(cpphttp::request::request &req)
{
    return m_impl->process(req);
}

void router::error(error_function f)
{
    m_impl->error(f);
}
