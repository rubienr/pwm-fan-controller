#if defined(ENV_NATIVE)

int main(int argc, char **argv) { return 0; }

#else

    #include "firmware/Firmware.h"

Firmware f;
void setup() { f.setup(); }
void loop() { f.process(); }

#endif
