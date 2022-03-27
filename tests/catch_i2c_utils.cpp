#include <catch2/catch_all.hpp>
#include <catch_common.hpp>
#include <timer_manager.hpp>
#include <i2c_utils.hpp>

const uint8_t EXPECTED_ADDRESS {0x45};

bool testfixture_i2c_periph(I2C_TypeDef *i2c_handle)
{
    if (i2c_handle == nullptr)
    {
        return false;
    }
    // loop while peripheral is enabled
    while((i2c_handle->CR1 & I2C_CR1_PE_Msk) == I2C_CR1_PE_Msk)
    {
        // start condition was generated
        if (i2c_handle->CR2 & I2C_CR2_START_Msk == I2C_CR2_START_Msk)
        {
            
            if ((i2c_handle->CR2 & I2C_CR2_SADD_Msk) == EXPECTED_ADDRESS)
            {
                // expected address was used
                return true;
            }
            else
            {
                // UNexpected address was used
                i2c_handle->ISR = i2c_handle->ISR | I2C_ISR_NACKF_Msk;
                return false;
            }
        }
        else if (i2c_handle->CR2 & I2C_CR2_STOP_Msk == I2C_CR2_STOP_Msk)
        {

        }
    }

    return true;
}


TEST_CASE("i2c_utils - send_addr function", "[i2c_utils]")
{
    // enable timer test fixture
    TIM_TypeDef *timer = new TIM_TypeDef;
    REQUIRE(stm32::TimerManager::initialise(timer));
    // start test fixture thread
    std::future<bool> tim_res = std::async(std::launch::async, testfixture_timer_sim, timer); 
       
      
    uint8_t i2c_addr {0x65};
    I2C_TypeDef *i2c_handle = new I2C_TypeDef;
    // enable the periph
	i2c_handle->CR1 = i2c_handle->CR1 | I2C_CR1_PE_Msk; 
    // start test fixture thread
    std::future<bool> i2c_res = std::async(std::launch::async, testfixture_i2c_periph, i2c_handle); 
    
    SECTION("PROBE: Invalid Address")
    {
        REQUIRE(stm32::i2c::send_addr(i2c_handle, i2c_addr, stm32::i2c::MsgType::PROBE) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(tim_res.get());

        // returns false with valid address
        REQUIRE_FALSE(i2c_res.get());

        // Confirm we enabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_AUTOEND | I2C_CR2_START)));

    }

    SECTION("PROBE: Valid Address")
    {
        REQUIRE(stm32::i2c::send_addr(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::MsgType::PROBE) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(tim_res.get());

        // returns true with valid address
        REQUIRE(i2c_res.get());

        // Confirm we enabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_AUTOEND | I2C_CR2_START)));        
    }

    SECTION("WRITE: Invalid Address")
    {
        REQUIRE(stm32::i2c::send_addr(i2c_handle, i2c_addr, stm32::i2c::MsgType::WRITE) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(tim_res.get());

        // returns false with valid address
        REQUIRE_FALSE(i2c_res.get());

        // Confirm we disabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_START))); 
    }

    SECTION("WRITE: Valid Address")
    {
        REQUIRE(stm32::i2c::send_addr(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::MsgType::WRITE) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(tim_res.get());

        // returns true with valid address
        REQUIRE(i2c_res.get());

        // Confirm we disabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_START)));         
    }    

    SECTION("READ: Invalid Address")
    {
        REQUIRE(stm32::i2c::send_addr(i2c_handle, i2c_addr, stm32::i2c::MsgType::READ) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(tim_res.get());
        
        // returns false with valid address
        REQUIRE_FALSE(i2c_res.get());

        // confirm we requested a read operation
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_START | I2C_CR2_RD_WRN)));         
    }

    SECTION("READ: Valid Address")
    {
        REQUIRE(stm32::i2c::send_addr(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::MsgType::READ) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(tim_res.get());

        // returns true with valid address
        REQUIRE(i2c_res.get());
        
        // confirm we requested a read operation
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_START | I2C_CR2_RD_WRN)));               
    }    

}

TEST_CASE("i2c_utils - set_numbytes", "[i2c_utils]")
{
    I2C_TypeDef *i2c_handle = new I2C_TypeDef;
    // set the mocked register
    stm32::i2c::set_numbytes(i2c_handle, 2);
    // read back the value
    REQUIRE((i2c_handle->CR2 & (2 << I2C_CR2_NBYTES_Pos)));
}