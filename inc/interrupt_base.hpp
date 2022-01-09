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

#ifndef __INTERRUPT_BASE_HPP__
#define __INTERRUPT_BASE_HPP__

namespace stm32::isr
{

// https://www.embedded.com/interrupts-in-c/
class InterruptBase
{
public:
    InterruptBase() = default;

    enum class InterruptVectors
    {
        exti4_15_irqhandler = 0,
        isr_count = 1,
    };

    static void Register(InterruptVectors interrupt_number, InterruptBase* intThisPtr);
    static void EXTI4_15_IRQHandler(void);

    virtual void ISR(void) = 0;

    // https://stackoverflow.com/questions/52135456/why-i-cant-call-a-static-member-variable-in-an-static-member-function-like-this
    static inline InterruptBase* ISRVectorTable[ static_cast<int>(InterruptVectors::isr_count) ];
};




} // namespace stm32::isr

#endif // __INTERRUPT_BASE_HPP__