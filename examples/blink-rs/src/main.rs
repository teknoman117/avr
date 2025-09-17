#![no_std]
#![no_main]

#![feature(abi_avr_interrupt)]

extern crate attiny_hal as hal;
extern crate avr_device as device;

extern crate panic_halt;

use avr_device::interrupt::{self, Mutex};

use core::cell::RefCell;
use core::ops::DerefMut;

use hal::pac;
use hal::prelude::*;

type Delay = hal::delay::Delay<hal::clock::MHz8>;

//static G_USI: Mutex<RefCell<Option<pac::USI>>> =
//    Mutex::new(RefCell::new(None));

static G_PORTB: Mutex<RefCell<Option<pac::PORTB>>> =
    Mutex::new(RefCell::new(None));

#[device::entry]
fn main() -> ! {
    let peripherals = pac::Peripherals::take().unwrap();

    let mut delay = Delay::new();
    //let mut portb = peripherals.PORTB.split();
    //let mut pin0 = portb.pb0.into_output(&mut portb.ddr);
    //let _ = portb.pb1.into_output(&mut portb.ddr);

    peripherals.PORTB.ddrb.modify(|_r, w| {
        w.pb0().bit(true)
         .pb1().bit(true)
    });

    interrupt::free(|cs| {
        //G_USI.borrow(cs).replace(Some(peripherals.USI));
        G_PORTB.borrow(cs).replace(Some(peripherals.PORTB));
    });

    loop {
        interrupt::free(|cs| {
            let port = G_PORTB.borrow(cs).borrow();
            port.as_ref().unwrap().pinb.write(|w| {
                w.pb0().bit(true)
            });
        });
        //pin0.toggle().void_unwrap();
        delay.delay_ms(500u16);
    }
}

#[device::interrupt(attiny85)]
fn TIMER0_OVF() {
    interrupt::free(|cs| {
        let port = G_PORTB.borrow(cs).borrow();
        port.as_ref().unwrap().pinb.write(|w| {
            w.pb1().bit(false)
        });
    });
}
