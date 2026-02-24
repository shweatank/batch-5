#include "iaq_internal.h"

void iaq_parse_dts(void)
{
    struct device_node *np;

    np = of_find_node_by_name(NULL, "iaq_monitor");
    if (!np) {
        pr_info("IAQ: DTS node not found, using defaults\n");
        iaq->threshold = 1000;
        return;
    }

    of_property_read_u32(np, "threshold",
                         &iaq->threshold);

    pr_info("IAQ: Threshold from DTS = %u\n",
            iaq->threshold);
}

