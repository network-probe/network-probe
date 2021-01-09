#ifndef SRC_UTIL_CONFIG_H_
#define SRC_UTIL_CONFIG_H_

#include <string>
#include <vector>
#include <mutex>

using namespace std;

typedef enum PROTOCOL_TYPE_T
{
    NP_PROTO_UNKNOWN,
    NP_PROTO_NTP,
    NP_PROTO_DHCP
}PROTOCOL_TYPE;

class ProtocolOption
{
public:
    ProtocolOption();
    virtual ~ProtocolOption();

private:
    PROTOCOL_TYPE mProtoType;
    std::string mAddress;
    int mPort;

public:
    void SetProtocol(PROTOCOL_TYPE proto);
    void SetAddress(std::string address);
    void SetPort(int port);

    PROTOCOL_TYPE GetProtocol() const;
    std::string GetAddress() const;
    int GetPort() const;
};

class OptionArgs
{
public:
    virtual ~OptionArgs();
    static OptionArgs *GetInstance();

private:
    static std::mutex mLock;
    static OptionArgs *mpInstance;
    std::vector<ProtocolOption *> mProtos;
    OptionArgs();

private:
    ProtocolOption *getOptionPtr(PROTOCOL_TYPE proto);

public:
    int ParseOptions(int _argc, char *_argv[]);
    vector<ProtocolOption *> GetOptions();
    void PrintHelp();
};

#endif