/dts-v1/;
/plugin/;

/ {
    compatible = "brcm,bcm2711";

    fragment@0 {
        target-path = "/";
        __overlay__ {
            hcsr04@0 {
                compatible = "mycompany,hcsr04";

                trig-gpios = <&gpio 23 0>;
                echo-gpios = <&gpio 24 0>;

                led-near-gpios = <&gpio 17 0>;  // GPIO17
                led-far-gpios  = <&gpio 27 0>;  // GPIO27
            };
        };
    };
};
