#include <windows.h>
#include <filter.h>

#define	TRACK_N	1
static TCHAR* track_name[TRACK_N] = { const_cast<char*>("ペースト先") };
static int track_default[TRACK_N] = { 0 };
static int track_s[TRACK_N] = { 0 };
static int track_e[TRACK_N] = { 49 };

#define	CHECK_N	2
static TCHAR* check_name[CHECK_N] = { const_cast<char*>("レイヤー名"),const_cast<char*>("その他") };
static int check_default[CHECK_N] = { 0,0 };

FILTER_DLL filter = {
    FILTER_FLAG_ALWAYS_ACTIVE,
    NULL,NULL,
    const_cast<char*>("レイヤー情報コピペ"),
    TRACK_N,track_name,track_default,track_s,track_e,
    CHECK_N,check_name,check_default,
    NULL,
    func_init,
    NULL,
    NULL,
    func_WndProc,
};
EXTERN_C FILTER_DLL __declspec(dllexport)* __stdcall GetFilterTable(void) {
    return &filter;
}

static int exedit_base;

FILTER* get_exeditfp(FILTER* fp) {
    SYS_INFO si;
    fp->exfunc->get_sys_info(NULL, &si);

    for (int i = 0; i < si.filter_n; i++) {
        FILTER* tfp = (FILTER*)fp->exfunc->get_filterp(i);
        if (tfp->information != NULL) {
            if (!strcmp(tfp->information, "拡張編集(exedit) version 0.92 by ＫＥＮくん")) return tfp;
        }
    }
    return NULL;
}


BOOL func_init(FILTER* fp) {
    FILTER* exeditfp = get_exeditfp(fp);
    if (exeditfp == NULL) {
        MessageBoxA(fp->hwnd, "拡張編集0.92が見つかりませんでした", fp->name, MB_OK);
        return TRUE;
    }
    exedit_base = (int)exeditfp->dll_hinst;

    return TRUE;
}

struct layerinfo {
    int flag;
    char* name;
};

void filter_change_param(void* editp, FILTER* fp) {
    if (!fp->exfunc->is_editing(editp)) {
        return;
    }
    int current_scene = *(int*)(exedit_base + 0x1a5310);
    if (current_scene == fp->track[0]) {
        return;
    }
    char str[64];

    if (fp->check[0]) {
        if (fp->track[0] == 0) {
            wsprintfA(str, "現在のシーンのレイヤー名を Root にも適応しますか?");
        } else {
            wsprintfA(str, "現在のシーンのレイヤー名を Scene %d にも適応しますか?", fp->track[0]);
        }
        if (IDYES == MessageBoxA(fp->hwnd, str, fp->name, MB_YESNO)) {
            layerinfo* layer = (layerinfo*)(exedit_base + 0x188498);
            layerinfo* scene_c = layer + 100 * current_scene;
            layerinfo* scene_p = layer + 100 * fp->track[0];
            for (int i = 0; i < 100; i++) {
                scene_p[i].name = scene_c[i].name;
            }
        }
    }

    if (fp->check[1]) {
        if (fp->track[0] == 0) {
            wsprintfA(str, "現在のシーンのレイヤー情報を Root にも適応しますか?");
        } else {
            wsprintfA(str, "現在のシーンのレイヤー情報を Scene %d にも適応しますか?", fp->track[0]);
        }
        if (IDYES == MessageBoxA(fp->hwnd, str, fp->name, MB_YESNO)) {
            layerinfo* layer = (layerinfo*)(exedit_base + 0x188498);
            layerinfo* scene_c = layer + 100 * current_scene;
            layerinfo* scene_p = layer + 100 * fp->track[0];
            for (int i = 0; i < 100; i++) {
                scene_p[i].flag = scene_c[i].flag;
            }
        }
    }
}


BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void* editp, FILTER* fp) {
    if (message == WM_FILTER_CHANGE_PARAM) {
        filter_change_param(editp, fp);
        fp->check[0] = fp->check[1] = 0;
        fp->exfunc->filter_window_update(fp);
    }
    return FALSE;
}

