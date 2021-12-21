// MIT License

// Copyright (c) 2021 Chris Sutton

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// @brief common functions for using with STM32 LL SPI
// @tparam SPI_HANDLE 
template<typename SPI_HANDLE>
class ll_spi_utils
{

public: 

    ll_spi_utils() = default;


    #if defined(USE_SSD1306_LL_DRIVER)
        // @brief Check and retry (with timeout) the SPIx_SR TXE register.
        // @param delay_ms The timeout
        // @return true if TX FIFO is empty, false if TX FIFO is full
        inline bool check_txe_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_ms = 1)
        {
            if (spi_handle == nullptr)
            {
                return false;
            }

            // The TXE flag is set when transmission TXFIFO has enough space to store data to send.
            //if (LL_SPI_IsActiveFlag_TXE(spi_handle) == 0)
            if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
            {
                // give TX FIFO a chance to clear before checking again
                LL_mDelay(delay_ms);
                //if (LL_SPI_IsActiveFlag_TXE(spi_handle) == 0)
                if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
                { 
                    return false;
                }
                
            }
            return true;
        }        

        // @brief Check and retry (with timeout) the SPIx_SR BSY register.
        // @param delay_ms The timeout
        // @return true if SPI bus is busy, false if SPI bus is not busy.
        inline bool check_bsy_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_ms = 1)
        {
            if (spi_handle == nullptr)
            {
                return false;
            }
            // When BSY is set, it indicates that a data transfer is in progress on the SPI
            // if (LL_SPI_IsActiveFlag_BSY(SPI_HANDLE)) 
            if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
            {
                // give SPI bus a chance to finish sending data before checking again
                LL_mDelay(delay_ms);
                //if (LL_SPI_IsActiveFlag_BSY(SPI_HANDLE)) 
                if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
                { 
                    return false;
                }
            }    
            return true; 
        }        
    #endif    

};


    // #if defined(USE_SSD1306_LL_DRIVER)
    // bool ll_spi_utils::check_txe_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_ms)
    // {
    //     if (spi_handle == nullptr)
    //     {
    //         return false;
    //     }

    //     // The TXE flag is set when transmission TXFIFO has enough space to store data to send.
    //     //if (LL_SPI_IsActiveFlag_TXE(spi_handle) == 0)
    //     if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
    //     {
    //         // give TX FIFO a chance to clear before checking again
    //         LL_mDelay(delay_ms);
    //         //if (LL_SPI_IsActiveFlag_TXE(spi_handle) == 0)
    //         if ((spi_handle->SR & SPI_SR_TXE) != (SPI_SR_TXE))
    //         { 
    //             return false;
    //         }
            
    //     }
    //     return true;
    // }

    // // check SPI bus has finished sending the last byte
    // bool ll_spi_utils::check_bsy_flag_status(const SPI_HANDLE *spi_handle, uint32_t delay_ms)
    // {
    //     if (spi_handle == nullptr)
    //     {
    //         return false;
    //     }
    //     // When BSY is set, it indicates that a data transfer is in progress on the SPI
    //     // if (LL_SPI_IsActiveFlag_BSY(SPI_HANDLE)) 
    //     if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
    //     {
    //         // give SPI bus a chance to finish sending data before checking again
    //         LL_mDelay(delay_ms);
    //         //if (LL_SPI_IsActiveFlag_BSY(SPI_HANDLE)) 
    //         if ((spi_handle->SR & SPI_SR_BSY) == (SPI_SR_BSY))
    //         { 
    //             return false;
    //         }
    //     }    
    //     return true; 
    // }
    // #endif    