#include <catch2/catch_all.hpp>

#include <timer_manager.hpp>
#include <i2c_utils.hpp>
#include <mock.hpp>
#include <mock_fuse.hpp>
#include <filesystem>
#include <semaphore.h>
#include <thread>
#include <signal.h>

TEST_CASE("test_fuse", "[fuse]")
{
    stm32::mock::MockFuse mf;
    REQUIRE(mf.init_session());
    std::thread fuse_thread(&stm32::mock::MockFuse::start_async_session, &mf);
    // std::this_thread::sleep_for(500ms);

    // conduct some tests on the FUSE filesystem
    int ls_result = system("ls -la /tmp/fuse");
    REQUIRE(ls_result == 0);
    int cat_result = system("cat /tmp/fuse/hello");
    REQUIRE(cat_result == 0);
    
    // unmount the filesystem and force FUSE to exit loop
    int fusermount_result = system("fusermount3 -u /tmp/fuse");
    REQUIRE(fusermount_result == 0);
    fuse_thread.join();
	
}

TEST_CASE("i2c_utils - Generate stop condition")
{
    std::cout << "i2c_utils - generate_stop_condition" << std::endl;

    // mocked i2c periph
    stm32::mock::I2C mock_i2c;

    std::cout << "i2c_utils - generate_stop_condition" << std::endl;
    stm32::i2c::generate_stop_condition(mock_i2c.get_handle());
    REQUIRE((mock_i2c.get_handle()->CR2 & I2C_CR2_STOP) == I2C_CR2_STOP);
}

TEST_CASE("i2c_utils - Master sends status")
{
    // mocked i2c periph
    stm32::mock::I2C mock_i2c;

    SECTION("Master sends status - ACK")
    {
        std::cout << "i2c_utils - send_ack: ACK" << std::endl;
        stm32::i2c::send_ack(mock_i2c.get_handle());
        REQUIRE((mock_i2c.get_handle()->CR2 & I2C_CR2_NACK) != I2C_CR2_NACK);
    }

    SECTION("Master sends status - NACK")
    {
        std::cout << "i2c_utils - send_ack: NACK" << std::endl;
        stm32::i2c::send_nack(mock_i2c.get_handle());
        REQUIRE((mock_i2c.get_handle()->CR2 & I2C_CR2_NACK) == I2C_CR2_NACK);
    }
}

TEST_CASE("i2c_utils - Receive Bytes", "[i2c_utils")
{
    std::cout << "i2c_utils - receive_byte: receive_byte() returns ACK" << std::endl;

    const uint8_t test_byte {0xFF};
    
    // mock the RXDR having data available
    stm32::mock::I2C mock_i2c;
    mock_i2c.get_handle()->RXDR = test_byte;

    // read the byte from RXDR and check it matches
    uint8_t read_byte {0};
    REQUIRE(stm32::i2c::receive_byte(mock_i2c.get_handle(), read_byte) == stm32::i2c::Status::ACK);
    REQUIRE(read_byte == test_byte);
}


TEST_CASE("i2c_utils - send_bytes", "[i2c_utils]")
{
    // Enable timer test fixture and start it in a new thread
    stm32::mock::Timer mt;
    std::future<bool> tim_result;
    TIM_TypeDef *timer = nullptr;
    timer = mt.init_timer(tim_result);

    SECTION("Bytes sent - Slave returns ACK")
    {
        std::cout << "i2c_utils - send_byte: Slave returns ACK" << std::endl;
        // Start mock I2C peripheral in a new thread. Simulate ACK slave return status  
        stm32::mock::I2C mock_i2c;
        std::future<bool> tx_fifo_future;
        mock_i2c.init_i2c_tx_fifo(tx_fifo_future, stm32::mock::SlaveStatus::ACK);

        // Call the SUT function
        REQUIRE(stm32::i2c::send_byte(mock_i2c.get_handle(), 0xFF) == stm32::i2c::Status::ACK);

        // Success! Now disable the mock periphs and allow their threads to end
        timer->CR1 = 0;
        mock_i2c.get_handle()->CR1 = mock_i2c.get_handle()->CR1 & ~I2C_CR1_PE_Msk; 

        // yield to the async threads until they return
        REQUIRE(tim_result.get());
        REQUIRE(tx_fifo_future.get());
    }

    SECTION("Bytes sent - Slave returns NACK")
    {
        std::cout << "i2c_utils - send_byte: Slave returns NACK" << std::endl;

        // Start mock I2C peripheral in a new thread. Simulate NACK slave return status 
        stm32::mock::I2C mock_i2c;
        std::future<bool> tx_fifo_future;
        mock_i2c.init_i2c_tx_fifo(tx_fifo_future, stm32::mock::SlaveStatus::NACK);


        // Call the SUT function
        REQUIRE(stm32::i2c::send_byte(mock_i2c.get_handle(), 0xFF) == stm32::i2c::Status::NACK);

        // Success! Now disable the mock periphs and allow their threads to end
        timer->CR1 = 0;
        mock_i2c.get_handle()->CR1 = mock_i2c.get_handle()->CR1 & ~I2C_CR1_PE_Msk; 

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
    stm32::mock::I2C mock_i2c;
    std::future<bool> start_condition_future;
    
    SECTION("PROBE: Invalid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: PROBE/Invalid Address" << std::endl;
        mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(mock_i2c.get_handle(), INVALID_ADDRESS, stm32::i2c::StartType::PROBE) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns false with valid address
        REQUIRE_FALSE(start_condition_future.get());

        // Confirm we enabled AUTOEND Mode
        REQUIRE((mock_i2c.get_handle()->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((mock_i2c.get_handle()->CR2 & (I2C_CR2_AUTOEND | I2C_CR2_START)));

    }

    SECTION("PROBE: Valid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: PROBE/Valid Address" << std::endl;
        mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(mock_i2c.get_handle(), EXPECTED_ADDRESS, stm32::i2c::StartType::PROBE) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns true with valid address
        REQUIRE(start_condition_future.get());

        // Confirm we enabled AUTOEND Mode
        REQUIRE((mock_i2c.get_handle()->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((mock_i2c.get_handle()->CR2 & (I2C_CR2_AUTOEND | I2C_CR2_START)));        
    }

    SECTION("WRITE: Invalid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: WRITE/Invalid Address" << std::endl;
        mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(mock_i2c.get_handle(), INVALID_ADDRESS, stm32::i2c::StartType::WRITE) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns false with valid address
        REQUIRE_FALSE(start_condition_future.get());

        // Confirm we disabled AUTOEND Mode
        REQUIRE((mock_i2c.get_handle()->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((mock_i2c.get_handle()->CR2 & (I2C_CR2_START))); 
    }

    SECTION("WRITE: Valid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: WRITE/Valid Address" << std::endl;
        mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(mock_i2c.get_handle(), EXPECTED_ADDRESS, stm32::i2c::StartType::WRITE) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns true with valid address
        REQUIRE(start_condition_future.get());

        // Confirm we disabled AUTOEND Mode
        REQUIRE((mock_i2c.get_handle()->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((mock_i2c.get_handle()->CR2 & (I2C_CR2_START)));         
    }    

    SECTION("READ: Invalid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: READ/Invalid Address" << std::endl;
        mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(mock_i2c.get_handle(), INVALID_ADDRESS, stm32::i2c::StartType::READ) == stm32::i2c::Status::NACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());
        
        // returns false with valid address
        REQUIRE_FALSE(start_condition_future.get());

        // confirm we requested a read operation
        REQUIRE((mock_i2c.get_handle()->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((mock_i2c.get_handle()->CR2 & (I2C_CR2_START | I2C_CR2_RD_WRN)));         
    }

    SECTION("READ: Valid Address")
    {
        std::cout << "i2c_utils - initialise_slave_device: READ/Valid Address" << std::endl;
        mock_i2c.init_i2c_start_condition(start_condition_future, EXPECTED_ADDRESS);
        REQUIRE(stm32::i2c::initialise_slave_device(mock_i2c.get_handle(), EXPECTED_ADDRESS, stm32::i2c::StartType::READ) == stm32::i2c::Status::ACK);
        
        // SUT has returned so simulate disabling of the HW Timer
        timer->CR1 = 0;    

        REQUIRE(timer_future.get());

        // returns true with valid address
        REQUIRE(start_condition_future.get());
        
        // confirm we requested a read operation
        REQUIRE((mock_i2c.get_handle()->CR1 & (I2C_CR1_PE_Msk)));
        REQUIRE((mock_i2c.get_handle()->CR2 & (I2C_CR2_START | I2C_CR2_RD_WRN)));               
    }    

}

TEST_CASE("i2c_utils - set_numbytes", "[i2c_utils]")
{
    std::cout << "i2c_utils - set_numbytes" << std::endl;

    // set the mocked register
    stm32::mock::I2C mock_i2c;
    stm32::i2c::set_numbytes(mock_i2c.get_handle(), 2);

    // read back the value
    REQUIRE((mock_i2c.get_handle()->CR2 & (2 << I2C_CR2_NBYTES_Pos)));
}

