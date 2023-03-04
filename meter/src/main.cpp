#if defined(ENV_NATIVE)

int main(int argc, char **argv) { return 0; }

#else

#include "firmware/firmware.h"
Firmware f{};
void setup(void) { f.setup(); }
void loop(void) { f.process(); }

#endif
