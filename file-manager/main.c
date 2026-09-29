#include "kef_api.h"

#define MAX_FILES 32

typedef struct {
    char name[64];
    uint32_t size;
    uint8_t type; // 0 = Dosya, 1 = Klasör/Dizin
} vfs_file_info_t;

// Uygulama Durumu (State)
static char current_path[128] = "/";
static vfs_file_info_t file_list[MAX_FILES];
static int file_count = 0;

// Fonksiyon Bildirimleri
void refresh_directory(void);
void render_file_list(void);

// Helper: String Birleştirme (Kernel/Lib dependency olmadan güvenli ekleme)
void append_path(char* dest, const char* src) {
    size_t len = kef_strlen(dest);
    
    // Eğer sonunda '/' yoksa ve root değilse ekle
    if (len > 0 && dest[len - 1] != '/') {
        dest[len] = '/';
        dest[len + 1] = '\0';
        len++;
    }
    
    size_t i = 0;
    while (src[i] != '\0' && (len + i) < 127) {
        dest[len + i] = src[i];
        i++;
    }
    dest[len + i] = '\0';
}

void refresh_directory(void) {
    if (KEF_API_TABLE && KEF_API_TABLE->get_directory_files) {
        file_count = KEF_API_TABLE->get_directory_files(current_path, file_list, MAX_FILES);
    } else {
        file_count = 0;
    }
}

void on_root_click(void) {
    current_path[0] = '/';
    current_path[1] = '\0';
    refresh_directory();
    kef_print("Kok dizine gecildi: /\n");
}

void on_refresh_click(void) {
    refresh_directory();
    kef_print("Dizin yenilendi.\n");
}

// Dosya / Klasör Tıklama Yakalayıcıları (Maksimum 7 görünür öge için)
void on_item_click_0(void) {
    if (file_count > 0 && file_list[0].type == 1) { // Klasörse dizine gir
        append_path(current_path, file_list[0].name);
        refresh_directory();
    }
}
void on_item_click_1(void) {
    if (file_count > 1 && file_list[1].type == 1) {
        append_path(current_path, file_list[1].name);
        refresh_directory();
    }
}
void on_item_click_2(void) {
    if (file_count > 2 && file_list[2].type == 1) {
        append_path(current_path, file_list[2].name);
        refresh_directory();
    }
}
void on_item_click_3(void) {
    if (file_count > 3 && file_list[3].type == 1) {
        append_path(current_path, file_list[3].name);
        refresh_directory();
    }
}
void on_item_click_4(void) {
    if (file_count > 4 && file_list[4].type == 1) {
        append_path(current_path, file_list[4].name);
        refresh_directory();
    }
}
void on_item_click_5(void) {
    if (file_count > 5 && file_list[5].type == 1) {
        append_path(current_path, file_list[5].name);
        refresh_directory();
    }
}
void on_item_click_6(void) {
    if (file_count > 6 && file_list[6].type == 1) {
        append_path(current_path, file_list[6].name);
        refresh_directory();
    }
}

// Tıklama callback dizisi
static void (*item_callbacks[7])(void) = {
    on_item_click_0, on_item_click_1, on_item_click_2,
    on_item_click_3, on_item_click_4, on_item_click_5, on_item_click_6
};

int main(void) {
    kef_print("KryonOS Dosya Yöneticisi başlatılıyor...\n");
    
    // 1. Pencere Oluştur
    kef_window_create("Dosya Yöneticisi - KryonOS", 580, 360);
    backgroundColor(0xFF181818);

    // 2. Üst Toolbar
    panel(10, 10, 560, 40, 0xFF222222, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP);
    button(18, 16, 65, 28, 0xFF333333, 0xFFFFFFFF, " / (Kok)", on_root_click, ANCHOR_LEFT | ANCHOR_TOP);
    button(88, 16, 75, 28, 0xFF333333, 0xFFFFFFFF, "Yenile", on_refresh_click, ANCHOR_LEFT | ANCHOR_TOP);
    
    // Adres Göstergesi
    panel(170, 16, 390, 28, 0xFF121212, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP);
    label(180, 23, 0xFF00FFCC, current_path, ANCHOR_LEFT | ANCHOR_TOP);

    // 3. Sol Sidebar
    panel(10, 58, 130, 262, 0xFF202020, ANCHOR_LEFT | ANCHOR_TOP | ANCHOR_BOTTOM);
    label(20, 68, 0xFF888888, "HIZLI ERISIM", ANCHOR_LEFT | ANCHOR_TOP);
    button(18, 90, 114, 30, 0xFF2A2A2A, 0xFFDDDDDD, " Kök (/)", on_root_click, ANCHOR_LEFT | ANCHOR_TOP);

    // 4. Ana Liste Arka Plan Paneli
    panel(148, 58, 422, 262, 0xFF242424, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP | ANCHOR_BOTTOM);

    // Dizin Dosyalarını Çek
    refresh_directory();

    int start_y = 68;
    int item_height = 32;

    if (file_count <= 0) {
        label(165, start_y + 10, 0xFFAAAAAA, "Dizin bos veya okunamadi.", ANCHOR_LEFT | ANCHOR_TOP);
    } else {
        for (int i = 0; i < file_count && i < 7; i++) {
            int current_y = start_y + (i * item_height);

            uint32_t bg_col = (i % 2 == 0) ? 0xFF2C2C2C : 0xFF262626;
            // Klasörler Sarı (0xFFFFCC00), Dosyalar Beyaz (0xFFFFFFFF)
            uint32_t text_col = (file_list[i].type == 1) ? 0xFFFFCC00 : 0xFFFFFFFF; 

            // Eğer öge KLASÖR ise hover rengi verelim (0xFF383838), DOSYA ise hover kapalı olsun (0)
            uint32_t hover_col = (file_list[i].type == 1) ? 0xFF383838 : 0;

            // kef_panel_create ile tıklama callback'i ve hover rengini doğrudan bağlıyoruz
            kef_panel_create(154, current_y, 410, 28, bg_col, hover_col, item_callbacks[i], NULL, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_TOP);

            // Dosya / Klasör İsmi
            label(166, current_y + 7, text_col, file_list[i].name, ANCHOR_LEFT | ANCHOR_TOP);
        }
    }

    // 5. Alt Durum Çubuğu (Status Bar)
    panel(10, 326, 560, 24, 0xFF1C1C1C, ANCHOR_LEFT | ANCHOR_RIGHT | ANCHOR_BOTTOM);
    label(20, 331, 0xFFAAAAAA, "Sistem: VFS Baglandi | Ready", ANCHOR_LEFT | ANCHOR_BOTTOM);

    return 0;
}

__attribute__((section(".text._start"), naked)) void _start(void) {
    __asm__ volatile (
        "call main\n\t"
        "ret\n\t"
    );
}