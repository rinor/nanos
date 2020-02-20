#define APIC_APICID      0x802
#define APIC_APICVER     0x803
#define APIC_TASKPRIOR   0x808
#define APIC_EOI         0x80B
#define APIC_LDR         0x80D
#define APIC_DFR         0x80E
#define APIC_SPURIOUS    0x80F
#define APIC_ESR         0x828
#define APIC_ICR         0x830
#define APIC_LVT_TMR     0x832
#define APIC_LVT_PERF    0x834
#define APIC_LVT_LINT0   0x835
#define APIC_LVT_LINT1   0x836
#define APIC_LVT_ERR     0x837
#define APIC_TMRINITCNT  0x838
#define APIC_TMRCURRCNT  0x839
#define APIC_TMRDIV      0x83E
#define APIC_SELF_IPI    0x83F
#define APIC_LIMIT       0x840

#define APIC_DISABLE     0x10000
#define APIC_SW_ENABLE   0x100
#define APIC_CPUFOCUS    0x200
#define APIC_NMI         (4 << 8)
#define TMR_PERIODIC     0x20000
#define TMR_TSC_DEADLINE 0x40000
#define TMR_BASEDIV      (1 << 20)
#define APIC_LVT_INTMASK 0x00010000

/* ICR flags */
#define ICR_TYPE_FIXED        0x00000000
#define ICR_TYPE_SMI          0x00000200
#define ICR_TYPE_NMI          0x00000400
#define ICR_TYPE_INIT         0x00000500
#define ICR_TYPE_STARTUP      0x00000600
#define ICR_PHYSICAL          0x00000000
#define ICR_LOGICAL           0x00000800
#define ICR_DELIVS            0x00001000
#define ICR_DEASSERT          0x00000000
#define ICR_ASSERT            0x00004000
#define ICR_TRIGGER_EDGE      0x00000000
#define ICR_TRIGGER_LEVEL     0x00008000
#define ICR_DEST_SELF         0x00040000
#define ICR_DEST_ALL          0x00080000
#define ICR_DEST_ALL_EXC_SELF 0x000C0000

#define TARGET_EXCLUSIVE_BROADCAST 0xfffffffful
#define IA32_APIC_BASE_MSR 0x1b

/* 64 bit data for x2apic, only 32 used for xapic */
typedef struct apic_iface {
    const char *name;
    void (*write)(struct apic_iface *, int reg, u64 val);
    u64 (*read)(struct apic_iface *, int reg);       /* XXX 64 for x2? */
    void (*ipi)(struct apic_iface *, u32 target, u64 flags, u8 vector);
    boolean (*detect_and_init)(struct apic_iface *, kernel_heaps kh);
} *apic_iface;

void lapic_eoi(void);
void init_apic(kernel_heaps kh);
void lapic_set_tsc_deadline_mode(u32 v);
boolean init_lapic_timer(clock_timer *ct, thunk *per_cpu_init);
void apic_ipi(u32 target, u64 flags, u8 vector);
u32 apic_id(void);
void enable_apic(void);

static inline int this_cpu(void)
{
    // for now, assume apic id == cpu num
    return apic_id();
}

static inline cpuinfo get_cpuinfo(void)
{
    int cpu = this_cpu();
    return &cpuinfos[cpu];
}
