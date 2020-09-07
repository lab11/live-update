#include <sys/printk.h>
#include <kernel.h>

// #include "ladder.h"
#include "drivers/gpio.h"

#define PLC_SCAN_TIME 10

typedef signed short SWORD;     // from platform
typedef unsigned char BOOL;     // from platform

/* ldmicro */

/* Define EXTERN_EVERYTHING in ladder.h if you want all symbols extern.
   This could be useful to implement `magic variables,' so that for
   example when you write to the ladder variable duty_cycle, your PLC
   runtime can look at the C variable U_duty_cycle and use that to set
   the PWM duty cycle on the micro. That way you can add support for
   peripherals that LDmicro doesn't know about. */
#ifdef EXTERN_EVERYTHING
#define STATIC 
#else
#define STATIC static
#endif

/* Define NO_PROTOTYPES if you don't want LDmicro to provide prototypes for
   all the I/O functions (Read_U_xxx, Write_U_xxx) that you must provide.
   If you define this then you must provide your own prototypes for these
   functions in ladder.h, or provide definitions (e.g. as inlines or macros)
   for them in ladder.h. */
#ifdef NO_PROTOTYPES
#define PROTO(x)
#else
#define PROTO(x) x
#endif

/* U_xxx symbols correspond to user-defined names. There is such a symbol
   for every internal relay, variable, timer, and so on in the ladder
   program. I_xxx symbols are internally generated. */
STATIC BOOL I_b_mcr = 0;
#define Read_I_b_mcr() I_b_mcr
#define Write_I_b_mcr(x) I_b_mcr = x
STATIC BOOL I_b_rung_top = 0;
#define Read_I_b_rung_top() I_b_rung_top
#define Write_I_b_rung_top(x) I_b_rung_top = x
STATIC BOOL U_b_Rosc = 0;
#define Read_U_b_Rosc() U_b_Rosc
#define Write_U_b_Rosc(x) U_b_Rosc = x
STATIC SWORD U_i_Tosc_on = 0;
STATIC BOOL I_b_Tosc_off_antiglitch = 0;
#define Read_I_b_Tosc_off_antiglitch() I_b_Tosc_off_antiglitch
#define Write_I_b_Tosc_off_antiglitch(x) I_b_Tosc_off_antiglitch = x
STATIC SWORD U_i_Tosc_off = 0;

/* You provide this function. */
PROTO(extern BOOL Read_U_b_Xbutton(void);)

STATIC BOOL I_b_parOut_0000 = 0;
#define Read_I_b_parOut_0000() I_b_parOut_0000
#define Write_I_b_parOut_0000(x) I_b_parOut_0000 = x
STATIC BOOL I_b_parThis_0000 = 0;
#define Read_I_b_parThis_0000() I_b_parThis_0000
#define Write_I_b_parThis_0000(x) I_b_parThis_0000 = x
STATIC BOOL I_b_oneShot_0000 = 0;
#define Read_I_b_oneShot_0000() I_b_oneShot_0000
#define Write_I_b_oneShot_0000(x) I_b_oneShot_0000 = x
STATIC SWORD U_i_Cstate = 0;
STATIC BOOL U_b_Rbon = 0;
#define Read_U_b_Rbon() U_b_Rbon
#define Write_U_b_Rbon(x) U_b_Rbon = x
STATIC SWORD U_i_Tkill_delay = 0;
STATIC BOOL U_b_Rkill = 0;
#define Read_U_b_Rkill() U_b_Rkill
#define Write_U_b_Rkill(x) U_b_Rkill = x
STATIC BOOL I_b_parOut_0001 = 0;
#define Read_I_b_parOut_0001() I_b_parOut_0001
#define Write_I_b_parOut_0001(x) I_b_parOut_0001 = x
STATIC BOOL I_b_parThis_0001 = 0;
#define Read_I_b_parThis_0001() I_b_parThis_0001
#define Write_I_b_parThis_0001(x) I_b_parThis_0001 = x
STATIC BOOL U_b_Rhigh = 0;
#define Read_U_b_Rhigh() U_b_Rhigh
#define Write_U_b_Rhigh(x) U_b_Rhigh = x
STATIC BOOL I_b_parOut_0002 = 0;
#define Read_I_b_parOut_0002() I_b_parOut_0002
#define Write_I_b_parOut_0002(x) I_b_parOut_0002 = x
STATIC BOOL I_b_parThis_0002 = 0;
#define Read_I_b_parThis_0002() I_b_parThis_0002
#define Write_I_b_parThis_0002(x) I_b_parThis_0002 = x

/* You provide these functions. */
PROTO(BOOL Read_U_b_Ytx(void);)
PROTO(void Write_U_b_Ytx(BOOL v);)

STATIC BOOL I_b_parOut_0003 = 0;
#define Read_I_b_parOut_0003() I_b_parOut_0003
#define Write_I_b_parOut_0003(x) I_b_parOut_0003 = x
STATIC BOOL I_b_parThis_0003 = 0;
#define Read_I_b_parThis_0003() I_b_parThis_0003
#define Write_I_b_parThis_0003(x) I_b_parThis_0003 = x
STATIC SWORD I_i_scratch2 = 0;

/* You provide these functions. */
PROTO(BOOL Read_U_b_Yled(void);)
PROTO(void Write_U_b_Yled(BOOL v);)



/* Call this function once per PLC cycle. You are responsible for calling
   it at the interval that you specified in the MCU configuration when you
   generated this code. */
void PlcCycle(void)
{
    Write_I_b_mcr(1);
    
    /* start rung 1 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_Rosc()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_I_b_rung_top()) {
        if(U_i_Tosc_on < 24) {
            U_i_Tosc_on++;
            Write_I_b_rung_top(0);
        }
    } else {
        U_i_Tosc_on = 0;
    }
    
    if(!Read_I_b_Tosc_off_antiglitch()) {
        U_i_Tosc_off = 24;
    }
    Write_I_b_Tosc_off_antiglitch(1);
    if(!Read_I_b_rung_top()) {
        if(U_i_Tosc_off < 24) {
            U_i_Tosc_off++;
            Write_I_b_rung_top(1);
        }
    } else {
        U_i_Tosc_off = 0;
    }
    
    if(Read_I_b_rung_top()) {
        Write_U_b_Rosc(0);
    } else {
        Write_U_b_Rosc(1);
    }
    
    /* ] finish series */
    
    /* start rung 2 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_Xbutton()) {
        Write_I_b_rung_top(0);
    }
    
    /* start parallel [ */
    Write_I_b_parOut_0000(0);
    Write_I_b_parThis_0000(Read_I_b_rung_top());
    if(Read_I_b_parThis_0000()) {
        if(!Read_I_b_oneShot_0000()) {
            U_i_Cstate++;
            if(U_i_Cstate < 3) {
            } else {
                U_i_Cstate = 0;
            }
        }
    }
    Write_I_b_oneShot_0000(Read_I_b_parThis_0000());
    
    if(Read_I_b_parThis_0000()) {
        Write_I_b_parOut_0000(1);
    }
    Write_I_b_parThis_0000(Read_I_b_rung_top());
    Write_U_b_Rbon(Read_I_b_parThis_0000());
    
    if(Read_I_b_parThis_0000()) {
        Write_I_b_parOut_0000(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0000());
    /* ] finish parallel */
    /* ] finish series */
    
    /* start rung 3 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(Read_U_b_Rbon()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_I_b_rung_top()) {
        if(U_i_Tkill_delay < 49) {
            U_i_Tkill_delay++;
            Write_I_b_rung_top(0);
        }
    } else {
        U_i_Tkill_delay = 0;
    }
    
    Write_U_b_Rkill(Read_I_b_rung_top());
    
    /* ] finish series */
    
    /* start rung 4 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    /* start parallel [ */
    Write_I_b_parOut_0001(0);
    Write_I_b_parThis_0001(Read_I_b_rung_top());
    if(!Read_U_b_Rbon()) {
        Write_I_b_parThis_0001(0);
    }
    
    if(Read_I_b_parThis_0001()) {
        Write_I_b_parOut_0001(1);
    }
    Write_I_b_parThis_0001(Read_I_b_rung_top());
    if(!Read_U_b_Rhigh()) {
        Write_I_b_parThis_0001(0);
    }
    
    if(Read_I_b_parThis_0001()) {
        Write_I_b_parOut_0001(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0001());
    /* ] finish parallel */
    if(Read_U_b_Rkill()) {
        Write_I_b_rung_top(0);
    }
    
    /* start parallel [ */
    Write_I_b_parOut_0002(0);
    Write_I_b_parThis_0002(Read_I_b_rung_top());
    Write_U_b_Rhigh(Read_I_b_parThis_0002());
    
    if(Read_I_b_parThis_0002()) {
        Write_I_b_parOut_0002(1);
    }
    Write_I_b_parThis_0002(Read_I_b_rung_top());
    Write_U_b_Ytx(Read_I_b_parThis_0002());
    
    if(Read_I_b_parThis_0002()) {
        Write_I_b_parOut_0002(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0002());
    /* ] finish parallel */
    /* ] finish series */
    
    /* start rung 5 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    /* start parallel [ */
    Write_I_b_parOut_0003(0);
    Write_I_b_parThis_0003(Read_I_b_rung_top());
    I_i_scratch2 = 1;
    if(U_i_Cstate == I_i_scratch2) {
    } else {
        Write_I_b_parThis_0003(0);
    }
    
    if(Read_I_b_parThis_0003()) {
        Write_I_b_parOut_0003(1);
    }
    Write_I_b_parThis_0003(Read_I_b_rung_top());
    /* start series [ */
    I_i_scratch2 = 2;
    if(U_i_Cstate == I_i_scratch2) {
    } else {
        Write_I_b_parThis_0003(0);
    }
    
    if(!Read_U_b_Rosc()) {
        Write_I_b_parThis_0003(0);
    }
    
    /* ] finish series */
    if(Read_I_b_parThis_0003()) {
        Write_I_b_parOut_0003(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0003());
    /* ] finish parallel */
    Write_U_b_Yled(Read_I_b_rung_top());
    
    /* ] finish series */
}

/********************** Ladder Files Start Here ***********************/

#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 10
#define BUTTON1 6
#define BUTTON2 7
#define SWITCH1 8
#define SWITCH2 9

#define USE_LED 1                           /* true if the program uses on-board LEDs. false otherwise */

#define NUM_PINS 1                          /* Number of pins to use as outputs */
#define NUM_INPUTS 1                        /* Number of pins to use as inputs */

//static uint8_t output_pins[] = {10};          /* List pins to use as outputs as {0, 1, ...} */
//static uint8_t input_pins[] = {11};         /* List pins to use as inputs as {0, 1, ...} */

/* from ladder */

#define NUM_LED_PINS 3
//static uint8_t LED_pins[] __attribute__((section(".rodata"))) = {2, 3, 4, 5};
// super hacky change this later
// this is a useful thing to tell the compiler where a variable should be stored

/* platform */

static uint32_t gpio_in_table = 0x00000000;
static uint32_t gpio_out_table = 0x00000000;
static gpio_port_pins_t output_mask = 0x00000000;

struct device *gpio_dev;

void enable_pin_inputs(uint8_t* pins, uint8_t num_pins) {
    if (!num_pins) { return; }
    printk("Enabling Pin Inputs...\n");
    gpio_dev = device_get_binding("GPIO_0");

    for (uint8_t i = 0; i < num_pins; i++) {
        printk("Enabling PIN%d...\n", pins[i]);
        int ret = gpio_pin_configure(gpio_dev, pins[i], (GPIO_INPUT));
        if (ret) {
            printk("Could not enable PIN%d with error code %d\n", pins[i], ret);
        }
    }
}

void enable_pin_outputs(uint8_t* pins, uint8_t num_pins) {
    printk("Enabling Pin Outputs...\n");
    gpio_dev = device_get_binding("GPIO_0");
    // uint32_t out = 0;
    for (uint8_t i = 0; i < num_pins; i++) {
        printk("Enabling PIN%d...\n", pins[i]);
        int ret = gpio_pin_configure(gpio_dev, pins[i], GPIO_OUTPUT_INACTIVE);
        if (ret) {
            printk("gpio_pin_configure failed with error code: %d\n", ret);
            return;
        }
        output_mask |= (1 << pins[i]);
    }
    // For Musca
    // tfm_gpio_enable_outputs(out);

    return;
}

void enable_pin_output(uint8_t pin) {
    if (gpio_pin_configure(gpio_dev, pin, GPIO_OUTPUT_INACTIVE) != 0) {
        printk("Error! Could not enable PIN %d!\n", pin);
    }
    output_mask |= (1 << pin);
    return;
}

void read_in_table(void) {
    if (gpio_port_get_raw(gpio_dev, &gpio_in_table) != 0) {
        printk("Error! read_in_table() failed!\n");
    }
    //printk("%x\n", gpio_in_table);
    return;
}

void write_out_table(void) {
    if (gpio_port_set_masked_raw(gpio_dev, output_mask, gpio_out_table) != 0) {
        printk("Error! write_out_table() failed!\n");
    }
    //printk("%x\n\n", gpio_out_table);
    return;
}

void set_pin(uint8_t pin) {
    if (pin > 31) { return; }
    gpio_out_table |= (1 << pin);
    return;
}

void clr_pin(uint8_t pin) {
    if (pin > 31) { return; }
    gpio_out_table &= ~(1 << pin);
    return;
}

void write_pin(uint8_t pin, BOOL val) {
    if (val) {
        set_pin(pin);
    } else {
        clr_pin(pin);
    }
    return;
}

BOOL read_pin(uint8_t pin) {
    if (pin > 15) { return pin; }
    return (BOOL) (gpio_in_table >> pin) % 2;
}

void LED_on(uint8_t led) {
    set_pin(led);
    return;
}

void LED_off(uint8_t led) {
    clr_pin(led);
    return;
}

BOOL read_LED(uint8_t led) {
    return (BOOL) read_pin(led);
}

void write_LED(uint8_t led, BOOL v) {
    if (v == 0) {
        LED_off(led);
    } else {
        LED_on(led);
    }
}

BOOL read_button(uint8_t pin) {
    return read_pin(pin) ? 0 : 1;
}


/* ladder */

// static uint32_t time_old = 0;
// static uint32_t time_cnt = 0;

void plc_callback(struct k_timer *t) {  
    // printk("Timer Fired!!\n\n");
    // time_old = time_cnt;
    // time_cnt = k_uptime_get_32();
    // printk("elapsed time since last callback: %d\n", time_cnt - time_old);
    // printk("Reading...\n");
    read_in_table();
    // printk("Executing...\n");
    PlcCycle();
    // printk("Writing...\n");
    write_out_table();
    // printk("Scan Complete.\n");
}

void init_plc(void) {
    printk("Initializing PLC...\n");
    gpio_dev = device_get_binding("GPIO_0");

    gpio_pin_configure(gpio_dev, 11, (GPIO_INPUT));
    printk("Inputs Initialized!\n");

    read_in_table();

    gpio_pin_configure(gpio_dev, 2, GPIO_OUTPUT_INACTIVE);
    output_mask |= (1 << 2);
    gpio_pin_configure(gpio_dev, 3, GPIO_OUTPUT_INACTIVE);
    output_mask |= (1 << 3);
    gpio_pin_configure(gpio_dev, 4, GPIO_OUTPUT_INACTIVE);
    output_mask |= (1 << 4);
    gpio_pin_configure(gpio_dev, 5, GPIO_OUTPUT_INACTIVE);
    output_mask |= (1 << 5);
    gpio_pin_configure(gpio_dev, 10, GPIO_OUTPUT_INACTIVE);
    output_mask |= (1 << 10);

    write_out_table();
    // printk("plc initialized\n");
}

/**************** Define Auto-Generated Functions Below ****************/

BOOL Read_U_b_Xbutton(void) {
    return (BOOL) read_button(11);
}

BOOL Read_U_b_Yled(void) {
    return (BOOL) read_LED(LED1);
}

void Write_U_b_Yled(BOOL v) {
    write_LED(LED1, v);
}

BOOL Read_U_b_Ytx(void) {
    return (BOOL) read_LED(10);
}

void Write_U_b_Ytx(BOOL v) {
    write_LED(10, v);
}

/********************** End Ladder Files ***********************/


struct k_timer scan_timer;

void main(void) {
    printk("Starting Program...\n\n");
	init_plc();
    printk("\n\n\nINIT_PLC FINISHED!!!!!\n\n\n");
    k_timer_init(&scan_timer, plc_callback, NULL);

    // gpio veneer tests
    // tfm_gpio_write_all(0xBEEF);
    // printk("wrote %x to pins\n", 0xBEEF);
    // u32_t data = tfm_gpio_read_all();
    // printk("read all pins: %x\n", data);
    // tfm_gpio_enable_outputs(0xFFFF);
    // printk("enabled outputs on all pins\n");
    // u8_t single_pin_data = tfm_gpio_read(2);
    // printk("single PIN 2 value: %x\n", single_pin_data);

	k_timer_start(&scan_timer, K_MSEC(PLC_SCAN_TIME), K_MSEC(PLC_SCAN_TIME));
}
