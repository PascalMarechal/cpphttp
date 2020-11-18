#include "router.h"

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

        if (m_errorFunction)
            m_errorFunction("error", req, res);

        if (res.hasEnded())
            return res.toString();

        return "500 Internal Server Error";
    }

    void error(error_function f)
    {
        m_errorFunction = f;
    }

private:
    error_function m_errorFunction = nullptr;
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
