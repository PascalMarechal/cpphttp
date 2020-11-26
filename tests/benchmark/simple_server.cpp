/**
 *  Copyright (c) 2020 Pascal Marechal
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE)
 */
#include "cpphttp.h"

using namespace cpphttp::server;
using namespace cpphttp::request;
using namespace cpphttp::response;

int main(void)
{
    // Create a server at port 8080
    server myserver(8080);

    // Create a simple router with several routes
    // One main page
    // Form get & post path
    router myrouter;
    myrouter.onGet("/", [](cpphttp::request::request &, cpphttp::response::response &res, error_callback) {
        res.send("<h1>Welcome to simple server</h1><a href='/form'>Go to form page</a>");
    });

    myrouter.onGet("/form", [](cpphttp::request::request &, cpphttp::response::response &res, error_callback) {
        res.send("<!DOCTYPE html>"
                 "<html lang=\"en\">"
                 "<head>"
                 "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
                 "</head>"
                 "<body>"
                 "<h1>Test form</h1>"
                 "<form action='/form' method='post'><div><label for='name'>Name :</label>"
                 "<input type='text' id='name' name='user_name'>"
                 "</div>"
                 "<div>"
                 "<label for='mail'>Email :</label>"
                 "<input type='email' id='mail' name='user_email'>"
                 "</div>"
                 "<div class='button'>"
                 "<button type='submit'>Send message</button>"
                 "</div>"
                 "</form>"
                 "<a href='/'>Go back to home page</a>"
                 "</body>");
    });

    myrouter.onPost("/form", [](cpphttp::request::request &req, cpphttp::response::response &res, error_callback) {
        res.send("<!DOCTYPE html>"
                 "<html lang=\"en\">"
                 "<head>"
                 "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
                 "</head>"
                 "<body>"
                 "<h1>Form has been sent to the server</h1>"
                 "<div>Name was : " +
                 req.getParam("user_name") +
                 "</div>"
                 "<div>Email was : " +
                 req.getParam("user_email") +
                 "</div>"
                 "<a href='/'>Go back to home page</a>"
                 "</body>");
    });

    // Set the router to the server
    myserver.setRouter(std::move(myrouter));

    // Start the server
    myserver.start();
    return 0;
}