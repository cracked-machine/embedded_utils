#include <catch2/catch_all.hpp>

#include <timer_manager.hpp>
#include <i2c_utils.hpp>

const uint8_t EXPECTED_ADDRESS {0x45};



TEST_CASE("i2c_utils - Generate stop condition")
{
    std::cout << "i2c_utils - generate_stop_condition" << std::endl;

    // mocked i2c periph
    I2C_TypeDef *i2c_handle = new I2C_TypeDef;

    std::cout << "i2c_utils - generate_stop_condition" << std::endl;
    stm32::i2c::generate_stop_condition(i2c_handle);
    REQUIRE((i2c_handle->CR2 & I2C_CR2_STOP) == I2C_CR2_STOP);
}

TEST_CASE("i2c_utils - Master sends status")
{
    // mocked i2c periph
    I2C_TypeDef *i2c_handle = new I2C_TypeDef;

    SECTION("Master sends status - ACK")
    {
        std::cout << "i2c_utils - send_ack: ACK" << std::endl;
        stm32::i2c::send_ack(i2c_handle);
        REQUIRE((i2c_handle->CR2 & I2C_CR2_NACK) != I2C_CR2_NACK);
    }

    SECTION("Master sends status - NACK")
    {
        std::cout << "i2c_utils - send_ack: NACK" << std::endl;
        stm32::i2c::send_nack(i2c_handle);
        REQUIRE((i2c_handle->CR2 & I2C_CR2_NACK) == I2C_CR2_NACK);
    }
}

TEST_CASE("i2c_utils - Receive Bytes", "[i2c_utils")
{
    std::cout << "i2c_utils - receive_byte: receive_byte() returns ACK" << std::endl;

    const uint8_t test_byte {0xFF};
    
    // mock the RXDR having data available
    I2C_TypeDef *i2c_handle = new I2C_TypeDef;
    i2c_handle->RXDR = test_byte;

    // read the byte from RXDR and check it matches
    uint8_t read_byte {0};
    REQUIRE(stm32::i2c::receive_byte(i2c_handle, read_byte) == stm32::i2c::Status::ACK);
    REQUIRE(read_byte == test_byte);
}


TEST_CASE("i2c_utils - send_bytes", "[i2c_utils]")
{
    // Enable timer test fixture and start it in a new thread
    TIM_TypeDef *timer = new TIM_TypeDef;
    REQUIRE(stm32::TimerManager::initialise(timer));
    std::future<bool> tim_res = std::async(std::launch::async, mock_timer_count, timer); 

    // Enable mock I2C peripheral
    I2C_TypeDef *i2c_handle = new I2C_TypeDef;
    i2c_handle->CR1 = i2c_handle->CR1 | I2C_CR1_PE_Msk; 
    
    SECTION("Bytes sent - Slave returns ACK")
    {
        std::cout << "i2c_utils - send_byte: Slave returns ACK" << std::endl;
        // Start mock I2C peripheral in a new thread. Simulate ACK slave return status
        std::future<bool> i2c_res = 
            std::async(std::launch::async, mock_i2c_tx_fifo_empty, i2c_handle, SlaveStatus::ACK);   

        // Call the SUT function
        REQUIRE(stm32::i2c::send_byte(i2c_handle, 0xFF) == stm32::i2c::Status::ACK);

        // Success! Now disable the mock periphs and allow their threads to end
        timer->CR1 = 0;
        i2c_handle->CR1 = i2c_handle->CR1 & ~I2C_CR1_PE_Msk; 

        // make sure the mocks returned success
        REQUIRE(tim_res.get());
        REQUIRE(i2c_res.get());
    }

    SECTION("Bytes sent - Slave returns NACK")
    {
        std::cout << "i2c_utils - send_byte: Slave returns NACK" << std::endl;

        // Start mock I2C peripheral in a new thread. Simulate NACK slave return status
        std::future<bool> i2c_res = 
            std::async(std::launch::async, mock_i2c_tx_fifo_empty, i2c_handle, SlaveStatus::NACK);  

        // Call the SUT function
        REQUIRE(stm32::i2c::send_byte(i2c_handle, 0xFF) == stm32::i2c::Status::NACK);

        // Success! Now disable the mock periphs and allow their threads to end
        timer->CR1 = 0;
        i2c_handle->CR1 = i2c_handle->CR1 & ~I2C_CR1_PE_Msk; 

        // make sure the mocks returned success
        REQUIRE(tim_res.get());
        REQUIRE(i2c_res.get());
    }    

}

TEST_CASE("i2c_utils - initialise_slave_device function", "[i2c_utils]")
{
    // Enable timer test fixture and start it in a new thread
    TIM_TypeDef *timer = new TIM_TypeDef;
    REQUIRE(stm32::TimerManager::initialise(timer));
    std::future<bool> tim_res = std::async(std::launch::async, mock_timer_count, timer); 
       
      
    uint8_t i2c_addr {0x65};
    I2C_TypeDef *i2c_handle = new I2C_TypeDef;
    // enable the periph
	i2c_handle->CR1 = i2c_handle->CR1 | I2C_CR1_PE_Msk; 
    // start test fixture thread
    std::future<bool> i2c_res = std::async(std::launch::async, mock_i2c_start_condition_generation, i2c_handle, EXPECTED_ADDRESS); 
    
    SECTION("PROBE: Invalid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: PROBE/Invalid Address" << std::endl;

        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, i2c_addr, stm32::i2c::StartType::PROBE) == stm32::i2c::Status::NACK);
        
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
        std::cout << "i2c_utils - initialise_slave_device: PROBE/Valid Address" << std::endl;

        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::StartType::PROBE) == stm32::i2c::Status::ACK);
        
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
        std::cout << "i2c_utils - initialise_slave_device: WRITE/Invalid Address" << std::endl;

        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, i2c_addr, stm32::i2c::StartType::WRITE) == stm32::i2c::Status::NACK);
        
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
        std::cout << "i2c_utils - initialise_slave_device: WRITE/Valid Address" << std::endl;

        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::StartType::WRITE) == stm32::i2c::Status::ACK);
        
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
        std::cout << "i2c_utils - initialise_slave_device: READ/Invalid Address" << std::endl;

        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, i2c_addr, stm32::i2c::StartType::READ) == stm32::i2c::Status::NACK);
        
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
        std::cout << "i2c_utils - initialise_slave_device: READ/Valid Address" << std::endl;

        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::StartType::READ) == stm32::i2c::Status::ACK);
        
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
    std::cout << "i2c_utils - set_numbytes" << std::endl;

    I2C_TypeDef *i2c_handle = new I2C_TypeDef;
    // set the mocked register
    stm32::i2c::set_numbytes(i2c_handle, 2);
    // read back the value
    REQUIRE((i2c_handle->CR2 & (2 << I2C_CR2_NBYTES_Pos)));
}

