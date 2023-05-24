## 音乐的产生：

使用定时器产生1MHzPWM波，再根据音乐频谱，使用1Mhz产生对应频率的音符。

## 消抖

```c
 if (GPIO_Pin == GPIO_PIN_11){

	  HAL_Delay(20);
	  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)==1){
```

中断里先判断中断产生，延时20ms,再判断一次中断引脚状态进行消抖、