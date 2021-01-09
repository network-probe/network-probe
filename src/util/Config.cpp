#include "Config.h"
#include <getopt.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "../util/Debug.h"

using namespace std;

ProtocolOption::ProtocolOption() : mProtoType(NP_PROTO_UNKNOWN), mPort(-1)
{
}
ProtocolOption::~ProtocolOption()
{
}


void ProtocolOption::SetProtocol(PROTOCOL_TYPE proto)
{
    mProtoType = proto;
}

void ProtocolOption::SetAddress(std::string address)
{
    mAddress = address;
}

void ProtocolOption::SetPort(int port)
{
    mPort = port;
}

PROTOCOL_TYPE ProtocolOption::GetProtocol() const
{
    return mProtoType;
}

std::string ProtocolOption::GetAddress() const
{
    return mAddress;
}

int ProtocolOption::GetPort() const
{
    return mPort;
}


std::mutex OptionArgs::mLock;
OptionArgs *OptionArgs::mpInstance = NULL;

OptionArgs::OptionArgs()
{
}

OptionArgs::~OptionArgs()
{
}

OptionArgs* OptionArgs::GetInstance()
{
    if(mpInstance == NULL)
    {
        std::lock_guard<std::mutex> lock_guard(mLock);
        if(mpInstance == NULL)
        {
            mpInstance = new OptionArgs();
        }
    }
    
    return mpInstance;
}

ProtocolOption *OptionArgs::getOptionPtr(PROTOCOL_TYPE proto)
{
    ProtocolOption *p = nullptr;
    for(vector<ProtocolOption *>::iterator iter = mProtos.begin(); iter != mProtos.end(); ++iter)
    {
        if((*iter)->GetProtocol() == proto)
        {
            p = (*iter);
            break;
        }
        else if((*iter)->GetProtocol() == NP_PROTO_UNKNOWN)
        {
            (*iter)->SetProtocol(proto);
            p = (*iter);
            break;
        }
    }

    if(p == nullptr)
    {
        ProtocolOption *p = new ProtocolOption;
        p->SetProtocol(proto);
        mProtos.push_back(p);
        NP_LOGGER(Logger::NP_LOG_LEVEL_TRACE, "Create Proto=[%p]\n", p);
    }    

    return p;
}

#define SET_OPTION(X, Y, Z)   do { getOptionPtr(X);   getOptionPtr(X)->Y(Z); } while(0)
int OptionArgs::ParseOptions(int _argc, char *_argv[])
{
    int ret = 0;
    PROTOCOL_TYPE proto_type = NP_PROTO_UNKNOWN;
    while (1) 
    {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = 
        {
            {"add",     required_argument, 0,  1 },
            {"append",  no_argument,       0,  0 },
            {"delete",  required_argument, 0,  0 },
            {"verbose", no_argument,       0,  0 },
            {"create",  required_argument, 0, 'c'},
            {"file",    required_argument, 0,  0 },
            {0,         0,                 0,  0 }
        };

        int digit_optind = 0;
        int c = getopt_long(_argc, _argv, "bt:a:p:012h", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) 
        {
            case 0:
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case '0':
            case '1':
            case '2':
                if (digit_optind != 0 && digit_optind != this_option_optind)
                    printf("digits occur in two different argv-elements.\n");
                digit_optind = this_option_optind;
                printf("option %c\n", c);
                break;

            case 'b':
                printf("option b\n");
                break;

            case 't':
                {
                    if(!strcasecmp("ntp", optarg))
                    {
                        getOptionPtr(NP_PROTO_NTP);
                        proto_type = NP_PROTO_NTP;
                    }
                    else if(!strcasecmp("dhcp", optarg))
                    {
                        getOptionPtr(NP_PROTO_DHCP);
                        proto_type = NP_PROTO_DHCP;
                    }
                    else
                    {
                        printf("option c with value '%s'\n", optarg);
                    }
                }                
                break;

            case 'a':
                /*
                    CheckTodo
                    SegFault...
                */
                // getOptionPtr(proto_type);
                // getOptionPtr(proto_type)->SetAddress(optarg);
                SET_OPTION(proto_type, SetAddress, optarg);
                break;

            case 'p':
                // getOptionPtr(proto_type)->SetPort(atoi(optarg));
                SET_OPTION(proto_type, SetPort, atoi(optarg));
                break;    

            case 'h':
                PrintHelp();
                break;

            case '?':
                break;

            default:
                break;
        }
    }

    if (optind < _argc) 
    {
        PrintHelp();
        printf("non-option ARGV-elements: ");
        while (optind < _argc)
        {
            printf("%s ", _argv[optind++]);
        }                
        printf("\n");
    }


    return ret;
}

vector<ProtocolOption *> OptionArgs::GetOptions()
{
    return mProtos;
}

void OptionArgs::PrintHelp()
{
    stringstream ss;
    
    ss << "Usage: NetworkProbe [-abcdefg] [-t ProtocolType] [-a Address] [-p Port]\n";
    ss << "\t\t    [-c code]\n";
    ss << "Examples: \n";
    ss << " ntp: NetworkProbe -t ntp -a 216.239.35.0 -p 123\n";
    ss << " dhcp: NetworkProbe -t dhcp -p 68\n";

    cout << ss.str();
}