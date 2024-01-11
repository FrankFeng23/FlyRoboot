#include "adc.h"

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma1;
ADC_ChannelConfTypeDef adc1_config;

uint16_t ADC_DMA_BUFFER[4] = {0,0,0,0};

void adc_config(void)
{
    //DMA模式配置
    DMA_CLK_ENABLE();

    hdma1.Instance = DMA1_Channel1;
    //数据传输方向外设--内存
    hdma1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    //内存数据对其，半字对齐，32位处理器
    hdma1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    //内存地址自增使能
    hdma1.Init.MemInc = DMA_MINC_ENABLE;
    //DMA模式循环
    hdma1.Init.Mode = DMA_CIRCULAR;
    //外设数据对其，半字对齐
    hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    //外设地址自增
    hdma1.Init.PeriphInc = DMA_PINC_DISABLE;
    //优先级
    hdma1.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma1);

    //链接ADC与DMA
    __HAL_LINKDMA(&hadc1,DMA_Handle,hdma1);

    //ADC配置
    hadc1.Instance = ADC1;
    //扫描模式
    hadc1.Init.ScanConvMode = ENABLE;
    //连续模式
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    //数据对齐
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //启动方式
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.NbrOfConversion = 4;
    hadc1.Init.NbrOfDiscConversion = 0;
    HAL_ADC_Init(&hadc1);

    adc1_config.Channel = ADC_CHANNEL_10;
    adc1_config.Rank = ADC_REGULAR_RANK_1;
    adc1_config.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    HAL_ADC_ConfigChannel(&hadc1,&adc1_config);

    adc1_config.Channel = ADC_CHANNEL_11;
    adc1_config.Rank = ADC_REGULAR_RANK_2;
    HAL_ADC_ConfigChannel(&hadc1,&adc1_config);

    adc1_config.Channel = ADC_CHANNEL_12;
    adc1_config.Rank = ADC_REGULAR_RANK_3;
    HAL_ADC_ConfigChannel(&hadc1,&adc1_config);

    adc1_config.Channel = ADC_CHANNEL_13;
    adc1_config.Rank = ADC_REGULAR_RANK_4;
    HAL_ADC_ConfigChannel(&hadc1,&adc1_config);

    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_DMA_BUFFER,4);
}



void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if(hadc==&hadc1)
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        ADC_GPIO_CLK_ENABLE();
        ADC_CLK_ENABLE();


        GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        GPIO_InitStructure.Pin = YAH_PIN | THR_PIN | PITCH_PIN | ROLL_PIN ;

        HAL_GPIO_Init(ADC_GPIO_PORT,&GPIO_InitStructure);
    }
} 

