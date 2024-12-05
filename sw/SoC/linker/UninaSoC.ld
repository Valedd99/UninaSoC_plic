/* This file is auto-generated with create_linker_script.py */

/* Memory blocks */
MEMORY
{
	BRAM (xrw) : ORIGIN = 0x0,  LENGTH = 0x20000
	PERIPHERALS (rw) : ORIGIN = 0x20000,  LENGTH = 0x28000
}

/* Sections */
SECTIONS
{
	_slave_BRAM_base = 0x0;
	_slave_BRAM_end = 0x20000;
	_slave_UART_base = 0x10000;
	_slave_UART_end = 0x12000;
	_slave_GPIO_out_base = 0x20000;
	_slave_GPIO_out_end = 0x22000;
	_slave_GPIO_in_base = 0x30000;
	_slave_GPIO_in_end = 0x32000;
	_slave_TIM_base = 0x40000;
	_slave_TIM_end = 0x42000;
	_slave_PLIC_base = 0x50000;
	_slave_PLIC_end = 0x70000;
}

