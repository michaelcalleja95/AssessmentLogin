// AssessmentLogin.cpp : Defines the entry point for the application.
//

#include "AssessmentLogin.h"

#include <boost/system/system_error.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include <iostream>

constexpr auto VERSION = 11;                            ///< http version 1.1
constexpr auto WEBSITE = "testphp.vulnweb.com";         ///< host to connect to
constexpr auto PORT = "80";                             ///< HTTP port
constexpr auto TARGET = "/userinfo.php";                   ///< target site on the host
constexpr auto CREDENTAILS = "uname=test&pass=test";    ///< payload data

int main(int argc, char** argv)
{
    try
    {
        //initial setup to connect to the site
        boost::asio::io_context context;
        const boost::asio::ip::tcp::resolver resolver(context);
        boost::beast::tcp_stream stream(context);
        const auto endpoint = stream.connect(boost::asio::ip::tcp::resolver(context).resolve(WEBSITE, PORT));

        //create request to replicate sending login credentials to the login page
        boost::beast::http::request<boost::beast::http::string_body> request{ boost::beast::http::verb::post, TARGET, VERSION };
        request.set(boost::beast::http::field::host, WEBSITE);
        request.set(boost::beast::http::field::connection, "keep-alive");
        request.set(boost::beast::http::field::cache_control, "max-age=0");
        request.set(boost::beast::http::field::origin, "http://testphp.vulnweb.com");
        request.set(boost::beast::http::field::content_type, "application/x-www-form-urlencoded");
        request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);       
        request.set(boost::beast::http::field::accept,"text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
        request.set(boost::beast::http::field::referer, "http://testphp.vulnweb.com/login.php");
        request.set(boost::beast::http::field::accept_encoding, "gzip, deflate");
        request.set(boost::beast::http::field::accept_language, "en-US,en;q=0.9");
        request.set(boost::beast::http::field::cookie, "login=test%2Ftest");
        request.body() = CREDENTAILS;
        request.prepare_payload();

        //sends request
        boost::beast::http::write(stream, request);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> response;

        //reads and outputs response
        boost::beast::http::read(stream, buffer, response);
        std::cout << response << std::endl;

        //cleanup
        boost::beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

        if (ec && ec != boost::beast::errc::not_connected)
        {
            throw boost::beast::system_error{ ec };
        }
    }
    catch (boost::system::system_error const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
