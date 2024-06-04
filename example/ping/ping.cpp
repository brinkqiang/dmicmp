#include <iostream>
#include "dmicmp.hpp"
#include "CLI/CLI11.hpp"
int ping(const std::string& url, int count)
{
    std::string resolved;
    std::string address = url;
    try {
        if (!icmplib::IPAddress::IsCorrect(address, icmplib::IPAddress::Type::Unknown)) {
            resolved = address; address = icmplib::IPAddress(address);
        }
    } catch (...) {
        std::cout << "Ping request could not find host " << address << ". Please check the name and try again." << std::endl;
        return 1;
    }

    int ret = EXIT_SUCCESS;
    std::cout << "Pinging " << (resolved.empty() ? address : resolved + " [" + address + "]")
              << " with " << ICMPLIB_PING_DATA_SIZE << " bytes of data:" << std::endl;

	for (int i=0; i < count; ++i)
	{
		auto result = icmplib::Ping(address, ICMPLIB_TIMEOUT_1S);
		switch (result.response) {
		case icmplib::PingResponseType::Failure:
			std::cout << "Network error." << std::endl;
			ret = EXIT_FAILURE;
			break;
		case icmplib::PingResponseType::Timeout:
			std::cout << "Request timed out." << std::endl;
			break;
		default:
			std::cout << "Reply from " << static_cast<std::string>(result.address) << ": ";
			switch (result.response) {
			case icmplib::PingResponseType::Success:
				std::cout << "time=" << result.delay;
				if (result.address.GetType() != icmplib::IPAddress::Type::IPv6) {
					std::cout << " TTL=" << static_cast<unsigned>(result.ttl);
				}
				break;
			case icmplib::PingResponseType::Unreachable:
				std::cout << "Destination unreachable.";
				break;
			case icmplib::PingResponseType::TimeExceeded:
				std::cout << "Time exceeded.";
				break;
			default:
				std::cout << "Response not supported.";
			}
			std::cout << std::endl;
		}
	}

    return ret;
}
int main(int argc, char *argv[])
{
    CLI::App app("ping");

    std::string address = "8.8.8.8";
	int count = 4;
    app.add_option("address", address, "Hostname or IP address to ping");
	app.add_option("-t", count, "Ping 指定运行次数，直到停止");

	CLI11_PARSE(app, argc, argv);

    return ping(address, count);
}
