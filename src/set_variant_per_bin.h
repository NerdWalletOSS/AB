extern int 
set_variant_per_bin(
    uint32_t *proportions,
    int num_variants,
    uint8_t *variant_per_bin
    );
extern int 
alt_set_variant_per_bin(
    uint32_t *proportions,
    int num_variants, /* this includes control */
    uint8_t *variant_per_bin
    );
extern int
chk_variant_per_bin(
    uint32_t *proportions,
    uint32_t num_variants,
    uint8_t *variant_per_bin
    );
