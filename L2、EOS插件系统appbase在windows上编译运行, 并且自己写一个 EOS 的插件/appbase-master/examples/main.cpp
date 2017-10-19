#include <appbase/application.hpp>
#include <iostream>
#include <boost/exception/diagnostic_information.hpp>
#include <eos/http_plugin/http_plugin.hpp>

struct database { };

namespace bpo = boost::program_options;

using bpo::options_description;
using bpo::variables_map;
using std::string;
using std::vector;

namespace eos {

	class chain_plugin : public appbase::plugin<chain_plugin>
	{
	public:
		APPBASE_PLUGIN_REQUIRES();

		virtual void set_program_options(options_description& cli, options_description& cfg) override
		{
			cfg.add_options()
				("readonly", "open the database in read only mode")
				("dbsize", bpo::value<uint64_t>()->default_value(8 * 1024), "Minimum size MB of database shared memory file")
				;
			cli.add_options()
				("replay", "clear chain database and replay all blocks")
				("reset", "clear chain database and block log")
				;

			std::cout << "chain_plugin options\n";
		}

		void plugin_initialize(const variables_map& options)
		{
			std::cout << "initialize chain plugin\n";
		}
		void plugin_startup() { std::cout << "starting chain plugin \n"; }
		void plugin_shutdown() { std::cout << "shutdown chain plugin \n"; }

		database& db() { return _db; }

	private:
		database _db;
	};


	class net_plugin : public appbase::plugin<net_plugin>
	{
	public:
		net_plugin() {};
		~net_plugin() {};

		APPBASE_PLUGIN_REQUIRES((chain_plugin));

		virtual void set_program_options(options_description& cli, options_description& cfg) override
		{
			cfg.add_options()
				("listen-endpoint", bpo::value<string>()->default_value("127.0.0.1:9876"), "The local IP address and port to listen for incoming connections.")
				("remote-endpoint", bpo::value< vector<string> >()->composing(), "The IP address and port of a remote peer to sync with.")
				("public-endpoint", bpo::value<string>()->default_value("0.0.0.0:9876"), "The public IP address and port that should be advertized to peers.")
				;
			std::cout << "net_plugin options\n";
		}

		void plugin_initialize(const variables_map& options)
		{
			std::cout << "initialize net plugin\n";
		}
		void plugin_startup() { std::cout << "starting net plugin \n"; }
		void plugin_shutdown() { std::cout << "shutdown net plugin \n"; }

	};

class producer_plugin : public appbase::plugin<producer_plugin>
{
public:
	producer_plugin() {}
	~producer_plugin(){}
	APPBASE_PLUGIN_REQUIRES();

	virtual void set_program_options(options_description& cli, options_description& cfg) override
	{
		cfg.add_options()
			("producer-endpoint", bpo::value<string>()->default_value("127.0.0.1:9876"), "The local IP address and port to listen for incoming connections.")
			("producer-endpoint", bpo::value< vector<string> >()->composing(), "The IP address and port of a remote peer to sync with.")
			("producer-endpoint", bpo::value<string>()->default_value("0.0.0.0:9876"), "The public IP address and port that should be advertized to peers.")
			;
		std::cout << "producer options\n";
	}

	void plugin_initialize(const variables_map& options)
	{
		std::cout << "initialize producer plugin\n";
	}
	void plugin_startup() 
	{ 
		std::cout << "starting producer plugin \n"; 
	}
	void plugin_shutdown() 
	{ 
		std::cout << "shutdown producer plugin \n"; 
	}

};

} // end of eos

using namespace appbase;
using namespace eos;

int main( int argc, char** argv ) {
	

	std::cout << "https://github.com/eosjacker/eosvs2015\n";
	std::cout << "enter...\n";

	try {
	  //test
      appbase::app().register_plugin<producer_plugin>();
	  appbase::app().register_plugin<net_plugin>();
	  appbase::app().register_plugin<chain_plugin>();

	  // eosd
	  appbase::app().register_plugin<http_plugin>();

      if( !appbase::app().initialize( argc, argv ) )
         return -1;
      appbase::app().startup();
      appbase::app().exec();

	  std::cout << "\n\nplease visit http://127.0.0.1:8888\n";

   } catch ( const boost::exception& e ) {
      std::cerr << boost::diagnostic_information(e) << "\n";
   } catch ( const std::exception& e ) {
      std::cerr << e.what() << "\n";
   } catch ( ... ) {
      std::cerr << "unknown exception\n";
   }
   std::cout << "exited cleanly\n";
   return 0;
}
