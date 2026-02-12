/dts-v1/;
/plugin/;

/ {
    compatible = "brcm,bcm2711";

    fragment@0 {
        target-path = "/";

        __overlay__ {
            td_gpio_blink: td_gpio_blink@0 {
                compatible = "techdhaba,td-gpio-blink";

                /* BCM numbering, active-high */
                in-gpios  = <&gpio 17 0>;
                out-gpios = <&gpio 27 0>;

                status = "okay";
            };
        };
    };
};
