#include <catch2/catch_all.hpp>

#include <timer_manager.hpp>
#include <i2c_utils.hpp>
#include <mock.hpp>





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
    stm32::mock::Timer mt;
    std::future<bool> tim_result;
    TIM_TypeDef *timer = nullptr;
    timer = mt.init_timer(tim_result);

    
    I2C_TypeDef *i2c_handle = nullptr;
    // i2c_handle->CR1 = i2c_handle->CR1 | I2C_CR1_PE_Msk; 
    
    SECTION("Bytes sent - Slave returns ACK")
    {
        std::cout << "i2c_utils - send_byte: Slave returns ACK" << std::endl;
        // Start mock I2C peripheral in a new thread. Simulate ACK slave return status
        
        // std::future<bool> i2c_res = 
        //     std::async(std::launch::async, mock_i2c_tx_fifo_empty, i2c_handle, SlaveStatus::ACK);   
        stm32::mock::I2C mock_i2c;
        std::future<bool> tx_fifo_future;
        i2c_handle = mock_i2c.init_i2c_tx_fifo(tx_fifo_future, stm32::mock::SlaveStatus::ACK);

        // Call the SUT function
        REQUIRE(stm32::i2c::send_byte(i2c_handle, 0xFF) == stm32::i2c::Status::ACK);

        // Success! Now disable the mock periphs and allow their threads to end
        timer->CR1 = 0;
        i2c_handle->CR1 = i2c_handle->CR1 & ~I2C_CR1_PE_Msk; 

        // yield to the async threads until they return
        REQUIRE(tim_result.get());
        REQUIRE(tx_fifo_future.get());
    }

    SECTION("Bytes sent - Slave returns NACK")
    {
        std::cout << "i2c_utils - send_byte: Slave returns NACK" << std::endl;

        // Start mock I2C peripheral in a new thread. Simulate NACK slave return status
        // std::future<bool> i2c_res = 
        //     std::async(std::launch::async, mock_i2c_tx_fifo_empty, i2c_handle, SlaveStatus::NACK);  
        stm32::mock::I2C mock_i2c;
        std::future<bool> tx_fifo_future;
        i2c_handle = mock_i2c.init_i2c_tx_fifo(tx_fifo_future, stm32::mock::SlaveStatus::NACK);


        // Call the SUT function
        REQUIRE(stm32::i2c::send_byte(i2c_handle, 0xFF) == stm32::i2c::Status::NACK);

        // Success! Now disable the mock periphs and allow their threads to end
        timer->CR1 = 0;
        i2c_handle->CR1 = i2c_handle->CR1 & ~I2C_CR1_PE_Msk; 

        // make sure the mocks returned success
        REQUIRE(tim_result.get());
        REQUIRE(tx_fifo_future.get());
    }    

   

}

TEST_CASE("i2c_utils - initialise_slave_device function", "[i2c_utils]")
{
    const uint8_t EXPECTED_ADDRESS {0x45};

    // Enable timer test fixture and start it in a new thread
    stm32::mock::Timer mt;
    std::future<bool> timer_future;
    TIM_TypeDef *timer = nullptr;
    timer = mt.init_timer(timer_future);       
      
    const uint8_t INVALID_ADDRESS {0x65};

    // start test fixture thread
    // std::future<bool> i2c_res = std::async(std::launch::async, mock_i2c_start_condition_generation, i2c_handle, EXPECTED_ADDRESS); 
    stm32::mock::I2C mock_i2c;
    std::future<bool> start_condition_future;
    I2C_TypeDef *i2c_handle = nullptr;
        
    
    SECTION("PROBE: Invalid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: PROBE/Invalid Address" << std::endl;
        i2c_handle = mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, INVALID_ADDRESS, stm32::i2c::StartType::PROBE) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns false with valid address
        REQUIRE_FALSE(start_condition_future.get());

        // Confirm we enabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_AUTOEND | I2C_CR2_START)));

    }

    SECTION("PROBE: Valid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: PROBE/Valid Address" << std::endl;
        i2c_handle = mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::StartType::PROBE) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns true with valid address
        REQUIRE(start_condition_future.get());

        // Confirm we enabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_AUTOEND | I2C_CR2_START)));        
    }

    SECTION("WRITE: Invalid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: WRITE/Invalid Address" << std::endl;
        i2c_handle = mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, INVALID_ADDRESS, stm32::i2c::StartType::WRITE) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns false with valid address
        REQUIRE_FALSE(start_condition_future.get());

        // Confirm we disabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_START))); 
    }

    SECTION("WRITE: Valid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: WRITE/Valid Address" << std::endl;
        i2c_handle = mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::StartType::WRITE) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns true with valid address
        REQUIRE(start_condition_future.get());

        // Confirm we disabled AUTOEND Mode
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_START)));         
    }    

    SECTION("READ: Invalid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: READ/Invalid Address" << std::endl;
        i2c_handle = mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, INVALID_ADDRESS, stm32::i2c::StartType::READ) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());
        
        // returns false with valid address
        REQUIRE_FALSE(start_condition_future.get());

        // confirm we requested a read operation
        REQUIRE((i2c_handle->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((i2c_handle->CR2 & (I2C_CR2_START | I2C_CR2_RD_WRN)));         
    }

    SECTION("READ: Valid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: READ/Valid Address" << std::endl;
        i2c_handle = mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(i2c_handle, EXPECTED_ADDRESS, stm32::i2c::StartType::READ) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns true with valid address
        REQUIRE(start_condition_future.get());
        
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

