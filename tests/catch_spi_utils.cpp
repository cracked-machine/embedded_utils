#include <catch2/catch_all.hpp>
#include <spi_utils.hpp>
#include <mock_cmsis.hpp>
#include <catch_timer_manager.hpp>

#include <chrono>
#include <thread>
#include <iostream>
#include <future>

using namespace std::chrono_literals;

/// @brief Mock function to test stm32::spi::wait_for_txe_flag() and stm32::spi::wait_for_bsy_flag()
/// @param i2c_handle The mocked SPI peripheral
/// @return true if unit test disables the peripheral (upon test completion), false if spi_handle is null_ptr
bool mock_spi_data_register(SPI_TypeDef *spi_handle)
{

    if (spi_handle == nullptr) { return false; }

    

    // loop while peripheral is enabled
    while((spi_handle->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE)
    {
    
        // wait for test bytes to be copied into SPI FIFO
        if (spi_handle->DR != 0)
        {
            
            // exaggerate the delay for MCU to clear the BSY flag so that our SUT has chance to run coverage
            spi_handle->SR = spi_handle->SR | SPI_SR_BSY_Msk;
            std::this_thread::sleep_for(500ms);
            spi_handle->SR = spi_handle->SR & ~SPI_SR_BSY_Msk;

            // exaggerate the delay for MCU to set the TXE flag so that our SUT has chance to run coverage
            std::this_thread::sleep_for(100ms);
            spi_handle->DR = 0;
            spi_handle->SR = spi_handle->SR | SPI_SR_TXE_Msk;
    
        }
    }

    return true;
}

TEST_CASE("spi_utils - send_bytes")
{
    // Enable timer test fixture and start it in a new thread
    TIM_TypeDef *timer = new TIM_TypeDef;
    REQUIRE(stm32::TimerManager::initialise(timer));
    std::future<bool> tim_res = std::async(std::launch::async, mock_timer_count, timer);    

    // mocked SPI periph
    SPI_TypeDef *spi_handle = new SPI_TypeDef;

    // setup the periph
    std::cout << "spi_utils - enable_spi: true" << std::endl;
    stm32::spi::enable_spi(spi_handle, true);
    REQUIRE(spi_handle->CR1 & SPI_CR1_SPE_Msk);

    SECTION("Check send_byte - OK")
    {
        std::future<bool> spi_res = 
           std::async(std::launch::async, mock_spi_data_register, spi_handle);


        stm32::spi::send_byte(spi_handle, 0xFF);

        // send_byte should have returned so disable the mock periph to let the mock_spi_data_register thread end
        spi_handle->SR = 0;
        stm32::spi::enable_spi(spi_handle, false);
        REQUIRE_FALSE(spi_handle->CR1 & SPI_CR1_SPE_Msk);

        // check the mock_spi_data_register function returned true 
        REQUIRE(spi_res.get());           
    }


    // don't forget to disable the timer for each SECTION test
    timer->CR1 = 0;

}