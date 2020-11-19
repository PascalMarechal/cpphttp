#include "router.h"

#include <deque>
#include <regex>

using namespace cpphttp::server;
using namespace cpphttp::response;

class router::impl
{
public:
    inline std::string process(cpphttp::request::request &req) const
    {
        if (!req.isReady())
            throw std::invalid_argument("Invalid requests are not allowed");

        response::response res;

        if (std::regex_search(req.header().getPath(), std::regex(m_path), std::regex_constants::match_continuous))
            callFunctions(req, res);

        callErrorFunctions("error", req, res);

        if (!res.hasEnded())
            defaultError(res);

        return res.toString();
    }

    inline void error(error_function f) noexcept
    {
        m_errorFunctions.push_back(f);
    }

    inline void use(std::string pathStartingWith, router_function function) noexcept
    {
        m_functions.push_back(function);
        m_path = pathStartingWith;
    }

private:
    std::deque<error_function> m_errorFunctions;
    std::deque<router_function> m_functions;
    std::string m_path;

    inline void callFunctions(request::request &req, response::response &res) const noexcept
    {
        auto errorCallback = [](std::string value) {};
        for (auto f : m_functions)
        {
            f(req, res, errorCallback);
        }
    }

    inline void callErrorFunctions(const std::string &error, request::request &req, response::response &res) const noexcept
    {
        for (auto f : m_errorFunctions)
        {
            if (res.hasEnded())
                return;
            f(error, req, res);
        }
    }

    inline void defaultError(response::response &res) const noexcept
    {
        res.status(status::_500);
        res.send("Internal Server Error Detected.");
    }
};

router::router() : m_impl(std::make_unique<impl>()) {}

router::~router() {}

std::string router::process(cpphttp::request::request &req) const
{
    return m_impl->process(req);
}

void router::error(error_function f) noexcept
{
    m_impl->error(f);
}

void router::use(std::string pathStartingWith, router_function function) noexcept
{
    m_impl->use(pathStartingWith, function);
}
