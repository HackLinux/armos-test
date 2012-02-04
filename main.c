#define OMAP34XX_L4_PER                 0x49000000

/* UART */
#define OMAP34XX_UART3                  (OMAP34XX_L4_PER+0x20000)

#define CFG_NS16550_CLK         48000000
#define CONFIG_BAUDRATE         115200

struct NS16550 {
	unsigned char rbr;		/* 0 */
	int pad1:24;
	unsigned char ier;		/* 1 */
	int pad2:24;
	unsigned char fcr;		/* 2 */
	int pad3:24;
	unsigned char lcr;		/* 3 */
	int pad4:24;
	unsigned char mcr;		/* 4 */
	int pad5:24;
	unsigned char lsr;		/* 5 */
	int pad6:24;
	unsigned char msr;		/* 6 */
	int pad7:24;
	unsigned char scr;		/* 7 */
	int pad8:24;
} __attribute__ ((packed));
typedef volatile struct NS16550 *NS16550_t;

static NS16550_t console = (NS16550_t) OMAP34XX_UART3;

#define LCR_BKSE	0x80		/* Bank select enable */
/* useful defaults for LCR */
#define LCR_8N1		0x03

#define LSR_DR		0x01		/* Data ready */
#define LSR_THRE	0x20		/* Xmit holding register empty */

#define MCR_DTR         0x01
#define MCR_RTS         0x02

#define FCR_FIFO_EN     0x01		/* Fifo enable */
#define FCR_RXSR        0x02		/* Receiver soft reset */
#define FCR_TXSR        0x04		/* Transmitter soft reset */

#define LCRVAL LCR_8N1					/* 8 data, 1 stop, no parity */
#define MCRVAL (MCR_DTR | MCR_RTS)			/* RTS/DTR */
#define FCRVAL (FCR_FIFO_EN | FCR_RXSR | FCR_TXSR)	/* Clear & enable FIFOs */

#define thr rbr
#define dll rbr
#define dlm ier

void NS16550_init (NS16550_t com_port, int baud_divisor)
{
	com_port->ier = 0x00;
	com_port->lcr = LCR_BKSE | LCRVAL;
	com_port->dll = baud_divisor & 0xff;
	com_port->dlm = (baud_divisor >> 8) & 0xff;
	com_port->lcr = LCRVAL;
	com_port->mcr = MCRVAL;
	com_port->fcr = FCRVAL;
}

void NS16550_putc (NS16550_t com_port, char c)
{
	while ((com_port->lsr & LSR_THRE) == 0);
	com_port->thr = c;
}

int serial_init (void)
{
	NS16550_init(console, (CFG_NS16550_CLK / 16 / CONFIG_BAUDRATE));
	return (0);
}

void
serial_putc(const char c)
{
	if (c == '\n')
		NS16550_putc(console, '\r');

	NS16550_putc(console, c);
}

void
serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}

int main (void)
{
    serial_puts ("hello, world!\n\n");
    return 0;
}

