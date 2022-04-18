#include <catch2/catch_all.hpp>

#include <timer_manager.hpp>
#include <spi_utils.hpp>
#include <mock.hpp>

TEST_CASE("spi_utils - send_bytes")
{
    std::cout << "spi_utils - send_bytes" << std::endl;

    // Enable timer test fixture and start it in a new thread
    stm32::mock::Timer mt;
    std::future<bool> tim_res;
    TIM_TypeDef *timer = nullptr;
    timer = mt.mock_init_timer(tim_res); 

    // mocked SPI periph (check for nullptr)
    SPI_TypeDef *spi_handle = nullptr;
    REQUIRE_FALSE(stm32::spi::enable_spi(spi_handle, true));
    REQUIRE_FALSE(stm32::spi::send_byte(spi_handle, 0x00));
    spi_handle = new SPI_TypeDef;

    // setup the periph

    stm32::spi::enable_spi(spi_handle, true);
    REQUIRE(spi_handle->CR1 & SPI_CR1_SPE_Msk);


    std::future<bool> spi_res = 
        std::async(std::launch::async, mock_spi_data_register, spi_handle);

    stm32::spi::send_byte(spi_handle, 0xFF);

    // send_byte should have returned so disable the mock periph to let the mock_spi_data_register thread end
    spi_handle->SR = 0;
    stm32::spi::enable_spi(spi_handle, false);
    REQUIRE_FALSE(spi_handle->CR1 & SPI_CR1_SPE_Msk);

    // check the mock_spi_data_register function returned true 
    REQUIRE(spi_res.get());           

    // don't forget to disable the timer for each SECTION test
    timer->CR1 = 0;

}

TEST_CASE("spi_utils - wait_for_bsy_flag")
{
    std::cout << "spi_utils - wait_for_bsy_flag" << std::endl;

    // Enable timer test fixture and start it in a new thread
    stm32::mock::Timer mt;
    std::future<bool> tim_res;
    TIM_TypeDef *timer = nullptr;
    timer = mt.mock_init_timer(tim_res);

    SECTION("wait_for_bsy_flag - SPI_SR_BSY not set")
    {
        SPI_TypeDef * spi_handle = new SPI_TypeDef;
        spi_handle->SR = spi_handle->SR & ~SPI_SR_BSY;
        REQUIRE(stm32::spi::wait_for_bsy_flag(spi_handle));
    }

    SECTION("wait_for_bsy_flag - SPI_SR_BSY is set")
    {
        SPI_TypeDef * spi_handle = new SPI_TypeDef;
        spi_handle->SR = spi_handle->SR | SPI_SR_BSY;
        REQUIRE_FALSE(stm32::spi::wait_for_bsy_flag(spi_handle));
    }

    // don't forget to disable the timer for each SECTION test
    timer->CR1 = 0;

    // check the mock_spi_data_register function returned true 
    REQUIRE(tim_res.get()); 

}

TEST_CASE("spi_utils - wait_for_txe_flag")
{
    std::cout << "spi_utils - wait_for_txe_flag" << std::endl;

    // Enable timer test fixture and start it in a new thread
    stm32::mock::Timer mt;
    std::future<bool> tim_res;
    TIM_TypeDef *timer = nullptr;
    timer = mt.mock_init_timer(tim_res);

    SECTION("wait_for_txe_flag - SPI_SR_TXE not set")
    {
        SPI_TypeDef * spi_handle = new SPI_TypeDef;
        spi_handle->SR = spi_handle->SR & ~SPI_SR_TXE;
        REQUIRE_FALSE(stm32::spi::wait_for_txe_flag(spi_handle));
    }

    SECTION("wait_for_txe_flag - SPI_SR_TXE is set")
    {
        SPI_TypeDef * spi_handle = new SPI_TypeDef;
        spi_handle->SR = spi_handle->SR | SPI_SR_TXE;
        REQUIRE(stm32::spi::wait_for_txe_flag(spi_handle));
    }

    // don't forget to disable the timer for each SECTION test
    timer->CR1 = 0;

    // check the mock_spi_data_register function returned true 
    REQUIRE(tim_res.get()); 

}

TEST_CASE("spi_utils - set_prescaler")
{
    std::cout << "spi_utils - set_prescaler" << std::endl;

    // mocked SPI periph
    SPI_TypeDef *spi_handle = nullptr;
    REQUIRE_FALSE(stm32::spi::set_prescaler(spi_handle, (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)));
    spi_handle = new SPI_TypeDef;

    REQUIRE(stm32::spi::set_prescaler(spi_handle, SPI_CR1_BR_0));
    REQUIRE(spi_handle->CR1 == 8);    

    REQUIRE(stm32::spi::set_prescaler(spi_handle, SPI_CR1_BR_1));
    REQUIRE(spi_handle->CR1 == 16);  

    REQUIRE(stm32::spi::set_prescaler(spi_handle, SPI_CR1_BR_2));
    REQUIRE(spi_handle->CR1 == 32);      

    REQUIRE(stm32::spi::set_prescaler(spi_handle, (SPI_CR1_BR_1 | SPI_CR1_BR_0)));
    REQUIRE(spi_handle->CR1 == 24);

    REQUIRE(stm32::spi::set_prescaler(spi_handle, (SPI_CR1_BR_2 | SPI_CR1_BR_1)));
    REQUIRE(spi_handle->CR1 == 48);        
    
    REQUIRE(stm32::spi::set_prescaler(spi_handle, (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0)));
    REQUIRE(spi_handle->CR1 == 56);    
}