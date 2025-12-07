#include <stdint.h>
#include "stm32f4xx.h"

void init_gpio(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    GPIOD->MODER &= ~(GPIO_MODER_MODER12_Msk | GPIO_MODER_MODER13_Msk | GPIO_MODER_MODER14_Msk | GPIO_MODER_MODER15_Msk);
    GPIOD->MODER |= (1U << GPIO_MODER_MODE12_Pos) | (1U << GPIO_MODER_MODE13_Pos) | (1U << GPIO_MODER_MODE14_Pos) | (1U << GPIO_MODER_MODE15_Pos);
}

static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}

int main(void)
{
    
    init_gpio();

    while(1)
    {
        GPIOD->BSRR = GPIO_BSRR_BS12 | GPIO_BSRR_BS13 | GPIO_BSRR_BS14 | GPIO_BSRR_BS15;
        spin(999999);
        GPIOD->BSRR = GPIO_BSRR_BR12 | GPIO_BSRR_BR13 | GPIO_BSRR_BR14 | GPIO_BSRR_BR15;
        spin(999999);
    }

    return 0;
}