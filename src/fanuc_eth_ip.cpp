#include <EIPScanner/MessageRouter.h>
#include <EIPScanner/utils/Logger.h>
#include <EIPScanner/utils/Buffer.h>
#include <iostream>

using eipScanner::SessionInfo;
using eipScanner::MessageRouter;
using namespace eipScanner::cip;
using namespace eipScanner::utils;

class fanuc_eth_ip
{
private:
    std::string ip_;
    std::shared_ptr< eipScanner::SessionInfo > si_;
    std::shared_ptr< eipScanner::MessageRouter > messageRouter_ = std::make_shared< eipScanner::MessageRouter >();

public:
    fanuc_eth_ip(std::string ip);
    ~fanuc_eth_ip();
    std::vector<double> get_current_joint_pos();
    void write_register(int val, int reg = 1);
    void write_pos_register(std::vector<double> j_vals, int reg = 1);
};

fanuc_eth_ip::fanuc_eth_ip(std::string ip)
{
    ip_=ip;
    si_.reset( new eipScanner::SessionInfo( ip_, 0xAF12 ) );

}

fanuc_eth_ip::~fanuc_eth_ip()
{
}
std::vector<double> fanuc_eth_ip::get_current_joint_pos()
{
    auto response = messageRouter_->sendRequest(si_, ServiceCodes::GET_ATTRIBUTE_SINGLE, EPath(0x7E, 0x01, 0x01));

    std::vector<uint8_t> myList = response.getData();
    int numArrays = myList.size() / 4;

    std::vector<float> full;

    for (int j = 0; j < numArrays; ++j) 
    {
        unsigned char byteArray[4];
        for (int i = 0; i < 4; ++i) {
            byteArray[i] = myList[j * 4 + i];
        }
        float floatValue;
        std::memcpy(&floatValue, byteArray, sizeof(float));
        full.push_back(floatValue);
    }

    std::vector<double> j_val(full.begin()+1, full.begin() + 7);
    return j_val;
}

// WRITEs value on a REGULAR REGISTER 
void fanuc_eth_ip::write_register(int val, int reg)
{
    Buffer buffer;
    CipDint arg = val;
    buffer << arg;
    std::cout << buffer.data().size() << std::endl;

    auto response3 = messageRouter_->sendRequest(si_, ServiceCodes::SET_ATTRIBUTE_SINGLE, EPath(0x6B, 0x01, reg), buffer.data());

}


// WRITEs value on a POSITION REGISTER 
void fanuc_eth_ip::write_pos_register(std::vector<double> j_vals, int reg)
{
    
    Buffer buffer;
    buffer  << CipReal(  0.0  )
            << CipReal( static_cast<float>( j_vals[0] ) )
            << CipReal( static_cast<float>( j_vals[1] ) )
            << CipReal( static_cast<float>( j_vals[2] ) )
            << CipReal( static_cast<float>( j_vals[3] ) )
            << CipReal( static_cast<float>( j_vals[4] ) )
            << CipReal( static_cast<float>( j_vals[5] ) )
            << CipReal(  0.0  )
            << CipReal(  0.0  )
            << CipReal(  0.0  ) ;
    
    std::cout << buffer.data().size() << std::endl;

    auto response2 = messageRouter_->sendRequest(si_, 0X10, EPath(0x7C, 0x01, reg), buffer.data());
}


int main() {
    
    // creates the driver object
    fanuc_eth_ip driver("10.11.31.111");

    // reads joint position
    std::vector<double> j_pos = driver.get_current_joint_pos();

    for(auto j:j_pos)
        Logger(LogLevel::ERROR) << "jpos: " << j;


    // writes on a register
    driver.write_register(55,7);

    // writes on a position register
    driver.write_pos_register(j_pos);


    return 0;
}



