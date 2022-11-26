// Let's suppose you are building a server application that receives requests from a network 
// processes them and sends back results of the processing 
//
// suppose you have the following set of classes
// do not pay attention to the fact that there is no implementation
// whenever provided it gives you an outlook of what is happening inside. 



/*
	The application should call Network::Select in a loop to get a list of network activities happened
	The Select() blocks control and sits inside till anything happens or the timeout expires, then returns the list.

	A client connection is identified by an integer handle.

	Any request (i.e. when Request::activity() returns newRequest for it) has to be processed (i.e. the Request::process has to be called)
	and the reply should be sent back to the client.

	Requests are of two types: long and short.
	Short requests do not take much time for processing. Let's suppose that we have just a single form of short requests - ping-pong.
	Long requests take considerably longer time to process.

	The method Network::sendReply is non-blocking but requires valid connection (connection handle).

	The Network implementation is considered as not thread safe.


	In the simplest way the server might look like this:
*/

/*
 This is a kind of working implementation. But it is non responsive. It hangs up while processing long request(s).

 The aim is to make the server more responsive in general and especially in processing short requests.
 I.e. short requests should be processed as soon as possible.
 It is obviously (at least to me) that a parallel working is needed to achieve the aim.

 There is no need to show how to work with network (a la FD_SET and other socket related stuff). Just stay with the set of classes provided.

 Requirements:
 1. Short requests should be handled quickly.
 2. While calling sendReply the connection parameter has to be valid. A client connection handle is valid only while the client is connected.
	The client disconnect is detected (i.e. the connection handle invalidates) inside the Network::Select call.
	Connection handles are guaranteed to be unique while they represent live connections.
	But once a client disconnects, its handle might be reused for a newly connected client.
 3. All request for live clients should be handled and reply has to be sent back to them.
 4. You may not send a reply supposed to be for one client to another client. This is the situation when connection handle is
	reused. For example, a client sends a request then disconnects and then another client connects and takes the same handle as the previous one.
	If this happens while you process the request, the connection handle associated with the request will be still valid (identifies live connection)
	but identifies different client.

 I will be looking at an architecture and a synchronization.
 The architecture is a set of classes/objects needed to achieve the aim.
 The sycnronization should be done in a way you usually do. I assume that you always try to do it up to your best abilities.
 The solution has not to be compileable or 100% syntactically correct - I will understand (I hope) what you are trying to achieve.
 Even if it will not be working at all, it will give me outlook at the way of your thinking.
 I expect up to two-three pages of code. I guess the task is not so demanding for more than that.
 Good luck.
*/

#include <map>
#include <memory>
#include "constants.h"
#include "Network.h"
#include "NetworkActivity.h"
#include "Reply.h"
#include "Request.h"
#include "RequestGenerator.h"
#include "NetworkLogsRepo.h"
#include "RequestProcessor.h"
#include "NetworkLogger.h"
#include "ConsolePrinter.h"

void main()
{
	std::cout << "This is a simulation of network activity";

	auto nw = std::make_shared<Network>();
	auto nlr = std::make_shared<NetworkLogsRepo>();
	auto tr = ThreadRepo::instance();
	auto nl = std::make_shared<NetworkLogger>(nw, nlr);
	auto cp = std::make_shared<ConsolePrinter>();
	auto rg = std::make_shared<RequestGenerator>(
		nl, 
		MIN_MSEC_WAIT_BETWEEN_REQUESTS, 
		MAX_MSEC_WAIT_BETWEEN_REQUESTS);
	auto rp = std::make_shared<RequestProcessor>(THREADS_COUNT);

	cp->addPrinterSource(nlr);
	cp->addPrinterSource(tr);
	rg->start();
	rp->start();
	cp->startPrintLogs();

	while (true)
	{
		std::list<NetworkActivity> act = nl->Select(SELECT_MSEC);

		for (auto it = act.begin(); it != act.end(); ++it)
		{
			NetworkActivity& a = *it;
			if (a.activity() == NetworkActivity::newRequest)
			{
				rp->processRequest(a, nl);
			}
		}
	}

	rg->stop();

	fflush(stdin);
	getchar();
}

