#include <stdint.h>
#include "stm32f4xx.h"

void init_leds(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;

    GPIOD->MODER &= ~(GPIO_MODER_MODER12_Msk | GPIO_MODER_MODER13_Msk | GPIO_MODER_MODER14_Msk | GPIO_MODER_MODER15_Msk);
    GPIOD->MODER |= (1U << GPIO_MODER_MODE12_Pos) | (1U << GPIO_MODER_MODE13_Pos) | (1U << GPIO_MODER_MODE14_Pos) | (1U << GPIO_MODER_MODE15_Pos);
}

void init_i2c_lsm303agr(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // enable clock for GPIOB

    // PB6 is I2C1 SCL, PB9 is I2C1 SDA 
    GPIOB->MODER &= ~(GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE9_Msk); 
    GPIOB->MODER |= (2U << GPIO_MODER_MODE6_Pos) | (2U << GPIO_MODER_MODE9_Pos); // set pins to alternate function (AF)

    GPIOB->OTYPER |= (1U << GPIO_OTYPER_OT6_Pos) | (1U << GPIO_OTYPER_OT9_Pos); // set to open drain since I2C only pulls signals down

    GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6_Msk | GPIO_OSPEEDR_OSPEED9_Msk); 
    GPIOB->OSPEEDR |= (2U << GPIO_OSPEEDR_OSPEED6_Pos) | (2U << GPIO_OSPEEDR_OSPEED9_Pos); // set to fast speed since I2C is fast

    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6_Msk | GPIO_PUPDR_PUPD9_Msk);
    GPIOB->PUPDR |= (1U << GPIO_PUPDR_PUPD6_Pos) | (1U << GPIO_PUPDR_PUPD9_Pos); // set to pull up since I2C only pulls down so pull up is needed

    GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk);
    GPIOB->AFR[0] |= (4U << GPIO_AFRL_AFSEL6_Pos); // set to correct AF for I2C1 SCL
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL9_Msk);
    GPIOB->AFR[1] |= (4U << GPIO_AFRH_AFSEL9_Pos); // set to correct AF for I2C1 SDA

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // enable clock for I2C1

    I2C1->CR1 &= ~I2C_CR1_PE;    // disable I2C during config
    I2C1->CR2 = 16;              // APB1 freq in MHz
    I2C1->CCR = 80;              // 100 kHz I2C (CCR = Fpclk1 / (2*Fscl))
    I2C1->TRISE = 17;            // max rise time (TRISE = Fpclk1 + 1)
    I2C1->CR1 |= I2C_CR1_PE;     // enable I2C
}


static inline void spin(volatile uint32_t count) {
  while (count--) (void) 0;
}



int main(void)
{
    
    init_leds();
    init_i2c_lsm303agr();

    while(1)
    {


    }

    return 0;
}