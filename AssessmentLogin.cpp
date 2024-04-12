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

constexpr auto VERSION = 11;
constexpr auto WEBSITE = "testphp.vulnweb.com";
constexpr auto PORT = "80";
constexpr auto TARGET = "/login.php";
constexpr auto CREDENTAILS = "test";

int main(int argc, char** argv)
{
    try
    {
        boost::asio::io_context context;

        boost::asio::ip::tcp::resolver resolver(context);

        boost::beast::tcp_stream stream(context);

        //const auto results = resolver.resolve(WEBSITE, PORT);

        const auto endpoint = stream.connect(boost::asio::ip::tcp::resolver(context).resolve(WEBSITE, PORT));

        boost::beast::http::request<boost::beast::http::string_body> request{ boost::beast::http::verb::post, TARGET, VERSION };
        request.set(boost::beast::http::field::host, WEBSITE);
        request.set(boost::beast::http::field::connection, "keep-alive");
        request.set(boost::beast::http::field::content_length, "20");
        request.set(boost::beast::http::field::cache_control, "max-age=0");
        request.set(boost::beast::http::field::upgrade, "1"); 
        request.set(boost::beast::http::field::origin, "http://testphp.vulnweb.com");
        request.set(boost::beast::http::field::content_type, "application/x-www-form-urlencoded");
        request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);       
        request.set(boost::beast::http::field::accept,"text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
        request.set(boost::beast::http::field::referer, "http://testphp.vulnweb.com/login.php");
        request.set(boost::beast::http::field::accept_encoding, "gzip, deflate");
        request.set(boost::beast::http::field::accept_language, "en-US,en;q=0.9");
        request.set(boost::beast::http::field::cookie, "login=test%2Ftest");
        request.body() = "uname=test&pass=test";
        request.prepare_payload();

        std::cout << request << std::endl;

        boost::beast::http::write(stream, request);

        boost::beast::flat_buffer buffer;
        boost::beast::http::response<boost::beast::http::dynamic_body> res;

        boost::beast::http::read(stream, buffer, res);
        std::cout << res << std::endl;

        //std::string decompressedString;

        /*boost::iostreams::filtering_ostream decompressingStream;
        decompressingStream.push(boost::iostreams::gzip_decompressor());
        decompressingStream.push(boost::iostreams::back_inserter(decompressedString));
        decompressingStream << res;
        boost::iostreams::close(decompressingStream);

        std::cout << decompressedString << std::endl;*/

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
