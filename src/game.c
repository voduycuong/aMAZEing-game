#include "game.h"
#include "uart.h"

#define PLAYER_STEP 24
#define PLAYER_RADIUS 5

// Colors
#define GUTS 0x0068ADFF
#define GRIFFITH 0x007030A0
#define WALL 0x00000000
#define PATH 0x00FFFFFF
#define BOMB 0x00DC143C
#define STAR 0x00A459D1
#define KEY 0x0070AD47
#define FRAME_CHANGE_INTERVAL 6

int32_t FOV_RADIUS = 60;
int32_t star_flag = 1;
int32_t bomb_flag = 1;
int32_t key_flag = 1;

Entity guts;
Entity griffith;
Entity star;
Entity bomb;
Entity key;

uint32_t front_idle[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00061123, 0x00061123, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x000a101a, 0x00f1d0ab, 0x00f1d0ab, 0x00f2d2ad, 0x00f0d1ac, 0x00f0d2aa, 0x00ecd6a5, 0x00081016, 0x00000000,
    0x00000000, 0x00edd1ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00ecd2b0, 0x00000000,
    0x00000000, 0x00f0d1ae, 0x00edd2ad, 0x000a0f1a, 0x00ead7a7, 0x00f0d2a8, 0x000b0d24, 0x00efd2ab, 0x00e3d4bf, 0x00000000,
    0x00000000, 0x00f0d1af, 0x00f0d1ac, 0x00ebd1a8, 0x00efd2a9, 0x00f0d2aa, 0x00eed2b3, 0x00f0d1ac, 0x00e3d4bf, 0x00000000,
    0x00000000, 0x00f3d3a8, 0x00f0d1ac, 0x00edcfa9, 0x00f0d2a9, 0x00f0d2aa, 0x00edd4b1, 0x00ecd4ab, 0x00f1d1ad, 0x00000000,
    0x00000000, 0x00071120, 0x00a4a154, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00a7a454, 0x0009101d, 0x00000000,
    0x00021125, 0x00eed2ac, 0x007c7266, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x007c7066, 0x00efd2ab, 0x0006131e,
    0x00000000, 0x000d0f1d, 0x00f0d2aa, 0x00f0d1ac, 0x00eed2ab, 0x00eed2ad, 0x00f0d1ac, 0x00f0d1ab, 0x000a101d, 0x00000000,
    0x00000000, 0x00000000, 0x00041024, 0x00041024, 0x00000000, 0x00000000, 0x0003111d, 0x00060e23, 0x00000000, 0x00000000};

uint32_t front_walk1[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x000a0f25, 0x00061123, 0x00051123, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00061019, 0x00eed1ac, 0x00f0d2aa, 0x00f0d1ac, 0x00f0d1ac, 0x00f1d3ad, 0x00f5d1a9, 0x00000000, 0x00000000,
    0x0007131b, 0x00eed3a8, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x000f0f10, 0x00000000,
    0x0007111a, 0x00f0d2a8, 0x00130f0f, 0x0008101c, 0x00f0d2aa, 0x00080f24, 0x000d0c24, 0x00f0d1ac, 0x00160e05, 0x00000000,
    0x00071217, 0x00eed3a8, 0x00f0d1ab, 0x00090d1e, 0x00f0d1ac, 0x00071329, 0x00f5d0ad, 0x00efd1ac, 0x00160e05, 0x00000000,
    0x00081117, 0x00ecd4a8, 0x00eed2ab, 0x0009101f, 0x00f0d2aa, 0x00021025, 0x00f2d1ad, 0x00e6d8a8, 0x00120e0d, 0x00000000,
    0x00000000, 0x0007101f, 0x00a7a255, 0x00eed1b2, 0x00efd1ac, 0x00f0d1ac, 0x00efd1ac, 0x00a9a052, 0x005b58a4, 0x000d101b,
    0x00041118, 0x00f0d1ac, 0x007c7264, 0x00eed2a7, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d2aa, 0x00f0d3a9, 0x000a0e22, 0x00000000,
    0x00000000, 0x000b101e, 0x00f1d3a9, 0x00edd3ad, 0x00edd3ad, 0x00edd1b1, 0x00edd2ae, 0x00f3d0b0, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x000d1013, 0x000b111b, 0x000d1419, 0x00000000, 0x00000000};

uint32_t front_walk2[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00061123, 0x00061123, 0x00061123, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00f3d0a2, 0x00f0d0ac, 0x00f1d2af, 0x00f0d1ac, 0x00f0d1ac, 0x00f1d3ad, 0x00151007, 0x00000000,
    0x00000000, 0x000f1015, 0x00eed2ae, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d2aa, 0x00071020,
    0x00000000, 0x000e0f14, 0x00edd3ac, 0x000a101c, 0x00031122, 0x00f0d1ab, 0x00080f24, 0x000a0f21, 0x00f0d2aa, 0x000b1017,
    0x00000000, 0x000e1011, 0x00eed2ac, 0x00f0d1ac, 0x00070d25, 0x00f0d1ac, 0x00071329, 0x00f1d3ad, 0x00f0d2aa, 0x000b1017,
    0x00000000, 0x00130d13, 0x00f3cfac, 0x00efd2aa, 0x00051027, 0x00f0d1ac, 0x00030f21, 0x00f0d2ac, 0x00f2d2a4, 0x00090f24,
    0x000a0f1d, 0x005b5ba4, 0x000e1022, 0x00f0d1aa, 0x00efd1ac, 0x00f0d1ac, 0x00f0d3ab, 0x00a69f5b, 0x00090f20, 0x00000000,
    0x00000000, 0x000a1022, 0x00f1d2a6, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f1d3ad, 0x007e7166, 0x00f0d3ab, 0x00080e1e,
    0x00000000, 0x00000000, 0x00f0d1a9, 0x00edd2ac, 0x00eed2ac, 0x00edd2ab, 0x00edd3ab, 0x00edd3ab, 0x000b0d23, 0x00000000,
    0x00000000, 0x00000000, 0x00080e16, 0x00081017, 0x000d121a, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

uint32_t back_idle[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00061124, 0x00031125, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00061018, 0x00f0d1ab, 0x00f0d1ab, 0x00f1d1ac, 0x00efd2ab, 0x00f1d1ab, 0x00edd4a6, 0x00070e1b, 0x00000000,
    0x00000000, 0x00edd2b2, 0x00f0d1ac, 0x00f0d1ab, 0x00f0d1ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00e8d4b8, 0x00000000,
    0x00000000, 0x00f0d1ac, 0x00efd1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00e3d4bf, 0x00000000,
    0x00000000, 0x00eed1b2, 0x00efd1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00e3d4bf, 0x00000000,
    0x00000000, 0x00f3d2a6, 0x00efd1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00edd3ac, 0x00ebd1b2, 0x00000000,
    0x00000000, 0x00f1ceb3, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f1d1ab, 0x00ebd3b1, 0x00000000,
    0x00041221, 0x007e6f66, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2ab, 0x007f7162, 0x0006141b,
    0x00000000, 0x0007121b, 0x00efd2ab, 0x00f0d1ac, 0x00efd2aa, 0x00f0d1ab, 0x00f0d1ac, 0x00efd2aa, 0x000b101f, 0x00000000,
    0x00000000, 0x00000000, 0x00041123, 0x00061021, 0x00000000, 0x00000000, 0x0003111c, 0x00040f24, 0x00000000, 0x00000000};

uint32_t back_walk1[100] = {
    0x00000000, 0x000c0c13, 0x000b0d1d, 0x005d574f, 0x005f5852, 0x005e5852, 0x002f3335, 0x000a111c, 0x00000000, 0x00000000,
    0x0024272e, 0x00726a5f, 0x00b6a188, 0x00edceaa, 0x00efd1ab, 0x00efd2aa, 0x00c4ad90, 0x009c8b78, 0x00262b30, 0x00001c1c,
    0x004b4845, 0x00f0d1aa, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d2ac, 0x006e6358, 0x0007111c,
    0x004b4746, 0x00f0d1aa, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x006e6456, 0x0007111c,
    0x004b4844, 0x00f0d1aa, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x006e6456, 0x0007111c,
    0x00433f3f, 0x00bda88c, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00dabf9f, 0x00484340, 0x0007111c,
    0x00101527, 0x0034333d, 0x00cdb597, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00b7a387, 0x0027294c, 0x001f213c,
    0x00202443, 0x007b73a2, 0x00dec3aa, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00e8cba6, 0x005f5952, 0x000a0f1d, 0x000e0e22,
    0x0014172a, 0x00191c35, 0x00998878, 0x00b5a188, 0x00b7a087, 0x005c5760, 0x003c3b55, 0x00413f76, 0x00090e19, 0x00000000,
    0x00000000, 0x00000000, 0x000e0e1c, 0x000b0d20, 0x000b0d1d, 0x00232848, 0x00292b4f, 0x0026274a, 0x00060c18, 0x00000000};

uint32_t back_walk2[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00061124, 0x00051124, 0x00041222, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00f1d2a6, 0x00f0d1ad, 0x00efd3aa, 0x00efd1ac, 0x00efd1ad, 0x00f0d1ab, 0x00120f0e, 0x00000000,
    0x00000000, 0x000f0f10, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ab, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f1d2ab, 0x000a101a,
    0x00000000, 0x000f0f13, 0x00efd1ac, 0x00eed2ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ab, 0x000c1015,
    0x00000000, 0x000c1110, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ab, 0x000e0f15,
    0x00000000, 0x00140d0e, 0x00f1d1ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f4d1a6, 0x000c0e1e,
    0x00091014, 0x005d599d, 0x00f1d1a8, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1a9, 0x00100b19, 0x00000000,
    0x00000000, 0x000a101c, 0x007d7160, 0x00efd2ab, 0x00efd1ad, 0x00f0d1ac, 0x00f0d1ac, 0x00edd3a8, 0x00605a99, 0x00080f18,
    0x00000000, 0x00000000, 0x005d57a5, 0x000a0f1d, 0x00051124, 0x00efd2a9, 0x00efd2a9, 0x00ebd3af, 0x00070e1e, 0x00000000,
    0x00000000, 0x00000000, 0x00090f1b, 0x00090f17, 0x000c1319, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000};

uint32_t side_idle[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x00041127, 0x00050f21, 0x00031122, 0x00031325, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00051122, 0x00f2d3a9, 0x00f0d3ab, 0x00f2d1ac, 0x00f0d1ab, 0x00f0d1ac, 0x00f0d1a6, 0x00061022, 0x00000000,
    0x0008111a, 0x00f2d2ab, 0x00eed2ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2a9, 0x00090d1e,
    0x000a1019, 0x00f0d1ab, 0x007a7267, 0x00071021, 0x00edd2ab, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2a9, 0x000b0f1a,
    0x000a101a, 0x00f0d1aa, 0x00797364, 0x00f0d3a4, 0x00f0d2ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2a9, 0x000b0f1a,
    0x000b0f1a, 0x00eed3aa, 0x00787363, 0x00f3d0ae, 0x00f0d1ad, 0x00f0d1ab, 0x00f0d1ac, 0x00f0d1ac, 0x00f3d2a3, 0x000c0e1c,
    0x00000000, 0x00011122, 0x00f0d2aa, 0x00efd1ac, 0x00efd3a7, 0x00071020, 0x00031021, 0x007d7264, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x007b7165, 0x00f0d1ac, 0x00817062, 0x00f1d2ad, 0x00f3ceac, 0x000d101c, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x007c7160, 0x00f0d2a9, 0x00f0d2b0, 0x000b111e, 0x00021325, 0x007f7068, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00041027, 0x00041320, 0x0003131a, 0x00021120, 0x00000000, 0x00000000, 0x00000000};

uint32_t side_walk1[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x00041125, 0x00050f23, 0x00060f24, 0x00051224, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00051122, 0x00f2d2ab, 0x00f0d3ab, 0x00f2d2aa, 0x00f0d1ac, 0x00f0d1ab, 0x00f0d1a8, 0x00041022, 0x00000000,
    0x0008111a, 0x00f2d2ab, 0x00eed2ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2aa, 0x00090e1d,
    0x000a101a, 0x00f0d2aa, 0x007a7267, 0x00071021, 0x00edd2ab, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2ab, 0x000b0f1a,
    0x000a101a, 0x00f0d1aa, 0x00797364, 0x00f0d3a4, 0x00f0d2ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2ab, 0x000b0f1a,
    0x000b0f1a, 0x00eed3aa, 0x00787363, 0x00f2d0ae, 0x00f0d2ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f3d2a3, 0x000e0e1a,
    0x00000000, 0x00011122, 0x00f0d2aa, 0x00eed2ae, 0x000a0f1e, 0x00f1d3ad, 0x00efd1ac, 0x007b7065, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x007b7165, 0x0001101e, 0x006156aa, 0x00a695ac, 0x007d7264, 0x0040413f, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x007c7162, 0x00eed1a9, 0x00090f1b, 0x00061120, 0x00f0d1ab, 0x00a696a3, 0x00080e1b, 0x00000000,
    0x00000000, 0x00000000, 0x00071019, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00080d17, 0x00000000, 0x00000000};

uint32_t side_walk2[100] = {
    0x00000000, 0x00000000, 0x00000000, 0x00041125, 0x00050f23, 0x00031124, 0x00031324, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00051122, 0x00f2d2ab, 0x00f0d3ab, 0x00f2d2aa, 0x00f0d1ac, 0x00f0d1ab, 0x00f0d1a8, 0x00061022, 0x00000000,
    0x0008111a, 0x00f2d2ab, 0x00eed2ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2aa, 0x00080e1b,
    0x000a101a, 0x00f0d2aa, 0x007a7267, 0x00071021, 0x00edd2ab, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2ab, 0x000b0f1a,
    0x000a101a, 0x00f0d1aa, 0x00797364, 0x00f0d3a4, 0x00f0d2ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ac, 0x00efd2ab, 0x000b0f1a,
    0x000b0f1a, 0x00eed3aa, 0x00787363, 0x00f3d0ae, 0x00f0d1ad, 0x00f0d1ac, 0x00f0d1ac, 0x00f0d1ab, 0x00f3d2a3, 0x000c0e1a,
    0x00000000, 0x00011122, 0x00f0d2aa, 0x00f0d1ad, 0x00f0d1ab, 0x00f0d1ac, 0x00f1d3a4, 0x000a1020, 0x00000000, 0x00000000,
    0x00000000, 0x000a101a, 0x007b7165, 0x00f0d1ac, 0x00f0d1ab, 0x007c7264, 0x00a894a1, 0x00323462, 0x00000000, 0x00000000,
    0x00080f20, 0x005c58a6, 0x00a595aa, 0x00f0d1ad, 0x00ecd3ad, 0x00e8d5ad, 0x0009111d, 0x00353461, 0x00080e1a, 0x00000000,
    0x00000000, 0x00000000, 0x000a1018, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00070e16, 0x00000000, 0x00000000};

AnimationState guts_animation_state = FRONT_IDLE;

void drawCharacterFrame(Position pos, AnimationState state)
{
    uint32_t *currentFrame;
    switch (state)
    {
    case FRONT_IDLE:
        currentFrame = front_idle;
        uart_puts("Drawing FRONT_IDLE frame.\n");
        break;
    case FRONT_WALK1:
        currentFrame = front_walk1;
        uart_puts("Drawing FRONT_WALK1 frame.\n");
        break;
    case FRONT_WALK2:
        currentFrame = front_walk2;
        uart_puts("Drawing FRONT_WALK2 frame.\n");
        break;
    case BACK_IDLE:
        currentFrame = back_idle;
        break;
    case BACK_WALK1:
        currentFrame = back_walk1;
        break;
    case BACK_WALK2:
        currentFrame = back_walk2;
        break;
    case SIDE_IDLE:
        currentFrame = side_idle;
        break;
    case SIDE_WALK1:
        currentFrame = front_walk1;
        break;
    case SIDE_WALK2:
        currentFrame = side_walk2;
        break;
    default:
        currentFrame = front_idle;
    }

    int frameWidth = 10;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            drawPixelARGB32(startX + x, startY + y, currentFrame[y * frameWidth + x]);
        }
    }
}

void clearCharacterFrame(Position pos)
{
    int frameWidth = 10;

    int startX = pos.x - frameWidth / 2;
    int startY = pos.y - frameWidth / 2;

    for (int y = 0; y < frameWidth; y++)
    {
        for (int x = 0; x < frameWidth; x++)
        {
            drawPixelARGB32(startX + x, startY + y, WALL);
        }
    }
}

void handleAndAnimateCharacterMovement(Position pos, int input)
{
    uart_puts("calling this function.\n");
    AnimationState animations[4];

    switch (input)
    {
    case 'w':
        animations[0] = BACK_IDLE;
        animations[1] = BACK_WALK1;
        animations[2] = BACK_WALK2;
        animations[3] = BACK_IDLE;
        break;
    case 's':
        animations[0] = FRONT_IDLE;
        animations[1] = FRONT_WALK1;
        animations[2] = FRONT_WALK2;
        animations[3] = FRONT_IDLE;
        break;
    case 'a':
    case 'd':
        animations[0] = SIDE_IDLE;
        animations[1] = SIDE_WALK1;
        animations[2] = SIDE_WALK2;
        animations[3] = SIDE_IDLE;
        break;
    default:
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        clearCharacterFrame(guts.box.pos);

        switch (input)
        {
        case 'w':
            pos.y -= FRAME_CHANGE_INTERVAL;
            break;
        case 's':
            pos.y += FRAME_CHANGE_INTERVAL;
            break;
        case 'a':
            pos.x -= FRAME_CHANGE_INTERVAL;
            break;
        case 'd':
            pos.x += FRAME_CHANGE_INTERVAL;
            break;
        }

        guts.box.pos = pos;
        guts_animation_state = animations[i];
        drawCharacterFrame(guts.box.pos, guts_animation_state);
        wait_msec(30000);
    }
}

void game()
{
    Position start_pos1 = {PLAYER_STEP / 2, MAZE_WIDTH / 2 - PLAYER_STEP};
    Position start_pos2 = {MAZE_WIDTH / 2 - PLAYER_STEP, PLAYER_STEP / 2 + PLAYER_STEP};
    Position end_pos = {MAZE_WIDTH - (PLAYER_STEP / 2), MAZE_WIDTH / 2 + PLAYER_STEP};
    Position star_pos = {0, 0};
    Position bomb_pos = {0, 0};
    Position key_pos = {0, 0};

    Box guts_box = {start_pos1, 5, 5};
    Box griffith_box = {start_pos2, 5, 5};
    Box star_box = {star_pos, 5, 5};
    Box bomb_box = {bomb_pos, 5, 5};
    Box key_box = {key_pos, 5, 5};

    guts.box = guts_box;
    griffith.box = griffith_box;
    star.box = star_box;
    bomb.box = bomb_box;
    key.box = key_box;

    load_full_maze();

    do
    {
        star.box.pos = set_random_position();
        bomb.box.pos = set_random_position();
        key.box.pos = set_random_position();
    } while (getPixelARGB32(star.box.pos.x, star.box.pos.y) == WALL ||
             getPixelARGB32(bomb.box.pos.x, bomb.box.pos.y) == WALL ||
             getPixelARGB32(key.box.pos.x, key.box.pos.y) == WALL ||
             star.box.pos.x == bomb.box.pos.x ||
             star.box.pos.y == bomb.box.pos.y);

    clear_maze();

    while (1)
    {
        if (win(guts.box.pos, end_pos, key_flag))
        {
            clear_maze();
            uart_puts("ok you win! more?\n");
            char input = uart_getc();
            if (input == 'y')
            {
                FOV_RADIUS = 60;
                star_flag = 1;
                bomb_flag = 1;
                key_flag = 1;
                break;
            }
            else if (input == 'n')
                exit();
        }

        else
        {
            make_fov(guts.box.pos, FOV_RADIUS);
            make_fov(griffith.box.pos, FOV_RADIUS);
            drawCharacterFrame(guts.box.pos, guts_animation_state);
            drawCircleARGB32(griffith.box.pos.x, griffith.box.pos.y, PLAYER_RADIUS, GRIFFITH);

            if (star_flag == 1)
                drawCircleARGB32(star.box.pos.x, star.box.pos.y, PLAYER_RADIUS, STAR);
            if (bomb_flag == 1)
                drawCircleARGB32(bomb.box.pos.x, bomb.box.pos.y, PLAYER_RADIUS, BOMB);
            if (key_flag == 1)
                drawCircleARGB32(key.box.pos.x, key.box.pos.y, PLAYER_RADIUS, KEY);

            // uart_puts("\nColor: ");
            // uart_hex(getPixelARGB32(guts.box.pos.x, guts.box.pos.y));

            char input = uart_getc();
            handle_input(&guts.box.pos, input);
            handle_input(&griffith.box.pos, input);
        }
    }
}

// Function for input directions
void handle_input(Position *pos, int input)
{
    switch (input)
    {
    case 'w': // Up
        if (interact(pos->x, pos->y - PLAYER_STEP) == 's')
            star_flag = 0;
        if (interact(pos->x, pos->y - PLAYER_STEP) == 'b')
            bomb_flag = 2;
        if (interact(pos->x, pos->y - PLAYER_STEP) == 'k')
            key_flag = 3;
        if (interact(pos->x, pos->y - PLAYER_STEP) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->y - PLAYER_STEP > 0)
            {
                handleAndAnimateCharacterMovement(guts.box.pos, input);
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 's': // Down
        if (interact(pos->x, pos->y + PLAYER_STEP) == 's')
            star_flag = 0;
        if (interact(pos->x, pos->y + PLAYER_STEP) == 'b')
            bomb_flag = 2;
        if (interact(pos->x, pos->y + PLAYER_STEP) == 'k')
            key_flag = 3;
        if (interact(pos->x, pos->y + PLAYER_STEP) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->y + PLAYER_STEP < MAZE_WIDTH)
            {
                handleAndAnimateCharacterMovement(guts.box.pos, input);
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 'a': // Left
        if (interact(pos->x - PLAYER_STEP, pos->y) == 's')
            star_flag = 0;
        if (interact(pos->x - PLAYER_STEP, pos->y) == 'b')
            bomb_flag = 2;
        if (interact(pos->x - PLAYER_STEP, pos->y) == 'k')
            key_flag = 3;
        if (interact(pos->x - PLAYER_STEP, pos->y) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->x - PLAYER_STEP > 0)
            {
                handleAndAnimateCharacterMovement(guts.box.pos, input);
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 'd': // Right
        if (interact(pos->x + PLAYER_STEP, pos->y) == 's')
            star_flag = 0;
        if (interact(pos->x + PLAYER_STEP, pos->y) == 'b')
            bomb_flag = 2;
        if (interact(pos->x + PLAYER_STEP, pos->y) == 'k')
            key_flag = 3;
        if (interact(pos->x + PLAYER_STEP, pos->y) != 'w')
        {
            clear_fov(*pos, FOV_RADIUS);
            if (pos->x + PLAYER_STEP < MAZE_HEIGHT)
            {
                handleAndAnimateCharacterMovement(guts.box.pos, input);
                check_entity(*pos, &star_flag);
                check_entity(*pos, &bomb_flag);
                check_entity(*pos, &key_flag);
            }
        }
        break;

    case 'o': // Star
        increase_fov(*pos);
        break;

    case 'p': // Bomb
        decrease_fov(*pos);
        break;

    default:
        break;
    }
}

// Draw field of view
void make_fov(Position pos, int rad)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y < rad * rad)
            {
                if (pos.x + x < MAZE_WIDTH && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x + x, pos.y + y, epd_bitmap_allArray[0][(pos.y + y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y - y > 0)
                    drawPixelARGB32(pos.x - x, pos.y - y, epd_bitmap_allArray[0][(pos.y - y) * MAZE_WIDTH + (pos.x - x)]);
                if (pos.x + x < MAZE_HEIGHT && pos.y - y > 0)
                    drawPixelARGB32(pos.x + x, pos.y - y, epd_bitmap_allArray[0][(pos.y - y) * MAZE_WIDTH + (pos.x + x)]);
                if (pos.x - x > 0 && pos.y + y < MAZE_HEIGHT)
                    drawPixelARGB32(pos.x - x, pos.y + y, epd_bitmap_allArray[0][(pos.y + y) * MAZE_WIDTH + (pos.x - x)]);
            }
}

// Clear field of view
void clear_fov(Position pos, int rad)
{
    for (int y = 0; y < MAZE_HEIGHT; y++)
        for (int x = 0; x < MAZE_WIDTH; x++)
            if (x * x + y * y <= rad * rad)
            {
                drawPixelARGB32(pos.x + x, pos.y + y, WALL);
                drawPixelARGB32(pos.x - x, pos.y - y, WALL);
                drawPixelARGB32(pos.x + x, pos.y - y, WALL);
                drawPixelARGB32(pos.x - x, pos.y + y, WALL);
            }
}

// Increase field of view
void increase_fov(Position pos)
{
    drawCircleARGB32(pos.x, pos.y, FOV_RADIUS, WALL);
    if (FOV_RADIUS < 200)
        FOV_RADIUS += 60;
}

// Decrease field of view
void decrease_fov(Position pos)
{
    drawCircleARGB32(pos.x, pos.y, FOV_RADIUS, WALL);
    if (FOV_RADIUS > 30)
        FOV_RADIUS -= 20;
}

// Check if player has escaped or not
int win(Position pos, Position win, int flag)
{
    if (pos.x == win.x && pos.y == win.y && flag == -1)
        return 1;
    else
        return 0;
}

int interact(int pos_x, int pos_y)
{
    if (getPixelARGB32(pos_x, pos_y) == WALL)
        return 'w';
    else if (detect_collision(guts.box, star.box) == 1)
    {
        uart_puts("Star found\n");
        return 's';
    }
    else if (detect_collision(guts.box, bomb.box) == 1)
    {
        uart_puts("Bomb found\n");
        return 'b';
    }
    else if (detect_collision(guts.box, key.box) == 1)
    {
        uart_puts("Key found\n");
        return 'k';
    }
    else
        return 'n';
}

int detect_collision(Box a, Box b)
{
    if (a.pos.x < b.pos.x + b.width &&
        a.pos.x + a.width > b.pos.x &&
        a.pos.y < b.pos.y + b.height &&
        a.pos.y + a.height > b.pos.y)
    {
        return 1; // Collision detected
    }
    return 0; // No collision
}

Position set_random_position()
{
    Position temp_pos;

    temp_pos.x = rand(5, 30) * PLAYER_STEP + PLAYER_STEP / 2;
    temp_pos.y = rand(5, 30) * PLAYER_STEP + PLAYER_STEP / 2;

    return temp_pos;
}

void load_full_maze()
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, epd_bitmap_allArray[0][y * MAZE_WIDTH + x]);
}

void clear_maze()
{
    for (int y = 0; y <= MAZE_HEIGHT; y++)
        for (int x = 0; x <= MAZE_WIDTH; x++)
            drawPixelARGB32(x, y, WALL);
}

void check_entity(Position pos, int *flag)
{
    int temp = *flag;

    if (temp == 0)
    {
        drawCircleARGB32(pos.x, pos.y, PLAYER_RADIUS, WALL);
        increase_fov(pos);
        temp = -1;
    }

    else if (temp == 2)
    {
        drawCircleARGB32(pos.x, pos.y, PLAYER_RADIUS, WALL);
        decrease_fov(pos);
        temp = -1;
    }

    else if (temp == 3)
    {
        drawCircleARGB32(pos.x, pos.y, PLAYER_RADIUS, WALL);
        temp = -1;
    }

    *flag = temp;
}