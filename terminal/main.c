#include "kef_api.h"

int main(void) {
    // 1. Konsol Penceresini Oluştur
    kef_window_create("Kryon Konsol", 500, 320);
    
    // 2. Doğrudan arka plan rengini ayarla (Üstteki gri boşluk kapanır)
    backgroundColor(0xFF000000);
    
    // 3. Beyaz Konsol Satırları (Koordinatları üst boşluğa göre güncelledik)
    label(15, 15, 0xFFFFFFFF, "KryonOS Shell [Version 1.0.0]", ANCHOR_LEFT);
    label(15, 45, 0xFFFFFFFF, "root@kryon:/# cat /system/info.txt", ANCHOR_LEFT);
    label(15, 70, 0xFFFFFFFF, "Isletim Sistemi: KryonOS (32-bit x86)", ANCHOR_LEFT);
    label(15, 95, 0xFFFFFFFF, "Mimari: KEF (Kryon Executable Format)", ANCHOR_LEFT);
    label(15, 120, 0xFFFFFFFF, "Durum: Tum servisler aktif ve kararli.", ANCHOR_LEFT);
    label(15, 155, 0xFFFFFFFF, "root@kryon:/# _", ANCHOR_LEFT);

    return 0;
}

__attribute__((section(".text._start"), naked)) void _start(void) {
    __asm__ volatile (
        "call main\n\t"
        "ret\n\t"
    );
}