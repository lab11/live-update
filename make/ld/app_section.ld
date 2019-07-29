    .picapps :
    {
        _picapps = .;
        KEEP(*(.picapp))
        _epicapps = .;
    } > FLASH

