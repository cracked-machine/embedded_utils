#include <catch2/catch_all.hpp>

#include <timer_manager.hpp>
#include <usart_utils.hpp>
#include <mock.hpp>


TEST_CASE("usart_utils", "[usart_utils]")
{
    // setup mock TIMER periph (used by usart_utils)
    stm32::mock::Timer mt;
    std::future<bool> tim_res;
    TIM_TypeDef *timer = nullptr;
    timer = mt.mock_init_timer(tim_res);

    
    // setup mock USART periph
    USART_TypeDef *usart_handle = nullptr;
    REQUIRE_FALSE(stm32::usart::enable_usart(usart_handle));
    REQUIRE_FALSE(stm32::usart::transmit_byte(usart_handle, 0));
    REQUIRE_FALSE(stm32::usart::wait_for_bsy_flag(usart_handle));
    REQUIRE_FALSE(stm32::usart::wait_for_tc_flag(usart_handle));
    
    usart_handle = new USART_TypeDef;
    REQUIRE(stm32::usart::enable_usart(usart_handle));
    REQUIRE(usart_handle->CR1 & USART_CR1_UE_Msk);

    SECTION("usart_utils - transmit_byte")
    {      
        std::cout << "usart_utils - transmit_byte" << std::endl;
        uint32_t test_byte {0xA0};
        usart_handle->ISR = usart_handle->ISR | USART_ISR_TC_Msk;
        usart_handle->ISR = usart_handle->ISR & ~USART_ISR_BUSY_Msk;
        REQUIRE(stm32::usart::transmit_byte(usart_handle, test_byte));
        REQUIRE(usart_handle->TDR == test_byte);
    }

    SECTION("usart_utils - wait_for_bsy_flag: USART_ISR_BUSY not set")
    {
        std::cout << "usart_utils - wait_for_bsy_flag: USART_ISR_BUSY not set" << std::endl;
        usart_handle->ISR = usart_handle->ISR & ~USART_ISR_BUSY;
        REQUIRE(stm32::usart::wait_for_bsy_flag(usart_handle));
    }

    SECTION("usart_utils - wait_for_bsy_flag: USART_ISR_BUSY is set")
    {
        std::cout << "usart_utils - wait_for_bsy_flag: USART_ISR_BUSY is set" << std::endl;
        usart_handle->ISR = usart_handle->ISR | USART_ISR_BUSY;
        REQUIRE_FALSE(stm32::usart::wait_for_bsy_flag(usart_handle));
    }

    SECTION("usart_utils - wait_for_tc_flag: USART_ISR_TC not set")
    {
        std::cout << "usart_utils - wait_for_tc_flag: USART_ISR_TC not set" << std::endl;
        usart_handle->ISR = usart_handle->ISR & ~USART_ISR_TC;
        REQUIRE_FALSE(stm32::usart::wait_for_tc_flag(usart_handle));
    }

    SECTION("usart_utils - wait_for_tc_flag: USART_ISR_TC is set")
    {
        std::cout << "usart_utils - wait_for_tc_flag: USART_ISR_TC is set" << std::endl;
        usart_handle->ISR = usart_handle->ISR | USART_ISR_TC;
        REQUIRE(stm32::usart::wait_for_tc_flag(usart_handle));
    }    

    // tear down
    timer->CR1 = 0;
    REQUIRE(tim_res.get());
}

