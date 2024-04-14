# Login Simulation example

##General information

From looking at a trace of the traffic, the login happens when the form is submitted and a POST HTTP 1.1 request is sent to /userprofile.php page. The /userprofile.php page will then either send the HTML if the credentials are "test, test" or send back just the data "you must login"

My solution replicates the valid username and password use case as that is what returns the HTML, and uses boost-beast to create the HTTP request, send it and read the response and boost-asio for networking.

The solution itself: 
1. sets up the connection to the site
1. creates the http request and sets the fields closely resembling a real request and attaches the username and password to the payload
1. sends the request
1. reads the response and prints the data part of it containing the HTML

##
Initial Note

Note that the CMAKE file provided expects output relevant to Windows so this solution as is expects Windows  

##Setup

###Required

1. git

1. Cmake installed (https://cmake.org/download/) and after installing the path to the bin folder needs to be set as a new enviornment variable for "Path" under the "System Variables"

1. vcpkg installed
* clone "git clone https://github.com/microsoft/vcpkg.git" wherever you want vcpkg to be installed
* run bootstrap-vcpkg.bat in the cloned repository
* the path for the root folder of the vcpkg installation needs to be set as a new enviornment variable for "Path" under the "System Variables"

1. In my case since I am using visual studio 2022 as my generator for cmake

###Installation

1. open a terminal where you want to clone the repository
1. clone my solution's repository "git clone https://github.com/michaelcalleja95/AssessmentLogin.git"
1. cd to the root of the repository
1. "vcpkg install". in the ternminal. this will install all the 3rd pary libraries to the vcpkg_installed folder
1. Depending on which generator is preferred to be used, using Cmake generate the necessary files
1. run "cmake -G "Visual Studio 17 2022" 
1. Open the generated solution and build in either debug and release


