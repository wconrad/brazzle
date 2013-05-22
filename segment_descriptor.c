#include "segment_descriptor.h"

#include "string.h"

void sd_init(SegmentDescriptor * sd) {
  memset(sd, 0, sizeof(*sd));
}

void sd_set_address(SegmentDescriptor * sd, uint32_t address) {
  sd->base_low = address & 0xffff;
  sd->base_mid = (address >> 16) & 0xff;
  sd->base_high = (address >> 24) & 0xff;
}

void sd_set_limit(SegmentDescriptor * sd, uint32_t limit) {
  sd->limit_low = limit & 0xffff;
  sd->flags &= ~SD_FLG_LIMIT_MASK;
  sd->flags |= (limit >> 16) & SD_FLG_LIMIT_MASK;
}
