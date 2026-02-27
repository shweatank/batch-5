//sample code for DMA                                                                                                                                                                                                                     #include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/completion.h>
#include <linux/slab.h>

#define DMA_BUF_SIZE 1024

struct dma_test_dev {
    struct dma_chan *chan;
    dma_addr_t src_dma;
    dma_addr_t dst_dma;
    void *src_buf;
    void *dst_buf;
    struct completion dma_complete;
};

static void dma_test_callback(void *param)
{
    struct dma_test_dev *dev = param;
    complete(&dev->dma_complete);
    pr_info("DMA transfer completed\n");
}

static int dma_test_probe(struct platform_device *pdev)
{
    struct dma_test_dev *dev;
    struct dma_slave_config cfg = {0};
    struct dma_async_tx_descriptor *tx;
    dma_cookie_t cookie;
    int ret;

    pr_info("DMA test probe\n");

    dev = devm_kzalloc(&pdev->dev, sizeof(*dev), GFP_KERNEL);
    if (!dev)
        return -ENOMEM;

    init_completion(&dev->dma_complete);

    /* Request DMA channel */
    dev->chan = dma_request_chan(&pdev->dev, "memcpy");
    if (IS_ERR(dev->chan)) {
        pr_err("Failed to request DMA channel\n");
        return PTR_ERR(dev->chan);
    }

    /* Allocate DMA buffers */
    dev->src_buf = dma_alloc_coherent(&pdev->dev, DMA_BUF_SIZE,
                                      &dev->src_dma, GFP_KERNEL);
    dev->dst_buf = dma_alloc_coherent(&pdev->dev, DMA_BUF_SIZE,
                                      &dev->dst_dma, GFP_KERNEL);

    if (!dev->src_buf || !dev->dst_buf) {
        pr_err("DMA buffer allocation failed\n");
        ret = -ENOMEM;
        goto err_buf;
    }

    /* Fill source buffer */
    memset(dev->src_buf, 0xA5, DMA_BUF_SIZE);
    memset(dev->dst_buf, 0x00, DMA_BUF_SIZE);

    /* Prepare DMA memcpy */
    tx = dmaengine_prep_dma_memcpy(
            dev->chan,
            dev->dst_dma,
            dev->src_dma,
            DMA_BUF_SIZE,
            DMA_CTRL_ACK | DMA_PREP_INTERRUPT);

    if (!tx) {
        pr_err("Failed to prepare DMA memcpy\n");
        ret = -EIO;
        goto err_buf;
    }

    tx->callback = dma_test_callback;
    tx->callback_param = dev;

    cookie = dmaengine_submit(tx);
    ret = dma_submit_error(cookie);
    if (ret) {
        pr_err("DMA submit error\n");
        goto err_buf;
    }

    dma_async_issue_pending(dev->chan);

    /* Wait for completion */
    wait_for_completion_timeout(&dev->dma_complete, msecs_to_jiffies(3000));

    pr_info("DMA memcpy verification: %s\n",
        memcmp(dev->src_buf, dev->dst_buf, DMA_BUF_SIZE) ? "FAIL" : "PASS");

    platform_set_drvdata(pdev, dev);
    return 0;

err_buf:
    if (dev->src_buf)
        dma_free_coherent(&pdev->dev, DMA_BUF_SIZE,
                          dev->src_buf, dev->src_dma);
    if (dev->dst_buf)
        dma_free_coherent(&pdev->dev, DMA_BUF_SIZE,
                          dev->dst_buf, dev->dst_dma);

    dma_release_channel(dev->chan);
    return ret;
}

static int dma_test_remove(struct platform_device *pdev)
{
    struct dma_test_dev *dev = platform_get_drvdata(pdev);

    dma_free_coherent(&pdev->dev, DMA_BUF_SIZE,
                      dev->src_buf, dev->src_dma);
    dma_free_coherent(&pdev->dev, DMA_BUF_SIZE,
                      dev->dst_buf, dev->dst_dma);

    dma_release_channel(dev->chan);
    pr_info("DMA test driver removed\n");
    return 0;
}

static const struct of_device_id dma_test_of_match[] = {
    { .compatible = "techdhaba,dma-test" },
    { }
};
MODULE_DEVICE_TABLE(of, dma_test_of_match);

static struct platform_driver dma_test_driver = {
    .probe  = dma_test_probe,
    .remove = dma_test_remove,
    .driver = {
        .name = "dma_test",
        .of_match_table = dma_test_of_match,
    },
};

module_platform_driver(dma_test_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TechDhaba");
MODULE_DESCRIPTION("Sample Linux DMA Engine Driver");
