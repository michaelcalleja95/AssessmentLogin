// AssessmentLogin.cpp : Contains all needed functionality
//
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/http/read.hpp>

#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/basic_stream.hpp>

#include <boost/beast/version.hpp>

#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/system/system_error.hpp>

#include <iostream>
#include <string>

constexpr auto VERSION = 11;                                        ///< http version 1.1
constexpr auto WEBSITE = "testphp.vulnweb.com";                     ///< host to connect to
constexpr auto ORIGIN = "http://testphp.vulnweb.com";               ///< simulation hostname origin
constexpr auto REFERER = "http://testphp.vulnweb.com/login.php";    ///< simulation where the request originated from
constexpr auto PORT = "80";                                         ///< HTTP port
constexpr auto TARGET = "/userinfo.php";                            ///< target site on the host
constexpr auto CREDENTAILS = "uname=test&pass=test";                ///< payload data

int main()
{
    try
    {
        //initial setup to connect to the site
        boost::asio::io_context context;

        boost::beast::tcp_stream stream(context);
        const auto endpoint = stream.connect(boost::asio::ip::tcp::resolver(context).resolve(WEBSITE, PORT));

        //create request to replicate sending login credentials to the login page
        boost::beast::http::request<boost::beast::http::string_body> request{ boost::beast::http::verb::post, TARGET, VERSION };
        request.set(boost::beast::http::field::host, WEBSITE);
        request.set(boost::beast::http::field::origin, ORIGIN);
        request.set(boost::beast::http::field::content_type, "application/x-www-form-urlencoded");
        request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);       
        request.set(boost::beast::http::field::referer, REFERER);
        request.body() = CREDENTAILS;
        request.prepare_payload();

        //sends request
        boost::beast::http::write(stream, request);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> response;

        //reads and outputs response
        boost::beast::http::read(stream, buffer, response);

        //return just the html body as requested
        const std::string responseHTML(boost::beast::buffers_to_string(response.body().data()));

        std::cout << responseHTML << std::endl;

        //cleanup
        boost::beast::error_code ec;
        stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

        //according to the examples sometimes this error can be thrown
        if (ec && ec != boost::beast::errc::not_connected)
        {
            throw boost::beast::system_error{ ec };
        }

        return EXIT_SUCCESS;
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
