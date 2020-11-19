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

        callFunctions(req, res);
        if (res.hasEnded())
            return res.toString();

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
        m_functions.push_back({std::regex(pathStartingWith), function});
    }

private:
    std::deque<error_function> m_errorFunctions;
    std::deque<std::tuple<std::regex, router_function>> m_functions;

    static inline bool pathStartWith(const std::string &path, const std::regex &startWith)
    {
        return std::regex_search(path, startWith, std::regex_constants::match_continuous);
    }

    inline void callFunctions(request::request &req, response::response &res) const noexcept
    {
        std::string errorVal;
        auto errorCallback = [&errorVal](std::string value) {
            if (value.size())
                errorVal = value;
            else
                errorVal = "Server Error!";
        };
        for (auto &funcInfo : m_functions)
        {
            if (res.hasEnded() || errorVal.size())
                return;

            if (pathStartWith(req.header().getPath(), std::get<0>(funcInfo)))
                std::get<1>(funcInfo)(req, res, errorCallback);
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

    static inline void defaultError(response::response &res) noexcept
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
